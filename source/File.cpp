#include "File.h"

#include <fstream>

std::vector<char> File::ReadAllBytes(const char* fileName)
{
	std::ifstream file(fileName, std::ios::ate);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}