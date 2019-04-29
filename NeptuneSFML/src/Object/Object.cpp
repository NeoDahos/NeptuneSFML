#include <NeptuneSFML\Object\Object.h>
#include <NeptuneSFML\Object\Transform.h>
#include <NeptuneSFML\Physic\CircleCollider.h>
#include <NeptuneSFML\Physic\CollisionInfos.h>

#include <SFML\Graphics\RenderTarget.hpp>

#include <typeinfo>

namespace nep
{
	unsigned short Object::s_instanceCount = 0;

	Object::Object()
	{
		m_transform = new Transform(this);
		m_components.push_back(m_transform);
		m_positionGizmo.color = sf::Color::Red;

		m_id = s_instanceCount;
		s_instanceCount++;
	}

	Object::Object(const Object& _other)
	{
		m_id = s_instanceCount;
		s_instanceCount++;
		m_tag = _other.m_tag;

		for (auto&& component : _other.m_components)
		{
			m_components.push_back(component->Clone());
		}

		m_transform = static_cast<Transform*>(m_components[0]);
	}

	Object::~Object()
	{
	}

	sf::Vector2f Object::GetPosition() const
	{
		return m_transform->getPosition();
	}

	unsigned short Object::GetId() const
	{
		return m_id;
	}

	void Object::Move(const sf::Vector2f& _moveVector)
	{
		m_transform->move(_moveVector);
	}

	void Object::Update(float _deltaTime)
	{
	}

	void Object::Draw(sf::RenderTarget& _target, const sf::RenderStates& _states)
	{
		_target.draw(&m_positionGizmo, 1, sf::PrimitiveType::Points, m_transform->getTransform());
	}

	void Object::OnCollisionEnter(const CollisionInfos& _collisionInfos)
	{
		printf("%d enter with %d\n", m_id, _collisionInfos.m_object->GetId());
	}

	void Object::OnCollisionStay(const CollisionInfos& _collisionInfos)
	{
		printf("%d stay with %d\n", m_id, _collisionInfos.m_object->GetId());
	}

	void Object::OnCollisionExit(const CollisionInfos& _collisionInfos)
	{
		printf("%d exit with %d\n", m_id, _collisionInfos.m_object->GetId());
	}

	void Object::OnTriggerEnter(const CollisionInfos& _collisionInfos)
	{
	}

	void Object::OnTriggerStay(const CollisionInfos& _collisionInfos)
	{
	}

	void Object::OnTriggerExit(const CollisionInfos& _collisionInfos)
	{
	}
}