#ifndef MAP_H
#define MAP_H

#include <SFML\System\Vector2.hpp>
#include <SFML\System\String.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

#include <vector>

namespace sf
{
	class Sprite;
}

class Map
{
public:
	void Init(const sf::String & _path, float _scale);
	void Draw(sf::RenderTarget & _target);

	void SetWalkability(const sf::Vector2i & _position, unsigned short _walkability);
	void SetWalkability(int _positionX, int _positionY, unsigned short _walkability);
	unsigned short GetWalkability(const sf::Vector2i & _position) const;
	unsigned short GetWalkability(int _positionX, int _positionY) const;

	bool IsWalkable(const sf::Vector2i & _position) const;
	bool IsWalkable(int _positionX, int _positionY) const;

private:
	std::vector<sf::Sprite *> m_tiles;
	sf::Texture * m_terrainTexture;

	sf::Vector2i m_size;
	float m_scale;
	unsigned short * m_map;
};

#endif // MAP_H