#ifndef COLLIDER_MANAGER_H
#define COLLIDER_MANAGER_H
#include <NeptuneSFML\Export.h>

#include <SFML\System\NonCopyable.hpp>

#include <list>

namespace nep
{
class Collider;

	class NEPTUNE_API ColliderManager : public sf::NonCopyable
	{
	public:
		ColliderManager();
		~ColliderManager();

		void AddCollider(Collider* const _collider);
		void EraseCollider(Collider* const _collider);

		void ComputeCollisions();

	private:
		std::list<Collider*> m_colliders;
	};
}

#endif // COLLIDER_MANAGER_H