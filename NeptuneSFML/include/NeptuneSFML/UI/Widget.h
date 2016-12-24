#ifndef WIDGET_H
#define WIDGET_H
#include <NeptuneSFML\Export.h>

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Window\Event.hpp>

#include <functional>

// http://sfgui.sfml-dev.de/

namespace nep
{
	class NEPTUNE_API Widget
	{
	public:
		enum WidgetState { Normal = 0, Hover, Clicked, Inactive, WidgetStateCount };

		virtual ~Widget();

		void SetState(WidgetState _state);
		WidgetState GetState() const;
		void SetVisiblity(bool _isVisible);
		bool GetVisibility() const;

		void SetOnMouseEnterFct(const std::function<void(sf::Event::MouseMoveEvent)> & _fct);
		void SetOnMouseLeaveFct(const std::function<void(sf::Event::MouseMoveEvent)> & _fct);
		void SetOnClickFct(const std::function<void(sf::Event::MouseButtonEvent)> & _fct);

		virtual bool HandleEvent(const sf::Event& _event) = 0;
		virtual void Draw(sf::RenderTarget& _target) = 0;

	protected:
		Widget();

		virtual void HandleMouseEnter(int _x, int _y);
		virtual void HandleMouseLeave(int _x, int _y);
		virtual void HandleMouseButtonEvent(sf::Mouse::Button _button, bool _isPressed, int _x, int _y);

		std::function<void(sf::Event::MouseMoveEvent)> m_onMouseEnterFct;
		std::function<void(sf::Event::MouseMoveEvent)> m_onMouseLeaveFct;
		std::function<void(sf::Event::MouseButtonEvent)> m_onClickFct;

		sf::Vector2f m_position;
		WidgetState m_state;
		bool m_isMouseIn;
		bool m_isVisible;
	};
}

#endif //WIDGET_H