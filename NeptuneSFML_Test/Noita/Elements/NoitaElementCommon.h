#pragma once

#include <SFML\Config.hpp>
#include <SFML\Graphics\Color.hpp>

namespace NoitaElementCommon
{
	enum class Type : sf::Uint8 { Empty, Sand, Water, Wood };
	const sf::Color ParticleColors[] = { sf::Color::Transparent, sf::Color(255, 200, 0), sf::Color(0, 100, 255), sf::Color(110, 50, 25) };

	struct ChunkElement
	{
		class NoitaParticleChunk* m_chunk;
		class NoitaElement* m_element;
		int m_index;
	};
}