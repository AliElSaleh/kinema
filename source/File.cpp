#include "File.h"

#include <fstream>

std::vector<uint8_t> ReadAll(std::ifstream& file)
{
	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<uint8_t> buffer(fileSize);

	file.seekg(0);
	file.read((char*)(buffer.data()), fileSize); // TODO: static cast/reinterpret cast?

	file.close();

	return buffer;
}

std::vector<char> File::ReadAllText(const char* fileName)
{
	std::ifstream file(fileName, std::ios::ate);
	return *(std::vector<char>*)(&ReadAll(file)); //TODO: inline?
}

std::vector<uint8_t> File::ReadAllBytes(const char* fileName)
{
	std::ifstream file(fileName, std::ios::ate | std::ios::binary);
	return ReadAll(file); // TODO: inline?
	//   std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {}); // this?
}