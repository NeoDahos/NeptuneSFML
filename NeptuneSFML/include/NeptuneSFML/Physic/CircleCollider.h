#ifndef CIRCLE_COLLIDER_H
#define CIRCLE_COLLIDER_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\Physic\Collider.h>

#include <SFML\System\Vector2.hpp>

namespace nep
{
	class NEPTUNE_API CircleCollider : public Collider
	{
	public:
		CircleCollider(Object* const _parent, float _radius, bool _isTrigger = false);
		CircleCollider(const CircleCollider& _other);
		virtual ~CircleCollider();

		virtual Component * Clone() const;

		virtual void SetPosition(const sf::Vector2f& _position);
		virtual sf::Vector2f GetPosition() const;

		virtual bool TestCollision(CircleCollider* const _other);

	private:
		sf::Vector2f m_centerPosition; // Relative to parent transform
		float m_radius;
	};
}

#endif // CIRCLE_COLLIDER_H