#include <NeptuneSFML\Particles\Effectors\Repeller.h>
#include <NeptuneSFML\Particles\Particle.h>
#include <NeptuneSFML\Tools.h>

namespace nep
{
	void Repeller::Init(const sf::Vector2f& _position, float _strength, bool _activated)
	{
		m_isActive = _activated;
		m_strength = _strength;
		if(m_strength > 0.f)
			setFillColor(sf::Color::Red);
		else
			setFillColor(sf::Color::Green);

		setPosition(_position);
		setRadius(fabs(m_strength) / 10000.f);
		setOrigin(getRadius(), getRadius());
	}

	void Repeller::Apply(class Particle * const _particle)
	{
		if (m_isActive)
		{
			sf::Vector2f direction = getPosition() - _particle->GetPosition();
			_particle->AddForce(VectorNormalization(direction) * (-m_strength));

			/*sf::Vector2f direction = getPosition() - _particle->GetPosition();
			float magnitudeSq = VectorLengthSq(direction);
			float force;

			VectorNormalize(direction);
			magnitudeSq = Clamp(magnitudeSq, 500.f, 10000.f);

			force = -m_strength / magnitudeSq;
			direction *= force;

			return VectorNormalization(direction) * -m_strength;*/
		}
	}

	void Repeller::SetStrength(float _newStrength)
	{
		m_strength = _newStrength;
	}
}