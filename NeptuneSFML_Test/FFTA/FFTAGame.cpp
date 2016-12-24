#include "FFTAGame.h"

#include <NeptuneSFML\EngineCore.h>

void FFTAGame::Init(sf::RenderWindow& _window)
{
	nep::TextureMng.AddTexture("Aisen Field", "../Data/Sprites/FFTA/Maps/Aisen Field.png");
	nep::TextureMng.AddTexture("Humans_Black Mage", "../Data/Sprites/FFTA/Characters/Humans/Humans_Black Mage.png");
	nep::TextureMng.AddTexture("BattleCursor", "../Data/Sprites/FFTA/Battle/BattleCursor.png");
	nep::TextureMng.AddTexture("SelectedCharacterInfos", "../Data/Sprites/FFTA/Battle/SelectedCharacterInfos.png");
	nep::TextureMng.AddTexture("PlaceCharacterInfos", "../Data/Sprites/FFTA/Battle/PlaceCharacterInfos.png");
	nep::TextureMng.AddTexture("TileHighlightBlue", "../Data/Sprites/FFTA/Battle/TileHighlightBlue.png");

	m_battle.Init(_window);
}

void FFTAGame::DeInit()
{
	nep::TextureMng.EraseTexture("Aisen Field");
	nep::TextureMng.EraseTexture("Humans_Black Mage");
	nep::TextureMng.EraseTexture("BattleCursor");
	nep::TextureMng.EraseTexture("SelectedCharacterInfos");
	nep::TextureMng.EraseTexture("PlaceCharacterInfos");
	nep::TextureMng.EraseTexture("TileHighlightBlue");

	m_battle.DeInit();
}

void FFTAGame::HandleEvent(const sf::Event& _event)
{
	m_battle.HandleEvent(_event);
}

void FFTAGame::Update(float _deltaTime)
{
	m_battle.Update(_deltaTime);
}

void FFTAGame::Draw(sf::RenderTarget& _target)
{
	m_battle.Draw(_target);
}