#version 330 core

out vec4 color;

in vec3 ourColor;
in vec3 TexCoord;

void main()
{
	color=vec4(ourColor,1.0);
}