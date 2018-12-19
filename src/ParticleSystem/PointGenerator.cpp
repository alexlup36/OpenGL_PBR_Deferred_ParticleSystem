#include "..\..\include\ParticleSystem\PointGenerator.h"

#include <chrono>
#include <iostream>

PointGenerator::PointGenerator()
{
	for (int particleIndex = 0; particleIndex < MAX_COUNT; ++particleIndex)
	{
		float vx = (float)rand() / RAND_MAX;
		float vy = (float)rand() / RAND_MAX;
		float vz = (float)rand() / RAND_MAX;

		// Initialize particle data using random initial velocity
		m_pos[particleIndex] = m_pos0;
		m_v0[particleIndex] = glm::vec3(vx, vy, vz);
		m_lifespan[particleIndex] = m_duration;
	}

	// Calculate batch size
	m_batchSize = 4;
	m_batchCount = MAX_COUNT / m_batchSize;
}

PointGenerator::~PointGenerator()
{
}

void PointGenerator::update(float t)
{
	auto start = std::chrono::steady_clock::now();

	for (int batchIndex = 0; batchIndex < m_batchCount; ++batchIndex)
	{
		size_t startIndex = batchIndex * m_batchSize;

		for (int batchElem = 0; batchElem < m_batchSize; ++batchElem)
		{
			// Update particle
			auto index = startIndex + batchElem;
			m_pos[index] = m_pos[index] + t * m_v0[index] + m_acceleration_half * t * t;
			m_lifespan[index] -= t;

			// Reset particle
			if (m_lifespan[index] < 0)
			{
				m_pos[index] = m_pos0;
				m_lifespan[index] = m_duration;
			}
		}
	}

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
	std::cout << "Update set 4: " << duration.count() << "ms\n";
}

void PointGenerator::draw()
{

}