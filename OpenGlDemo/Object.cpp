#include "Object.h"
#include <glm/gtx/transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

glm::mat4 Object::modelMatrix() {
	glm::mat4 translation = glm::translate(glm::mat4(1), position);
	glm::mat4 rot = glm::toMat4(glm::quat(rotation));
	if (parent == nullptr) {
		return translation * rot * glm::mat4(1);
	}
	else {
		return parent->modelMatrix() * translation * rot * glm::mat4(1);
	}
}

GLuint CreateAndBindBuffer(std::vector<GLfloat> &data) {
	GLuint bufferId;

	glGenBuffers(1, &bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), &data[0], GL_STATIC_DRAW);

	return bufferId;
}

Object CreateObject() {
	Object newObject;

	newObject.position = { 0, 0, 0 };

	newObject.vertexArray = {
		-1, -1, 0,
		1, -1, 0,
		-1, 1, 0,
		1, 1, 0
	};

	newObject.uvArray = {
		0, 1,
		1, 1,
		0, 0,
		1, 0
	};

	newObject.elementArray = {
		0, 1, 2, 
		3, 2, 1
	};

	glGenVertexArrays(1, &newObject.vertexArrayObject);
	glBindVertexArray(newObject.vertexArrayObject);

	newObject.vertexPositionBuffer = CreateAndBindBuffer(newObject.vertexArray);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(0);

	newObject.vertexUVBuffer = CreateAndBindBuffer(newObject.uvArray);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);

	//uv tähän, channel 1

	glGenBuffers(1, &newObject.elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newObject.elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, newObject.elementArray.size() * sizeof(unsigned short), &newObject.elementArray[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	return newObject;
}

Object LoadObjectFromFile(std::string filename)
{
	Object newObject;

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast);

	aiMesh* mesh = scene->mMeshes[0];

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		aiVector3D position = mesh->mVertices[i];
		aiVector3D uvCoord = mesh->mTextureCoords[0][i];
		aiVector3D normals = mesh->mNormals[i];

		newObject.vertexArray.push_back(position.x);
		newObject.vertexArray.push_back(position.y);
		newObject.vertexArray.push_back(position.z);

		newObject.uvArray.push_back(uvCoord.x);
		newObject.uvArray.push_back(uvCoord.y);

		newObject.normalArray.push_back(normals.x);
		newObject.normalArray.push_back(normals.y);
		newObject.normalArray.push_back(normals.z);
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		newObject.elementArray.push_back(face.mIndices[0]);
		newObject.elementArray.push_back(face.mIndices[1]);
		newObject.elementArray.push_back(face.mIndices[2]);
	}

	glGenVertexArrays(1, &newObject.vertexArrayObject);
	glBindVertexArray(newObject.vertexArrayObject);

	newObject.vertexPositionBuffer = CreateAndBindBuffer(newObject.vertexArray);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(0);

	newObject.vertexUVBuffer = CreateAndBindBuffer(newObject.uvArray);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);

	newObject.vertexNormalBuffer = CreateAndBindBuffer(newObject.normalArray);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);

	//uv tähän, channel 1

	glGenBuffers(1, &newObject.elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newObject.elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, newObject.elementArray.size() * sizeof(unsigned short), &newObject.elementArray[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	return newObject;
}
