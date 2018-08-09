#ifndef BODY_H
#define BODY_H

#include <Shader.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Body
{
private:
	/// These are the vertices that make up the body.
	std::vector<float> vertices;
	/// The indices that will be used if the user wants to use glDrawElements().
	std::vector<unsigned int> indices;
	/// handles for all of the buffers that will be created
	unsigned int VAO, VBO, EBO;
	/// The shader program that will be used when drawing the body.
	Shader shaderProgram;

	/// internal boolean used to determine whether to use glDrawElements().
	bool useElements;

	/// the transform of the body in world space.
	glm::mat4 model;
	/// this is the matrix that we want to to give to the shader program.
	glm::mat4 clip_mat;

public:
	Body();

	/// grabs an std::vector of vertices.
	void setVertices(std::vector<float> verts);
	/// grabs an std::vector of indices.
	void setIndices(std::vector<unsigned int> inds);
	/// adds a new vertex attribute to the body VAO.
	void setVertexAttribute(GLuint index,
							GLint size,
							GLenum type,
							GLenum normalize,
							GLsizei stride,
							const void *pointer);
	/// gets the shader program to use.
	void setShaderProgram(Shader newProgram);
	
	/// loads the program into the buffers.
	void load();
	// loads the texture into memory.
	void grabTexture(std::string path);
	/// draws the program.
	void draw(int num_verts, std::string mat_uniform_name);

	/// scales the model matrix by magnitude.
	void scale(glm::vec3 magnitude);
	/// rotates the model matrix by rotation.
	void rotate(float angle, glm::vec3 axis);
	/// translates the model matrix by translation.
	void translate(glm::vec3 translation);

	glm::mat4 getClipMatrix(glm::mat4 proj, glm::mat4 view);
};

#endif
