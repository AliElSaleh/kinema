#include "VoxelChunk.h"

VoxelChunk::VoxelChunk()
{
	VB = nullptr;
	IB = nullptr;

	Dimensions = glm::ivec3(0, 0, 0);
}

VoxelChunk::~VoxelChunk()
{
	delete VB;
	VB = nullptr;

	delete IB;
	IB = nullptr;
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

inline BlockFace VoxelChunk::GetBlockFace(const glm::ivec3& inCoordinate, uint8_t side)
{
	Block block = GetBlockLocal(inCoordinate);

	BlockFace face;
	face.Side = side;
	face.Culled = !block.Active;
	face.Color = block.Color;

	if (!face.Culled)
	{
		glm::ivec3 adjacentCoordinate = GetAdjacentCoordinate(inCoordinate, side);
		glm::ivec3 adjacentBlockType = GetBlockLocal(adjacentCoordinate).Color;

		if (GetBlockLocal(adjacentCoordinate).Active != 0)
		{
			face.Culled = true;
		}
	}

	return face;
}

Block& VoxelChunk::GetBlockLocal(int32_t x, int32_t y, int32_t z)
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

Block& VoxelChunk::GetBlockLocal(glm::ivec3 coord)
{
	return GetBlockLocal(coord.x, coord.y, coord.z);
}

void VoxelChunk::DrawChunkBoundary(Device* device, DebugRenderer* db, glm::mat4 t)
{
	//voxmat = glm::scale(voxmat, glm::vec3(0.1f, 0.1f, 0.1f));
	//voxmat = glm::translate(voxmat, (glm::vec3)(Chunk.loc * ChunkSize));

	//voxmat = glm::translate(voxmat, glm::vec3(0, 0, 32.0f));

	glm::vec3 start = (glm::vec3)loc * (glm::vec3)Dimensions * 0.1f;
	glm::vec3 ext = (glm::vec3)Dimensions * BLOCK_SIZE;

	glm::vec4 newstart(start.x, start.y, start.z, 1.0);
	newstart = t * newstart;

	start = newstart;

	//start += glm::vec3(0, 0, 32.0f * 0.1f);
	//ext *= 0.1f;

	glm::vec3 color(1.0f);
	if (ready)
		color = glm::vec3(0, 1, 0);
	else
		color = glm::vec3(1, 1, 0);

	db->DrawCube(device, start, ext, color);
}

void VoxelChunk::Update()
{
	ready = false;

	if (VB)
	{
		delete VB;
		VB = nullptr;
	}
	if (IB)
	{
		delete IB;
		IB = nullptr;
	}

	glm::ivec3 volumeSize = Dimensions;

	vertices.clear();
	normals.clear();
	colors.clear();
	indices.clear();

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

	ready = true;
	//std::cout << "Generated " << vertices.size() << " vertices with " << indices.size() << " indices\n";
}

void VoxelChunk::cleardata()
{
	ready = false;

	if (VB)
	{
		delete VB;
		VB = nullptr;
	}
	if (IB)
	{
		delete IB;
		IB = nullptr;
	}
}

void VoxelChunk::Update_Upload()
{
	if (VB)
	{
		delete VB;
		VB = nullptr;
	}
	if (IB)
	{
		delete IB;
		IB = nullptr;
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

	VB = new VertexBuffer(vbdata.data(), vbdata.size() * sizeof(float),
		{
			{ AttributeType::FLOAT, 3, 9 * sizeof(float), 0 },
			{ AttributeType::FLOAT, 3, 9 * sizeof(float), 3 * sizeof(float) },
			{ AttributeType::FLOAT, 3, 9 * sizeof(float), 6 * sizeof(float) }
		}, BufferUsage::Dynamic);
	IB = new IndexBuffer(indices.data(), indices.size() * sizeof(uint32_t), BufferUsage::Dynamic);

	temp_indices_count = indices.size();
}




