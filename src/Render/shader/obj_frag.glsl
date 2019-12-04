#version 330 core

out vec4 FragColor;

uniform vec3 lightDir;
uniform vec3 viewPos;
uniform sampler2D tex;

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

vec3 lightColor = vec3(1.0f);
//vec3 objectColor= vec3(1.0f, 0.5f, 0.31f);


void main()
{	
	vec3 objectColor=vec3(texture(tex,TexCoord));
	float ambientStrenth=0.1;
	vec3 ambient= ambientStrenth*lightColor;

	vec3 lightDirN=normalize(lightDir);
	vec3 norm=normalize(Normal);
	vec3 diffuse = lightColor*max(dot(norm, lightDirN), 0.0);
	 	
	float specularStrength=0.5;
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 reflectDir = reflect(-lightDirN, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result=(ambient+diffuse+specular)*objectColor;
	FragColor=vec4(result,1.0);
}