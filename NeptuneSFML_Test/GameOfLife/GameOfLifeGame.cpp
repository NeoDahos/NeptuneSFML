#include "GameOfLifeGame.h"
#include <NeptuneSFML\Tools.h>

void GameOfLifeGame::Init(sf::RenderWindow & _window)
{
	m_timeStep = 0.1f;
	m_timeSinceLastStep = 0.f;
	m_pause = true;
	m_world.InitWorld(400, 300);
}

void GameOfLifeGame::DeInit()
{
}

void GameOfLifeGame::HandleEvent(const sf::Event & _event)
{
	if (_event.type == sf::Event::KeyPressed)
	{
		if (_event.key.code == sf::Keyboard::G)
		{
			m_pause = true;
			m_timeSinceLastStep = 0.f;
			m_world.InitWorld(400, 300);
		}
		else if (_event.key.code == sf::Keyboard::P)
		{
			m_pause = !m_pause;
		}
		else if (_event.key.code == sf::Keyboard::Add)
		{
			m_timeStep -= 0.01f;

			if (m_timeStep < 0.01f)
				m_timeStep = 0.01f;
		}
		else if (_event.key.code == sf::Keyboard::Subtract)
		{
			m_timeStep += 0.01f;
		}
	}
	else if (_event.type == sf::Event::MouseWheelScrolled)
	{
		if (_event.mouseWheelScroll.delta > 0)
		{
			m_world.ZoomIn();
		}
		else
		{
			m_world.ZoomOut();
		}
	}
}

void GameOfLifeGame::Update(float _deltaTime)
{
	if (m_pause)
		return;

	m_timeSinceLastStep += _deltaTime;

	if (m_timeSinceLastStep >= m_timeStep)
	{
		m_world.Simulate();
		m_timeSinceLastStep = 0.f;
	}
}

void GameOfLifeGame::Draw(sf::RenderTarget & _target, const sf::RenderStates& _states)
{
	m_world.Draw(_target);
}