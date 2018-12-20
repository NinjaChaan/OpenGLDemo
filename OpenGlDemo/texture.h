#pragma once
#include <GLEW\GL\glew.h>
#include <GLFW\glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <string>

class Texture {
public:
	GLuint textureid;
	int width, height;
	int colorChannels;

	Texture(std::string filepath);
};