#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <memory>
#include <assert.h>

#include "Generator.h"

class ParticleSystem
{
public:
	
	static ParticleSystem &instance()
	{
		static ParticleSystem instance;
		return instance;
	}

	Generator *addGenerator(Generator::Type type);
	inline Generator *getGenerator(size_t index) const 
	{ 
		assert(index < m_generators.size() && "Invalid generator index.");
		return m_generators[index].get();
	}

	void update(float t);
	void draw();

private:

	ParticleSystem();
	~ParticleSystem();

	ParticleSystem(const ParticleSystem &other) = delete;
	void operator=(const ParticleSystem &other) = delete;

	std::vector<std::unique_ptr<Generator>> m_generators;
};

#endif // PARTICLESYSTEM_H