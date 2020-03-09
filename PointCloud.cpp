#include "PointCloud.h"

PointCloud::PointCloud(std::string objFilename, GLfloat pointSize, glm::vec3 rotationAxis)
	: pointSize(pointSize), rotationAxis(rotationAxis), 
		translationMat(glm::mat4(1.0f)), scaleMat(glm::mat4(1.0f)), rotationMat(glm::mat4(1.0f))
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

	parseOBJFile(objFilename);

	/*
	 * TODO: Section 4, you will need to normalize the object to fit in the
	 * screen.
	 */

	 // Set the model matrix to an identity matrix. 
	model = glm::mat4(1);
	// Set the color. 
	color = glm::vec3(1, 0, 0);

	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind to the VAO.
	// This tells OpenGL which data it should be paying attention to
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexBuffer.size(),
		vertexBuffer.data(), GL_STATIC_DRAW);
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

PointCloud::~PointCloud()
{
	// Delete the VBO and the VAO.
	// Failure to delete your VAOs, VBOs and other data given to OpenGL
	// is dangerous and may slow your program and cause memory leaks
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void PointCloud::parseOBJFile(std::string objFilename)
{
	FILE* filePointer;	// file pointer
	const char* fileName = objFilename.c_str();
	float x, y, z;		// vertex coordinates
	float r, g, b;		// vertex color
	float xn, yn, zn;
	int c1, c2;			// characters read from file

	std::cerr << "File name: " << fileName << std::endl;

	filePointer = fopen(fileName, "rb");
	if (filePointer == NULL) { std::cerr << "Error loading obj file" << std::endl; exit(-1); }

	while (c1 != EOF) {
		c1 = fgetc(filePointer);
		c2 = fgetc(filePointer);

		if (c1 == 'v' && c2 == ' ')
		{
			fscanf(filePointer, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
			points.push_back(glm::vec3(x, y, z));
		}
		else if (c1 == 'v' && c2 == 'n')
		{
			fscanf(filePointer, "%f %f %f", &xn, &yn, &zn);
			normals.push_back(glm::vec3(xn, yn, zn));
		}
	}
	fclose(filePointer);

	for (int i = 0; i < points.size(); i++)
	{
		vertexBuffer.push_back(points[i]);
		vertexBuffer.push_back(normals[i]);
	}
}

void PointCloud::draw()
{
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Set point size.
	glPointSize(pointSize);
	// Draw points 
	glDrawArrays(GL_POINTS, 0, vertexBuffer.size());
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void PointCloud::update()
{
	// Spin the cube by 1 degree.
	spin(0.1f, rotationAxis);
	/*
	 * TODO: Section 3: Modify this function to spin the dragon and bunny about
	 * different axes. Look at the spin function for an idea
	 */
}

void PointCloud::updatePointSize(GLfloat size)
{
	/*
	 * TODO: Section 3: Implement this function to adjust the point size.
	 */

	if(pointSize*size > 1 && pointSize*size < 100.0f)
		pointSize *= size;

	std::cerr << "update point size to" << pointSize << std::endl;
}

void PointCloud::spin(float deg, glm::vec3 rotationAxis)
{
	// Update the model matrix by multiplying a rotation matrix
	// model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(deg), rotationAxis);
	rotationMat = glm::rotate(rotationMat, glm::radians(deg), rotationAxis);
}

void PointCloud::resetOrientation()
{
	model = model * glm::inverse(rotationMat);
	rotationMat = glm::mat4(1.0f);
}

void PointCloud::translate(glm::vec3 translation)
{
	glm::mat4 tMat = glm::translate(translation);
	model = tMat * model;
	translationMat = tMat * translationMat;
}

void PointCloud::resetPosition()
{
	model = glm::inverse(translationMat) * model;
	translationMat = glm::mat4(1.0f);
}

void PointCloud::scale(float scaleFactor)
{
	model = glm::scale(model, glm::vec3(scaleFactor));
	scaleMat = glm::scale(scaleMat, glm::vec3(scaleFactor));
}

void PointCloud::resetScale()
{
	model = model * glm::inverse(scaleMat);
	scaleMat = glm::mat4(1.0f);
}

std::vector<glm::vec3> PointCloud::getPoints()
{
	return points;
}

std::vector<glm::vec3> PointCloud::getNormals()
{
	return normals;
}

GLfloat PointCloud::getPointSize()
{
	return pointSize;
}

