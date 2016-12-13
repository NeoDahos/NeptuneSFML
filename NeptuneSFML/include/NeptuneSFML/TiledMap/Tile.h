#ifndef TILE_H
#define TILE_H
#include <NeptuneSFML\Export.h>

#include <SFML\Graphics\Sprite.hpp>

#include <vector>

namespace nep
{
	enum TileType {Walkable = 0, Obstacle, Hole, ShallowWater, DeepWater, Lava, Prickles};

	class TileInfos
	{
	public:
		sf::IntRect textureRect;
		sf::Vector2u size;
		TileType type = TileType::Walkable;
		unsigned short defaultLayer = 0;
		unsigned int index = 0;
	};

	class AnimatedTileInfos : public TileInfos
	{
	public:
		std::vector<sf::Vector2u> framesPosition;
		float frameTime = 0.12f;
		size_t frameCount = 1;
	};
	
	class Tile : public sf::Sprite
	{
	public:
		TileInfos infos;
		unsigned short layer = 0;
	};

	class AnimatedTile : public sf::Sprite
	{
	public:
		void Update(float _deltaTime)
		{
			currentFrameTime += _deltaTime;
			if (currentFrameTime >= infos.frameTime)
			{
				frameIndex = (frameIndex + 1) % infos.frameCount;
				setTextureRect(sf::IntRect(infos.framesPosition[frameIndex].x, infos.framesPosition[frameIndex].y, infos.size.x, infos.size.y));
				infos.textureRect.left = infos.framesPosition[frameIndex].x;
				infos.textureRect.top = infos.framesPosition[frameIndex].y;
				currentFrameTime -= infos.frameTime;
			}
		}

		AnimatedTileInfos infos;
		unsigned short layer = 0;

	private:
		float currentFrameTime = 0.f;
		unsigned short frameIndex = 0;
	};
}

#endif // TILE_H