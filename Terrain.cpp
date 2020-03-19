#include "Terrain.h"

Terrain::Terrain(int tl, int tr, int bl, int br)
{
	for (int i = 0; i < vertexCount; i++)
	{
		for (int j = 0; j < vertexCount; j++)
		{
			grid[i][j] = 0;
		}
		
	}

	int gridLength = vertexCount - 1;

	grid[0][0] = tl;
	grid[gridLength][0] = tr;
	grid[0][gridLength] = bl;
	grid[gridLength][gridLength] = br;

	
	diamondSquare(grid, gridLength);

	/*
	for (int i = 0; i < vertexCount; i++)
	{
		for (int j = 0; j < vertexCount; j++)
		{
			std::cerr << "| " << grid[j][i] << " | ";
		}
		std::cerr << std::endl;
	}
	*/

	createTexture();
	normalizeGrid();
	createVertexBufferData();
	createIndices();
	
	for (int i = 0; i < vertices.size(); i++)
	{
		vertexBufferData.push_back(vertices[i].x);
		vertexBufferData.push_back(vertices[i].y);
		vertexBufferData.push_back(vertices[i].z);
		vertexBufferData.push_back(normals[i].x);
		vertexBufferData.push_back(normals[i].y);
		vertexBufferData.push_back(normals[i].z);
		vertexBufferData.push_back(textureCoords[i].x);
		vertexBufferData.push_back(textureCoords[i].y); // tex origin is in top left, 1 -y?

		//std::cerr << "Vertex: " << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
		//std::cerr << "Texture Coords: " << textureCoords[i].x << " " << textureCoords[i].y << std::endl;
	}

	createVAO();
	
}

Terrain::~Terrain()
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteTextures(1, &texture);
}

void Terrain::diamondSquare(float grid[CHUNK_X][CHUNK_Z], int stepSize)
{
	//std::cerr << "diamondSquare : stepSize " << stepSize << std::endl;
	int half = stepSize / 2;
	if (half < 1)
		return;
	//square steps
	for (int z = half; z < CHUNK_Z; z += stepSize)
		for (int x = half; x < CHUNK_X; x += stepSize)
			squareStep(grid, x % CHUNK_X, z % CHUNK_Z, half);
	// diamond steps
	int col = 0;
	for (int x = 0; x < CHUNK_X; x += half)
	{
		col++;
		//If this is an odd column.
		if (col % 2 == 1)
			for (int z = half; z < CHUNK_Z; z += stepSize)
				diamondStep(grid, x % CHUNK_X, z % CHUNK_Z, half);
		else
			for (int z = 0; z < CHUNK_Z; z += stepSize)
				diamondStep(grid, x % CHUNK_X, z % CHUNK_Z, half);
	}
	diamondSquare(grid, stepSize / 2);
}

void Terrain::squareStep(float grid[CHUNK_X][CHUNK_Z], int x, int z, int reach)
{
	//std::cerr << "squareStep : x - " << x << " z - " << z << " reach - " << reach << std::endl;
	int count = 0;
	float avg = 0.0f;
	if (x - reach >= 0 && z - reach >= 0)
	{
		avg += grid[x - reach][z - reach];
		count++;
	}
	if (x - reach >= 0 && z + reach < CHUNK_Z)
	{
		avg += grid[x - reach][z + reach];
		count++;
	}
	if (x + reach < CHUNK_X && z - reach >= 0)
	{
		avg += grid[x + reach][z - reach];
		count++;
	}
	if (x + reach < CHUNK_X && z + reach < CHUNK_Z)
	{
		avg += grid[x + reach][z + reach];
		count++;
	}
	avg += random(reach);
	avg /= count;
	grid[x][z] = avg;
}

void Terrain::diamondStep(float grid[CHUNK_X][CHUNK_Z], int x, int z, int reach)
{
	//std::cerr << "diamondStep : x - " << x << " z - " << z << " reach - " << reach << std::endl;
	int count = 0;
	float avg = 0.0f;
	if (x - reach >= 0)
	{
		avg += grid[x - reach][z];
		count++;
	}
	if (x + reach < CHUNK_X)
	{
		avg += grid[x + reach][z];
		count++;
	}
	if (z - reach >= 0)
	{
		avg += grid[x][z - reach];
		count++;
	}
	if (z + reach < CHUNK_Z)
	{
		avg += grid[x][z + reach];
		count++;
	}
	avg += random(reach);
	avg /= count;
	grid[x][z] = avg;
}

