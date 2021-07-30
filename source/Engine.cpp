#include "Engine.h"

#include "GL/glew.h"

#include "VertexBuffer.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include "IndexBuffer.h"

#include "glm/gtc/matrix_transform.hpp"

//float vertices[] = 
//{
//	 0.0f,  0.5f, 0.0f,
//	 0.5f, -0.5f, 0.0f,
//	-0.5f, -0.5f, 0.0f
//};

float vertices[] = 
{
	-0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f
};

// uint32_t indices[] = { 0, 1, 2 };
uint32_t indices[] = 
{ 
	0, 1, 2,
	2, 3, 0 
};

const GLchar* vertexSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 position;

	layout(std140) uniform camera
	{
		uniform mat4 projection;
		uniform mat4 view;
	};
	uniform mat4 model;

    void main()
    {
        gl_Position = projection * view * model * vec4(position, 1.0);
    }
)glsl";

const GLchar* fragmentSource = R"glsl(
    #version 330 core
    out vec4 color;

    void main()
    {
        color = vec4(1.0, 1.0, 1.0, 1.0);
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

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		// TODO: err
		return;
	}


	// Temp
	//VertexBuffer* vb = new VertexBuffer(this, vertices, sizeof(vertices));
	VertexBuffer* vb = new VertexBuffer(this, vertices, sizeof(vertices),
		{ 
			{ AttributeType::FLOAT, 3, 3 * sizeof(float), 0 } 
		});

	IndexBuffer* ib = new IndexBuffer(this, indices, sizeof(indices));

	struct
	{
		glm::mat4 projection;
		glm::mat4 view;
	} camData;

	camData.projection = glm::perspective(glm::radians(75.0f), 1280.0f / 720.0f, 0.1f, 10.0f);
	camData.view = glm::lookAt(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	UniformBuffer* ub = new UniformBuffer(this, &camData, sizeof(camData));

	Shader* shader = Shader::FromSource(this, vertexSource, fragmentSource);
	shader->SetMatrix("model", glm::mat4(1.0f));
	shader->SetUniformBuffer("camera", ub);

	float x = 12.0f / 255.0f;
	glClearColor(x, x, x, 1.0f);

	glEnable(GL_CULL_FACE);

	// TODO: common GL check err define
	std::cout << "GLError: " << glGetError() << "\n";

	while (true)
	{
		SDL_PollEvent(nullptr);

		glm::mat4 mat(1.0f);
		uint32_t tick = SDL_GetTicks();
		mat = glm::rotate(mat, glm::radians(tick * 0.25f), glm::vec3(0.0f, 1.0f, 0.0f));

		shader->SetMatrix("model", mat);


		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader->ShaderProgram);

		glBindVertexArray(vb->VertexArray);

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->BufferObject);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		SDL_GL_SwapWindow(Window);
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