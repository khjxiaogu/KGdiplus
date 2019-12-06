#include "IStreamWrapper.h"

size_t IStreamWrapper::Seek(size_t position)
{
	LARGE_INTEGER sp;
	sp.QuadPart = position;
	ULARGE_INTEGER ep;
	stream->Seek(sp, SEEK_SET,&ep);
	return ep.QuadPart;
}
size_t IStreamWrapper::getPos()
{
	LARGE_INTEGER sp;
	sp.QuadPart = 0;
	ULARGE_INTEGER ep;
	stream->Seek(sp, SEEK_CUR, &ep);
	return ep.QuadPart;
}
size_t IStreamWrapper::getSize()
{
	STATSTG ss;
	stream->Stat(&ss, STATFLAG_NONAME);
	return ss.cbSize.QuadPart;
}

size_t IStreamWrapper::read(char* ptr, size_t count)
{
	ULONG ep;
	stream->Read(ptr, count, &ep);
	return ep;
}

size_t IStreamWrapper::write(char* ptr, size_t count)
{
	ULONG ep;
	stream->Write(ptr, count, &ep);
	return ep;
}
IStream* IStreamWrapper::getStream() {
	return stream;
}