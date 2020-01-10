#version 330 core

layout (location=0)in vec3 position;
layout (location=1)in vec3 normal;
layout (location=2)in vec2 texCoords;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out VS_OUT
{
	vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
}vs_out;

void main()
{
	gl_Position=P*V*M*vec4(position,1.0);
	
	vs_out.FragPos=vec3(M * vec4(position, 1.0));
	vs_out.Normal=mat3(transpose(inverse(M)))*normal;
	vs_out.TexCoords = texCoords;
}