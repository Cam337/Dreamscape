#include "Window.h"

/* 
 * Declare your variables below. Unnamed namespace is used here to avoid 
 * declaring global or static variables.
 */
namespace
{
	int width, height;
	glm::vec3 lastPoint;
	std::string windowTitle("GLFW Starter Project");

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
	bool pointLightOn = true;
	bool pointLightMovement = false;
	bool directionalLightOn = true;
	
	// FPS Mouse Variables
	bool initCursorPos = true;
	float lastX = 0.0f;
	float lastY = 0.0f;

	// Trackball Variables
	int movement = NONE;

	Cube* cube;
	PointCloud* cubePoints;
	TriangleMesh* bunny;
	TriangleMesh* dragon;
	TriangleMesh* bear;
	TriangleMesh* currentObj;  // The object currently displaying.
	PointLight* pointLight;
	DirectionalLight* directionalLight;
	Skybox* skybox;
	Track* track;

	// Scene Graph Robot
	Transform* robotSquad;
	Transform* robot;
	Transform* bodyT;
	Geometry* body;
	Transform* headT;
	Geometry* head;
	Transform* eye1;
	Transform* eye2;
	Geometry* eye;
	Transform* antenna1;
	Transform* antenna2;
	Geometry* antenna;
	Transform* limb1;
	Transform* limb2;
	Transform* limb3;
	Transform* limb4;
	Geometry* limb;

	// Water
	Water* water;

	Camera camera(glm::vec3(0.0f, 10.0f, 30.0f));
	float near = 0.1f;
	float far = 100.0f;
	glm::mat4 view = camera.GetViewMatrix(); // View matrix, defined by eye, center and up.
	glm::mat4 projection; // Projection matrix.

	GLuint program; // The shader program id.
	GLuint projectionLoc; // Location of projection in shader.
	GLuint viewLoc; // Location of view in shader.
	GLuint modelLoc; // Location of model in shader.
	GLuint colorLoc; // Location of color in shader.

	GLuint lightDirectionLoc;
	GLuint dirLightAmbientLoc;
	GLuint dirLightDiffuseLoc;
	GLuint dirLightSpecularLoc;

	GLuint phongShader; // The shader for the phong illumination lighting
	GLuint lightPosLoc;
	GLuint viewPosLoc;
	GLuint lightAmbientLoc;
	GLuint lightDiffuseLoc;
	GLuint lightSpecularLoc;
	GLuint materialAmbientLoc;
	GLuint materialDiffuseLoc;
	GLuint materialSpecularLoc;
	GLuint materialShininessLoc;
	GLuint phongProjectionLoc; // Location of projection in shader.
	GLuint phongViewLoc; // Location of view in shader.
	GLuint phongModelLoc; // Location of model in shader.
	GLuint normalColorModeLoc;

	// Skybox
	GLuint skyboxShader;
	GLuint skyboxLoc;
	GLuint sb_projectionLoc; // Location of projection in shader.
	GLuint sb_viewLoc; // Location of view in shader.

	// Track
	GLuint trackShader;
	GLuint trackModelLoc;
	GLuint trackProjectionLoc;
	GLuint trackViewLoc;
	GLuint trackColorLoc;

	// Water
	GLuint waterShader;
	GLuint waterProjectionLoc; // Location of projection in shader.
	GLuint waterViewLoc; // Location of view in shader.
	GLuint waterModelLoc; // Location of model in shader.

	std::vector<BezierCurve*> curves;
	BezierCurve* currentCurve;
	int numCurves;
	int curveCounter = 0;
	std::vector<glm::vec3> points;
	int numPoints;
	int pointCounter = 0;
	glm::vec3 direction;
};

