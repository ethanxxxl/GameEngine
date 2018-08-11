#include <Shader.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

void Shader::grabShader(GLenum shaderType, std::string filename)
{
	// creates an fstream object for the shader source. it also opens the file
	//  for reading.
	std::fstream shaderSourceFile(filename.c_str(), std::fstream::in);

	// this will take data from the file and bring it into something we can work
	//  with
	std::stringstream shaderSourceStream;
	std::string       shaderSource; // this is so we can actually use the data

	// moves the source code from the file to the stringstream
	shaderSourceStream << shaderSourceFile.rdbuf();
	// moves the source code from the stringstream into the shaderSource
	//  variable
	shaderSource = shaderSourceStream.str();
	// this essentially converts the std::string to a c_str so that we can make
	//  a double pointer out of it for use in an openGL function
	const char *shaderSource_c_str = shaderSource.c_str();

	// create the shader object
	unsigned int shaderObject;
	shaderObject = glCreateShader(shaderType);

	// adds the shader source to the shader
	glShaderSource(shaderObject, 1, &shaderSource_c_str, NULL);
	// compile the shader
	glCompileShader(shaderObject);

	// error checking
	int success;
	char infoLog[512];
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);

	if ( !success )
	{
		glGetShaderInfoLog(shaderObject, 512, NULL, infoLog);
		std::cout << "ERROR: shader compilation failed\n" << infoLog << std::endl;
	}
	
	// adds the shader object to the list of shaders that are xn the "system"
	shaderObjects.push_back(shaderObject);
	
	// closes the file because we are now done with it.
	shaderSourceFile.close();
}

void Shader::createProgram()
{
	// create the shader program
	shaderProgram = glCreateProgram();

	// this loops through all the shader object identifiers and attaches them to
	//  the shader program
	for ( int i = 0; i < shaderObjects.size(); i++)
	{
		glAttachShader(shaderProgram, shaderObjects[i]);
	}

	glLinkProgram(shaderProgram);

	// error checking
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if ( !success )
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Error: program linking failed\n" << infoLog << std::endl;
	}
	// deletes the shaders because we are no longer using them.
	for ( int i = 0; i < shaderObjects.size(); i++)
	{
		glDeleteShader(shaderObjects[i]);
	}

	// destroys all the data in the vector because the handles are not attached
	//  to anything anymore.
	shaderObjects.clear();

	// returns the newely created shader program
}

void Shader::useProgram()
{
	glUseProgram(shaderProgram);
}

int Shader::getUniformLocation(std::string name)
{
	return glGetUniformLocation(shaderProgram, name.c_str());
}

unsigned int Shader::getID()
{
	return shaderProgram;
}

void Shader::genUniformList()
{
	// gets the total number of active uniforms.
	int active_uniforms;
	glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &active_uniforms);
	// maximum size that will need allocated to store the longest uniform
	//  variables name.
	int buffer_size;
	glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &buffer_size);

	// the size of the uniform variable in the shader program
	int uniform_size;
	// the type of the uniform in the shader program.
	GLenum uniform_type;
	// the name of the uniform in the shader program.
	std::string uniform_name;
	uniform_name.reserve(buffer_size);
	
	for ( unsigned int i = 0; i < active_uniforms; i++ )
	{
		glGetActiveUniform(shaderProgram,
						   i,
						   buffer_size,
						   NULL,
						   &uniform_size,
						   &uniform_type,
						   &uniform_name[0]
						   );

		// add the uniform info to the list.
		uniforms.emplace(uniform_name, std::make_pair(uniform_size, uniform_type));
	}
}

const Shader::uniform_list_t& Shader::getUniforms()
{
	return uniforms;
}

int Shader::getUniLoc(std::string name)
{
	return glGetUniformLocation(shaderProgram, name.c_str());
}

/*
 * Vector Uniforms
 */
template <typename T>
void Shader::accessUniform(std::string uniform_name, T data)
{
	std::cout << "Error!: no specialized function for the vector type that you\
 are trying to use!" << std::endl;
}

template <>
void Shader::accessUniform(std::string name, float data)
{
	glUniform1f(getUniLoc(name), data);
}


template <>
void Shader::accessUniform(std::string name, glm::vec2 data)
{
	glUniform2f(getUniLoc(name), data.x, data.y);
}

template <>
void Shader::accessUniform(std::string name, glm::vec3 data)
{
	glUniform3f(getUniLoc(name), data.x, data.y, data.z);
}

template <>
void Shader::accessUniform(std::string name, glm::vec4 data)
{
	glUniform4f(getUniLoc(name), data.x, data.y, data.z, data.w);
}

template <>
void Shader::accessUniform(std::string name, int data)
{
	glUniform1i(getUniLoc(name), data);
}

template <>
void Shader::accessUniform(std::string name, glm::ivec2 data)
{
	glUniform2i(getUniLoc(name), data.x, data.y);
}

template <>
void Shader::accessUniform(std::string name, glm::ivec3 data)
{
	glUniform3i(getUniLoc(name), data.x, data.y, data.z);
}

template <>
void Shader::accessUniform(std::string name, glm::ivec4 data)
{
	glUniform4i(getUniLoc(name), data.x, data.y, data.z, data.w);
}

/*
 * Matrix Uniforms
 */

template <typename T>
void Shader::accessUniform(std::string name, GLsizei count, T)
{
	std::cout << "Error!: no specialized function for the matrix type that you\
 are trying to use!" << std::endl;
}

template <>
void Shader::accessUniform(std::string name, GLsizei count, glm::mat2 data)
{
	glUniformMatrix2fv(getUniLoc(name), count, false, glm::value_ptr(data));
}

template <>
void Shader::accessUniform(std::string name, GLsizei count, glm::mat3 data)
{
	glUniformMatrix3fv(getUniLoc(name), count, false, glm::value_ptr(data));
}

template <>
void Shader::accessUniform(std::string name, GLsizei count, glm::mat4 data)
{
	glUniformMatrix4fv(getUniLoc(name), count, false, glm::value_ptr(data));
}

template <>
void Shader::accessUniform(std::string name, GLsizei count, glm::mat2x3 data)
{
	glUniformMatrix2x3fv(getUniLoc(name), count, false, glm::value_ptr(data));
}

template <>
void Shader::accessUniform(std::string name, GLsizei count, glm::mat2x4 data)
{
	glUniformMatrix2x4fv(getUniLoc(name), count, false, glm::value_ptr(data));
}

template <>
void Shader::accessUniform(std::string name, GLsizei count, glm::mat3x2 data)
{
	glUniformMatrix3x2fv(getUniLoc(name), count, false, glm::value_ptr(data));
}

template <>
void Shader::accessUniform(std::string name, GLsizei count, glm::mat3x4 data)
{
	glUniformMatrix3x4fv(getUniLoc(name), count, false, glm::value_ptr(data));
}

template <>
void Shader::accessUniform(std::string name, GLsizei count, glm::mat4x2 data)
{
	glUniformMatrix3x4fv(getUniLoc(name), count, false, glm::value_ptr(data));
}

template <>
void Shader::accessUniform(std::string name, GLsizei count, glm::mat4x3 data)
{
	glUniformMatrix3x4fv(getUniLoc(name), count, false, glm::value_ptr(data));
}
