#include <NeptuneSFML\TiledMap\TileSet.h>
#include <NeptuneSFML\EngineCore.h>

#include <fstream>

namespace nep
{
	TileSet::TileSet(const sf::String& _name, const sf::String& _tilesInfosPath) : m_name(_name), m_width(0), m_height(0)
	{

		const sf::Texture& texture = TextureMng.GetTexture(m_name);
		setTexture(texture);

		LoadTileSetInfos(_tilesInfosPath);
	}

	TileInfos TileSet::GetTileInfos(unsigned int _index) const
	{
		if (_index >= m_tilesInfos.size())
			return TileInfos();

		return m_tilesInfos[_index];
	}

	AnimatedTileInfos TileSet::GetAnimatedTileInfos(unsigned int _index) const
	{
		if (_index >= m_animatedTilesInfos.size())
			return AnimatedTileInfos();

		return m_animatedTilesInfos[_index];
	}

	size_t TileSet::GetTileCount() const
	{
		return m_tilesInfos.size();
	}

	size_t TileSet::GetAnimatedTileCount() const
	{
		return m_animatedTilesInfos.size();
	}

	void TileSet::LoadTileSetInfos(const sf::String& _tilesInfosPath)
	{
		std::ifstream fileStream(_tilesInfosPath.toAnsiString(), std::ifstream::in);
		nlohmann::json fileJson;

		if (fileStream.is_open())
		{
			fileStream >> fileJson;

			LoadTileInfos(fileJson);
			LoadAnimatedTileInfos(fileJson);

			fileStream.close();
		}
	}

	void TileSet::LoadTileInfos(const nlohmann::json& _fileJson)
	{
		nlohmann::json tileJson;
		TileInfos tileInfos;
		size_t tileCount;
		unsigned int index = 0;

		tileCount = _fileJson["tiles"].size();
		for (index; index < tileCount; index++)
		{
			tileJson = _fileJson["tiles"][index];
			tileInfos.textureRect.left = tileJson["x"];
			tileInfos.textureRect.top = tileJson["y"];
			tileInfos.textureRect.width = tileJson["width"];
			tileInfos.textureRect.height = tileJson["height"];

			tileInfos.size.x = tileInfos.textureRect.width;
			tileInfos.size.y = tileInfos.textureRect.height;
			
			tileInfos.type = GetTileType(tileJson);
			tileInfos.defaultLayer = tileJson["defaultLayer"];
			tileInfos.index = index;

			m_tilesInfos.push_back(tileInfos);
		}
	}

	void TileSet::LoadAnimatedTileInfos(const nlohmann::json& _fileJson)
	{
		nlohmann::json tileJson;
		AnimatedTileInfos animatedTileInfos;
		size_t tileCount;
		unsigned int index = 0;

		tileCount = _fileJson["animatedTiles"].size();
		for (index = 0; index < tileCount; index++)
		{
			tileJson = _fileJson["animatedTiles"][index];

			animatedTileInfos.frameCount = tileJson["x"].size();
			animatedTileInfos.framesPosition.clear();
			for (size_t i = 0; i < animatedTileInfos.frameCount; i++)
				animatedTileInfos.framesPosition.emplace_back(tileJson["x"][i], tileJson["y"][i]);

			animatedTileInfos.textureRect.left = tileJson["x"][0];
			animatedTileInfos.textureRect.top = tileJson["y"][0];
			animatedTileInfos.textureRect.width = tileJson["width"];
			animatedTileInfos.textureRect.height = tileJson["height"];

			animatedTileInfos.size.x = animatedTileInfos.textureRect.width;
			animatedTileInfos.size.y = animatedTileInfos.textureRect.height;

			animatedTileInfos.type = GetTileType(tileJson);
			animatedTileInfos.defaultLayer = tileJson["defaultLayer"];
			animatedTileInfos.index = index;

			m_animatedTilesInfos.push_back(animatedTileInfos);
		}
	}

	TileType TileSet::GetTileType(const nlohmann::json& _tileJson)
	{
		if (_tileJson["type"] == "Walkable")
			return TileType::Walkable;
		else if (_tileJson["type"] == "Obstacle")
			return TileType::Obstacle;
		else if (_tileJson["type"] == "Hole")
			return TileType::Hole;
		else if (_tileJson["type"] == "ShallowWater")
			return TileType::ShallowWater;
		else if (_tileJson["type"] == "DeepWater")
			return TileType::DeepWater;
		else if (_tileJson["type"] == "Lava")
			return TileType::Lava;
		else if (_tileJson["type"] == "Prickles")
			return TileType::Prickles;

		return TileType::Walkable;
	}
}