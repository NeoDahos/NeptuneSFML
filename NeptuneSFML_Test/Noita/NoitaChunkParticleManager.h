#pragma once

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\Color.hpp>

#include <vector>

#include "NoitaParticleChunk.h"

class NoitaChunkParticleManager
{
public:
	static constexpr const float Gravity = -10.f;

	~NoitaChunkParticleManager();

	void Update(float deltaTime);
	void Draw(sf::RenderTarget& target);

	void SpawnParticle(const sf::Vector2i& world, const NoitaParticle& particleToSpawn);
	void SpawnParticle(int worldX, int worldY, const NoitaParticle& particleToSpawn);

	NoitaParticleChunk* GetChunk(const sf::Vector2i& world);
	NoitaParticleChunk* GetChunk(int worldX, int worldY);

	const sf::Color& GetColor(NoitaParticle::Type type) const;
	int GetChunksCount() const;
	int GetTotalParticleCount() const;
	int GetTotalDynamicParticleCount() const;

private:
	static const sf::Color ParticleColors[4];

	static constexpr const float TimeBetweenNextStep = 0.f;
	static constexpr const float FixedDeltaTime = 0.02f;
	static constexpr const bool UseFixedDeltaTime = true;

	std::vector<NoitaParticleChunk*> m_chunks;
	float m_timeUntilNextStep{ TimeBetweenNextStep };
	int m_totalParticlesCount{};
	int m_totalDynamicParticlesCount{};
	sf::Uint16 m_simulationStepCount{};
};

