#ifndef _CONTROLHANDLE_H_
#define _CONTROLHANDLE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Geometry.h"
#include "AnchorPoint.h"
#include "TangentPoint.h"

class ControlHandle : public Geometry
{
private:
	std::vector<glm::vec3> vertexData;
	GLuint VAO, VBO;

	AnchorPoint* anchorPoint;
	TangentPoint* tangentPoint1;
	TangentPoint* tangentPoint2;

public:
	ControlHandle(AnchorPoint* ap, TangentPoint* tp1, TangentPoint* tp2);
	~ControlHandle();

	void draw(GLuint shaderProgram);
	void update(glm::mat4 transform);

	void updateVertexData();
	void updateBufferObjects();
};
#endif