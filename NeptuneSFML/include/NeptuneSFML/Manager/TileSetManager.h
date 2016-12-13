#ifndef TILE_SET_MANAGER_H
#define TILE_SET_MANAGER_H
#include <NeptuneSFML\Export.h>
#include <NeptuneSFML\TiledMap\TileSet.h>

#include <SFML\System\String.hpp>
#include <SFML\System\NonCopyable.hpp>

#include <map>

namespace nep
{
	class NEPTUNE_API TileSetManager : public sf::NonCopyable
	{
	public:
		bool AddTileSet(const sf::String& _name, const sf::String& _path);
		const TileSet& GetTileSet(const sf::String& _name);
		bool EraseTileSet(const sf::String& _name);
		void ClearAllTileSets();

	private:
		std::map<sf::String, TileSet*> m_tileSetsMap;
	};
}

#endif // TILE_SET_MANAGER_H