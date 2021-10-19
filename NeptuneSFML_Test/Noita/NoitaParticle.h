#pragma once
#include <SFML\System.hpp>
#include <SFML\Config.hpp>

#include "Elements/NoitaElementCommon.h"

struct NoitaParticle
{
public:
	static constexpr sf::Uint8 SandSwapAllowed = 1 << (sf::Uint8)NoitaElementCommon::Type::Empty | 1 << (sf::Uint8)NoitaElementCommon::Type::Water;
	static constexpr sf::Uint8 WaterSwapAllowed = 1 << (sf::Uint8)NoitaElementCommon::Type::Empty;

	static const NoitaParticle EmptyParticle;
	static const NoitaParticle SandParticle;
	static const NoitaParticle WaterParticle;
	static const NoitaParticle WoodParticle;

	sf::Vector2f m_velocity;
	sf::Uint16 m_lastUpdateStep{};
	NoitaElementCommon::Type m_type{};
	bool m_isStatic{};
};