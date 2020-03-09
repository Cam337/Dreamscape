#include "BezierCurve.h"

BezierCurve::BezierCurve(std::vector<glm::vec4> controlPoints)
{
	//std::cerr << "Bezier Curve Constructor" << std::endl;
	p0 = controlPoints[0];
	p1 = controlPoints[1];
	p2 = controlPoints[2];
	p3 = controlPoints[3];

	controlPointMat = glm::mat4(p0,p1,p2,p3);
	bernsteinMat = glm::mat4(
		glm::vec4(-1.0f, 3.0f, -3.0f, 1.0f), glm::vec4(3.0f, -6.0f, 3.0f, 0.0f),
		glm::vec4(-3.0f, 3.0f, 0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

	calculateBezierCurve();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

BezierCurve::~BezierCurve()
{

}

void BezierCurve::draw(GLuint shaderProgram)
{
	//std::cerr << "Draw BezierCurve: " << vertexData.size() << std::endl;
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, points.size());
	//glDrawElements(GL_LINES, 2*indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void BezierCurve::update(glm::mat4 transform)
{
	// need to redraw the curve when a point is updated
	updatePoints();

	calculateBezierCurve();

	updateBufferObjects();
}

void BezierCurve::updatePoints()
{
	p0 = glm::vec4(anchorPoints[0]->getPoint(), 1.0f);
	p3 = glm::vec4(anchorPoints[1]->getPoint(), 1.0f);
	p1 = glm::vec4(tangentPoints[0]->getPoint(), 1.0f);
	p2 = glm::vec4(tangentPoints[1]->getPoint(), 1.0f);

	//controlPointMat = glm::mat4(p0, p1, p2, p3);
}

void BezierCurve::calculateBezierCurve()
{
	points.clear();
	for (float t = 0; t < 0.999; t += 1.0 / 150.0)
	{
		glm::vec3 x_t = getPoint(t);
		points.push_back(x_t);
		//std::cerr << "At t=" << t << ", x_t=" << x_t.x << " " << x_t.y << " " << x_t.z << " " <<std::endl;
	}
	//std::cerr << vertexData.size() << " points created! is it 150?" << std::endl;
}

void BezierCurve::updateBufferObjects()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * points.size(), points.data());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

glm::vec3 BezierCurve::getPoint(float t)
{
	glm::vec4 T(pow(t, 3.0f), pow(t, 2.0f), t, 1.0f);
	glm::vec3 x_t = controlPointMat * bernsteinMat * T;
	return x_t;
}

std::vector<glm::vec3> BezierCurve::getPoints()
{
	return points;
}

void BezierCurve::addAnchorPoint(AnchorPoint* anchorPoint)
{
	anchorPoints.push_back(anchorPoint);
}

void BezierCurve::addTangentPoint(TangentPoint* tangentPoint)
{
	tangentPoints.push_back(tangentPoint);
}