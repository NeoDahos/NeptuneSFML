#include <NeptuneSFML\Particles\VertexParticle.h>
#include <NeptuneSFML\Tools.h>

namespace nep
{
	void VertexParticle::Init(const sf::Vector2f& _position, const sf::Vector2f& _initialForce, float _mass)
	{
		Particle::Init(_position, _initialForce, _mass);
		position = m_position;
		color = sf::Color::White;
	}

	void VertexParticle::Update(float _deltaTime)
	{
		Particle::Update(_deltaTime);
		position = m_position;
	}

	void VertexParticle::Draw(sf::RenderTarget& _target)
	{
		_target.draw(this, 1, sf::PrimitiveType::Points);
	}

	void VertexParticle::SetPosition(const sf::Vector2f& _newPosition)
	{
		m_position = _newPosition;
		position = m_position;
	}

	void VertexParticle::SetColor(const sf::Color& _color)
	{
		color = _color;
	}
}