#include "VoxelMesh.h"

#include <fstream>

#include "SDL2/SDL.h"
#include "glm/gtc/matrix_transform.hpp"

#include "MemoryStream.h"

Block& VoxelMesh::GetBlock(int32_t x, int32_t y, int32_t z)
{
	if (x >= Size.x || y >= Size.y || z >= Size.z ||
		x < 0 || y < 0 || z < 0)
	{
		//assert(false);
		return Block::Default;
	}

	uint32_t index = x + y * Size.x + z * Size.x * Size.y;
	return Blocks[index];
}

Block& VoxelMesh::GetBlock(const glm::ivec3& coordinates)
{
	return GetBlock(coordinates.x, coordinates.y, coordinates.z);
}

//glm::ivec3 colorPalette[] = {
//	glm::ivec3(255, 0, 0),
//	glm::ivec3(63, 55, 55),
//	glm::ivec3(33, 33, 38),
//	glm::ivec3(255, 255, 0),
//	glm::ivec3(181, 161, 159),
//	glm::ivec3(64, 0, 0),
//	glm::ivec3(137, 131, 97),
//	glm::ivec3(255, 255, 255)
//};

void VoxelMesh::GenerateWave(int x, int y, int z)
{
	Size = glm::ivec3(x, y, z);

	uint64_t sizenew = (size_t)x * (size_t)y * (size_t)z;

	Blocks.resize(sizenew);
	for (int xp = 0; xp < x; xp++)
	{
		for (int yp = 0; yp < y; yp++)
		{
			for (int zp = 0; zp < z; zp++)
			{
				Block nb;
				nb.Active = true;
				//nb.Color = glm::ivec3((float)xp / (float)x * 255.0f,
				//	(float)yp / (float)y * 255.0f,
				//	(float)zp / (float)z * 255.0f);
				//nb.Color = glm::ivec3(255,
				//	(float)yp / (float)y * 255.0f,
				//	255);
				int type = (yp / y) * 255;
				if (type > 255) type = 255;
				else if (type < 0) type = 0;
				nb.Type = type;

				float lol = (sin((float)xp / (float)x * 8 * 3.14159) + 1) / 2;
				lol *= (float)y;

				if (yp > (int)lol)
					nb.Active = false;

				//nb.Color = IntVector(128, 48, 48);

				GetBlock(xp, yp, zp) = nb;
			}
		}
	}
}

void VoxelMesh::InitChunks()
{
	ChunkSize = glm::ivec3(32, 32, 32);

	int32_t ChunksWidth = ceilf((float)Size.x / (float)ChunkSize.x);
	int32_t ChunksHeight = ceilf((float)Size.y / (float)ChunkSize.y);
	int32_t ChunksDepth = ceilf((float)Size.z / (float)ChunkSize.z);

	ChunkDims = glm::ivec3(ChunksWidth, ChunksHeight, ChunksDepth);

	Chunks.resize(ChunksWidth * ChunksHeight * ChunksDepth);

	for (VoxelChunk& Chunk : Chunks)
	{
		Chunk.Blocks.resize(ChunkSize.x * ChunkSize.y * ChunkSize.z);
		Chunk.Dimensions = ChunkSize;
	}

	for (int32_t x = 0; x < Size.x; x++)
	{
		for (int32_t y = 0; y < Size.y; y++)
		{
			for (int32_t z = 0; z < Size.z; z++)
			{
				GetBlock_Chunked(x, y, z) = GetBlock(x, y, z);
				GetChunk(x, y, z).Dimensions = ChunkSize;

				int32_t ChunkX = x / ChunkSize.x;
				int32_t ChunkY = y / ChunkSize.y;
				int32_t ChunkZ = z / ChunkSize.z;
				GetChunk(x, y, z).loc = glm::ivec3(ChunkX, ChunkY, ChunkZ);
			}
		}
	}
}

