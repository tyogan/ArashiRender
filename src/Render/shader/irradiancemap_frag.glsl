#version 330 core
	
in vec3 localPos;
out vec4 FragColor;

uniform samplerCube cubeEnvmap;

const float PI = 3.14159265359;

vec3 irradiance=vec3(0.0);

vec3 irradianceColor()
{
	vec3 up=vec3(0,1.0,0);
	vec3 right=normalize(cross(up,Normal));
	up=normalize(cross(Normal,right));
	vec3 N=normalize(Normal)

	float sampleDelta=1.0/40;
	float nrSamples=0.0;

	for(float phi=0.0;phi<2*PI;phi+=sampleDelta)
	{
		for(float theta=0.0;theta<0.5*PI;theta+=sampleDelta)
		{
			vec3 tangentSample=vec3(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
			vec3 sampleVec=tangentSample.x*right+tangentSample.y*up+tangentSample.z*N;
			irradiance+=texture(cubeEnvmap,sampleVec).rgb*cos(theta)*sin(theta);
			nrSamples++;
		}
	}
	irradiance=PI*irradiance*(1.0/float(nrSamples));
	return irradiance;
}

void main()
{
	vec3 color=irradianceColor();
	FragColor=vec4(color,1.0);
}