#include <Body.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

void Body::setVertices(std::vector<float> verts)
{
	vertices = verts;
}

void Body::setIndices(std::vector<float> ind)
{
	indices = ind;
}
void Body::setShaderProgram(Shader newProgram)
{
	shaderProgram = newProgram;
}

/**
 * @brief This method is responsible for making sure that all of the body vertex
 *  and other data is put away in buffers and bound to the VAO so that the object
 *  can easily be drawn with the draw() function.
 */
void Body::load()
{
	// create handles for the buffer object and the element buffer object
	unsigned int VBO, EBO;

	// create the body vertex array and then bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// TODO: finish putting everything in the buffers properly.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices.data()), vertices.data(), GL_STATIC_DRAW);

	// unbind the vertex array
	glBindVertexArray(0);
}
