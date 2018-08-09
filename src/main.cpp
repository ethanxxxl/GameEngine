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

// this function gets called every time the window is resized, and so the
//  viewport will be updated everytime the windows size changes.
float mixer = 0.5f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

int main()
{
	/*
	 * SETUP
	 */
	glfwInit();
	
	// the following lines are just basic configuration.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

	/*
	 * SHADERS
	 */

	Shader shaderProgram;
	shaderProgram.grabShader(GL_VERTEX_SHADER, "shaders/vertexshader.glsl");
	shaderProgram.grabShader(GL_FRAGMENT_SHADER, "shaders/fragmentshader.glsl");
	shaderProgram.createProgram();

	/*
	 * Draw Data
	 */

	std::vector<float> vertices = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

	std::vector<unsigned int> indices =
	{
		0, 1, 3,
		1, 2, 3
	};

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)(800 / 600), 0.1f, 100.0f);

	glm::mat4 view;
	view = glm::lookAt(glm::vec3(2, 2, 2), glm::vec3(0, 0, 0), glm::vec3(0,0,1));

	Body cube;
	cube.setVertices(vertices);
	cube.setShaderProgram(shaderProgram);
	cube.load();
	cube.setVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	cube.getClipMatrix(projection, view);


	/*
	 * TRANSFORMATIONS
	 */
	
	while ( !glfwWindowShouldClose(window) )
	{
		/*
		 * RENDER COMMANDS HERE
		 */

		glClearColor(0.16f, 0.23f, 0.35f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cube.getClipMatrix(projection, view);
		cube.draw(36, "clip_matrix");

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
