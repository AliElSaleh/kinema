#include "Engine.h"


#include "Device.h"
#include "Voxel.h"
#include "File.h"

#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glad/glad.h"

#include <fstream>

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

Engine::Engine()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	Window = SDL_CreateWindow(
		"Kinema",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		1280, 720,
		SDL_WINDOW_OPENGL);

	device = new Device(Window);

	squareVB = new VertexBuffer(this, squareVertices, sizeof(squareVertices),
		{
			{ AttributeType::FLOAT, 3, 3 * sizeof(float), 0 }
		});

	squareIB = new IndexBuffer(this, squareIndices, sizeof(squareIndices));

	triangleVB = new VertexBuffer(this, triangleVertices, sizeof(triangleVertices),
		{
			{ AttributeType::FLOAT, 2, 5 * sizeof(float), 0 },
			{ AttributeType::FLOAT, 3, 5 * sizeof(float), 2 * sizeof(float) }
		});
	triangleIB = new IndexBuffer(this, triangleIndices, sizeof(triangleIndices));

	cubeVB = new VertexBuffer(this, cubeVertices, sizeof(cubeVertices),
		{
			{ AttributeType::FLOAT, 3, 6 * sizeof(float), 0 },
			{ AttributeType::FLOAT, 3, 6 * sizeof(float), 3 * sizeof(float) }
		});

	currentPos = glm::vec3(0, 0, 5);
	currentFwd = glm::vec3(0, 0, -1);

	CameraData camData;
	camData.Projection = glm::perspective(glm::radians(75.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
	camData.View = glm::lookAt(currentPos, glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));

	ub = new UniformBuffer(this, &camData, sizeof(camData));

	colorShader = Shader::FromSource(this,
		File::ReadAllText("shaders/vertColor.vert").data(),
		File::ReadAllText("shaders/vertColor.frag").data());

	litShader = Shader::FromSource(this,
		File::ReadAllText("shaders/test.vert").data(),
		File::ReadAllText("shaders/test.frag").data());

	colorShader->SetMatrix("model", glm::mat4(1.0f));
	colorShader->SetUniformBuffer("camera", ub);

	device->CheckErrorTemp();


	vm = VoxelMap();
	vm.LoadFromFile("bin.map");
	//vm.generate(64, 64, 64);

	vr = VoxelRenderer();
	vr.Update(&vm);


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

float movspeed = 0.001f;
float minspd = 0.001f;
float maxspd = .25f;

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

		if (ImGui::GetIO().WantCaptureMouse)
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

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 dir;
	dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	dir.y = sin(glm::radians(pitch));
	dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	currentFwd = glm::normalize(dir);

	glm::vec3 right = glm::cross(currentFwd, camUp);
	right = glm::normalize(right);
	
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

	CameraData camdat;
	camdat.View = glm::lookAt(currentPos, currentPos + currentFwd, camUp);

	ub->SetData(&camdat.View, sizeof(glm::mat4), sizeof(glm::mat4));
	//ub->SetData(&camdat, offsetof(camdat, View), sizeof(glm::mat4));

	//std::cout << currentPos.x << " " << currentPos.y << " " << currentPos.z << "\n";
}

void Engine::Render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowDemoWindow();

	ImGui::Render();


	device->Clear();

	device->SetShader(colorShader);

	// square1
	glm::mat4 mat(1.0f);
	uint32_t tick = SDL_GetTicks();
	mat = glm::rotate(mat, glm::radians(tick * 0.25f), glm::vec3(0.0f, 1.0f, 0.0f));
	colorShader->SetMatrix("model", mat);

	device->Draw(squareVB, squareIB, 6);


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

	vr.Render(device);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	device->CheckErrorTemp();
}