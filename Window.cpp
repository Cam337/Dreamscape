#include "Window.h"

/* 
 * Declare your variables below. Unnamed namespace is used here to avoid 
 * declaring global or static variables.
 */
namespace
{
	int width, height;
	glm::vec3 lastPoint;
	std::string windowTitle("Dreamscape");

	// timing
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	int counter = 0;
	const int DURATION = 200;

	// Constants
	const int NONE = 0;
	const int ROTATE = 1;
	const float ROTSCALE = 1.5f;
	const int TRANSLATE = 2;
	const float TRANSCALE = 0.1f;

	// Boolean switches
	bool normalColorMode = false;
	bool dudvMode = false;
	
	// FPS Mouse Variables
	bool initCursorPos = true;
	float lastX = 0.0f;
	float lastY = 0.0f;

	// Trackball Variables
	int movement = NONE;

	// Water
	Water* water;

	// Objects
	Geometry* bear;
	PointLight* sun;

	// Terrain
	Terrain* terrain;

	// Skybox
	Skybox* skybox;

	Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));
	float near = 0.1f;
	float far = 1000.0f;
	glm::mat4 view = camera.GetViewMatrix(); // View matrix, defined by eye, center and up.
	glm::mat4 projection; // Projection matrix.

	// Shaders
	Shader* waterShader; // The shader program id.
	Shader* staticShader;
	Shader* terrainShader;
	Shader* skyboxShader;
	Shader* lightShader;
	Shader* phongShader;
	
};

bool Window::initializeProgram()
{
	// Create a shader program with a vertex shader and a fragment shader.
	waterShader = new Shader("shaders/water.vert", "shaders/water.frag");
	staticShader = new Shader("shaders/shader.vert", "shaders/shader.frag");
	terrainShader = new Shader("shaders/terrain.vert", "shaders/terrain.frag");
	skyboxShader = new Shader("shaders/skybox.vert", "shaders/skybox.frag");
	lightShader = new Shader("shaders/light.vert", "shaders/light.frag");
	phongShader = new Shader("shaders/phongShader.vert", "shaders/phongShader.frag");

	// Check the shader programs.
	if (!(waterShader->programID))
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
	if (!(staticShader->programID))
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
	if (!(terrainShader->programID))
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
	if (!(skyboxShader->programID))
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	waterShader->use();
	waterShader->setInt("reflectionTexture", 0);
	waterShader->setInt("refractionTexture", 1);
	waterShader->setInt("dudvMap", 2);
	waterShader->setInt("normalMap", 3);
	waterShader->setInt("depthMap", 4);

	terrainShader->use();
	terrainShader->setInt("grasstexture", 6);

	skyboxShader->use();
	skyboxShader->setInt("skybox", 5);


	return true;
}

bool Window::initializeObjects()
{
	projection = glm::perspective(glm::radians(camera.Zoom),
		(float)width / (float)height, near, far);

	// Water
	water = new Water(0,0,0);

	// Objects
	bear = new Geometry("resources/objects/bear.obj");
	bear->setAmbient(glm::vec3(0.41f, 0.34f, 0.41f));
	bear->setDiffuse(glm::vec3(0.82f, 0.68f, 0.82f));
	bear->setSpecular(glm::vec3(0.1f));
	bear->setShininess(32.0f);
	bear->translate(glm::vec3(0.0f, 8.0f, -45.0f));
	bear->scale(10.0f);

	sun = new PointLight("resources/objects/sphere.obj", glm::vec3(1.0f, 0.0f, 0.0f));
	sun->translate(glm::vec3(0.0f, 75.0f, 0.0f));
	sun->scale(5.0f);
	sun->setLightColor(glm::vec3(1.0f, 0.82f, 0.08f));

	// Terrain
	terrain = new Terrain(0,0,0,0);

	// Skybox
	skybox = new Skybox(view, projection);

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete bear;
	delete skybox;
	
	// Delete the shader programs.
	glDeleteProgram(waterShader->programID);
	glDeleteProgram(staticShader->programID);
	glDeleteProgram(skyboxShader->programID);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int w, int h)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height);
#endif
	width = w;
	height = h;

	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	projection = glm::perspective(glm::radians(camera.Zoom),
		(float)width / (float)height, near, far);
}

void Window::idleCallback()
{
	sun->update();
}

