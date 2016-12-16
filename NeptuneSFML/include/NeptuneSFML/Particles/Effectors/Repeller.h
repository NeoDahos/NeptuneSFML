#ifndef REPELLER_H
#define REPELLER_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\Particles\Effectors\ParticleEffector.h>
#include <SFML\Graphics\CircleShape.hpp>

namespace nep
{
	class NEPTUNE_API Repeller :  public nep::ParticleEffector, public sf::CircleShape
	{
	public:
		void Init(const sf::Vector2f& _position, float _strength);

		virtual void Apply(class Particle * const _particle);

		void SetStrength(float _newStrength);

	private:
		float m_strength;
	};
}

#endif // REPELLER_H