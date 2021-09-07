#include "Device.h"

#include "glad/glad.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <iostream>

// TODO: VertexAttribFormat 4.3
// TODO: BindProgramPipeline 4.1

Device::Device(SDL_Window* window)
{
	// SDL2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	Context = SDL_GL_CreateContext(window);

	SDL_GL_SetSwapInterval(1);

	// GLAD
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		// bad
		std::cout << "glad fail\n";
		return;
	}

	// TODO: pipeline object?
	glEnable(GL_CULL_FACE);
	//glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);

	float gray = 4.0f / 255.0f;
	glClearColor(gray, gray, gray, 1.0f);

	// No default VAO in core profile
	glGenVertexArrays(1, &VertexArrayObject);
	glBindVertexArray(VertexArrayObject);

	CurrentAttributeCount = 0;

	// temp
	const GLubyte* strver = glGetString(GL_VERSION);
	std::cout << strver << "\n";
}

Device::~Device()
{
	SDL_GL_DeleteContext(Context);
}

void Device::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Device::SetShader(Shader* shader)
{
	glUseProgram(shader->ShaderProgram);
}

void Device::Draw(VertexBuffer* vertexBuffer, uint32_t count)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->BufferObject);

	SetupVertexAttributes(vertexBuffer->Attributes);

	glDrawArrays(GL_TRIANGLES, 0, count);
}

void Device::Draw(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, uint32_t count)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->BufferObject);

	SetupVertexAttributes(vertexBuffer->Attributes);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->BufferObject);
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

void Device::DrawLines(VertexBuffer* vertexBuffer, uint32_t count)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->BufferObject);

	SetupVertexAttributes(vertexBuffer->Attributes);

	glDrawArrays(GL_LINES, 0, count);
}

void Device::SetupVertexAttributes(const std::vector<VertexAttribute>& attributes)
{
	uint32_t attributeNumber = 0;
	for (const VertexAttribute& attribute : attributes)
	{
		GLenum type = GL_FLOAT;
		if (attribute.Type == AttributeType::Float) type = GL_FLOAT;
		else if (attribute.Type == AttributeType::UnsignedInt) type = GL_UNSIGNED_INT;

		if (attribute.Type != AttributeType::UnsignedInt)
		{
			glVertexAttribPointer(
				attributeNumber,
				attribute.Count,
				type,
				GL_FALSE,
				attribute.Stride,
				(void*)uintptr_t(attribute.Offset));
		}
		else
		{
			glVertexAttribIPointer(
				attributeNumber,
				attribute.Count,
				type,
				attribute.Stride,
				(void*)uintptr_t(attribute.Offset));
		}

		glEnableVertexAttribArray(attributeNumber);
		//std::cout << attributeNumber << " enabled!\n";

		attributeNumber++;
	}

	for (size_t i = attributes.size(); i < CurrentAttributeCount; i++)
	{
		glDisableVertexAttribArray(i);
		//std::cout << i << " disabled!\n";
	}

	CurrentAttributeCount = attributes.size();
}




// TODO: common GL check err define
void Device::CheckErrorTemp()
{
	GLenum err = glGetError();
	if (err != 0)
		std::cout << "GLError: " << err << "\n";
}