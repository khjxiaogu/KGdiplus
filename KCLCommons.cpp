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
