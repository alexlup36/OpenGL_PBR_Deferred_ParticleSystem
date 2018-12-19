#include "..\..\include\ParticleSystem\ParticleSystem.h"

#include <iostream>

#include "ParticleSystem/PointGenerator.h"
#include "ParticleSystem/CircleGenerator.h"
#include "ParticleSystem/SquareGenerator.h"

ParticleSystem::ParticleSystem()
{

}

ParticleSystem::~ParticleSystem()
{

}

Generator *ParticleSystem::addGenerator(Generator::Type type)
{
	switch (type)
	{
	case Generator::Type::Point:
	{
		m_generators.push_back(std::unique_ptr<Generator>(new PointGenerator()));
		return m_generators.back().get();
	}
	case Generator::Type::Circle:
	{
		m_generators.push_back(std::unique_ptr<Generator>(new CircleGenerator()));
		return m_generators.back().get();
	}
	case Generator::Type::Square:
	{
		m_generators.push_back(std::unique_ptr<Generator>(new SquareGenerator()));
		return m_generators.back().get();
	}
	default:
		std::cout << "Invalid generator type.";
		return nullptr;
	}
}

void ParticleSystem::update(float t)
{
	for (auto &generator : m_generators)
		generator->update(t);
}

void ParticleSystem::draw()
{
	for (auto &generator : m_generators)
		generator->draw();
}