#ifndef ISAAC_GAME_H
#define ISAAC_GAME_H

#include <NeptuneSFML\Game.h>
#include <NeptuneSFML\EngineCore.h>

#include "Floor.h"

#include <vector>

class IsaacGame : public nep::Game
{
public:
	virtual void Init(sf::RenderWindow& _window);
	virtual void DeInit();
	virtual void HandleEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderTarget& _target);

private:
	std::vector<Floor> m_floor;
	int m_currentFloor;
};

#endif // ISAAC_GAME_H