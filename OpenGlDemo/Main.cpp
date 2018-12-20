#include <stdio.h>
#include <stdlib.h>
#include <GLEW\GL\glew.h>
#include <GLFW\glfw3.h>
#include "shader.hpp"
#include "Window.h"
#include "Object.h"
#include "Camera.h"
#include <assimp/config.h>
#include "texture.h"
#include <vector>
#include <string>
#include "stb_image.h"
#include <iostream>

const int width = 1920;
const int height = 1080;

unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void drawSkybox(GLuint skyboxShader, Object &cube, GLuint cubemapTexture, Camera &cam) {
	glDisable(GL_CULL_FACE);
	glUseProgram(skyboxShader);
	glDepthMask(GL_FALSE);

	auto P = cam.projMatrix(width / height);

	glm::mat4 V = glm::mat4(glm::mat3(cam.viewMatrix()));

	GLuint viewID = glGetUniformLocation(skyboxShader, "view");
	GLuint projectionID = glGetUniformLocation(skyboxShader, "projection");

	glUniformMatrix4fv(viewID, 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &P[0][0]);

	glBindVertexArray(cube.vertexArrayObject);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawElements(GL_TRIANGLES, cube.elementArray.size(), GL_UNSIGNED_SHORT, (void*)0);
	glDepthMask(GL_TRUE);	
	glEnable(GL_CULL_FACE);
}

