#include <NeptuneSFML\UI\Label.h>
#include <NeptuneSFML\EngineCore.h>

namespace nep
{
	unsigned int Label::s_instanceCount = 0;

	Label::Label() : Widget()
	{
		s_instanceCount++;
		m_name = "RangeWidget" + std::to_string(s_instanceCount);

		m_text.setFont(FontMng.GetFont("default"));
		m_text.setString(m_name);
	}

	Label::~Label()
	{
		s_instanceCount--;
	}

	void Label::SetTextString(const sf::String & _string)
	{
		m_text.setString(_string);
	}

	void Label::SetTextCharacterSize(unsigned int _size)
	{
		m_text.setCharacterSize(_size);
	}

	void Label::SetTextStyle(sf::Uint32 _style)
	{
		m_text.setStyle(_style);
	}

	void Label::SetTextFont(const sf::Font & _font)
	{
		m_text.setFont(_font);
	}

	void Label::SetTextFillColor(const sf::Color & _color)
	{
		m_text.setFillColor(_color);
	}

	void Label::SetTextOutlineColor(const sf::Color & _color)
	{
		m_text.setOutlineColor(_color);
	}

	void Label::SetTextOutlineThickness(float _thickness)
	{
		m_text.setOutlineThickness(_thickness);
	}

	void Label::SetPosition(const sf::Vector2f & _position)
	{
		m_position = _position;
		m_text.setPosition(m_position);
	}

	bool Label::HandleEvent(const sf::Event & _event)
	{
		return false;
	}

	void Label::Draw(sf::RenderTarget & _target)
	{
		if(m_isVisible)
			_target.draw(m_text);
	}
}