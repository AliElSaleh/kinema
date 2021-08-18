#include "IndexBuffer.h"

#include "glad/glad.h"

IndexBuffer::IndexBuffer(class Engine* device, const uint32_t* data, uint32_t size)
{
	glGenBuffers(1, &BufferObject);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	// TODO:
	glDeleteBuffers(1, &BufferObject);
}