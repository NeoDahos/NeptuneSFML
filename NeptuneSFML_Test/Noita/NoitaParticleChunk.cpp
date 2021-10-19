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

NoitaParticleChunk::NoitaParticleChunk(NoitaChunkParticleManager& managerRef, int chunkWorldX, int chunkWorldY, bool updateFlag) :
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
			m_elements[index] = new NoitaElement(updateFlag);
			SetPixelsColor(NoitaElementCommon::Type::Empty, index);
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

NoitaParticleChunk::~NoitaParticleChunk()
{
	for (int i = 0; i < ChunkSize * ChunkSize; i++)
		delete m_elements[i];
}

int NoitaParticleChunk::UpdateSimulation(bool updateFlag, float deltaTime)
{
	CurrentRandValue = (int)(nep::GetRandomValue() * 100000.f);

	int dynamicParticleCount = 0;
	for (int y = 0; y < ChunkSize; y++)
	{
		for (int x = 0; x < ChunkSize; x++)
		{
			int elementIndex = GetIndex(x, y);
			NoitaElement* currentElement = m_elements[elementIndex];
			currentElement->Update(*this, x, y, elementIndex, updateFlag);
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
			NoitaElement* sourceElement = m_changes[rand].m_sourceChunk->m_elements[m_changes[rand].m_sourceIndex];
			NoitaElement* destinationElement = m_elements[m_changes[rand].m_destinationIndex];
			sf::Uint8 allowedTypesToSwap = 0;

			switch (sourceElement->m_type)
			{
			case NoitaElementCommon::Type::Sand:
				allowedTypesToSwap = NoitaParticle::SandSwapAllowed;
				break;
			case NoitaElementCommon::Type::Water:
				allowedTypesToSwap = NoitaParticle::WaterSwapAllowed;
				break;
			}

			if (IsTypeInMask(destinationElement.m_type, allowedTypesToSwap))
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
	NoitaElementCommon::ChunkElement destinationChunkElement = GetChunkElement(destX, destY);

	NoitaParticle& destinationParticle = destinationChunkElement.m_chunk->m_elements[destinationChunkElement.m_index];

	if (IsTypeInMask(destinationParticle.m_type, allowedTypesToSwap))
	{
		destinationChunkElement.m_chunk->m_changes.push_back(ParticleMovement{ this, sourceIndex, destinationChunkElement.m_index });
		return true;
	}

	return false;
}

void NoitaParticleChunk::AddChange(ParticleMovement&& particleMovement)
{
	m_changes.push_back(std::forward<ParticleMovement>(particleMovement));
}

NoitaElementCommon::ChunkElement NoitaParticleChunk::GetChunkElement(int destX, int destY)
{
	NoitaElementCommon::ChunkElement chunkElement;

	if (IsInBounds(destX, destY))
	{
		chunkElement.m_chunk = this;
		chunkElement.m_index = GetIndex(destX, destY);
		chunkElement.m_element = &m_elements[chunkElement.m_index];
	}
	else
	{
		sf::Vector2i world = ComputeWorldPosition(destX, destY);
		chunkElement.m_chunk = m_particleManager.GetChunk(world.x, world.y);
		sf::Vector2i local = chunkElement.m_chunk->ComputeLocalPosition(world.x, world.y);
		chunkElement.m_index = chunkElement.m_chunk->GetIndex(local.x, local.y);
		chunkElement.m_element = &chunkElement.m_chunk->m_elements[chunkElement.m_index];
	}

	return chunkElement;
}

void NoitaParticleChunk::PhysicSwapAction_SandParticle(NoitaParticle& original, NoitaParticle& swapped)
{
	switch (swapped.m_type)
	{
	case NoitaElementCommon::Type::Water:
		swapped.m_velocity.x = (CurrentRandValue & 1) == 0 ? -1.f : 1.f;
		swapped.m_velocity.y = -original.m_velocity.y * 0.25f;
		original.m_velocity.x = 0.f;
		original.m_velocity.y = 0.f;
		break;
	case NoitaElementCommon::Type::Empty:
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

			NoitaElementCommon::Type newType;
			if (currentChunk->IsInBounds(nextPosition))
				newType = currentChunk->m_elements[currentChunk->GetIndex(nextPosition)].m_type;
			else
			{
				sf::Vector2i worldPosition = currentChunk->ComputeWorldPosition(nextPosition);
				currentChunk = m_particleManager.GetChunk(worldPosition);
				nextPosition = currentChunk->ComputeLocalPosition(worldPosition);
				localPosition = nextPosition;
				int newIndex = currentChunk->GetIndex(nextPosition);
				newType = currentChunk->m_elements[newIndex].m_type;
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

	m_elements[index] = particleToSpawn;
	SetPixelsColor(m_elements[index].m_type, index);
}

// Utility
void NoitaParticleChunk::SwapParticles(int sourceIndex, NoitaParticleChunk* destinationChunk, int destinationIndex)
{
	std::swap(m_elements[sourceIndex], destinationChunk->m_elements[destinationIndex]);

	SetPixelsColor(m_elements[sourceIndex]->GetType(), sourceIndex);
	destinationChunk->SetPixelsColor(destinationChunk->m_elements[destinationIndex]->GetType(), destinationIndex);
}

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

constexpr bool NoitaParticleChunk::IsTypeInMask(NoitaElementCommon::Type type, sf::Uint8 mask) const
{
	return ((1 << (sf::Uint8)type) & mask) != 0;
}

void NoitaParticleChunk::SetPixelsColor(NoitaElementCommon::Type type, int particleIndex)
{
	int colorIndex = particleIndex * 4;
	const sf::Color& newColor = NoitaElementCommon::ParticleColors[(sf::Uint8)type];
	m_pixelsColor[colorIndex] = newColor.r;
	m_pixelsColor[colorIndex + 1] = newColor.g;
	m_pixelsColor[colorIndex + 2] = newColor.b;
	m_pixelsColor[colorIndex + 3] = newColor.a;
}