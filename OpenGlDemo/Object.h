#pragma once
#include <GLEW\GL\glew.h>
#include <GLFW\glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <string>

class Object {
public:
	std::vector<GLfloat> vertexArray;
	std::vector<GLfloat> uvArray;
	std::vector<unsigned short> elementArray;
	std::vector<GLfloat> normalArray;

	GLuint vertexArrayObject;
	GLuint vertexPositionBuffer;
	GLuint vertexUVBuffer;
	GLuint elementBuffer;
	GLuint vertexNormalBuffer;

	Object* parent;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::mat4 modelMatrix();
};

Object CreateObject();

Object LoadObjectFromFile(std::string filename);