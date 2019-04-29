#pragma once

#include <SFML\System\String.hpp>
#include <SFML\Graphics\Image.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\RenderTexture.hpp>
#include <SFML\Graphics\VertexBuffer.hpp>

class CreeperMap
{
public:
	struct CreeperEmiter
	{
		sf::Vector2i position;
		float time = 2.f;
		float rate = 0.f; // In milliseconds
		float quantity = 0.f;
	};

	struct Node
	{
		float totalHeight = 0.f; // Terrain height + creeper
		float creeper = 0.f; // 1.f per height level
		float newCreeper = 0.f; // 1.f per height level
		unsigned char height = 11; // Maximum level 10, 0 is void
		unsigned char index = 16;
	};

	CreeperMap() {};
	~CreeperMap();

	Node GetNodeAt(int _posX, int _posY);

	void Init(const sf::Vector2i& _windowSize, const sf::String& _path);
	void Update(float _deltaTime);
	void Draw(sf::RenderTarget& _renderTarget);

	void AddEmitter(int _posX, int _posY, float _startTime, float _rate, float _quantity);
	void Zoom(float _delta, int _mousePositionX, int _mousePositionY);
	void StartMoveMap(int _mousePositionX, int _mousePositionY);
	void StopMoveMap();
	void MoveMap(int _mousePositionX, int _mousePositionY);

protected:
	void Load(const sf::String& _path);
	void Emit(float _deltaTime);
	void TransferCreeper(Node& _source, Node& _target, float _deltaTime);
	void LimitMapMovement(float _deltaX, float _deltaY);
	void DrawTerrain();
	void DrawCreeper(sf::RenderTarget& _renderTarget);

protected:
	std::vector<CreeperEmiter> m_emitters;
	sf::Vertex* m_creeperVertices;
	sf::VertexBuffer m_creeperVerticesBuffer;
	sf::RenderStates m_creeperRenderStates;

	sf::Texture m_terrainTilesTextures[10];
	sf::Texture m_terrainMaskTexture;
	sf::Texture m_creeperTilesTexture;

	sf::Sprite m_terrainSprite;
	sf::Sprite m_terrainMaskSprite;
	sf::Sprite m_terrainTileSprite;
	sf::Sprite m_creeperSprite;

	sf::RenderTexture m_terrainRenderTexture;

	sf::RectangleShape m_terrainLimit;

	sf::Vector2f m_mapPosition;
	sf::Vector2i m_oldMousePosition;
	sf::Vector2i m_windowSize;

	const float m_diffusionRate = 0.35f;
	float m_scale;
	float m_updateTimer;

	Node* m_nodes;

	int m_mapWidth;
	int m_mapHeight;
	int m_mapSize;
	int x;
	int y;

	bool m_movingMap;
};