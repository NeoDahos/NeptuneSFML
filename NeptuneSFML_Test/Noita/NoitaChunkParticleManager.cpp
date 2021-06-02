#include "NoitaChunkParticleManager.h"

#include <algorithm>
#include <cmath>
#include <iostream>

const sf::Color NoitaChunkParticleManager::ParticleColors[4] = { sf::Color::Transparent, sf::Color(255, 200, 0), sf::Color(0, 100, 255), sf::Color(110, 50, 25) };

NoitaChunkParticleManager::~NoitaChunkParticleManager()
{
	for (NoitaParticleChunk* chunk : m_chunks)
		delete chunk;
}

void NoitaChunkParticleManager::Update(float deltaTime)
{
	m_timeUntilNextStep -= deltaTime;
	if (m_timeUntilNextStep > 0.f)
		return;

	m_timeUntilNextStep = TimeBetweenNextStep;
	m_simulationStepCount++;

	deltaTime = UseFixedDeltaTime ? FixedDeltaTime : deltaTime;

	// Update all chuncks
	m_totalDynamicParticlesCount = 0;
	for (size_t i = 0; i < m_chunks.size(); i++)
		m_totalDynamicParticlesCount += m_chunks[i]->UpdateSimulation(m_simulationStepCount, deltaTime);

	for (NoitaParticleChunk* chunk : m_chunks)
		chunk->ApplyChanges();
}

void NoitaChunkParticleManager::Draw(sf::RenderTarget& target)
{
	for (NoitaParticleChunk* chunk : m_chunks)
		chunk->Draw(target);
}

void NoitaChunkParticleManager::SpawnParticle(const sf::Vector2i& world, const NoitaParticle& particleToSpawn)
{
	SpawnParticle(world.x, world.y, particleToSpawn);
}

void NoitaChunkParticleManager::SpawnParticle(int worldX, int worldY, const NoitaParticle& particleToSpawn)
{
	NoitaParticleChunk* chunk = GetChunk(worldX, worldY);
	sf::Vector2i local = chunk->ComputeLocalPosition(worldX, worldY);
	chunk->SpawnParticle(local, particleToSpawn);
}

NoitaParticleChunk* NoitaChunkParticleManager::GetChunk(const sf::Vector2i& world)
{
	return GetChunk(world.x, world.y);
}

NoitaParticleChunk* NoitaChunkParticleManager::GetChunk(int worldX, int worldY)
{
	int chunkX = (int)std::floor((float)worldX / NoitaParticleChunk::ChunkSize);
	int chunkY = (int)std::floor((float)worldY / NoitaParticleChunk::ChunkSize);
	auto chunk = std::find_if(m_chunks.begin(), m_chunks.end(), [chunkX, chunkY](NoitaParticleChunk* current) {
		sf::Vector2i worldChunkPosition = current->GetChunkWorldPosition();
		return worldChunkPosition.x == chunkX && worldChunkPosition.y == chunkY;
		});

	if (chunk == m_chunks.end())
	{
		m_chunks.emplace_back(new NoitaParticleChunk(*this, chunkX, chunkY));
		chunk = m_chunks.begin() + (m_chunks.size() - 1);
		m_totalParticlesCount = m_chunks.size() * NoitaParticleChunk::ChunkSize * NoitaParticleChunk::ChunkSize;
	}

	return *chunk;
}

const sf::Color& NoitaChunkParticleManager::GetColor(NoitaParticle::Type type) const
{
	return ParticleColors[(sf::Uint8)type];
}

int NoitaChunkParticleManager::GetChunksCount() const
{
	return m_chunks.size();
}

int NoitaChunkParticleManager::GetTotalParticleCount() const
{
	return m_totalParticlesCount;
}

int NoitaChunkParticleManager::GetTotalDynamicParticleCount() const
{
	return m_totalDynamicParticlesCount;
}
