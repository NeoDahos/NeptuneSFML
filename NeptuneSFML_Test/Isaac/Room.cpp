#include "Room.h"
#include "Floor.h"

#include <SFML\Graphics\RenderTarget.hpp>

Room::Room(Floor* _floor, unsigned short _posX, unsigned short _posY, const sf::Color& _color) : m_floor(_floor), m_posX(_posX), m_posY(_posY), m_neighborCount(0)
{
	m_isNeighborRoomOccupied[EDirection::Up] = false;
	m_isNeighborRoomOccupied[EDirection::Rigth] = false;
	m_isNeighborRoomOccupied[EDirection::Down] = false;
	m_isNeighborRoomOccupied[EDirection::Left] = false;

	m_isBuildPossible[EDirection::Up] = m_posY - 1 >= 0;
	m_isBuildPossible[EDirection::Rigth] = m_posX + 1 < m_floor->GetWidth();
	m_isBuildPossible[EDirection::Down] = m_posY + 1 < m_floor->GetHeight();
	m_isBuildPossible[EDirection::Left] = m_posX - 1 >= 0;

	m_shape.setFillColor(_color);
	m_shape.setPosition((float)m_posX * 12.f + 100.f, (float)m_posY * 12.f + 100.f);
	m_shape.setSize(sf::Vector2f(10.f, 10.f));
}

void Room::SetNeighborRoom(EDirection _direction)
{
	m_isNeighborRoomOccupied[_direction] = true;
	m_isBuildPossible[_direction] = false;

	m_neighborCount++;
}

bool Room::CanCreateNeighborRoom()
{
	for (unsigned char dir = 0; dir < EDirection::Count; dir++)
		if (!m_isNeighborRoomOccupied[dir] && m_isBuildPossible[dir])
			return true;

	return false;
}

bool Room::CanCreateNeighborRoom(EDirection _direction)
{
	return !m_isNeighborRoomOccupied[_direction] && m_isBuildPossible[_direction];
}

unsigned short Room::GetX() const
{
	return m_posX;
}

unsigned short Room::GetY() const
{
	return m_posY;
}

unsigned short Room::GetNeighborCount() const
{
	return m_neighborCount;
}

void Room::Draw(sf::RenderTarget& _renderTarget)
{
	_renderTarget.draw(m_shape);
}