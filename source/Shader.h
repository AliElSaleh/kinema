#pragma once

#include <cstdint>
#include <vector>

class Shader
{
public:
	Shader();
	~Shader();

	static Shader* FromSource(class Engine* device, const char* vertexSource, const char* fragmentSource);

private:
public:
	uint32_t ShaderProgram;
};