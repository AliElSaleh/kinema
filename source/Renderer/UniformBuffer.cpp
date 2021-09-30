#include "UniformBuffer.h"

#include "glad/glad.h"



#include <iostream>

static int testCount = 0;

UniformBuffer::UniformBuffer(class Engine* device, const void* data, uint32_t size)
{
	glGenBuffers(1, &BufferObject);
	glBindBuffer(GL_UNIFORM_BUFFER, BufferObject);

	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW); // TODO: GL_DYNAMIC_DRAW?
	glBindBufferRange(GL_UNIFORM_BUFFER, testCount, BufferObject, 0, size);

	index = testCount;
	testCount++;

	AllocatedSize = size;

	glBindBuffer(GL_UNIFORM_BUFFER, 0); // TODO: rem
}

void UniformBuffer::SetData(const void* data, uint32_t offset, uint32_t size)
{
	if (size > AllocatedSize)
	{
		// TODO: err
		std::cout << "oob set data\n";
		return;
	}

	glBindBuffer(GL_UNIFORM_BUFFER, BufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);

	glBindBuffer(GL_UNIFORM_BUFFER, 0); // TODO: rem
}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &BufferObject); // TODO: assuming this is correct
}