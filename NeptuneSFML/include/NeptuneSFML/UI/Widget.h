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

		inline void SetRenderTarger(sf::RenderTarget * _renderTarget);
		inline void SetVisiblity(bool _isVisible);

		inline WidgetState GetState() const;
		inline bool GetVisibility() const;

		virtual void SetPosition(const sf::Vector2f & _position);

		inline void SetOnMouseEnterFct(const std::function<void(sf::Event::MouseMoveEvent)> & _fct);
		inline void SetOnMouseLeaveFct(const std::function<void(sf::Event::MouseMoveEvent)> & _fct);
		inline void SetOnMouseMoveFct(const std::function<void(sf::Event::MouseMoveEvent)> & _fct);
		inline void SetOnClickFct(const std::function<void(sf::Event::MouseButtonEvent)> & _fct);

		virtual bool HandleEvent(const sf::Event& _event) = 0;
		virtual void Draw(sf::RenderTarget& _target) = 0;

	protected:
		Widget();

		virtual void HandleMouseEnter(int _x, int _y);
		virtual void HandleMouseLeave(int _x, int _y);
		virtual void HandleMouseMove(int _x, int _y);
		virtual void HandleMouseButtonEvent(sf::Mouse::Button _button, bool _isPressed, int _x, int _y);

		std::function<void(sf::Event::MouseMoveEvent)> m_onMouseEnterFct;
		std::function<void(sf::Event::MouseMoveEvent)> m_onMouseLeaveFct;
		std::function<void(sf::Event::MouseMoveEvent)> m_onMouseMoveFct;
		std::function<void(sf::Event::MouseButtonEvent)> m_onClickFct;

		sf::String m_name;
		sf::Vector2f m_position;
		sf::RenderTarget* m_renderTarget;
		WidgetState m_state;
		bool m_isMouseIn;
		bool m_isVisible;
	};
}

#endif //WIDGET_H