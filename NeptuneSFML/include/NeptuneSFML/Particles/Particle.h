#ifndef PARTICLE_H
#define PARTICLE_H
#include <NeptuneSFML\Export.h>

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\System\String.hpp>

namespace nep
{
	class NEPTUNE_API Particle
	{
	public:
		Particle() {}

		virtual void Init(const sf::Vector2f& _position, const sf::Vector2f& _initialForce = sf::Vector2f(), float _mass = 1.f, const sf::String& _texturename = "");
		virtual void Update(float _deltaTime);
		virtual void Draw(sf::RenderTarget& _target) = 0;

		void AddForce(sf::Vector2f _force);

		bool IsDead() const;
		virtual sf::Vector2f GetPosition() const;

	protected:
		sf::Vector2f m_acceleration;
		sf::Vector2f m_velocity;
		sf::Vector2f m_position;
		float m_mass;
	};
}

#endif // PARTICLE_H