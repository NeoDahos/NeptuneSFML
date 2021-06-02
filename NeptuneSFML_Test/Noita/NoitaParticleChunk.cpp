#include "NoitaParticleChunk.h"

#include <NeptuneSFML\Tools.h>

#include "NoitaChunkParticleManager.h"

#include <algorithm>
#include <cmath>

bool ParticleMovement::operator<(const ParticleMovement& other)
{
	return m_destinationIndex < other.m_destinationIndex;
}

int NoitaParticleChunk::CurrentRandValue = 0;

NoitaParticleChunk::NoitaParticleChunk(NoitaChunkParticleManager& managerRef, int chunkWorldX, int chunkWorldY) :
	m_particleManager(managerRef)
{
	m_worldPosition.x = chunkWorldX * ChunkSize;
	m_worldPosition.y = chunkWorldY * ChunkSize;
	m_chunkWorldPosition.x = chunkWorldX;
	m_chunkWorldPosition.y = chunkWorldY;

	m_texture.create(ChunkSize, ChunkSize);
	m_texture.setRepeated(false);
	m_texture.setSmooth(false);

	int index = 0;
	int colorIndex = 0;
	for (int y = 0; y < ChunkSize; y++)
	{
		for (int x = 0; x < ChunkSize; x++, index++, colorIndex += 4)
		{
			m_particles[index] = NoitaParticle::EmptyParticle;
			SetPixelsColor(NoitaParticle::Type::Empty, index);
		}
	}

	m_texture.update(m_pixelsColor);

	m_sprite.setPosition((float)m_worldPosition.x, (float)(-ChunkSize - m_worldPosition.y));
	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(0, ChunkSize, ChunkSize, -ChunkSize));

	m_outline.setPosition((float)m_worldPosition.x, (float)(-ChunkSize - m_worldPosition.y));
	m_outline.setSize(sf::Vector2f(ChunkSize, ChunkSize));
	m_outline.setFillColor(sf::Color::Transparent);
	m_outline.setOutlineColor(sf::Color::White);
	m_outline.setOutlineThickness(1.f);
}

int NoitaParticleChunk::UpdateSimulation(sf::Uint16 simulationStepCount, float deltaTime)
{
	m_currentSimulationStepCount = simulationStepCount;
	CurrentRandValue = (int)(nep::GetRandomValue() * 100000.f);

	int particleIndex;
	int dynamicParticleCount = 0;
	for (int y = 0; y < ChunkSize; y++)
	{
		for (int x = 0; x < ChunkSize; x++)
		{
			particleIndex = GetIndex(x, y);
			NoitaParticle& currentParticle = m_particles[particleIndex];
			if (currentParticle.m_isStatic || currentParticle.m_lastUpdateStep == m_currentSimulationStepCount)
				continue;

			dynamicParticleCount++;
			switch (currentParticle.m_type)
			{
			case NoitaParticle::Type::Sand:
				UpdateSandParticle(x, y, particleIndex, deltaTime, currentParticle);
				break;
			case NoitaParticle::Type::Water:
				UpdateWaterParticle(x, y, particleIndex, deltaTime, currentParticle);
				break;
			default:
				currentParticle.m_isStatic = true;
				break;
			}
		}
	}

	return dynamicParticleCount;
}

