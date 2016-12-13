#ifndef TILED_MAP_ZONE_H
#define TILED_MAP_ZONE_H
#include <NeptuneSFML\Export.h>
#include <NeptuneSFML\TiledMap\Tile.h>
#include <NeptuneSFML\TiledMap\TileSet.h>
#include <NeptuneSFML\json.hpp>

#include <SFML\System\String.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RenderTexture.hpp>


namespace nep
{
	class NEPTUNE_API TiledMapZone
	{
	public:
		void Init(const sf::String& _name, const TileSet& _tileSet);
		void Init(const sf::String& _name, const sf::String& _zoneInfosPath);

		const sf::String& GetName() const;

		void Update(float _deltaTime);
		void Draw(sf::RenderTarget& _target);

	private:
		static bool SortTileByLayer(Tile _left, Tile _right);
		static bool SortAnimatedTileByLayer(AnimatedTile _left, AnimatedTile _right);

		void LoadTiles(const nlohmann::json& _fileJson);

		std::vector<Tile> m_tiles;
		std::vector<AnimatedTile> m_animatedTiles;
		sf::String m_name;
		const TileSet* m_tileSet;
		unsigned short m_minLayer;
		unsigned short m_maxLayer;
	};
}

#endif // TILED_MAP_ZONE_H