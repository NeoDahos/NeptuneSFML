#ifndef RIGID_BODY_H
#define RIGID_BODY_H
#include <NeptuneSFML\Export.h>

#include <SFML\System\Vector2.hpp>

namespace nep
{
	class NEPTUNE_API RigidBody
	{
	public:
		RigidBody(float _mass);

		sf::Vector2f GetForce() const;
		void SetIsAffectedByGravity(bool _isAffected);

		void AddForce(const sf::Vector2f& _force, float _deltaTime);
		void AddImpulse(const sf::Vector2f& _impulse, float _deltaTime);

		void Update(float _deltaTime);

	private:
		sf::Vector2f m_force;
		float m_mass = 1.0f;
		bool m_isAffectedByGravity = true;
	};
}

#endif // RIGID_BODY_H