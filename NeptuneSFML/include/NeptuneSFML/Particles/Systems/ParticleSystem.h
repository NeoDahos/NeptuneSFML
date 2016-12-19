#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\Particles\Effectors\ParticleEffector.h>
#include <NeptuneSFML\Particles\Emitter.h>

#include <SFML\Graphics\RenderTarget.hpp>

#include <vector>

namespace nep
{
	class NEPTUNE_API ParticleSystem
	{
	public:
		virtual ~ParticleSystem() {};

		virtual void Update(float _deltaTime) = 0;
		virtual void Draw(sf::RenderTarget& _target) = 0;

		virtual void AddParticle(const sf::Vector2f & _position = sf::Vector2f(), const sf::Vector2f & _initialForce = sf::Vector2f(), float _mass = 1.f, const sf::Color & _color = sf::Color::White) = 0;
		
		inline void AddEmitter(Emitter * const _emitter)
		{
			m_emitters.push_back(_emitter);
		}
		
		inline void AddEffector(ParticleEffector * const _effector)
		{
			m_effectors.push_back(_effector);
		}

		virtual void AddForce(sf::Vector2f _force) = 0;

		inline sf::Vector2f GetPosition() const
		{
			return m_position;
		}

		virtual size_t GetParticleCount() const = 0;

	protected:
		std::vector<Emitter *> m_emitters;
		std::vector<ParticleEffector *> m_effectors;
		sf::Vector2f m_position;
	};
}

#endif // PARTICLE_SYSTEM_H