void Window::displayCallback(GLFWwindow* window)
{
	// Time keeping
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// Set the projection matrix.
	projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, near, far);
	view = camera.GetViewMatrix();
	glm::mat4 model(1.0f);
	glm::vec3 color(1.0f, 0.82f, 0.2f);

	// Process input for FPS Camera
	processInput(window);

	glEnable(GL_CLIP_DISTANCE0);

	// Render reflection texture
	water->bindReflectionFrameBuffer();
	float distance = 2 * (camera.Position.y - water->getHeight());
	camera.Position.y -= distance;
	camera.invertPitch();
	renderScene(glm::vec4(0, 1, 0, -(water->getHeight()))); // can add offest to make edges smoother
	camera.Position.y += distance;
	camera.invertPitch();
	
	// Render refraction texture
	water->bindRefractionFrameBuffer();
	renderScene(glm::vec4(0, -1, 0, water->getHeight()));

	// render the scene
	water->unbindCurrentFrameBuffer();
	renderScene(glm::vec4(0, 1, 0, 1000));

	// Render water
	projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, near, far);
	view = camera.GetViewMatrix();
	model = water->getModel();
	float moveFactor = water->updateMoveFactor(deltaTime);
	glm::vec3 cameraPosition = camera.Position;
	glm::vec3 lightPosition(1.0f, 20.0f, -75.0f);
	//glm::vec3 lightColor(1.0f, 0.73f, 0.08f);
	glm::vec3 lightColor(0.3f);
	waterShader->use();
	waterShader->setMat4("projection", projection);
	waterShader->setMat4("view", view);
	waterShader->setMat4("model", model);
	waterShader->setFloat("moveFactor", moveFactor);
	waterShader->setVec3("cameraPosition", cameraPosition);
	waterShader->setVec3("lightPosition", lightPosition);
	waterShader->setVec3("lightColor", lightColor);
	waterShader->setInt("normalColorMode", normalColorMode);
	waterShader->setInt("dudvMode", dudvMode);
	water->draw();
	waterShader->stop();

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::renderScene(glm::vec4 clipPlane)
{
	// Set the projection matrix.
	projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, near, far);
	view = camera.GetViewMatrix();
	glm::mat4 model(1.0f);
	glm::vec3 color(1.0f, 1.0f, 1.0f);

	// Prepare to render. Clears the color and depth buffers.
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);


	// Bear
	model = bear->getModel();
	//material properties
	glm::vec3 viewPos = camera.Position;
	glm::vec3 materialAmbient = bear->getAmbient();
	glm::vec3 materialDiffuse = bear->getDiffuse();
	glm::vec3 materialSpecular = bear->getSpecular();
	float materialShininess = bear->getShininess();
	//light properties
	glm::vec3 lightPosition = sun->getLightPosition();
	glm::vec3 lightColor = sun->getLightColor();
	glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
	glm::vec3 specularColor = glm::vec3(1.0f);
	float shininess = bear->getShininess();
	phongShader->use();
	phongShader->setVec3("viewPos", viewPos);
	phongShader->setInt("normalColorMode", normalColorMode);
	phongShader->setMat4("projection", projection);
	phongShader->setMat4("view", view);
	phongShader->setMat4("model", model);
	phongShader->setVec4("clipPlane", clipPlane);
	phongShader->setVec3("material.ambient", materialAmbient);
	phongShader->setVec3("material.diffuse", materialDiffuse);
	phongShader->setVec3("material.specular", materialSpecular);
	phongShader->setFloat("material.shininess", materialShininess);
	phongShader->setVec3("light.position", lightPosition);
	phongShader->setVec3("light.ambient", ambientColor);
	phongShader->setVec3("light.diffuse", diffuseColor);
	phongShader->setVec3("light.specular", specularColor);
	bear->draw(phongShader->programID, model);
	phongShader->stop();

	// Sun
	model = sun->getModel();
	color = sun->getLightColor();
	lightShader->use();
	lightShader->setMat4("projection", projection);
	lightShader->setMat4("view", view);
	lightShader->setMat4("model", model);
	lightShader->setVec4("clipPlane", clipPlane);
	lightShader->setVec3("color", color);
	sun->draw();
	lightShader->stop();

	// Draw Terrain
	model = glm::mat4(1.0f);
	glm::vec3 cameraPosition = camera.Position;
	lightPosition = sun->getLightPosition();
	terrainShader->use();
	terrainShader->setMat4("projection", projection);
	terrainShader->setMat4("view", view);
	terrainShader->setMat4("model", model);
	terrainShader->setVec3("cameraPosition", cameraPosition);
	terrainShader->setVec3("lightPosition", lightPosition);
	terrainShader->setVec4("clipPlane", clipPlane);
	terrain->draw();

	// Skybox
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader->use();
	view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
	skyboxShader->setMat4("projection", projection);
	skyboxShader->setMat4("view", view);
	skybox->draw(skyboxShader->programID);
	skyboxShader->stop();
	glDepthFunc(GL_LESS); // set depth function back to default
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Section 4: Modify below to add your key callbacks.
	 */

	 // Check for a key press.
	if (action == GLFW_PRESS)
	{
		// Uppercase key presses (shift held down + key press)
		if (mods == GLFW_MOD_SHIFT) {
			switch (key) {
			case GLFW_KEY_P:
				//currentObj->updatePointSize(1.25f);
				break;
			case GLFW_KEY_Z:
				//updateViewMatrix(1.25f);
				break;
			case GLFW_KEY_C:
				//currentObj->scale(1.25f);
				break;
			case GLFW_KEY_R:
				//currentObj->resetScale();
				//currentObj->resetOrientation();
				break;
			default:
				break;
			}
		} else

		// Deals with lowercase key presses
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_1:
			break;
		case GLFW_KEY_2:
			break;
		case GLFW_KEY_3:
			break;
		case GLFW_KEY_F1:
			//currentObj = bunny;
			break;
		case GLFW_KEY_F2:
			//currentObj = dragon;
			break;
		case GLFW_KEY_F3:
			//currentObj = bear;
			break;
		case GLFW_KEY_N:
			if (normalColorMode)
				std::cout << "Normal color mode off\n";
			else
				std::cout << "Normal color mode on\n";
			normalColorMode = !normalColorMode;
			break;
		case GLFW_KEY_D:
			if (dudvMode)
				std::cout << "dudv mode mode off\n";
			else
				std::cout << "dudv mode on\n";
			dudvMode = !dudvMode;
			break;
		case GLFW_KEY_Z:
			//updateViewMatrix(0.8f);
			break;
		case GLFW_KEY_C:
			//currentObj->scale(0.8f);
			break;
		case GLFW_KEY_R:
			//currentObj->resetPosition();
			break;
		default:
			break;
		}
	}
}