void NoitaParticleChunk::ApplyChanges()
{
	std::sort(m_changes.begin(), m_changes.end());

	int previousChangeIndex = 0;
	m_changes.push_back(ParticleMovement());

	for (size_t i = 0; i < m_changes.size() - 1; i++)
	{
		if (m_changes[i + 1].m_destinationIndex != m_changes[i].m_destinationIndex || m_changes[i + 1].m_sourceChunk == nullptr)
		{
			int rand = nep::GetRandomValue(previousChangeIndex, i);
			NoitaParticle& sourceParticle = m_changes[rand].m_sourceChunk->m_particles[m_changes[rand].m_sourceIndex];
			NoitaParticle& destinationParticle = m_particles[m_changes[rand].m_destinationIndex];
			sf::Uint8 allowedTypesToSwap = 0;

			switch (sourceParticle.m_type)
			{
			case NoitaParticle::Type::Sand:
				allowedTypesToSwap = NoitaParticle::SandSwapAllowed;
				break;
			case NoitaParticle::Type::Water:
				allowedTypesToSwap = NoitaParticle::WaterSwapAllowed;
				break;
			}

			if (IsTypeInMask(destinationParticle.m_type, allowedTypesToSwap))
				m_changes[rand].m_sourceChunk->SwapParticles(m_changes[rand].m_sourceIndex, this, m_changes[rand].m_destinationIndex);

			previousChangeIndex = i + 1;
		}
	}

	m_texture.update(m_pixelsColor);
	m_changes.clear();
}

void NoitaParticleChunk::Draw(sf::RenderTarget& target)
{
	target.draw(m_sprite);
	target.draw(m_outline);
}

bool NoitaParticleChunk::AddChange(int destX, int destY, int sourceIndex, sf::Uint8 allowedTypesToSwap)
{
	NoitaParticleChunk* destinationChunk;
	int destinationIndex;
	if (IsInBounds(destX, destY))
	{
		destinationChunk = this;
		destinationIndex = GetIndex(destX, destY);
	}
	else
	{
		sf::Vector2i world = ComputeWorldPosition(destX, destY);
		destinationChunk = m_particleManager.GetChunk(world.x, world.y);
		sf::Vector2i local = destinationChunk->ComputeLocalPosition(world.x, world.y);
		destinationIndex = destinationChunk->GetIndex(local.x, local.y);
	}

	NoitaParticle& destinationParticle = destinationChunk->m_particles[destinationIndex];

	if (IsTypeInMask(destinationParticle.m_type, allowedTypesToSwap))
	{
		destinationChunk->m_changes.push_back(ParticleMovement{ this, sourceIndex, destinationIndex });
		return true;
	}

	return false;
}

void NoitaParticleChunk::PhysicSwapAction_SandParticle(NoitaParticle& original, NoitaParticle& swapped)
{
	switch (swapped.m_type)
	{
	case NoitaParticle::Type::Water:
		swapped.m_velocity.x = (CurrentRandValue & 1) == 0 ? -1.f : 1.f;
		swapped.m_velocity.y = -original.m_velocity.y * 0.25f;
		original.m_velocity.x = 0.f;
		original.m_velocity.y = 0.f;
		break;
	case NoitaParticle::Type::Empty:
		break;
	default:
		original.m_velocity.x = 0.f;
		original.m_velocity.y = 0.f;
		break;
	}
};

void NoitaParticleChunk::UpdateSandParticle(int x, int y, int index, float deltaTime, NoitaParticle& currentParticle)
{
	if (UpdateParticlePhysic(currentParticle, x, y, index, deltaTime, NoitaParticle::SandSwapAllowed, &NoitaParticleChunk::PhysicSwapAction_SandParticle))
		return;

	if (AddChange(x, y - 1, index, NoitaParticle::SandSwapAllowed))
		return;

	AddChange(x - 1, y - 1, index, NoitaParticle::SandSwapAllowed);
	AddChange(x + 1, y - 1, index, NoitaParticle::SandSwapAllowed);
}

void NoitaParticleChunk::UpdateWaterParticle(int x, int y, int index, float deltaTime, NoitaParticle& currentParticle)
{
	if (UpdateParticlePhysic(currentParticle, x, y, index, deltaTime, NoitaParticle::WaterSwapAllowed))
		return;

	if (AddChange(x, y - 1, index, NoitaParticle::WaterSwapAllowed))
		return;

	bool changeAdded = AddChange(x - 1, y - 1, index, NoitaParticle::WaterSwapAllowed);
	if (AddChange(x + 1, y - 1, index, NoitaParticle::WaterSwapAllowed) || changeAdded)
		return;

	AddChange(x - 1, y, index, NoitaParticle::WaterSwapAllowed);
	AddChange(x + 1, y, index, NoitaParticle::WaterSwapAllowed);
}