void drawObject(GLuint shader, Camera &cam, Object &obj, Texture &tex, glm::vec3 lightDir) {
	glUseProgram(shader);

	glBindTexture(GL_TEXTURE_2D, tex.textureid);

	glBindVertexArray(obj.vertexArrayObject);

	auto M = obj.modelMatrix();
	auto V = cam.viewMatrix();
	auto P = cam.projMatrix(width / height);

	glm::mat3 nM = glm::mat3(glm::transpose(glm::inverse(M))); 

	GLuint modelID = glGetUniformLocation(shader, "model");
	GLuint viewID = glGetUniformLocation(shader, "view");
	GLuint projectionID = glGetUniformLocation(shader, "projection");
	GLuint normalMatID = glGetUniformLocation(shader, "norMat");

	GLuint lightDirID = glGetUniformLocation(shader, "lightDir");
	GLuint cameraPosID = glGetUniformLocation(shader, "cameraPos");


	glUniformMatrix4fv(modelID, 1, GL_FALSE, &M[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &P[0][0]);

	glUniformMatrix3fv(normalMatID, 1, GL_FALSE, &nM[0][0]);

	glUniform3f(lightDirID, lightDir.x, lightDir.y, lightDir.z);
	glUniform3f(cameraPosID, cam.position.x, cam.position.y, cam.position.z);

	glDrawElements(GL_TRIANGLES, obj.elementArray.size(), GL_UNSIGNED_SHORT, (void*)0);
}

int main(void)
{
	GLFWwindow* window = InitWindow(width, height);

	if (window == NULL) {
		return -1;
	}

	glClearColor(0.2f, 0.0f, 0.0f, 1.0f);

	std::vector<std::string> faces
	{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};

	unsigned int cubemapTexture = loadCubemap(faces);

	Texture gif[15] = {
		Texture("Gif/0.jpg"),
		Texture("Gif/1.jpg"),
		Texture("Gif/2.jpg"),
		Texture("Gif/3.jpg"),
		Texture("Gif/4.jpg"),
		Texture("Gif/5.jpg"),
		Texture("Gif/6.jpg"),
		Texture("Gif/7.jpg"),
		Texture("Gif/8.jpg"),
		Texture("Gif/9.jpg"),
		Texture("Gif/10.jpg"),
		Texture("Gif/11.jpg"),
		Texture("Gif/12.jpg"),
		Texture("Gif/13.jpg"),
		Texture("Gif/14.jpg")
	};

	Texture goldTex("gold.jpg");

	glm::vec3 haloOffset = glm::vec3( -0.4, 2.5, 0 ) - glm::vec3(0, 0.9, 0);

	Object* halos[4];

	Object bunny2 = LoadObjectFromFile("bunny.obj");
	Object halo2 = LoadObjectFromFile("torus.obj");
	bunny2.position = { 5, -0.1, 0 };
	bunny2.rotation = { 0, glm::radians(-90.0f), 0 };
	halo2.parent = &bunny2;
	halo2.position = haloOffset;
	halos[0] = &halo2;

	Object bunny3 = LoadObjectFromFile("bunny.obj");
	Object halo3 = LoadObjectFromFile("torus.obj");
	bunny3.position = { -5, -0.1, 0 };
	bunny3.rotation = { 0, glm::radians(90.0f), 0 };
	halo3.parent = &bunny3;
	halo3.position = haloOffset;
	halos[1] = &halo3;

	Object bunny4 = LoadObjectFromFile("bunny.obj");
	Object halo4 = LoadObjectFromFile("torus.obj");
	bunny4.position = { 0, -0.1, 5 };
	bunny4.rotation = { 0, glm::radians(180.0f), 0 };
	halo4.parent = &bunny4;
	halo4.position = haloOffset;
	halos[2] = &halo4;

	Object bunny5 = LoadObjectFromFile("bunny.obj");
	Object halo5 = LoadObjectFromFile("torus.obj");
	bunny5.position = { 0, -0.1, -5 };
	bunny5.rotation = { 0, glm::radians(0.0f), 0 };
	halo5.parent = &bunny5;
	halo5.position = haloOffset;
	halos[3] = &halo5;

	Object cineCube = LoadObjectFromFile("cube2.obj");

	Object skybox = LoadObjectFromFile("cube3.obj");

	cineCube.position = { 0, 1, 0 };

	Camera camera;

	camera.position = { 6, 6, 6 };

	glm::vec3 lightDirection = { 1, -1, 0 };

	GLuint shaders = LoadShaders("VertexShader.glsl", "FragmentShader.glsl");
	GLuint skyboxShaders = LoadShaders("SkyboxVertex.glsl", "SkyboxFragment.glsl");

	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LEQUAL);

	bool goingUp = true;

	int i = 0;

	double lastFrameChange = glfwGetTime();

	do
	{
		double time = glfwGetTime();

		if (goingUp) {
			for (size_t i = 0; i < 4; i++)
			{
				halos[i]->position.y += 0.00075f;
			}

			if (halo2.position.y > 1.7) {
				goingUp = false;
			}
		}
		else {
			for (size_t i = 0; i < 4; i++)
			{
				halos[i]->position.y -= 0.00075f;
			}

			if (halo2.position.y < 1.5) {
				goingUp = true;
			}
		}

		lightDirection = {
			sin(2 * -time),
			-1,
			cos(2 * -time)
		};

		camera.position = {
			12 * sin(time/-3),
			1,
			12 * cos(time/-3)
		};

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		drawSkybox(skyboxShaders, skybox, cubemapTexture, camera);

		drawObject(shaders, camera, bunny2, goldTex, lightDirection);
		drawObject(shaders, camera, halo2, goldTex, lightDirection);
		drawObject(shaders, camera, bunny3, goldTex, lightDirection);
		drawObject(shaders, camera, halo3, goldTex, lightDirection);
		drawObject(shaders, camera, bunny4, goldTex, lightDirection);
		drawObject(shaders, camera, halo4, goldTex, lightDirection);
		drawObject(shaders, camera, bunny5, goldTex, lightDirection);
		drawObject(shaders, camera, halo5, goldTex, lightDirection);

		drawObject(shaders, camera, cineCube, gif[i], lightDirection);

		if (lastFrameChange + 0.1f < time) {
			i++;
			lastFrameChange = time;
		}
		if (i > 14) {
			i = 0;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	glDeleteProgram(shaders);

	glfwTerminate();
}