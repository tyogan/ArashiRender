#version 330 core
in vec2 fTexCoords;
out float FragColor;

uniform sampler2D gPositionDepth;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];
int kernelSize = 64;
float radius = 1.f;

uniform mat4 projection;
uniform mat4 view;

const vec2 noiseScale = vec2(960.0/4.0, 720.0/4.0); 

void main()
{
	ivec2 resolution = ivec2(960,720);

    vec3 fragPos = (view * vec4(texture(gPositionDepth, fTexCoords).xyz,1.f)).xyz;
	vec3 normal = transpose(inverse(mat3(view))) * texture(gNormal, fTexCoords).xyz;
	vec3 randomVec = normalize(texture(texNoise, fTexCoords*noiseScale).xyz);
	
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	float occlusion = 0.0;
	for(int i = 0; i < kernelSize; ++i)
	{
		vec3 pos = TBN * samples[i];
		pos = fragPos + pos * radius; 

		vec4 offset = vec4(pos, 1.0);
		offset = projection * offset;
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5 + 0.5;

		float sampleDepth = (view * vec4(texture(gPositionDepth, offset.xy).xyz, 1.f)).z;
		
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= pos.z ? 1.0 : 0.0) * rangeCheck; 
	}

	occlusion = 1.0 - (occlusion / kernelSize);
	FragColor = occlusion;  
}