#version 330 core

out vec3 color;

in vec2 uv;
in vec3 normal;
in vec3 fragPos;

uniform sampler2D tex;
uniform vec3 lightDir;
uniform vec3 cameraPos;

void main(){
	vec3 texCol = texture(tex, uv).rgb;
	vec3 lightCol = vec3(1, 1, 1);

	//AMBIENT
	float ambientStrength = 0.3;
	vec3 ambient = lightCol * ambientStrength;

	//DIFFUSE
	vec3 n = normalize(normal);
	vec3 ld = -normalize(lightDir);
	float dif = max(dot(n, ld), 0);
	vec3 diffuse = lightCol * dif;

	//SPECULAR
	float specularStrength = 2;
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-ld, n);
	float spec = pow(max(dot(viewDir, reflectDir), 0), 16);
	vec3 specular = specularStrength * spec * lightCol;


	color = (ambient + diffuse + specular) * texCol;
}