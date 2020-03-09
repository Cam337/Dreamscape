#include "ControlHandle.h"

ControlHandle::ControlHandle(AnchorPoint* ap, TangentPoint* tp1, TangentPoint* tp2) : 
	anchorPoint(ap), tangentPoint1(tp1), tangentPoint2(tp2)
{
	/*
	glm::vec3 anchor = anchorPoint->getPoint();
	glm::vec3 tangent1 = tangentPoint1->getPoint();
	glm::vec3 tangent2 = tangentPoint2->getPoint();
	anchorPoint->setPoint(tangent2-anchor+tangent1);
	tangentPoint1->setPoint(2.0f*anchor-tangent2);
	tangentPoint2->setPoint(2.0f*anchor-tangent1);
	vertexData.push_back(tangentPoint1->getPoint());
	vertexData.push_back(tangentPoint2->getPoint());
	*/
	vertexData.push_back(tangentPoint1->getPoint());
	vertexData.push_back(tangentPoint2->getPoint());

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexData.size(), vertexData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

ControlHandle::~ControlHandle()
{

}

void ControlHandle::draw(GLuint shaderProgram)
{
	glm::vec3 color(1.0f, 1.0f, 0.0f);
	glBindVertexArray(VAO);
	GLuint colorLoc = glGetUniformLocation(shaderProgram, "color");
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));
	glDrawArrays(GL_LINE_STRIP, 0, vertexData.size());
	glBindVertexArray(0);
}

void ControlHandle::update(glm::mat4 transform)
{
	updateVertexData();
	updateBufferObjects();
}

void ControlHandle::updateVertexData()
{
	vertexData.clear();
}
void ControlHandle::updateBufferObjects()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * vertexData.size(), vertexData.data());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}