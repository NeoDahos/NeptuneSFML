#ifndef ZELDA_GAME_H
#define ZELDA_GAME_H

#include <NeptuneSFML\Game.h>
#include <NeptuneSFML\TiledMap\TileSet.h>
#include <NeptuneSFML\TiledMap\TiledMapZone.h>
#include <NeptuneSFML\TiledMap\TiledMap.h>

class ZeldaGame : public nep::Game
{
public:
	virtual void Init(sf::RenderWindow& _window);
	virtual void DeInit();
	virtual void HandleEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderTarget& _target);

private:
	nep::TiledMap m_map;
	nep::TiledMapZone m_zone;
};

#endif // ZELDA_GAME_H