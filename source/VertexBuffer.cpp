#include "VertexBuffer.h"

#include "glad/glad.h"

constexpr GLenum GetUsage(BufferUsage hint)
{
	switch (hint)
	{
	default:
	case BufferUsage::Static:
		return GL_STATIC_DRAW;
	case BufferUsage::Dynamic:
		return GL_DYNAMIC_DRAW;
	}
}

VertexBuffer::VertexBuffer(const void* data, uint32_t size, std::vector<VertexAttribute> attributes, BufferUsage hint)
{
	glGenBuffers(1, &BufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, BufferObject);

	glBufferData(GL_ARRAY_BUFFER, size, data, GetUsage(hint));

	Attributes = attributes;
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &BufferObject);
}

void VertexBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
{
	glBindBuffer(GL_ARRAY_BUFFER, BufferObject);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}
