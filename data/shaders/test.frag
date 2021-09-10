#version 330 core
	
in vec3 iColor;
in vec3 iNormal;

in vec3 fragpos;

out vec4 color;

void main()
{
    vec3 lightpos = vec3(0, 24, 0);

    vec3 normal = normalize(iNormal);
    //vec3 lightdir = normalize(lightpos - fragpos);
    vec3 lightdir = normalize(-(vec3(1, -1, 1)));

    float diffuse = max(dot(normal, lightdir), 0.0);

    vec3 ambient = iColor * 0.025;

    color = vec4(diffuse * iColor + ambient, 1.0);
    color.rgb = pow(color.rgb, vec3(1.0 / 2.2));

    //color.rgb = iColor;
}