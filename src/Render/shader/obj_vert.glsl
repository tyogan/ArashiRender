#version 330 core

layout (location=0)in vec3 position;
layout (location=1)in vec3 normal;
layout (location=2)in vec2 texcoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

void main()
{
	gl_Position=P*V*M*vec4(position,1.0);
	
	Normal=mat3(transpose(inverse(M)))*normal;
	TexCoord=texcoord;
	FragPos=vec3(M*vec4(position,1.0f));
}