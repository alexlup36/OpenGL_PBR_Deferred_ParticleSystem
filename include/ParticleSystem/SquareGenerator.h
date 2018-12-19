#ifndef SQUAREGENERATOR_H
#define SQUAREGENERATOR_H

#include "Generator.h"

class SquareGenerator : public Generator
{
public:
	SquareGenerator();
	virtual ~SquareGenerator();

	virtual Type type() { return Type::Square; }

	virtual void update(float t) override;
	virtual void draw() override;
};

#endif // SQUAREGENERATOR_H