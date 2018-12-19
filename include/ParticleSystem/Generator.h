#ifndef GENERATOR_H
#define GENERATOR_H

#include <random>

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

	virtual void update(float t) = 0;
	virtual void draw() = 0;

protected:
	static const size_t MAX_COUNT = 10000000;
	float m_acceleration = -9.81f;
	float m_acceleration_half = m_acceleration * 0.5f;

private:

	// Random device to use for the random seed
	std::random_device m_randomDevice;
	// Random number generator
	std::mt19937 m_mtGenerator;
};

#endif // GENERATOR_H