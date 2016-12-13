#include <NeptuneSFML\Object\SpriteBatch.h>

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

namespace nep
{
	SpriteBatch::SpriteBatch()
	{
		m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);
	}

	void SpriteBatch::SetTexture(const sf::Texture* _texture)
	{
		m_renderStates.texture = _texture;
	}

	void SpriteBatch::AddSprite(sf::Sprite* _sprite)
	{
		//!à opti
		size_t spriteCount = m_sprites.size();
		/*size_t i = 0;
		sf::Sprite** currentSprite = m_sprites.data();;

		while (i < spriteCount && *currentSprite != _sprite)
		{
			i++;
			currentSprite++;
		}

		if (i != spriteCount)
			return;*/

		m_sprites.push_back(_sprite);

		spriteCount++;
		m_vertices.resize(spriteCount * 4);
	}

	void SpriteBatch::RemoveSprite(sf::Sprite* _sprite)
	{
		size_t spriteCount = m_sprites.size();
		size_t i = 0;
		sf::Sprite** currentSprite = m_sprites.data();;

		while (i < spriteCount && *currentSprite != _sprite)
		{
			i++;
			currentSprite++;
		}

		if (i == spriteCount)
			return;

		std::swap(m_sprites[i], m_sprites.back());
		m_sprites.erase(m_sprites.end() - 1);

		spriteCount--;
		m_vertices.resize(spriteCount * 4);
	}

	// Thanks to http://www.gamedev.net/blog/1739/entry-2259439-arkanong-part-6-batching-draw-calls/
	void SpriteBatch::Draw(sf::RenderTarget& _target)
	{
		sf::FloatRect bounds;
		sf::IntRect textureRect;
		const size_t spriteCount = m_sprites.size();
		sf::Vertex* currentVertex;
		sf::Sprite** currentSprite;

		if (spriteCount == 0)
			return;

		currentVertex = &m_vertices[0];
		currentSprite = m_sprites.data();

		for (size_t i = 0; i < spriteCount; i++)
		{
			bounds = (*currentSprite)->getGlobalBounds();
			textureRect = (*currentSprite)->getTextureRect();

			currentVertex->position.x = bounds.left;
			currentVertex->position.y = bounds.top;
			currentVertex->texCoords.x = static_cast<float>(textureRect.left);
			currentVertex->texCoords.y = static_cast<float>(textureRect.top);

			currentVertex++;
			currentVertex->position.x = bounds.left + bounds.width;
			currentVertex->position.y = bounds.top;
			currentVertex->texCoords.x = static_cast<float>(textureRect.left + textureRect.width);
			currentVertex->texCoords.y = static_cast<float>(textureRect.top);

			currentVertex++;
			currentVertex->position.x = bounds.left + bounds.width;
			currentVertex->position.y = bounds.top + bounds.height;
			currentVertex->texCoords.x = static_cast<float>(textureRect.left + textureRect.width);
			currentVertex->texCoords.y = static_cast<float>(textureRect.top + textureRect.height);

			currentVertex++;
			currentVertex->position.x = bounds.left;
			currentVertex->position.y = bounds.top + bounds.height;
			currentVertex->texCoords.x = static_cast<float>(textureRect.left);
			currentVertex->texCoords.y = static_cast<float>(textureRect.top + textureRect.height);

			currentVertex++;
			currentSprite++;

			/*if (spriteCount % 2500 == 0)
			{
				_target.draw(m_vertices, m_renderstate);
				m_vertices.clear();
			}*/
		}

		_target.draw(m_vertices, m_renderStates);
	}

	void SpriteBatch::Draw(sf::RenderTarget & _target, const std::vector<sf::Sprite *> & _toDraw, const sf::Texture * _texture)
	{
		sf::RenderStates renderStates;
		sf::VertexArray vertices;
		sf::Color color;
		sf::FloatRect bounds;
		sf::IntRect textureRect;
		sf::Vertex* currentVertex;
		sf::Sprite * const * currentSprite;
		const size_t spriteCount = _toDraw.size();

		if (spriteCount == 0)
			return;

		renderStates.texture = _texture;
		vertices.setPrimitiveType(sf::PrimitiveType::Quads);
		vertices.resize(spriteCount * 4);
		currentVertex = &vertices[0];
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

		_target.draw(vertices, renderStates);
	}

	void SpriteBatch::Draw(sf::RenderTarget & _target, const std::vector<sf::RectangleShape *> & _toDraw)
	{
		sf::RenderStates renderStates;
		sf::VertexArray vertices;
		sf::Color color;
		sf::FloatRect bounds;
		sf::IntRect textureRect;
		sf::Vertex* currentVertex;
		sf::RectangleShape * const * currentShape;
		const size_t shapeCount = _toDraw.size();

		if (shapeCount == 0)
			return;

		vertices.setPrimitiveType(sf::PrimitiveType::Quads);
		vertices.resize(shapeCount * 4);
		currentVertex = &vertices[0];
		currentShape = _toDraw.data();

		for (size_t i = 0; i < shapeCount; i++)
		{
			bounds = (*currentShape)->getGlobalBounds();
			textureRect = (*currentShape)->getTextureRect();
			color = (*currentShape)->getFillColor();

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
			currentShape++;
		}

		_target.draw(vertices, renderStates);
	}
}