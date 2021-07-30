#include "UniformBuffer.h"

#include "GL/glew.h"

UniformBuffer::UniformBuffer(class Engine* device, const void* data, uint32_t size)
{
	glGenBuffers(1, &BufferObject);
	glBindBuffer(GL_UNIFORM_BUFFER, BufferObject);

	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW); // TODO: GL_DYNAMIC_DRAW?
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, BufferObject, 0, size);

	glBindBuffer(GL_UNIFORM_BUFFER, 0); // TODO: rem
}

UniformBuffer::~UniformBuffer()
{
	// TODO:
}