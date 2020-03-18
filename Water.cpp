#include "Water.h"

Water::Water(float x, float z, float h) :
	xPos(x), zPos(z), height(h)
{
	/*
	std::vector<glm::vec3> vertexBuffer
	{
		glm::vec3(x - TILE_SIZE, h, z - TILE_SIZE),
		glm::vec3(x + TILE_SIZE, h, z - TILE_SIZE),
		glm::vec3(x - TILE_SIZE, h, z + TILE_SIZE),
		glm::vec3(x + TILE_SIZE, h, z + TILE_SIZE),
	};
	*/
	model = glm::scale(glm::vec3(TILE_SIZE));

	std::vector<glm::vec3> vertexBuffer = 
	{
		glm::vec3(-1, 0, -1),
		glm::vec3(1, 0, -1),
		glm::vec3(-1, 0, 1),
		glm::vec3(1, 0, 1)
	};

	std::vector<glm::ivec3> indexBuffer =
	{
		glm::ivec3(0, 1, 2),
		glm::ivec3(1, 3, 2)
	};

	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	// Bind to the VAO.
	// This tells OpenGL which data it should be paying attention to
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexBuffer.size(),
		vertexBuffer.data(), GL_STATIC_DRAW);

	// Bind to the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	// Pass in the data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indexBuffer.size(), indexBuffer.data(), GL_STATIC_DRAW);

	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Initialize Frame Buffer Objects
	initialiseReflectionFrameBuffer();
	initialiseRefractionFrameBuffer();
}

Water::~Water()
{
}

void Water::draw()
{
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflectionTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refractionTexture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

glm::mat4 Water::getModel()
{
	return model;
}
