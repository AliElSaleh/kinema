#include "File.h"

#include <fstream>

std::vector<char> ReadAll(std::ifstream& file)
{
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

std::vector<char> File::ReadAllText(const char* fileName)
{
	std::ifstream file(fileName, std::ios::ate);
	return ReadAll(file); //TODO: inline?
}

std::vector<char> File::ReadAllBytes(const char* fileName)
{
	std::ifstream file(fileName, std::ios::ate | std::ios::binary);
	return ReadAll(file); // TODO: inline?
	//   std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {}); // this?
}