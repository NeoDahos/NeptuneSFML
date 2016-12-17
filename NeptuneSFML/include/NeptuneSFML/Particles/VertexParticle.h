#ifndef VERTEX_PARTICLE_H
#define VERTEX_PARTICLE_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\Particles\Particle.h>
#include <SFML\Graphics\Vertex.hpp>

namespace nep
{
	class NEPTUNE_API VertexParticle : public nep::Particle, public sf::Vertex
	{
	public:
		virtual ~VertexParticle() {}

		virtual void Init(const sf::Vector2f& _position, const sf::Vector2f& _initialForce = sf::Vector2f(), float _mass = 1.f);
		virtual void Update(float _deltaTime);
		virtual void Draw(sf::RenderTarget& _target);

		virtual void SetPosition(const sf::Vector2f& _newPosition);
		virtual void SetColor(const sf::Color& _color);
	};
}

#endif //VERTEX_PARTICLE_H