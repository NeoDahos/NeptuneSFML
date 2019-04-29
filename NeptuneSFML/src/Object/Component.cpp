#include <NeptuneSFML\Object\Component.h>
#include <NeptuneSFML\Object\Object.h>

namespace nep
{
	Component::Component(Object* const _parent)
	{
		m_parent = _parent;
	}

	Component::Component(const Component & _other)
	{
		m_parent = nullptr;
		m_isUnique = _other.m_isUnique;
		m_componentCategory = _other.m_componentCategory;
	}

	Component::~Component()
	{
	}
	
	Component::ComponentCategory Component::GetComponentCategory()
	{
		return m_componentCategory;
	}

	void Component::SetParent(Object* const _parent)
	{
		m_parent = _parent;
	}
}