#version 330 core
	
in vec3 iColor;
in vec3 iNormal;

in vec3 fragpos;

out vec4 color;

void main()
{
    vec3 lightpos = vec3(0, 24, 0);

    vec3 normal = normalize(iNormal);
    vec3 lightdir = normalize(lightpos - fragpos);

    float diffuse = max(dot(normal, lightdir), 0.0);

    color = vec4(diffuse * iColor + vec3(0.025), 1.0);
}