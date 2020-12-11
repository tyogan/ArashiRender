#version 330 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexcoords;

uniform mat4 V;
uniform mat4 P;

out vec3 fLocalPosition;

void main()
{
	gl_Position=P*V*vec4(vPosition,1.f);
	fLocalPosition=vPosition;
}