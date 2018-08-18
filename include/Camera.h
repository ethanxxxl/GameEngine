#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include <Body.h>

class Camera
{
private:
	/// this is basically where the player "is" in the world.
	glm::mat4 view;
	/// this is how the player sees the world.
	glm::mat4 projection;

	/// only this camera will have its objects drawn.
	static Camera *activeCamera;
	
	/// the bodies that will be drawn when this camera is active.
	std::vector<Body*> bodies;

public:
	Camera(glm::vec3 eye, glm::vec3 direction, float fov, float aspect, float near, float far);
	Camera(float fov, float aspect, float near, float far);

	glm::vec3 getPosition();

	void lookAt(glm::vec3 eye, glm::vec3 center);

	void activate();

	void drawAll();

	void addBody(Body *new_body);
};

#endif
