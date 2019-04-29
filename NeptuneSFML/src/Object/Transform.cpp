#include <NeptuneSFML\Object\Transform.h>

namespace nep
{
	Transform::Transform(Object* const _parent) : Component(_parent)
	{
		m_isUnique = true;
		m_componentCategory = MiscComponent;
	}

	Transform::Transform(const Transform& _other) : Transformable(_other), Component(_other)
	{
	}

	Transform::~Transform()
	{
	}

	Component * Transform::Clone() const
	{
		return new nep::Transform(*this);
	}
}