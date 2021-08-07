    #version 330 core
    layout (location = 0) in vec3 position;
	layout (location = 1) in vec3 color;
	layout (location = 2) in vec3 normal;

	out vec3 iColor;
	out vec3 iNormal;
	out vec3 fragpos;

	layout(std140) uniform camera
	{
		uniform mat4 projection;
		uniform mat4 view;
	};
	uniform mat4 model;

    void main()
    {
		iColor = color;
		iNormal = normal;

		fragpos = vec3(model * vec4(position, 1.0));

        gl_Position = projection * view * model * vec4(position, 1.0);
    }