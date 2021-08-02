#include "Engine.h"

#include "VertexBuffer.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include "IndexBuffer.h"
#include "Device.h"

#include "File.h"

#include "glm/gtc/matrix_transform.hpp"

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

	Device device(Window);

	VertexBuffer* squareVB = new VertexBuffer(this, squareVertices, sizeof(squareVertices),
		{
			{ AttributeType::FLOAT, 3, 3 * sizeof(float), 0 }
		});

	IndexBuffer* squareIB = new IndexBuffer(this, squareIndices, sizeof(squareIndices));

	VertexBuffer* triangleVB = new VertexBuffer(this, triangleVertices, sizeof(triangleVertices),
		{
			{ AttributeType::FLOAT, 2, 5 * sizeof(float), 0 },
			{ AttributeType::FLOAT, 3, 5 * sizeof(float), 2 * sizeof(float) }
		});
	IndexBuffer* triangleIB = new IndexBuffer(this, triangleIndices, sizeof(triangleIndices));

	VertexBuffer* cubeVB = new VertexBuffer(this, cubeVertices, sizeof(cubeVertices),
		{
			{ AttributeType::FLOAT, 3, 6 * sizeof(float), 0 },
			{ AttributeType::FLOAT, 3, 6 * sizeof(float), 3 * sizeof(float) }
		});

	struct
	{
		glm::mat4 projection;
		glm::mat4 view;
	} camData;

	camData.projection = glm::perspective(glm::radians(75.0f), 1280.0f / 720.0f, 0.1f, 10.0f);
	camData.view = glm::lookAt(glm::vec3(0, 0, 5.0f), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));

	UniformBuffer* ub = new UniformBuffer(this, &camData, sizeof(camData));

	Shader* shader = Shader::FromSource(this,
		File::ReadAllBytes("Shaders/test.vert").data(),
		File::ReadAllBytes("Shaders/test.frag").data());
	shader->SetMatrix("model", glm::mat4(1.0f));
	shader->SetUniformBuffer("camera", ub);

	device.CheckErrorTemp();
	while (true)
	{
		SDL_PollEvent(nullptr);

		device.Clear();

		device.SetShader(shader);

		// square1
		glm::mat4 mat(1.0f);
		uint32_t tick = SDL_GetTicks();
		mat = glm::rotate(mat, glm::radians(tick * 0.25f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader->SetMatrix("model", mat);

		device.Draw(squareVB, squareIB, 6);


		// triangle1
		glm::mat4 mat2(1.0f);
		mat2 = glm::translate(mat2, glm::vec3(1.5f, 0.0f, 0.0f));
		mat2 = glm::rotate(mat2, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader->SetMatrix("model", mat2);

		device.Draw(triangleVB, triangleIB, 3);


		// cube1
		glm::mat4 mat3(1.0f);
		mat3 = glm::translate(mat3, glm::vec3(-1.5f, 0.0f, 0.0f));
		shader->SetMatrix("model", mat3 * mat * mat * mat);

		device.Draw(cubeVB, 36);


		// cube2
		mat3 = glm::translate(mat3, glm::vec3(0, 2.0f, 0.0f));
		mat3 = glm::rotate(mat3, glm::radians(tick * -0.25f), glm::vec3(0.0f, 1.0f, 0.0f));
		mat3 = glm::scale(mat3, glm::vec3(2 + (sin(tick * 0.0025f) + 1) * 0.25f, 0.25f + (cos(tick * 0.005f) + 1) * 0.25f, 0.5f + cos(tick * 0.01f) + 1));
		shader->SetMatrix("model", mat3);

		device.Draw(cubeVB, 36);


		device.CheckErrorTemp();
		SDL_GL_SwapWindow(Window);
	}
}

Engine::~Engine()
{
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

void Engine::Update()
{
}

void Engine::Render()
{
}