#pragma once

#include "glm/vec3.hpp"

const float BLOCK_SIZE = 0.1f;

struct Block
{
	bool Active;

	uint8_t Type;

	Block()
	{
		Active = false;
		Type = 0;
	}

	static Block Default;
};

struct BlockFace
{
	bool Culled;
	uint8_t Type;

	uint8_t Side;

	BlockFace()
	{
		Culled = false;
		Type = 0;
		Side = 0;
	}

	inline bool Equals(const BlockFace& other) const
	{
		return other.Culled == Culled && other.Type == Type;
	}
};

class VoxelRenderer
{
};