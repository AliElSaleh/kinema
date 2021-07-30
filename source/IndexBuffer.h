#pragma once

#include <cstdint>

class IndexBuffer
{
public:
	IndexBuffer(class Engine* device, const uint32_t* data, uint32_t size);
	~IndexBuffer();

private:
public:
	uint32_t BufferObject;
};