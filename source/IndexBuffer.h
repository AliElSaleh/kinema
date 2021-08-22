#pragma once

#include <cstdint>

#include "VertexBuffer.h"

class IndexBuffer
{
public:
	IndexBuffer(const uint32_t* data, uint32_t size, BufferUsage usage);
	~IndexBuffer();

private:
public:
	uint32_t BufferObject;

	void SetData(const uint32_t* data, uint32_t size, uint32_t offset);
};