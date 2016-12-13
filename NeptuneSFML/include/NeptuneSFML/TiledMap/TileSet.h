#ifndef TILE_SET_H
#define TILE_SET_H
#include <NeptuneSFML\Export.h>
#include <NeptuneSFML\TiledMap\Tile.h>
#include <NeptuneSFML\json.hpp>

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\System\String.hpp>


namespace nep
{
	class NEPTUNE_API TileSet : public sf::Sprite
	{
	public:
		TileSet(const sf::String& _name, const sf::String& _tilesInfosPath);

		TileInfos GetTileInfos(unsigned int _index) const;
		AnimatedTileInfos GetAnimatedTileInfos(unsigned int _index) const;
		size_t GetTileCount() const;
		size_t GetAnimatedTileCount() const;

	private:
		void LoadTileSetInfos(const sf::String& _tilesInfosPath);
		void LoadTileInfos(const nlohmann::json& _fileJson);
		void LoadAnimatedTileInfos(const nlohmann::json& _fileJson);
		TileType GetTileType(const nlohmann::json& _tileJson);

		std::vector<TileInfos> m_tilesInfos;
		std::vector<AnimatedTileInfos> m_animatedTilesInfos;
		sf::String m_name;
		unsigned short m_width;
		unsigned short m_height;
	};
}

#endif // TILE_SET_H