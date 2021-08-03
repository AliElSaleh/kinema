#version 330 core
	
in vec3 iColor;

out vec4 color;

void main()
{
    color = vec4(iColor, 1.0);
}