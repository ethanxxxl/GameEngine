#ifndef BODY_H
#define BODY_H

#include <Shader.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Uniform.h>

class Body
{
private:
	/// These are the vertices that make up the body.
	std::vector<float> *vertices;
	/// The indices that will be used if the user wants to use glDrawElements().
	std::vector<unsigned int> *indices;
	/// handles for all of the buffers that will be created.
	unsigned int VAO, VBO, EBO;

	/// The shader program that will be used when drawing the body.
	Shader shader_program;
	/// The name of the uniform that is multiplied with vertex in shader program.
	std::string matrix_uniform_name;

	/// internal boolean used to determine whether to use glDrawElements().
	bool use_elements;

	/// the number of vertices of the model
	int num_model_verts;

	/// the transform of the body in world space.
	glm::mat4 model;
	/// this is the matrix that we want to to give to the shader program.
	glm::mat4 clip_mat;

	Uniform<glm::mat4> clip_matrix;

public:
	/// default constructor, just declares the body, and not much else.
	Body();
	/// constructor for glDrawArrays()
	Body(std::vector<float> *verts, GLenum usage);
	/// constructor for glDrawElements()
	Body(std::vector<float> *verts, std::vector<unsigned int> *inds, GLenum);

	/// gets the shader program to use.
	void setShaderProgram(Shader newProgram);

	/// loads data for drawing with glDrawArrays()
	void load(std::vector<float> *verts, GLenum usage);
	/// loads data for drawing with glDrawElements()
	void load(std::vector<float> *verts,
			  std::vector<unsigned int> *inds,
			  GLenum usage
			  );

	/// adds a new vertex attribute to the body VAO.
	void setVertexAttribute(GLuint index,
							GLint size,
							GLenum type,
							GLenum normalize,
							GLsizei stride,
							const void *pointer);

	/// loads the texture into memory.
	void grabTexture(std::string path);

	/// draws the program.
	void draw(glm::mat4 proj_view);
	/// scales the model matrix by magnitude.
	void scale(glm::vec3 magnitude);
	/// rotates the model matrix by rotation.
	void rotate(float angle, glm::vec3 axis);
	/// translates the model matrix by translation.
	void translate(glm::vec3 translation);
};

#endif
