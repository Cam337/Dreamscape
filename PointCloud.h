#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

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

#include "Object.h"

class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> vertexBuffer;
	GLuint vao, vbo;
	GLfloat pointSize;
	glm::vec3 rotationAxis;
	glm::mat4 translationMat;
	glm::mat4 scaleMat;
	glm::mat4 rotationMat;
public:
	PointCloud(std::string objFilename, GLfloat pointSize, glm::vec3 rotationAxis);
	~PointCloud();

	void parseOBJFile(std::string objFilename);

	void draw();
	void update();

	void updatePointSize(GLfloat size);
	void spin(float deg, glm::vec3 rotationAxis);
	void resetOrientation();

	void translate(glm::vec3 translation);
	void resetPosition();

	void scale(float scaleFactor);
	void resetScale();

	std::vector<glm::vec3> getPoints();
	std::vector<glm::vec3> getNormals();
	GLfloat getPointSize();

	
};

#endif

