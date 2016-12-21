#include "..\..\..\include\NeptuneSFML\Particles\Effectors\Repeller.h"
#include "..\..\..\include\NeptuneSFML\Particles\Effectors\Repeller.h"
#include <NeptuneSFML\Particles\Effectors\Repeller.h>
#include <NeptuneSFML\Particles\Particle.h>
#include <NeptuneSFML\Tools.h>

namespace nep
{
	void Repeller::Init(const sf::Vector2f& _position, float _strength, bool _activated)
	{
		m_isActive = _activated;
		m_position = _position;
		m_strength = _strength;
	}

	void Repeller::Apply(class Particle * const _particle)
	{
		if (m_isActive)
		{
			sf::Vector2f direction = m_position - _particle->GetPosition();
			_particle->AddForce(VectorNormalization(direction) * (-m_strength));

			/*sf::Vector2f direction = m_position - _particle->GetPosition();
			float magnitudeSq = VectorLengthSq(direction);
			float force;

			VectorNormalize(direction);
			magnitudeSq = Clamp(magnitudeSq, 500.f, 10000.f);

			force = -m_strength / magnitudeSq;
			direction *= force;

			return VectorNormalization(direction) * -m_strength;*/
		}
	}

	void Repeller::SetPosition(const sf::Vector2f & _position)
	{
		m_position = _position;
	}

	void Repeller::SetPosition(float _x, float _y)
	{
		m_position.x = _x;
		m_position.y = _y;
	}

	void Repeller::SetStrength(float _newStrength)
	{
		m_strength = _newStrength;
	}
}