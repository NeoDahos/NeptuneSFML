#include "Floor.h"

#include <SFML\Graphics\RenderTarget.hpp>

#include <stdlib.h>

Floor::Floor(unsigned short _width, unsigned short _heigth, unsigned short _roomCount) : m_width(_width), m_heigth(_heigth), m_totalRoomCount(_roomCount)
{
	Generate();
}

Floor::~Floor()
{
	size_t roomCount = m_rooms.size();
	for (size_t i = 0; i < roomCount; i++)
		delete m_rooms[i];

	m_rooms.clear();
}

unsigned short Floor::GetWidth() const
{
	return m_width;
}

unsigned short Floor::GetHeight() const
{
	return m_heigth;
}

void Floor::Regenerate(unsigned short _width, unsigned short _heigth, unsigned short _roomCount)
{
	size_t roomCount = m_rooms.size();
	for (size_t i = 0; i < roomCount; i++)
		delete m_rooms[i];

	m_rooms.clear();

	m_width = _width;
	m_heigth = _heigth;
	m_totalRoomCount = _roomCount;
	Generate();
}

void Floor::Draw(sf::RenderTarget& _renderTarget)
{
	size_t roomCount = m_rooms.size();
	for (size_t i = 0; i < roomCount; i++)
		m_rooms[i]->Draw(_renderTarget);
}

void Floor::Generate()
{
	Room* newRoomNeighbors[4];
	unsigned short posX = std::rand() % (m_width + 1);
	unsigned short posY = std::rand() % (m_heigth + 1);
	unsigned short roomCreated = 0;
	unsigned short selectedRoom;
	Room::EDirection nextRoomDirection;
	bool isSelectedRoomOk;

	m_roomsNeighborCount[0] = 0;
	m_roomsNeighborCount[1] = m_totalRoomCount;
	m_roomsNeighborCount[2] = m_totalRoomCount;
	m_roomsNeighborCount[3] = m_totalRoomCount / 6;
	m_roomsNeighborCount[4] = m_totalRoomCount / 6;

	m_rooms.push_back(new Room(this, posX, posY, sf::Color::Red));
	roomCreated++;

	while (roomCreated < m_totalRoomCount)
	{
		selectedRoom = rand() % roomCreated;
		isSelectedRoomOk = false;
		newRoomNeighbors[0] = nullptr;
		newRoomNeighbors[1] = nullptr;
		newRoomNeighbors[2] = nullptr;
		newRoomNeighbors[3] = nullptr;

		if (!m_rooms[selectedRoom]->CanCreateNeighborRoom())
			continue;
		
		nextRoomDirection = ChooseNextRoomDrection(m_rooms[selectedRoom]);
		
		if (nextRoomDirection == Room::EDirection::None)
			continue;

		if (!CheckFutureNeighborCount(m_rooms[selectedRoom], newRoomNeighbors, nextRoomDirection))
			continue;

		PopulateRoom(m_rooms[selectedRoom], newRoomNeighbors, nextRoomDirection);
		roomCreated++;
	}
}

Room::EDirection Floor::ChooseNextRoomDrection(Room* _room)
{
	Room::EDirection nextRoomDirection;
	unsigned short retryCount = 0;

	do
	{
		nextRoomDirection = (Room::EDirection)(rand() % Room::EDirection::Count);

		retryCount++;
	} while (!_room->CanCreateNeighborRoom(nextRoomDirection) && retryCount < 10);

	if (retryCount == 10)
		return Room::EDirection::None;

	return nextRoomDirection;
}

