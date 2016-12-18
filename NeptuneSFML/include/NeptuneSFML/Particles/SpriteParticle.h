#ifndef SPRITE_PARTICLE_H
#define SPRITE_PARTICLE_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\Particles\Particle.h>
#include <SFML\Graphics\Sprite.hpp>

namespace nep
{
	class NEPTUNE_API SpriteParticle : public sf::Sprite, public nep::Particle
	{
	public:
		virtual ~SpriteParticle() {}

		virtual void Init(const sf::Vector2f& _position, sf::Vector2f _initialForce = sf::Vector2f(), float _mass = 1.f, const sf::String& _texturename = "");
		virtual void Update(float _deltaTime);
		virtual void Draw(sf::RenderTarget& _target);

		virtual void SetPosition(const sf::Vector2f& _newPosition);
		virtual void SetColor(const sf::Color& _color);
	};
}

#endif //SPRITE_PARTICLE_H