uint32_t indexget(int x, int y, int z, glm::ivec3 Size)
{
	uint32_t index = x + y * Size.x + z * Size.x * Size.y;
	return index;
}
void VoxelMesh::LoadFromFile(const char* fileName)
{
	std::ifstream file(fileName, std::ios::binary);

	file.read((char*)&Size, sizeof(glm::ivec3));
	Size = glm::ivec3(Size.x, Size.y, Size.z);

	uint32_t sizecubed = Size.x * Size.y * Size.z;

	std::vector<uint8_t> blockdata;
	blockdata.resize(sizecubed);
	Blocks.resize(sizecubed);

	file.read((char*)blockdata.data(), sizecubed);

	std::cout << "Map size " << Size.x << ", " << Size.y << ", " << Size.z << "\n";

	// convert yup zup
	for (int32_t x = 0; x < Size.x; x++)
	{
		for (int32_t y = 0; y < Size.y; y++)
		{
			for (int32_t z = 0; z < Size.z; z++)
			{
				glm::ivec3 sizeconv(Size.x, Size.z, Size.y);
				uint32_t index2 = indexget(x, z, y, sizeconv);

				uint32_t index = indexget(x, y, z, Size);

				uint8_t type = blockdata[index2];
				Blocks[index].Active = type > 0;

				if (type > 7)
					Blocks[index].Type = 9;
				else
					Blocks[index].Type = type;
				//if (type > 0)
				//{
				//	if (type > 7)
				//	{
				//		Blocks[index].Color = glm::ivec3(81, 124, 0);
				//	}
				//	else
				//	{
				//		Blocks[index].Color = colorPalette[type];
				//	}
				//}
			}
		}
	}

	Block endblock;
	endblock.Type = 8;
	endblock.Active = true;

	Blocks[indexget(0, 0, 0, Size)] = endblock;
	Blocks[indexget(Size.x - 1, 0, 0, Size)] = endblock;
	Blocks[indexget(0, Size.y - 1, 0, Size)] = endblock;
	Blocks[indexget(0, 0, Size.z - 1, Size)] = endblock;
	Blocks[indexget(Size.x - 1, 0, Size.z - 1, Size)] = endblock;
	Blocks[indexget(Size.x - 1, Size.y - 1, Size.z - 1, Size)] = endblock;
	Blocks[indexget(Size.x - 1, Size.y - 1, 0, Size)] = endblock;
	Blocks[indexget(0, Size.y - 1, Size.z - 1, Size)] = endblock;
}

void VoxelMesh::GenChunksGreedy(int cnumthreads)
{
	//for (VoxelChunk& Chunk : Chunks)
	//{
	//	Chunk.Update();
	//}

	// the easy way

	starttime = SDL_GetTicks();

	generating = true;

	const int numthreads = cnumthreads;

	threads.clear();
	threads.resize(numthreads);

	int chunksalloc = 0;
	int totalchunks = Chunks.size();

	for (int i = 0; i < numthreads; i++)
	{
		std::vector<VoxelChunk*> tchunks;

		int totaltocopy = totalchunks / numthreads;

		if (i == numthreads - 1)
		{
			totaltocopy += totalchunks % numthreads;
		}

		tchunks.resize(totaltocopy);
		for (int z = 0; z < totaltocopy; z++)
		{
			tchunks[z] = &Chunks[chunksalloc];
			chunksalloc++;
		}

		threads[i] = new ChunkGenThreadObj();
		threads[i]->start(tchunks);
	}
}



void VoxelMesh::CheckThreads()
{
	if (!generating)
		return;

	for (auto thread : threads)
	{
		if (!thread->finished)
		{
			return;
		}
	}

	for (auto thread : threads)
	{
		thread->ourthread.join();
	}

	std::cout << Chunks.size() << " generated\n";

	generating = false;
	endtime = SDL_GetTicks();

	lasttime = endtime - starttime;
}

void VoxelMesh::UploadAllChunks()
{
	for (VoxelChunk& Chunk : Chunks)
	{
		Chunk.Update_Upload();
	}
	std::cout << "All chunks uploaded\n";
}

void VoxelMesh::RenderChunks(Device* device, Shader* shader)
{
	for (VoxelChunk& Chunk : Chunks)
	{
		if (!Chunk.ready)
			continue;

		if (Chunk.needsupdate)
		{
			Chunk.Update_Upload();
		}

		if (!Chunk.VB)
		{
			Chunk.Update_Upload();
		}

		glm::mat4 voxmat(1.0f);

		voxmat *= maptransform;
		voxmat = glm::translate(voxmat, (glm::vec3)((glm::vec3)Chunk.loc * (glm::vec3)ChunkSize * BLOCK_SIZE));

		//voxmat = glm::translate(voxmat, glm::vec3(0, 0, 32.0f));

		shader->SetMatrix("model", voxmat);

		device->Draw(Chunk.VB, Chunk.IB, Chunk.temp_indices_count);
	}
}


VoxelChunk& VoxelMesh::GetChunk(int32_t x, int32_t y, int32_t z)
{
	int32_t ChunkX = x / ChunkSize.x;
	int32_t ChunkY = y / ChunkSize.y;
	int32_t ChunkZ = z / ChunkSize.z;

	int32_t chunkIndex = ChunkX + ChunkY * ChunkDims.x + ChunkZ * ChunkDims.x * ChunkDims.y;
	return Chunks[chunkIndex];
}

