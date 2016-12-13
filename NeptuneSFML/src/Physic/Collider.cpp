#include <NeptuneSFML\Physic\Collider.h>
#include <NeptuneSFML\EngineCore.h>
#include <NeptuneSFML\Physic\CollisionInfos.h>
#include <NeptuneSFML\Object\Object.h>

namespace nep
{
	unsigned short Collider::s_instanceCount = 0;

	Collider::Collider(Object* const _parent, bool _isTrigger) : Component(_parent)
	{
		m_isTrigger = _isTrigger;
		m_isUnique = false;
		m_id = s_instanceCount;
		s_instanceCount++;

		ColliderMng.AddCollider(this);
	}

	Collider::Collider(const Collider& _other) : Collider(_other.m_parent, _other.m_isTrigger)
	{
		m_componentType = _other.m_componentType;
		m_colliderType = _other.m_colliderType;
	}

	unsigned short Collider::GetId() const
	{
		return m_id;
	}

	Collider::ColliderType Collider::GetColliderType() const
	{
		return m_colliderType;
	}

	auto Collider::FindCollider(unsigned short _id)
	{
		auto it = m_collidingIds.begin();
		auto itEnd = m_collidingIds.end();

		while (it != itEnd && (*it) != _id)
			++it;

		return it;
	}

	void Collider::ManageCollision(Collider* const _other, bool _isColliding)
	{
		CollisionInfos collisionInfos;
		collisionInfos.m_collider = _other;
		collisionInfos.m_object = _other->m_parent;

		CollisionInfos collisionInfosOther;
		collisionInfosOther.m_collider = this;
		collisionInfosOther.m_object = m_parent;

		auto itSearch = FindCollider(_other->m_id);
		auto itEnd = m_collidingIds.end();

		if (_isColliding)
		{
			if (itSearch == itEnd)
			{
				m_collidingIds.push_back(_other->m_id);
				_other->m_collidingIds.push_back(m_id);

				if (m_isTrigger)
				{
					m_parent->OnTriggerEnter(collisionInfos);
					_other->m_parent->OnTriggerEnter(collisionInfosOther);
				}
				else
				{
					m_parent->OnCollisionEnter(collisionInfos);
					_other->m_parent->OnCollisionEnter(collisionInfosOther);
				}
			}
			else
			{
				if (m_isTrigger)
				{
					m_parent->OnTriggerStay(collisionInfos);
					_other->m_parent->OnTriggerStay(collisionInfosOther);
				}
				else
				{
					m_parent->OnCollisionStay(collisionInfos);
					_other->m_parent->OnCollisionStay(collisionInfosOther);
				}
			}
		}
		else if (!_isColliding && itSearch != itEnd)
		{
			m_collidingIds.erase(itSearch);
			_other->m_collidingIds.erase(_other->FindCollider(m_id));

			if (m_isTrigger)
			{
				m_parent->OnTriggerExit(collisionInfos);
				_other->m_parent->OnTriggerExit(collisionInfosOther);
			}
			else
			{
				m_parent->OnCollisionExit(collisionInfos);
				_other->m_parent->OnCollisionExit(collisionInfosOther);
			}
		}
	}
}