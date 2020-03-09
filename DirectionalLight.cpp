#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 c, glm::vec3 d) :
	color(c), direction(d)
{

}

DirectionalLight::~DirectionalLight()
{
	
}

void DirectionalLight::update()
{
	rotate(0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
}

void DirectionalLight::rotate(float deg, glm::vec3 rotationAxis)
{
	direction = glm::rotate(glm::mat4(1.0f), glm::radians(deg), rotationAxis) * glm::vec4(direction, 1.0f);
}

glm::vec3 DirectionalLight::getColor()
{
	return color;
}

glm::vec3 DirectionalLight::getDirection()
{
	return direction;
}
