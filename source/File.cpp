#include "File.h"

#include <fstream>

template<typename T>
inline std::vector<T> ReadAll(std::ifstream& file)
{
	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<T> buffer(fileSize);

	file.seekg(0);
	file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

	file.close();

	return buffer;
}

// TODO: would it make more sense to expose the above template directly, with binary and text options?

std::vector<char> File::ReadAllText(const char* fileName)
{
	std::ifstream file(fileName, std::ios::ate);
	return ReadAll<char>(file);
}

std::vector<uint8_t> File::ReadAllBytes(const char* fileName)
{
	std::ifstream file(fileName, std::ios::ate | std::ios::binary);
	return ReadAll<uint8_t>(file);
	// std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {}); // this?
}