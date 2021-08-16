#include "Voxel.h"

#include <fstream>

#include <iostream>

#include <cassert>

Color colorPalette[] = {
	Color(255, 0, 0),
	Color(63, 55, 55),
	Color(33, 33, 38),
	Color(255, 255, 0),
	Color(181, 161, 159),
	Color(64, 0, 0),
	Color(137, 131, 97),
	Color(255, 255, 255)
};

void VoxelMap::generate(int x, int y, int z)
{
	Size = IntVector(x, y, z);

	Blocks = std::vector<Block>(x * y * z);
	for (int xp = 0; xp < x; xp++)
	{
		for (int yp = 0; yp < y; yp++)
		{
			for (int zp = 0; zp < z; zp++)
			{
				Block nb;
				nb.Active = true;
				nb.Color = IntVector((float)xp / (float)x * 255.0f,
					(float)yp / (float)y * 255.0f,
					(float)zp / (float)z * 255.0f);
				nb.Color = IntVector(255,
					(float)yp / (float)y * 255.0f,
					255);

				float lol = sin((float)xp / (float)x * 8 * 3.14159);
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

Block& VoxelMap::GetBlock(const IntVector& coordinates)
{
	return GetBlock(coordinates.x, coordinates.y, coordinates.z);
}

uint32_t indexget(int x, int y, int z, IntVector Size)
{
	uint32_t index = x + y * Size.x + z * Size.x * Size.y;
	return index;
}
void VoxelMap::LoadFromFile(const char* fileName)
{
	std::ifstream file(fileName, std::ios::binary);

	file.read((char*)&Size, sizeof(IntVector));
	Size = IntVector(Size.x, Size.y, Size.z);

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
				IntVector sizeconv(Size.x, Size.z, Size.y);
				uint32_t index2 = indexget(x, z, y, sizeconv);

				uint32_t index = indexget(x, y, z, Size);

				uint8_t type = blockdata[index2];
				Blocks[index].Active = type > 0;

				if (type > 0)
				{
					if (type > 7)
					{
						Blocks[index].Color = IntVector(81, 124, 0);
					}
					else
					{
						Blocks[index].Color = colorPalette[type];
					}
				}
			}
		}
	}
	//for (int32_t x = 0; x < sizecubed; x++)
	//{
	//	//uint32_t index = x + y * Size.x + z * Size.x * Size.y;

	//	uint8_t type = blockdata[x];
	//	Blocks[x].Active = type > 0;

	//	if (type > 0)
	//	{
	//		if (type > 7)
	//		{
	//			Blocks[x].Color = IntVector(255, 0, 255);
	//		}
	//		else
	//		{
	//			Blocks[x].Color = colorPalette[type];
	//		}
	//	}
	//}
}

void VoxelRenderer::Init()
{
	//VB = new VertexBuffer(nullptr, nullptr, 
}

static const uint32_t BlockDirectionAxis[6] = { 1, 1, 2, 2, 0, 0 };

static const IntVector BlockForwardDirections[6] =
{
	IntVector(0, 1, 0), // east
	IntVector(0, -1, 0), // west
	IntVector(0, 0, -1), // bottom
	IntVector(0, 0, 1), // top
	IntVector(-1, 0, 0), // south
	IntVector(1, 0, 0) // north
};

typedef glm::vec3 Vector;
static const Vector VertexOffsets[24] =
{
	Vector(0, 1, 0), Vector(1, 1, 0), Vector(1, 1, 1), Vector(0, 1, 1),
	Vector(0, 0, 0), Vector(0, 0, 1), Vector(1, 0, 1), Vector(1, 0, 0),
	Vector(0, 0, 0), Vector(1, 0, 0), Vector(1, 1, 0), Vector(0, 1, 0),
	Vector(0, 0, 1), Vector(0, 1, 1), Vector(1, 1, 1), Vector(1, 0, 1),
	Vector(0, 0, 0), Vector(0, 1, 0), Vector(0, 1, 1), Vector(0, 0, 1),
	Vector(1, 0, 0), Vector(1, 0, 1), Vector(1, 1, 1), Vector(1, 1, 0),
};

static const Vector BlockForwardVectors[6] =
{
	Vector(0, 1, 0),
	Vector(0, -1, 0),
	Vector(0, 0, -1),
	Vector(0, 0, 1),
	Vector(-1, 0, 0),
	Vector(1, 0, 0)
};

IntVector GetAdjacentCoordinate(IntVector x, uint8_t side)
{
	return x + BlockForwardDirections[side];
}

BlockFace VoxelRenderer::GetBlockFace(const IntVector& inCoordinate, uint8_t side) const
{
	Block block = Map->GetBlock(inCoordinate);

	BlockFace face;
	face.Side = side;
	face.Culled = !block.Active;
	face.Color = block.Color;

	if (!face.Culled)
	{
		IntVector adjacentCoordinate = GetAdjacentCoordinate(inCoordinate, side);
		IntVector adjacentBlockType = Map->GetBlock(adjacentCoordinate).Color;

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

	IntVector volumeSize = Map->Size; // tt

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> colors;
	std::vector<uint32_t> indices;

	IntVector blockPosition(0, 0, 0);
	IntVector blockOffset(0, 0, 0);

	BlockFace faceA;
	BlockFace faceB;

	int32_t faceWidth = 0;
	int32_t faceHeight = 0;

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

		blockPosition = IntVector(0, 0, 0);
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
					faceB.Color = IntVector(255, 255, 255);

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
						IntVector blockColor = blockFaceMask[maskIndex].Color;

						

						uint32_t numIndices = indices.size();
						for (int32_t vertexIndex = 0; vertexIndex < 6; ++vertexIndex)
						{
							glm::vec3 vertexPosition = VertexOffsets[blockSide * 4 + triangleIndices[vertexIndex]];

							vertexPosition[uAxis] *= faceWidth;
							vertexPosition[vAxis] *= faceHeight;
							vertexPosition += blockPosition; // TODO::!!!! check

							vertices.push_back(vertexPosition * BLOCK_SIZE);
							normals.push_back(BlockForwardVectors[blockSide]);
							colors.push_back(blockColor);

							indices.push_back(numIndices + vertexIndex);
						}
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

//void VoxelRenderer::UpdateAlg2(VoxelMap* mapINPUT)
//{
//	std::vector<glm::vec3> vertices;
//	std::vector<glm::vec3> normals;
//	std::vector<glm::vec3> colors;
//	std::vector<uint32_t> indices;
//
//	int32_t indoff = 0;
//
//	//int64_t CHUNK_SIZE = mapINPUT->Size.x * mapINPUT->Size.y * mapINPUT->Size.z;
//	//CHUNK_SIZE = 32;
//
//	int32_t chunkPosX = 0;
//	int32_t chunkPosY = 0;
//	int32_t chunkPosZ = 0;
//
//	int32_t chunkSizeX = mapINPUT->Size.x;
//	int32_t chunkSizeY = mapINPUT->Size.y;
//	int32_t chunkSizeZ = mapINPUT->Size.z;
//
//	int32_t CHUNK_SIZE = 512;
//
//	// Sweep over each axis (X, Y and Z)
//	for (int32_t dimension = 0; dimension < 3; ++dimension)
//	{
//		//int32_t CHUNK_SIZE = 0;
//		//switch (dimension)
//		//{
//		//case 0:
//		//	CHUNK_SIZE = chunkSizeX;
//		//	break;
//		//case 1:
//		//	CHUNK_SIZE = chunkSizeZ;
//		//	break;
//		//case 2:
//		//	CHUNK_SIZE = chunkSizeY;
//		//	break;
//		//default:
//		//	std::cout << "panic!\n";
//		//	break;
//		//}
//
//		int i, j, k, l, width, height;
//		int u = (dimension + 1) % 3;
//		int v = (dimension + 2) % 3;
//		int32_t blockPosition[] = { 0, 0, 0 };
//		int32_t q[] = { 0, 0, 0 };
//
//		//bool mask = new bool[CHUNK_SIZE * CHUNK_SIZE];
//		std::vector<bool> mask(CHUNK_SIZE * CHUNK_SIZE);
//		q[dimension] = 1;
//
//		// Check each slice of the chunk one at a time
//		for (blockPosition[dimension] = -1; blockPosition[dimension] < CHUNK_SIZE;)
//		{
//			// Compute the mask
//			int32_t n = 0;
//			for (blockPosition[v] = 0; blockPosition[v] < CHUNK_SIZE; ++blockPosition[v])
//			{
//				for (blockPosition[u] = 0; blockPosition[u] < CHUNK_SIZE; ++blockPosition[u])
//				{
//					// q determines the direction (X, Y or Z) that we are searching
//					// m.IsBlockAt(x,y,z) takes global map positions and returns true if a block exists there
//
//					bool blockCurrent = 0 <= blockPosition[dimension] ? !mapINPUT->GetBlock(blockPosition[0] + chunkPosX, blockPosition[1] + chunkPosY, blockPosition[2] + chunkPosZ).Active : true;
//					bool blockCompare = blockPosition[dimension] < CHUNK_SIZE - 1 ? !mapINPUT->GetBlock(blockPosition[0] + q[0] + chunkPosX, blockPosition[1] + q[1] + chunkPosY, blockPosition[2] + q[2] + chunkPosZ).Active : true;
//
//					// The mask is set to true if there is a visible face between two blocks,
//					//   i.e. both aren't empty and both aren't blocks
//					mask[n++] = blockCurrent != blockCompare;
//				}
//			}
//
//			++blockPosition[dimension];
//
//			n = 0;
//
//			// Generate a mesh from the mask using lexicographic ordering,      
//			//   by looping over each block in this slice of the chunk
//			for (j = 0; j < CHUNK_SIZE; ++j)
//			{
//				for (i = 0; i < CHUNK_SIZE;)
//				{
//					if (mask[n])
//					{
//						// Compute the width of this quad and store it in w                        
//						//   This is done by searching along the current axis until mask[n + w] is false
//						for (width = 1; i + width < CHUNK_SIZE && mask[n + width]; width++) {}
//
//						// Compute the height of this quad and store it in h                        
//						//   This is done by checking if every block next to this row (range 0 to w) is also part of the mask.
//						//   For example, if w is 5 we currently have a quad of dimensions 1 x 5. To reduce triangle count,
//						//   greedy meshing will attempt to expand this quad out to CHUNK_SIZE x 5, but will stop if it reaches a hole in the mask
//
//						bool done = false;
//						for (height = 1; j + height < CHUNK_SIZE; height++)
//						{
//							// Check each block next to this quad
//							for (k = 0; k < width; ++k)
//							{
//								// If there's a hole in the mask, exit
//								if (!mask[n + k + height * CHUNK_SIZE])
//								{
//									done = true;
//									break;
//								}
//							}
//
//							if (done)
//								break;
//						}
//
//						blockPosition[u] = i;
//						blockPosition[v] = j;
//
//						// du and dv determine the size and orientation of this face
//						int32_t du[] = { 0, 0, 0 };
//						du[u] = width;
//
//						int32_t dv[] = { 0, 0, 0 };
//						dv[v] = height;
//
//						// Create a quad for this face. Colour, normal or textures are not stored in this block vertex format.
//						{
//							vertices.push_back(glm::vec3(
//								blockPosition[0], blockPosition[1], blockPosition[2]));
//							vertices.push_back(glm::vec3(
//								blockPosition[0] + du[0], blockPosition[1] + du[1], blockPosition[2] + du[2]));
//							vertices.push_back(glm::vec3(
//								blockPosition[0] + dv[0], blockPosition[1] + dv[1], blockPosition[2] + dv[2]));
//							vertices.push_back(glm::vec3(
//								blockPosition[0] + du[0] + dv[0], blockPosition[1] + du[1] + dv[1], blockPosition[2] + du[2] + dv[2]));
//
//							colors.push_back(glm::vec3(0, 1, 0));
//							colors.push_back(glm::vec3(0, 1, 0));
//							colors.push_back(glm::vec3(0, 1, 0));
//							colors.push_back(glm::vec3(0, 1, 0));
//
//							normals.push_back(glm::vec3(0, 1, 0));
//							normals.push_back(glm::vec3(0, 1, 0));
//							normals.push_back(glm::vec3(0, 1, 0));
//							normals.push_back(glm::vec3(0, 1, 0));
//
//							indices.push_back(0 + indoff);
//							indices.push_back(2 + indoff);
//							indices.push_back(1 + indoff);
//							indices.push_back(1 + indoff);
//							indices.push_back(2 + indoff);
//							indices.push_back(3 + indoff);
//							indoff += 4;
//						}
//
//						// Clear this part of the mask, so we don't add duplicate faces
//						for (l = 0; l < height; ++l)
//							for (k = 0; k < width; ++k)
//								mask[n + k + l * CHUNK_SIZE] = false;
//
//						// Increment counters and continue
//						i += width;
//						n += width;
//					}
//					else
//					{
//						i++;
//						n++;
//					}
//				}
//			}
//		}
//	}
//
//	// finally
//	std::vector<float> vbdata;
//
//	auto pushvector = [](std::vector<float>& data, glm::vec3 vector)
//	{
//		data.push_back(vector.x);
//		data.push_back(vector.y);
//		data.push_back(vector.z);
//	};
//
//	// finally..
//	for (uint32_t blockno = 0; blockno < vertices.size(); blockno++)
//	{
//		pushvector(vbdata, vertices[blockno]);
//		pushvector(vbdata, colors[blockno]);
//		pushvector(vbdata, normals[blockno]);
//	}
//
//	VB = new VertexBuffer(nullptr, vbdata.data(), vbdata.size() * sizeof(float),
//		{
//			{ AttributeType::FLOAT, 3, 9 * sizeof(float), 0 },
//			{ AttributeType::FLOAT, 3, 9 * sizeof(float), 3 * sizeof(float) },
//			{ AttributeType::FLOAT, 3, 9 * sizeof(float), 6 * sizeof(float) }
//		});
//	IB = new IndexBuffer(nullptr, indices.data(), indices.size() * sizeof(uint32_t));
//
//	temp_indices_count = indices.size();
//
//	std::cout << "Generated " << vertices.size() << " vertices with " << indices.size() << " indices\n";
//
//}

void VoxelRenderer::UpdateAlg2(VoxelMap* mapINPUT)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> colors;
	std::vector<uint32_t> indices;

	int32_t indoff = 0;

	//int64_t CHUNK_SIZE = mapINPUT->Size.x * mapINPUT->Size.y * mapINPUT->Size.z;
	//CHUNK_SIZE = 32;

	int32_t chunkPosX = 0;
	int32_t chunkPosY = 0;
	int32_t chunkPosZ = 0;

	int32_t chunkSizeX = mapINPUT->Size.x;
	int32_t chunkSizeY = mapINPUT->Size.y;
	int32_t chunkSizeZ = mapINPUT->Size.z;

	int32_t CHUNK_SIZE = 512;

	// Sweep over each axis (X, Y and Z)
	for (int32_t dimension = 0; dimension < 3; ++dimension)
	{
		//int32_t CHUNK_SIZE = 0;
		//switch (dimension)
		//{
		//case 0:
		//	CHUNK_SIZE = chunkSizeX;
		//	break;
		//case 1:
		//	CHUNK_SIZE = chunkSizeZ;
		//	break;
		//case 2:
		//	CHUNK_SIZE = chunkSizeY;
		//	break;
		//default:
		//	std::cout << "panic!\n";
		//	break;
		//}

		int i, j, k, l, width, height;
		int u = (dimension + 1) % 3;
		int v = (dimension + 2) % 3;
		int32_t blockPosition[] = { 0, 0, 0 };
		int32_t q[] = { 0, 0, 0 };

		//bool mask = new bool[CHUNK_SIZE * CHUNK_SIZE];
		std::vector<bool> mask(CHUNK_SIZE * CHUNK_SIZE);
		q[dimension] = 1;

		// Check each slice of the chunk one at a time
		for (blockPosition[dimension] = -1; blockPosition[dimension] < CHUNK_SIZE;)
		{
			// Compute the mask
			int32_t n = 0;
			for (blockPosition[v] = 0; blockPosition[v] < CHUNK_SIZE; ++blockPosition[v])
			{
				for (blockPosition[u] = 0; blockPosition[u] < CHUNK_SIZE; ++blockPosition[u])
				{
					// q determines the direction (X, Y or Z) that we are searching
					// m.IsBlockAt(x,y,z) takes global map positions and returns true if a block exists there

					bool blockCurrent = 0 <= blockPosition[dimension] ? !mapINPUT->GetBlock(blockPosition[0] + chunkPosX, blockPosition[1] + chunkPosY, blockPosition[2] + chunkPosZ).Active : true;
					bool blockCompare = blockPosition[dimension] < CHUNK_SIZE - 1 ? !mapINPUT->GetBlock(blockPosition[0] + q[0] + chunkPosX, blockPosition[1] + q[1] + chunkPosY, blockPosition[2] + q[2] + chunkPosZ).Active : true;

					// The mask is set to true if there is a visible face between two blocks,
					//   i.e. both aren't empty and both aren't blocks
					mask[n++] = blockCurrent != blockCompare;
				}
			}

			++blockPosition[dimension];

			n = 0;

			// Generate a mesh from the mask using lexicographic ordering,      
			//   by looping over each block in this slice of the chunk
			for (j = 0; j < CHUNK_SIZE; ++j)
			{
				for (i = 0; i < CHUNK_SIZE;)
				{
					if (mask[n])
					{
						// Compute the width of this quad and store it in w                        
						//   This is done by searching along the current axis until mask[n + w] is false
						for (width = 1; i + width < CHUNK_SIZE && mask[n + width]; width++) {}

						// Compute the height of this quad and store it in h                        
						//   This is done by checking if every block next to this row (range 0 to w) is also part of the mask.
						//   For example, if w is 5 we currently have a quad of dimensions 1 x 5. To reduce triangle count,
						//   greedy meshing will attempt to expand this quad out to CHUNK_SIZE x 5, but will stop if it reaches a hole in the mask

						bool done = false;
						for (height = 1; j + height < CHUNK_SIZE; height++)
						{
							// Check each block next to this quad
							for (k = 0; k < width; ++k)
							{
								// If there's a hole in the mask, exit
								if (!mask[n + k + height * CHUNK_SIZE])
								{
									done = true;
									break;
								}
							}

							if (done)
								break;
						}

						blockPosition[u] = i;
						blockPosition[v] = j;

						// du and dv determine the size and orientation of this face
						int32_t du[] = { 0, 0, 0 };
						du[u] = width;

						int32_t dv[] = { 0, 0, 0 };
						dv[v] = height;

						// Create a quad for this face. Colour, normal or textures are not stored in this block vertex format.
						{
							vertices.push_back(glm::vec3(
								blockPosition[0], blockPosition[1], blockPosition[2]));
							vertices.push_back(glm::vec3(
								blockPosition[0] + du[0], blockPosition[1] + du[1], blockPosition[2] + du[2]));
							vertices.push_back(glm::vec3(
								blockPosition[0] + dv[0], blockPosition[1] + dv[1], blockPosition[2] + dv[2]));
							vertices.push_back(glm::vec3(
								blockPosition[0] + du[0] + dv[0], blockPosition[1] + du[1] + dv[1], blockPosition[2] + du[2] + dv[2]));

							colors.push_back(glm::vec3(0, 1, 0));
							colors.push_back(glm::vec3(0, 1, 0));
							colors.push_back(glm::vec3(0, 1, 0));
							colors.push_back(glm::vec3(0, 1, 0));

							normals.push_back(glm::vec3(0, 1, 0));
							normals.push_back(glm::vec3(0, 1, 0));
							normals.push_back(glm::vec3(0, 1, 0));
							normals.push_back(glm::vec3(0, 1, 0));

							indices.push_back(0 + indoff);
							indices.push_back(2 + indoff);
							indices.push_back(1 + indoff);
							indices.push_back(1 + indoff);
							indices.push_back(2 + indoff);
							indices.push_back(3 + indoff);
							indoff += 4;
						}

						// Clear this part of the mask, so we don't add duplicate faces
						for (l = 0; l < height; ++l)
							for (k = 0; k < width; ++k)
								mask[n + k + l * CHUNK_SIZE] = false;

						// Increment counters and continue
						i += width;
						n += width;
					}
					else
					{
						i++;
						n++;
					}
				}
			}
		}
	}

	// finally
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
		pushvector(vbdata, colors[blockno]);
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

void VoxelRenderer::UpdateAlg3(VoxelMap* mapINPUT)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> colors;
	std::vector<uint32_t> indices;

	int32_t indexOffset = 0;

	int32_t chunkPosX = 0;
	int32_t chunkPosY = 0;
	int32_t chunkPosZ = 0;

	glm::uvec3 dims = mapINPUT->Size;

	// Sweep over each axis (X, Y and Z)
	for (int32_t dimension = 0; dimension < 3; ++dimension)
	{
		std::cout << "dimension " << dimension << "\n";

		int i, j, k, l, width, height;
		int u = (dimension + 1) % 3;
		int v = (dimension + 2) % 3;
		glm::ivec3 blockPosition(0, 0, 0);
		glm::ivec3 blockOffset(0, 0, 0); // should be blockDirection? search direction

		// If mask.length ...
		std::vector<bool> mask(dims[u] * dims[v]);

		blockOffset[dimension] = 1;

		// Check each slice of the chunk one at a time
		int blockup = blockPosition[dimension];
		int dimsup = dims[dimension];
		for (blockPosition[dimension] = -1; blockup < dimsup;)
		{
			// Compute the mask
			int32_t maskIndex = 0;
			for (blockPosition[v] = 0; blockPosition[v] < dims[v]; ++blockPosition[v])
			{
				for (blockPosition[u] = 0; blockPosition[u] < dims[u]; ++blockPosition[u])
				{
					// q determines the direction (X, Y or Z) that we are searching
					// m.IsBlockAt(x,y,z) takes global map positions and returns true if a block exists there

					//bool blockCurrent = 0 <= blockPosition[dimension] ? !mapINPUT->GetBlock(blockPosition[0] + chunkPosX, blockPosition[1] + chunkPosY, blockPosition[2] + chunkPosZ).Active : true;
					//bool blockCompare = blockPosition[dimension] < dimensions[dimension] - 1 ? !mapINPUT->GetBlock(blockPosition[0] + q[0] + chunkPosX, blockPosition[1] + q[1] + chunkPosY, blockPosition[2] + q[2] + chunkPosZ).Active : true;

					// The mask is set to true if there is a visible face between two blocks,
					//   i.e. both aren't empty and both aren't blocks
					//mask[n++] = blockCurrent != blockCompare;


					//bool a = 0 <= blockPosition[dimension] ? mapINPUT->GetBlock(blockPosition).Active : 0;
					//bool b = blockPosition[dimension] < dimensions[dimension] - 1 ? mapINPUT->GetBlock(blockPosition + blockOffset).Active : 0;

					//if (a == b)
					//	mask[maskIndex] = 0;
					//else if (a)
					//	mask[maskIndex] = a;
					//else
					//	mask[maskIndex] = -b;

					bool blockCurrent =
						0 <= blockPosition[dimension] ? !mapINPUT->GetBlock(blockPosition).Active : true;
					bool blockCompare =
						blockPosition[dimension] < dims[dimension] - 1 ? !mapINPUT->GetBlock(blockPosition + blockOffset).Active : true;
										
					mask[maskIndex++] = blockCurrent != blockCompare;

					//maskIndex++;
				}
			}

			++blockPosition[dimension];

			maskIndex = 0;

			// Generate a mesh from the mask using lexicographic ordering,      
			//   by looping over each block in this slice of the chunk
			for (j = 0; j < dims[v]; ++j)
			{
				for (i = 0; i < dims[u];)
				{
					if (mask[maskIndex])
					{
						// Compute the width of this quad and store it in w                        
						//   This is done by searching along the current axis until mask[n + w] is false
						for (width = 1; i + width < dims[u] && mask[maskIndex + width]; width++) {}

						// Compute the height of this quad and store it in h                        
						//   This is done by checking if every block next to this row (range 0 to w) is also part of the mask.
						//   For example, if w is 5 we currently have a quad of dimensions 1 x 5. To reduce triangle count,
						//   greedy meshing will attempt to expand this quad out to CHUNK_SIZE x 5, but will stop if it reaches a hole in the mask

						bool done = false;
						for (height = 1; j + height < dims[v]; height++)
						{
							// Check each block next to this quad
							for (k = 0; k < width; ++k)
							{
								// If there's a hole in the mask, exit
								if (!mask[maskIndex + k + height * dims[u]])
								{
									done = true;
									break;
								}
							}

							if (done)
								break;
						}

						blockPosition[u] = i;
						blockPosition[v] = j;

						// du and dv determine the size and orientation of this face
						int32_t du[] = { 0, 0, 0 };
						du[u] = width;

						int32_t dv[] = { 0, 0, 0 };
						dv[v] = height;

						// Create a quad for this face. Colour, normal or textures are not stored in this block vertex format.
						{
							vertices.push_back(glm::vec3(
								blockPosition[0], blockPosition[1], blockPosition[2]));
							vertices.push_back(glm::vec3(
								blockPosition[0] + du[0], blockPosition[1] + du[1], blockPosition[2] + du[2]));
							vertices.push_back(glm::vec3(
								blockPosition[0] + dv[0], blockPosition[1] + dv[1], blockPosition[2] + dv[2]));
							vertices.push_back(glm::vec3(
								blockPosition[0] + du[0] + dv[0], blockPosition[1] + du[1] + dv[1], blockPosition[2] + du[2] + dv[2]));

							colors.push_back(glm::vec3(0, 1, 0));
							colors.push_back(glm::vec3(0, 1, 0));
							colors.push_back(glm::vec3(0, 1, 0));
							colors.push_back(glm::vec3(0, 1, 0));

							normals.push_back(glm::vec3(0, 1, 0));
							normals.push_back(glm::vec3(0, 1, 0));
							normals.push_back(glm::vec3(0, 1, 0));
							normals.push_back(glm::vec3(0, 1, 0));

							indices.push_back(0 + indexOffset);
							indices.push_back(2 + indexOffset);
							indices.push_back(1 + indexOffset);
							indices.push_back(1 + indexOffset);
							indices.push_back(2 + indexOffset);
							indices.push_back(3 + indexOffset);
							indexOffset += 4;
						}

						// Clear this part of the mask, so we don't add duplicate faces
						for (l = 0; l < height; ++l)
							for (k = 0; k < width; ++k)
								mask[maskIndex + k + l * dims[u]] = false;

						// Increment counters and continue
						i += width;
						maskIndex += width;
					}
					else
					{
						i++;
						maskIndex++;
					}
				}
			}
		}
	}

	// finally
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
		pushvector(vbdata, colors[blockno]);
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

	glm::ivec3 dimensions = mapINPUT->Size;

	for (int32_t x = 0; x < dimensions.x; x++)
	{
		for (int32_t y = 0; y < dimensions.y; y++)
		{
			for (int32_t z = 0; z < dimensions.z; z++)
			{
			}
		}
	}
}

void VoxelRenderer::Render(Device* device)
{
	device->Draw(VB, IB, temp_indices_count);
}