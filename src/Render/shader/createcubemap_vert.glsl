#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texcoords;

out vec3 localPos;
out vec2 TexCoords;

uniform mat4 V;
uniform mat4 P;

void main()
{
	TexCoords=texcoords;
	localPos=position;
	gl_Position=P*V*vec4(position,1.0);
}