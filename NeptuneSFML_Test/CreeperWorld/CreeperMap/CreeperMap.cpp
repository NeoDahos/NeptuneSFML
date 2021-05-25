#include "CreeperMap.h"
#include <fstream>
#include <iostream>
#include <SFML\System\Clock.hpp>
#include <NeptuneSFML\Tools.h>

#define IX(i, j) ((i) + (m_mapWidth + 2) * (j))
#define TILE_SIZE 16
#define UPDATE_TIME 0.015f

CreeperMap::~CreeperMap()
{
	if (m_nodes)
		delete[] m_nodes;

	if (m_creeperVertices)
		delete[] m_creeperVertices;
}

CreeperMap::Node CreeperMap::GetNodeAt(int _posX, int _posY)
{
	float posX = (-m_mapPosition.x + _posX) / TILE_SIZE / m_scale;
	float posY = (-m_mapPosition.y + _posY) / TILE_SIZE / m_scale;

	if (posX < 0 || posX > m_mapWidth || posY < 0 || posY > m_mapHeight)
		return Node();

	return m_nodes[IX((int)posX + 1, (int)posY + 1)];
}

void CreeperMap::Init(const sf::Vector2i& _windowSize, const sf::String& _path)
{
	m_windowSize = _windowSize;
	m_nodes = nullptr;
	m_mapWidth = 0;
	m_mapHeight = 0;
	m_mapSize = 0;
	m_movingMap = false;

	Load(_path);

	m_creeperTilesTexture.loadFromFile("../Data/Sprites/Creeper World/creeper.png");
	//m_creeperTilesTexture.loadFromFile("../Data/Sprites/Creeper World/creeperNoAlpha.png");
	m_terrainMaskTexture.loadFromFile("../Data/Sprites/Creeper World/mask.png");
	for (int i = 0; i < 10; i++)
	{
		m_terrainTilesTextures[i].loadFromFile("../Data/Sprites/Creeper World/level" + std::to_string(i) + ".png");
		m_terrainTilesTextures[i].setRepeated(true);
	}

	m_creeperVerticesBuffer.setPrimitiveType(sf::Quads);
	m_creeperVerticesBuffer.setUsage(sf::VertexBuffer::Usage::Stream);
	m_creeperVerticesBuffer.create(m_mapWidth * m_mapHeight * 4 * 10);
	m_creeperVertices = new sf::Vertex[m_mapWidth * m_mapHeight * 4 * 10];

	m_creeperRenderStates.texture = &m_creeperTilesTexture;
	m_creeperRenderStates.blendMode = sf::BlendAlpha;
	m_creeperSprite.setTexture(m_creeperTilesTexture);

	m_terrainRenderTexture.create(m_mapWidth * TILE_SIZE, m_mapHeight * TILE_SIZE);

	m_terrainLimit.setFillColor(sf::Color::Transparent);
	m_terrainLimit.setOutlineColor(sf::Color::Red);
	m_terrainLimit.setOutlineThickness(5);
	m_terrainLimit.setSize(sf::Vector2f((float)(m_mapWidth * TILE_SIZE), (float)(m_mapHeight * TILE_SIZE)));

	m_terrainMaskSprite.setTexture(m_terrainMaskTexture);
	
	SetZoom(0.25f);
	DrawTerrain();

	AddEmitter(64, 0, 2.f, 0.25f, 1000.f);
	AddEmitter(64, 32, 2.f, 0.5f, 100.f);
	AddEmitter(64, 64, 2.f, 0.75f, 100.f);
	AddEmitter(62, 118, 2.f, 2.f, 100.f);

	AddEmitter(100, 0, 2.f, 0.25f, 1000.f);
	AddEmitter(100, 32, 2.f, 0.5f, 100.f);
	AddEmitter(100, 64, 2.f, 0.75f, 100.f);
	AddEmitter(100, 118, 0.f, 0.5f, 0.5f);

	AddEmitter(0, 0, 2.f, 0.25f, 1000.f);
	AddEmitter(0, 100, 2.f, 0.5f, 100.f);
	AddEmitter(0, 64, 2.f, 0.75f, 100.f);
	AddEmitter(0, 32, 2.f, 2.f, 100.f);
}

