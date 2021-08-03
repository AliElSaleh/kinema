#pragma once

#include <iostream>

#include "SDL2/SDL.h"

#include "Device.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "UniformBuffer.h"
#include "Voxel.h"

struct CameraData
{
	glm::mat4 Projection;
	glm::mat4 View;
};

class Engine
{
public:
	Engine();
	~Engine();

	bool Running;

	//t
	glm::vec3 currentPos;
	glm::vec3 currentFwd;
	const glm::vec3 camUp = glm::vec3(0, 1, 0);

	float pitch = 0;
	float yaw = 0;

	Device* device;

	Shader* colorShader;
	Shader* litShader;

	VertexBuffer* squareVB;
	IndexBuffer* squareIB;

	VertexBuffer* triangleVB;
	IndexBuffer* triangleIB;

	VertexBuffer* cubeVB;

	UniformBuffer* ub;

	VoxelMap vm;
	VoxelRenderer vr;

private:
	SDL_Window* Window;

	void Update();
	void Render();
};