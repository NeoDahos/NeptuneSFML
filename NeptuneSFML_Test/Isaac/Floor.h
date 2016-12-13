#ifndef FLOOR_H
#define FLOOR_H

#include <vector>
#include "Room.h"

class Floor
{
public:
	Floor(unsigned short _width = 11, unsigned short _heigth = 11, unsigned short _roomCount = 10);
	~Floor();

	unsigned short GetWidth() const;
	unsigned short GetHeight() const;

	void Regenerate(unsigned short _width = 11, unsigned short _heigth = 11, unsigned short _roomCount = 10);
	void Draw(sf::RenderTarget& _renderTarget);
	
private:
	void Generate();
	Room::EDirection ChooseNextRoomDrection(Room* _room);
	bool CheckFutureNeighborCount(Room* _sourceRoom, Room** _nextRoomNeighbors, Room::EDirection _nextRoomDirection); // Return false if next neighbor count is not ok
	void PopulateRoom(Room* _sourceRoom, Room** _nextRoomNeighbors, Room::EDirection _nextRoomDirection);


	std::vector<Room*> m_rooms;

	short m_roomsNeighborCount[5]; // Index indicate the number of neighbor, the value indicate the remaining number of room with this number of neighbor
	unsigned short m_totalRoomCount;
	unsigned short m_width;
	unsigned short m_heigth;
};

#endif // FLOOR_H