#include "MemoryStream.h"

MemoryStream::MemoryStream() : Position(0) {}

MemoryStream::MemoryStream(uint64_t size) : MemoryStream()
{
	Buffer.resize(size);
}

void MemoryStream::Write(const void* data, std::size_t size)
{
	std::size_t start = Buffer.size();
	Buffer.resize(Buffer.size() + size);
	std::memcpy(&Buffer[start], data, size);
}