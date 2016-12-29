#include <NeptuneSFML\UI\Button.h>
#include <NeptuneSFML\EngineCore.h>

namespace nep
{
	unsigned int  Button::s_instanceCount = 0;

	Button::Button()
	{
		s_instanceCount++;
		m_name = "Button" + std::to_string(s_instanceCount);

		m_text.setFont(FontMng.GetFont("default"));
		m_text.setString(m_name);
		const sf::FloatRect textBounds = m_text.getLocalBounds();
		m_text.setOrigin(textBounds.left + (textBounds.width / 2.f), textBounds.top + (textBounds.height / 2.f));
		m_text.setPosition(60.f, 25.f);

		m_shape.setPointCount(4);
		m_shape.setPoint(0, { 0.f, 0.f });
		m_shape.setPoint(1, { 120.f, 0.f });
		m_shape.setPoint(2, { 120.f, 50.f });
		m_shape.setPoint(3, { 0.f, 50.f });
		m_shape.setFillColor(sf::Color(150, 150, 150));
		m_shape.setOutlineColor(sf::Color(200, 200, 200));
		m_shape.setOutlineThickness(2.f);
	}

	Button::~Button()
	{
		s_instanceCount--;
	}

	void Button::SetTextString(const sf::String & _string)
	{
		m_text.setString(_string);
		const sf::FloatRect textBounds = m_text.getLocalBounds();
		m_text.setOrigin(textBounds.left + (textBounds.width / 2.f), textBounds.top + (textBounds.height / 2.f));
	}

	inline void Button::SetTextCharacterSize(unsigned int _size)
	{
		m_text.setCharacterSize(_size);
	}

	inline void Button::SetTextStyle(sf::Uint32 _style)
	{
		m_text.setStyle(_style);
	}

	inline void Button::SetTextFont(const sf::Font & _font)
	{
		m_text.setFont(_font);
	}

	inline void Button::SetTextFillColor(const sf::Color & _color)
	{
		m_text.setFillColor(_color);
	}

	inline void Button::SetTextOutlineColor(const sf::Color & _color)
	{
		m_text.setOutlineColor(_color);
	}

	inline void Button::SetTextOutlineThickness(float _thickness)
	{
		m_text.setOutlineThickness(_thickness);
	}

	inline void Button::SetTextPosition(const sf::Vector2f & _position)
	{
		m_text.setPosition(m_position + _position);
	}

	inline void Button::SetShapePointcount(size_t _count)
	{
		m_shape.setPointCount(_count);
	}

	inline void Button::SetShapePoint(size_t _index, const sf::Vector2f & _position)
	{
		m_shape.setPoint(_index, _position);
	}

	inline void Button::SetShapeFillColor(const sf::Color & _color)
	{
		m_shape.setFillColor(_color);
	}

	inline void Button::SetShapeOutlineColor(const sf::Color & _color)
	{
		m_shape.setOutlineColor(_color);
	}

	inline void Button::SetShapeOutlineThickness(float _thickness)
	{
		m_shape.setOutlineThickness(_thickness);
	}

	void Button::SetPosition(const sf::Vector2f & _position)
	{
		const sf::Vector2f offset = _position - m_position;
		m_position = _position;
		m_text.move(offset);
		m_shape.move(offset);
	}

	bool Button::HandleEvent(const sf::Event & _event)
	{
		if (m_state != WidgetState::Inactive)
		{
			if (_event.type == sf::Event::MouseMoved)
			{
				bool mouseIn = m_renderTarget ?
					m_shape.getGlobalBounds().contains(m_renderTarget->mapPixelToCoords({ _event.mouseMove.x, _event.mouseMove.y })) :
					m_shape.getGlobalBounds().contains(static_cast<float>(_event.mouseMove.x), static_cast<float>(_event.mouseMove.y));

				if (mouseIn)
				{
					if (!m_isMouseIn)
					{
						HandleMouseEnter(_event.mouseMove.x, _event.mouseMove.y);
						return true;
					}
				}
				else if (m_isMouseIn)
				{
					HandleMouseLeave(_event.mouseMove.x, _event.mouseMove.y);
					return true;
				}
			}
			else if (_event.type == sf::Event::MouseButtonPressed)
			{
				if (m_isMouseIn)
				{
					HandleMouseButtonEvent(_event.mouseButton.button, true, _event.mouseButton.x, _event.mouseButton.y);
					return true;
				}
			}
			else if (_event.type == sf::Event::MouseButtonReleased)
			{
				if (m_isMouseIn)
				{
					HandleMouseButtonEvent(_event.mouseButton.button, false, _event.mouseButton.x, _event.mouseButton.y);
					return true;
				}
			}
		}

		return false;
	}

	void Button::Draw(sf::RenderTarget & _target)
	{
		if (m_isVisible)
		{
			_target.draw(m_shape);
			_target.draw(m_text);
		}
	}
}