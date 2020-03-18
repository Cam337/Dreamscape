#include "Water.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


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
	model = glm::mat4(1.0f);
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

	// Load dudvMap
	glGenTextures(1, &dudvTexture);
	glBindTexture(GL_TEXTURE_2D, dudvTexture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	std::string fileName = "resources/textures/waterDUDV.png";
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// Load dudvMap
	glGenTextures(1, &normalMapTexture);
	glBindTexture(GL_TEXTURE_2D, normalMapTexture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	fileName = "resources/textures/normalMap.png";
	data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

Water::~Water()
{
}

void Water::draw()
{
	glBindVertexArray(vao);

	// Bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflectionTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refractionTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, dudvTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, normalMapTexture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, refractionDepthTexture);

	// Enable alpha blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Render water
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	glEnable(GL_BLEND);
	glBindVertexArray(0);
}

glm::mat4 Water::getModel()
{
	return model;
}