void Window::processInput(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		double xpos, ypos;
		glm::vec3 point;
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			glfwGetCursorPos(window, &xpos, &ypos);
			point = glm::vec3(xpos, ypos, 0.0f);
			lastPoint = trackBallMapping(point);

			movement = ROTATE;
			//std::cerr << "left press - last point: " << lastPoint.x << " " 
				// << lastPoint.y << " " << lastPoint.z << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			glfwGetCursorPos(window, &xpos, &ypos);
			point = glm::vec3(xpos, ypos, 0.0f);
			lastPoint = point;
		    
			movement = TRANSLATE;
			//std::cerr << "right press - last point: " << lastPoint.x << " " 
				// << lastPoint.y << " " << lastPoint.z << std::endl;
			break;
		default:
			break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			movement = NONE;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			movement = NONE;
			break;
		}
	}
}

glm::vec3 Window::trackBallMapping(glm::vec3 point)
{
	glm::vec3 v; // Vector v is the 3D position of the mouse on the trackball
	float d;

	v.x = (2.0f * point.x - width) / width;
	v.y = (height - 2.0f * point.y) / height;
	v.z = 0.0f;

	d = glm::length(v); // distance from the trackball's origin to the mouse location,
						// wihout considering depth (so in the plane of the trackballs origin

	//std::cerr << "x: " << v.x << " y: " << v.y << " d: " << d << std::endl;

	d = (d < 1.0f) ? d : 1.0f;
	v.z = sqrt(1.001f - d * d);

	v = glm::normalize(v);
	return v;
}

void Window::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (initCursorPos) 
	{
		lastX = xpos;
		lastY = ypos;
		initCursorPos = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
	/*
	glm::vec3 direction;
	float rotAngle, velocity;
	glm::vec3 curPoint;

	switch (movement)
	{
	case ROTATE:
		glm::vec3 point(xpos, ypos, 0.0f);
		curPoint = trackBallMapping(point);
		direction = curPoint - lastPoint;
		velocity = glm::length(direction);
		if (velocity > 0.0001)
		{
			glm::vec3 rotAxis = glm::cross(lastPoint, curPoint);
			rotAngle = velocity * ROTSCALE;
			camera.processMouseTrackball(rotAngle, rotAxis);
			//robotSquad->rotate(rotAngle, rotAxis);
		}
		break;
	case TRANSLATE:
		curPoint = glm::vec3(xpos, ypos, 0.0f);
		direction = curPoint - lastPoint;
		//currentObj->translate(direction * glm::vec3(1.0f, -1.0f, 0.0f) * deltaTime * TRANSCALE);
		break;
	default:
		break;
	}
	*/
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
