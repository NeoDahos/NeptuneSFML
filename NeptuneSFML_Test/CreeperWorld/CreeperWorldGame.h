#ifndef CREEPER_WORLD_GAME_H
#define CREEPER_WORLD_GAME_H

#include <NeptuneSFML\Game.h>
#include <NeptuneSFML\EngineCore.h>

#include "CreeperMap\CreeperMap.h"

#include <vector>

class CreeperWorldGame : public nep::Game
{
public:
	virtual void Init(sf::RenderWindow& _window);
	virtual void DeInit();
	virtual void HandleEvent(const sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderTarget& _target);

private:
	CreeperMap m_map;
};

#endif // CREEPER_WORLD_GAME_H