#ifndef LEVEL_H
#define LEVEL_H

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\System\String.hpp>

#include <vector>

class Level
{
public:
	struct Chunk
	{
		sf::Sprite sprite;
		sf::IntRect rect;
		sf::Vector2f position;
		char heightMask[256][256];
	};

	void Init(const sf::String& _textureName);
	void Draw(sf::RenderTarget& _target);

	void AddChunk(const Chunk& _chunk);

private:
	const sf::Texture* m_texture;
	std::vector<Chunk> m_chunks;
};

#endif // LEVEL_H