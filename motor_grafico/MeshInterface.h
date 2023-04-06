#ifndef MESH_INTERFACE_H
#define MESH_INTERFACE_H

#include <string>
#include <vector>

#ifdef __APPLE__
#include <glad/glad.h>
#else
#include "GL/glew.h"
#endif

#include <GLFW/glfw3.h>
#include "Texture2D.h"
#include "glm/glm.hpp"

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoords;
};

class MeshInterface
{
    public:
		//Abstract methods		
    	virtual bool load(const std::string& filename) = 0;
		virtual const char* getName() const= 0;

		void configure(const std::string& textureName, glm::vec3 &position, glm::vec3 &scaling);		
	    void draw();		
		Texture2D &getTexture() { return mTexture; }
		glm::vec3 &getPosition() { return mPosition; }
		glm::vec3 &getScaling() { return mScaling; }

	protected:
		std::vector<Vertex> mVertices;
		GLuint mVBO, mVAO;			
		void initBuffers();
		Texture2D mTexture;
		glm::vec3 mPosition;
		glm::vec3 mScaling;

	private:
		bool initialized = false;
};

#endif
