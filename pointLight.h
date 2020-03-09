#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

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

#include "Object.h"

class PointLight : public Object
{
private:
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::ivec3> faces;
	std::vector<glm::vec3> vertexBuffer;
	GLuint vao, vbo, ebo;
	GLfloat pointSize;
	glm::vec3 rotationAxis;
	glm::mat4 normalizeMat;
	glm::mat4 translationMat;
	glm::mat4 scaleMat;
	glm::mat4 spinMat;
	glm::mat4 rotationMat;

	// Light properties
	glm::vec3 lightColor;
	glm::vec3 lightPosition;
	
public:
	PointLight(std::string objFilename, GLfloat pointSize, glm::vec3 rotationAxis);
	~PointLight();

	void parseOBJFile(std::string objFilename);
	void parseOBJFile2(std::string objFilename);

	void draw();
	void update();

	void updatePointSize(GLfloat size);
	void spin(float deg, glm::vec3 rotationAxis);
	void rotate(float deg, glm::vec3 rotationAxis);
	void rotateModel(float deg, glm::vec3 rotationAxis);
	void resetOrientation();

	void translate(glm::vec3 translation);
	void resetPosition();

	void scale(float scaleFactor);
	void resetScale();

	// Setters
	void setLightColor(glm::vec3 color);

	// Getters
	std::vector<glm::vec3> getPoints();
	std::vector<glm::vec3> getNormals();
	GLfloat getPointSize();
	glm::vec3 getLightColor();
	glm::vec3 getLightPosition();
	
};

#endif

