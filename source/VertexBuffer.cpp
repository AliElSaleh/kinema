#include "VertexBuffer.h"

#include "glad/glad.h"

VertexBuffer::VertexBuffer(class Engine* device, const float* data, uint32_t size, std::vector<VertexAttribute> attributes)
{
	glGenBuffers(1, &BufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, BufferObject);

	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	Attributes = attributes;
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &BufferObject);
}

