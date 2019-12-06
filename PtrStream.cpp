#include "PtrStream.h"

PtrStream::PtrStream(char* ptr, int size, int current,PtrStream*par) {
	this->ptr = ptr;
	this->size = size;
	this->current = current;
	this->refstream = par;
	if (par)
		par->AddRef();
}
PtrStream::PtrStream(PtrStream* par=NULL) {
	if (par) {
		this->ptr = par->ptr;
		this->size = par->size;
		this->current = par->current;
		this->refstream = par;
		par->AddRef();
	}
}
PtrStream::~PtrStream() {
	if (refstream) {
		refstream->Release();
	}else
	if (ptr)
		delete[] ptr;
}

HRESULT __stdcall PtrStream::QueryInterface(REFIID riid, void** ppvObject) {
	if (riid == IID_IStream) {
		*ppvObject = (IStream*)this;
	}
	else if (riid == IID_ISequentialStream) {
		*ppvObject = (ISequentialStream*)this;
	}
	else if (riid == IID_IUnknown) {
		*ppvObject = (IUnknown*)this;
	}
	if (ppvObject) {
		AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;
}

ULONG __stdcall PtrStream::AddRef(void) {
	std::lock_guard<std::mutex> l(reflock);
	return ++refcount;
}

ULONG __stdcall PtrStream::Release(void) {
	if (refcount == 1) {
		delete this;
		return 0;
	}
	std::lock_guard<std::mutex> l(reflock);
	return --refcount;
}

HRESULT __stdcall PtrStream::Read(void* pv, ULONG cb, ULONG* pcbRead) {
	if (cb == 0)
		return S_OK;
	if (cb < size - current) {
		memcpy(pv, ptr + current, *pcbRead = cb);
		current += cb;
		return S_OK;
	}
	else {
		memcpy(pv, ptr + current, *pcbRead = size - current);
		current = size;
		return S_OK;
	}
}

HRESULT __stdcall PtrStream::Write(const void* pv, ULONG cb, ULONG* pcbWritten) {
	if (cb == 0)
		return S_OK;
	if (cb < size - current) {
		memcpy(ptr + current, pv, *pcbWritten = cb);
		current += cb;
		return S_OK;
	}
	else {
		memcpy(ptr + current, pv, *pcbWritten = size - current);
		current = size;
		return S_OK;
	}
}

HRESULT __stdcall PtrStream::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition) {
	if (dwOrigin == STREAM_SEEK_SET) {
		if (dlibMove.QuadPart > 0)
			current = dlibMove.QuadPart;
		else
			current = 0;
	}
	if (dwOrigin == STREAM_SEEK_CUR) {
		if (current + dlibMove.QuadPart > 0)
			current += dlibMove.QuadPart;
		else
			current = 0;
	}
	if (dwOrigin == STREAM_SEEK_END) {
		if (dlibMove.QuadPart < size)
			current = size - dlibMove.QuadPart;
		else
			current = 0;
	}
	if (current > size) {
		current = size;
	}
	plibNewPosition->QuadPart = current;
	return S_OK;
}

HRESULT __stdcall PtrStream::SetSize(ULARGE_INTEGER libNewSize) {
	char* nptr = new char[libNewSize.QuadPart];
	if (libNewSize.QuadPart > size)
		memcpy(nptr, ptr, size);
	else
		memcpy(nptr, ptr, libNewSize.QuadPart);
	delete[] ptr;
	ptr = nptr;
	return S_OK;
}

HRESULT __stdcall PtrStream::CopyTo(IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten) {
	if (cb.QuadPart <= size) {
		ULONG writ;
		pstm->Write(ptr, cb.QuadPart, &writ);
		pcbRead->QuadPart = cb.QuadPart;
		pcbWritten->QuadPart = writ;
	}
	else {
		ULONG writ;
		pstm->Write(ptr, size, &writ);
		pcbRead->QuadPart = size;
		pcbWritten->QuadPart = writ;
	}
	return S_OK;
}

HRESULT __stdcall PtrStream::Commit(DWORD grfCommitFlags) {
	return E_NOTIMPL;
}

HRESULT __stdcall PtrStream::Revert(void) {
	return E_NOTIMPL;
}

HRESULT __stdcall PtrStream::LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) {
	return S_OK;
}

HRESULT __stdcall PtrStream::UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) {
	return S_OK;
}

HRESULT __stdcall PtrStream::Stat(STATSTG* pstatstg, DWORD grfStatFlag) {
	pstatstg->cbSize.QuadPart = size;
	pstatstg->grfLocksSupported = false;
	pstatstg->type = STGTY_LOCKBYTES;
	return S_OK;
}

HRESULT __stdcall PtrStream::Clone(IStream** ppstm) {
	*ppstm = new PtrStream(ptr, size, current,this);
	return S_OK;
}
