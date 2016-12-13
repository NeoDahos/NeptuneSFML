#include <NeptuneSFML\TiledMap\TiledMapZone.h>
#include <NeptuneSFML\EngineCore.h>

#include <SFML\Graphics\Text.hpp>

#include <fstream>

namespace nep
{
	void TiledMapZone::Init(const sf::String& _name, const TileSet& _tileSet)
	{
		const sf::Texture* tileSetTexture;
		Tile tile;
		AnimatedTile animatedTile;

		m_name = _name;
		m_tileSet = &_tileSet;
		m_minLayer = -1;
		m_maxLayer = 0;
		tileSetTexture = m_tileSet->getTexture();
		tile.setTexture(*tileSetTexture);
		animatedTile.setTexture(*tileSetTexture);
				
		//TEST
		for (size_t i = 0; i < m_tileSet->GetTileCount(); i++)
		{
				tile.infos = m_tileSet->GetTileInfos(i);
				tile.setTextureRect(tile.infos.textureRect);
				tile.setPosition(static_cast<float>(tile.infos.textureRect.left), static_cast<float>(tile.infos.textureRect.top));
				tile.layer = tile.infos.defaultLayer;

				if (tile.layer < m_minLayer)
					m_minLayer = tile.layer;
				if (tile.layer > m_maxLayer)
					m_maxLayer = tile.layer;

				m_tiles.push_back(tile);
		}
		std::sort(m_tiles.begin(), m_tiles.end(), &TiledMapZone::SortTileByLayer);

		for (size_t i = 0; i < m_tileSet->GetAnimatedTileCount(); i++)
		{
			animatedTile.infos = m_tileSet->GetAnimatedTileInfos(i);
			animatedTile.setTextureRect(animatedTile.infos.textureRect);
			animatedTile.setPosition(static_cast<float>(animatedTile.infos.textureRect.left), static_cast<float>(animatedTile.infos.textureRect.top));
			animatedTile.layer = animatedTile.infos.defaultLayer;

			if (tile.layer < m_minLayer)
				m_minLayer = tile.layer;
			if (tile.layer > m_maxLayer)
				m_maxLayer = tile.layer;

			m_animatedTiles.push_back(animatedTile);
		}
		std::sort(m_animatedTiles.begin(), m_animatedTiles.end(), &TiledMapZone::SortAnimatedTileByLayer);
		// END TEST
	}

	void TiledMapZone::Init(const sf::String& _name, const sf::String& _zoneInfosPath)
	{
		std::ifstream fileStream(_zoneInfosPath.toAnsiString(), std::ifstream::in);
		nlohmann::json fileJson;

		m_name = _name;
		m_minLayer = -1;
		m_maxLayer = 0;

		if (fileStream.is_open())
		{
			fileStream >> fileJson;

			m_tileSet = &TileSetMng.GetTileSet(fileJson["tileSet"]);

			LoadTiles(fileJson);

			fileStream.close();
		}
	}

	const sf::String& TiledMapZone::GetName() const
	{
		return m_name;
	}

	void TiledMapZone::Update(float _deltaTime)
	{
		size_t animatedTileCount = m_animatedTiles.size();
		for (size_t i = 0; i < animatedTileCount; i++)
			m_animatedTiles[i].Update(_deltaTime);
	}

	void TiledMapZone::Draw(sf::RenderTarget& _target)
	{
		size_t tileCount = m_tiles.size();
		size_t animatedTleCount = m_animatedTiles.size();
		size_t tileIndex = 0;
		size_t animatedTileIndex = 0;


		sf::Text indexText;
		sf::Font defaultFont;
		defaultFont.loadFromFile("../NeptuneSFML_Test/Data/Font/arial.ttf");
		indexText.setFont(defaultFont);
		indexText.setFillColor(sf::Color::Red);
		indexText.setCharacterSize(10);

		for (short layer = m_maxLayer; layer >= m_minLayer; layer--)
		{
			for (tileIndex; tileIndex < tileCount; tileIndex++)
			{
				if (m_tiles[tileIndex].layer != layer)
					break;

				_target.draw(m_tiles[tileIndex]);

				indexText.setPosition(m_tiles[tileIndex].getPosition());
				indexText.setString(std::to_string(m_tiles[tileIndex].infos.index));
				_target.draw(indexText);
			}

			for (animatedTileIndex; animatedTileIndex < animatedTleCount; animatedTileIndex++)
			{
				if (m_animatedTiles[animatedTileIndex].layer != layer)
					break;

				_target.draw(m_animatedTiles[animatedTileIndex]);

				indexText.setPosition(m_animatedTiles[animatedTileIndex].getPosition());
				indexText.setString(std::to_string(animatedTileIndex));
				_target.draw(indexText);
			}
		}
	}

	bool TiledMapZone::SortTileByLayer(Tile _left, Tile _right)
	{
		return _left.layer > _right.layer;
	}

	bool TiledMapZone::SortAnimatedTileByLayer(AnimatedTile _left, AnimatedTile _right)
	{
		return _left.layer > _right.layer;
	}

	void TiledMapZone::LoadTiles(const nlohmann::json& _fileJson)
	{
		nlohmann::json tileJson;
		Tile tile;
		size_t tileCount = _fileJson["tiles"].size();
		const sf::Texture* tileSetTexture = m_tileSet->getTexture();
		unsigned int width = 1;
		unsigned int height = 1;
		unsigned int x;
		unsigned int y;

		tile.setTexture(*tileSetTexture);

		for (size_t i = 0; i < tileCount; i++)
		{
			tileJson = _fileJson["tiles"][i];
			tile.infos = m_tileSet->GetTileInfos(tileJson["index"]);

			tile.setTextureRect(tile.infos.textureRect);
			//tile.setPosition(tileJson["x"], tileJson["y"]);

			if (tileJson["layer"].is_null())
				tile.layer = tile.infos.defaultLayer;
			else
				tile.layer = tileJson["layer"];

			if (tile.layer < m_minLayer)
				m_minLayer = tile.layer;
			if (tile.layer > m_maxLayer)
				m_maxLayer = tile.layer;


			if (tileJson["width"].is_null())
				width = 1;
			else
				width = tileJson["width"];

			if (tileJson["height"].is_null())
				height = 1;
			else
				height = tileJson["height"];

			for (x = 0; x < width; x++)
			{
				for (y = 0; y < height; y++)
				{
					tile.setPosition(static_cast<float>(tileJson["x"] + x * tile.infos.size.x), static_cast<float>(tileJson["y"] + y * tile.infos.size.y));
					m_tiles.push_back(tile);
				}
			}

		}
	}
}