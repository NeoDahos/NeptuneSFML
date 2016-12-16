#include <NeptuneSFML\Particles\CircleShapeParticle.h>

namespace nep
{
	void CircleShapeParticle::Init(const sf::Vector2f& _position, sf::Vector2f _initialForce, float _mass, const sf::String& _texturename)
	{
		Particle::Init(_position, _initialForce, _mass);
		setPosition(_position);
		setFillColor(sf::Color::White);
		setRadius(3.f);
		setPointCount(4);
	}

	void nep::CircleShapeParticle::Update(float _deltaTime)
	{
		Particle::Update(_deltaTime);
		setPosition(Particle::m_position);
	}

	void nep::CircleShapeParticle::Draw(sf::RenderTarget& _target)
	{
		_target.draw(*this);
	}
}