#include <NeptuneSFML\Particles\Effectors\Repeller.h>
#include <NeptuneSFML\Tools.h>

namespace nep
{
	void Repeller::Init(const sf::Vector2f& _position, float _strength)
	{
		m_strength = _strength;
		if(m_strength > 0.f)
			setFillColor(sf::Color::Red);
		else
			setFillColor(sf::Color::Green);

		setPosition(_position);
		setRadius(fabs(m_strength) / 10000.f);
		setOrigin(getRadius(), getRadius());
	}

	sf::Vector2f Repeller::ComputeRepelForce(const Particle* _particle)
	{
		sf::Vector2f direction = getPosition() - _particle->GetPosition();
		float magnitudeSq = VectorLengthSq(direction);
		float force;

		VectorNormalize(direction);
		magnitudeSq = Clamp(magnitudeSq, 100.f, 10000.f);

		force = -1.f * m_strength / magnitudeSq;
		direction *= force;
		return direction;
	}
}