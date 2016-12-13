#pragma once

#include "ShotInterface.h"

class SimpleShot : public ShotInterface
{
public:
	SimpleShot(BulletsManager* _bulletsManager);
	virtual ~SimpleShot();

	virtual void Fire(const sf::Vector2f& _position, const sf::Vector2f& _direction, float _speed, const sf::Color& _color = sf::Color::White);
};