void Terrain::normalizeGrid()
{
	//float sum = 0.0f;
	//float average = 0.0f;
	float current = 0.0f;
	float max = 0.0f;
	for (int i = 0; i < CHUNK_X; i++)
	{
		for (int j = 0; j < CHUNK_Z; j++)
		{
			current = grid[i][j];
			if (current > max)
				max = current;

			//sum += grid[i][j];
		}
	}

	//average = sum / (CHUNK_X * CHUNK_Z);

	for (int i = 0; i < CHUNK_X; i++)
	{
		for (int j = 0; j < CHUNK_Z; j++)
		{
			grid[i][j] /= max;
			//grid[i][j] -= 0.5f;
			//std::cerr << grid[i][j] << std::endl;
			grid[i][j] *= 5.0f;
		}
	}

}

void Terrain::createVertexBufferData()
{
	for (int i = 0; i < vertexCount; i++)
	{
		for (int j = 0; j < vertexCount; j++)
		{
			glm::vec3 vertex;
			glm::vec3 normal;
			glm::vec2 textureCoord;
			float offset = (float)vertexCount / 2.0f;
			vertex.x = (j - offset);
			vertex.z = (i - offset);
			vertex.y = grid[j][i];
			vertices.push_back(vertex);

			normal = calculateNormal(j, i);
			normals.push_back(normal);

			//texture coordinates
			textureCoord.x = (float)j / ((float)vertexCount - 1);
			textureCoord.y = (float)i / ((float)vertexCount - 1);
			textureCoords.push_back(textureCoord);

			//std::cerr << "TexCoord: " << textureCoord.x << " " << textureCoord.y << std::endl;
		}
	}
	std::cerr << "Vertices " << vertices.size() << std::endl;
}

glm::vec3 Terrain::calculateNormal(int x, int z)
{
	float hL, hR, hD, hU;
	
	hL = (x > 0) ? grid[x - 1][z] : grid[x][z];
	hR = (x < vertexCount-1) ? grid[x + 1][z] : grid[x][z];
	hD = (z > 0) ? grid[x][z - 1] : grid[x][z];
	hU = (z < vertexCount-1) ? grid[x][z + 1] : grid[x][z];
	glm::vec3 normal = glm::vec3(hL-hR, 2.0f, hD-hU);
	normal = glm::normalize(normal);
	return normal;
}

void Terrain::createIndices()
{
	int i = 0;
	for (int gz = 0; gz < vertexCount-1; gz++)
	{
		for (int gx = 0; gx < vertexCount-1; gx++)
		{
			int topLeft = (gz * vertexCount) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * vertexCount) + gx;
			int bottomRight = bottomLeft + 1;
			glm::ivec3 topTriangle;
			glm::ivec3 bottomTriangle;
			topTriangle.x = topLeft;
			topTriangle.y = bottomLeft;
			topTriangle.z = topRight;
			bottomTriangle.x = topRight;
			bottomTriangle.y = bottomLeft;
			bottomTriangle.z = bottomRight;
			indices.push_back(topTriangle);
			indices.push_back(bottomTriangle);
			//std::cerr << "Top " << topTriangle.x << " " << topTriangle.y << " " << topTriangle.z << std::endl;
			//std::cerr << "Bot " << bottomTriangle.x << " " << bottomTriangle.y << " " << bottomTriangle.z << std::endl;
		}
	}
	std::cerr << "Indices " << indices.size() << std::endl;
}

void Terrain::createVAO()
{
	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexBufferData.size(),
		vertexBufferData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(),
		indices.data(), GL_STATIC_DRAW);

	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	// Vertices (Position Attribute)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	// Normals (Color Attribute)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture Attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Terrain::createTexture()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	std::string fileLocation = "resources/textures/sand.jpg";
	unsigned char* data = Image::loadImage(fileLocation.c_str(), &width, &height, &nrChannels);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	Image::freeImage(data);
}

void Terrain::draw()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 3 * indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

float Terrain::random(int range)
{
	return (rand() % (range*2)) - range;
}

