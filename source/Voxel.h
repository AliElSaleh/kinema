#pragma once

#include <vector>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Device.h"

#include "glm/vec3.hpp"

#include <iostream>

//typedef glm::ivec3 IntVector;
//typedef IntVector Color;

struct IntVector
{
	int32_t X;
	int32_t Y;
	int32_t Z;

	IntVector(int32_t x, int32_t y, int32_t z) : X(x), Y(y), Z(z) {}
	IntVector(int32_t value) : IntVector(value, value, value) {}
	IntVector() : IntVector(0) {}

	int32_t& operator[](int32_t index)
	{
		switch (index)
		{
		case 0:
			return X;
		case 1:
			return Y;
		case 2:
			return Z;
		default:
			std::cout << "OOB! intvector\n"; // TODO: err
			return X;
		}
	}

	inline glm::vec3 v3() const
	{
		return glm::vec3(X, Y, Z);
	}

};

inline bool operator==(const IntVector& left, const IntVector& right)
{
	return (left.X == right.X && left.Y == right.Y && left.Z == right.Z);
}

inline IntVector operator+(const IntVector& left, const IntVector& right)
{
	return IntVector(left.X + right.X, left.Y + right.Y, left.Z + right.Z);
}


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

	inline bool Equals(const BlockFace& other) const
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

	inline BlockFace GetBlockFace(const IntVector& inCoordinate, uint8_t side) const;

	void Init();
	void Update(VoxelMap* mapINPUT); // 3d texture for colors? (merges more faces)
	void UpdateCulled(VoxelMap* mapINPUT);

	void Render(Device* device);
};