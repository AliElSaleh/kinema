#include "VertexBuffer.h"

#include "GL/glew.h"

VertexBuffer::VertexBuffer(class Engine* device, const float* data, uint32_t size, std::vector<VertexAttribute> attributes)
{
	//glGenVertexArrays(1, &VertexArray);
	glGenBuffers(1, &BufferObject);

	//glBindVertexArray(VertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, BufferObject);

	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	Attributes = attributes;
	//for (int32_t i = 0; i < attributes.size(); i++)
	//{
	//	const VertexAttribute& attribute = attributes[i];

	//	uint32_t type = GL_FLOAT;
	//	switch (attribute.Type)
	//	{
	//		case AttributeType::FLOAT:
	//			type = GL_FLOAT;
	//	}

	//	glVertexAttribPointer(i,
	//		attribute.Count,
	//		type,
	//		GL_FALSE,
	//		attribute.Stride,
	//		(void*)uintptr_t(attribute.Offset));

	//	glEnableVertexAttribArray(i);
	//}

	//glBindVertexArray(0); // TODO: maybe rm
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &BufferObject);
	//glDeleteVertexArrays(1, &VertexArray);
}

