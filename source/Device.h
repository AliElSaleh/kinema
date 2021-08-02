#pragma once

#include <cstdint>

#include "SDL2/SDL.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Device
{
public:
	Device(SDL_Window* window);
	~Device();

	void Clear();

	void SetShader(Shader* shader);

	void Draw(VertexBuffer* vertexBuffer, uint32_t count);
	void Draw(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, uint32_t count);

	void CheckErrorTemp();

private:
	SDL_GLContext Context;

	uint32_t VertexArrayObject;
	size_t CurrentAttributeCount;

	void SetupVertexAttributes(const std::vector<VertexAttribute>& attributes);
};