#ifndef CIRCLE_SHAPE_PARTICLE_H
#define CIRCLE_SHAPE_PARTICLE_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\Particles\Particle.h>
#include <SFML\Graphics\CircleShape.hpp>

namespace nep
{
	class NEPTUNE_API CircleShapeParticle : public nep::Particle, public sf::CircleShape
	{
	public:
		virtual void Init(const sf::Vector2f& _position, sf::Vector2f _initialForce = sf::Vector2f(), float _mass = 1.f, const sf::String& _texturename = "");
		virtual void Update(float _deltaTime);
		virtual void Draw(sf::RenderTarget& _target);
	};
}

#endif //CIRCLE_SHAPE_PARTICLE_H