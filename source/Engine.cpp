#include "Engine.h"


#include "Device.h"
#include "Voxel.h"
#include "File.h"

#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glad/glad.h"

#include <fstream>

#include <thread>

#include "MemoryStream.h"

#include <sstream>


void pvec3(glm::vec3 vec)
{
	std::cout << vec.x << " " << vec.y << " " << vec.z;;
}

float squareVertices[] =
{
	-0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f
};

uint32_t squareIndices[] =
{
	0, 1, 2,
	2, 3, 0
};

const float PI = 3.14159265359f;

// V2 pos V3 color
float triangleVertices[] =
{
	sin((0.0f * PI) / 3.0f) * 0.5f, cos((0.0f * PI) / 3.0f) * 0.5f, 1.0f, 0.0f, 0.0f,
	sin((2.0f * PI) / 3.0f) * 0.5f, cos((2.0f * PI) / 3.0f) * 0.5f,	0.0f, 1.0f, 0.0f,
	sin((4.0f * PI) / 3.0f) * 0.5f, cos((4.0f * PI) / 3.0f) * 0.5f,	0.0f, 0.0f, 1.0f
};

uint32_t triangleIndices[] = { 0, 1, 2 };

float cubeVertices[] = {
	-0.5, -0.5,  0.5, 0, 0, 1,
	 0.5, -0.5,  0.5, 0, 1, 0,
	 0.5,  0.5,  0.5, 0, 1, 1,
	-0.5,  0.5,  0.5, 1, 0, 0,
	// back
	-0.5, -0.5, -0.5, 1, 0, 1,
	 0.5, -0.5, -0.5, 1, 1, 0,
	 0.5,  0.5, -0.5, 1, 1, 1,
	-0.5,  0.5, -0.5, 0, 0, 1
};

uint32_t cubeIndices[] = {
	// front
	0, 1, 2,
	2, 3, 0,
	// right
	1, 5, 6,
	6, 2, 1,
	// back
	7, 6, 5,
	5, 4, 7,
	// left
	4, 0, 3,
	3, 7, 4,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// top
	3, 2, 6,
	6, 7, 3
};

struct teststruct
{
	int a;
	char b[8];
	float c;
	int64_t d;

	void print()
	{
		std::cout << a << " " << b << " " << c << " " << d << "\n";
	}
};

static physx::PxDefaultAllocator physAllocator;
static physx::PxDefaultErrorCallback physErrCallback;

physx::PxFoundation* foundation = nullptr;

physx::PxPhysics* physics = nullptr;

physx::PxCooking* cooking = nullptr;

physx::PxDefaultCpuDispatcher* dispatcher = nullptr;

physx::PxScene* scene = nullptr;

physx::PxMaterial* material = nullptr;

std::vector<physx::PxRigidActor*> actors;

PxShape* shape = nullptr;

void createCubePhys(PxTransform t)
{
	PxRigidDynamic* body = physics->createRigidDynamic(t);
	body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
	scene->addActor(*body);
	actors.push_back(body);
}

