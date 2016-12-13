#ifndef PATH_FINDING_H
#define PATH_FINDING_H

#include <NeptuneSFML\Game.h>

#include <SFML\System\Clock.hpp>

#include "Map.h"
#include "AStar.h"

class PathFindingGame : public nep::Game
{
public:
	virtual void Init(sf::RenderWindow& _window);
	virtual void DeInit();
	virtual void HandleEvent(sf::Event & _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderTarget & _target);

private:
	Map m_map;
	AStar m_astar;

	sf::Clock m_clockSearch;
	sf::Time m_searchTime;

	sf::Vector2i m_start;
	sf::Vector2i m_goal;
	sf::Vector2i m_oldMousePosition;
	AStar::Status m_searchStatus;
	float m_scale;
	bool m_editMap;
};

#endif // PATH_FINDING_H