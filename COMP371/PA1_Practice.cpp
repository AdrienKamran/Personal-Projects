#include <iostream>
#include <chrono>
#include <vector>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "OpenGLWindow.h"
#include "Shader.h"
#include "UI_InputManager.h"
#include "Camera.h"
#include "Cube.h"
#include "Grid.h"
#include "CoordinateLines.h"

// Constants for defining window size
const float SCR_WIDTH = 1440.0, SCR_HEIGHT = 900.0;

// We keep track of the time between frames so that camera movement can be calculated independent of frame rate
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

// Create a new OpenGLWindow
OpenGLWindow * mainWindow = new OpenGLWindow(SCR_WIDTH, SCR_HEIGHT);
// Associate a GLFWindow
GLFWwindow * mWind = mainWindow->glWindow();

// Instantiate the camera at 0,2,20 so the first model is visible. Maybe better to just start at 0,0,0, but then the grid isn't visible and we are inside of a model.
Camera camera(mainWindow, glm::vec3(0, 2, 10));

// Instantiate the shaders. The shader class takes care of reading them
Shader gridShader("../Shaders/basicShader2.vs", "../Shaders/basicShader2.fs");
Shader nicksCubeShader("../Shaders/nick_CubeShader.vs", "../Shaders/nick_CubeShader.fs");
Shader lineShader("../Shaders/lineShader.vs", "../Shaders/lineShader.fs");