Engine::Engine()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	// temp physx init
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, physAllocator, physErrCallback);
	if (!foundation)
	{
		std::cout << "PhysX failed to init\n";
		return;
	}

	physx::PxTolerancesScale scale;
	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, scale, true, nullptr);
	if (!physics)
	{
		std::cout << "PhysX physics engine failed to init\n";
		return;
	}

	PxCookingParams cookingps(scale);
	cookingps.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
	cookingps.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
	//cookingps.midphaseDesc.mBVH33Desc // TODO: ?? eCOOKING_PERFORMANCE
	cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, cookingps);
	if (!cooking)
	{
		std::cout << "Failed to init physx cooking\n";
		return;
	}
	
	PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	dispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	scene = physics->createScene(sceneDesc);

	material = physics->createMaterial(0.5f, 0.5f, 0.6f);

	PxRigidStatic* ground = PxCreatePlane(*physics, PxPlane(0, 1, 0, 0), *material);
	scene->addActor(*ground);

	{
		//PxShape* shape = physics->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *material);
		shape = physics->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *material);

		uint32_t count = 3;
		static PxTransform transforms[] =
		{
			PxTransform(PxVec3(-0.75f, 5.0f, -1.0f)),
			PxTransform(PxVec3(0.75f, 5.0f, -1.0f)),
			PxTransform(PxVec3(0.0f, 7.0f, -1.0f))
		};

		for (int i = 0; i < count; i++)
		{
			createCubePhys(transforms[i]);
		}

		//shape->release();
	}

	//

	Window = SDL_CreateWindow(
		"Kinema",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		1600, 900,
		SDL_WINDOW_OPENGL);

	device = new Device(Window);

	cubeVB = new VertexBuffer(cubeVertices, sizeof(cubeVertices),
		{
			{ AttributeType::Float, 3, 6 * sizeof(float), 0 },
			{ AttributeType::Float, 3, 6 * sizeof(float), 3 * sizeof(float) }
		}, BufferUsage::Static);

	cubeIB = new IndexBuffer(cubeIndices, sizeof(cubeIndices), BufferUsage::Static);

	camera.SetPosition(glm::vec3(0, 0, 5));
	std::cout << camera.GetForward().x << " " << camera.GetForward().y << " " << camera.GetForward().z << "\n";
	//currentPos = glm::vec3(0, 0, 5);
	//currentFwd = glm::vec3(0, 0, -1);

	CameraData camData;
	//camData.Projection = glm::perspective(glm::radians(75.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
	//camData.View = glm::lookAt(currentPos, glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
	camData.Projection = camera.GetProjection();
	camData.View = camera.GetView();

	cameraUB = new UniformBuffer(this, &camData, sizeof(camData));

	// vec3s are secretly vec4/16 bytes in glsl?
	static glm::vec4 palette[256] = {
		glm::vec4(255, 0, 0, 0),
		glm::vec4(63, 55, 55, 0),
		glm::vec4(33, 33, 38, 0),
		glm::vec4(255, 255, 0, 0),
		glm::vec4(181, 161, 159, 0),
		glm::vec4(64, 0, 0, 0),
		glm::vec4(137, 131, 97, 0),
		glm::vec4(255, 255, 255, 0),
		glm::vec4(255, 0, 110, 0),
		glm::vec4(81, 124, 0, 0)
	};

	voxelUB = new UniformBuffer(this, &palette, sizeof(glm::vec4) * 256);

	colorShader = Shader::FromSource(this,
		File::ReadAllText("shaders/vertColor.vert").data(),
		File::ReadAllText("shaders/vertColor.frag").data());

	litShader = Shader::FromSource(this,
		File::ReadAllText("shaders/test.vert").data(),
		File::ReadAllText("shaders/test.frag").data());

	colorShader->SetMatrix("model", glm::mat4(1.0f));
	colorShader->SetUniformBuffer("camera", cameraUB);

	// litShader camera???
	litShader->SetUniformBuffer("camera", cameraUB);
	litShader->SetUniformBuffer("voxel", voxelUB);

	db = new DebugRenderer(cameraUB);

	device->CheckErrorTemp();


	// menger
	VoxelMesh* menger = new VoxelMesh("menger");
	menger->LoadXRAW("menger.xraw");

	menger->InitChunks();

	menger->SetPosition(glm::vec3(9.6f, 0, -9.6f));

	VoxelMesh* xrcube = new VoxelMesh("cube");
	xrcube->LoadXRAW("cube.xraw");

	xrcube->InitChunks();

	xrcube->SetPosition(glm::vec3(-9.6f, 0, -9.6f));

	//mengerUB = new UniformBuffer(this, menger->tempPalette.data(), menger->tempPalette.size());
	//litShader->SetUniformBuffer("voxel", mengerUB);

	// map
	VoxelMesh* vm = new VoxelMesh("test");
	vm->LoadFromFile("test.map");
	//vm->GenerateWave(2048, 64, 2048);

	vm->InitChunks();

	meshii.push_back(vm);
	meshii.push_back(menger);
	meshii.push_back(xrcube);
	currentvm = vm;

	VoxelMesh* wave = new VoxelMesh("wave");
	wave->GenerateWave(64, 16, 64);

	wave->InitChunks();
	meshii.push_back(wave);

	// imgui
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(Window, device->Context);
	ImGui_ImplOpenGL3_Init("#version 330");

	f = 0.0;

	Running = true;
	while (Running)
	{
		auto startTime = std::chrono::high_resolution_clock::now();
		Update();
		Render();

		SDL_GL_SwapWindow(Window);

		auto endTime = std::chrono::high_resolution_clock::now();

		f += std::chrono::duration_cast<std::chrono::microseconds>(
			endTime - startTime).count() / 1000.0;
		if (f >= (1000.0 / 10.0))
		{
			f = 0.0;
			frameTime = std::chrono::duration_cast<std::chrono::microseconds>(
				endTime - startTime).count() / 1000.0;
		}
	}
}

