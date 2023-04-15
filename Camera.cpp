	#include "Camera.h"
	#include <iostream>

	float Camera::fov = 45.0f;
	glm::vec3 velocity(0, 0, 0);

	Camera::Camera() {}

	Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
	{
		position = startPosition;
		worldUp = startUp;
		yaw = startYaw;
		pitch = startPitch;
		front = glm::vec3(0.0f, 0.0f, -1.0f);

		moveSpeed = startMoveSpeed;
		turnSpeed = startTurnSpeed;


		update();
	}

	void Camera::keyControl(bool* keys, GLfloat deltaTime)
	{

		if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
		{
			position += (front * velocity * deltaTime);
		}

		if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
		{
			//position -= front * velocity;
			position -= front * velocity * deltaTime;
		}

		if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
		{
			//position -= right * velocity;
			position -= right * velocity * deltaTime;
		}

		if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
		{
			//position += right * velocity;
			position -= right * velocity * deltaTime;
		}
		if (keys[GLFW_KEY_SPACE])
		{
			position += up * velocity * deltaTime;
		}
		//if (keys[GLFW_KEY_LEFT_CONTROL])
		//{
		//	position -= worldUp * (velocity / 2);
		//}
	}



	void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
	{
		xChange *= turnSpeed;
		yChange *= turnSpeed;

		yaw += xChange;

		pitch += yChange;

		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}

		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}

		update();
	}



	void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		fov -= yoffset * 4;
	}


	glm::mat4 Camera::calculateViewMatrix()
	{
		return glm::lookAt(position, position + front, up);
	}

	glm::vec3 Camera::getCameraPosition() {
		return position;
	}

	void Camera::update()
	{
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(front);

		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));

		position += worldUp * velocity;
	}


	Camera::~Camera()
	{
	}


