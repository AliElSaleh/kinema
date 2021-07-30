#pragma once

#include <cstdint>
#include <vector>

enum class AttributeType : uint32_t
{
	FLOAT
};

struct VertexAttribute
{
	AttributeType Type;
	uint32_t Count;
	uint32_t Stride;
	uint32_t Offset;
};

class VertexBuffer
{
public:
	VertexBuffer(class Engine* device, const float* data, uint32_t size, std::vector<VertexAttribute> attributes);
	~VertexBuffer();

	template <typename T>
	static VertexBuffer* Create(class Engine* device, const T* data, uint32_t size)
	{
		return nullptr;
		//VertexBuffer* vertexBuffer = new VertexBuffer(device, 
	}

private:
public:
	uint32_t BufferObject;
	uint32_t VertexArray;
};