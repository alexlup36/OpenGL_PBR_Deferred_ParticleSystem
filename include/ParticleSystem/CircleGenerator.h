#ifndef CIRCLEGENERATOR_H
#define CIRCLEGENERATOR_H

#include "Generator.h"

class CircleGenerator : public Generator
{
public:
	CircleGenerator();
	~CircleGenerator();

	virtual Type type() { return Type::Square; }

	virtual void update(float t) override;
	virtual void draw() override;
};

#endif // CIRCLEGENERATOR_H