#include "Transform.h"

Transform::Transform(glm::mat4 mat) : transformMat(mat)
{
	// anything else to do in the constructor?

}

Transform::~Transform()
{
	
}

void Transform::draw(GLuint shaderProgram, glm::mat4 C) 
{
	glm::mat4 newMat = C * transformMat;

	for (Node* child : childNodes) {
		child->draw(shaderProgram, newMat);
	}
}

void Transform::update(glm::mat4 transform)
{
	transformMat *= transform;
}

void Transform::translate(glm::vec3 translation)
{
	transformMat *= glm::translate(translation);
}

void Transform::rotate(float deg, glm::vec3 rotationAxis)
{
	transformMat = glm::rotate(glm::mat4(1.0f), glm::radians(deg), rotationAxis) * transformMat;
}

void Transform::scale(float scaleFactor)
{
	transformMat = glm::scale(transformMat, glm::vec3(scaleFactor));
}

void Transform::addChild(Node* child)
{
	childNodes.push_back(child);
}