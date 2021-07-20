#pragma once

#include <cstdint>

class VertexBuffer
{
public:
	VertexBuffer(class Engine* device, const float* data, uint32_t size);
	~VertexBuffer();

private:
public:
	uint32_t BufferObject;
	uint32_t VertexArray;
};