#pragma once

#include <vector>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Device.h"

#include "glm/vec3.hpp"

#include <iostream>

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
	~VoxelChunk();

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

	Block& GetBlock(int32_t x, int32_t y, int32_t z);
	Block& GetBlock(const glm::ivec3& coordinates);

	void LoadFromFile(const char* fileName);
	void GenerateWave(int x, int y, int z);
	void InitChunks();

	void GenChunksGreedy();
	void GenChunksCulled();

	VoxelChunk& GetChunk(int32_t x, int32_t y, int32_t z);
	Block& GetBlock_Chunked(int32_t x, int32_t y, int32_t z);

	void RenderChunks(Device* device, Shader* shader);
};

class VoxelRenderer
{
};