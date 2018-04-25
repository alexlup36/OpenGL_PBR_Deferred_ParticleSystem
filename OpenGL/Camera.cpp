#include "Camera.h"

#include "Input.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ----------------------------------------------------------------------------

Camera::Camera(const Window& window, const char* cameraName, float fov, float near, float far)
	: m_positionVec(0.0f, 0.0f, 0.0f), 
	m_frontVec(0.0f, 0.0f, -1.0f),
	m_upVec(0.0f, 1.0f, 0.0f)
{
	m_windowWidth = window.windowWidth;
	m_windowHeight = window.windowHeight;

	m_windowCenterX = window.windowWidth * 0.5f;
	m_windowCenterY = window.windowHeight * 0.5f;

	glfwSetCursorPos(window.window, m_windowCenterX, m_windowCenterY);

	m_fYaw = -90.0f;
	m_fPitch = 0.0f;

	m_cameraName = cameraName;

	setPerspectiveProjection(m_windowWidth, m_windowHeight, fov, near, far);
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

void Camera::setPerspectiveProjection(int windowWidth, int windowHeight, float fov, float near, float far)
{
	// Update window size
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	m_windowCenterX = windowWidth * 0.5f;
	m_windowCenterY = windowHeight * 0.5f;
	// Update projection matrix
	m_projMat = glm::perspective(glm::radians(fov), static_cast<float>(m_windowWidth) / m_windowHeight, 0.01f, 10000.0f);
}

// ----------------------------------------------------------------------------

void Camera::setOrthographicsProjection(float left, float right, float top, float bottom, float near, float far)
{

}

// ----------------------------------------------------------------------------

void Camera::processInput(GLFWwindow* window, double deltaTime)
{
	float cameraSpeed = static_cast<float>(Input::camereaSpeed() * deltaTime);
	float movementSpeed = static_cast<float>(Input::movementSpeed() * deltaTime);

	// Get cursor position
	double xCursorPos, yCursorPos;
	glfwGetCursorPos(window, &xCursorPos, &yCursorPos);
	// Calc distance from center of the screen scaled by mouse speed
	double xOffset = (xCursorPos - m_windowCenterX) * cameraSpeed;
	double yOffset = (m_windowCenterY - yCursorPos) * cameraSpeed;
	// Update total yaw/pitch
	m_fYaw += xOffset;
	m_fPitch += yOffset;
	// Limit the angles
	if (m_fPitch > 89.0f) m_fPitch = 89.0f;
	if (m_fPitch < -89.0f) m_fPitch = -89.0f;

	// Calculate the new direction, right and up vectors
	m_frontVec = glm::normalize(glm::vec3(
		cos(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch)),
		sin(glm::radians(m_fPitch)),
		sin(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch))));

	// Camera right vector
	glm::vec3 right = glm::normalize(glm::cross(m_frontVec, glm::vec3(0.0f, 1.0f, 0.0f)));
	// Camera up vector
	m_upVec = glm::normalize(glm::cross(right, m_frontVec));

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		m_positionVec += movementSpeed * m_frontVec;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		m_positionVec -= movementSpeed * m_frontVec;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		m_positionVec -= movementSpeed * glm::normalize(glm::cross(m_frontVec, m_upVec));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		m_positionVec += movementSpeed *glm::normalize(glm::cross(m_frontVec, m_upVec));

	glfwSetCursorPos(window, m_windowCenterX, m_windowCenterY);
}

// ----------------------------------------------------------------------------