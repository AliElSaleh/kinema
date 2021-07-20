#include "Engine.h"

#include "GL/glew.h"

#include "VertexBuffer.h"
#include "Shader.h"

float vertices[] = {
	 0.0f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
};

const GLchar* vertexSource = R"glsl(
    #version 330 core
    in vec3 position;
    //in vec3 color;
    //out vec3 Color;
    void main()
    {
        //Color = color;
        gl_Position = vec4(position, 1.0);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 330 core
    out vec4 color
    void main()
    {
        color = vec4(1.0, 1.0, 1.0, 1.0);
    }
)glsl";

Engine::Engine()
{
	SDL_Init(SDL_INIT_VIDEO);

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
	VertexBuffer* vb = new VertexBuffer(this, vertices, sizeof(vertices));

	Shader* shader = Shader::FromSource(this, vertexSource, fragmentSource);

	float x = 12.0f / 255.0f;
	glClearColor(x, x, x, 1.0f);

	std::cout << glGetError() << "\n";

	while (true)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader->ShaderProgram);

		glBindVertexArray(vb->VertexArray);

		glDrawArrays(GL_TRIANGLES, 0, 3);

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