bool NoitaParticleChunk::UpdateParticlePhysic(NoitaParticle& currentParticle, int x, int y, int index, float deltaTime, sf::Uint8 allowedTypesToSwap, NoitaParticleChunk::SwapAction physicSwapAction)
{
	float tmpVelocityY = currentParticle.m_velocity.y + NoitaChunkParticleManager::Gravity * deltaTime;
	currentParticle.m_velocity.y = nep::Clamp(tmpVelocityY, -10.f, 10.f);

	float velocityMagnitudeSqr = nep::Square(currentParticle.m_velocity.x) + nep::Square(currentParticle.m_velocity.y);

	if (velocityMagnitudeSqr > 1.f)
	{
		NoitaParticleChunk* originChunk = this;
		NoitaParticleChunk* currentChunk = this;
		float velocityMagnitude = std::sqrt(velocityMagnitudeSqr);
		sf::Vector2f velocityDirection(currentParticle.m_velocity.x / velocityMagnitude, currentParticle.m_velocity.y / velocityMagnitude);
		sf::Vector2i localPosition(x, y);

		sf::Vector2i nextPosition(x, y);

		int stepCount = (int)velocityMagnitude;
		for (int i = 1; i <= stepCount; i++)
		{
			nextPosition.x = (int)(localPosition.x + i * velocityDirection.x + 0.5f);
			nextPosition.y = (int)(localPosition.y + i * velocityDirection.y + 0.5f);

			NoitaParticle::Type newType;
			if (currentChunk->IsInBounds(nextPosition))
				newType = currentChunk->m_particles[currentChunk->GetIndex(nextPosition)].m_type;
			else
			{
				sf::Vector2i worldPosition = currentChunk->ComputeWorldPosition(nextPosition);
				currentChunk = m_particleManager.GetChunk(worldPosition);
				nextPosition = currentChunk->ComputeLocalPosition(worldPosition);
				localPosition = nextPosition;
				int newIndex = currentChunk->GetIndex(nextPosition);
				newType = currentChunk->m_particles[newIndex].m_type;
			}

			if (!IsTypeInMask(newType, allowedTypesToSwap))
			{
				currentParticle.m_velocity.x = 0.f;
				currentParticle.m_velocity.y = 0.f;
				nextPosition.x = (int)(localPosition.x + (i - 1) * velocityDirection.x + 0.5f);
				nextPosition.y = (int)(localPosition.y + (i - 1) * velocityDirection.y + 0.5f);

				if (!currentChunk->IsInBounds(nextPosition))
				{
					sf::Vector2i world = currentChunk->ComputeWorldPosition(nextPosition);
					currentChunk = m_particleManager.GetChunk(world);
					nextPosition = currentChunk->ComputeLocalPosition(world);
				}
				break;
			}
		}

		if (originChunk != currentChunk)
		{
			sf::Vector2i world = currentChunk->ComputeWorldPosition(nextPosition);
			nextPosition = originChunk->ComputeLocalPosition(world);
		}

		if (localPosition.x != nextPosition.x || localPosition.y != nextPosition.y)
		{
			TrySwapParticles(index, nextPosition.x, nextPosition.y, allowedTypesToSwap, physicSwapAction);
			return true;
		}
		else
		{
			currentParticle.m_velocity.x = 0.f;
			currentParticle.m_velocity.y = 0.f;
			return false;
		}
	}

	return false;
}

void NoitaParticleChunk::SpawnParticle(const sf::Vector2i& position, const NoitaParticle& particleToSpawn)
{
	SpawnParticle(position.x, position.y, particleToSpawn);
}

void NoitaParticleChunk::SpawnParticle(int x, int y, const NoitaParticle& particleToSpawn)
{
	int index = GetIndex(x, y);

	m_particles[index] = particleToSpawn;
	SetPixelsColor(m_particles[index].m_type, index);
}

