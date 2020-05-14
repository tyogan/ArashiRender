#version 330 core
out vec4 FragColor;

layout(std140) uniform lightBlock
{
	vec4 lightDir[64];
	vec4 lightColor[64];
};

uniform vec3 viewPos;

uniform sampler2D texImage;
uniform sampler2D shadowmap;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowmap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float bias = max(0.05 * (1.0 - dot(normalize(fs_in.Normal), normalize(lightBlock.lightDir[0].xyz))), 0.005);
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
	vec3 objectColor=vec3(0.8);
	//float ambientStrenth=0.6;
	vec3 ambient=vec3(0);
	for(int i=1;i<64;i++)
	{
		
		vec3 lightDirN=normalize(lightBlock.lightDir[i].xyz);
		vec3 norm=normalize(fs_in.Normal);
		vec3 diffuse = lightBlock.lightColor[i].rgb*max(dot(norm, lightDirN), 0.0);
	 	
		float specularStrength=0.5;
		vec3 viewDir=normalize(viewPos-fs_in.FragPos);
		vec3 reflectDir = reflect(-lightDirN, norm);
		vec3 halfNormal = normalize(-lightDirN+viewDir);

		float spec = pow(max(dot(halfNormal, norm), 0.0), 32);
		vec3 specular = specularStrength * spec * lightBlock.lightColor[i].rgb;

		ambient+=(diffuse+specular)*objectColor;
	}
	//ambient=0.05*lightBlock.lightColor[0].rgb;

	vec3 lightDirN=normalize(lightBlock.lightDir[0].xyz);
	vec3 norm=normalize(fs_in.Normal);
	vec3 diffuse = lightBlock.lightColor[0].rgb*max(dot(norm, lightDirN), 0.0);
	 	
	float specularStrength=0.5;
	vec3 viewDir=normalize(viewPos-fs_in.FragPos);
	vec3 reflectDir = reflect(-lightDirN, norm);
	vec3 halfNormal = normalize(lightDirN+viewDir);

	float spec = pow(max(dot(halfNormal, norm), 0.0), 32);
	vec3 specular = specularStrength * spec * lightBlock.lightColor[0].rgb;

	float shadow=ShadowCalculation(fs_in.FragPosLightSpace);

	vec3 result=pow((ambient+(1-shadow)*(diffuse+specular))*objectColor,vec3(1/2.2));
	FragColor=vec4(result,1.0);
}