#version 330 core
out vec4 FragColor;

in vec2 fTexCoords;

uniform sampler2D screenTexture;

void main()
{ 
	vec3 color = texture(screenTexture, fTexCoords).rgb;
    FragColor = vec4(color,1.f);
}