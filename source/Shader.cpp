#include "Shader.h"

#include <iostream>

#include "glad/glad.h"

#include "Engine.h"
#include "UniformBuffer.h"

Shader::Shader()
{
	ShaderProgram = 0;
}

Shader::~Shader()
{
	if (ShaderProgram != 0)
	{
		glDeleteProgram(ShaderProgram);
	}
}

Shader* Shader::FromSource(Engine* device, const char* vertexSource, const char* fragmentSource)
{
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	glCompileShader(vertexShader);

	int32_t vertexResult = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexResult);
	if (vertexResult != GL_TRUE)
	{
		// TODO: err
		int32_t logLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			std::vector<char> log(logLength);
			glGetShaderInfoLog(vertexShader, logLength , nullptr, log.data());

			std::cout << "Vertex shader failed to compile\n";
			std::cout << log.data();
		}
	}

	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShader);

	int32_t fragmentResult = 0;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentResult);
	if (fragmentResult != GL_TRUE)
	{
		// TODO: err
		int32_t logLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			std::vector<char> log(logLength);
			glGetShaderInfoLog(fragmentShader, logLength, nullptr, log.data());

			std::cout << "Fragment shader failed to compile\n";
			std::cout << log.data() << "\n";
		}
	}

	Shader* shader = new Shader();

	shader->ShaderProgram = glCreateProgram();
	glAttachShader(shader->ShaderProgram, vertexShader);
	glAttachShader(shader->ShaderProgram, fragmentShader);
	glLinkProgram(shader->ShaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	int32_t programResult = 0;
	glGetProgramiv(shader->ShaderProgram, GL_LINK_STATUS, &programResult);
	if (programResult != GL_TRUE)
	{
		// TODO: err
		delete shader;
		shader = nullptr;

		return nullptr;
	}

	return shader;
}

void Shader::SetMatrix(const char* name, const glm::mat4& value)
{
	uint32_t location = glGetUniformLocation(ShaderProgram, name);
	if (location != -1) // TODO: err?
	{
		glUseProgram(ShaderProgram);
		glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
	}
	else
	{
		std::cout << name << ": invalid uniform name\n";
		// TODO:
	}
}

void Shader::SetUniformBuffer(const char* name, UniformBuffer* value)
{
	uint32_t location = glGetUniformBlockIndex(ShaderProgram, name);
	if (location != -1)
	{
		glUseProgram(ShaderProgram);
		glUniformBlockBinding(ShaderProgram, location, 0); // TODO: 0 = uniformbuffer id
	}
	else
	{
		std::cout << name << ": invalid uniform name\n";
		// TODO:
	}
}