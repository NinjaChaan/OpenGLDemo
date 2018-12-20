#version 330 core

layout(location=0) in vec3 vertexPosition_modelspace;
layout(location=1) in vec2 uvcoords;
layout(location=2) in vec3 normals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 norMat;

out vec2 uv;
out vec3 normal;
out vec3 fragPos;

void main(){
	gl_Position = projection * view * model * vec4(vertexPosition_modelspace, 1);

	fragPos = vec3(model * vec4(vertexPosition_modelspace, 1));

	uv = uvcoords;
	normal =  norMat * normals;
}