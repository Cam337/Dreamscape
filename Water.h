#ifndef _WATER_H_
#define _WATER_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class Water
{
private:
	glm::mat4 model;
	float xPos, zPos, height;
	GLuint vao, vbo, ebo;

public:
	const float TILE_SIZE = 100.0f;

	Water(float x, float z, float h);
	~Water();

	void draw(GLuint shaderProgram);

	glm::mat4 getModel();

	float getHeight() {
		return height;
	}

	float getXpos() {
		return xPos;
	}

	float getZpos() {
		return zPos;
	}
};


#endif
