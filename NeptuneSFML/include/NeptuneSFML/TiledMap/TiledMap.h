#ifndef TILED_MAP_H
#define TILED_MAP_H
#include <NeptuneSFML\Export.h>

#include <SFML\System\String.hpp>

#include <map>

namespace sf
{
	class RenderTarget;
}

namespace nep
{
	class TiledMapZone;

	class NEPTUNE_API TiledMap
	{
	public:
		void AddZone(const sf::String& _name, TiledMapZone* _zone);

		void SetCurrentZone(const sf::String& _name);

		void Update(float _deltaTime);
		void Draw(sf::RenderTarget& _target);

	private:
		std::map<sf::String, TiledMapZone*> m_zones;
		TiledMapZone* m_currentZone;
	};
}

#endif // TILED_MAP_H