Block fakeblock; // TODO: lol
Block& VoxelMesh::GetBlock_Chunked(int32_t x, int32_t y, int32_t z)
{
	if (x >= Size.x || y >= Size.y || z >= Size.z ||
		x < 0 || y < 0 || z < 0)
	{
		return fakeblock;
		//return Block::Default;
	}

	int32_t ChunkX = x / ChunkSize.x;
	int32_t ChunkY = y / ChunkSize.y;
	int32_t ChunkZ = z / ChunkSize.z;

	int32_t BlockX = x % ChunkSize.x;
	int32_t BlockY = y % ChunkSize.y;
	int32_t BlockZ = z % ChunkSize.z;

	int32_t chunkIndex = ChunkX + ChunkY * ChunkDims.x + ChunkZ * ChunkDims.x * ChunkDims.y;
	int32_t blockIndex = BlockX + BlockY * ChunkSize.x + BlockZ * ChunkSize.x * ChunkSize.y;

	return Chunks[chunkIndex].Blocks[blockIndex];
}
Block& VoxelMesh::GetBlock_Chunked(glm::ivec3 pos)
{
	return GetBlock_Chunked(pos.x, pos.y, pos.z);
}

void VoxelMesh::makechunkupdateforblock(glm::ivec3 pos)
{
	int x = pos.x;
	int y = pos.y;
	int z = pos.z;

	int32_t ChunkX = x / ChunkSize.x;
	int32_t ChunkY = y / ChunkSize.y;
	int32_t ChunkZ = z / ChunkSize.z;

	int32_t chunkIndex = ChunkX + ChunkY * ChunkDims.x + ChunkZ * ChunkDims.x * ChunkDims.y;

	Chunks[chunkIndex].needsupdate = true;

	Chunks[chunkIndex].Update();

	Chunks[chunkIndex].needsupdate = false;

	std::cout << "Updated chunk for block " << pos.x << " " << pos.y << " " << pos.z << "\n";
}

void VoxelMesh::GenChunksCulled()
{
	std::vector<float> data;

	glm::ivec3 dimensions = Size;

	glm::ivec3 ChunkSize;
	ChunkSize.x = dimensions.x;
	ChunkSize.y = dimensions.y;
	ChunkSize.z = dimensions.z;

	for (int32_t x = 0; x < dimensions.x; x++)
	{
		for (int32_t y = 0; y < dimensions.y; y++)
		{
			for (int32_t z = 0; z < dimensions.z; z++)
			{
				if (!GetBlock(x, y, z).Active)
					continue;

				bool positiveX = true;
				if (x < ChunkSize.x - 1)
					positiveX = !GetBlock(x + 1, y, z).Active;
				bool negativeX = true;
				if (x > 0)
					negativeX = !GetBlock(x - 1, y, z).Active;

				bool positiveY = true;
				if (y < ChunkSize.y - 1)
					positiveY = !GetBlock(x, y + 1, z).Active;
				bool negativeY = true;
				if (y > 0)
					negativeY = !GetBlock(x, y - 1, z).Active;

				bool positiveZ = true;
				if (z < ChunkSize.z - 1)
					positiveZ = !GetBlock(x, y, z + 1).Active;
				bool negativeZ = true;
				if (z > 0)
					negativeZ = !GetBlock(x, y, z - 1).Active;


				// ... proc generate

			}
		}
	}
}

float rem(float value, float modulus) // So temporary
{
	//return value % modulus;
	return fmodf((fmodf(value, modulus) + modulus), modulus);
}
float intbound(float s, float ds)
{
	if (ds < 0)
		return intbound(-s, -ds);

	s = rem(s, 1);
	return (1 - s) / ds;

}

glm::vec3 intbound(glm::vec3 left, glm::vec3 right)
{
	glm::vec3 result;
	result.x = intbound(left.x, right.x);
	result.y = intbound(left.y, right.y);
	result.z = intbound(left.z, right.z);
	return result;
}

bool VoxelMesh::callback(glm::ivec3 copy, glm::ivec3 face, glm::vec3 direction, Block block)
{
	if (GetBlock_Chunked(copy).Active)
	{
		if (block.Active)
			copy += face;

		GetBlock_Chunked(copy) = block;
		makechunkupdateforblock(copy);
		return true;
	}
	return false;
}

