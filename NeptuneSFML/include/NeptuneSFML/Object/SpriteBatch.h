#ifndef SPRITE_BATCH_H
#define SPRITE_BATCH_H
#include <NeptuneSFML\Export.h>

#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\VertexArray.hpp>

#include <vector>

namespace sf
{
	class Sprite;
	class RectangleShape;
}

namespace nep
{
	class NEPTUNE_API SpriteBatch
	{
	public:
		SpriteBatch();

		void SetTexture(const sf::Texture* _texture);
		void AddSprite(sf::Sprite* _sprite);
		void RemoveSprite(sf::Sprite* _sprite);

		void Draw(sf::RenderTarget& _target);

		template<typename T>
		static void Draw(sf::RenderTarget & _target, const std::vector<sf::T *> & _toDraw, const sf::Texture * _texture);
		static void Draw(sf::RenderTarget & _target, const std::vector<sf::Sprite *> & _toDraw, const sf::Texture * _texture);
		static void Draw(sf::RenderTarget & _target, const std::vector<sf::RectangleShape *> & _toDraw);

	private:
		std::vector<sf::Sprite*> m_sprites;
		sf::VertexArray m_vertices;
		sf::RenderStates m_renderStates;
	};

	template<typename T>
	void SpriteBatch::Draw(sf::RenderTarget & _target, const std::vector<T *> & _toDraw, const sf::Texture * _texture)
	{
		sf::RenderStates renderStates;
		sf::VertexArray vertices;
		sf::Color color;
		sf::FloatRect bounds;
		sf::IntRect textureRect;
		sf::Vertex* currentVertex;
		T * const * currentSprite;
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
}

#endif // SPRITE_BATCH_H