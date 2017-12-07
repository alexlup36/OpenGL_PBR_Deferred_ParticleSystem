#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Camera
{
public:
	Camera(GLFWwindow* window, int windowWidth, int windowHeight);
	~Camera();

	void updateView();
	void updateProj(int windowWidth, int windowHeight, float fov, float near, float far);
	void processInput(GLFWwindow* window, double deltaTime);

	inline const glm::mat4& viewMatrix() const { return m_viewMat; }
	inline const glm::mat4& projMatrix() const { return m_projMat; }
	inline const glm::vec3& viewPos() const { return m_positionVec; }

private:
	glm::vec3 m_positionVec;
	glm::vec3 m_frontVec;
	glm::vec3 m_upVec;

	double m_fYaw, m_fPitch;

	glm::mat4 m_viewMat;
	glm::mat4 m_projMat;

	int m_windowWidth;
	int m_windowHeight;

	double m_windowCenterX;
	double m_windowCenterY;
};

#endif // CAMERA_H