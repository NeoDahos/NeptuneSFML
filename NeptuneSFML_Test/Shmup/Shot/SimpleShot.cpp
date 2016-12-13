#include "SimpleShot.h"

SimpleShot::SimpleShot(BulletsManager* _bulletsManager) : ShotInterface(_bulletsManager)
{
}

SimpleShot::~SimpleShot()
{
}

void SimpleShot::Fire(const sf::Vector2f& _position, const sf::Vector2f& _direction, float _speed, const sf::Color& _color)
{
	m_bulletsManager->AddBullet(_position, _direction, _speed, _color);
}