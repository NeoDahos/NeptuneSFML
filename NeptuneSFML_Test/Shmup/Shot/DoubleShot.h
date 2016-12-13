#pragma once

#include "ShotInterface.h"

class DoubleShot : public ShotInterface
{
public:
	DoubleShot(BulletsManager* _bulletsManager, float _offset);
	virtual ~DoubleShot();

	virtual void Fire(const sf::Vector2f& _position, const sf::Vector2f& _direction, float _speed, const sf::Color& _color = sf::Color::White);

private:
	float m_offset;
};