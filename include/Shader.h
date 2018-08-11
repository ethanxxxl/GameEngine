#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <tuple>
#include <map>

class Shader
{
private:
	// this is a list of all the shaders that will be made into the program
	//  when createProgram is called
	std::vector<GLuint> shaderObjects;
	
	/// the handle for the shader program
	unsigned int shaderProgram;

	/// names of all of the unforms in the shader program
	typedef std::map<std::string, std::pair<int, GLenum>> uniform_list_t;
	uniform_list_t uniforms;

	inline int getUniLoc(std::string name);

public:
	/// adds shaders to the list and compiles them.
	void grabShader(GLenum shaderType, std::string filename);

	// attaches the compiled shaders from the list, links the program, and
	//  clears out the shaderObjects vector.
	void createProgram();

	void useProgram();

	unsigned int getID();

	int getUniformLocation(std::string name);

	void genUniformList();
	
	const uniform_list_t& getUniforms();


	/// changes a uniform variable.
	template <typename T>
	void accessUniform(std::string uniform_name, T data);

	/// changes a matrix uniform variable.
	template <typename T>
	void accessUniform(std::string name, GLsizei count, T);

};
#endif
