    #version 330 core
    layout (location = 0) in vec3 position;
	//layout (location = 1) in vec3 color;
	layout (location = 1) in vec3 normal;
	layout (location = 2) in float type;

	out vec3 iColor;
	out vec3 iNormal;
	out vec3 fragpos;

	layout(std140) uniform camera
	{
		uniform mat4 projection;
		uniform mat4 view;
	};
	uniform mat4 model;

	vec3 palette[10] = vec3[](
		vec3(255, 0, 0),
		vec3(63, 55, 55),
		vec3(33, 33, 38),
		vec3(255, 255, 0),
		vec3(181, 161, 159),
		vec3(64, 0, 0),
		vec3(137, 131, 97),
		vec3(255, 255, 255),
		vec3(255, 0, 110),
		vec3(81, 124, 0)
	);

    void main()
    {
		iColor = palette[int(type)] / 255.0;
		iNormal = mat3(model) * normal;

		fragpos = vec3(model * vec4(position, 1.0));

        gl_Position = projection * view * model * vec4(position, 1.0);
    }