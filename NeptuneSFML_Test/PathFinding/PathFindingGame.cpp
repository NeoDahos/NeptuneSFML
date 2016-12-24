#include "PathFindingGame.h"

#include <windows.h>


void PathFindingGame::Init(sf::RenderWindow& _window)
{
	m_scale = 2.f;
	m_map.Init("../Data/Misc/map256.txt", m_scale);
	m_astar.Init(&m_map, m_scale);

	m_start = { 4, 4 };
	m_goal = { 246, 246 };
	m_searchStatus = AStar::Status::Waiting;
	m_editMap = false;
}

void PathFindingGame::DeInit()
{
}

void PathFindingGame::HandleEvent(const sf::Event & _event)
{
	if (_event.type == sf::Event::KeyPressed)
	{
		if (_event.key.code == sf::Keyboard::Space)
		{
			if (m_searchStatus != AStar::Status::Running)
			{
				m_searchTime = sf::Time::Zero;
				m_searchStatus = AStar::Status::Running;
				m_editMap = false;
			}
		}
		else if (_event.key.code == sf::Keyboard::B)
		{
			if (m_searchStatus != AStar::Status::Running)
			{
				m_searchTime = sf::Time::Zero;
				m_clockSearch.restart();
				m_searchStatus = m_astar.Search(m_start, m_goal);
				m_searchTime += m_clockSearch.getElapsedTime();
			}

		}
		else if (_event.key.code == sf::Keyboard::E)
		{
			m_editMap = !m_editMap;
		}
	}
	else if (_event.type == sf::Event::MouseButtonPressed && m_searchStatus != AStar::Status::Running)
	{
		if (m_editMap)
		{
			if (_event.mouseButton.button == sf::Mouse::Left)
				m_map.SetWalkability(static_cast<int>(_event.mouseButton.x / m_scale), static_cast<int>(_event.mouseButton.y / m_scale), 1);
			else if (_event.mouseButton.button == sf::Mouse::Right)
				m_map.SetWalkability(static_cast<int>(_event.mouseButton.x / m_scale), static_cast<int>(_event.mouseButton.y / m_scale), 0);
		}
		else
		{
			if (_event.mouseButton.button == sf::Mouse::Left)
			{
				m_start.x = static_cast<int>(_event.mouseButton.x / m_scale);
				m_start.y = static_cast<int>(_event.mouseButton.y / m_scale);
			}
			else if (_event.mouseButton.button == sf::Mouse::Right)
			{
				m_goal.x = static_cast<int>(_event.mouseButton.x / m_scale);
				m_goal.y = static_cast<int>(_event.mouseButton.y / m_scale);
			}
		}
	}
	else if (_event.type == sf::Event::MouseMoved && m_searchStatus != AStar::Status::Running && m_editMap)
	{
		if (m_oldMousePosition.x != _event.mouseMove.x || m_oldMousePosition.y != _event.mouseMove.y)
		{
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
				m_map.SetWalkability(static_cast<int>(_event.mouseMove.x / m_scale), static_cast<int>(_event.mouseMove.y / m_scale), 1);
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				m_map.SetWalkability(static_cast<int>(_event.mouseMove.x / m_scale), static_cast<int>(_event.mouseMove.y / m_scale), 0);

			m_oldMousePosition.x = _event.mouseMove.x;
			m_oldMousePosition.y = _event.mouseMove.y;
		}
	}
}

void PathFindingGame::Update(float _deltaTime)
{
	if (m_searchStatus == AStar::Status::Running)
	{
		m_clockSearch.restart();
		m_searchStatus = m_astar.SearchStep(m_start, m_goal);
		m_searchTime += m_clockSearch.getElapsedTime();
	}
}

void PathFindingGame::Draw(sf::RenderTarget & _target)
{
	m_map.Draw(_target);
	m_astar.Draw(_target);
}