#ifndef COMPONENT_H
#define COMPONENT_H
#include <NeptuneSFML\Export.h>

#include <SFML\System\String.hpp>

namespace nep
{
	class Object;

	class NEPTUNE_API Component abstract
	{
	public:
		enum ComponentCategory { PhysicComponent, MiscComponent };

		Component(Object* const _parent = nullptr);
		Component(const Component & _other);
		virtual ~Component();

		virtual Component * Clone() const = 0;

		ComponentCategory GetComponentCategory();
		void SetParent(Object* const _parent);

	protected:
		Object* m_parent;
		ComponentCategory m_componentCategory;
		bool m_isUnique;
	};
}

#endif // COMPONENT_H