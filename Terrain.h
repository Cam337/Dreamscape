#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Image.h"

class Terrain
{
private:
	static const int CHUNK_X = 257;
	static const int CHUNK_Z = 257;
	int vertexCount = 257.0f;
	float grid[CHUNK_X][CHUNK_Z];
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	std::vector<float> vertexBufferData;
	std::vector<glm::ivec3> indices;
	GLuint vao, vbo, ebo, texture;
public:
	Terrain(int tl, int tr, int bl, int br);
	~Terrain();

	void diamondSquare(float grid[CHUNK_X][CHUNK_Z], int size);
	void squareStep(float grid[CHUNK_X][CHUNK_Z], int x, int z, int reach);
	void diamondStep(float grid[CHUNK_X][CHUNK_Z], int x, int z, int reach);
	void normalizeGrid();
	void createVertexBufferData();
	glm::vec3 calculateNormal(int x, int z);
	void createIndices();
	void createVAO();
	void createTexture();
	void draw();
	static float random(int range);
	
};

#endif
