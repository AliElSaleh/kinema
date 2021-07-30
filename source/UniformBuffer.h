#pragma once

#include <cstdint>

class UniformBuffer
{
public:
	UniformBuffer(class Engine* device, const void* data, uint32_t size);
	~UniformBuffer();

private:
public:
	uint32_t BufferObject;
};