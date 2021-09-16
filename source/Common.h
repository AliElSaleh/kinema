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
	static std::vector<std::string> LogLines;

	template <typename... Args>
	inline static void CategoryLog(const char* category, const char* format, Args... args)
	{
		std::string message = fmt::format("[{}] {}\n", category, fmt::format(format, args...));
		std::cout << message;
		LogLines.push_back(message);
		// TODO: file out?
	}

public:
	static const std::vector<std::string>& GetLog();

	template <typename... Args>
	inline static void Info(const char* format, Args... args)
	{
		CategoryLog("INFO", format, args...);
	}

	template <typename... Args>
	inline static void Warning(const char* format, Args... args)
	{
		CategoryLog("WARNING", format, args...);
	}

	template <typename... Args>
	inline static void Error(const char* format, Args... args)
	{
		CategoryLog("ERROR", format, args...);
	}
};
