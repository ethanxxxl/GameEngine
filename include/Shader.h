#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>
#include <string>

class Shader
{
private:
	// this is a list of all the shaders that will be made into the program
	//  when createProgram is called
	std::vector<GLuint> shaderObjects;
	
	unsigned int shaderProgram;

public:
	/// adds shaders to the list and compiles them.
	void grabShader(GLenum shaderType, std::string filename);

	// attaches the compiled shaders from the list, links the program, and
	//  clears out the shaderObjects vector.
	void createProgram();

	void useProgram();

	int getUniformLocation(std::string name);

	unsigned int getID();
};

#endif
