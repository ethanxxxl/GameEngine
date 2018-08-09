#include <Body.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <string>

Body::Body()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	useElements = false;
}

void Body::setVertices(std::vector<float> verts)
{
	vertices = verts;
}

void Body::setIndices(std::vector<unsigned int> inds)
{
	indices = inds;
	useElements = true;
}

/*!
 * Adds a vertex attriute to the list. this does not check for adding
 *  to many vertex attributes.
 */
void Body::setVertexAttribute(GLuint index,
							  GLint size,
							  GLenum type,
							  GLenum normalize,
							  GLsizei stride,
							  const void *pointer)
{
	// adds a vertex attribute to the vertex array and enables it
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, type, normalize, stride, pointer);
	glBindVertexArray(0);
}

/*!
 * Sets the shader program for the body
 */
void Body::setShaderProgram(Shader newProgram)
{
	shaderProgram = newProgram;
}

/*!
 * Creates and Populates the Vertex Buffer Object and the Element Buffer
 *  Object. this funcion also binds the body Vertex Array Object so it can be
 *  used when drawing the body in the draw method.
 */
void Body::load()
{
	// Bind the body vertex array
	glBindVertexArray(VAO);

	// puts the vertex data into the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices.data()) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// creates a element buffer object if there is index data to put in the
	//  buffer
	if ( useElements )
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices.data()), indices.data(), GL_STATIC_DRAW);
	}

	// unbind the vertex array
	glBindVertexArray(0);
}

void Body::draw(int num_verts, std::string mat_uniform_name)
{
	shaderProgram.useProgram();
	glBindVertexArray(VAO);

	// if the user wants to move the camera/object arount, then they need to
	//  provide the uniform name so that it can be sent over.
	if ( !mat_uniform_name.empty() )
	{
		glUniformMatrix4fv(shaderProgram.getUniformLocation(mat_uniform_name),
						   1,
						   GL_FALSE,
						   glm::value_ptr(clip_mat)
						   );
	}

	if ( useElements )
	{
		glDrawElements(GL_TRIANGLES, num_verts, GL_FLOAT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, num_verts);
	}

	glBindVertexArray(0);
}

void Body::scale(glm::vec3 magnitude)
{
	glm::scale(model, magnitude);
}

void Body::rotate(float angle, glm::vec3 axis)
{
	glm::rotate(model, angle, axis);
}

void Body::translate(glm::vec3 translation)
{
	glm::translate(model, translation);
}

glm::mat4 Body::getClipMatrix(glm::mat4 proj, glm::mat4 view)
{
	clip_mat =  proj * view * model;
	for ( int x = 0; x < 3; x++)
	{
		for ( int y = 0; y < 3; y++ )
		{
			std::cout << view[y][x] << " ";
		}
		std::cout << std::endl;
	}

	return clip_mat;
}
