#ifndef ROOM_H
#define ROOM_H

#include <SFML\Graphics\RectangleShape.hpp>

class Floor;

class Room
{
public:
	enum EDirection { None = -1, Up = 0, Rigth, Down, Left, Count };

	Room(Floor* _floor, unsigned short _posX, unsigned short _posY, const sf::Color& _color = sf::Color::White);
	void SetNeighborRoom(EDirection _direction);
	bool CanCreateNeighborRoom();
	bool CanCreateNeighborRoom(EDirection _direction);
	
	unsigned short GetX() const;
	unsigned short GetY() const;
	unsigned short GetNeighborCount() const;

	void Draw(sf::RenderTarget& _renderTarget);

private:
	bool m_isNeighborRoomOccupied[EDirection::Count]; // Up, Rigth, Down, Left
	bool m_isBuildPossible[EDirection::Count]; // Up, Rigth, Down, Left

	sf::RectangleShape m_shape;
	Floor* m_floor;
	unsigned short m_posX;
	unsigned short m_posY;
	unsigned short m_neighborCount;
};

#endif // ROOM_H