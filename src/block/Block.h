#pragma once

#include <utility>
#include <string>
#include <glm/vec3.hpp>

#include "../graphics/utility/CascadedShadowMap.h"

// Textures for each side of a block
struct SideTextureIndices
{
	SideTextureIndices(int i) : top(i), front(i), right(i), left(i), back(i), bottom(i) {}
	SideTextureIndices(int top, int side, int bottom) : top(top), front(side), right(side), left(side), back(side), bottom(bottom) {}
	SideTextureIndices(int top, int front, int right, int left, int back, int bottom) : top(top), front(front), right(right), left(left), back(back), bottom(bottom) {}

	unsigned top;
	unsigned front;
	unsigned right;
	unsigned left;
	unsigned back;
	unsigned bottom;
};

// Defines info for each block
struct Block
{
	// This matches order in sprite sheet
	enum BlockType : unsigned int
	{
		BLOCK_ERROR = MAXUINT,

		BLOCK_AIR = 0,
		BLOCK_GRASS,
		BLOCK_DIRT,
		BLOCK_STONE,
		BLOCK_LOG,
		BLOCK_LEAVES,

		BLOCK_COUNT
	};

	BlockType type;
	std::string name;

	bool operator==(const Block &rhs) const
	{
		return type == rhs.type || name == rhs.name;
	}
};

typedef std::pair<glm::ivec3, Block> BlockInfo;

// Block-texture data
namespace BlockData
{
	static const SideTextureIndices sideIndices[Block::BLOCK_COUNT] =
	{
		0,
		{ 2, 1, 0 },
		0,
		3,
		{ 5, 4, 5 },
		6,
	};
}