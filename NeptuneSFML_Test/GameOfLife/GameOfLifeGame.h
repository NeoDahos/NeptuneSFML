#ifndef GAME_Of_LIFE_GAME_H
#define GAME_Of_LIFE_GAME_H
#include <NeptuneSFML\Game.h>

#include "GameOfLifeWorld.h"

class GameOfLifeGame : public nep::Game
{
public:
	virtual void Init(sf::RenderWindow& _window);
	virtual void DeInit();
	virtual void HandleEvent(const sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderTarget& _target, const sf::RenderStates& _states = sf::RenderStates::Default);

private:
	GameOfLifeWorld m_world;

	float m_timeStep;
	float m_timeSinceLastStep;
	bool m_pause;
};

#endif //GAME_Of_LIFE_GAME_H