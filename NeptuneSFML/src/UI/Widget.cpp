#include <NeptuneSFML\UI\Widget.h>
#include <NeptuneSFML\UI\Container.h>

namespace nep
{
	Widget::Widget() : m_renderTarget(nullptr), m_parent(nullptr), m_state(WidgetState::Normal), m_isVisible(true), m_isMouseIn(false),
		m_onMouseEnterFct(nullptr), m_onMouseLeaveFct(nullptr), m_onMouseMoveFct(nullptr), m_onClickFct(nullptr)
	{
	}

	Widget::~Widget()
	{
	}

	void Widget::SetRenderTarger(sf::RenderTarget * _renderTarget)
	{
		m_renderTarget = _renderTarget;
	}

	void Widget::SetVisiblity(bool _isVisible)
	{
		m_isVisible = _isVisible;
		if (!m_isVisible)
		{
			m_state = WidgetState::Inactive;
			m_isMouseIn = false;
		}
		else if(m_state == WidgetState::Inactive)
			m_state = WidgetState::Normal;
	}

	void Widget::SetPosition(const sf::Vector2f & _position)
	{
		m_position = _position;
	}

	void Widget::SetRelativePosition(const sf::Vector2f & _position)
	{
		if (m_parent)
			SetPosition(m_parent->GetPosition() + _position);
		else
			SetPosition(_position);
	}

	void Widget::SetParent(Container * _parent)
	{
		m_parent = _parent;
	}

	void Widget::SetActive(bool _isActive)
	{
		if(_isActive)
			m_state = WidgetState::Normal;
		else
			m_state = WidgetState::Inactive;
	}

	void Widget::SetOnMouseEnterFct(const std::function<void(sf::Event::MouseMoveEvent)>& _fct)
	{
		m_onMouseEnterFct = _fct;
	}

	void Widget::SetOnMouseLeaveFct(const std::function<void(sf::Event::MouseMoveEvent)>& _fct)
	{
		m_onMouseLeaveFct = _fct;
	}

	void Widget::SetOnMouseMoveFct(const std::function<void(sf::Event::MouseMoveEvent)>& _fct)
	{
		m_onMouseMoveFct = _fct;
	}

	void Widget::SetOnClickFct(const std::function<void(sf::Event::MouseButtonEvent)> & _fct)
	{
		m_onClickFct = _fct;
	}

	Widget::WidgetState Widget::GetState() const
	{
		return m_state;
	}

	bool Widget::GetVisibility() const
	{
		return m_isVisible;
	}

	sf::Vector2f Widget::GetPosition() const
	{
		return m_position;
	}

	Container * Widget::GetParent() const
	{
		return m_parent;
	}

	void Widget::HandleMouseEnter(int _x, int _y)
	{
		m_isMouseIn = true;
		m_state = WidgetState::Hover;

		if (m_onMouseEnterFct)
			m_onMouseEnterFct({ _x, _y });
	}

	void Widget::HandleMouseLeave(int _x, int _y)
	{
		m_isMouseIn = false;
		m_state = WidgetState::Normal;

		if (m_onMouseLeaveFct)
			m_onMouseLeaveFct({ _x, _y });
	}

	void Widget::HandleMouseMove(int _x, int _y)
	{
		if (m_onMouseMoveFct)
			m_onMouseMoveFct({ _x, _y });
	}

	void Widget::HandleMouseButtonEvent(sf::Mouse::Button _button, bool _isPressed, int _x, int _y)
	{
		if (_isPressed && m_isMouseIn)
		{
			m_state = WidgetState::Clicked;
			if (m_onClickFct)
				m_onClickFct({ _button, _x, _y });
		}
		else
			m_state = WidgetState::Normal;
	}
}