#pragma once

#include <SFML\Graphics\Rect.hpp>
#include <SFML\Graphics\Color.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\Sprite.hpp>

#include <vector>
#include <functional>

#include "Elements\NoitaElement.h"

class NoitaChunkParticleManager;

class NoitaParticleChunk
{
public:
	static constexpr const sf::Uint16 ChunkSize = 256;
	typedef std::function<void(NoitaElement& original, NoitaElement& swapped)> SwapAction;

	NoitaParticleChunk(NoitaChunkParticleManager& managerRef, int worldX, int worldY, bool updateFlag);
	~NoitaParticleChunk();

	int UpdateSimulation(bool updateFlag, float deltaTime);
	void ApplyChanges();
	void Draw(sf::RenderTarget& target);

	void SpawnParticle(const sf::Vector2i& position, const NoitaElement& particleToSpawn);
	void SpawnParticle(int x, int y, const NoitaElement& particleToSpawn);
	NoitaElementCommon::ChunkElement GetChunkElement(int destX, int destY);


	// Utility
	void SwapParticles(int sourceIndex, NoitaParticleChunk* destinationChunk, int destinationIndex);
	sf::Vector2i ComputeLocalPosition(const sf::Vector2i& world) const;
	sf::Vector2i ComputeWorldPosition(const sf::Vector2i& local) const;
	sf::Vector2i ComputeLocalPosition(int worldX, int worldY) const;
	sf::Vector2i ComputeWorldPosition(int localX, int localY) const;

	const sf::Vector2i& GetChunkWorldPosition() const;


private:
	static void PhysicSwapAction_SandParticle(NoitaElement& original, NoitaElement& swapped);

	void UpdateSandParticle(int x, int y, int index, float deltaTime, NoitaElement& currentParticle);
	void UpdateWaterParticle(int x, int y, int index, float deltaTime, NoitaElement& currentParticle);
	bool UpdateParticlePhysic(NoitaElement& currentParticle, int x, int y, int index, float deltaTime, sf::Uint8 allowedTypesToSwap, NoitaParticleChunk::SwapAction physicSwapAction = nullptr);


	// Utility
	constexpr int GetIndex(const sf::Vector2i& position) const;
	constexpr int GetIndex(int x, int y) const;
	constexpr bool IsInBounds(const sf::Vector2i& position) const;
	constexpr bool IsInBounds(int x, int y) const;
	constexpr bool IsTypeInMask(NoitaElementCommon::Type type, sf::Uint8 mask) const;
	void SetPixelsColor(NoitaElementCommon::Type type, int particleIndex);


	static int CurrentRandValue;

	NoitaElement* m_elements[ChunkSize * ChunkSize];
	sf::Uint8 m_pixelsColor[ChunkSize * ChunkSize * 4];
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	sf::RectangleShape m_outline;
	sf::Vector2i m_chunkWorldPosition;
	sf::Vector2i m_worldPosition;
	NoitaChunkParticleManager& m_particleManager;
};