void CreeperMap::Update(float _deltaTime)
{
	m_updateTimer += _deltaTime;
	if (m_updateTimer < UPDATE_TIME)
		return;

	m_updateTimer = 0.0f;

	Emit(UPDATE_TIME);

	for (y = 1; y <= m_mapHeight; y++)
	{
		for (x = 1; x <= m_mapWidth; x++)
		{
			TransferCreeper(m_nodes[IX(x, y)], m_nodes[IX(x + 1, y)], UPDATE_TIME); // Rigth neighbour
			TransferCreeper(m_nodes[IX(x, y)], m_nodes[IX(x - 1, y)], UPDATE_TIME); // Left neighbour
			TransferCreeper(m_nodes[IX(x, y)], m_nodes[IX(x, y + 1)], UPDATE_TIME); // Bottom neighbour
			TransferCreeper(m_nodes[IX(x, y)], m_nodes[IX(x, y - 1)], UPDATE_TIME); // Top neighbour
		}
	}

	for (y = 1; y <= m_mapHeight; y++)
	{
		for (x = 1; x <= m_mapWidth; x++)
		{
			if (m_nodes[IX(x, y)].newCreeper < 0.001f)
				m_nodes[IX(x, y)].newCreeper = 0.f;

			m_nodes[IX(x, y)].creeper = m_nodes[IX(x, y)].newCreeper;
			m_nodes[IX(x, y)].totalHeight = m_nodes[IX(x, y)].height + m_nodes[IX(x, y)].creeper;
		}
	}
}

void CreeperMap::Draw(sf::RenderTarget& _renderTarget)
{
	_renderTarget.draw(m_terrainSprite);

	DrawCreeper(_renderTarget);

	_renderTarget.draw(m_terrainLimit);
}

void CreeperMap::AddEmitter(int _posX, int _posY, float _startTime, float _rate, float _quantity)
{
	CreeperEmiter emitter;
	emitter.position = sf::Vector2i(_posX + 1, _posY + 1);
	emitter.time = _startTime;
	emitter.rate = _rate;
	emitter.quantity = _quantity;
	m_emitters.push_back(emitter);
}

void CreeperMap::Emit(float _deltaTime)
{
	for (auto&& emitter : m_emitters)
	{
		emitter.time -= _deltaTime;
		if (emitter.time <= 0)
		{
			emitter.time = emitter.rate;
			m_nodes[IX(emitter.position.x, emitter.position.y)].creeper += emitter.quantity;
			m_nodes[IX(emitter.position.x, emitter.position.y)].newCreeper = m_nodes[IX(emitter.position.x, emitter.position.y)].creeper;
		}
	}
}

void CreeperMap::Zoom(float _delta, int _mousePositionX, int _mousePositionY)
{
	if (nep::IsEqualByEpsilon(m_scale, 0.25f) && _delta < 0 || nep::IsEqualByEpsilon(m_scale, 1.f) && _delta > 0)
		return;

	float zoomFactor = 0.95f;
	if (_delta > 0)
		zoomFactor = 1 / zoomFactor;

	m_scale *= zoomFactor;
	m_scale = nep::Clamp(m_scale, 0.25f, 1.f);

	//m_terrainSprite.setScale(m_scale, m_scale);
	//m_terrainTileSprite.setScale(m_scale, m_scale);
	m_terrainLimit.setScale(m_scale, m_scale);
	m_terrainMaskSprite.setScale(m_scale, m_scale);
	m_creeperSprite.setScale(m_scale, m_scale);

	float deltaX = (m_mapPosition.x - _mousePositionX) * (zoomFactor - 1);
	float deltaY = (m_mapPosition.y - _mousePositionY) * (zoomFactor - 1);

	LimitMapMovement(deltaX, deltaY);
}

void CreeperMap::SetZoom(float _zoomValue)
{
	m_scale = nep::Clamp(_zoomValue, 0.25f, 1.f);

	//m_terrainSprite.setScale(m_scale, m_scale);
	//m_terrainTileSprite.setScale(m_scale, m_scale);
	m_terrainLimit.setScale(m_scale, m_scale);
	m_terrainMaskSprite.setScale(m_scale, m_scale);
	m_creeperSprite.setScale(m_scale, m_scale);

	LimitMapMovement(0.f, 0.f);
}

void CreeperMap::StartMoveMap(int _mousePositionX, int _mousePositionY)
{
	m_movingMap = true;
	m_oldMousePosition.x = _mousePositionX;
	m_oldMousePosition.y = _mousePositionY;
}

void CreeperMap::StopMoveMap()
{
	m_movingMap = false;
}

void CreeperMap::MoveMap(int _mousePositionX, int _mousePositionY)
{
	if (m_movingMap)
	{
		float deltaX = (float)(_mousePositionX - m_oldMousePosition.x);
		float deltaY = (float)(_mousePositionY - m_oldMousePosition.y);

		LimitMapMovement(deltaX, deltaY);

		m_oldMousePosition.x = _mousePositionX;
		m_oldMousePosition.y = _mousePositionY;
	}
}