int main() {

	// Instantiate a UI input manager. In UI_InputManager.h we handle all inputs, passing some information on to the Camera object (movement, looking around, etc.)
	UI_InputManager UI_InputManager(mWind, &camera);

	float cFrame;
	float time;

	int frames = 0;

	// 100x100 square grid = 101x101 vertices since 1 extra vertex is needed to define the last square. If we do 100x100 vertices there will only be 99x99 squares.
	Grid squareGrid(101,101);

	// Just budge the grid down  a unit so it's below us
	squareGrid.setTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.001f, 0.0f)));
	
	// Scale the grid up 100x.
	*squareGrid.getTransformMatrix() = glm::scale(*squareGrid.getTransformMatrix(), glm::vec3(100.0f));

	// Pass the grid's model and transform matrices to its shader, since they won't need to be updated in the render loop (they're static)
	// You have to call shader.use() before doing these operations, which will call glUseProgram(this shader's ID #) and tell OpenGL we are using it.
	gridShader.use();
	gridShader.setMat4("transform", *squareGrid.getTransformMatrix());
	gridShader.setMat4("model", *squareGrid.getModelMatrix());
	gridShader.setMat4("projection", *camera.GetProjectionMatrix());

	Line lx(0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f);
	
	
	// (Nick) Prepare the cube and its shader
	// Create a new cube object. We can all re-use the same cube since its transform matrix keeps getting reset after use.
	Cube c(1.0f);
	nicksCubeShader.use();
	nicksCubeShader.setMat4("model", *c.getModelMatrix());


	while (!glfwWindowShouldClose(mWind))
	{
		mainWindow->clearColor(0.235, 0.364, 0.349, 1.0);


		frames++;
		cFrame = glfwGetTime(); // Time of the current frame
		deltaTime = cFrame - lastFrame; // Time difference between frames
		//std::cout << "FPS: " << (1.0 / deltaTime) << std::endl; // Should return the FPS if you want to see it. Just printing the statement slows the render loop though.
		lastFrame = cFrame;

		// Draw the grid
		// We use the grid's shader and tell it the camera's current view matrix as well as the time
		gridShader.use();
	
		gridShader.setFloat("scalingFactor", UI_InputManager.getScalingFactor());
		gridShader.setMat4("projection", *camera.GetProjectionMatrix());
		gridShader.setMat4("view", camera.GetViewMatrix());
		gridShader.setFloat("time", cFrame);
		
		squareGrid.Draw();

		// DRAW THE COORDINATE AXES
		lineShader.use();
		lineShader.setMat4("projection", *camera.GetProjectionMatrix());
		lineShader.setMat4("view", camera.GetViewMatrix());
		lineShader.setMat4("model", glm::mat4(1.0f));
		lineShader.setMat4("transform", glm::mat4(1.0f));
		lineShader.setFloat("time", cFrame);
		lineShader.setColor(0.0f, 1.0f, 0.0f);
		lx.Draw();
		lx.setTransform(glm::rotate(*lx.getTransformMatrix(), -1.5708f, glm::vec3(0, 0, 1)));
		lineShader.setMat4("transform", *lx.getTransformMatrix());
		lineShader.setColor(1.0f, 0.0f, 0.0f);
		lx.Draw();
		lx.resetTransform();
		lx.setTransform(glm::rotate(*lx.getTransformMatrix(), 1.5708f, glm::vec3(1, 0, 0)));
		lineShader.setMat4("transform", *lx.getTransformMatrix());
		lineShader.setColor(0.0f, 0.0f, 1.0f);
		lx.Draw();
		lx.resetTransform();



		////////////////////////////////////////////////////////////////////////////////////

		// Draw Nick's model in Cubes (C3)
		nicksCubeShader.use();
		// Pass in the camera's projection matrix again to catch zooming events
		nicksCubeShader.setMat4("projection", *camera.GetProjectionMatrix());

		// Pass in the model scaling factor from the instructions
		nicksCubeShader.setFloat("scalingFactor", UI_InputManager.getScalingFactor());
		nicksCubeShader.setMat4("view", camera.GetViewMatrix());
		nicksCubeShader.setFloat("time", cFrame);

		//First we draw the ' | ' of the c. Translate to 0.5 y, scale to 3x units in Y
		c.setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f,1.0f)));
		c.setTransform(glm::translate(*c.getTransformMatrix(), glm::vec3(-2.0f, 0.5, 0)));
		nicksCubeShader.setMat4("transform", *c.getTransformMatrix());
		c.Draw();

		// Now draw the top and bottom horizontal parts of the c. Make them 2x units in X, 1x units in Y and Z.
		c.resetTransform();
		// First, translate the cube to where you want it using glm::translate. Then scale it. The order in which you scale and translate matters.
		c.setTransform(glm::translate(*c.getTransformMatrix(), glm::vec3(-0.5f, 0.5f, 0)));
		c.setTransform(glm::scale(*c.getTransformMatrix(), glm::vec3(2.0f, 1.0f, 1.0f)));
		nicksCubeShader.setMat4("transform", *c.getTransformMatrix());
		c.Draw();

		c.setTransform(glm::translate(*c.getTransformMatrix(), glm::vec3(-0.0f, 4.0f, 0)));
		nicksCubeShader.setMat4("transform", *c.getTransformMatrix());
		c.Draw();

		// Second, we draw the ' | ' of the 3. Translate to 1.5 in X. 
		c.resetTransform();
		c.setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
		c.setTransform(glm::translate(*c.getTransformMatrix(), glm::vec3(4.5, 0.5, 0)));
		nicksCubeShader.setMat4("transform", *c.getTransformMatrix());
		c.Draw();

		// Now draw the top and bottom horizontal parts of the c. Make them 2x units in X, 1x units in Y and Z.
		c.resetTransform();
		c.setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f)));
		c.setTransform(glm::translate(*c.getTransformMatrix(), glm::vec3(1.5f, 4.5,0)));
		nicksCubeShader.setMat4("transform", *c.getTransformMatrix());
		c.Draw();

		c.setTransform(glm::translate(*c.getTransformMatrix(), glm::vec3(0.0f, -2.0, 0)));
		nicksCubeShader.setMat4("transform", *c.getTransformMatrix());
		c.Draw();


		c.setTransform(glm::translate(*c.getTransformMatrix(), glm::vec3(0.0f, -2.0, 0)));
		nicksCubeShader.setMat4("transform", *c.getTransformMatrix());
		c.Draw();
		c.resetTransform(); // Reset the cube to a unit cube.

		////////////////////////////////////////////////////////////////////////////////////

		// Now, to draw Adrien's model (Ad[r]ien, 400[9]393 -> R9)

		/*
		
		# #     # # #
		#   #   #   #
		# #     # # #
		#   #       #
		#   #   # # #

		*/

		// Draw the left column of the R
		c.setTransform(glm::scale(*c.getTransformMatrix(), glm::vec3(1.0f, 5.0f, 1.0f))); // Scale upwards to 5 cubes in height
		c.setTransform(glm::translate(*c.getTransformMatrix(), glm::vec3(-3.0f, 0.5f, -7.0f))); // Move the bar to the left by 3 units, up by 0.5, and towards the camera by 7
		nicksCubeShader.setMat4("transform", *c.getTransformMatrix());
		c.Draw();

		// Reset the transformation matrix
		c.resetTransform();

		// Draw the middle column of the R (2 cubes)
		c.setTransform(glm::translate(*c.getTransformMatrix(), glm::vec3(-2.0f, 2.5f, -7.0f))); // First cube at (-2, 2.5, 7)
		nicksCubeShader.setMat4("transform", *c.getTransformMatrix());
		c.Draw();

		c.setTransform(glm::translate(*c.getTransformMatrix(), glm::vec3(0.f, 2.f, 0.f))); // Second cube at (-2, 4.5, 7)
		nicksCubeShader.setMat4("transform", *c.getTransformMatrix());
		c.Draw();

		// Reset the transformation matrix
		c.resetTransform();

		// Draw the right column of the R (1 short vertical rectangle and 1 cube)
		c.setTransform(glm::translate(*c.getTransformMatrix(), glm::vec3(-1.0f, 3.5f, -7.0f))); // Cube at (-1, 3.5, 7)
		nicksCubeShader.setMat4("transform", *c.getTransformMatrix());
		c.Draw();

		c.resetTransform();

		c.setTransform(glm::scale(*c.getTransformMatrix(), glm::vec3(1.0f, 2.0f, 1.0f))); // Scale upwards to 2 cubes in height
		c.setTransform(glm::translate(*c.getTransformMatrix(), glm::vec3(-1.0f, 0.5f, -7.0f))); // Move the bar to the left by 1 unit, up by 0.5, and towards the camera by 7
		nicksCubeShader.setMat4("transform", *c.getTransformMatrix());
		c.Draw();

		// Reset the transformation matrix
		c.resetTransform();

		// Draw the bottom bar of the 9
		c.setTransform(glm::scale(*c.getTransformMatrix(), glm::vec3(2.0f, 1.0f, 1.0f))); // Scale to 2 cubes towards the right on the X axis
		c.setTransform(glm::translate(*c.getTransformMatrix(), glm::vec3(1.0f, 0.5f, -7.0f))); // Move the rectangle to the right by 1 unit
		nicksCubeShader.setMat4("transform", *c.getTransformMatrix());
		c.Draw();

		// Reset the transformation matrix
		c.resetTransform();
		
		// Draw the right bar of the 9
		c.setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f))); // Scale to 5 cubes in height on the Y axis
		c.setTransform(glm::translate(*c.getTransformMatrix(), glm::vec3(3.5f, 0.5f, -7.0f))); // Move the rectangle to the right by 3.5 units
		nicksCubeShader.setMat4("transform", *c.getTransformMatrix());
		c.Draw();

		// Reset the transformation matrix
		c.resetTransform();

		// Draw the vertical bar on the left of 9's loop
		c.setTransform(glm::translate(*c.getTransformMatrix(), glm::vec3(1.5f, 3.5f, -7.0f)));
		c.setTransform(glm::scale(*c.getTransformMatrix(), glm::vec3(1.0f, 3.0f, 1.0f)));
		nicksCubeShader.setMat4("transform", *c.getTransformMatrix());
		c.Draw();

		// Reset the transformation matrix
		c.resetTransform();

		// Draw the connecting cubes in the middle of 9's loop
		c.setTransform(glm::translate(*c.getTransformMatrix(), glm::vec3(2.5, 2.5f, -7.0f)));
		nicksCubeShader.setMat4("transform", *c.getTransformMatrix());
		c.Draw();

		c.setTransform(glm::translate(*c.getTransformMatrix(), glm::vec3(0.f, 2.f, 0.f)));
		nicksCubeShader.setMat4("transform", *c.getTransformMatrix());
		c.Draw();


		////////////////////////////////////////////////////////////////////////////////////

		// Poll events
		glfwPollEvents();

		// Process input w/ the UI Input Manager
		UI_InputManager.processInput(deltaTime);

		// Swap the buffers
		glfwSwapBuffers(mWind);

	}

	// Properly clean/Delete all of GLFW's resources that were allocated
	glfwTerminate();

	return -1;



}