Engine::~Engine()
{
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

float minspd = 0.001f;
float maxspd = .25f;
float movspeed = 0.01f * 5.0f;

static bool simulatePhysics = false;

bool capd = false;
void Engine::Update()
{
	// physics!
	if (simulatePhysics)
	{
		scene->simulate(1.0f / 144.0f);
		scene->fetchResults(true);
	}

	//



	const float mousesense = 0.04f;

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			Running = false;
			break;
		}

		if (!capd && ImGui::GetIO().WantCaptureMouse)
			break;

		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			if (!capd)
			{
				std::cout << "Mouse captured\n";
				//SDL_ShowCursor(SDL_DISABLE);
				SDL_SetRelativeMouseMode(SDL_TRUE);
				capd = true;
			}
			else
			{
				// voxelclick
				glm::vec3 start = camera.GetPosition();
				glm::vec3 dir = camera.GetForward();

				float radius = (500.0f * BLOCK_SIZE);

				templine t;
				t.start = start;
				t.end = start + dir * (radius * BLOCK_SIZE);
				t.color = glm::vec3(1, 1, 0);
				t.ticksleft = 720;

				templines.push_back(t);

				Block b;
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					b.Type = 0;
					//b.Color = glm::vec3(255.0f, 0.0f, 110.0f);
				}
				else if (event.button.button == SDL_BUTTON_RIGHT)
				{
					b.Type = 8;
					//b.Color = glm::vec3(255.0f, 0.0f, 110.0f);
				}

				for (VoxelMesh* vm : meshii)
				{
					vm->Raycast(start, dir, radius, b);
				}

				//db->DrawLine(device, start, start + dir * 5.0f, glm::vec3(1.0f, 0.0f, 1.0f));

				//vm->Raycast();
			}
			break;
		case SDL_MOUSEMOTION:
			if (capd)
			{
				pitch -= event.motion.yrel * mousesense;
				yaw += event.motion.xrel * mousesense;
			}
			break;
		case SDL_MOUSEWHEEL:
			if (capd)
			{
				if (event.wheel.y > 0)
				{
					movspeed += 0.01f;
				}
				else if (event.wheel.y < 0)
				{
					movspeed -= 0.01f;
				}

				if (movspeed > maxspd)
					movspeed = maxspd;
				if (movspeed < minspd)
					movspeed = minspd;
				std::cout << "Camspeed " << movspeed << "\n";
			}
			break;
		}
	}

	for (templine& t : templines)
	{
		t.ticksleft -= 1;
		if (t.ticksleft < 1)
		{
			templines.erase(templines.begin()); // wont work for different times tho lol
		}
		else
		{
			db->DrawLine(device, t.start, t.end, t.color);
		}
	}

	// clamp pitch, mod yaw
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 currentPos = camera.GetPosition();
	glm::vec3 currentFwd = camera.GetForward();
	glm::vec3 right = camera.GetRight();
	glm::vec3 camUp = glm::vec3(0, 1, 0);

	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		const uint8_t* kb = SDL_GetKeyboardState(nullptr);
		if (kb[SDL_SCANCODE_W])
		{
			currentPos += currentFwd * movspeed;
		}
		if (kb[SDL_SCANCODE_A])
		{
			currentPos -= right * movspeed;
		}
		if (kb[SDL_SCANCODE_S])
		{
			currentPos -= currentFwd * movspeed;
		}
		if (kb[SDL_SCANCODE_D])
		{
			currentPos += right * movspeed;
		}

		if (kb[SDL_SCANCODE_SPACE])
		{
			currentPos += camUp * movspeed;
		}
		if (kb[SDL_SCANCODE_LCTRL])
		{
			currentPos -= camUp * movspeed;
		}

		// focus unfocus
		if (kb[SDL_SCANCODE_ESCAPE] && capd)
		{
			std::cout << "Mouse uncaptured\n";
			SDL_SetRelativeMouseMode(SDL_FALSE);
			capd = false;
		}
	}

	camera.SetPosition(currentPos);
	camera.SetRotation(glm::vec3(pitch, yaw, 0));

	CameraData camdat;
	camdat.View = camera.GetView();
	camdat.Projection = camera.GetProjection();

	cameraUB->SetData(&camdat, 0, sizeof(camdat));
}

