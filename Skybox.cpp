#include "Skybox.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Skybox::Skybox(glm::mat4 view, glm::mat4 projection) :
	view(view), projection(projection)
{
	// Model matrix. Since the original size of the cube is 2, in order to
	// have a cube of some size, we need to scale the cube by size / 2.
	model = glm::mat4(1);
	view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix

	// The color of the cube. Try setting it to something else!
	//color = glm::vec3(0.1f, 0.95f, 1.0f);

	/*
	 * Cube indices used below.
	 *    4----7
     *   /|   /|
     *  0-+--3 |
     *  | 5--+-6
     *  |/   |/
     *  1----2
	 *
	 */

	// The 8 vertices of a cube.
	std::vector<glm::vec3> vertices
	{
		glm::vec3(-1, 1, 1),
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(-1, 1, -1),
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, -1)
	};

	// Each ivec3(v1, v2, v3) define a triangle consists of vertices v1, v2
	// and v3 in counter-clockwise order.
	std::vector<glm::ivec3> indices
	{
		/*
		// Front face.
		glm::ivec3(0, 1, 2),
		glm::ivec3(2, 3, 0),
		// Back face.
		glm::ivec3(7, 6, 5),
		glm::ivec3(5, 4, 7),
		// Right face.
		glm::ivec3(3, 2, 6),
		glm::ivec3(6, 7, 3),
		// Left face.
		glm::ivec3(4, 5, 1),
		glm::ivec3(1, 0, 4),
		// Top face.
		glm::ivec3(4, 0, 3),
		glm::ivec3(3, 7, 4),
		// Bottom face.
		glm::ivec3(1, 5, 6),
		glm::ivec3(6, 2, 1)
		*/

		// Front face.
		glm::ivec3(2, 1, 0),
		glm::ivec3(0, 3, 2),
		// Back face.
		glm::ivec3(5, 6, 7),
		glm::ivec3(7, 4, 5),
		// Right face.
		glm::ivec3(6, 2, 3),
		glm::ivec3(3, 7, 6),
		// Left face.
		glm::ivec3(1, 5, 4),
		glm::ivec3(4, 0, 1),
		// Top face.
		glm::ivec3(3, 0, 4),
		glm::ivec3(4, 7, 3),
		// Bottom face.
		glm::ivec3(6, 5, 1),
		glm::ivec3(1, 2, 6)

	};

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbos);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the vertices.
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
		vertices.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0.
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	// Bind to the second VBO. We will use it to store the indices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
	// Pass in the data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(),
		indices.data(), GL_STATIC_DRAW);

	// Unbind from the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
	/*
	std::vector<std::string> faces
	{
		"mp_blood/blood_ft.jpg",
		"mp_blood/blood_bk.jpg",
		"mp_blood/blood_up.jpg",
		"mp_blood/blood_dn.jpg",
		"mp_blood/blood_rt.jpg",
		"mp_blood/blood_lf.jpg"
	};
	*/

	std::vector<std::string> faces
	{
		"resources/skybox/posx.jpg",
		"resources/skybox/negx.jpg",
		"resources/skybox/posy.jpg",
		"resources/skybox/negy.jpg",
		"resources/skybox/posz.jpg",
		"resources/skybox/negz.jpg"
	};

	cubemapTexture = loadCubemap(faces);
}

Skybox::~Skybox()
{
	// Delete the VBOs and the VAO.
	glDeleteBuffers(2, vbos);
	glDeleteVertexArrays(1, &vao);
}

void Skybox::draw(GLuint shader)
{
	/*
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	glUseProgram(shader);

	// Set uniforms
	GLuint viewLoc = glGetUniformLocation(shader, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	GLuint projectionLoc = glGetUniformLocation(shader, "skybox");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	*/

	glBindVertexArray(vao); // Bind to the VAO.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); // Unbind from the VAO.

	//glDepthFunc(GL_LESS); // set depth function back to default

}

void Skybox::update()
{
	// Spin the cube by 1 degree.
	//spin(0.1f);
}

void Skybox::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

GLuint Skybox::loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

GLuint Skybox::getCubemap()
{
	return cubemapTexture;
}
