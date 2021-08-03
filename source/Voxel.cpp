#include "Voxel.h"

#include <fstream>

#include <iostream>

struct Color
{
	float R;
	float G;
	float B;

	Color(uint8_t r, uint8_t g, uint8_t b)
	{
		R = r / 255.0f;
		G = g / 255.0f;
		B = b / 255.0f;
	}
};

Color colorPalette[] = {
	Color(255, 0, 0),
	Color(0, 255, 0),
	Color(0, 0, 255),
	Color(255, 255, 0),
	Color(0, 255, 255),
	Color(64, 0, 0),
	Color(0, 0, 0),
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
				//nb.Color = IntVector(255,
				//	(float)yp / (float)y * 255.0f,
				//	255);

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
		return Block::Default;
	}

	uint32_t index = x + y * Size.x + z * Size.x * Size.y;
	return Blocks[index];
}

Block& VoxelMap::GetBlock(const IntVector& coordinates)
{
	return GetBlock(coordinates.x, coordinates.y, coordinates.z);
}

void VoxelMap::LoadFromFile(const char* fileName)
{
	std::ifstream file(fileName, std::ios::binary);

	file.read((char*)&Size, sizeof(IntVector));

	std::cout << "Map size " << Size.x << ", " << Size.y << ", " << Size.z << "\n";
	//for (size_t i = 0; i < 
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
	}

	VB = new VertexBuffer(nullptr, vbdata.data(), vbdata.size() * sizeof(float),
		{
			{ AttributeType::FLOAT, 3, 6 * sizeof(float), 0 },
			{ AttributeType::FLOAT, 3, 6 * sizeof(float), 3 * sizeof(float) }
		});
	IB = new IndexBuffer(nullptr, indices.data(), indices.size() * sizeof(uint32_t));

	temp_indices_count = indices.size();

	std::cout << "Generated " << vertices.size() << " vertices with " << indices.size() << " indices\n";
}

void VoxelRenderer::Render(Device* device)
{
	device->Draw(VB, IB, temp_indices_count);
}