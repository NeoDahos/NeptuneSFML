#ifndef RIGID_BODY_H
#define RIGID_BODY_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\Object\Component.h>

#include <SFML\System\Vector2.hpp>

namespace nep
{
	class NEPTUNE_API RigidBody : public Component
	{
	public:
		RigidBody(Object* const _parent, float _mass = 1.0f);
		RigidBody(const RigidBody & _other);

		virtual Component * Clone() const;

		sf::Vector2f GetForce() const;
		void SetIsAffectedByGravity(bool _isAffected);

		void AddForce(const sf::Vector2f& _force, float _deltaTime);
		void AddImpulse(const sf::Vector2f& _impulse, float _deltaTime);

		void Update(float _deltaTime);

	private:
		sf::Vector2f m_force;
		float m_mass;
		bool m_isAffectedByGravity;
	};
}

#endif // RIGID_BODY_H