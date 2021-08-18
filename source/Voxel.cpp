#include "Voxel.h"

#include <fstream>

#include <iostream>

#include <cassert>

#include "glm/gtc/matrix_transform.hpp"


glm::ivec3 colorPalette[] = {
	glm::ivec3(255, 0, 0),
	glm::ivec3(63, 55, 55),
	glm::ivec3(33, 33, 38),
	glm::ivec3(255, 255, 0),
	glm::ivec3(181, 161, 159),
	glm::ivec3(64, 0, 0),
	glm::ivec3(137, 131, 97),
	glm::ivec3(255, 255, 255)
};

void VoxelMap::generate(int x, int y, int z)
{
	Size = glm::ivec3(x, y, z);

	Blocks = std::vector<Block>(x * y * z);
	for (int xp = 0; xp < x; xp++)
	{
		for (int yp = 0; yp < y; yp++)
		{
			for (int zp = 0; zp < z; zp++)
			{
				Block nb;
				nb.Active = true;
				nb.Color = glm::ivec3((float)xp / (float)x * 255.0f,
					(float)yp / (float)y * 255.0f,
					(float)zp / (float)z * 255.0f);
				nb.Color = glm::ivec3(255,
					(float)yp / (float)y * 255.0f,
					255);

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

Block Block::Default = Block();

Block& VoxelMap::GetBlock(int32_t x, int32_t y, int32_t z)
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

Block& VoxelMap::GetBlock(const glm::ivec3& coordinates)
{
	return GetBlock(coordinates.x, coordinates.y, coordinates.z);
}

uint32_t indexget(int x, int y, int z, glm::ivec3 Size)
{
	uint32_t index = x + y * Size.x + z * Size.x * Size.y;
	return index;
}
void VoxelMap::LoadFromFile(const char* fileName)
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
	//for (size_t i = 0; i < 

	// convert yup zup
	for (int32_t x = 0; x < Size.x; x++)
	{
		for (int32_t y = 0; y < Size.y; y++)
		{
			for (int32_t z = 0; z < Size.z; z++)
			{
				//uint32_t index = x + y * Size.x + z * Size.x * Size.y;
				//convert index
				glm::ivec3 sizeconv(Size.x, Size.z, Size.y);
				uint32_t index2 = indexget(x, z, y, sizeconv);

				uint32_t index = indexget(x, y, z, Size);

				uint8_t type = blockdata[index2];
				Blocks[index].Active = type > 0;

				if (type > 0)
				{
					if (type > 7)
					{
						Blocks[index].Color = glm::ivec3(81, 124, 0);
					}
					else
					{
						Blocks[index].Color = colorPalette[type];
					}
				}
			}
		}
	}
}

void VoxelRenderer::Init()
{
	//VB = new VertexBuffer(nullptr, nullptr, 
}

static const uint32_t BlockDirectionAxis[6] = { 1, 1, 2, 2, 0, 0 };

static const glm::ivec3 BlockForwardDirections[6] =
{
	glm::ivec3(0, 1, 0),
	glm::ivec3(0, -1, 0),
	glm::ivec3(0, 0, -1),
	glm::ivec3(0, 0, 1),
	glm::ivec3(-1, 0, 0),
	glm::ivec3(1, 0, 0)
};

static const glm::vec3 VertexOffsets[24] =
{
	glm::vec3(0, 1, 0), glm::vec3(1, 1, 0), glm::vec3(1, 1, 1), glm::vec3(0, 1, 1),
	glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(1, 0, 1), glm::vec3(1, 0, 0),
	glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 1, 0), glm::vec3(0, 1, 0),
	glm::vec3(0, 0, 1), glm::vec3(0, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 0, 1),
	glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 1), glm::vec3(0, 0, 1),
	glm::vec3(1, 0, 0), glm::vec3(1, 0, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 0),
};

static const glm::vec3 BlockForwardVectors[6] =
{
	glm::vec3(0, 1, 0),
	glm::vec3(0, -1, 0),
	glm::vec3(0, 0, -1),
	glm::vec3(0, 0, 1),
	glm::vec3(-1, 0, 0),
	glm::vec3(1, 0, 0)
};

inline glm::ivec3 GetAdjacentCoordinate(glm::ivec3 x, uint8_t side)
{
	return x + BlockForwardDirections[side];
}

inline BlockFace VoxelRenderer::GetBlockFace(const glm::ivec3& inCoordinate, uint8_t side) const
{
	Block block = Map->GetBlock(inCoordinate);

	BlockFace face;
	face.Side = side;
	face.Culled = !block.Active;
	face.Color = block.Color;

	if (!face.Culled)
	{
		glm::ivec3 adjacentCoordinate = GetAdjacentCoordinate(inCoordinate, side);
		glm::ivec3 adjacentBlockType = Map->GetBlock(adjacentCoordinate).Color;

		if (Map->GetBlock(adjacentCoordinate).Active != 0)
		{
			face.Culled = true;
		}
	}

	return face;
}

void VoxelRenderer::Update(VoxelMap* mapINPUT)
{
	Map = mapINPUT;

	glm::ivec3 volumeSize = Map->Size; // tt

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> colors;
	std::vector<uint32_t> indices;

	glm::ivec3 blockPosition(0, 0, 0);
	glm::ivec3 blockOffset(0, 0, 0);

	BlockFace faceA;
	BlockFace faceB;

	int32_t faceWidth = 0;
	int32_t faceHeight = 0;

	uint32_t indexOffset = 0;

	std::vector<BlockFace> blockFaceMask;
	blockFaceMask.resize(volumeSize.x * volumeSize.y * volumeSize.z);

	int32_t uAxis = 0;
	int32_t vAxis = 0;

	for (int32_t faceSide = 0; faceSide < 6; ++faceSide) // TODO: ++x?
	{
		int32_t axis = BlockDirectionAxis[faceSide];

		// 2 other axis
		uAxis = (axis + 1) % 3;
		vAxis = (axis + 2) % 3;

		blockPosition = glm::ivec3(0, 0, 0);
		blockOffset = BlockForwardDirections[faceSide];

		// loop through the current axis
		for (blockPosition[axis] = 0; blockPosition[axis] < volumeSize[axis]; ++blockPosition[axis])
		{
			int32_t maskIndex = 0;
			bool maskEmpty = true;

			for (blockPosition[vAxis] = 0; blockPosition[vAxis] < volumeSize[vAxis]; ++blockPosition[vAxis])
			{
				for (blockPosition[uAxis] = 0; blockPosition[uAxis] < volumeSize[uAxis]; ++blockPosition[uAxis])
				{
					faceB.Culled = true;
					faceB.Side = faceSide;
					faceB.Color = glm::ivec3(255, 255, 255);

					// face of this block
					faceA = GetBlockFace(blockPosition, faceSide);

					if ((blockPosition[axis] + blockOffset[axis]) < volumeSize[axis])
					{
						// adjacent face on axis
						faceB = GetBlockFace(blockPosition + blockOffset, faceSide);
					}

					if (!faceA.Culled && !faceB.Culled && faceA.Equals(faceB))
					{
						blockFaceMask[maskIndex].Culled = true;
					}
					else
					{
						blockFaceMask[maskIndex] = faceA;

						if (!faceA.Culled)
						{
							// theres a face so mask is not empty
							maskEmpty = false;
						}
					}

					maskIndex++;
				}
			}

			if (maskEmpty)
			{
				// mask has no faces, no point going any further
				continue;
			}

			maskIndex = 0;

			for (int32_t vAxisIndex = 0; vAxisIndex < volumeSize[vAxis]; ++vAxisIndex)
			{
				for (int32_t uAxisIndex = 0; uAxisIndex < volumeSize[uAxis];)
				{
					if (blockFaceMask[maskIndex].Culled)
					{
						uAxisIndex++;
						maskIndex++;

						// if this face doesn't exist then no face is added
						continue;
					}

					// calculate the face width by checking if adjacent face is the same
					for (faceWidth = 1;
						uAxisIndex + faceWidth < volumeSize[uAxis] &&
						!blockFaceMask[maskIndex + faceWidth].Culled &&
						blockFaceMask[maskIndex + faceWidth].Equals(blockFaceMask[maskIndex]);
						faceWidth++);

					// calculate the face height by checking if adjacent face is the same
					bool finished = false;
					for (faceHeight = 1; vAxisIndex + faceHeight < volumeSize[vAxis]; ++faceHeight)
					{
						for (int32_t faceWidthIndex = 0; faceWidthIndex < faceWidth; ++faceWidthIndex)
						{
							const BlockFace& maskFace = blockFaceMask[maskIndex + faceWidthIndex + faceHeight * volumeSize[uAxis]];

							// face doesn't exist or there's a new type of face
							if (maskFace.Culled || !maskFace.Equals(blockFaceMask[maskIndex]))
							{
								// finished, got the face height
								finished = true;
								break;
							}
						}

						if (finished)
						{
							// finished, got the face height
							break;
						}
					}

					if (!blockFaceMask[maskIndex].Culled)
					{
						blockPosition[uAxis] = uAxisIndex;
						blockPosition[vAxis] = vAxisIndex;

						static int triangleIndices[6] = { 0, 1, 2, 2, 3, 0 };

						uint8_t blockSide = blockFaceMask[maskIndex].Side;
						glm::ivec3 blockColor = blockFaceMask[maskIndex].Color;

						for (int32_t vertexIndex = 0; vertexIndex < 4; ++vertexIndex)
						{
							glm::vec3 vertexPosition = VertexOffsets[blockSide * 4 + vertexIndex];

							vertexPosition[uAxis] *= faceWidth;
							vertexPosition[vAxis] *= faceHeight;
							vertexPosition += blockPosition; // TODO::!!!! check

							vertices.push_back(vertexPosition * BLOCK_SIZE);
							normals.push_back(BlockForwardVectors[blockSide]);
							colors.push_back(blockColor);
						}

						indices.push_back(0 + indexOffset);
						indices.push_back(2 + indexOffset);
						indices.push_back(1 + indexOffset);
						indices.push_back(0 + indexOffset);
						indices.push_back(3 + indexOffset);
						indices.push_back(2 + indexOffset);
						indexOffset += 4;
					}

					for (int32_t faceHeightIndex = 0; faceHeightIndex < faceHeight; ++faceHeightIndex)
					{
						for (int32_t faceWidthIndex = 0; faceWidthIndex < faceWidth; ++faceWidthIndex)
						{
							blockFaceMask[maskIndex + faceWidthIndex + faceHeightIndex * volumeSize[uAxis]].Culled = true;
						}
					}

					uAxisIndex += faceWidth;
					maskIndex += faceWidth;

				}
			}
		}
	}

	std::vector<float> vbdata;

	auto pushvector = [](std::vector<float>& data, glm::vec3 vector)
	{
		data.push_back(vector.x);
		data.push_back(vector.y);
		data.push_back(vector.z);
	};

	// finally..
	for (uint32_t blockno = 0; blockno < vertices.size(); blockno++)
	{
		pushvector(vbdata, vertices[blockno]);
		pushvector(vbdata, colors[blockno] / 255.0f);
		pushvector(vbdata, normals[blockno]);
	}

	VB = new VertexBuffer(nullptr, vbdata.data(), vbdata.size() * sizeof(float),
		{
			{ AttributeType::FLOAT, 3, 9 * sizeof(float), 0 },
			{ AttributeType::FLOAT, 3, 9 * sizeof(float), 3 * sizeof(float) },
			{ AttributeType::FLOAT, 3, 9 * sizeof(float), 6 * sizeof(float) }
		});
	IB = new IndexBuffer(nullptr, indices.data(), indices.size() * sizeof(uint32_t));

	temp_indices_count = indices.size();

	std::cout << "Generated " << vertices.size() << " vertices with " << indices.size() << " indices\n";
}

void VoxelRenderer::UpdateCulled(VoxelMap* mapINPUT)
{
	auto pushvector = [](std::vector<float>& data, glm::vec3 vector)
	{
		data.push_back(vector.x);
		data.push_back(vector.y);
		data.push_back(vector.z);
	};

	std::vector<float> data;

	//glm::ivec3 dimensions = mapINPUT->Size;
	glm::ivec3 dimensions = mapINPUT->Size;

	glm::ivec3 ChunkSize;
	ChunkSize.x = dimensions.x;
	ChunkSize.y = dimensions.y;
	ChunkSize.z = dimensions.z;

	// naive implementation?
	for (int32_t x = 0; x < dimensions.x; x++)
	{
		for (int32_t y = 0; y < dimensions.y; y++)
		{
			for (int32_t z = 0; z < dimensions.z; z++)
			{
				if (!mapINPUT->GetBlock(x, y, z).Active)
					continue;

				bool positiveX = true;
				if (x < ChunkSize.x - 1)
					positiveX = !mapINPUT->GetBlock(x + 1, y, z).Active;
				bool negativeX = true;
				if (x > 0)
					negativeX = !mapINPUT->GetBlock(x - 1, y, z).Active;

				bool positiveY = true;
				if (y < ChunkSize.y - 1)
					positiveY = !mapINPUT->GetBlock(x, y + 1, z).Active;
				bool negativeY = true;
				if (y > 0)
					negativeY = !mapINPUT->GetBlock(x, y - 1, z).Active;

				bool positiveZ = true;
				if (z < ChunkSize.z - 1)
					positiveZ = !mapINPUT->GetBlock(x, y, z + 1).Active;
				bool negativeZ = true;
				if (z > 0)
					negativeZ = !mapINPUT->GetBlock(x, y, z - 1).Active;

				
			}
		}
	}
}

VoxelChunk::VoxelChunk()
{
	VB = nullptr;
	IB = nullptr;

	Dimensions = glm::ivec3(0, 0, 0);
}

inline BlockFace VoxelChunk::GetBlockFace(const glm::ivec3& inCoordinate, uint8_t side)
{
	Block block = GetBlock(inCoordinate);

	BlockFace face;
	face.Side = side;
	face.Culled = !block.Active;
	face.Color = block.Color;

	if (!face.Culled)
	{
		glm::ivec3 adjacentCoordinate = GetAdjacentCoordinate(inCoordinate, side);
		glm::ivec3 adjacentBlockType = GetBlock(adjacentCoordinate).Color;

		if (GetBlock(adjacentCoordinate).Active != 0)
		{
			face.Culled = true;
		}
	}

	return face;
}

Block& VoxelChunk::GetBlock(glm::ivec3 coord)
{
	return GetBlock(coord.x, coord.y, coord.z);
}

void VoxelChunk::Update()
{
	glm::ivec3 volumeSize = Dimensions;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> colors;
	std::vector<uint32_t> indices;

	glm::ivec3 blockPosition(0, 0, 0);
	glm::ivec3 blockOffset(0, 0, 0);

	BlockFace faceA;
	BlockFace faceB;

	int32_t faceWidth = 0;
	int32_t faceHeight = 0;

	uint32_t indexOffset = 0;

	std::vector<BlockFace> blockFaceMask;
	blockFaceMask.resize(volumeSize.x * volumeSize.y * volumeSize.z);

	int32_t uAxis = 0;
	int32_t vAxis = 0;

	for (int32_t faceSide = 0; faceSide < 6; ++faceSide) // TODO: ++x?
	{
		int32_t axis = BlockDirectionAxis[faceSide];

		// 2 other axis
		uAxis = (axis + 1) % 3;
		vAxis = (axis + 2) % 3;

		blockPosition = glm::ivec3(0, 0, 0);
		blockOffset = BlockForwardDirections[faceSide];

		// loop through the current axis
		for (blockPosition[axis] = 0; blockPosition[axis] < volumeSize[axis]; ++blockPosition[axis])
		{
			int32_t maskIndex = 0;
			bool maskEmpty = true;

			for (blockPosition[vAxis] = 0; blockPosition[vAxis] < volumeSize[vAxis]; ++blockPosition[vAxis])
			{
				for (blockPosition[uAxis] = 0; blockPosition[uAxis] < volumeSize[uAxis]; ++blockPosition[uAxis])
				{
					faceB.Culled = true;
					faceB.Side = faceSide;
					faceB.Color = glm::ivec3(255, 255, 255);

					// face of this block
					faceA = GetBlockFace(blockPosition, faceSide);

					if ((blockPosition[axis] + blockOffset[axis]) < volumeSize[axis])
					{
						// adjacent face on axis
						faceB = GetBlockFace(blockPosition + blockOffset, faceSide);
					}

					if (!faceA.Culled && !faceB.Culled && faceA.Equals(faceB))
					{
						blockFaceMask[maskIndex].Culled = true;
					}
					else
					{
						blockFaceMask[maskIndex] = faceA;

						if (!faceA.Culled)
						{
							// theres a face so mask is not empty
							maskEmpty = false;
						}
					}

					maskIndex++;
				}
			}

			if (maskEmpty)
			{
				// mask has no faces, no point going any further
				continue;
			}

			maskIndex = 0;

			for (int32_t vAxisIndex = 0; vAxisIndex < volumeSize[vAxis]; ++vAxisIndex)
			{
				for (int32_t uAxisIndex = 0; uAxisIndex < volumeSize[uAxis];)
				{
					if (blockFaceMask[maskIndex].Culled)
					{
						uAxisIndex++;
						maskIndex++;

						// if this face doesn't exist then no face is added
						continue;
					}

					// calculate the face width by checking if adjacent face is the same
					for (faceWidth = 1;
						uAxisIndex + faceWidth < volumeSize[uAxis] &&
						!blockFaceMask[maskIndex + faceWidth].Culled &&
						blockFaceMask[maskIndex + faceWidth].Equals(blockFaceMask[maskIndex]);
						faceWidth++);

					// calculate the face height by checking if adjacent face is the same
					bool finished = false;
					for (faceHeight = 1; vAxisIndex + faceHeight < volumeSize[vAxis]; ++faceHeight)
					{
						for (int32_t faceWidthIndex = 0; faceWidthIndex < faceWidth; ++faceWidthIndex)
						{
							const BlockFace& maskFace = blockFaceMask[maskIndex + faceWidthIndex + faceHeight * volumeSize[uAxis]];

							// face doesn't exist or there's a new type of face
							if (maskFace.Culled || !maskFace.Equals(blockFaceMask[maskIndex]))
							{
								// finished, got the face height
								finished = true;
								break;
							}
						}

						if (finished)
						{
							// finished, got the face height
							break;
						}
					}

					if (!blockFaceMask[maskIndex].Culled)
					{
						blockPosition[uAxis] = uAxisIndex;
						blockPosition[vAxis] = vAxisIndex;

						static int triangleIndices[6] = { 0, 1, 2, 2, 3, 0 };

						uint8_t blockSide = blockFaceMask[maskIndex].Side;
						glm::ivec3 blockColor = blockFaceMask[maskIndex].Color;

						for (int32_t vertexIndex = 0; vertexIndex < 4; ++vertexIndex)
						{
							glm::vec3 vertexPosition = VertexOffsets[blockSide * 4 + vertexIndex];

							vertexPosition[uAxis] *= faceWidth;
							vertexPosition[vAxis] *= faceHeight;
							vertexPosition += blockPosition; // TODO::!!!! check

							vertices.push_back(vertexPosition * BLOCK_SIZE);
							normals.push_back(BlockForwardVectors[blockSide]);
							colors.push_back(blockColor);
						}

						indices.push_back(0 + indexOffset);
						indices.push_back(2 + indexOffset);
						indices.push_back(1 + indexOffset);
						indices.push_back(0 + indexOffset);
						indices.push_back(3 + indexOffset);
						indices.push_back(2 + indexOffset);
						indexOffset += 4;
					}

					for (int32_t faceHeightIndex = 0; faceHeightIndex < faceHeight; ++faceHeightIndex)
					{
						for (int32_t faceWidthIndex = 0; faceWidthIndex < faceWidth; ++faceWidthIndex)
						{
							blockFaceMask[maskIndex + faceWidthIndex + faceHeightIndex * volumeSize[uAxis]].Culled = true;
						}
					}

					uAxisIndex += faceWidth;
					maskIndex += faceWidth;

				}
			}
		}
	}

	std::vector<float> vbdata;

	auto pushvector = [](std::vector<float>& data, glm::vec3 vector)
	{
		data.push_back(vector.x);
		data.push_back(vector.y);
		data.push_back(vector.z);
	};

	// finally..
	for (uint32_t blockno = 0; blockno < vertices.size(); blockno++)
	{
		pushvector(vbdata, vertices[blockno]);
		pushvector(vbdata, colors[blockno] / 255.0f);
		pushvector(vbdata, normals[blockno]);
	}

	VB = new VertexBuffer(nullptr, vbdata.data(), vbdata.size() * sizeof(float),
		{
			{ AttributeType::FLOAT, 3, 9 * sizeof(float), 0 },
			{ AttributeType::FLOAT, 3, 9 * sizeof(float), 3 * sizeof(float) },
			{ AttributeType::FLOAT, 3, 9 * sizeof(float), 6 * sizeof(float) }
		});
	IB = new IndexBuffer(nullptr, indices.data(), indices.size() * sizeof(uint32_t));

	temp_indices_count = indices.size();

	//std::cout << "Generated " << vertices.size() << " vertices with " << indices.size() << " indices\n";
}

Block& VoxelChunk::GetBlock(int32_t x, int32_t y, int32_t z)
{
	if (x >= Dimensions.x || y >= Dimensions.y || z >= Dimensions.z ||
		x < 0 || y < 0 || z < 0)
	{
		//assert(false);
		return Block::Default;
	}

	uint32_t index = x + y * Dimensions.x + z * Dimensions.x * Dimensions.y;
	return Blocks[index];
}

void VoxelMap::InitChunks()
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

void VoxelMap::GenChunks()
{
	for (VoxelChunk& Chunk : Chunks)
	{
		Chunk.Update();
	}
	std::cout << Chunks.size() << " generated\n";
}

void VoxelMap::RenderChunks(Device* device, Shader* shader)
{
	for (VoxelChunk& Chunk : Chunks)
	{
		glm::mat4 voxmat(1.0f);
		voxmat = glm::scale(voxmat, glm::vec3(0.1f, 0.1f, 0.1f));
		voxmat = glm::translate(voxmat, (glm::vec3)(Chunk.loc * ChunkSize));

		voxmat = glm::translate(voxmat, glm::vec3(0, 0, 32.0f));

		shader->SetMatrix("model", voxmat);

		device->Draw(Chunk.VB, Chunk.IB, Chunk.temp_indices_count);
	}
}


VoxelChunk& VoxelMap::GetChunk(int32_t x, int32_t y, int32_t z)
{
	int32_t ChunkX = x / ChunkSize.x;
	int32_t ChunkY = y / ChunkSize.y;
	int32_t ChunkZ = z / ChunkSize.z;

	int32_t chunkIndex = ChunkX + ChunkY * ChunkDims.x + ChunkZ * ChunkDims.x * ChunkDims.y;
	return Chunks[chunkIndex];
}

Block& VoxelMap::GetBlock_Chunked(int32_t x, int32_t y, int32_t z)
{
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

void VoxelRenderer::Render(Device* device)
{
	device->Draw(VB, IB, temp_indices_count);
}