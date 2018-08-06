#ifndef BODY_H
#define BODY_H

#include <vector>
#include <Shader.h>

class Body
{
private:
	std::vector<float> vertices;
	Shader bodyShaderProgram;
	
public:
	// grabs an std::vector of vertices
	void setVertices(std::vector<float> verts);

	void loadModel();

	void setShaderProgram(Shader shaderProgram);

	void draw();
};

#endif
