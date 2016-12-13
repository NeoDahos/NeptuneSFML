#include <NeptuneSFML\Manager\TileSetManager.h>

namespace nep
{
	bool TileSetManager::AddTileSet(const sf::String& _name, const sf::String& _path)
	{
		TileSet* texture = new TileSet(_name, _path);
		m_tileSetsMap.emplace(_name, texture);
		return true;
	}

	const TileSet& TileSetManager::GetTileSet(const sf::String& _name)
	{
		return *m_tileSetsMap[_name];
	}

	bool TileSetManager::EraseTileSet(const sf::String& _name)
	{
		if (m_tileSetsMap.find(_name) != m_tileSetsMap.end())
		{
			delete m_tileSetsMap[_name];
			m_tileSetsMap.erase(_name);

			return true;
		}

		return false;
	}

	void TileSetManager::ClearAllTileSets()
	{
		for (auto&& texture : m_tileSetsMap)
		{
			if (texture.second)
				delete texture.second;
		}

		m_tileSetsMap.clear();
	}
}