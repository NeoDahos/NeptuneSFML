#include "GameOfLifeWorld.h"

#include <chrono>
#include <random>

#include <SFML\Graphics\RenderTarget.hpp>

#include <stdio.h>

GameOfLifeWorld::GameOfLifeWorld()
{
	m_cellSize = 10.f;
	m_cell.setSize({ m_cellSize, m_cellSize });
	m_cell.setFillColor(sf::Color::White);
}

void GameOfLifeWorld::InitWorld(unsigned int _width, unsigned int _height, bool _randomInit)
{
	if (m_currentWorld != nullptr)
	{
		for (unsigned int i = 0; i < m_height; i++)
		{
			delete[] m_currentWorld[i];
			delete[] m_nextWorld[i];
		}

		delete[] m_currentWorld;
		delete[] m_nextWorld;
	}

	m_width = _width;
	m_height = _height;

	m_currentWorld = new unsigned char*[m_height];
	m_nextWorld = new unsigned char*[m_height];

	for (unsigned int i = 0; i < m_height; i++)
	{
		m_currentWorld[i] = new unsigned char[m_width];
		m_nextWorld[i] = new unsigned char[m_width];
	}


	if (_randomInit)
	{
		std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
		std::uniform_int_distribution<unsigned int> distribution(0, 10);

		for (unsigned int y = 0; y < m_height; y++)
		{
			for (unsigned int x = 0; x < m_width; x++)
			{
				m_currentWorld[y][x] = (distribution(generator) <= 1) ? 1 : 0;
				m_nextWorld[y][x] = m_currentWorld[y][x];
			}
		}
	}
	else
	{
		for (unsigned int y = 0; y < m_height; y++)
		{
			for (unsigned int x = 0; x < m_width; x++)
			{
				m_currentWorld[y][x] = 0;
				m_nextWorld[y][x] = 0;
			}
		}
	}
}

void GameOfLifeWorld::ZoomIn()
{
	m_cellSize += 0.5f;
	m_cell.setSize({ m_cellSize, m_cellSize });
}

void GameOfLifeWorld::ZoomOut()
{
	m_cellSize -= 0.5f;
	if (m_cellSize < 1.f)
		m_cellSize = 1.f;

	m_cell.setSize({ m_cellSize, m_cellSize });
}


void GameOfLifeWorld::Simulate()
{
	unsigned char nextWorldValue = 0;

	for (unsigned int y = 1; y < m_height - 1; y++)
	{
		for (unsigned int x = 1; x < m_width - 1; x++)
		{
			nextWorldValue = m_currentWorld[y-1][x-1] +	m_currentWorld[y-1][x] +	m_currentWorld[y-1][x+1] +
								m_currentWorld[y][x-1] +							m_currentWorld[y][x+1] +
								m_currentWorld[y+1][x-1] +	m_currentWorld[y+1][x] +	m_currentWorld[y+1][x+1];

			m_nextWorld[y][x] = ((m_currentWorld[y][x] == 1 && (nextWorldValue >= 2 && nextWorldValue <= 3))
								|| (m_currentWorld[y][x] == 0 && nextWorldValue == 3)) ? 1 : 0;
		}
	}

	std::swap(m_currentWorld, m_nextWorld);
}

void GameOfLifeWorld::PrintWorld()
{
	for (unsigned int y = 0; y < m_height; y++)
	{
		for (unsigned int x = 0; x < m_width; x++)
		{
			printf("%d ", m_currentWorld[y][x]);
		}
		printf("\n");
	}
	printf("\n\n\n");
}

void GameOfLifeWorld::Draw(class sf::RenderTarget & _target)
{	
	for (unsigned int y = 0; y < m_height; y++)
	{
		for (unsigned int x = 0; x < m_width; x++)
		{
			if (m_currentWorld[y][x] == 1)
			{
				m_cell.setPosition(x * m_cellSize, y * m_cellSize);
				_target.draw(m_cell);
			}
		}
	}
}