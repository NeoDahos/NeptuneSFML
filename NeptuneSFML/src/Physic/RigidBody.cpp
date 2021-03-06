#include <NeptuneSFML\Physic\RigidBody.h>
#include <NeptuneSFML\Tools.h>

namespace nep
{
	RigidBody::RigidBody(Object* const _parent, float _mass) : Component(_parent), m_mass(_mass), m_isAffectedByGravity(true)
	{
	}

	RigidBody::RigidBody(const RigidBody & _other) : Component(_other)
	{
		m_force = sf::Vector2f();
		m_mass = _other.m_mass;
		m_isAffectedByGravity = _other.m_isAffectedByGravity;
	}

	Component * RigidBody::Clone() const
	{
		return new RigidBody(*this);
	}

	sf::Vector2f RigidBody::GetForce() const
	{
		return m_force;
	}

	void RigidBody::SetIsAffectedByGravity(bool _isAffected)
	{
		m_isAffectedByGravity = _isAffected;
	}

	void RigidBody::AddForce(const sf::Vector2f& _force, float _deltaTime)
	{
		m_force += (m_mass * (_force / (_deltaTime * _deltaTime)));
	}

	void RigidBody::AddImpulse(const sf::Vector2f& _impulse, float _deltaTime)
	{
		m_force += (m_mass * (_impulse / _deltaTime));
	}

	void RigidBody::Update(float _deltaTime)
	{
		if(m_isAffectedByGravity)
			AddForce(sf::Vector2f(0.f, 10.f), _deltaTime);
	}
}