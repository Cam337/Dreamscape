#ifndef _LOADER_H_
#define	_LOADER_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include "RawModel.h"

class Loader
{
private:

	std::vector<GLuint> vaos; // static?
	std::vector<GLuint> vbos; // static?

	int createVAO();
	void storeDataInAttributeList(int attributeNumber, std::vector<glm::vec3> data);
	void unbindVAO();
	void bindIndicesBuffer(std::vector<glm::ivec3> indices);
public:
	RawModel* loadToVAO(std::vector<glm::vec3> positions, std::vector<glm::ivec3> indices);
	void cleanUp();
};

#endif