#ifndef BUTTON_H
#define BUTTON_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\UI\Widget.h>

#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\ConvexShape.hpp>

namespace nep
{
	class NEPTUNE_API Button : public Widget
	{
	public:
		Button();
		virtual ~Button();

		void SetTextString(const sf::String & _string);
		void SetTextCharacterSize(unsigned int _size);
		void SetTextStyle(sf::Uint32 _style);
		void SetTextFont(const sf::Font & _font);
		void SetTextFillColor(const sf::Color & _color);
		void SetTextOutlineColor(const sf::Color & _color);
		void SetTextOutlineThickness(float _thickness);
		void SetTextPosition(const sf::Vector2f & _position);

		void SetShapePointcount(size_t _count);
		void SetShapePoint(size_t _index, const sf::Vector2f & _position);
		void SetShapeFillColor(const sf::Color & _color);
		void SetShapeOutlineColor(const sf::Color & _color);
		void SetShapeOutlineThickness(float _thickness);

		virtual void SetPosition(const sf::Vector2f & _position);

		virtual bool HandleEvent(const sf::Event& _event);
		virtual void Draw(sf::RenderTarget& _target);

	private:
		static unsigned int  s_instanceCount;

		sf::ConvexShape m_shape;
		sf::Text m_text;
	};
}

#endif // BUTTON_H