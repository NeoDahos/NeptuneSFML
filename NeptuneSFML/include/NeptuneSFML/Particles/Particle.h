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
		virtual ~Particle() {}

		virtual void Init(const sf::Vector2f& _position, const sf::Vector2f& _initialForce = sf::Vector2f(), float _mass = 1.f);
		virtual void Update(float _deltaTime);
		virtual void Draw(sf::RenderTarget& _target) = 0;

		void AddForce(sf::Vector2f _force);

		bool IsDead() const;
		void Kill();
		virtual void SetPosition(const sf::Vector2f& _newPosition);
		virtual sf::Vector2f GetPosition() const;
		sf::Vector2f GetVelocity() const;
		virtual void SetColor(const sf::Color& _color);
		float GetLifeTime() const;

	protected:
		sf::Vector2f m_acceleration;
		sf::Vector2f m_velocity;
		sf::Vector2f m_position;
		sf::Color m_color;
		float m_mass;
		float m_lifeTime;
		bool m_isAlive;
	};
}

#endif // PARTICLE_H