void CreeperMap::Load(const sf::String& _path)
{
	std::ifstream file(_path.toAnsiString(), std::ios::in);

	if (file)
	{
		file >> m_mapWidth >> m_mapHeight;

		m_mapSize = (m_mapWidth + 2) * (m_mapHeight + 2);
		m_nodes = new Node[m_mapSize];

		char terrainHeight;
		for (y = 1; y <= m_mapHeight; y++)
		{
			for (x = 1; x <= m_mapWidth; x++)
			{
				file.get(terrainHeight);

				if (terrainHeight == '\n')
				{
					x--;
					continue;
				}

				m_nodes[IX(x, y)].creeper = 0.f;
				m_nodes[IX(x, y)].newCreeper = 0.f;
				m_nodes[IX(x, y)].height = terrainHeight - '0' + 1;
			}
		}

		file.close();
	}
	else
		std::cerr << __FUNCTION__ << "Can't open file !" << std::endl;
}

void CreeperMap::TransferCreeper(Node& _source, Node& _target, float _deltaTime)
{
	if (_source.height > 0 && _target.height > 0)
	{
		float sourceCreeper = _source.creeper;
		if (sourceCreeper > 0.f)
		{
			float sourceTotal = _source.height + sourceCreeper;
			float targetTotal = _target.height + _target.creeper;
			float delta = 0.f;

			if (sourceTotal > targetTotal)
			{
				delta = sourceTotal - targetTotal;
				if (delta > sourceCreeper)
					delta = sourceCreeper;

				delta *= m_diffusionRate * _deltaTime;
				_source.creeper -= delta;
				_target.newCreeper += delta;
			}
		}
	}
}

void CreeperMap::LimitMapMovement(float _deltaX, float _deltaY)
{
	const float mapPosLimitLeft = TILE_SIZE * 2.f;
	const float mapPosLimitRight = m_windowSize.x - mapPosLimitLeft;
	const float mapPosLimitTop = TILE_SIZE * 2.f;
	const float mapPosLimitBottom = m_windowSize.y - mapPosLimitTop;

	m_mapPosition.x += _deltaX;
	m_mapPosition.y += _deltaY;

	if (m_mapPosition.x < mapPosLimitLeft)
		m_mapPosition.x = mapPosLimitLeft;
	else if (m_mapPosition.x + m_mapWidth * m_scale * TILE_SIZE > mapPosLimitRight)
		m_mapPosition.x = mapPosLimitRight - m_mapWidth * m_scale * TILE_SIZE;

	if (m_mapPosition.y < mapPosLimitTop)
		m_mapPosition.y = mapPosLimitTop;
	else if (m_mapPosition.y + m_mapHeight * m_scale * TILE_SIZE > mapPosLimitBottom)
		m_mapPosition.y = mapPosLimitBottom - m_mapHeight * m_scale * TILE_SIZE;

	m_terrainLimit.setPosition(m_mapPosition);
	m_terrainSprite.setPosition(m_mapPosition);
}

void CreeperMap::DrawTerrain()
{
	const sf::IntRect windowRect(0, 0, m_windowSize.x, m_windowSize.y);
	const int tileSize = TILE_SIZE;

	sf::RenderTexture terrainRenderTextureTmp;

	terrainRenderTextureTmp.create(m_mapWidth * tileSize, m_mapHeight * tileSize);
	m_terrainRenderTexture.clear(sf::Color::Transparent);

	for (int layer = 1; layer < 11; layer++)
	{
		terrainRenderTextureTmp.clear(sf::Color::Transparent);

		for (y = 1; y <= m_mapHeight; y++)
		{
			for (x = 1; x <= m_mapWidth; x++)
			{
				if (m_nodes[IX(x, y)].height >= layer)
				{
					m_nodes[IX(x, y)].index = 0;

					// Right
					if (x <= m_mapWidth && layer <= m_nodes[IX(x + 1, y)].height)
						m_nodes[IX(x, y)].index += 1;
					// Up
					if (y > 0 && layer <= m_nodes[IX(x, y - 1)].height)
						m_nodes[IX(x, y)].index += 2;
					// Left
					if (x > 0 && layer <= m_nodes[IX(x - 1, y)].height)
						m_nodes[IX(x, y)].index += 4;
					// Down
					if (y <= m_mapHeight && layer <= m_nodes[IX(x, y + 1)].height)
						m_nodes[IX(x, y)].index += 8;

					m_terrainMaskSprite.setTextureRect({ m_nodes[IX(x, y)].index * tileSize, 0, tileSize, tileSize });
					m_terrainMaskSprite.setPosition((x - 1) * tileSize * m_scale, (y - 1) * tileSize * m_scale);
					terrainRenderTextureTmp.draw(m_terrainMaskSprite);
				}
			}
		}

		m_terrainTileSprite.setTexture(m_terrainTilesTextures[layer - 1]);
		m_terrainTileSprite.setTextureRect({ 0, 0, m_mapWidth * tileSize, m_mapHeight * tileSize });
		terrainRenderTextureTmp.draw(m_terrainTileSprite, sf::RenderStates(sf::BlendMultiply));
		terrainRenderTextureTmp.display();

		m_terrainSprite.setTexture(terrainRenderTextureTmp.getTexture());
		m_terrainRenderTexture.draw(m_terrainSprite);
	}

	m_terrainRenderTexture.display();
	m_terrainSprite.setTexture(m_terrainRenderTexture.getTexture());
}

