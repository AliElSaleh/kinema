#pragma once

#include <cstdint>
#include <vector>

#include "glm/mat4x4.hpp"

class Shader
{
public:
	Shader();
	~Shader();

	static Shader* FromSource(class Engine* device, const char* vertexSource, const char* fragmentSource);

	void SetMatrix(const char* name, const glm::mat4& value);
	void SetUniformBuffer(const char* name, class UniformBuffer* value);

private:
public:
	uint32_t ShaderProgram;
};