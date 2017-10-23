#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ----------------------------------------------------------------------------

Camera::Camera(int windowWidth, int windowHeight)
	: m_positionVec(0.0f, 0.0f, 3.0f), 
	m_frontVec(0.0f, 0.0f, -1.0f),
	m_upVec(0.0f, 1.0f, 0.0f)
{
	m_fYaw = -90.0f;
	m_fPitch = 0.0f;
	m_fLastX = windowWidth * 0.5f;
	m_fLastY = windowHeight * 0.5f;
}

// ----------------------------------------------------------------------------

Camera::~Camera()
{
}

// ----------------------------------------------------------------------------

void Camera::updateView()
{
	m_viewMat = glm::lookAt(m_positionVec, m_positionVec + m_frontVec, m_upVec);
}

// ----------------------------------------------------------------------------

void Camera::processInput(GLFWwindow* window, float deltaTime)
{
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		m_positionVec += cameraSpeed * m_frontVec;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		m_positionVec -= cameraSpeed * m_frontVec;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		m_positionVec -= cameraSpeed * glm::normalize(glm::cross(m_frontVec, m_upVec));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		m_positionVec += cameraSpeed *glm::normalize(glm::cross(m_frontVec, m_upVec));
}

// ----------------------------------------------------------------------------