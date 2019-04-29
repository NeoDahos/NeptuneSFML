#ifndef TRASNFORM_H
#define TRANSFORM_H

#include <NeptuneSFML\Object\Component.h>

#include <SFML\Graphics\Transformable.hpp>

namespace nep
{
	class Transform : public Component, public sf::Transformable
	{
	public:
		Transform(Object* const _parent);
		Transform(const Transform& _other);
		~Transform();

		virtual Component * Clone() const;
	};
}

#endif // TRANSFORM_H