#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#ifdef __APPLE__
#include <glad/glad.h>
#else
#include "GL/glew.h"
#endif

#include <GLFW/glfw3.h>

#include <string>
using std::string;

class Texture2D
{
public:
	Texture2D();
	virtual ~Texture2D();

	bool loadTexture(const string& fileName, bool generateMipMaps = true);
	void bind();

private:
	GLuint mTexture;
};
#endif //TEXTURE2D_H
