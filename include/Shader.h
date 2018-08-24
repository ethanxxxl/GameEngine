#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>
#include <tuple>
#include <map>
#include <iostream>

#include <Uniform.h>

class Shader
{
private:
	// this is a list of all the shaders that will be made into the program
	//  when createProgram is called
	std::vector<GLuint> shader_objects;
	
	/// the handle for the shader program
	unsigned int shader_program;

	/// names of all of the unforms in the shader program
	typedef std::map<std::string, std::pair<int, GLenum>> uniform_list_t;
	uniform_list_t uniforms;

	/// simple helper function for less typing
	int getUniLoc(std::string name);

	///
	std::string matrix_uniform_name;

public:
	/// makes sure the shader has a matrix uniform variable.
	Shader(std::string name);
	~Shader();

	/// adds shaders to the list and compiles them.
	void grabShader(GLenum shaderType, std::string filename);

	// attaches the compiled shaders from the list, links the program, and
	//  clears out the shader_objects vector.
	void createProgram();

	/// activates the program
	void useProgram();

	unsigned int getID();

	const std::string getMatrixUniformName();

	template <typename T>
	void setUniform(std::string name, T data)
	{
		glUseProgram(shader_program);
		// just store the location of the uniform we are setting
		int location = glGetUniformLocation(shader_program, name.c_str());
		// initialize the uniform handler
		els_core::UniformHandler<T> handler;
		// set the uniform data
		handler.setUniform(location, data);
		std::cout << "The location of " << name << " is: " << location << std::endl;
	}
};

#endif
