#pragma once

#include <iostream>

#include "SDL2/SDL.h"

#include "Camera.h"
#include "Renderer/Device.h"
#include "Renderer/Shader.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/UniformBuffer.h"
#include "Voxel.h"
#include "VoxelMesh.h"
#include "Renderer/DebugRenderer.h"
#include <thread>
#include "PxPhysicsAPI.h"
#include "Input.h"

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

using namespace physx;



extern physx::PxFoundation* foundation;

extern physx::PxPhysics* physics;

extern physx::PxCooking* cooking;

extern physx::PxDefaultCpuDispatcher* dispatcher;

extern physx::PxScene* scene;

extern physx::PxMaterial* material;

extern std::vector<physx::PxRigidActor*> actors;

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

	VertexBuffer* cubeVB;
	IndexBuffer* cubeIB;

	UniformBuffer* cameraUB;
	UniformBuffer* voxelUB;
	UniformBuffer* mengerUB;

	std::vector<VoxelMesh*> meshii;
	VoxelMesh* currentvm = nullptr;

	DebugRenderer* db;

private:
	SDL_Window* Window;

	double frameTime;
	double f;

	void Update();
	void Render();

	void LoadVox(const char* fileName);
};