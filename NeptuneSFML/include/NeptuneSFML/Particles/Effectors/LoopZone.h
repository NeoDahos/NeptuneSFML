#ifndef LOOP_ZONE_H
#define LOOP_ZONE_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\Particles\Effectors\ParticleEffector.h>

#include <SFML\Graphics\Rect.hpp>

namespace nep
{
	class NEPTUNE_API LoopZone : public nep::ParticleEffector
	{
	public:
		void Init(const sf::FloatRect& _zone, bool _activated = true);

		virtual void Apply(class Particle * const _particle);

	private:
		sf::FloatRect m_zone;
	};
}

#endif // LOOP_ZONE_H