void VoxelMesh::Raycast(glm::vec3 position, glm::vec3 direction, float radius, Block block)
{
	glm::vec4 newpos = glm::inverse(maptransform) * glm::vec4(position, 1.0f);
	position = newpos;
	position = position * newpos.w;

	position /= BLOCK_SIZE;

	//direction = glm::inverse(maprot) * glm::vec4(direction, 1.0f);
	direction = glm::inverse(glm::mat3(maptransform)) * direction; // epic


	glm::ivec3 intPosition = glm::floor(position);
	glm::vec3 step = glm::sign(direction);
	glm::vec3 max = intbound(position, direction);
	glm::vec3 delta = step / direction;
	glm::ivec3 face(0);

	if (glm::length(direction) == 0.0f)
	{
		std::cout << "Raycast with 0 length\n";
		return;
	}

	radius /= glm::sqrt(
		direction.x * direction.x +
		direction.y * direction.y +
		direction.z * direction.z);

	while (
		(step.x > 0 ? intPosition.x < Size.x : intPosition.x >= 0) &&
		(step.y > 0 ? intPosition.y < Size.y : intPosition.y >= 0) &&
		(step.z > 0 ? intPosition.z < Size.z : intPosition.z >= 0))
	{
		if (!(intPosition.x < 0 || intPosition.y < 0 || intPosition.z < 0 ||
			intPosition.x >= Size.x || intPosition.y >= Size.y || intPosition.z >= Size.z))
		{
			if (callback(intPosition, face, direction, block))
				return;
		}

		if (max.x < max.y)
		{
			if (max.x < max.z)
			{
				if (max.x > radius)
					break;

				intPosition.x += step.x;
				max.x += delta.x;

				face = glm::ivec3(-step.x, 0, 0);
			}
			else
			{
				if (max.z > radius)
					break;

				intPosition.z += step.z;
				max.z += delta.z;

				face = glm::ivec3(0, 0, -step.z);
			}
		}
		else
		{
			if (max.y < max.z)
			{
				if (max.y > radius)
					break;

				intPosition.y += step.y;
				max.y += delta.y;

				face = glm::ivec3(0, -step.y, 0);
			}
			else
			{
				// Identical to the second case, repeated for simplicity in
				// the conditionals.
				if (max.z > radius)
					break;
				intPosition.z += step.z;
				max.z += delta.z;

				face = glm::ivec3(0, 0, -step.z);
			}
		}
	}
}

void swap(float& left, float& right)
{
	float a = left;
	left = right;
	right = a;
}


//void VoxelMap::tempSave()
//{
//	std::ofstream out("new.vxlm", std::ios::binary);
//
//	MemoryStream mem;
//
//	mem.Write<int>(Size.x);
//	mem.Write<int>(Size.y);
//	mem.Write<int>(Size.z);
//
//	for (int x = 0; x < Size.x; x++)
//	{
//		for (int y = 0; y < Size.y; y++)
//		{
//			for (int z = 0; z < Size.z; z++)
//			{
//				mem.Write<int32_t>((int32_t)GetBlock_Chunked(x, y, z).Active);
//				mem.Write<float>(GetBlock_Chunked(x, y, z).Color.r);
//				mem.Write<float>(GetBlock_Chunked(x, y, z).Color.g);
//				mem.Write<float>(GetBlock_Chunked(x, y, z).Color.b);
//			}
//		}
//	}
//
//	out.write((char*)mem.GetData(), mem.GetSize());
//}
//
//void VoxelMap::tempLoad()
//{
//	std::ifstream in("new.vxlm", std::ios::binary);
//
//	in.seekg(0, std::ios::end);
//	size_t size = in.tellg();
//	in.seekg(0, std::ios::beg);
//	std::vector<uint8_t> bytes(size);
//	in.read((char*)bytes.data(), size);
//
//	MemoryStream mem(bytes);
//
//	Size = glm::vec3(0);
//
//	Size.x = mem.Read<int>();
//	Size.y = mem.Read<int>();
//	Size.z = mem.Read<int>();
//
//	Blocks.resize(Size.x * Size.y * Size.z);
//
//	for (int x = 0; x < Size.x; x++)
//	{
//		for (int y = 0; y < Size.y; y++)
//		{
//			for (int z = 0; z < Size.z; z++)
//			{
//				Block b;
//				b.Active = mem.Read<int32_t>();
//				b.Color.r = mem.Read<float>();
//				b.Color.g = mem.Read<float>();
//				b.Color.b = mem.Read<float>();
//
//				GetBlock(x, y, z) = b;
//			}
//		}
//	}
//
//	InitChunks();
//}