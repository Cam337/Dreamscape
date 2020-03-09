#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "Object.h"

class Skybox
{
private:
	GLuint vao;
	GLuint vbos[2];
	GLuint cubemapTexture;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
public:
	Skybox(glm::mat4 view, glm::mat4 projection);
	~Skybox();

	void draw(GLuint shader);
	void update();

	void spin(float deg);
	GLuint loadCubemap(std::vector<std::string> faces);
	GLuint getCubemap();
};

#endif

