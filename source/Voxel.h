#pragma once

#include <vector>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Device.h"

#include "glm/vec3.hpp"

typedef glm::ivec3 IntVector;
typedef IntVector Color;

const float BLOCK_SIZE = 1.0f;

struct Block
{
	bool Active;

	IntVector Color;

	Block()
	{
		Active = false;
		Color = IntVector(127);
	}

	static Block Default;
};

struct BlockFace
{
	bool Culled;
	IntVector Color;

	uint8_t Side;

	BlockFace()
	{
		Culled = false;
		Color = IntVector(0, 0, 0);
		Side = 0;
	}

	bool Equals(const BlockFace& other) const
	{
		return other.Culled == Culled && other.Color == Color;
	}
};

class VoxelMap
{
public:
	VoxelMap() {}

	std::vector<Block> Blocks;
	IntVector Size;

	void generate(int x, int y, int z);

	Block& GetBlock(int32_t x, int32_t y, int32_t z);
	Block& GetBlock(const IntVector& coordinates);

	void LoadFromFile(const char* fileName);
};

class VoxelChunk
{
};

class VoxelRenderer
{
public:
	uint32_t temp_indices_count = 0;

	VoxelMap* Map;

	VertexBuffer* VB;
	IndexBuffer* IB;

	BlockFace GetBlockFace(const IntVector& inCoordinate, uint8_t side) const;

	void Init();
	void Update(VoxelMap* mapINPUT);

	void Render(Device* device);
};