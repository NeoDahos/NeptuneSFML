#include "CreeperWorldGame.h"

void CreeperWorldGame::Init(sf::RenderWindow& _window)
{
	m_map.Init(static_cast<sf::Vector2i>(_window.getSize()), "../Data/Misc/terrain.txt");
	m_isLeftMouseButtonDown = false;
}

void CreeperWorldGame::DeInit()
{
}

void CreeperWorldGame::HandleEvent(const sf::Event & _event)
{
	if (_event.type == sf::Event::MouseButtonPressed)
	{
		if (_event.mouseButton.button == sf::Mouse::Button::Left)
		{
			m_map.StartMoveMap(_event.mouseButton.x, _event.mouseButton.y);
			m_isLeftMouseButtonDown = true;
		}
	}
	else if (_event.type == sf::Event::MouseButtonReleased)
	{
		if (_event.mouseButton.button == sf::Mouse::Button::Left)
		{
			m_map.StopMoveMap();
			m_isLeftMouseButtonDown = false;
		}
	}
	else if (m_isLeftMouseButtonDown && _event.type == sf::Event::MouseMoved)
	{
		m_map.MoveMap(_event.mouseMove.x, _event.mouseMove.x);
	}
}

void CreeperWorldGame::Update(float _deltaTime)
{
	m_map.Update(_deltaTime);
}

void CreeperWorldGame::Draw(sf::RenderTarget & _target, const sf::RenderStates& _states)
{
	m_map.Draw(_target);
}