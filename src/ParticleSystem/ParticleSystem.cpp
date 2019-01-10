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

Generator *ParticleSystem::addGenerator(Generator::Type type, size_t particleCount, const std::string &spritePath)
{
	switch (type)
	{
	case Generator::Type::Point:
	{
		m_generators.push_back(std::unique_ptr<Generator>(new PointGenerator(particleCount, spritePath)));
		return m_generators.back().get();
	}
	case Generator::Type::Circle:
	{
		m_generators.push_back(std::unique_ptr<Generator>(new CircleGenerator(particleCount, spritePath)));
		return m_generators.back().get();
	}
	case Generator::Type::Square:
	{
		m_generators.push_back(std::unique_ptr<Generator>(new SquareGenerator(particleCount, spritePath)));
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

void ParticleSystem::buildVertexBuffer()
{
	// Iterate through the generators
	for (auto &generator : m_generators)
	{
		// Iterate through the particles for each generator
		auto particleCount = generator->getParticleCount();
		for (size_t particleIndex = 0; particleIndex < particleCount; ++particleIndex)
		{

		}
	}
}