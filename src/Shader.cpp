#include <Shader.h>
#include <iostream>
#include <fstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Uniform.h>

Shader::Shader(std::string name)
{
	matrix_uniform_name = name;
}

void Shader::grabShader(GLenum shader_type, std::string filename)
{
	// creates a filestream to grab the file source file and put it into a
	//  stream object.
	std::ifstream shader_source_stream(filename);
	// moves the data from the stream object and puts it into a more workable
	//  string object.
	std::string shader_source_string((std::istreambuf_iterator<char>(shader_source_stream)),
									 std::istreambuf_iterator<char>()
									);

	// this essentially converts the std::string to a c_str so that we can make
	//  a double pointer out of it for use in an openGL function
	const char *shader_source_c_str = shader_source_string.c_str();

	// create the shader object
	unsigned int shader_object;
	shader_object = glCreateShader(shader_type);

	// adds the shader source to the shader
	glShaderSource(shader_object, 1, &shader_source_c_str, NULL);
	// compile the shader
	glCompileShader(shader_object);

	// error checking
	int success;
	char infoLog[512];
	glGetShaderiv(shader_object, GL_COMPILE_STATUS, &success);

	if ( !success )
	{
		glGetShaderInfoLog(shader_object, 512, NULL, infoLog);
		std::cout << filename << std::endl;
		std::cout << "ERROR: shader compilation failed\n" << infoLog << std::endl;
		std::cout << std::endl;
	}
	
	// adds the shader object to the list of shaders that are in the "system"
	shader_objects.push_back(shader_object);
	
	// closes the file because we are now done with it.
	shader_source_stream.close();
}

void Shader::createProgram()
{
	// create the shader program
	shader_program = glCreateProgram();

	// this loops through all the shader object identifiers and attaches them to
	//  the shader program
	for ( int i = 0; i < shader_objects.size(); i++)
	{
		glAttachShader(shader_program, shader_objects[i]);
	}

	glLinkProgram(shader_program);

	// error checking
	int success;
	char infoLog[512];
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

	if ( !success )
	{
		glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
		std::cout << "Error: program linking failed\n" << infoLog << std::endl;
	}
	// deletes the shaders because we are no longer using them.
	for ( int i = 0; i < shader_objects.size(); i++)
	{
		glDeleteShader(shader_objects[i]);
	}

	// destroys all the data in the vector because the handles are not attached
	//  to anything anymore.
	shader_objects.clear();

	// returns the newely created shader program
}

void Shader::useProgram()
{
	glUseProgram(shader_program);
}

unsigned int Shader::getID()
{
	return shader_program;
}

//void Shader::genUniformList()
//{
//	// gets the total number of active uniforms.
//	int active_uniforms;
//	glGetProgramiv(shader_program, GL_ACTIVE_UNIFORMS, &active_uniforms);
//	// maximum size that will need allocated to store the longest uniform
//	//  variables name.
//	int buffer_size;
//	glGetProgramiv(shader_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &buffer_size);
//
//	// the size of the uniform variable in the shader program
//	int uniform_size;
//	// the type of the uniform in the shader program.
//	GLenum uniform_type;
//	// the name of the uniform in the shader program.
//	std::string uniform_name;
//	uniform_name.reserve(buffer_size);
//	
//	for ( unsigned int i = 0; i < active_uniforms; i++ )
//	{
//		glGetActiveUniform(shader_program,
//						   i,
//						   buffer_size,
//						   NULL,
//						   &uniform_size,
//						   &uniform_type,
//						   &uniform_name[0]
//						   );
//
//		// add the uniform info to the list.
//		uniforms.emplace(uniform_name, std::make_pair(uniform_size, uniform_type));
//	}
//}

int Shader::getUniLoc(std::string name)
{
	return glGetUniformLocation(shader_program, name.c_str());
}

const std::string Shader::getMatrixUniformName()
{
	return matrix_uniform_name;
}


Shader::~Shader()
{
	glDeleteShader(shader_program);
}
