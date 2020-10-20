#version 330 core

layout (location=0)in vec3 position;
layout (location=1)in vec3 normal;
layout (location=2)in vec2 texCoords;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexcoords;

void main()
{
	gl_Position=P*V*M*vec4(position,1.f);
	vPosition=(M*vec4(position,1.f)).xyz;
	vNormal=transpose(inverse(mat3(M)))*normal;
	vTexcoords=texCoords;
}