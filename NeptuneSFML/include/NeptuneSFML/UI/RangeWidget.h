#ifndef RANGE_WIDGET_H
#define RANGE_WIDGET_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\UI\Widget.h>

namespace nep
{
	class NEPTUNE_API RangeWidget : public Widget
	{
	public:
		enum RangeWidgetOrientation { OrientationHorizontal = 0, OrientationVertical };

		RangeWidget();
		virtual ~RangeWidget();

		inline void SetLittleStep(float _step);
		inline void SetBigStep(float _step);
		virtual void SetMinValue(float _value);
		virtual void SetMaxValue(float _value);
		virtual void SetValue(float _value);
		virtual void SetOrientation(RangeWidgetOrientation _orientation);
		virtual void SetPosition(const sf::Vector2f & _position) = 0;
		inline void SetOnValueChangeFct(const std::function<void(float)> & _fct);

		inline float GetLittleStep() const;
		inline float GetBigStep() const;
		inline float GetMinValue() const;
		inline float GetMaxValue() const;
		inline float GetValue() const;
		inline RangeWidgetOrientation GetOrientation() const;

		virtual void LittleIncrement();
		virtual void LittleDecrement();
		virtual void BigIncrement();
		virtual void BigDecrement();

		virtual bool HandleEvent(const sf::Event& _event);
		virtual void Draw(sf::RenderTarget& _target) = 0;
	
	protected:
		static unsigned int  s_instanceCount;

		std::function<void(float)> m_onValueChangeFct;
		float m_littleStep;
		float m_bigStep;
		float m_minValue;
		float m_maxValue;
		float m_value;
		RangeWidgetOrientation m_orientation;
	};
}

#endif // RANGE_WIDGET_H