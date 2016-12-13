#include "CreeperWorldGame.h"

void CreeperWorldGame::Init(sf::RenderWindow& _window)
{
	
	m_map.Init(static_cast<sf::Vector2i>(_window.getSize()), "../Data/Misc/terrain.txt");
}

void CreeperWorldGame::DeInit()
{
}

void CreeperWorldGame::HandleEvent(sf::Event & _event)
{
}

void CreeperWorldGame::Update(float _deltaTime)
{
	m_map.Update(_deltaTime);
}

void CreeperWorldGame::Draw(sf::RenderTarget & _target)
{
	m_map.Draw(_target);
}