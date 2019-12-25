#version 330 core

in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube skybox;

void main()
{
	vec3 envColor = texture(skybox, TexCoords).rgb;
    FragColor = vec4(envColor, 1.0);
}