#ifndef REPELLER_H
#define REPELLER_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\Particles\Effectors\ParticleEffector.h>

#include <SFML\System\Vector2.hpp>

namespace nep
{
	class NEPTUNE_API Repeller :  public nep::ParticleEffector
	{
	public:
		void Init(const sf::Vector2f& _position, float _strength, bool _activated = true);

		virtual void Apply(class Particle * const _particle);

		void SetPosition(const sf::Vector2f & _position);
		void SetPosition(float _x, float _y);
		void SetStrength(float _newStrength);

	private:
		sf::Vector2f m_position;
		float m_strength;
	};
}

#endif // REPELLER_H