#include <Body.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <string>

#include <Uniform.h>

Body::Body() : shader_program("clip_matrix")
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	use_elements = false;

	shader_program.grabShader(GL_VERTEX_SHADER, "../shaders/default/vertextshader.glsl");
	shader_program.grabShader(GL_FRAGMENT_SHADER, "../shaders/default/vertextshader.glsl");
	shader_program.createProgram();
}


Body::Body(std::vector<float> *verts, GLenum usage = GL_STATIC_DRAW) : shader_program("clip_matrix")
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// set up the draw function to use glDrawElements()
	use_elements = false;

	// moves copys the pointer to the obect so the object knows where its data
	//  is (may not be needed)
	vertices = verts;
	
	// binds the object VAO
	glBindVertexArray(VAO);

	// put the new vertex data in the buffer.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,
				 sizeof(vertices->data()) * vertices->size(),
				 vertices->data(),
				 usage
				 );

	// unbinds the object VAO to make sure it doesn't get screwed up by
	//  something else outside of the object class.
	glBindVertexArray(0);

	shader_program.grabShader(GL_VERTEX_SHADER, "../shaders/default/vertextshader.glsl");
	shader_program.grabShader(GL_FRAGMENT_SHADER, "../shaders/default/vertextshader.glsl");
	shader_program.createProgram();
}


Body::Body(std::vector<float> *verts,
		   std::vector<unsigned int> *inds,
		   GLenum usage = GL_STATIC_DRAW
		   )
		   : shader_program("clip_matrix")
{
	// generate the buffers and array object.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// set up the draw function to use glDrawElements()
	use_elements = true;

	// moves copys the pointer to the obect so the object knows where its data
	//  is (may not be needed)
	vertices = verts;

	// binds the object VAO
	glBindVertexArray(VAO);

	// put the new vertex data in the buffer.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,
				 sizeof(vertices->data()) * vertices->size(),
				 vertices->data(),
				 usage
				 );

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				 sizeof(indices->data()) * indices->size(),
				 indices->data(),
				 usage
				 );

	// unbinds the object VAO to make sure it doesn't get screwed up by
	//  something else outside of the object class.
	glBindVertexArray(0);

	shader_program.grabShader(GL_VERTEX_SHADER, "../shaders/default/vertextshader.glsl");
	shader_program.grabShader(GL_FRAGMENT_SHADER, "../shaders/default/vertextshader.glsl");
	shader_program.createProgram();
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

	num_model_verts = vertices->size() / stride;
}

/*!
 * Sets the shader program for the body
 */
void Body::setShaderProgram(Shader newProgram)
{
	shader_program = newProgram;
}

/*!
 * Creates and Populates the Vertex Buffer Object and the Element Buffer
 *  Object. this funcion also binds the body Vertex Array Object so it can be
 *  used when drawing the body in the draw method.
 */

void Body::load(std::vector<float> *verts, GLenum usage = GL_STATIC_DRAW)
{
	// moves copys the pointer to the obect so the object knows where its data
	//  is (may not be needed)
	vertices = verts;

	// sets the draw mode for the draw function
	use_elements = false;
	
	// binds the object VAO
	glBindVertexArray(VAO);

	// put the new vertex data in the buffer.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,
				 sizeof(vertices->data()) * vertices->size(),
				 vertices->data(),
				 usage
				 );

	// unbinds the object VAO to make sure it doesn't get screwed up by
	//  something else outside of the object class.
	glBindVertexArray(0);
}

void Body::load(std::vector<float> *verts,
				std::vector<unsigned int> *inds,
				GLenum usage = GL_STATIC_DRAW
				)
{
	// moves copys the pointer to the obect so the object knows where its data
	//  is (may not be needed)
	vertices = verts;

	// sets the draw mode for the draw function
	use_elements = false;
	
	// binds the object VAO
	glBindVertexArray(VAO);

	// put the new vertex data in the buffer.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,
				 sizeof(vertices->data()) * vertices->size(),
				 vertices->data(),
				 usage
				 );

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				 sizeof(indices->data()) * indices->size(),
				 indices->data(),
				 usage
				 );

	// unbinds the object VAO to make sure it doesn't get screwed up by
	//  something else outside of the object class.
	glBindVertexArray(0);

}

void Body::draw(glm::mat4 proj_view)
{
	shader_program.useProgram();
	glBindVertexArray(VAO);

	clip_matrix.aquire(shader_program.getMatrixUniformName(), shader_program.getID());
	clip_matrix.setData(model * proj_view);

	if ( use_elements )
	{
		glDrawElements(GL_TRIANGLES, num_model_verts, GL_FLOAT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, num_model_verts);
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
