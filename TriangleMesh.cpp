#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(std::string objFilename, GLfloat pointSize, glm::vec3 rotationAxis)
	: pointSize(pointSize), rotationAxis(rotationAxis), 
		translationMat(glm::mat4(1.0f)), scaleMat(glm::mat4(1.0f)), rotationMat(glm::mat4(1.0f)),
		ambient(glm::vec3(0.2f)), diffuse(glm::vec3(0.5f)), specular(glm::vec3(1.0f)), shininess(32.0f)
{
	/*
	 * TODO: Section 2: Currently, all the points are hard coded below.
	 * Modify this to read points from an obj file.
	 * Don't forget to load in the object normals for normal coloring as well
	 
	points =
	{
		glm::vec3(-2.5, 2.5, 2.5),
		glm::vec3(-2.5, -2.5, 2.5),
		glm::vec3(2.5, -2.5, 2.5),
		glm::vec3(2.5, 2.5, 2.5),
		glm::vec3(-2.5, 2.5, -2.5),
		glm::vec3(-2.5, -2.5, -2.5),
		glm::vec3(2.5, -2.5, -2.5),
		glm::vec3(2.5, 2.5, -2.5)
	};
	*/

	parseOBJFile2(objFilename);

	/*
	 * TODO: Section 4, you will need to normalize the object to fit in the
	 * screen.
	 */

	// Set the model matrix. 
	//model = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	// Set the color. 
	color = glm::vec3(1, 0, 0);

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * faces.size(), faces.data(), GL_STATIC_DRAW);

	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	/*
	 * TODO: Section 2 and 3. 	 
	 * Following the above example but for vertex normals, 
	 * 1) Generate a new vertex bufferbuffer,
	 * 2) Bind it as a GL_ARRAY_BUFFER type, 
	 * 3) Pass in the data 
	 * 4) Enable the next attribute array (which is 1)
	 * 5) Tell it how to process each vertex using glVertexAttribPointer
	 */

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

