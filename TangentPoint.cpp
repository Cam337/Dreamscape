#include "TangentPoint.h"

TangentPoint::TangentPoint(glm::vec3 p) : point(p)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &point, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

TangentPoint::~TangentPoint() 
{

}

void TangentPoint::draw(GLuint shaderProgram)
{
	glm::vec3 color(0.0f, 1.0f, 0.0f);
	glBindVertexArray(VAO);
	GLuint colorLoc = glGetUniformLocation(shaderProgram, "color");
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));
	glPointSize(5.0f);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

void TangentPoint::update(glm::mat4 transform)
{
	updateBufferObjects();
}

void TangentPoint::updateBufferObjects()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3), &point);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TangentPoint::setPoint(glm::vec3 p)
{
	point = p;
}

glm::vec3 TangentPoint::getPoint()
{
	return point;
}