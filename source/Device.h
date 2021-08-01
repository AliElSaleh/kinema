#pragma once

#include <cstdint>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Device
{
public:
	Device();

	uint32_t VertexArrayObject;

	void Draw(VertexBuffer* vertexBuffer, uint32_t count);
	void Draw(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, uint32_t count);

private:
	size_t CurrentAttributeCount;

	void SetupVertexAttributes(const std::vector<VertexAttribute>& attributes);
};