#version 330 core

out vec4 FragColor;

uniform vec3 lightDir;
uniform vec3 viewPos;

uniform sampler2D texImage;
uniform sampler2D shadowmap;

uniform samplerCube envmap;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

vec3 lightColor = vec3(1.0f);
float ShadowCalculation(vec4 fragPosLightSpace)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowmap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float bias = max(0.05 * (1.0 - dot(normalize(fs_in.Normal), normalize(lightDir))), 0.005);
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowmap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowmap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;
	if(projCoords.z > 1.0)
        shadow = 0.0;
	return shadow;
}

void main()
{	
	//vec3 objectColor=vec3(texture(texImage,fs_in.TexCoords));
	vec3 objectColor=vec3(0.8);
	float ambientStrenth=0.5;
	vec3 ambient= vec3(texture(envmap,fs_in.Normal));
	ambient=lightColor*ambientStrenth;
	vec3 lightDirN=normalize(lightDir);
	vec3 norm=normalize(fs_in.Normal);
	vec3 diffuse = lightColor*max(dot(norm, lightDirN), 0.0);
	 	
	float specularStrength=0.5;
	vec3 viewDir=normalize(viewPos-fs_in.FragPos);
	vec3 reflectDir = reflect(-lightDirN, norm);
	vec3 halfNormal = normalize(-lightDirN+viewDir);

	float spec = pow(max(dot(halfNormal, norm), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	float shadow=ShadowCalculation(fs_in.FragPosLightSpace);

	vec3 result=(ambient+(1-shadow)*(diffuse+specular))*objectColor;
	FragColor=vec4(result,1.0);
}