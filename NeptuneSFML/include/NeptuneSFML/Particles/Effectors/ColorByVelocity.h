#ifndef COLOR_BY_VELOCITY_H
#define COLOR_BY_VELOCITY_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\Particles\Effectors\ParticleEffector.h>

#include <SFML\Graphics\Color.hpp>

namespace nep
{
	class NEPTUNE_API ColorByVelocity : public nep::ParticleEffector
	{
	public:
		void Init(const sf::Color& _startColor, const sf::Color& _endColor, float _minVelocity, float _maxVelocity, bool _activated = true);

		virtual void Apply(class Particle * const _particle);

	private:
		sf::Color m_startColor;
		sf::Color m_endColor;
		float m_minVelocity;
		float m_maxVelocity;
	};
}

#endif // COLOR_BY_VELOCITY_H