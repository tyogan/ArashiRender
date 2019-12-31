#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texcoords;

uniform mat4 V;
uniform mat4 P;

out vec3 localPos;

void main()
{
	gl_Position=P * V * vec4(position,1.0);
	localPos=position;
}