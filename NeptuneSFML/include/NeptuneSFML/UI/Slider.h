#ifndef SLIDER_H
#define SLIDER_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\UI\RangeWidget.h>
#include <SFML\Graphics\RectangleShape.hpp>

namespace nep
{
	class NEPTUNE_API Slider : public RangeWidget
	{
	public:
		Slider();
		virtual ~Slider();

		virtual void SetMinValue(float _value);
		virtual void SetMaxValue(float _value);
		virtual void SetValue(float _value);
		virtual void SetOrientation(RangeWidgetOrientation _orientation);
		virtual void SetPosition(const sf::Vector2f & _position);

		virtual void LittleIncrement();
		virtual void LittleDecrement();
		virtual void BigIncrement();
		virtual void BigDecrement();

		virtual bool HandleEvent(const sf::Event& _event);
		virtual void Draw(sf::RenderTarget& _target);

	private:
		virtual void HandleConfigurationChange();
		virtual void HandleMouseMove(int _x, int _y);
		virtual void HandleMouseButtonEvent(sf::Mouse::Button _button, bool _isPressed, int _x, int _y);

		void PlaceIndicator();

		sf::RectangleShape m_bar;
		sf::RectangleShape m_indicator;
	};
}

#endif // SLIDER_H