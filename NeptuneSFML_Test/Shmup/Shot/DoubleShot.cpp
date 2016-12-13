#include "DoubleShot.h"

#include <NeptuneSFML\Tools.h>

DoubleShot::DoubleShot(BulletsManager* _bulletsManager, float _offset) : ShotInterface(_bulletsManager)
{
	m_offset = _offset;
}

DoubleShot::~DoubleShot()
{
}

void DoubleShot::Fire(const sf::Vector2f& _position, const sf::Vector2f& _direction, float _speed, const sf::Color& _color)
{
	sf::Vector2f bulletOffset = _direction * 10.f;
	nep::VectorRotation<float>(bulletOffset, DegToRad(90));

	m_bulletsManager->AddBullet(_position + bulletOffset, _direction, _speed, _color);
	m_bulletsManager->AddBullet(_position - bulletOffset, _direction, _speed, _color);
}