#include <Shader.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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
