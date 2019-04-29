#ifndef SOFT_BODY_GAME_H
#define SOFT_BODY_GAME_H
#include <NeptuneSFML\Game.h>

#include "SoftBodyObject.h"

class SoftBodyGame : public nep::Game
{
public:
	virtual void Init(sf::RenderWindow& _window);
	virtual void DeInit();
	virtual void HandleEvent(const sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderTarget& _target, const sf::RenderStates& _states = sf::RenderStates::Default);

private:
	SoftBodyObject m_ball;
};

#endif //SOFT_BODY_GAME_H