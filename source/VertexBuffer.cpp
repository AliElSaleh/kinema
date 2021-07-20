#include "VertexBuffer.h"

#include "GL/glew.h"

VertexBuffer::VertexBuffer(class Engine* device, const float* data, uint32_t size)
{
	glGenVertexArrays(1, &VertexArray);
	glGenBuffers(1, &BufferObject);

	glBindVertexArray(VertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, BufferObject);

	// TODO:		   			  sizeof(CustomVertexType) * size ... or * data.size()
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // TODO: custom vert attrib
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); // TODO: maybe rm
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &BufferObject);
	glDeleteVertexArrays(1, &VertexArray);
}