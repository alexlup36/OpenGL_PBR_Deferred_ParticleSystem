#ifndef POINTGENERATOR_H
#define POINTGENERATOR_H

#include "Generator.h"
#include "glm/vec3.hpp"

// Test particle
struct Particle
{
	glm::vec3 p;
	glm::vec3 v0;
};

class PointGenerator : public Generator
{
public:
	
	PointGenerator();
	virtual ~PointGenerator();

	virtual Type type() { return Type::Square; }

	virtual void update(float t) override;
	virtual void draw() override;

private:

	glm::vec3 m_pos[MAX_COUNT];
	glm::vec3 m_v0[MAX_COUNT];
	float m_lifespan[MAX_COUNT];

	// Original values for the point generator
	glm::vec3 m_pos0 = glm::vec3(0.0f);
	float m_duration = 2000;

	// Processing batch size
	size_t m_batchSize;
	size_t m_batchCount;
};

#endif // POINTGENERATOR_H