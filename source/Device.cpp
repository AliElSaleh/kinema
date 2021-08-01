#include "Device.h"

#include "GL/glew.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"



#include <iostream>

Device::Device()
{
	glGenVertexArrays(1, &VertexArrayObject);

	// No default VAO in core profile
	glBindVertexArray(VertexArrayObject);

	CurrentAttributeCount = 0;
}

void Device::Draw(VertexBuffer* vertexBuffer, uint32_t count)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->BufferObject);

	SetupVertexAttributes(vertexBuffer->Attributes);

	glDrawArrays(GL_TRIANGLES, 0, count);
}

void Device::Draw(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, uint32_t count)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->BufferObject);

	SetupVertexAttributes(vertexBuffer->Attributes);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->BufferObject);
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

void Device::SetupVertexAttributes(const std::vector<VertexAttribute>& attributes)
{
	uint32_t attributeNumber = 0;
	for (const VertexAttribute& attribute : attributes)
	{
		glVertexAttribPointer(
			attributeNumber,
			attribute.Count,
			GL_FLOAT,
			GL_FALSE,
			attribute.Stride,
			(void*)uintptr_t(attribute.Offset));

		glEnableVertexAttribArray(attributeNumber);
		std::cout << attributeNumber << " enabled!\n";

		attributeNumber++;
	}

	for (size_t i = attributes.size(); i < CurrentAttributeCount; i++)
	{
		glDisableVertexAttribArray(i);
		std::cout << i << " disabled!\n";
	}

	CurrentAttributeCount = attributes.size();
}