bool Window::initializeProgram()
{
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	phongShader = LoadShaders("shaders/phongShader.vert", "shaders/phongShader.frag");
	skyboxShader = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
	trackShader = LoadShaders("shaders/track.vert", "shaders/track.frag");
	waterShader = LoadShaders("shaders/water.vert", "shaders/water.frag");

	// This shader program is for displaying your rasterizer results
	// DO NOT MODIFY THESE SHADER PROGRAMS
	// programQuad = LoadShaders("shaders/RasterizerQuad.vert", "shaders/RasterizerQuad.frag");

	// Check the shader programs.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
	if (!phongShader)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	// Activate the shader program.
	glUseProgram(program);
	// Get the locations of uniform variables.
	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");
	colorLoc = glGetUniformLocation(program, "color");

	glUseProgram(phongShader);
	// Get the locations of uniform variables.
	lightDirectionLoc = glGetUniformLocation(phongShader, "light.direction");
	dirLightAmbientLoc = glGetUniformLocation(phongShader, "dirLight.ambient");
	dirLightDiffuseLoc = glGetUniformLocation(phongShader, "dirLight.diffuse");
	dirLightSpecularLoc = glGetUniformLocation(phongShader, "dirLight.specular");

	phongProjectionLoc = glGetUniformLocation(phongShader, "projection");
	phongViewLoc = glGetUniformLocation(phongShader, "view");
	phongModelLoc = glGetUniformLocation(phongShader, "model");
	lightPosLoc = glGetUniformLocation(phongShader, "light.position");
	viewPosLoc = glGetUniformLocation(phongShader, "viewPos");
	lightAmbientLoc = glGetUniformLocation(phongShader, "light.ambient");
	lightDiffuseLoc = glGetUniformLocation(phongShader, "light.diffuse");
	lightSpecularLoc = glGetUniformLocation(phongShader, "light.specular");
	materialAmbientLoc = glGetUniformLocation(phongShader, "material.ambient");
	materialDiffuseLoc = glGetUniformLocation(phongShader, "material.diffuse");
	materialSpecularLoc = glGetUniformLocation(phongShader, "material.specular");
	materialShininessLoc = glGetUniformLocation(phongShader, "material.shininess");
	normalColorModeLoc = glGetUniformLocation(phongShader, "normalColorMode");

	// Skybox
	glUseProgram(skyboxShader);
	skyboxLoc = glGetUniformLocation(skyboxShader, "skybox");
	sb_projectionLoc = glGetUniformLocation(skyboxShader, "projection");
	sb_viewLoc = glGetUniformLocation(skyboxShader, "view");

	// Track
	glUseProgram(trackShader);
	trackModelLoc = glGetUniformLocation(trackShader, "model");
	trackProjectionLoc = glGetUniformLocation(trackShader, "projection");
	trackViewLoc = glGetUniformLocation(trackShader, "view");
	trackColorLoc = glGetUniformLocation(trackShader, "color");

	// Water
	glUseProgram(waterShader);
	waterProjectionLoc = glGetUniformLocation(waterShader, "projection");
	waterViewLoc = glGetUniformLocation(waterShader, "view");
	waterModelLoc = glGetUniformLocation(waterShader, "model");

	return true;
}

