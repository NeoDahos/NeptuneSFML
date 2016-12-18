#include "Map.h"

#include <SFML\Graphics\RectangleShape.hpp>

#include <NeptuneSFML\EngineCore.h>
#include <NeptuneSFML\Object\SpriteBatch.h>

#include <fstream>

#define IX(i, j) ((i) + ((j) * (m_size.x)))

void Map::Init(const sf::String & _path, float _scale)
{
	std::ifstream fileStream(_path.toAnsiString(), std::ifstream::in);
	sf::Sprite * sprite;
	m_scale = _scale;

	nep::TextureMng.AddTexture("Terrain Levels", "../Data/Sprites/Creeper World/levels.png");	
	m_terrainTexture = &nep::TextureMng.GetTexture("Terrain Levels");

	if (fileStream.is_open())
	{
		fileStream >> m_size.x >> m_size.y;

		m_map = new unsigned short[m_size.x * m_size.y];
		fileStream.get();

		for (unsigned short y = 0; y < m_size.y; y++)
		{
			for (unsigned short x = 0; x < m_size.x; x++)
			{
				m_map[IX(x, y)] = fileStream.get() - '0';
				sprite = new sf::Sprite(*m_terrainTexture, sf::IntRect((m_map[IX(x, y)] % 4) * 256, (m_map[IX(x, y)] / 4) * 256, 255, 255));
				sprite->setPosition(x * m_scale, y * m_scale);
				sprite->setScale(m_scale / 256, m_scale / 256);
				m_tiles.push_back(sprite);
			}
			fileStream.get();
		}
		fileStream.close();
	}
}

void Map::Draw(sf::RenderTarget & _target)
{
	nep::SpriteBatch::GetInstance().Draw(_target, m_tiles, m_terrainTexture);
}

void Map::SetWalkability(const sf::Vector2i & _position, unsigned short _walkability)
{
	if (_position.x >= 0 && _position.x < m_size.x && _position.y >= 0 && _position.y < m_size.y)
	{
		m_map[IX(_position.x, _position.y)] = _walkability;
		m_tiles[IX(_position.x, _position.y)]->setTextureRect({ (_walkability % 4) * 256, (_walkability / 4) * 256, 256, 256 });
	}
}

void Map::SetWalkability(int _positionX, int _positionY, unsigned short _walkability)
{
	if (_positionX >= 0 && _positionX < m_size.x && _positionY >= 0 && _positionY < m_size.y)
	{
		m_map[IX(_positionX, _positionY)] = _walkability;
		m_tiles[IX(_positionX, _positionY)]->setTextureRect({ (_walkability % 4) * 256, (_walkability / 4) * 256, 256, 256 });
	}
}

unsigned short Map::GetWalkability(const sf::Vector2i & _position) const
{
	if (_position.x >= 0 && _position.x < m_size.x && _position.y >= 0 && _position.y < m_size.y)
		return m_map[IX(_position.x, _position.y)];

	return -1;
}

unsigned short Map::GetWalkability(int _positionX, int _positionY) const
{
	if (_positionX >= 0 && _positionX < m_size.x && _positionY >= 0 && _positionY < m_size.y)
		return m_map[IX(_positionX, _positionY)];

	return -1;
}

bool Map::IsWalkable(const sf::Vector2i & _position) const
{
	return _position.x >= 0 && _position.x < m_size.x && _position.y >= 0 && _position.y < m_size.y && m_map[IX(_position.x, _position.y)] > 0;
}

bool Map::IsWalkable(int _positionX, int _positionY) const
{
	return _positionX >= 0 && _positionX < m_size.x && _positionY >= 0 && _positionY < m_size.y && m_map[IX(_positionX, _positionY)] > 0;
}