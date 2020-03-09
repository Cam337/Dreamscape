#ifndef _ANCHORPOINT_H_
#define _ANCHORPOINT_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Geometry.h"

class AnchorPoint : public Geometry
{
private:
	GLuint VAO, VBO;

	glm::vec3 point;

public:
	AnchorPoint(glm::vec3 p);
	~AnchorPoint();

	void draw(GLuint shaderProgram);
	void update(glm::mat4 transform);

	void updateBufferObjects();

	void setPoint(glm::vec3 p);
	glm::vec3 getPoint();
};
#endif