bool Floor::CheckFutureNeighborCount(Room* _sourceRoom, Room** _nextRoomNeighbors , Room::EDirection _nextRoomDirection)
{
	short newRoomsNeighborCount[5];
	size_t roomCount;
	size_t i;
	unsigned short posX = _sourceRoom->GetX();
	unsigned short posY = _sourceRoom->GetY();
	unsigned short neighborPosX;
	unsigned short neighborPosY;
	unsigned short neighborCount;
	unsigned short nextRoomNeighborCount = 0;

	posX = ((_nextRoomDirection & 1) == 1) ? posX + ((_nextRoomDirection - 2) * -1) : posX;
	posY = ((_nextRoomDirection & 1) == 0) ? posY + (_nextRoomDirection - 1) : posY;

	for (i = 0; i < 5; i++)
		newRoomsNeighborCount[i] = m_roomsNeighborCount[i];

	roomCount = m_rooms.size();
	for (i = 0; i < roomCount; i++)
	{
		neighborPosX = m_rooms[i]->GetX();
		neighborPosY = m_rooms[i]->GetY();
		neighborCount = m_rooms[i]->GetNeighborCount();

		if (neighborPosX == posX && neighborPosY == posY - 1) // Up
		{
			newRoomsNeighborCount[neighborCount]++;
			newRoomsNeighborCount[neighborCount + 1]--;
			nextRoomNeighborCount++;

			_nextRoomNeighbors[Room::EDirection::Up] = m_rooms[i];
		}
		else if (neighborPosX == posX + 1 && neighborPosY == posY) // Rigth
		{
			newRoomsNeighborCount[neighborCount]++;
			newRoomsNeighborCount[neighborCount + 1]--;
			nextRoomNeighborCount++;

			_nextRoomNeighbors[Room::EDirection::Rigth] = m_rooms[i];
		}
		else if (neighborPosX == posX && neighborPosY == posY + 1) // Down
		{
			newRoomsNeighborCount[neighborCount]++;
			newRoomsNeighborCount[neighborCount + 1]--;
			nextRoomNeighborCount++;

			_nextRoomNeighbors[Room::EDirection::Down] = m_rooms[i];
		}
		else if (neighborPosX == posX - 1 && neighborPosY == posY) // Left
		{
			newRoomsNeighborCount[neighborCount]++;
			newRoomsNeighborCount[neighborCount + 1]--;
			nextRoomNeighborCount++;

			_nextRoomNeighbors[Room::EDirection::Left] = m_rooms[i];
		}
	}

	newRoomsNeighborCount[nextRoomNeighborCount]--;

	for (i = 0; i < 5; i++)
	{
		if (newRoomsNeighborCount[i] < 0)
			return false;
	}

	for (i = 0; i < 5; i++)
	{
		m_roomsNeighborCount[i] = newRoomsNeighborCount[i];
	}

	return true;
}

void Floor::PopulateRoom(Room* _sourceRoom, Room** _nextRoomNeighbors, Room::EDirection _nextRoomDirection)
{
	//size_t roomCount;
	Room* newRoom = nullptr;
	//Room* neighborRoom = nullptr;
	unsigned short posX = _sourceRoom->GetX();
	unsigned short posY = _sourceRoom->GetY();
	//unsigned short neighborPosX;
	//unsigned short neighborPosY;

	posX = ((_nextRoomDirection & 1) == 1) ? posX + ((_nextRoomDirection - 2) * -1) : posX;
	posY = ((_nextRoomDirection & 1) == 0) ? posY + (_nextRoomDirection - 1) : posY;

	newRoom = new Room(this, posX, posY);
	m_rooms.push_back(newRoom);

	for (short i = 0; i < 4; i++)
	{
		if (_nextRoomNeighbors[i] != nullptr)
		{
			newRoom->SetNeighborRoom((Room::EDirection)i);
			_nextRoomNeighbors[i]->SetNeighborRoom((Room::EDirection)((i + 2) % 4));
		}
	}

	/*newRoom = new Room(this, posX, posY);
	m_rooms.push_back(newRoom);
	m_roomsNeighborCount[_neighborCount]++;
	m_roomsNeighborCount[_neighborCount + 1]--;

	roomCount = m_rooms.size();
	for (size_t i = 0; i < roomCount; i++)
	{
		neighborRoom = m_rooms[i];
		neighborPosX = neighborRoom->GetX();
		neighborPosY = neighborRoom->GetY();

		if (neighborPosX == posX && neighborPosY == posY - 1)
		{
			newRoom->SetNeighborRoom(Room::EDirection::Up);
			neighborRoom->SetNeighborRoom(Room::EDirection::Down);
		}
		else if (neighborPosX == posX + 1 && neighborPosY == posY)
		{
			newRoom->SetNeighborRoom(Room::EDirection::Rigth);
			neighborRoom->SetNeighborRoom(Room::EDirection::Left);
		}
		else if (neighborPosX == posX && neighborPosY == posY + 1)
		{
			newRoom->SetNeighborRoom(Room::EDirection::Down);
			neighborRoom->SetNeighborRoom(Room::EDirection::Up);
		}
		else if (neighborPosX == posX - 1 && neighborPosY == posY)
		{
			newRoom->SetNeighborRoom(Room::EDirection::Left);
			neighborRoom->SetNeighborRoom(Room::EDirection::Rigth);
		}
	}*/
}