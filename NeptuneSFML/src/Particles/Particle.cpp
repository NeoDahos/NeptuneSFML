#include <NeptuneSFML\Particles\Particle.h>

namespace nep
{
	void Particle::Init(const sf::Vector2f & _position, const sf::Vector2f& _initialForce, float _mass)
	{
		m_position = _position;

		m_mass = _mass;
		if (_mass <= 0.f)
			m_mass = 1.f;

		m_velocity = _initialForce;
		m_isAlive = true;
	}

	void Particle::Update(float _deltaTime)
	{
		m_lifeTime += _deltaTime;

		m_velocity += (m_acceleration * _deltaTime);
		m_position += (m_velocity * _deltaTime);

		m_acceleration = { 0.f, 0.f };
	}

	void Particle::AddForce(sf::Vector2f _force)
	{
		m_acceleration += (_force / m_mass);
	}

	bool Particle::IsDead() const
	{
		return !m_isAlive;
	}

	sf::Vector2f Particle::GetPosition() const
	{
		return m_position;
	}

	sf::Vector2f Particle::GetVelocity() const
	{
		return m_velocity;
	}
}