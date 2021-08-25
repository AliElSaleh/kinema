#pragma once

#include "glm/vec3.hpp"

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

class VoxelRenderer
{
};