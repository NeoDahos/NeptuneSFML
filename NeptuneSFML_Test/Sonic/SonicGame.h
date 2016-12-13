#ifndef SONIC_GAME_H
#define SONIC_GAME_H
#include <NeptuneSFML\Game.h>

#include "Sonic.h"
#include "Level.h"

class SonicGame : public nep::Game
{
public:
	virtual void Init(sf::RenderWindow& _window);
	virtual void DeInit();
	virtual void HandleEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderTarget& _target);

private:
	Sonic m_sonic;
	Level m_level;
};

#endif // SONIC_GAME_H