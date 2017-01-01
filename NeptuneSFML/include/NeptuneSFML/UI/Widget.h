#ifndef WIDGET_H
#define WIDGET_H
#include <NeptuneSFML\Export.h>

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Window\Event.hpp>

#include <functional>

namespace nep
{
	class Container;

	class NEPTUNE_API Widget
	{
	public:
		enum WidgetState { Normal = 0, Hover, Clicked, Inactive, WidgetStateCount };

		Widget();
		virtual ~Widget();

		void SetRenderTarger(sf::RenderTarget * _renderTarget);
		void SetVisiblity(bool _isVisible);
		virtual void SetPosition(const sf::Vector2f & _position);
		void SetRelativePosition(const sf::Vector2f & _position);
		void SetParent(Container * _parent);
		virtual void SetActive(bool _isActive);

		void SetOnMouseEnterFct(const std::function<void(sf::Event::MouseMoveEvent)> & _fct);
		void SetOnMouseLeaveFct(const std::function<void(sf::Event::MouseMoveEvent)> & _fct);
		void SetOnMouseMoveFct(const std::function<void(sf::Event::MouseMoveEvent)> & _fct);
		void SetOnClickFct(const std::function<void(sf::Event::MouseButtonEvent)> & _fct);

		WidgetState GetState() const;
		bool GetVisibility() const;
		sf::Vector2f GetPosition() const;
		Container * GetParent() const;

		virtual bool HandleEvent(const sf::Event& _event) = 0;
		virtual void Draw(sf::RenderTarget& _target) = 0;

	protected:
		Widget(const Widget & _other) = delete;
		void operator=(const Widget & _other) = delete;

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
		Container * m_parent;
		WidgetState m_state;
		bool m_isMouseIn;
		bool m_isVisible;
	};
}

#endif //WIDGET_H