/*
 *  KCLCommons.cpp
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
#include "KCLCommons.h"

ClassLoaderData* AllocCLD(size_t size) {
	auto cld = new ClassLoaderData{ 0 };
	cld->addr = malloc(size);
	cld->size = size;
	return cld;
}

void releaseCLD(ClassLoaderData* data) {
	delete[] data->addr;
	delete data;
}
