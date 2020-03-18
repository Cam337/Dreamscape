#ifndef _RAW_MODEL_H_
#define _RAW_MODEL_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

class RawModel
{
private:
	GLuint vaoID;
	int vertexCount;
public:
	RawModel(GLuint vaoID, int vertexCount);
	GLuint getVaoID();
	int getVertexCount();
};

#endif
