/*
 *  KCLCommons.h
 *  Provides KClassLoader Basic Supports
 *
 *  Copyright (C) 2019-2020 khjxiaogu
 *
 *  Author: khjxiaogu
 *  Web: https://www.khjxiaogu.com
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
*/
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
	//delete data;
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

