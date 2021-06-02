#pragma once

#include <SFML\Graphics\Rect.hpp>
#include <SFML\Graphics\Color.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\Sprite.hpp>

#include <vector>
#include <functional>

#include "NoitaParticle.h"

class NoitaChunkParticleManager;

class NoitaParticleChunk
{
public:
	static constexpr const sf::Uint16 ChunkSize = 256;
	typedef std::function<void(NoitaParticle& original, NoitaParticle& swapped)> SwapAction;

	NoitaParticleChunk(NoitaChunkParticleManager& managerRef, int worldX, int worldY);

	int UpdateSimulation(sf::Uint16 simulationStepCount, float deltaTime);
	void ApplyChanges();
	void Draw(sf::RenderTarget& target);

	void SpawnParticle(const sf::Vector2i& position, const NoitaParticle& particleToSpawn);
	void SpawnParticle(int x, int y, const NoitaParticle& particleToSpawn);
	bool TrySwapParticles(int index, int destX, int destY, sf::Uint8 allowedTypesToSwap, NoitaParticleChunk::SwapAction physicSwapAction = nullptr);


	// Utility
	sf::Vector2i ComputeLocalPosition(const sf::Vector2i& world) const;
	sf::Vector2i ComputeWorldPosition(const sf::Vector2i& local) const;
	sf::Vector2i ComputeLocalPosition(int worldX, int worldY) const;
	sf::Vector2i ComputeWorldPosition(int localX, int localY) const;

	const sf::Vector2i& GetChunkWorldPosition() const;


private:
	static void PhysicSwapAction_SandParticle(NoitaParticle& original, NoitaParticle& swapped);

	bool AddChange(int destX, int destY, int sourceIndex, sf::Uint8 allowedTypesToSwap);
	void UpdateSandParticle(int x, int y, int index, float deltaTime, NoitaParticle& currentParticle);
	void UpdateWaterParticle(int x, int y, int index, float deltaTime, NoitaParticle& currentParticle);
	bool UpdateParticlePhysic(NoitaParticle& currentParticle, int x, int y, int index, float deltaTime, sf::Uint8 allowedTypesToSwap, NoitaParticleChunk::SwapAction physicSwapAction = nullptr);


	// Utility
	constexpr int GetIndex(const sf::Vector2i& position) const;
	constexpr int GetIndex(int x, int y) const;
	constexpr bool IsInBounds(const sf::Vector2i& position) const;
	constexpr bool IsInBounds(int x, int y) const;
	constexpr bool IsTypeInMask(NoitaParticle::Type type, sf::Uint8 mask) const;
	void SwapParticles(int sourceIndex, NoitaParticleChunk* destinationChunk, int destinationIndex);
	void SetPixelsColor(NoitaParticle::Type type, int particleIndex);


	static int CurrentRandValue;

	NoitaParticle m_particles[ChunkSize * ChunkSize];
	sf::Uint8 m_pixelsColor[ChunkSize * ChunkSize * 4];
	std::vector<struct ParticleMovement> m_changes;
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	sf::RectangleShape m_outline;
	sf::Vector2i m_chunkWorldPosition;
	sf::Vector2i m_worldPosition;
	NoitaChunkParticleManager& m_particleManager;
	sf::Uint16 m_currentSimulationStepCount{};
};

struct ParticleMovement
{
public:
	bool operator<(const ParticleMovement& other);

	NoitaParticleChunk* m_sourceChunk{};
	int m_sourceIndex{};
	int m_destinationIndex{};
};