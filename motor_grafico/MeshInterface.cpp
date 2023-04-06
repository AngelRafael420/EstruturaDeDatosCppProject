#include "MeshInterface.h"
#ifdef __APPLE__
#include <glad/glad.h>
#else
#include "GL/glew.h"
#endif

#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

void MeshInterface::draw()
{
	assert(initialized);
	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
	glBindVertexArray(0);
}

void MeshInterface::initBuffers()
{
	assert(initialized);
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (GLvoid*)0);

	// Vertex Texture Coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));

	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);
}

void MeshInterface::configure(const std::string& textureName, glm::vec3 &position, glm::vec3 &scaling)
{
	if (!mTexture.loadTexture(textureName))
	{
		std::cerr << "Cannot load texture: " << textureName << std::endl;
	}
	mPosition = position;
	mScaling = scaling;
	initialized = true;
}

