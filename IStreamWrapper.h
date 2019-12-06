#pragma once
#include <ObjIdl.h>
class IStreamWrapper
{
	IStream* stream;
public:
	IStreamWrapper(IStream* str);
	size_t Seek(size_t position);
	size_t getPos();
	size_t getSize();
	size_t read(char* ptr, size_t count);
	size_t write(char* ptr, size_t count);
	IStream* getStream();
};