void CreeperMap::DrawCreeper(sf::RenderTarget& _renderTarget)
{
	const float tileSizeScaled = TILE_SIZE * m_scale;
	const int tileSize = (int)TILE_SIZE;
	const int mapPosTiledX = (int)(m_mapPosition.x / tileSizeScaled);
	const int mapPosTiledY = (int)(m_mapPosition.y / tileSizeScaled);
	const int drawLimitLeft = std::max(1, -mapPosTiledX);
	const int drawLimitRight = std::min(m_mapWidth, (int)(m_windowSize.x / tileSizeScaled) - mapPosTiledX + 1);
	const int drawLimitTop = std::max(1, -mapPosTiledY);
	const int drawLimitBottom = std::min(m_mapHeight, (int)(m_windowSize.y / tileSizeScaled) - mapPosTiledY + 1);
	const int layerSize = m_mapWidth * m_mapHeight * 4;

	int quadIndexes[10];
	sf::IntRect texturePos;
	sf::Vector2f tilePos;
	sf::Vertex quad[4];
	int layer, index, creeperValue;

	texturePos.height = TILE_SIZE;
	texturePos.width = TILE_SIZE;
	texturePos.top = 0;

	for (layer = 0; layer < 10; layer++)
		quadIndexes[layer] = 0;

	// Fixed values
	quad[0].texCoords.y = 0.f;
	quad[1].texCoords.y = 0.f;
	quad[2].texCoords.y = tileSize;
	quad[3].texCoords.y = tileSize;

	for (x = drawLimitLeft; x <= drawLimitRight; x++)
	{
		for (y = drawLimitTop; y <= drawLimitBottom; y++)
		{
			if (m_nodes[IX(x, y)].creeper > 0.f)
			{
				layer = 0;
				creeperValue = (int)(m_nodes[IX(x, y)].height + ceil(m_nodes[IX(x, y)].creeper));

				tilePos.x = (float)(m_mapPosition.x + (x - 1) * tileSizeScaled);
				tilePos.y = (float)(m_mapPosition.y + (y - 1) * tileSizeScaled);

				quad[0].position = tilePos;
				quad[1].position.x = tilePos.x + tileSizeScaled;
				quad[1].position.y = tilePos.y;
				quad[2].position.x = tilePos.x + tileSizeScaled;
				quad[2].position.y = tilePos.y + tileSizeScaled;
				quad[3].position.x = tilePos.x;
				quad[3].position.y = tilePos.y + tileSizeScaled;

				while (layer < 10 && layer < creeperValue)
				{
					index = 0;

					// Up
					if (layer < m_nodes[IX(x, y - 1)].totalHeight)
						index += 2;
					// Down
					if (layer < m_nodes[IX(x, y + 1)].totalHeight)
						index += 8;
					// Left
					if (layer < m_nodes[IX(x - 1, y)].totalHeight)
						index += 4;
					// Right
					if (layer < m_nodes[IX(x + 1, y)].totalHeight)
						index += 1;

					quad[0].texCoords.x = (float)(index * tileSize);
					quad[1].texCoords.x = (float)((index + 1) * tileSize);
					quad[2].texCoords.x = (float)((index + 1) * tileSize);
					quad[3].texCoords.x = (float)(index * tileSize);

					m_creeperVertices[layer * layerSize + quadIndexes[layer]] = quad[0];
					m_creeperVertices[layer * layerSize + quadIndexes[layer] + 1] = quad[1];
					m_creeperVertices[layer * layerSize + quadIndexes[layer] + 2] = quad[2];
					m_creeperVertices[layer * layerSize + quadIndexes[layer] + 3] = quad[3];

					quadIndexes[layer] += 4;
					layer++;
				}
			}
		}
	}

	m_creeperVerticesBuffer.update(m_creeperVertices);
	_renderTarget.draw(m_creeperVerticesBuffer, m_creeperRenderStates);
}