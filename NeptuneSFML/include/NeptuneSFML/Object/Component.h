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
		enum ComponentType { TransformComponent, CircleColliderComponent };

		Component(Object* const _parent);
		virtual ~Component();

		ComponentType GetComponentType();
		void SetParent(Object* const _parent);

	protected:
		Object* m_parent;
		ComponentType m_componentType;
		bool m_isUnique;
	};
}

#endif // COMPONENT_H