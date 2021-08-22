#pragma once

#include <vector>

class MemoryStream
{
public:
	MemoryStream();
	MemoryStream(uint64_t size);
	MemoryStream(const std::vector<uint8_t>& data);

	template <typename T>
	T Read()
	{
		T value = *reinterpret_cast<T*>(&Buffer[Position]);
		Position += sizeof(T);

		return value;
	}

	template <typename T>
	void Write(const T& value)
	{
		Write(&value, sizeof(T));
	}

	void Write(const void* data, std::size_t size);

	const uint8_t* GetData() { return Buffer.data(); }
	std::size_t GetSize() { return Buffer.size(); }

private:
	std::vector<uint8_t> Buffer;
	std::size_t Position;
};