#ifndef VERTEX_PARTICLE_SYSTEM_H
#define VERTEX_PARTICLE_SYSTEM_H
#include <NeptuneSFML\Export.h>
#include <NeptuneSFML\Particles\Effectors\Repeller.h>

#include <SFML\Graphics\RenderTarget.hpp>

#include <vector>

namespace nep
{
	class VertexParticle;

	class NEPTUNE_API VertexParticleSystem
	{
	public:
		VertexParticleSystem() {}
		~VertexParticleSystem();

		void Init(const sf::Vector2f& _position, const sf::Vector2f& _windowSize);
		void Update(float _deltaTime);
		void Draw(sf::RenderTarget& _target);

		void AddParticle(sf::Vector2f _initialForce = sf::Vector2f(), float _mass = 1.f, const sf::Color& _color = sf::Color::White);

		void AddForce(sf::Vector2f _force);
		void AddRepellerForce(Repeller& _repeller);

		size_t GetParticleCount() const;

	private:
		std::vector<VertexParticle*> m_particles;
		sf::Vector2f m_position;
		sf::Vector2f m_windowSize;
	};
}

#endif // VERTEX_PARTICLE_SYSTEM_H