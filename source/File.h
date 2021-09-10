#pragma once

#include <vector>

namespace File
{
	std::vector<char> ReadAllText(const char* fileName);
	std::vector<uint8_t> ReadAllBytes(const char* fileName);
};