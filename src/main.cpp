#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Body.h>
#include <Shader.h>
#include <Camera.h>

// this function gets called every time the window is resized, and so the
//  viewport will be updated everytime the windows size changes.
float mixer = 0.5f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

int main()
{
	glfwInit();
	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	
	// the following lines are just basic configuration.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// this is the initial size of the area that will be rendered (inside the
	//  window)
	glViewport(0,0,800,600);

	// sets the callback function to be called everytime that the user resizes
	//  the window.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);


	Shader lamp_program("clip_matrix");
	lamp_program.grabShader(GL_VERTEX_SHADER, "shaders/default/vertexshader.glsl");
	lamp_program.grabShader(GL_FRAGMENT_SHADER, "shaders/lampFragment.glsl");
	lamp_program.createProgram();

	Shader cube_program("clip_matrix");
	cube_program.grabShader(GL_VERTEX_SHADER, "shaders/lightvertex.glsl");
	cube_program.grabShader(GL_FRAGMENT_SHADER, "shaders/lightfragment.glsl");
	cube_program.createProgram();

	std::vector<float> vertices =
	{
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

	// color vectors
	glm::vec3 light_color(1.0f, 1.0f, 1.0f);
	glm::vec3 cube_color(1.0f, 0.5f, 0.31f);

	// creating the cube
	Body cube(&vertices, GL_STATIC_DRAW);
	cube.setVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	cube.setVertexAttribute(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)3);
	cube.setShaderProgram(cube_program);

	// creating the lamp
	Body lamp(&vertices, GL_STATIC_DRAW);
	lamp.setVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	lamp.setShaderProgram(lamp_program);
	
	glm::vec3 light_pos(0.5, 0.25, -1);
	// Setting the cube uniform variables
	cube_program.setUniform<glm::vec3>("light_color", light_color);
	cube_program.setUniform<glm::vec3>("object_color", cube_color);
	cube_program.setUniform<glm::vec3>("light_pos", light_pos);

	lamp.translate(light_pos);
	lamp.scale((glm::vec3(0.2f)));

	// creating and setting up the camera
	Camera cam1(glm::vec3(2.0f, 1.5f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), 45.0f, 800 / 600, 0.01f, 10.0f);
	cam1.activate();
	cam1.addBody(&cube);
	cam1.addBody(&lamp);

	while ( !glfwWindowShouldClose(window) )
	{
		/*
		 * RENDER COMMANDS HERE
		 */

		glClearColor(0.17f, 0.17f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		cam1.drawAll();

		// input handler
		processInput(window);

		// glfw stuff
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0,0, width, height);
}

void processInput(GLFWwindow *window)
{
	if ( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )
		glfwSetWindowShouldClose(window, true);
	
	if ( glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS )
	{
		mixer += (mixer < 1.0f) ? 0.01f : 0.0f;
	}
	

	if ( glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS )
	{
		mixer -= (mixer > 0.0f) ? 0.01f : 0.0f;
	}
}
