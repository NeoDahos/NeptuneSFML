#include "IsaacGame.h"

void IsaacGame::Init(sf::RenderWindow& _window)
{
	m_currentFloor = 0;
	m_floor.emplace_back();
}

void IsaacGame::DeInit()
{
}

void IsaacGame::HandleEvent(const sf::Event& _event)
{
	if (_event.type == sf::Event::KeyPressed)
	{
		if (_event.key.code == sf::Keyboard::Space)
			m_floor[m_currentFloor].Regenerate(20, 20, 20);
	}
}

void IsaacGame::Update(float _deltaTime)
{
}

void IsaacGame::Draw(sf::RenderTarget & _target)
{
	m_floor[m_currentFloor].Draw(_target);
}