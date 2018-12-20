#include "..\..\include\ParticleSystem\PointGenerator.h"

#include <chrono>
#include <iostream>

PointGenerator::PointGenerator(size_t particleCount, const std::string &spritePath)
{
	reset(particleCount, glm::vec3(0.0f));
	
	std::string path = spritePath == "" ? "../Assets/Textures/particle/particle0.png" : spritePath;
	m_texture = std::make_unique<Texture2D>(path, TextureType::Diffuse1);
}

PointGenerator::~PointGenerator()
{
}

void PointGenerator::reset(size_t particleCount, const glm::vec3 &pos)
{
	assert(particleCount < MAX_COUNT && "Invalid particle count. Particle count has to be less than MAX_COUNT");
	m_particleCount = particleCount;

	// Initialize
	for (int particleIndex = 0; particleIndex < MAX_COUNT; ++particleIndex)
	{
		float vx = (float)rand() / RAND_MAX;
		float vy = (float)rand() / RAND_MAX;
		float vz = (float)rand() / RAND_MAX;

		int r = getRandomInt<int>(0, 256);
		int g = getRandomInt<int>(0, 256);
		int b = getRandomInt<int>(0, 256);

		// Initialize particle data using random initial velocity
		m_pos[particleIndex] = m_pos0;
		m_v0[particleIndex] = glm::vec3(vx, vy, vz);
		m_lifespan[particleIndex] = m_duration;
		m_rot[particleIndex] = (float)rand() / RAND_MAX;
		m_col[particleIndex] = glm::vec3(r, g, b);
	}

	// Calculate batch size
	m_batchSize = 4;
	m_batchCount = MAX_COUNT / m_batchSize;
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