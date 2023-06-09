#include <iostream>
#include <sstream>
#include <string>
#define GLEW_STATIC
#ifdef __APPLE__
#include <glad/glad.h>
#else
#include "GL/glew.h"
#endif

#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Mesh.h"
#include "../src/Lista.h"

// Global Variables
const char* APP_TITLE = "Proyecto Final - Estructura de Datos";
int gWindowWidth = 1024;
int gWindowHeight = 768;
GLFWwindow* gWindow = NULL;
bool gWireframe = false;
Lista* lista = new Lista();
glm::vec3  posicion;
glm::vec3  escala;
float xCaja = -5.0f;
const float DISTANCIA_CAJAS = -2.5f;

FPSCamera fpsCamera(glm::vec3(0.0f, 3.0f, 10.0f));
const double ZOOM_SENSITIVITY = -3.0;
const float MOVE_SPEED = 5.0; // units per second
const float MOUSE_SENSITIVITY = 0.1f;

// Function prototypes
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY);
void update(double elapsedTime);
void showFPS(GLFWwindow* window);
bool initOpenGL();
void agregarCaja();
void removerCaja(int indice);
bool init = false;

void agregarCaja()
{
	xCaja -= DISTANCIA_CAJAS;
	Mesh* caja = new Mesh();
	posicion = glm::vec3(xCaja, 1.0f, 0.0f);
	escala = glm::vec3(1.0f, 1.0f, 1.0f);
	caja->configure("textures/crate.jpg", posicion , escala);
	caja->load("models/crate.obj");
	lista->agregar(new Elemento(caja));
}

void removerCaja(int indice)
{
	lista->eliminar(indice);
}

//-----------------------------------------------------------------------------
// Main Application Entry Point
//-----------------------------------------------------------------------------
int iniciar()
{
	if (!initOpenGL())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return -1;
	}
	ShaderProgram shaderProgram;
	shaderProgram.loadShaders("shaders/basic.vert", "shaders/basic.frag");


	std::vector<MeshInterface*> models;

	Mesh robot;
	glm::vec3  position = glm::vec3(0.0f, 0.0f, -6.0f);
	glm::vec3  scale = glm::vec3(1.0f, 1.0f, 1.0f);
	robot.configure("textures/robot_diffuse.jpg", position , scale);
	robot.load("models/robot.obj");

	Mesh tree;
	position = glm::vec3(4.0f, 0.0f, 2.0f);
	scale = glm::vec3(4.0f, 4.0f, 4.0f);
	tree.configure("textures/tree.png", position , scale);
	tree.load("models/tree.obj");


	Mesh floor;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(10.0f, 1.0f, 10.0f);
	floor.configure("textures/tile_floor.jpg", position , scale);
	floor.load("models/floor.obj");

	Mesh crate;
	position = glm::vec3(-2.5f, 1.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	crate.configure("textures/crate.jpg", position , scale);
	crate.load("models/crate.obj");

	models.push_back(&robot);
	models.push_back(&floor);
	models.push_back(&tree);
	// models.push_back(&crate);

	double lastTime = glfwGetTime();
	double currentTime = 0.0;
	double deltaTime = 0.0;
	glm::mat4 model, view, projection;
	
	// Rendering loop
	while (!glfwWindowShouldClose(gWindow))
	{
		showFPS(gWindow);
		if(!init)
		{
			int x,y;
			glfwGetWindowPos(gWindow,&x,&y);
			glfwSetWindowPos(gWindow, x+1,y);
			init = true;
		}

		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;

		// Poll for and process events
		glfwPollEvents();
		update(deltaTime);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Create the View matrix
		view = fpsCamera.getViewMatrix();

		// Create the projection matrix
		projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 200.0f);

		// Must be called BEFORE setting uniforms because setting uniforms is done
		// on the currently active shader program.
		shaderProgram.use();

		// Pass the matrices to the shader
		shaderProgram.setUniform("view", view);
		shaderProgram.setUniform("projection", projection);

		// Render the scene
		for (int i = 0; i < models.size(); i++)
		{
			model = glm::translate(glm::mat4(), models[i]->getPosition()) * glm::scale(glm::mat4(), models[i]->getScaling());
			shaderProgram.setUniform("model", model);

			models[i]->getTexture().bind();		// set the texture before drawing.  Our simple OBJ mesh loader does not do materials yet.	
			models[i]->draw();
		}
		Elemento* i = lista -> getPrimer();

		while(i)
		{
			Mesh* modelo = i->getModelo();
			model = glm::translate(glm::mat4(), modelo->getPosition()) * glm::scale(glm::mat4(), modelo->getScaling());
			shaderProgram.setUniform("model", model);

			modelo->getTexture().bind();		// set the texture before drawing.  Our simple OBJ mesh loader does not do materials yet.	
			modelo->draw();
			i = i -> getSiguiente();
		}

		// Swap front and back buffers
		glfwSwapBuffers(gWindow);

		lastTime = currentTime;
	}

	glfwTerminate();
	return 0;
}

