#pragma once

#include "Common.h"

std::vector<std::string> Log::LogLines;

const std::vector<std::string>& Log::GetLog()
{
	return LogLines;
}