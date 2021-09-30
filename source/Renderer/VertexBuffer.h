#pragma once

#include <cstdint>
#include <vector>

enum class AttributeType : uint32_t
{
	Float,
	UnsignedInt
};

enum class BufferUsage : uint32_t
{
	Static,
	Dynamic
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
	VertexBuffer(const void* data, uint32_t size, std::vector<VertexAttribute> attributes, BufferUsage hint);
	~VertexBuffer();

	void SetData(const void* data, uint32_t size, uint32_t offset);

private:
public:
	uint32_t BufferObject;
	uint32_t VertexArray;

	std::vector<VertexAttribute> Attributes;
};

//template <typename T>
//static VertexBuffer* Create(class Engine* device, const T* data, uint32_t size)
//{
//	return nullptr;
//	//VertexBuffer* vertexBuffer = new VertexBuffer(device, 
//}