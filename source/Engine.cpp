#include "Engine.h"


#include "Device.h"
#include "Voxel.h"
#include "File.h"

#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glad/glad.h"

#include <fstream>

#include <thread>

#include "MemoryStream.h"

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
	 -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
	  0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	  0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
	  0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
	 -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	 -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,

	 -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
	  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
	  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
	  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
	 -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
	 -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,

	 -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
	 -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
	 -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
	 -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,

	  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
	  0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
	  0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
	  0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
	  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
	  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,

	 -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
	  0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
	  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
	  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
	 -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,

	 -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
	  0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
	  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
	  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
	 -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
	 -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f
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

Engine::Engine()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	Window = SDL_CreateWindow(
		"Kinema",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		1600, 900,
		SDL_WINDOW_OPENGL);

	device = new Device(Window);

	squareVB = new VertexBuffer(squareVertices, sizeof(squareVertices),
		{
			{ AttributeType::FLOAT, 3, 3 * sizeof(float), 0 }
		}, BufferUsage::Static);

	squareIB = new IndexBuffer(squareIndices, sizeof(squareIndices), BufferUsage::Static);

	triangleVB = new VertexBuffer(triangleVertices, sizeof(triangleVertices),
		{
			{ AttributeType::FLOAT, 2, 5 * sizeof(float), 0 },
			{ AttributeType::FLOAT, 3, 5 * sizeof(float), 2 * sizeof(float) }
		}, BufferUsage::Static);
	triangleIB = new IndexBuffer(triangleIndices, sizeof(triangleIndices), BufferUsage::Static);

	cubeVB = new VertexBuffer(cubeVertices, sizeof(cubeVertices),
		{
			{ AttributeType::FLOAT, 3, 6 * sizeof(float), 0 },
			{ AttributeType::FLOAT, 3, 6 * sizeof(float), 3 * sizeof(float) }
		}, BufferUsage::Static);

	camera.SetPosition(glm::vec3(0, 0, 5));
	std::cout << camera.GetForward().x << " " << camera.GetForward().y << " " << camera.GetForward().z << "\n";
	//currentPos = glm::vec3(0, 0, 5);
	//currentFwd = glm::vec3(0, 0, -1);

	CameraData camData;
	//camData.Projection = glm::perspective(glm::radians(75.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
	//camData.View = glm::lookAt(currentPos, glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
	camData.Projection = camera.GetProjection();
	camData.View = camera.GetView();

	ub = new UniformBuffer(this, &camData, sizeof(camData));

	colorShader = Shader::FromSource(this,
		File::ReadAllText("shaders/vertColor.vert").data(),
		File::ReadAllText("shaders/vertColor.frag").data());

	litShader = Shader::FromSource(this,
		File::ReadAllText("shaders/test.vert").data(),
		File::ReadAllText("shaders/test.frag").data());

	colorShader->SetMatrix("model", glm::mat4(1.0f));
	colorShader->SetUniformBuffer("camera", ub);

	db = new DebugRenderer(ub);

	device->CheckErrorTemp();


	//vm = VoxelMap();
	vm = new VoxelMap();
	vm->LoadFromFile("test.map");
	//vm->GenerateWave(2048, 64, 2048);

	vm->InitChunks();

	vr = VoxelRenderer();

	// imgui
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(Window, device->Context);
	ImGui_ImplOpenGL3_Init("#version 330");

	Running = true;
	while (Running)
	{
		Update();
		Render();

		SDL_GL_SwapWindow(Window);
	}

	// delete.. todo:
}

