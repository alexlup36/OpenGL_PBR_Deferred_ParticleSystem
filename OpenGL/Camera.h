#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Camera
{
public:
	Camera(int windowWidth, int windowHeight);
	~Camera();

	void updateView();
	void processInput(GLFWwindow* window, double deltaTime);

	inline const glm::mat4& viewMatrix() const { return m_viewMat; }

private:
	bool m_bFirstUse;

	glm::vec3 m_positionVec;
	glm::vec3 m_frontVec;
	glm::vec3 m_upVec;

	float m_fYaw, m_fPitch;
	float m_fLastX, m_fLastY;

	float m_fFOV;

	glm::mat4 m_viewMat;
};

#endif // CAMERA_H