//-----------------------------------------------------------------------------
// Initialize GLFW and OpenGL
//-----------------------------------------------------------------------------
bool initOpenGL()
{
	// Intialize GLFW 
	// GLFW is configured.  Must be called before calling any GLFW functions
	if (!glfwInit())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// forward compatible with newer versions of OpenGL as mody become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3
	#endif

	// Create an OpenGL 3.3 core, forward compatible context window
	gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	if (gWindow == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make the window's context the current one
	glfwMakeContextCurrent(gWindow);

	#ifdef __APPLE__
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }	
	#else
	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return false;
	}
	#endif

	// Set the required callback functions
	glfwSetKeyCallback(gWindow, glfw_onKey);
	glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);
	glfwSetScrollCallback(gWindow, glfw_onMouseScroll);

	// Hides and grabs cursor, unlimited movement
	glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
	// Define the viewport dimensions
	glViewport(0, 0, gWindowWidth, gWindowHeight);
	glEnable(GL_DEPTH_TEST);
	return true;
}

//-----------------------------------------------------------------------------
// Is called whenever a key is pressed/released via GLFW
//-----------------------------------------------------------------------------
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
	{
		gWireframe = !gWireframe;
		if (gWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		printf("Agregar\n");
		agregarCaja();
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		printf("Remover\n");
	}

	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		removerCaja(0);
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		removerCaja(1);
	}

	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		removerCaja(2);
	}

	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		removerCaja(3);
	}

	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		removerCaja(4);
	}

	if (key == GLFW_KEY_6 && action == GLFW_PRESS)
	{
		removerCaja(5);
	}

	if (key == GLFW_KEY_7 && action == GLFW_PRESS)
	{
		removerCaja(6);
	}

	if (key == GLFW_KEY_8 && action == GLFW_PRESS)
	{
		removerCaja(7);
	}

	if (key == GLFW_KEY_9 && action == GLFW_PRESS)
	{
		removerCaja(8);
	}
}

//-----------------------------------------------------------------------------
// Is called when the window is resized
//-----------------------------------------------------------------------------
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height)
{
	gWindowWidth = width;
	gWindowHeight = height;
	glViewport(0, 0, gWindowWidth, gWindowHeight);
}

//-----------------------------------------------------------------------------
// Called by GLFW when the mouse wheel is rotated
//-----------------------------------------------------------------------------
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY)
{
	double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;

	fov = glm::clamp(fov, 1.0, 120.0);

	fpsCamera.setFOV((float)fov);
}

//-----------------------------------------------------------------------------
// Update stuff every frame
//-----------------------------------------------------------------------------
void update(double elapsedTime)
{
	// Camera orientation
	double mouseX, mouseY;

	// Get the current mouse cursor position delta
	glfwGetCursorPos(gWindow, &mouseX, &mouseY);

	// Rotate the camera the difference in mouse distance from the center screen.  Multiply this delta by a speed scaler
	fpsCamera.rotate((float)(gWindowWidth / 2.0 - mouseX) * MOUSE_SENSITIVITY, (float)(gWindowHeight / 2.0 - mouseY) * MOUSE_SENSITIVITY);

	// Clamp mouse cursor to center of screen
	glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

	// Camera FPS movement

	// Forward/backward
	if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(gWindow, GLFW_KEY_UP) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getLook());
	else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(gWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getLook());

	// Strafe left/right
	if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(gWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());
	else if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(gWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());

	// Up/down
	if (glfwGetKey(gWindow, GLFW_KEY_Z) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getUp());
	else if (glfwGetKey(gWindow, GLFW_KEY_X) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getUp());
}

//-----------------------------------------------------------------------------
// Code computes the average frames per second, and also the average time it takes
// to render one frame.  These stats are appended to the window caption bar.
//-----------------------------------------------------------------------------
void showFPS(GLFWwindow* window)
{
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started, as double float

	elapsedSeconds = currentSeconds - previousSeconds;

	// Limit text updates to 4 times per second
	if (elapsedSeconds > 0.25)
	{
		previousSeconds = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		// The C++ way of setting the window title
		std::ostringstream outs;
		outs.precision(3);	// decimal places
		outs << std::fixed
			<< APP_TITLE << "    "
			<< "FPS: " << fps << "    "
			<< "Frame Time: " << msPerFrame << " (ms)";
		glfwSetWindowTitle(window, outs.str().c_str());

		// Reset for next average.
		frameCount = 0;
	}

	frameCount++;
}
