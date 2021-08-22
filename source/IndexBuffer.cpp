#include "IndexBuffer.h"

#include "glad/glad.h"

// TODO: generic buffer class?
// TODO: move this into common GL class, this code is repeated in VertexBuffer
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

IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t size, BufferUsage usage)
{
	glGenBuffers(1, &BufferObject);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GetUsage(usage));
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &BufferObject);
}

void IndexBuffer::SetData(const uint32_t* data, uint32_t size, uint32_t offset)
{
	glBindBuffer(GL_ARRAY_BUFFER, BufferObject);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}
