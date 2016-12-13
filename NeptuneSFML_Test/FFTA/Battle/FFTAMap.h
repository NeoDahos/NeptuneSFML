#ifndef FFTA_MAP_H
#define FFTA_MAP_H

#include <SFML\System\String.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderWindow.hpp>

#include <NeptuneSFML\Animation\Animation.h>

class FFTAMap
{
public:
	void Init(const sf::String& _jsonPath, sf::RenderWindow & _window);
	void DeInit();
	void Update(float _deltaTime);
	void DrawLayer(sf::RenderTarget& _target, unsigned int _layer);

	sf::Vector2f GetIsometricPosition(int _x, int _y) const;
	sf::Vector2f GetIsometricPosition(const sf::Vector2i & _position) const;
	sf::Vector2i GetClampedTilePosition(int _x, int _y) const;
	sf::Vector2i GetClampedTilePosition(const sf::Vector2i & _position) const;
	int GetTileElevation(int _x, int _y) const;
	int GetTileElevation(const sf::Vector2i _position) const;
	int GetTilePriority(int _x, int _y) const;
	int GetTilePriority(const sf::Vector2i _position) const;
	int GetLayerCount() const;
	const std::vector<sf::Vector2i> & GetStarTiles() const;

private:
	nep::Animation m_layer0;
	std::vector<sf::Sprite> m_layers;
	std::vector<sf::Vector2i> m_startTiles;
	sf::Color m_backgroundColor;
	sf::Vector2f m_referencePosition;
	int m_mapWidth;
	int m_mapHeight;
	int * m_heightMap;
	int * m_walkabilityMap;
	int * m_priorityMap;
};

#endif // FFTA_MAP_H