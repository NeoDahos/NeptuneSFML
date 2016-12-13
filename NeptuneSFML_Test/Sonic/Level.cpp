#include "Level.h"

#include <NeptuneSFML\EngineCore.h>

void Level::Init(const sf::String& _textureName)
{
	m_texture = &nep::TextureMng.GetTexture(_textureName);
}

void Level::Draw(sf::RenderTarget& _target)
{
	size_t chunkCount = m_chunks.size();
	for (size_t i = 0; i < chunkCount; i++)
		_target.draw(m_chunks[i].sprite);
}

void Level::AddChunk(const Chunk& _chunk)
{
	Chunk chunk = _chunk;
	chunk.sprite.setTexture(*m_texture);
	chunk.sprite.setTextureRect(chunk.rect);
	chunk.sprite.setPosition(chunk.position);
	m_chunks.push_back(chunk);
}