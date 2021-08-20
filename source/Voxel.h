#pragma once

#include <vector>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Device.h"

#include "glm/vec3.hpp"

#include <iostream>
#include <atomic>

#include <thread>

#include "DebugRendering.h"

const float BLOCK_SIZE = 0.1f;

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
	bool ready = false;
	bool needsupdate = false;

	std::vector<Block> Blocks;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> colors;
	std::vector<uint32_t> indices;

	glm::ivec3 Dimensions;
	glm::ivec3 loc;

	VertexBuffer* VB;
	IndexBuffer* IB;

	uint32_t temp_indices_count = 0;

	VoxelChunk();
	~VoxelChunk();

	Block& GetBlockLocal(int32_t x, int32_t y, int32_t z);
	Block& GetBlockLocal(glm::ivec3 coord);

	void Update();
	void Update_Upload();

	void cleardata();

	void DrawChunkBoundary(Device* device, DebugRendering* db, glm::mat4 t);

private:
	inline BlockFace GetBlockFace(const glm::ivec3& inCoordinate, uint8_t side);
};

struct ChunkGenThreadObj
{
	ChunkGenThreadObj()
	{

	}

	std::vector<VoxelChunk*> chunks;
	std::atomic<bool> finished{ false };

	void start(std::vector<VoxelChunk*> chunksz)
	{
		std::cout << "starting thread with " << chunksz.size() << " chunks.\n";
		chunks = chunksz;
		ourthread = std::thread(&ChunkGenThreadObj::GenThesePlease, this);
	}

	void GenThesePlease()
	{
		for (VoxelChunk* chunk : chunks)
		{
			chunk->Update();
		}

		finished = true;
	}

	std::thread ourthread;
};

class VoxelMap
{
public:
	VoxelMap() {}

	DebugRendering* tempdb;

	bool generating = false;
	//std::atomic<bool> generating{ false };

	std::vector<ChunkGenThreadObj*> threads;

	int starttime = 0;
	int endtime = 0;
	int lasttime = 0;

	glm::mat4 maptransform;
	glm::mat4 maprot;

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

	void GenChunksGreedy(int cnumthreads);
	void GenChunksCulled();

	void UploadAllChunks();

	void CheckThreads();

	VoxelChunk& GetChunk(int32_t x, int32_t y, int32_t z);
	Block& GetBlock_Chunked(int32_t x, int32_t y, int32_t z);
	Block& GetBlock_Chunked(glm::ivec3 pos);

	void makechunkupdateforblock(glm::ivec3 pos);

	void RenderChunks(Device* device, Shader* shader);

	bool callback(glm::ivec3 copy, glm::ivec3 face, glm::vec3 direction, Block block);
	void Raycast(glm::vec3 position, glm::vec3 direction, float distance, Block block);
};

class VoxelRenderer
{
};