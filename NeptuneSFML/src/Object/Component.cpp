#include <NeptuneSFML\Object\Component.h>
#include <NeptuneSFML\Object\Object.h>

namespace nep
{
	Component::Component(Object* const _parent)
	{
		m_parent = _parent;
	}

	Component::~Component()
	{
	}
	
	Component::ComponentType Component::GetComponentType()
	{
		return m_componentType;
	}

	void Component::SetParent(Object* const _parent)
	{
		m_parent = _parent;
	}
}