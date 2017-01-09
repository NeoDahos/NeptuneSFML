#include <NeptuneSFML\Particles\Particle.h>

namespace nep
{
	void Particle::Init(const sf::Vector2f & _position, const sf::Vector2f& _initialForce, float _mass)
	{
		position = _position;
		color = sf::Color::White;

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
		position += (m_velocity * _deltaTime);

		m_acceleration = { 0.f, 0.f };
	}

	void Particle::Draw(sf::RenderTarget & _target)
	{
		_target.draw(this, 1, sf::PrimitiveType::Points);
	}

	void Particle::AddForce(sf::Vector2f _force)
	{
		m_acceleration += (_force / m_mass);
	}

	bool Particle::IsDead() const
	{
		return !m_isAlive;
	}

	void Particle::Kill()
	{
		m_isAlive = false;
	}

	void Particle::SetPosition(const sf::Vector2f & _newPosition)
	{
		position = _newPosition;
	}

	sf::Vector2f Particle::GetPosition() const
	{
		return position;
	}

	sf::Vector2f Particle::GetVelocity() const
	{
		return m_velocity;
	}

	void Particle::SetColor(const sf::Color & _color)
	{
		color = _color;
	}

	float Particle::GetLifeTime() const
	{
		return m_lifeTime;
	}
}