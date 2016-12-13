#pragma once

#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Color.hpp>

#include "../BulletsManager.h"

class ShotInterface
{
public:
	enum ShotType
	{
		Shot_Unknown = -1,
		Shot_Simple,
		Shot_Double,
		Shot_Spriral,
		Shot_SpriralAroundCircle,
		Shot_CirlceAroundCircle,
		ShotType_Count
	};

	ShotInterface(BulletsManager* _bulletsManager)
	{
		m_bulletsManager = _bulletsManager;
	}

	virtual void Fire(const sf::Vector2f& _position, const sf::Vector2f& _direction, float _speed, const sf::Color& _color = sf::Color::White) = 0;

protected:
	BulletsManager* m_bulletsManager;
};

