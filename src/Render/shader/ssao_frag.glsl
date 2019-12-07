#version 330 core
layout(location=0) out vec4 gPositionDepth;
layout(location=1) out vec4 gNormal;
layout(location=2) out vec4 gAlbedoSpec;

in VS_OUT 
{
	vec2 TexCoords;
	vec3 FragPos;
	vec3 Normal;
}fs_in;

const float NEAR=0.1;
const float FAR=50.f;

float LinearizeDepth(float depth)
{
	float z=depth*2.0-1.0;
	return (2.0*NEAR*FAR)/(FAR+NEAR-z)
}

void main()
{
	gPositionDepth.xyz=fs_in.FragPos;
	gPositionDepth.a=;
	gNormal=normalize(Normal);
	gAlbedoSpec.rgb=vec3(0.95);
}