bool showDemoWindow = false;
bool showAboutWindow = false;

bool drawbounds = false;

int32_t imthr = 4;

float fov = 90.0f;
void Engine::Render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// imgui windows temp
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Engine"))
			{
				if (ImGui::MenuItem("About"))		showAboutWindow = true;
				if (ImGui::MenuItem("ImGui Demo"))	showDemoWindow = true;

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (showDemoWindow)
		{
			ImGui::ShowDemoWindow(&showDemoWindow);
		}

		if (showAboutWindow)
		{
			ImGui::SetNextWindowSize(ImVec2(0, 0));
			ImGui::Begin("About", &showAboutWindow);
			ImGui::Text("Kinema Engine\nCopyright 2021 William Yates\n");
			ImGui::End();
		}

		{
			ImGui::SetNextWindowSize(ImVec2(0, 0));
			ImGui::Begin("Voxels");

			static int currentSelection = 0;
			if (ImGui::BeginListBox("Voxel Meshes"))
			{
				for (int n = 0; n < meshii.size(); n++)
				{
					std::stringstream s;
					s << "VoxelMesh #" << n;

					const bool is_selected = currentSelection == n;
					if (ImGui::Selectable(meshii[n]->Name.c_str(), is_selected))
					{
						currentSelection = n;
						currentvm = meshii[n];
					}
				}

				ImGui::EndListBox();
			}

			glm::vec3 vmpos = currentvm->GetPosition();
			float voxpos[3] =
			{
				vmpos.x,
				vmpos.y,
				vmpos.z
			};

			ImGui::InputFloat3("Position", voxpos);
			ImGui::Checkbox("Show bounds", &drawbounds);
			ImGui::InputInt("Threads", &imthr);
			ImGui::Text("Last Time: %dms", currentvm->lasttime);
			if (ImGui::Button("Clear data"))
			{
				if (!currentvm->generating)
				{
					for (VoxelChunk& chunk : currentvm->Chunks)
					{
						chunk.cleardata();
					}
				}
			}
			if (ImGui::Button("Generate (Greedy)"))
			{
				if (!currentvm->generating)
				{
					for (VoxelChunk& chunk : currentvm->Chunks)
					{
						chunk.cleardata();
					}

					std::cout << "Starting generation..\n";


					currentvm->GenChunksGreedy(imthr);

				}
			}
			if (ImGui::Button("Add voxel object"))
			{
				VoxelMesh* newmesh = new VoxelMesh("newMesh", false);
				newmesh->LoadXRAW("axis.xraw");
				newmesh->InitChunks();

				newmesh->SetPosition(camera.GetPosition() + camera.GetForward() * 4.0f);

				meshii.push_back(newmesh);
			}
			if (ImGui::Button("Start sim for object"))
			{
				currentvm->startNonStaticSim();
			}

			//if (ImGui::Button("Save"))
			//{
			//	vm->tempSave();
			//}
			//if (ImGui::Button("Load"))
			//{
			//	vm->tempLoad();
			//}

			ImGui::End();

			if (currentvm)
			{
				glm::vec3 pos(voxpos[0], voxpos[1], voxpos[2]);

				//currentvm->pos = pos;
				currentvm->SetPosition(pos);
			}
		}

		{
			ImGui::SetNextWindowSize(ImVec2(0, 0));
			ImGui::Begin("Peformance");

			ImGui::Text("Frametime: %.3f", (float)frameTime);
			ImGui::Text("FPS: %.1f", 1000.0f / (float)frameTime);

			ImGui::End();
		}

		{
			ImGui::SetNextWindowSize(ImVec2(0, 0));
			ImGui::Begin("Physics");

			ImGui::Checkbox("Simulate", &simulatePhysics);
			if (ImGui::Button("Add cube"))
			{
				glm::vec3 result = camera.GetPosition() + camera.GetForward() * 4.0f;
				createCubePhys(PxTransform(PxVec3(result.x, result.y, result.z)));
			}

			ImGui::End();
		}

	}

	camera.SetFieldOfView(fov);

	for (VoxelMesh* m : meshii)
	{
		m->CheckThreads();
	}

	//if (/*generationThread.joinable() && */!vm->generating)
	//{
	//	//generationThread.join();
	//	algtimeend = SDL_GetTicks();

	//	lastalgtime = algtimeend - algtimestart;

	//	std::cout << "Work finished!\n";

	//	//vm->UploadAllChunks();
	//}

	ImGui::Render();

	device->Clear();

	//device->SetShader(colorShader);

	//// square1
	//glm::mat4 mat(1.0f);
	//uint32_t tick = SDL_GetTicks();
	//mat = glm::rotate(mat, glm::radians(tick * 0.25f), glm::vec3(0.0f, 1.0f, 0.0f));
	//colorShader->SetMatrix("model", mat);

	////device->Draw(squareVB, squareIB, 6);

	//
	//// triangle1
	//glm::mat4 mat2(1.0f);
	//mat2 = glm::translate(mat2, glm::vec3(1.5f, 0.0f, 0.0f));
	//mat2 = glm::rotate(mat2, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//colorShader->SetMatrix("model", mat2);

	//device->Draw(triangleVB, triangleIB, 3);


	//// cube1
	//glm::mat4 mat3(1.0f);
	//mat3 = glm::translate(mat3, glm::vec3(-1.5f, 0.0f, 0.0f));
	//colorShader->SetMatrix("model", mat3 * mat * mat * mat);

	//device->Draw(cubeVB, 36);


	//// cube2
	//mat3 = glm::translate(mat3, glm::vec3(0, 2.0f, 0.0f));
	//mat3 = glm::rotate(mat3, glm::radians(tick * -0.25f), glm::vec3(0.0f, 1.0f, 0.0f));
	//mat3 = glm::scale(mat3, glm::vec3(2 + (sin(tick * 0.0025f) + 1) * 0.25f, 0.25f + (cos(tick * 0.005f) + 1) * 0.25f, 0.5f + cos(tick * 0.01f) + 1));
	//colorShader->SetMatrix("model", mat3);

	//device->Draw(cubeVB, 36);

	device->SetShader(litShader);

	if (drawbounds)
	{
		for (VoxelChunk& ch : currentvm->Chunks)
		{
			// TODO: this doesnt respect transformations with rotation (debug box assumes orientation)
			ch.DrawChunkBoundary(device, db, currentvm->GetTransform());
		}
	}

	static float rot = 0.0f;
	//rot += 5.0f / 144.0f;

	for (VoxelMesh* vm : meshii)
	{
		vm->tempdb = db;
		//vr.Render(device);
		//vm->maptransform = glm::mat4(1.0f);
		//vm->maptransform = glm::translate(vm->maptransform, vm->pos);
		//vm->maptransform = glm::rotate(vm->maptransform, glm::radians(rot), glm::vec3(1, 1, 0));

		vm->RenderChunks(device, litShader);
	}

	// Physics render test
	for (PxRigidActor* actor : actors)
	{
		PxTransform t = actor->getGlobalPose();
		PxMat44 mat = PxMat44(t);

		glm::mat4 gmat = glm::make_mat4(mat.front());

		colorShader->SetMatrix("model", gmat);

		//colorShader->SetMatrix("model",

		device->Draw(cubeVB, cubeIB, 36);
	}


	//

	glm::vec3 zero(0.0f);
	glm::vec3 xvec(1, 0, 0);
	glm::vec3 yvec(0, 1, 0);
	glm::vec3 zvec(0, 0, 1);

	db->DrawLine(device, zero, xvec, glm::vec3(1, 0, 0));
	db->DrawLine(device, zero, yvec, glm::vec3(0, 1, 0));
	db->DrawLine(device, zero, zvec, glm::vec3(0, 0, 1));

	//db->DrawCube(device, glm::vec3(8, 0, 0), glm::vec3(2, 2, 2), glm::vec3(0, 1, 0));

	db->Render(device);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	device->CheckErrorTemp();
}