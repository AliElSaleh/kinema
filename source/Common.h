#pragma once

#include <cstdint>
#include <cstdio>
#include <cassert>

#define KINEMA_LOG(category, format, args) \
{ \
	std::printf(category); \
	std::printf(format, args); \
	std::printf("\n"); \
} \

namespace Log
{
	template <typename... Args>
	inline void Print(char* format, Args... args)
	{
		std::printf(format, args...);
	}

	template <typename... Args>
	inline void Info(char* format, Args... args)
	{
		KINEMA_LOG("[INFO] ", format, args...);
	}

	template <typename... Args>
	inline void Warning(char* format, Args... args)
	{
		KINEMA_LOG("[WARNING] ", format, args...);
	}

	template <typename... Args>
	inline void Error(char* format, Args... args)
	{
		KINEMA_LOG("[ERROR] ", format, args...);
	}
};
