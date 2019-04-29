#ifndef GAME_OF_LIFE_WORLD_H
#define GAME_OF_LIFE_WORLD_H

#include <SFML\Graphics\RectangleShape.hpp>

namespace sf
{
	class RenderTarget;
}

class GameOfLifeWorld
{
public:
	GameOfLifeWorld();

	void InitWorld(unsigned int _width, unsigned int _height, bool _randomInit = true);
	void ZoomIn();
	void ZoomOut();
	void Simulate();

	void PrintWorld();
	void Draw(sf::RenderTarget& _target);

private:
	sf::RectangleShape m_cell;

	float m_cellSize;

	unsigned int m_width;
	unsigned int m_height;

	unsigned char** m_currentWorld;
	unsigned char** m_nextWorld;
};

#endif // GAME_OF_LIFE_WORLD_H