#pragma once

#include <vector>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Device.h"

#include "glm/vec3.hpp"

#include <iostream>

//typedef glm::ivec3 IntVector;
//typedef IntVector Color;

//struct IntVector
//{
//	int32_t X;
//	int32_t Y;
//	int32_t Z;
//
//	IntVector(int32_t x, int32_t y, int32_t z) : X(x), Y(y), Z(z) {}
//	IntVector(int32_t value) : IntVector(value, value, value) {}
//	IntVector() : IntVector(0) {}
//
//	int32_t& operator[](int32_t index)
//	{
//		switch (index)
//		{
//		case 0:
//			return X;
//		case 1:
//			return Y;
//		case 2:
//			return Z;
//		default:
//			std::cout << "OOB! intvector\n"; // TODO: err
//			return X;
//		}
//	}
//
//	inline glm::vec3 v3() const
//	{
//		return glm::vec3(X, Y, Z);
//	}
//
//};
//
//inline bool operator==(const IntVector& left, const IntVector& right)
//{
//	return (left.X == right.X && left.Y == right.Y && left.Z == right.Z);
//}
//
//inline IntVector operator+(const IntVector& left, const IntVector& right)
//{
//	return IntVector(left.X + right.X, left.Y + right.Y, left.Z + right.Z);
//}


//typedef IntVector Color;

const float BLOCK_SIZE = 1.0f;

struct Block
{
	bool Active;

	glm::ivec3 Color;

	Block()
	{
		Active = false;
		Color = glm::ivec3(127);
	}

	static Block Default;
};

struct BlockFace
{
	bool Culled;
	glm::ivec3 Color;

	uint8_t Side;

	BlockFace()
	{
		Culled = false;
		Color = glm::ivec3(0, 0, 0);
		Side = 0;
	}

	inline bool Equals(const BlockFace& other) const
	{
		return other.Culled == Culled && other.Color == Color;
	}
};

class VoxelChunk
{
public:
	std::vector<Block> Blocks;

	glm::ivec3 Dimensions;
	glm::ivec3 loc;

	VertexBuffer* VB;
	IndexBuffer* IB;

	uint32_t temp_indices_count = 0;

	VoxelChunk();

	Block& GetBlock(int32_t x, int32_t y, int32_t z);
	Block& GetBlock(glm::ivec3 coord);

	void Update();

private:
	inline BlockFace GetBlockFace(const glm::ivec3& inCoordinate, uint8_t side);
};

class VoxelMap
{
public:
	VoxelMap() {}

	std::vector<VoxelChunk> Chunks;
	glm::ivec3 ChunkDims;

	std::vector<Block> Blocks;
	glm::ivec3 ChunkSize;
	glm::ivec3 Size;

	void generate(int x, int y, int z);

	Block& GetBlock(int32_t x, int32_t y, int32_t z);
	Block& GetBlock(const glm::ivec3& coordinates);

	void InitChunks();

	void GenChunks();

	VoxelChunk& GetChunk(int32_t x, int32_t y, int32_t z);
	Block& GetBlock_Chunked(int32_t x, int32_t y, int32_t z);

	void RenderChunks(Device* device, Shader* shader);

	void LoadFromFile(const char* fileName);
};

class VoxelRenderer
{
public:
	uint32_t temp_indices_count = 0;

	VoxelMap* Map;

	VertexBuffer* VB;
	IndexBuffer* IB;

	inline BlockFace GetBlockFace(const glm::ivec3& inCoordinate, uint8_t side) const;

	void Init();
	void Update(VoxelMap* mapINPUT); // 3d texture for colors? (merges more faces)
	void UpdateCulled(VoxelMap* mapINPUT);

	void Render(Device* device);
};