TriangleMesh::~TriangleMesh()
{
	// Delete the VBO and the VAO.
	// Failure to delete your VAOs, VBOs and other data given to OpenGL
	// is dangerous and may slow your program and cause memory leaks
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void TriangleMesh::parseOBJFile(std::string objFilename)
{
	FILE* filePointer;	// file pointer
	const char* fileName = objFilename.c_str();
	int c1 = 1;			// characters read from file
	int c2 = 1;			
	float x, y, z;		// vertex coordinates
	float r, g, b;		// vertex color
	float xn, yn, zn;   // normal vector components
	int v1, v2, v3;		// vertex indices for triangle face

	std::cerr << "File name: " << fileName << std::endl;

	filePointer = fopen(fileName, "rb");
	if (filePointer == NULL) { std::cerr << "Error loading obj file" << std::endl; exit(-1); }

	while (c1 != EOF) {
		c1 = fgetc(filePointer);
		c2 = fgetc(filePointer);

		if (c1 == 'v' && c2 == ' ')
		{
			fscanf(filePointer, "%f %f %f %f %f %f\n", &x, &y, &z, &r, &g, &b);
			points.push_back(glm::vec3(x, y, z));
		}
		else if (c1 == 'v' && c2 == 'n')
		{
			fscanf(filePointer, "%f %f %f\n", &xn, &yn, &zn);
			normals.push_back(glm::vec3(xn, yn, zn));
		}
		else if (c1 == '\n')
		{
			c1 = fgetc(filePointer);
		}
		else if (c1 == 'f')
		{
			fscanf(filePointer, "%i//%i %i//%i %i//%i\n", &v1, &v1, &v2, &v2, &v3, &v3); 
			faces.push_back(glm::ivec3(v1-1,v2-1,v3-1));
			//std::cerr << "Face: " << v1 << " " << v2 << " " << v3 << std::endl;
		}
	}
	fclose(filePointer);

	int size = faces.size();
	std::cerr << "First Face: " << faces[0].x << " " << faces[0].y << " " << faces[0].z << std::endl;
	std::cerr << "Last Face: " << faces[size - 1].x << " " << faces[size - 1].y << " " << faces[size - 1].z << std::endl;
	std::cerr << "Number of faces: " << faces.size() << std::endl;

	for (int i = 0; i < points.size(); i++)
	{
		vertexBuffer.push_back(points[i]);
		vertexBuffer.push_back(normals[i]);
	}
}

void TriangleMesh::parseOBJFile2(std::string objFilename)
{
	// Variables used to parse vertices, normals, and their indices
	std::vector<glm::vec3> vertexIndices;
	std::vector<glm::vec3> normalIndices;
	std::vector<glm::vec3> inputVertices;
	std::vector<glm::vec3> inputNormals;

	// Fit the model to screen variables
	glm::vec3 max = glm::vec3(0, 0, 0);
	glm::vec3 min = glm::vec3(0, 0, 0);
	glm::vec3 modelCenter = glm::vec3(0, 0, 0);
	float maxCenterDistance = 0.0F;

	std::ifstream objFile(objFilename); // pass name of file here

	// Parse file if it can be opened
	if (objFile.is_open())
	{
		std::cerr << "Opened the file " << objFilename << std::endl;
		std::string line; // line from file

		// Parse file line by line
		while (std::getline(objFile, line))
		{
			// Turn the line into a string stream for parsing.
			std::stringstream ss;
			ss << line;

			// Read the first word of the line.
			std::string label;
			ss >> label;

			// Parse vertex components
			if (label == "v")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 vertex;
				ss >> vertex.x >> vertex.y >> vertex.z;
				//std::cerr << "v " << vertex.x << std::endl;

				// Process the vertex. For example, you can save it to a.
				inputVertices.push_back(vertex);

				// Check for min and max x,y,z for centering
				if (vertex.x > max.x)
					max.x = vertex.x;
				if (vertex.y > max.y)
					max.y = vertex.y;
				if (vertex.z > max.z)
					max.z = vertex.z;
				if (vertex.x < min.x)
					min.x = vertex.x;
				if (vertex.y < min.y)
					min.y = vertex.y;
				if (vertex.z < min.z)
					min.z = vertex.z;
			}
			// Parse vertex normal components
			else if (label == "vn")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 vector;
				ss >> vector.x >> vector.y >> vector.z;
				//std::cerr << "vn " << vector.x << std::endl;

				// Process the vector. For example, you can save it to a.
				inputNormals.push_back(vector);
			}
			// Process face information
			else if (label == "f")
			{
				// Save the indices of the vertices and vector normals for each triangle
				std::string i1, i2, i3;
				glm::ivec3 vertexIndex;
				glm::ivec3 uvIndex;
				glm::ivec3 normalIndex;


				// Read in the indices of the vertices of the triangles
				ss >> i1 >> i2 >> i3;
				//std::cerr << "f " << i1 << " " << i2 << " " << i3 << " " << std::endl;
				//std::cerr << i1.c_str() << " " << i2.c_str() << " " << i3.c_str() << std::endl;

				
				int match1 = sscanf(i1.c_str(), "%d//%d", &vertexIndex[0], &normalIndex[0]);
				int match2 = sscanf(i2.c_str(), "%d//%d", &vertexIndex[1], &normalIndex[1]);
				int match3 = sscanf(i3.c_str(), "%d//%d", &vertexIndex[2], &normalIndex[2]);
				//std::cerr << "vertexIndex: " << vertexIndex.x << " " << vertexIndex.y << " " << vertexIndex.z << " " << std::endl;
				//std::cerr << "normalIndex: " << normalIndex.x << " " << normalIndex.y << " " << normalIndex.z << " " << std::endl;
				// Convert to integers
				// Correct indices to start from 0 instead of 1
				vertexIndex -= glm::vec3(1);
				//uvIndex -= glm::vec3(1);
				normalIndex -= glm::vec3(1);
				//std::cerr << vertexIndex.x << " " << vertexIndex.y << " " << vertexIndex.z << std::endl;

				// Process the indices
				vertexIndices.push_back(vertexIndex);
				normalIndices.push_back(normalIndex);
			}
		}
	}
	else
	{
		std::cerr << "Can't open the file " << objFilename << std::endl;
	}

	objFile.close();

	modelCenter = (max + min) / 2.0f; // the geometric center of the model
	
	// find the maximum distance from the model center
	float distance = 0;
	for (int i = 0; i < inputVertices.size(); i++)
	{
		distance = glm::distance(modelCenter, inputVertices[i]);
		if (distance > maxCenterDistance)
			maxCenterDistance = distance;
	}
	
	// translate to origin and scale model to be within [-1, 1]
	for (int i = 0; i < inputVertices.size(); i++)
	{
		inputVertices[i] = (inputVertices[i] - modelCenter) / maxCenterDistance;
	}

	// Structure vertex buffer data and triangle indices
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		// re-order and duplicate vertice and normal data to align indices
		points.push_back(inputVertices[vertexIndices[i].x]);
		points.push_back(inputVertices[vertexIndices[i].y]);
		points.push_back(inputVertices[vertexIndices[i].z]);
		//std::cerr << "vertices: " << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << " " << std::endl;

		// same normal for x,y,z robot obj files since they all belong to the same triangle
		normals.push_back(inputNormals[normalIndices[i].x]);
		normals.push_back(inputNormals[normalIndices[i].y]);
		normals.push_back(inputNormals[normalIndices[i].z]);
		//std::cerr << "normals: " << normals[i].x << " " << normals[i].y << " " << normals[i].z << " " << std::endl;

		faces.push_back(glm::ivec3(3 * i, 3 * i + 1, 3 * i + 2));
	}

	std::cerr << points.size() << std::endl;
	std::cerr << normals.size() << std::endl;
	std::cerr << faces.size() << std::endl;
	std::cerr << "max: "<< max.x << " " << max.y << " " << max.z << std::endl;
	std::cerr << "min: " << min.x << " " << min.y << " " << min.z << std::endl;

	for (int i = 0; i < points.size(); i++)
	{
		vertexBuffer.push_back(points[i]);
		vertexBuffer.push_back(normals[i]);
	}
	 
}

