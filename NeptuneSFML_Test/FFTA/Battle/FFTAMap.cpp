#include "FFTAMap.h"

#include <NeptuneSFML\EngineCore.h>
#include <NeptuneSFML\json.hpp>

#include <fstream>
#include <exception>


#define IX(x, y) ((x) + (y) * (m_mapWidth))
#define TILE_WIDTH 32.f
#define TILE_HEIGHT 16.f
#define TILE_ELEVATION 8.f

void FFTAMap::DeInit()
{
	if (m_heightMap)
		delete[] m_heightMap;

	if (m_walkabilityMap)
		delete[] m_walkabilityMap;

	if (m_priorityMap)
		delete[] m_priorityMap;

	m_layers.clear();
	m_startTiles.clear();
}

void FFTAMap::Init(const sf::String & _jsonPath, sf::RenderWindow & _window)
{
	std::ifstream fileStream(_jsonPath.toAnsiString(), std::ifstream::in);
	nlohmann::json fileJson;

	if (fileStream.is_open())
	{
		try
		{
			fileStream >> fileJson;

			const sf::Texture & texture = nep::TextureMng.GetTexture(fileJson["textureName"]);
			std::vector<sf::IntRect> backgroundRects;
			nlohmann::json rectJson;
			nlohmann::json backgroundColorJson;
			nlohmann::json heightMapJson;
			nlohmann::json walkabilityMapJson;
			nlohmann::json priorityMapJson;
			const sf::Vector2u windowSize = _window.getSize();
			sf::Vector2f position;
			size_t rectCount = fileJson["layer0Rects"].size();
			size_t layerCount = fileJson["layersRect"].size();
			size_t startTilesCount = fileJson["startTiles"].size();
			size_t i;

			for (i = 0; i < rectCount; i++)
			{
				rectJson = fileJson["layer0Rects"][i];
				backgroundRects.emplace_back(rectJson["x"], rectJson["y"], rectJson["width"], rectJson["height"]);
			}

			position.x = (windowSize.x - backgroundRects[0].width) / 2.f;
			position.y = (windowSize.y - backgroundRects[0].height) / 2.f;
			m_layer0.Init(texture, backgroundRects, fileJson["layer0FrameTime"].get<float>());
			m_layer0.setPosition(position);

			for (i = 0; i < layerCount; i++)
			{
				rectJson = fileJson["layersRect"][i];
				m_layers.emplace_back(texture);
				m_layers[i].setTextureRect({ rectJson["x"], rectJson["y"], rectJson["width"], rectJson["height"] });
				m_layers[i].setPosition(position);
			}

			backgroundColorJson = fileJson["backgroundColor"];
			m_backgroundColor = sf::Color(backgroundColorJson["r"], backgroundColorJson["g"], backgroundColorJson["b"]);
			
			m_mapWidth = fileJson["mapWidth"];
			m_mapHeight = fileJson["mapHeight"];
			m_referencePosition.x = fileJson["referencePosition"]["x"];
			m_referencePosition.y = fileJson["referencePosition"]["y"];
			m_referencePosition += position;

			for (i = 0; i < startTilesCount; i++)
				m_startTiles.push_back({ fileJson["startTiles"][i]["x"], fileJson["startTiles"][i]["y"] });

			heightMapJson = fileJson["heightMap"];
			walkabilityMapJson = fileJson["walkabilityMap"];
			priorityMapJson = fileJson["priorityMap"];
			m_heightMap = new int[m_mapWidth * m_mapHeight];
			m_walkabilityMap = new int[m_mapWidth * m_mapHeight];
			m_priorityMap = new int[m_mapWidth * m_mapHeight];

			for (int y = 0; y < m_mapHeight; y++)
			{
				for (int x = 0; x < m_mapWidth; x++)
				{
					m_heightMap[IX(x, y)] = heightMapJson[IX(x, y)];
					m_walkabilityMap[IX(x, y)] = walkabilityMapJson[IX(x, y)];
					m_priorityMap[IX(x, y)] = priorityMapJson[IX(x, y)];
				}
			}

			fileStream.close();
		}
		catch (std::exception e)
		{
			printf("An error occured while reading \"%s\".\nMessage : %s.\n\n", _jsonPath.toAnsiString().c_str(), e.what());
			fileStream.close();
		}
	}
}

void FFTAMap::Update(float _deltaTime)
{
	m_layer0.Update(_deltaTime);
}

void FFTAMap::DrawLayer(sf::RenderTarget& _target, unsigned int _layer)
{
	if (_layer == 0)
	{
		_target.clear(m_backgroundColor);
		_target.draw(m_layer0);
	}
	else if(_layer <= m_layers.size())
		_target.draw(m_layers[_layer - 1]);
}

sf::Vector2f FFTAMap::GetIsometricPosition(int _x, int _y) const
{
	sf::Vector2f isometricPos;

	if (_x >= 0 && _x < m_mapWidth && _y >= 0 && _y < m_mapHeight)
	{
		isometricPos.x = ((_x - _y) / 2.f) * TILE_WIDTH;
		isometricPos.y = ((_x + _y) / (TILE_WIDTH / TILE_HEIGHT)) * TILE_HEIGHT - m_heightMap[IX(_x, _y)] * TILE_ELEVATION;
	}

	return m_referencePosition + isometricPos;
}

sf::Vector2f FFTAMap::GetIsometricPosition(const sf::Vector2i & _position) const
{
	sf::Vector2f isometricPos;

	if (_position.x >= 0 && _position.x < m_mapWidth && _position.y >= 0 && _position.y < m_mapHeight)
	{
		isometricPos.x = ((_position.x - _position.y) / 2.f) * TILE_WIDTH;
		isometricPos.y = ((_position.x + _position.y) / (TILE_WIDTH / TILE_HEIGHT)) * TILE_HEIGHT - m_heightMap[IX(_position.x, _position.y)] * TILE_ELEVATION;
	}

	return m_referencePosition + isometricPos;
}

sf::Vector2i FFTAMap::GetClampedTilePosition(int _x, int _y) const
{
	sf::Vector2i clampedPosition;
	clampedPosition.x = ((_x < 0) ? 0 : ((_x >= m_mapWidth) ? m_mapWidth - 1 : _x));
	clampedPosition.y = ((_y < 0) ? 0 : ((_y >= m_mapHeight) ? m_mapHeight - 1 : _y));

	return clampedPosition;
}

sf::Vector2i FFTAMap::GetClampedTilePosition(const sf::Vector2i & _position) const
{
	sf::Vector2i clampedPosition;
	clampedPosition.x = ((_position.x < 0) ? 0 : ((_position.x >= m_mapWidth) ? m_mapWidth - 1 : _position.x));
	clampedPosition.y = ((_position.y < 0) ? 0 : ((_position.y >= m_mapHeight) ? m_mapHeight - 1 : _position.y));

	return clampedPosition;
}

int FFTAMap::GetTileElevation(int _x, int _y) const
{
	return m_heightMap[IX(_x, _y)];
}

int FFTAMap::GetTileElevation(const sf::Vector2i _position) const
{
	return m_heightMap[IX(_position.x, _position.y)];
}

int FFTAMap::GetTilePriority(int _x, int _y) const
{
	return m_priorityMap[IX(_x, _y)];
}

int FFTAMap::GetTilePriority(const sf::Vector2i _position) const
{
	return m_priorityMap[IX(_position.x, _position.y)];
}

int FFTAMap::GetLayerCount() const
{
	return m_layers.size();
}

const std::vector<sf::Vector2i> & FFTAMap::GetStarTiles() const
{
	return m_startTiles;
}
