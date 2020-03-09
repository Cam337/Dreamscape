#ifndef _BEZIERCURVE_H_
#define _BEZIERCURVE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <math.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Geometry.h"
#include "AnchorPoint.h"
#include "TangentPoint.h"
#include "ControlHandle.h"


class BezierCurve //: public Geometry
{
private:
	std::vector<glm::vec3> points;
	std::vector<glm::ivec2> indices;
	GLuint VAO, VBO;

	glm::vec4 p0, p1, p2, p3;
	glm::mat4 controlPointMat;
	glm::mat4 bernsteinMat;

	std::vector<AnchorPoint*> anchorPoints;
	std::vector<TangentPoint*> tangentPoints;

public:
	BezierCurve(std::vector<glm::vec4> controlPoints);
	~BezierCurve();

	void draw(GLuint shaderProgram);
	void update(glm::mat4 transform);
	
	void updatePoints();
	void calculateBezierCurve();
	void updateBufferObjects();
	
	
	glm::vec3 getPoint(float t);
	std::vector<glm::vec3> getPoints();
	//std::vector<glm::vec3> getVertexData();

	void addAnchorPoint(AnchorPoint* anchorPoint);
	void addTangentPoint(TangentPoint* tangentPoint);


};
#endif
