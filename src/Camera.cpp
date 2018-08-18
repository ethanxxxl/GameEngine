#include <Camera.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera *Camera::activeCamera;

Camera::Camera(glm::vec3 eye, glm::vec3 direction, float fov, float aspect, float near, float far )
{
	view = glm::lookAt(eye, direction, glm::vec3(0.0f, 0.0f, 1.0f));
	projection = glm::perspective(fov, aspect, near, far);
}

Camera::Camera(float fov, float aspect, float near, float far)
{
	view = glm::mat4(1.0f);
	projection = glm::perspective(fov, aspect, near, far);
}

void Camera::lookAt(glm::vec3 eye, glm::vec3 center)
{
	view = glm::lookAt(eye, center, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera::activate()
{
	activeCamera = this;
}

void Camera::drawAll()
{
	for ( int i = 0; i < activeCamera->bodies.size(); i++ )
	{
		// draws each body with the cameras projection and view matrix combo
		activeCamera->bodies.at(i)->draw(view * projection);
	}
}

void Camera::addBody(Body *new_body)
{
	bodies.push_back(new_body);
}