bool NoitaParticleChunk::TrySwapParticles(int index, int destX, int destY, sf::Uint8 allowedTypesToSwap, NoitaParticleChunk::SwapAction physicSwapAction)
{
	NoitaParticleChunk* newChunk;
	int newIndex;

	if (IsInBounds(destX, destY))
	{
		newChunk = this;
		newIndex = GetIndex(destX, destY);
	}
	else
	{
		sf::Vector2i world = ComputeWorldPosition(destX, destY);
		newChunk = m_particleManager.GetChunk(world.x, world.y);
		sf::Vector2i local = newChunk->ComputeLocalPosition(world.x, world.y);
		newIndex = newChunk->GetIndex(local.x, local.y);
	}

	if (IsTypeInMask(newChunk->m_particles[newIndex].m_type, allowedTypesToSwap))
	{
		SwapParticles(index, newChunk, newIndex);

		if (physicSwapAction)
			physicSwapAction(newChunk->m_particles[newIndex], m_particles[index]);

		return true;
	}

	return false;
}

// Utility
sf::Vector2i NoitaParticleChunk::ComputeLocalPosition(const sf::Vector2i& world) const
{
	return ComputeLocalPosition(world.x, world.y);
}

sf::Vector2i NoitaParticleChunk::ComputeWorldPosition(const sf::Vector2i& local) const
{
	return ComputeWorldPosition(local.x, local.y);
}

sf::Vector2i NoitaParticleChunk::ComputeLocalPosition(int worldX, int worldY) const
{
	return sf::Vector2i(worldX - m_worldPosition.x, worldY - m_worldPosition.y);
}

sf::Vector2i NoitaParticleChunk::ComputeWorldPosition(int localX, int localY) const
{
	return sf::Vector2i(m_worldPosition.x + localX, m_worldPosition.y + localY);
}

const sf::Vector2i& NoitaParticleChunk::GetChunkWorldPosition() const
{
	return m_chunkWorldPosition;
}

constexpr int NoitaParticleChunk::GetIndex(const sf::Vector2i& position) const
{
	return GetIndex(position.x, position.y);
}

constexpr int NoitaParticleChunk::GetIndex(int x, int y) const
{
	return (y * ChunkSize) + x;
}

constexpr bool NoitaParticleChunk::IsInBounds(const sf::Vector2i& position) const
{
	return IsInBounds(position.x, position.y);
}

constexpr bool NoitaParticleChunk::IsInBounds(int x, int y) const
{
	return x >= 0 && x < ChunkSize&& y >= 0 && y < ChunkSize;
}

constexpr bool NoitaParticleChunk::IsTypeInMask(NoitaParticle::Type type, sf::Uint8 mask) const
{
	return ((1 << (sf::Uint8)type) & mask) != 0;
}

void NoitaParticleChunk::SwapParticles(int sourceIndex, NoitaParticleChunk* destinationChunk, int destinationIndex)
{
	NoitaParticle& sourceParticle = m_particles[sourceIndex];
	NoitaParticle& destinationParticle = destinationChunk->m_particles[destinationIndex];

	std::swap(sourceParticle, destinationParticle);
	destinationParticle.m_lastUpdateStep = m_currentSimulationStepCount;

	SetPixelsColor(sourceParticle.m_type, sourceIndex);
	destinationChunk->SetPixelsColor(destinationParticle.m_type, destinationIndex);
}

void NoitaParticleChunk::SetPixelsColor(NoitaParticle::Type type, int particleIndex)
{
	int colorIndex = particleIndex * 4;
	const sf::Color& newColor = m_particleManager.GetColor(type);
	m_pixelsColor[colorIndex] = newColor.r;
	m_pixelsColor[colorIndex + 1] = newColor.g;
	m_pixelsColor[colorIndex + 2] = newColor.b;
	m_pixelsColor[colorIndex + 3] = newColor.a;
}