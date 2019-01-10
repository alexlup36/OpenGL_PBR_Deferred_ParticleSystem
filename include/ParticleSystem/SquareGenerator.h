#ifndef SQUAREGENERATOR_H
#define SQUAREGENERATOR_H

#include "Generator.h"

class SquareGenerator : public Generator
{
public:
	SquareGenerator(size_t particleCount, const std::string &spritePath);
	virtual ~SquareGenerator();

	virtual Type type() { return Type::Square; }

	virtual void reset(size_t particleCount, const glm::vec3 &pos = glm::vec3(0.0f)) override;
	virtual void update(float t) override;
	virtual void draw() override;
};

#endif // SQUAREGENERATOR_H