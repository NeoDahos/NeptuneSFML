#ifndef COLLIDER_H
#define COLLIDER_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\Object\Component.h>

#include <SFML\System\Vector2.hpp>

#include <list>

namespace nep
{
	class CircleCollider;
	class CollisionInfos;

	class NEPTUNE_API Collider : public Component
	{
	public:
		static unsigned short s_instanceCount;

		enum ColliderType { Circle };

		Collider(Object* const _parent, bool _isTrigger = false);
		Collider(const Collider& _other);

		virtual Component * Clone() const = 0;

		unsigned short GetId() const;
		ColliderType GetColliderType() const;
		virtual void SetPosition(const sf::Vector2f& _position) = 0;
		virtual sf::Vector2f GetPosition() const = 0;

		auto FindCollider(unsigned short _id);

		virtual bool TestCollision(CircleCollider* const _other) = 0;

	protected:
		void ManageCollision(Collider* const _other, bool _isColliding);
		
		std::list<unsigned short> m_collidingIds;
		ColliderType m_colliderType;
		unsigned short m_id;
		bool m_isTrigger;
	};
}

#endif // COLLIDER_H