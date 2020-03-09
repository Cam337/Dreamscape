#ifndef _TRIANGLE_MESH_H_
#define _TRIANGLE_MESH_H_

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

class TriangleMesh : public Object
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

	// Material properties
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
public:
	TriangleMesh(std::string objFilename, GLfloat pointSize, glm::vec3 rotationAxis);
	~TriangleMesh();

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
	void setAmbient(glm::vec3 color);
	void setDiffuse(glm::vec3 color);
	void setSpecular(glm::vec3 color);
	void setShininess(float shine);

	// Getters
	std::vector<glm::vec3> getPoints();
	std::vector<glm::vec3> getNormals();
	GLfloat getPointSize();
	glm::vec3 getAmbient();
	glm::vec3 getDiffuse();
	glm::vec3 getSpecular();
	float getShininess();
	
};

#endif