Engine::~Engine()
{
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

float minspd = 0.001f;
float maxspd = .25f;
float movspeed = 0.01f * 5.0f;

bool capd = false;
void Engine::Update()
{
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
					b.Active = false;
					b.Color = glm::vec3(255.0f, 0.0f, 110.0f);
				}
				else if (event.button.button == SDL_BUTTON_RIGHT)
				{
					b.Active = true;
					b.Color = glm::vec3(255.0f, 0.0f, 110.0f);
				}

				vm->Raycast(start, dir, radius, b);

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

	ub->SetData(&camdat, 0, sizeof(camdat));
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

		ImGui::SetNextWindowSize(ImVec2(0, 0));
		ImGui::Begin("Voxels");
		ImGui::Checkbox("Show bounds", &drawbounds);
		ImGui::InputInt("Threads", &imthr);
		ImGui::Text("Last Time: %dms", vm->lasttime);
		if (ImGui::Button("Clear data"))
		{
			if (!vm->generating)
			{
				for (VoxelChunk& chunk : vm->Chunks)
				{
					chunk.cleardata();
				}
			}
		}
		if (ImGui::Button("Generate (Greedy)"))
		{
			if (!vm->generating)
			{
				for (VoxelChunk& chunk : vm->Chunks)
				{
					chunk.cleardata();
				}

				std::cout << "Starting generation..\n";
				 

				vm->GenChunksGreedy(imthr);

			}
		}

		if (ImGui::Button("Reload shader\n"))
		{
			Shader* news = Shader::FromSource(this,
				File::ReadAllText("shaders/test.vert").data(),
				File::ReadAllText("shaders/test.frag").data());

			if (news != nullptr)
			{
				delete litShader;
				litShader = news;
			}
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
	}

	camera.SetFieldOfView(fov);

	vm->CheckThreads();

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

	device->SetShader(colorShader);

	// square1
	glm::mat4 mat(1.0f);
	uint32_t tick = SDL_GetTicks();
	mat = glm::rotate(mat, glm::radians(tick * 0.25f), glm::vec3(0.0f, 1.0f, 0.0f));
	colorShader->SetMatrix("model", mat);

	//device->Draw(squareVB, squareIB, 6);

	
	// triangle1
	glm::mat4 mat2(1.0f);
	mat2 = glm::translate(mat2, glm::vec3(1.5f, 0.0f, 0.0f));
	mat2 = glm::rotate(mat2, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	colorShader->SetMatrix("model", mat2);

	device->Draw(triangleVB, triangleIB, 3);


	// cube1
	glm::mat4 mat3(1.0f);
	mat3 = glm::translate(mat3, glm::vec3(-1.5f, 0.0f, 0.0f));
	colorShader->SetMatrix("model", mat3 * mat * mat * mat);

	device->Draw(cubeVB, 36);


	// cube2
	mat3 = glm::translate(mat3, glm::vec3(0, 2.0f, 0.0f));
	mat3 = glm::rotate(mat3, glm::radians(tick * -0.25f), glm::vec3(0.0f, 1.0f, 0.0f));
	mat3 = glm::scale(mat3, glm::vec3(2 + (sin(tick * 0.0025f) + 1) * 0.25f, 0.25f + (cos(tick * 0.005f) + 1) * 0.25f, 0.5f + cos(tick * 0.01f) + 1));
	colorShader->SetMatrix("model", mat3);

	device->Draw(cubeVB, 36);

	device->SetShader(litShader);

	glm::mat4 voxmat(1.0f);
	voxmat = glm::translate(voxmat, glm::vec3(0, 0, 8.0f));
	voxmat = glm::scale(voxmat, glm::vec3(0.1f, 0.1f, 0.1f));
	litShader->SetMatrix("model", voxmat);

	if (drawbounds)
	{
		for (VoxelChunk& ch : vm->Chunks)
		{
			// TODO: this doesnt respect transformations with rotation (debug box assumes orientation)
			ch.DrawChunkBoundary(device, db, vm->maptransform);
		}
	}

	static float rot = 0.0f;
	rot += 5.0f / 144.0f;

	vm->tempdb = db;
	//vr.Render(device);
	vm->maptransform = glm::mat4(1.0f);
	vm->maptransform = glm::translate(vm->maptransform, glm::vec3(0, 0, 8));
	vm->maptransform = glm::rotate(vm->maptransform, glm::radians(rot), glm::vec3(1, 1, 0));

	vm->RenderChunks(device, litShader);

	glm::vec3 zero(0.0f);
	glm::vec3 xvec(1, 0, 0);
	glm::vec3 yvec(0, 1, 0);
	glm::vec3 zvec(0, 0, 1);

	db->DrawLine(device, zero, xvec, glm::vec3(1, 0, 0));
	db->DrawLine(device, zero, yvec, glm::vec3(0, 1, 0));
	db->DrawLine(device, zero, zvec, glm::vec3(0, 0, 1));

	db->DrawCube(device, glm::vec3(8, 0, 0), glm::vec3(2, 2, 2), glm::vec3(0, 1, 0));

	db->Render(device);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	device->CheckErrorTemp();
}