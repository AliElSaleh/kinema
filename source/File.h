#pragma once

#include <vector>

namespace File
{
	std::vector<char> ReadAllText(const char* fileName);
	std::vector<char> ReadAllBytes(const char* fileName);
};