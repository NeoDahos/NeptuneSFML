#ifndef BUTTON_H
#define BUTTON_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\UI\Widget.h>

#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\ConvexShape.hpp>

#include <functional>

namespace nep
{
	class NEPTUNE_API Button : public Widget
	{
	public:
		Button();
		virtual ~Button();

		void SetTextString(const sf::String & _string);
		inline void SetTextCharacterSize(unsigned int _size);
		inline void SetTextStyle(sf::Uint32 _style);
		inline void SetTextFont(const sf::Font & _font);
		inline void SetTextFillColor(const sf::Color & _color);
		inline void SetTextOutlineColor(const sf::Color & _color);
		inline void SetTextOutlineThickness(float _thickness);
		inline void SetTextPosition(const sf::Vector2f & _position);

		inline void SetShapePointcount(size_t _count);
		inline void SetShapePoint(size_t _index, const sf::Vector2f & _position);
		inline void SetShapeFillColor(const sf::Color & _color);
		inline void SetShapeOutlineColor(const sf::Color & _color);
		inline void SetShapeOutlineThickness(float _thickness);

		void SetPosition(const sf::Vector2f & _position);

		virtual bool HandleEvent(const sf::Event& _event);
		virtual void Draw(sf::RenderTarget& _target);

	private:
		sf::ConvexShape m_shape;
		sf::Text m_text;
	};
}

#endif // BUTTON_H