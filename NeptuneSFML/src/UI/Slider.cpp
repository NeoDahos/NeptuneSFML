#include <NeptuneSFML\UI\Slider.h>

namespace nep
{
	Slider::Slider() : RangeWidget()
	{
		m_name = "Slider" + std::to_string(s_instanceCount);

		m_bar.setFillColor(sf::Color(150, 150, 150));
		m_bar.setOutlineColor(sf::Color(200, 200, 200));
		m_bar.setOutlineThickness(2.f);
		m_bar.setSize({ 200.f, 10.f });

		m_indicator.setFillColor(sf::Color(200, 200, 200));
		m_indicator.setOutlineColor(sf::Color(150, 150, 150));
		m_indicator.setOutlineThickness(1.f);
		m_indicator.setSize({ 5.f, 16.f });
		m_indicator.setOrigin(m_indicator.getSize() / 2.f);
		PlaceIndicator();
	}

	Slider::~Slider()
	{
	}

	inline void Slider::SetMinValue(float _value)
	{
		if (_value > m_maxValue)
			return;

		m_minValue = _value;
		if (m_value < m_minValue)
		{
			m_value = m_minValue;
			if (m_onValueChangeFct)
				m_onValueChangeFct(m_value);
		}

		PlaceIndicator();
	}

	inline void Slider::SetMaxValue(float _value)
	{
		if (_value < m_minValue)
			return;

		m_maxValue = _value;
		if (m_value > m_maxValue)
		{
			m_value = m_maxValue;
			if (m_onValueChangeFct)
				m_onValueChangeFct(m_value);
		}

		PlaceIndicator();
	}

	inline void Slider::SetValue(float _value)
	{
		m_value = _value;
		if (m_value < m_minValue)
			m_value = m_minValue;
		else if (m_value > m_maxValue)
			m_value = m_maxValue;

		if (m_onValueChangeFct)
			m_onValueChangeFct(m_value);

		PlaceIndicator();
	}

	void Slider::SetOrientation(RangeWidgetOrientation _orientation)
	{
		if (m_orientation == _orientation)
			return;

		const sf::Vector2f barSize = m_bar.getSize();
		const sf::Vector2f indicatorSize = m_indicator.getSize();
		m_bar.setSize({ barSize.y, barSize.x });
		m_indicator.setSize({ indicatorSize.y, indicatorSize.x });
		m_indicator.setOrigin(indicatorSize.y / 2.f, indicatorSize.x / 2.f);
		m_orientation = _orientation;

		PlaceIndicator();
	}

	void Slider::SetPosition(const sf::Vector2f & _position)
	{
		const sf::Vector2f offset = _position - m_position;
		m_position = _position;
		m_bar.move(offset);
		m_indicator.move(offset);
	}

	void Slider::LittleIncrement()
	{
		m_value += m_littleStep;
		if (m_value > m_maxValue)
			m_value = m_maxValue;

		if (m_onValueChangeFct)
			m_onValueChangeFct(m_value);

		PlaceIndicator();
	}

	void Slider::LittleDecrement()
	{
		m_value -= m_littleStep;
		if (m_value < m_minValue)
			m_value = m_minValue;

		if (m_onValueChangeFct)
			m_onValueChangeFct(m_value);

		PlaceIndicator();
	}

	void Slider::BigIncrement()
	{
		m_value += m_bigStep;
		if (m_value > m_maxValue)
			m_value = m_maxValue;

		if (m_onValueChangeFct)
			m_onValueChangeFct(m_value);

		PlaceIndicator();
	}

	void Slider::BigDecrement()
	{
		m_value -= m_bigStep;
		if (m_value < m_minValue)
			m_value = m_minValue;

		if (m_onValueChangeFct)
			m_onValueChangeFct(m_value);

		PlaceIndicator();
	}

	bool Slider::HandleEvent(const sf::Event & _event)
	{
		if (m_state != WidgetState::Inactive)
		{
			if (_event.type == sf::Event::MouseMoved)
			{
				const bool mouseIn = m_renderTarget ?
					m_bar.getGlobalBounds().contains(m_renderTarget->mapPixelToCoords({ _event.mouseMove.x, _event.mouseMove.y })) :
					m_bar.getGlobalBounds().contains(static_cast<float>(_event.mouseMove.x), static_cast<float>(_event.mouseMove.y));

				if (mouseIn && !m_isMouseIn)
				{
					HandleMouseEnter(_event.mouseMove.x, _event.mouseMove.y);
					return true;
				}
				else if (!mouseIn && m_isMouseIn && m_state != WidgetState::Clicked)
				{
					HandleMouseLeave(_event.mouseMove.x, _event.mouseMove.y);
					return true;
				}
				else if (m_state == WidgetState::Clicked)
				{
					HandleMouseMove(_event.mouseMove.x, _event.mouseMove.y);
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

	void Slider::Draw(sf::RenderTarget & _target)
	{
		_target.draw(m_bar);
		_target.draw(m_indicator);
	}

	void Slider::HandleMouseMove(int _x, int _y)
	{
		if (m_state == WidgetState::Clicked)
		{
			const sf::Vector2f mousePosition = m_renderTarget ? m_renderTarget->mapPixelToCoords({ _x, _y }) : sf::Vector2f(static_cast<float>(_x), static_cast<float>(_y));
			const sf::Vector2f barSize = m_bar.getSize();
			float value = mousePosition.x - m_bar.getGlobalBounds().left - m_bar.getOutlineThickness();

			if (value >= 0.f)
			{
				if (value <= barSize.x)
				{
					value = ((m_maxValue - m_minValue) * (value / barSize.x)) + m_minValue;
					SetValue(value);
				}
				else
					SetValue(m_maxValue);
			}
			else
				SetValue(m_minValue);
		}

		if (m_onMouseMoveFct)
			m_onMouseMoveFct({ _x, _y });
	}

	void Slider::HandleMouseButtonEvent(sf::Mouse::Button _button, bool _isPressed, int _x, int _y)
	{
		if (_isPressed && m_isMouseIn)
		{
			const sf::Vector2f mousePosition = m_renderTarget ? m_renderTarget->mapPixelToCoords({ _x, _y }) : sf::Vector2f(static_cast<float>(_x), static_cast<float>(_y));
			const bool indicatorClicked = m_indicator.getGlobalBounds().contains(mousePosition);

			if (indicatorClicked)
				m_state = WidgetState::Clicked;
			else
			{
				const bool barClicked = m_bar.getGlobalBounds().contains(mousePosition);
				if (barClicked)
				{
					if (_x > m_indicator.getPosition().x)
						BigIncrement();
					else
						BigDecrement();
				}
			}

			if (m_onClickFct)
				m_onClickFct({ _button, _x, _y });
		}
		else
			m_state = WidgetState::Normal;
	}

	void Slider::PlaceIndicator()
	{
		const sf::FloatRect barBounds = m_bar.getGlobalBounds();
		const sf::Vector2f barSize = m_bar.getSize();
		const float barOutlineThickness = m_bar.getOutlineThickness();
		const float percent = (m_value - m_minValue) / (m_maxValue - m_minValue);

		if (m_orientation == RangeWidgetOrientation::OrientationHorizontal)
			m_indicator.setPosition(barBounds.left + barOutlineThickness + percent * barSize.x, barBounds.top + barOutlineThickness + barSize.y / 2.f);
		else
			m_indicator.setPosition(barBounds.left + barOutlineThickness + barSize.x / 2.f, barBounds.top + barOutlineThickness + percent * barSize.y);
	}
}