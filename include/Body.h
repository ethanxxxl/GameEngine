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
	void setIndices(std::vector<float> inds);

	void setVertexAttribute(GLuint index,
							GLint size,
							GLenum type,
							GLenum normalize,
							GLsizei stride,
							const void *pointer);
	

	// gets the shader program to use
	void setShaderProgram(Shader newProgram);

	void load();

	void draw(int num_verts, int num_inds);

	Body();
};

#endif
