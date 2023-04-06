#ifndef MESH_H
#define MESH_H

#include <vector>
#ifdef __APPLE__
#include <glad/glad.h>
#else
#include "GL/glew.h"
#endif
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "MeshInterface.h"


struct Vertex;

class Mesh: public MeshInterface
{
public:

	 Mesh();
	~Mesh();

	virtual bool load(const std::string& filename);
	virtual const char* getName() const;

private:
	bool mLoaded;
};
#endif //MESH_H
