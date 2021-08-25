#pragma once

#include <iostream>

#include "SDL2/SDL.h"

#include "Camera.h"
#include "Device.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "UniformBuffer.h"
#include "Voxel.h"
#include "VoxelMesh.h"
#include "DebugRenderer.h"
#include <thread>

struct CameraData
{
	glm::mat4 Projection;
	glm::mat4 View;
};

struct templine
{
	glm::vec3 start;
	glm::vec3 end;
	glm::vec3 color;
	int32_t ticksleft;
};


class Engine
{
public:
	Engine();
	~Engine();

	bool Running;

	//t
	Camera camera;

	//glm::vec3 currentPos;
	//glm::vec3 currentFwd;
	//const glm::vec3 camUp = glm::vec3(0, 1, 0);

	float pitch = 0;
	float yaw = 0;


	std::vector<templine> templines;

	std::thread generationThread;

	Device* device;

	Shader* colorShader;
	Shader* litShader;

	VertexBuffer* squareVB;
	IndexBuffer* squareIB;

	VertexBuffer* triangleVB;
	IndexBuffer* triangleIB;

	VertexBuffer* cubeVB;

	UniformBuffer* ub;

	VoxelMap* vm;
	VoxelRenderer vr;

	DebugRenderer* db;

private:
	SDL_Window* Window;

	void Update();
	void Render();
};