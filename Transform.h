#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <list>

#include "Node.h"

class Transform : public Node 
{
private:
	glm::mat4 transformMat;
	std::list<Node*> childNodes;
public:
	Transform(glm::mat4 mat);
	~Transform();
	void addChild(Node* child);
	void draw(GLuint shaderProgram, glm::mat4 C);
	void update(glm::mat4 transform);

	void translate(glm::vec3 translation);
	void rotate(float deg, glm::vec3 rotationAxis);
	void scale(float scaleFactor);
};

#endif