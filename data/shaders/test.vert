#version 330 core
layout (location = 0) in uint vertex;

out vec3 iColor;
out vec3 iNormal;
out vec3 fragpos;

layout(std140) uniform camera
{
	uniform mat4 projection;
	uniform mat4 view;
};
uniform mat4 model;

layout(std140) uniform voxel
{
	uniform vec3 palette[256];
};

vec3 blockNormals[6] = vec3[](
	vec3(0, 1, 0),
	vec3(0, -1, 0),
	vec3(0, 0, -1),
	vec3(0, 0, 1),
	vec3(-1, 0, 0),
	vec3(1, 0, 0)
);

void main()
{
	//iColor = palette[int(type)] / 255.0;
	//iNormal = mat3(model) * normal;

	vec3 nposition;
	nposition.x = vertex & 127u;
	nposition.y = vertex >> 7u & 127u;
	nposition.z = vertex >> 14u & 127u;

	nposition *= 0.1;

	uint face = vertex >> 21u & 7u;
	iNormal = mat3(model) * blockNormals[face];

	uint type = vertex >> 24u & 255u;
	iColor = palette[type] / 255.0;

	fragpos = vec3(model * vec4(nposition, 1.0));

	gl_Position = projection * view * model * vec4(nposition, 1.0);
}