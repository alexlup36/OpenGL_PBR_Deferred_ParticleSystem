#ifndef CIRCLEGENERATOR_H
#define CIRCLEGENERATOR_H

#include "Generator.h"

class CircleGenerator : public Generator
{
public:
	CircleGenerator(size_t particleCount, const std::string &spritePath);
	~CircleGenerator();

	virtual Type type() { return Type::Square; }

	virtual void reset(size_t particleCount, const glm::vec3 &pos = glm::vec3(0.0f)) override;
	virtual void update(float t) override;
	virtual void draw() override;
};

#endif // CIRCLEGENERATOR_H