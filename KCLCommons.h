#pragma once
#include <Windows.h>

struct ClassLoaderData {
	void* addr;
	size_t size;
};
#ifdef USEISTREAM
#include "PtrStream.h"
PtrStream* packStream(ClassLoaderData* data) {
	auto ps= new PtrStream((char*)data->addr, data->size);
	delete data;
	return ps;
}
#endif
typedef ClassLoaderData* (*scriptGetter)(DWORD, long);
ClassLoaderData* AllocCLD(size_t size);
void releaseCLD(ClassLoaderData* data);
class StorageLister {
public:
	virtual void Add(const wchar_t* name) = 0;
};
class ClassLoader {
public:
	virtual bool isExistent(const wchar_t* name) = 0;
	virtual IStream* getIStream(const wchar_t* name) = 0;
	virtual ClassLoaderData* getStream(const wchar_t* name) = 0;
	virtual void listFiles(StorageLister* lister) = 0;
	virtual void AddRef() = 0;
	virtual void Release() = 0;
};