void TriangleMesh::draw()
{
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Set point size.
	// glPointSize(pointSize);
	// Draw points 
	glDrawElements(GL_TRIANGLES, 3 * faces.size(), GL_UNSIGNED_INT, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void TriangleMesh::update()
{
	// Spin the cube by 1 degree.
	spin(0.1f, rotationAxis);
	/*
	 * TODO: Section 3: Modify this function to spin the dragon and bunny about
	 * different axes. Look at the spin function for an idea
	 */
}

void TriangleMesh::updatePointSize(GLfloat size)
{
	/*
	 * TODO: Section 3: Implement this function to adjust the point size.
	 */

	if(pointSize*size > 1 && pointSize*size < 100.0f)
		pointSize *= size;

	std::cerr << "update point size to" << pointSize << std::endl;
}

void TriangleMesh::spin(float deg, glm::vec3 rotationAxis)
{
	// Update the model matrix by multiplying a rotation matrix
	// model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(deg), rotationAxis);
	spinMat = glm::rotate(spinMat, glm::radians(deg), rotationAxis);
}

void TriangleMesh::rotate(float deg, glm::vec3 rotationAxis)
{
	model = glm::rotate(glm::mat4(1.0f), glm::radians(deg), rotationAxis) * model;
	rotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(deg), rotationAxis) * rotationMat;
}

void TriangleMesh::rotateModel(float deg, glm::vec3 rotationAxis)
{
	glm::mat4 invTranslationMat = glm::inverse(translationMat);
	model = translationMat * glm::rotate(glm::mat4(1.0f), glm::radians(deg), rotationAxis) * invTranslationMat * model;
	rotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(deg), rotationAxis) * rotationMat;
}

void TriangleMesh::resetOrientation()
{
	model = model * glm::inverse(rotationMat);
	rotationMat = glm::mat4(1.0f);
}

void TriangleMesh::translate(glm::vec3 translation)
{
	glm::mat4 tMat = glm::translate(translation);
	model = tMat * model;
	translationMat = tMat * translationMat;
}

void TriangleMesh::resetPosition()
{
	model = glm::inverse(translationMat) * model;
	translationMat = glm::mat4(1.0f);
}

void TriangleMesh::scale(float scaleFactor)
{
	model = glm::scale(model, glm::vec3(scaleFactor));
	scaleMat = glm::scale(scaleMat, glm::vec3(scaleFactor));
}

void TriangleMesh::resetScale()
{
	model = model * glm::inverse(scaleMat);
	scaleMat = glm::mat4(1.0f);
}

void TriangleMesh::setAmbient(glm::vec3 color)
{
	ambient = color;
}

void TriangleMesh::setDiffuse(glm::vec3 color)
{
	diffuse = color;
}

void TriangleMesh::setSpecular(glm::vec3 color)
{
	specular = color;
}

void TriangleMesh::setShininess(float shine)
{
	shininess = shine;
}

std::vector<glm::vec3> TriangleMesh::getPoints()
{
	return points;
}

std::vector<glm::vec3> TriangleMesh::getNormals()
{
	return normals;
}

GLfloat TriangleMesh::getPointSize()
{
	return pointSize;
}

glm::vec3 TriangleMesh::getAmbient()
{
	return ambient;
}

glm::vec3 TriangleMesh::getDiffuse()
{
	return diffuse;
}

glm::vec3 TriangleMesh::getSpecular()
{
	return specular;
}

float TriangleMesh::getShininess()
{
	return shininess;
}

