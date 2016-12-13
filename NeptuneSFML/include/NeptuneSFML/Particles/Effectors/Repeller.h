#ifndef REPELLER_H
#define REPELLER_H
#include <NeptuneSFML\Export.h>

#include <SFML\Graphics\CircleShape.hpp>
#include <NeptuneSFML\Particles\Particle.h>

namespace nep
{
	class NEPTUNE_API Repeller : public sf::CircleShape
	{
	public:
		Repeller() {};
		void Init(const sf::Vector2f& _position, float _strength);

		sf::Vector2f ComputeRepelForce(const Particle* _particle);

	private:
		float m_strength;
	};
}

#endif // REPELLER_H