#ifndef GENERATOR_H
#define GENERATOR_H

#include <random>

#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include "Texture2D.h"

struct VertexParticle
{
	VertexParticle()
		: m_pos(0.0f), m_uv(0.0f), m_col(0.0f) {}

	glm::vec3 m_pos;	// Vertex position
	glm::vec2 m_uv;		// Vertex uv
	glm::vec4 m_col;	// Vertex color
};

class Generator
{
public:
	Generator();
	virtual ~Generator();

	enum class Type
	{
		Point,
		Square,
		Circle,
	};

	virtual Type type() = 0;

	template <typename T>
	T getRandomReal(T min, T max)
	{
		std::uniform_real_distribution<T> dist(min, max);
		return dist(m_mtGenerator);
	}

	template <typename T>
	T getRandomInt(T min, T max)
	{
		std::uniform_int_distribution<T> dist(min, max);
		return dist(m_mtGenerator);
	}

	inline const size_t getParticleCount() const { return m_particleCount; }

	virtual void update(float t) = 0;
	virtual void draw() = 0;
	virtual void reset(size_t particleCount, const glm::vec3 &pos = glm::vec3(0.0f)) = 0;

protected:
	static const size_t MAX_COUNT = 100000;
	size_t m_particleCount;
	float m_acceleration = -9.81f;
	float m_acceleration_half = m_acceleration * 0.5f;

	// Particle sprite texture
	std::unique_ptr<Texture2D> m_texture;

private:

	// Random device to use for the random seed
	std::random_device m_randomDevice;
	// Random number generator
	std::mt19937 m_mtGenerator;
};

#endif // GENERATOR_H