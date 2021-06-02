#pragma once
#include <SFML\System.hpp>
#include <SFML\Config.hpp>

struct NoitaParticle
{
public:
	enum class Type : sf::Uint8 { Empty, Sand, Water, Wood };

	static constexpr sf::Uint8 SandSwapAllowed = 1 << (sf::Uint8)Type::Empty | 1 << (sf::Uint8)Type::Water;
	static constexpr sf::Uint8 WaterSwapAllowed = 1 << (sf::Uint8)Type::Empty;

	static const NoitaParticle EmptyParticle;
	static const NoitaParticle SandParticle;
	static const NoitaParticle WaterParticle;
	static const NoitaParticle WoodParticle;

	sf::Vector2f m_velocity;
	sf::Uint16 m_lastUpdateStep{};
	Type m_type{};
	bool m_isStatic{};
};