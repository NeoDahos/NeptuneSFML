#include "ZeldaGame.h"
#include <NeptuneSFML\EngineCore.h>

void ZeldaGame::Init(sf::RenderWindow& _window)
{
	nep::TextureMng.AddTexture("Temple Green", "../Data/Zelda/tilesets/temple_green.tiles.png");
	nep::TileSetMng.AddTileSet("Temple Green", "../Data/Misc/temple_green.json");

	//m_zone.Init("Temple entrance", nep::TileSetMng.GetTileSet("Temple Green"));
	m_zone.Init("Temple entrance", "../Data/Misc/templeEntrance.json");
	m_map.AddZone("Temple entrance", &m_zone);
	m_map.SetCurrentZone("Temple entrance");
}

void ZeldaGame::DeInit()
{
	nep::TextureMng.EraseTexture("Temple Green");
	nep::TileSetMng.EraseTileSet("Temple Green");
}

void ZeldaGame::HandleEvent(sf::Event& _event)
{
}

void ZeldaGame::Update(float _deltaTime)
{
	m_map.Update(_deltaTime);
}

void ZeldaGame::Draw(sf::RenderTarget& _target)
{
	m_map.Draw(_target);
}