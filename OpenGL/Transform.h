#ifndef TRANSFORM_H
#define TRANSFORM_H

#define GLM_FORCE_RADIANS 1

#include "Common.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Transform
{
public:
	Transform()
		: m_scaleMat(IDENTITY), m_rotationMat(IDENTITY), m_translationMat(IDENTITY) {}

	inline glm::mat4 modelMat() const { return m_translationMat * m_rotationMat * m_scaleMat; }

	inline void setPos(const glm::vec3& pos) { m_translationMat = glm::translate(pos); }
	inline void setPos(GLfloat x, GLfloat y, GLfloat z) { m_translationMat = glm::translate(glm::vec3(x, y, z)); }

	inline void setRotation(const glm::vec3& rot) { m_rotationMat = glm::rotate(rot.z, FORWARD) * glm::rotate(rot.y, UP) * glm::rotate(rot.x, RIGHT); }
	inline void setRotation(GLfloat x, GLfloat y, GLfloat z) { m_rotationMat = glm::rotate(z, FORWARD) * glm::rotate(y, UP) * glm::rotate(x, RIGHT); }
	inline void setRotation(const glm::quat& rotation) { m_rotationMat = glm::toMat4(rotation); }

	inline void setScale(const glm::vec3& scale) { m_scaleMat = glm::scale(scale); }
	inline void setScale(GLfloat x, GLfloat y, GLfloat z) { m_scaleMat = glm::scale(glm::vec3(x, y, z)); }
protected:

private:
	glm::mat4 m_scaleMat;
	glm::mat4 m_rotationMat;
	glm::mat4 m_translationMat;

	const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec3 FORWARD = glm::vec3(0.0f, 0.0f, 1.0f);

	const glm::mat4 IDENTITY = glm::mat4(1.0f);
};


#endif