#include <Body.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

Body::Body()
{
	glGenVertexArrays(1, &VAO);
}

void Body::setVertices(std::vector<float> verts)
{
	vertices = verts;
}

void Body::setIndices(std::vector<float> inds)
{
	indices = inds;
}

/**
 * @brief Adds a vertex attriute to the list. this does not check for adding
 *  to many vertex attributes.
 */
void Body::setVertexAttribute(GLuint index,
							  GLint size,
							  GLenum type,
							  GLenum normalize,
							  GLsizei stride,
							  const void *pointer)
{
	// adds a vertex attribute to the vertex array
	glBindVertexArray(VAO);
	glVertexAttribPointer(index, size, type, normalize, stride, pointer);
	glBindVertexArray(0);
}

/**
 * @brief Sets the shader program for the body
 */
void Body::setShaderProgram(Shader newProgram)
{
	shaderProgram = newProgram;
}

/**
 * @brief Creates and Populates the Vertex Buffer Object and the Element Buffer
 *  Object. this funcion also binds the body Vertex Array Object so it can be
 *  used when drawing the body in the draw method.
 */
void Body::load()
{
	// create handles for the buffer object and the element buffer object
	unsigned int VBO, EBO;

	// Bind the body vertex array
	glBindVertexArray(VAO);

	// puts the vertex data into the vertex buffer
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices.data()), vertices.data(), GL_STATIC_DRAW);

	// creates a element buffer object if there is index data to put in the
	//  buffer
	if ( indices.size() > 0 )
	{
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices.data()), indices.data(), GL_STATIC_DRAW);
	}

	// unbind the vertex array
	glBindVertexArray(0);
}

void Body::draw(int num_verts, int num_inds)
{
	shaderProgram.useProgram();
	if ( indices.size() > 0 )
	{
		glDrawElements(GL_TRIANGLES, 0, num_inds);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, num_verts);
	}
}
