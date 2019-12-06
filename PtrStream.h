#pragma once
#include <objidl.h>
#include <mutex>
class PtrStream :
	public IStream
{
	int refcount=1;
	char* ptr=NULL;
	size_t size = NULL;
	size_t current = NULL;
	PtrStream* refstream = NULL;
	std::mutex reflock;
public:
	PtrStream(char* ptr, int size, int current = 0,PtrStream*par=NULL);
	PtrStream(PtrStream* par);
	~PtrStream();
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);

	ULONG STDMETHODCALLTYPE AddRef(void);

	ULONG STDMETHODCALLTYPE Release(void);
	HRESULT STDMETHODCALLTYPE Read(void* pv, ULONG cb, ULONG* pcbRead);

	HRESULT STDMETHODCALLTYPE Write(const void* pv, ULONG cb, ULONG* pcbWritten);
	HRESULT STDMETHODCALLTYPE Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition);

	HRESULT STDMETHODCALLTYPE SetSize(ULARGE_INTEGER libNewSize);

	HRESULT STDMETHODCALLTYPE CopyTo(IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten);

	HRESULT STDMETHODCALLTYPE Commit(
		/* [in] */ DWORD grfCommitFlags);

	HRESULT STDMETHODCALLTYPE Revert(void);

	HRESULT STDMETHODCALLTYPE LockRegion(
		/* [in] */ ULARGE_INTEGER libOffset,
		/* [in] */ ULARGE_INTEGER cb,
		/* [in] */ DWORD dwLockType);

	HRESULT STDMETHODCALLTYPE UnlockRegion(
		/* [in] */ ULARGE_INTEGER libOffset,
		/* [in] */ ULARGE_INTEGER cb,
		/* [in] */ DWORD dwLockType);

	HRESULT STDMETHODCALLTYPE Stat(STATSTG* pstatstg, DWORD grfStatFlag);

	HRESULT STDMETHODCALLTYPE Clone(
		/* [out] */ IStream** ppstm);
};

