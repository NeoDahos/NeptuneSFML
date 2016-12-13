#ifndef COLLISION_INFOS_H
#define COLLISION_INFOS_H
#include <NeptuneSFML\Export.h>

#include <SFML\System\NonCopyable.hpp>

namespace nep
{
	class Collider;
	class Object;

	class NEPTUNE_API CollisionInfos : public sf::NonCopyable
	{
	public:
		Collider* m_collider;
		Object* m_object;
	};
}

#include <NeptuneSFML\Physic\Collider.h>
#include <NeptuneSFML\Object\Object.h>

#endif // COLLISION_INFOS_H