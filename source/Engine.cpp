#include "Engine.h"

#include "GL/glew.h"

#include "VertexBuffer.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include "IndexBuffer.h"
#include "Device.h"

#include "glm/gtc/matrix_transform.hpp"

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
	sin(0.0f),  cos(0.0f) * 0.5f,								1.0f, 0.0f, 0.0f,
	sin((2.0f*PI)/3.0f) * 0.5f, cos((2.0f*PI)/ 3.0f) * 0.5f,	0.0f, 1.0f, 0.0f,
	sin((4.0f*PI)/ 3.0f) * 0.5f, cos((4.0f*PI)/ 3.0f) * 0.5f,	0.0f, 0.0f, 1.0f
};

 uint32_t triangleIndices[] = { 0, 1, 2 };

 GLfloat cubeVertices[] = {
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

const GLchar* vertexSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 position;
	layout (location = 1) in vec3 color;

	out vec3 outcolor;

	layout(std140) uniform camera
	{
		uniform mat4 projection;
		uniform mat4 view;
	};
	uniform mat4 model;

    void main()
    {
		outcolor = color;
        gl_Position = projection * view * model * vec4(position, 1.0);
    }
)glsl";

const GLchar* fragmentSource = R"glsl(
    #version 330 core
	
	in vec3 outcolor;

    out vec4 color;

    void main()
    {
        color = vec4(outcolor, 1.0);
    }
)glsl";

Engine::Engine()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	Window = SDL_CreateWindow(
		"Kinema",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		1280, 720,
		SDL_WINDOW_OPENGL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	Context = SDL_GL_CreateContext(Window);

	const GLubyte* strver = glGetString(GL_VERSION);

	std::cout << strver << "\n";

	SDL_GL_SetSwapInterval(0);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		// TODO: err
		return;
	}


	// Temp
	// VertexBuffer* vb = new VertexBuffer(this, vertices, sizeof(vertices));
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

	Shader* shader = Shader::FromSource(this, vertexSource, fragmentSource);
	shader->SetMatrix("model", glm::mat4(1.0f));
	shader->SetUniformBuffer("camera", ub);

	float x = 1.0f / 255.0f;
	glClearColor(x, x, x, 1.0f);

	//glEnable(GL_CULL_FACE);
	glEnable(GL_FRAMEBUFFER_SRGB);

	glEnable(GL_DEPTH_TEST);

	// TODO: common GL check err define
	std::cout << "GLError: " << glGetError() << "\n";

	//uint32_t newvao = 0;
	//glGenVertexArrays(1, &newvao);
	//glBindVertexArray(newvao);

	Device device;

	//glVertexAttribFormat(0, 2, GL_FLOAT, false, 0);
	//glVertexAttribBinding(0, 0);
	//glEnableVertexAttribArray(0);

	//glVertexAttribFormat(1, 3, GL_FLOAT, false, sizeof(float) * 2);
	//glVertexAttribBinding(1, 0);
	//glEnableVertexAttribArray(1);

	while (true)
	{
		SDL_PollEvent(nullptr);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader->ShaderProgram);

		glm::mat4 mat(1.0f);
		uint32_t tick = SDL_GetTicks();
		mat = glm::rotate(mat, glm::radians(tick * 0.25f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader->SetMatrix("model", mat);

		//glBindVertexArray(vb->VertexArray);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->BufferObject);

		//glBindVertexBuffer(0, squareVB->BufferObject, 0, sizeof(float) * 3);
		//glBindVertexBuffer(1, 0, 0, sizeof(float) * 3);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareIB->BufferObject);

		//glBindBuffer(GL_ARRAY_BUFFER, squareVB->BufferObject);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		//glEnableVertexAttribArray(0);

		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)uintptr_t(sizeof(float) * 3));
		//glEnableVertexAttribArray(1);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareIB->BufferObject);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		device.Draw(squareVB, squareIB, 6);

		glm::mat4 mat2(1.0f);
		mat2 = glm::translate(mat2, glm::vec3(1.5f, 0.0f, 0.0f));
		mat2 = glm::rotate(mat2, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader->SetMatrix("model", mat2);

		//glBindVertexArray(triangleVB->VertexArray);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIB->BufferObject);

		//glBindVertexBuffer(0, triangleVB->BufferObject, 0, sizeof(float) * 5);
		//glBindVertexBuffer(1, triangleVB->BufferObject, sizeof(float) * 3, sizeof(float) * 5);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareIB->BufferObject);

		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		device.Draw(triangleVB, triangleIB, 3);

		glm::mat4 mat3(1.0f);
		mat3 = glm::translate(mat3, glm::vec3(-1.5f, 0.0f, 0.0f));
		shader->SetMatrix("model", mat3 * mat* mat* mat);

		//glBindVertexArray(cubeVB->VertexArray);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		device.Draw(cubeVB, 36);

		mat3 = glm::translate(mat3, glm::vec3(0, 2.0f, 0.0f));
		mat3 = glm::rotate(mat3, glm::radians(tick * -0.25f), glm::vec3(0.0f, 1.0f, 0.0f));
		mat3 = glm::scale(mat3, glm::vec3(2 + (sin(tick * 0.0025f) + 1) * 0.25f, 0.25f + (cos(tick * 0.005f) + 1) * 0.25f, 0.5f + cos(tick * 0.01f) + 1));
		shader->SetMatrix("model", mat3);

		//glBindVertexArray(cubeVB->VertexArray);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		device.Draw(cubeVB, 36);

		SDL_GL_SwapWindow(Window);

		//std::cout << tick - last << " since last\n";

		GLenum err = glGetError();
		if (err != 0)
			std::cout << "GLError: " << err << "\n";
	}
}

Engine::~Engine()
{
	SDL_GL_DeleteContext(Context);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

void Engine::Update()
{
}

void Engine::Render()
{
}