#ifndef LABEL_H
#define LABEL_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\UI\Widget.h>
#include <SFML\Graphics\Text.hpp>

namespace nep
{
	class NEPTUNE_API Label : public Widget
	{
	public:
		Label();
		virtual ~Label();

		void SetTextString(const sf::String & _string);
		void SetTextCharacterSize(unsigned int _size);
		void SetTextStyle(sf::Uint32 _style);
		void SetTextFont(const sf::Font & _font);
		void SetTextFillColor(const sf::Color & _color);
		void SetTextOutlineColor(const sf::Color & _color);
		void SetTextOutlineThickness(float _thickness);
		virtual void SetPosition(const sf::Vector2f & _position);

		virtual bool HandleEvent(const sf::Event& _event);
		virtual void Draw(sf::RenderTarget& _target);

	private:
		static unsigned int s_instanceCount;

		sf::Text m_text;
	};
}

#endif // LABEL_H