#ifndef SHMUP_GAME_H
#define SHMUP_GAME_H

#include <NeptuneSFML\Game.h>
#include <NeptuneSFML\EngineCore.h>

#include "Player.h"
#include "BulletsManager.h"

class ShmupGame : public nep::Game
{
public:
	virtual void Init(sf::RenderWindow& _window);
	virtual void DeInit();
	virtual void HandleEvent(const sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderTarget& _target);

private:
	Player m_player;
	BulletsManager m_bulletsManager;
};

#endif // SHMUP_GAME_H