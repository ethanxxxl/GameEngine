#ifndef UNIFORM_H
#define UNIFORM_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

/*
 * Uniform
 *	shader program
 *	uniform name
 *	changeUniform function
 */
namespace els_core
{
	template <class T>
	class UniformHandler; // base case

	template <>
	struct UniformHandler <glm::vec3>
	{
		inline void setUniform(int location, glm::vec3 data)
		{
			glUniform3fv(location, 1, glm::value_ptr(data));
		}
	};

	template <>
	struct UniformHandler <glm::vec4>
	{
		inline void setUniform(int location, glm::vec4 data)
		{
			glUniform4fv(location, 1, glm::value_ptr(data));
		}
	};

	template <>
	struct UniformHandler <glm::mat4>
	{
		inline void setUniform(int location, glm::mat4 data)
		{
			glUniformMatrix4fv(location, 1, false, glm::value_ptr(data));
		}
	};
}

template <class T>
class Uniform
{
private:
	unsigned int shader_program;
	unsigned int uniform_location;
	std::string  uniform_name;

	els_core::UniformHandler<T> handler;

public:
//	Uniform();
//
//	Uniform(std::string name, unsigned int program)
//	{
//		uniform_name = name;
//		shader_program = program;
//		uniform_location = glGetUniformLocation(program, name.c_str());
//	}

	inline void aquire(std::string name, unsigned int program)
	{
		uniform_name = name;
		shader_program = program;
		uniform_location = glGetUniformLocation(program, name.c_str());
	}
	
	inline void setData(T data)
	{
		glUseProgram(shader_program);
		handler.setUniform(uniform_location, data);
	}

};

#endif
