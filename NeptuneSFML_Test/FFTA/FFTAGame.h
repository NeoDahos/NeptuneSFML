#ifndef FFTA_GAME_H
#define FFTA_GAME_H

#include <NeptuneSFML\Game.h>

#include "Battle\FFTABattle.h"

class FFTAGame : public nep::Game
{
public:
	virtual void Init(sf::RenderWindow& _window);
	virtual void DeInit();
	virtual void HandleEvent(const sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderTarget& _target);

private:
	FFTABattle m_battle;
};

#endif // FFTA_GAME_H