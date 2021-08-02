#pragma once

#include <cstdint>

class UniformBuffer
{
public:
	UniformBuffer(class Engine* device, const void* data, uint32_t size);
	~UniformBuffer();

	void SetData(const void* data, uint32_t offset, uint32_t size);

private:
public:
	uint32_t BufferObject;
	uint32_t AllocatedSize;
};