bool Window::initializeObjects()
{
	projection = glm::perspective(glm::radians(camera.Zoom),
		(float)width / (float)height, near, far);


	cube = new Cube(100.0f);
	/*
	// Create a cube of size 5.
	// cube = new Cube(5.0f);
	// Create a point cloud consisting of cube vertices.
	// cubePoints = new PointCloud("foo", 10);
	bunny = new TriangleMesh("bunny.obj", 3.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	//bunny->translate(glm::vec3(-2.0f, 0.0f, 0.0f));
	bunny->setAmbient(glm::vec3(1.0f));
	bunny->setDiffuse(glm::vec3(1.0f));
	bunny->setSpecular(glm::vec3(1.0f));
	bunny->setShininess(64.0f);
	
	dragon = new TriangleMesh("dragon.obj", 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	//dragon->translate(glm::vec3(2.0f, 0.0f, 0.0f));
	dragon->setAmbient(glm::vec3(0.135f, 0.223f, 0.1575f));
	dragon->setDiffuse(glm::vec3(0.54f, 0.89f, 0.63f));
	dragon->setSpecular(glm::vec3(0.516f, 0.516f, 0.516f));
	dragon->setShininess(32.0f);

	bear = new TriangleMesh("bear.obj", 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	bear->setAmbient(glm::vec3(0.3f, 0.3f, 0.7f));
	bear->setDiffuse(glm::vec3(0.5f, 0.5f, 1.0f));
	bear->setSpecular(glm::vec3(0.0f));
	bear->setShininess(32.0f);
	
	// directional light
	directionalLight = new DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, -1.0f, 0.0f));

	// point light
	pointLight = new PointLight("sphere.obj", 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	pointLight->setLightColor(glm::vec3(1.0f, 0.5f, 0.6f));
	pointLight->translate(glm::vec3(0.5f, 0.5f, 0.5f));
	pointLight->scale(0.2f);
	*/

	// Scene graph Robot
	glm::mat4 transform = glm::mat4(1.0f);
	transform *= glm::scale(glm::vec3(1.0f));
	robot = new Transform(transform);

	// body
	transform = glm::mat4(1.0f);
	bodyT = new Transform(transform);
	body = new Geometry("resources/objects/sphere.obj", 1.0f);
	robot->addChild(bodyT);
	bodyT->addChild(body);

	// head
	transform = glm::mat4(1.0f);
	transform *= glm::translate(glm::vec3(0.0f, 1.2f, 0.0f));
	headT = new Transform(transform);
	head = new Geometry("resources/objects/head_s.obj", 1.0f, glm::vec3(1.0f, 0.0f, 0.5f));
	robot->addChild(headT);
	headT->addChild(head);

	// eyes
	transform = glm::mat4(1.0f);
	transform *= glm::translate(glm::vec3(0.0f, 0.0f, 0.8f));
	transform *= glm::scale(glm::vec3(0.2f, 0.2f, 0.2f));
	eye1 = new Transform(transform);
	transform = glm::mat4(1.0f);
	transform *= glm::translate(glm::vec3(0.3f, -0.2f, 0.8f));
	transform *= glm::scale(glm::vec3(0.1f, 0.1f, 0.1f));
	eye2 = new Transform(transform);
	eye = new Geometry("resources/objects/eyeball_s.obj", 1.0f, glm::vec3(1.0f));
	headT->addChild(eye1);
	eye1->addChild(eye);
	headT->addChild(eye2);
	eye2->addChild(eye);


	// antenna
	transform = glm::mat4(1.0f);
	transform *= glm::translate(glm::vec3(0.0f, 0.35f, -0.3f));
	transform *= glm::scale(glm::vec3(0.3f, 0.4f, 0.3f));
	antenna1 = new Transform(transform);
	transform = glm::mat4(1.0f);
	transform *= glm::translate(glm::vec3(0.2f, 0.4f, -0.2f));
	transform *= glm::scale(glm::vec3(0.5f, 0.4f, 0.5f));
	antenna2 = new Transform(transform);
	antenna = new Geometry("resources/objects/antenna_s.obj", 1.0f, glm::vec3(1.0f));
	headT->addChild(antenna1);
	antenna1->addChild(antenna);
	headT->addChild(antenna2);
	antenna2->addChild(antenna);

	// limb w/ lighter
	transform = glm::mat4(1.0f);
	transform *= glm::translate(glm::vec3(0.5f, 0.2f, 0.8f));
	transform *= glm::scale(glm::vec3(0.8f, 0.5f, 0.8f));
	transform *= glm::rotate(glm::mat4(1.0f), glm::radians(120.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	limb1 = new Transform(transform);
	transform = glm::mat4(1.0f);
	transform *= glm::translate(glm::vec3(0.0f, -1.0f, 0.0f));
	transform *= glm::rotate(glm::mat4(1.0f), glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	transform *= glm::scale(glm::vec3(0.8f, 0.3f, 0.8f));
	transform *= glm::translate(glm::vec3(0.0f, -0.3f, 0.0f));
	limb2 = new Transform(transform);
	limb = new Geometry("resources/objects/antenna_s.obj", 1.0f, glm::vec3(1.0f));
	robot->addChild(limb1);
	limb1->addChild(limb);
	limb1->addChild(limb2);
	limb2->addChild(limb);

	transform = glm::mat4(1.0f);
	transform *= glm::translate(glm::vec3(-0.5f, 0.2f, 0.8f));
	transform *= glm::scale(glm::vec3(0.8f, 0.5f, 0.8f));
	transform *= glm::rotate(glm::mat4(1.0f), glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	limb3 = new Transform(transform);
	transform = glm::mat4(1.0f);
	transform *= glm::translate(glm::vec3(0.0f, 1.0f, 0.0f));
	transform *= glm::rotate(glm::mat4(1.0f), glm::radians(120.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	transform *= glm::scale(glm::vec3(0.8f, 0.3f, 0.8f));
	transform *= glm::translate(glm::vec3(0.0f, -0.3f, 0.0f));
	limb4 = new Transform(transform);
	robot->addChild(limb3);
	limb3->addChild(limb);
	limb3->addChild(limb4);
	limb4->addChild(limb);

	robotSquad = new Transform(glm::mat4(1.0f));
	for (float i = 0.0f; i < 5.0f; i += 1.0f)
		for (float j = 0.0f; j < 5.0f; j += 1.0f)
		{
			transform = glm::mat4(1.0f);
			transform *= glm::translate(glm::vec3(3.0f*(2.0f-i), 0.0f, 3.0f*(2.0f-j)));
			Transform* newRobot = new Transform(transform);
			robotSquad->addChild(newRobot);
			newRobot->addChild(robot);
		}

	// currentObj = bunny;

	// Skybox
	skybox = new Skybox(view, projection);
	glUseProgram(skyboxShader);
	glUniform1i(skyboxLoc, 0);
	
	// Bezier Curve Track
	track = new Track();
	curves = track->getCurves();
	numCurves = curves.size();
	currentCurve = curves[curveCounter];
	points = currentCurve->getPoints();
	numPoints = points.size();

	// start robot on track
	robotSquad->translate(points[0]);

	// Water
	water = new Water(0.0f, 0.0f, 0.0f);


	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	//delete bunny;
	
	// Delete the shader programs.
	glDeleteProgram(program);
	glDeleteProgram(phongShader);
	glDeleteProgram(skyboxShader);
	glDeleteProgram(trackShader);
	glDeleteProgram(waterShader);
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
	// Perform any updates as necessary. 
	bodyT->update(glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 1.0f)));
	headT->update(glm::rotate(glm::mat4(1.0f), glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f)));
	
	if(counter < DURATION / 2)
	{
		limb1->update(glm::rotate(glm::mat4(1.0f), glm::radians(-0.2f), glm::vec3(0.0f, 0.0f, 1.0f)));
		limb3->update(glm::rotate(glm::mat4(1.0f), glm::radians(0.2f), glm::vec3(0.0f, 0.0f, 1.0f)));
	}
	if (counter >= DURATION / 2)
	{
		limb1->update(glm::rotate(glm::mat4(1.0f), glm::radians(0.2f), glm::vec3(0.0f, 0.0f, 1.0f)));
		limb3->update(glm::rotate(glm::mat4(1.0f), glm::radians(-0.2f), glm::vec3(0.0f, 0.0f, 1.0f)));
	}
	counter++;
	if (counter >= DURATION)
		counter = 0;
		
	// Move squad along track
	// counter for bezier curve
	// counter for each point on bezier curve
	if (pointCounter+1 >= numPoints)
	{
		glm::vec3 lastPoint = points[pointCounter];

		pointCounter = 0;
		curveCounter++;
		if (curveCounter >= numCurves)
			curveCounter = 0;
		currentCurve = curves[curveCounter];
		points = currentCurve->getPoints();

		direction = points[pointCounter] - lastPoint;
		robotSquad->translate(direction);
	}
	else 
	{
		direction = points[pointCounter + 1] - points[pointCounter];
		robotSquad->translate(direction);
		pointCounter++;
	}
}

void Window::displayCallback(GLFWwindow* window)
{
	// Time keeping
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// Process input for FPS Camera
	processInput(window);


	// Clear the color and depth buffers.
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model = glm::mat4(1.0f);
	view = camera.GetViewMatrix();
	projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);

	glUseProgram(program);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	// Render the object.
	robotSquad->draw(program, glm::mat4(1.0f));
	
	/*
	glm::vec3 lightPos = pointLight->getLightPosition();
	glm::vec3 viewPos = camera.getPosition();
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));
	glUniform1i(normalColorModeLoc, normalColorMode);

	// directional light properties
	glm::vec3 lightDirection = directionalLight->getDirection();
	glm::vec3 dirLightColor = directionalLight->getColor();
	glm::vec3 diffuseColor = dirLightColor * glm::vec3(0.5f); // decrease the influence
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
	glm::vec3 specularColor = glm::vec3(1.0f);

	if (!directionalLightOn)
	{
		ambientColor = glm::vec3(0.0f);
		diffuseColor = glm::vec3(0.0f);
		specularColor = glm::vec3(0.0f);
	}

	glUniform3fv(dirLightAmbientLoc, 1, glm::value_ptr(ambientColor));
	glUniform3fv(dirLightDiffuseLoc, 1, glm::value_ptr(diffuseColor));
	glUniform3fv(dirLightSpecularLoc, 1, glm::value_ptr(specularColor));

	// point light properties
	glm::vec3 lightColor = pointLight->getLightColor();
	diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
	ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
	specularColor = glm::vec3(1.0f);
	glUniform3fv(lightAmbientLoc, 1, glm::value_ptr(ambientColor)); 
	glUniform3fv(lightDiffuseLoc, 1, glm::value_ptr(diffuseColor));
	glUniform3fv(lightSpecularLoc, 1, glm::value_ptr(specularColor));

	//material properties
	glm::vec3 materialAmbient = currentObj->getAmbient();
	glm::vec3 materialDiffuse = currentObj->getDiffuse();
	glm::vec3 materialSpecular = currentObj->getSpecular();
	float shininess = currentObj->getShininess();

	glUniform3fv(materialAmbientLoc, 1, glm::value_ptr(materialAmbient));
	glUniform3fv(materialDiffuseLoc, 1, glm::value_ptr(materialDiffuse));
	glUniform3fv(materialSpecularLoc, 1, glm::value_ptr(materialSpecular));
	glUniform1f(materialShininessLoc, shininess);
	
	//glm::mat4 model = currentObj->getModel();
	//glm::vec3 color = currentObj->getColor();

	glm::mat4 model = glm::mat4(1.0f);
	glUniformMatrix4fv(phongProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(phongViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(phongModelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//glUniform3fv(colorLoc, 1, glm::value_ptr(color));

	glUseProgram(program);
	model = pointLight->getModel();
	glm::vec3 color = pointLight->getLightColor();
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));
	pointLight->draw();
	*/

	// Draw Track
	glUseProgram(trackShader);
	model = glm::mat4(1);
	view = camera.GetViewMatrix();
	projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
	glm::vec3 color(0.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(trackModelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(trackViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(trackProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(trackColorLoc, 1, glm::value_ptr(color));
	//track->draw(trackShader);

	// Water
	glUseProgram(waterShader);
	model = water->getModel();
	view = camera.GetViewMatrix();
	projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
	glUniformMatrix4fv(waterProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(waterViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(waterModelLoc, 1, GL_FALSE, glm::value_ptr(model));
	water->draw(waterShader);

	// Skybox
	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	glUseProgram(skyboxShader);
	view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
	glUniformMatrix4fv(sb_viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(sb_projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	//skybox->draw(skyboxShader);
	glDepthFunc(GL_LESS); // set depth function back to default

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
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
			// Only Directional light(rotating)
			directionalLightOn = true;
			pointLightOn = false;
			break;
		case GLFW_KEY_2:
			// Directional and Point Light. Toggle mouse movement for point light
			directionalLightOn = true;
			pointLightOn = true;
			pointLightMovement = !pointLightMovement;
			break;
		case GLFW_KEY_3:
			// Toggle Directional Light
			directionalLightOn = !directionalLightOn;
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
		case GLFW_KEY_P:
			//currentObj->updatePointSize(0.8f);
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
