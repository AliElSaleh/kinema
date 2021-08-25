#pragma once

#include <atomic>
#include <vector>
#include <thread>
#include <iostream>

#include "DebugRenderer.h"
#include "VoxelChunk.h"

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

	DebugRenderer* tempdb;

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

	//void tempSave();
	//void tempLoad();

	VoxelChunk& GetChunk(int32_t x, int32_t y, int32_t z);
	Block& GetBlock_Chunked(int32_t x, int32_t y, int32_t z);
	Block& GetBlock_Chunked(glm::ivec3 pos);

	void makechunkupdateforblock(glm::ivec3 pos);

	void RenderChunks(Device* device, Shader* shader);

	bool callback(glm::ivec3 copy, glm::ivec3 face, glm::vec3 direction, Block block);
	void Raycast(glm::vec3 position, glm::vec3 direction, float distance, Block block);
};