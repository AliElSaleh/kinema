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
#include "PxPhysicsAPI.h"

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


extern PxShape* shape;

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
};