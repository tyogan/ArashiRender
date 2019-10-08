#version 330 core

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 TexCoord;
in vec3 FragPos;

vec3 lightColor = vec3(1.f);
vec3 objectColor= vec3(1.0f, 0.5f, 0.31f);


void main()
{	
	float ambientStrenth=0.1;
	vec3 ambient= ambientStrenth*lightColor;

	vec3 norm=normalize(Normal);
	vec3 lightDir = normalize(lightPos-FragPos);
	vec3 diffuse = lightColor*max(dot(norm, lightDir), 0.0);
	 	
	float specularStrength=0.5;
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result=(ambient+diffuse+specular)*objectColor;
	FragColor=vec4(result,1.0);
}