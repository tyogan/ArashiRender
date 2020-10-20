#version 330 core

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexcoords;

uniform int MeshIdx;
uniform int MtlIdx;

layout (location=0)out vec3 Position;
layout (location=1)out vec3 Normal;
layout (location=2)out vec3 Texcoords;
layout (location=3)out vec3 FragIndex;

void main()
{
	Position=vPosition;
	Normal=normalize(vNormal);
	Texcoords=vec3(vTexcoords,1.f);
	
	FragIndex=vec3(MeshIdx,MtlIdx,1);
}