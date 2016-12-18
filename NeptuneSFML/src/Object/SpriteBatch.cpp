#include <NeptuneSFML\Object\SpriteBatch.h>

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

namespace nep
{
	SpriteBatch& SpriteBatch::GetInstance()
	{
		static SpriteBatch instance;
		return instance;
	}

	/*SpriteBatch::~SpriteBatch()
	{
		m_threadData.Stop();
	}

	SpriteBatch::SpriteBatch() : m_threadData(m_vertices, SpriteBatch::ThreadData<4>::CopyToVertexArray)
	{
	}*/

	// Thanks to http://www.gamedev.net/blog/1739/entry-2259439-arkanong-part-6-batching-draw-calls/
	void SpriteBatch::Draw(sf::RenderTarget & _target, const std::vector<sf::Sprite *> & _toDraw, const sf::Texture * _texture)
	{
		sf::Color color;
		sf::FloatRect bounds;
		sf::IntRect textureRect;
		sf::Vertex* currentVertex;
		sf::Sprite * const * currentSprite;
		const size_t spriteCount = _toDraw.size();

		if (spriteCount == 0)
			return;

		m_renderStates.texture = _texture;
		m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);
		m_vertices.resize(spriteCount * 4);
		currentVertex = &m_vertices[0];
		currentSprite = _toDraw.data();

		for (size_t i = 0; i < spriteCount; i++)
		{
			bounds = (*currentSprite)->getGlobalBounds();
			textureRect = (*currentSprite)->getTextureRect();
			color = (*currentSprite)->getColor();

			currentVertex->position.x = bounds.left;
			currentVertex->position.y = bounds.top;
			currentVertex->texCoords.x = static_cast<float>(textureRect.left);
			currentVertex->texCoords.y = static_cast<float>(textureRect.top);
			currentVertex->color = color;

			currentVertex++;
			currentVertex->position.x = bounds.left + bounds.width;
			currentVertex->position.y = bounds.top;
			currentVertex->texCoords.x = static_cast<float>(textureRect.left + textureRect.width);
			currentVertex->texCoords.y = static_cast<float>(textureRect.top);
			currentVertex->color = color;

			currentVertex++;
			currentVertex->position.x = bounds.left + bounds.width;
			currentVertex->position.y = bounds.top + bounds.height;
			currentVertex->texCoords.x = static_cast<float>(textureRect.left + textureRect.width);
			currentVertex->texCoords.y = static_cast<float>(textureRect.top + textureRect.height);
			currentVertex->color = color;

			currentVertex++;
			currentVertex->position.x = bounds.left;
			currentVertex->position.y = bounds.top + bounds.height;
			currentVertex->texCoords.x = static_cast<float>(textureRect.left);
			currentVertex->texCoords.y = static_cast<float>(textureRect.top + textureRect.height);
			currentVertex->color = color;

			currentVertex++;
			currentSprite++;
		}

		_target.draw(m_vertices, m_renderStates);
	}
}