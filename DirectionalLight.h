#ifndef _DIRECTIONAL_LIGHT_H_
#define _DIRECTIONAL_LIGHT_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class DirectionalLight
{
private:
	glm::vec3 color;
	glm::vec3 direction;
public:
	DirectionalLight(glm::vec3 c, glm::vec3 d);
	~DirectionalLight();

	void update();
	void rotate(float deg, glm::vec3 rotationAxis);

	glm::vec3 getColor();
	glm::vec3 getDirection();

};
#endif
