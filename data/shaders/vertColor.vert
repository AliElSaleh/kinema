    #version 330 core
    layout (location = 0) in vec3 position;
	layout (location = 1) in vec3 color;

	out vec3 iColor;

	layout(std140) uniform camera
	{
		uniform mat4 projection;
		uniform mat4 view;
	};
	uniform mat4 model;

    void main()
    {
		iColor = color;
        gl_Position = projection * view * model * vec4(position, 1.0);
    }