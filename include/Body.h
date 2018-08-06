#ifndef BODY_H
#define BODY_H

#include <vector>
#include <Shader.h>

class Body
{
private:
	std::vector<float> vertices;
	std::vector<float> indices;
	Shader shaderProgram;

	unsigned int VAO;

public:
	// grabs an std::vector of vertices
	void setVertices(std::vector<float> verts);

	// grabs an std::vector of indices
	void setIndices(std::vector<float> ind);

	void setVertexAttribute(GLuint index,
							GLint size,
							GLenum type,
							GLsizei stride,
							constGLvoid *pointer)
	

	// gets the shader program to use
	void setShaderProgram(Shader newProgram);

	void load();

	void draw();

	Body();
};

#endif
