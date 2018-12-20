#ifndef POINTGENERATOR_H
#define POINTGENERATOR_H

#include <string>

#include "Generator.h"
#include "glm/vec3.hpp"

class PointGenerator : public Generator
{
public:
	
	PointGenerator(size_t particleCount, const std::string &spritePath);
	virtual ~PointGenerator();

	virtual Type type() { return Type::Square; }

	virtual void reset(size_t particleCount, const glm::vec3 &pos = glm::vec3(0.0f)) override;
	virtual void update(float t) override;
	virtual void draw() override;

private:

	glm::vec3 m_pos[MAX_COUNT];
	glm::vec3 m_v0[MAX_COUNT];
	glm::vec3 m_col[MAX_COUNT];
	float m_lifespan[MAX_COUNT];
	float m_rot[MAX_COUNT];

	// Original values for the point generator
	glm::vec3 m_pos0 = glm::vec3(0.0f);
	float m_duration = 2000;

	// Processing batch size
	size_t m_batchSize;
	size_t m_batchCount;
};

#endif // POINTGENERATOR_H