#include "Loader.h"

int Loader::createVAO()
{
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	vaos.push_back(vaoID);
	glBindVertexArray(vaoID);
	return vaoID;
}

void Loader::storeDataInAttributeList(int attributeNumber, std::vector<glm::vec3> data)
{
	GLuint vboID;
	glGenBuffers(1, &vboID);
	vbos.push_back(vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * data.size(), data.data(), GL_STATIC_DRAW);
	//glEnableVertexAttribArray(attributeNumber); // do i need this??
	glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::unbindVAO()
{
	glBindVertexArray(0);
}

void Loader::bindIndicesBuffer(std::vector<glm::ivec3> indices)
{
	GLuint vboID;
	glGenBuffers(1, &vboID);
	vbos.push_back(vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

RawModel* Loader::loadToVAO(std::vector<glm::vec3> positions, std::vector<glm::ivec3> indices)
{
	GLuint vaoID = createVAO();
	bindIndicesBuffer(indices);
	storeDataInAttributeList(0, positions);
	unbindVAO();
	return new RawModel(vaoID, indices.size()*3);
}

void Loader::cleanUp()
{
	for (GLuint vao : vaos) 
	{
		glDeleteVertexArrays(1, &vao);
	}

	for (GLuint vbo : vbos)
	{
		glDeleteBuffers(1, &vbo);
	}
}
