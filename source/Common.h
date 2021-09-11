#pragma once

#include <cstdint>
#include <cstdio>
#include <cassert>
#include <string>
#include <vector>

#include "fmt/format.h"

class Log
{
private:
	template <typename... Args>
	inline static void CategoryLog(char* category, char* format, Args... args)
	{
		std::string message = fmt::format("[{}] {}\n", category, fmt::format(format, args...));
		std::cout << message;
		Lines.push_back(message);
	}

public:
	static std::vector<std::string> Lines; // make const get

	template <typename... Args>
	inline static void Print(char* format, Args... args)
	{
		std::printf(format, args...);
	}

	template <typename... Args>
	inline static void Info(char* format, Args... args)
	{
		CategoryLog("INFO", format, args...);
	}

	template <typename... Args>
	inline static void Warning(char* format, Args... args)
	{
		CategoryLog("WARNING", format, args...);
	}

	template <typename... Args>
	inline static void Error(char* format, Args... args)
	{
		CategoryLog("ERROR", format, args...);
	}
};
