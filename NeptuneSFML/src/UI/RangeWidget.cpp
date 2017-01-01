#include <NeptuneSFML\UI\RangeWidget.h>

namespace nep
{
	unsigned int RangeWidget::s_instanceCount = 0;

	RangeWidget::RangeWidget() : Widget()
	{
		s_instanceCount++;
		m_name = "RangeWidget" + std::to_string(s_instanceCount);

		m_littleStep = 1.f;
		m_bigStep = 10.f;
		m_minValue = 0.f;
		m_maxValue = 100.f;
		m_value = 0.f;

		m_orientation = RangeWidgetOrientation::OrientationHorizontal;
	}

	RangeWidget::~RangeWidget()
	{
		s_instanceCount--;
	}

	void RangeWidget::Configure(float _littleStep, float _bigStep, float _minValue, float _maxValue, float _value)
	{
		m_littleStep = _littleStep;
		m_bigStep = _bigStep;
		m_minValue = _minValue;
		m_maxValue = _maxValue;
		SetValue(_value);
	}

	void RangeWidget::SetLittleStep(float _step)
	{
		m_littleStep = _step;
	}

	void RangeWidget::SetBigStep(float _step)
	{
		m_bigStep = _step;
	}

	void RangeWidget::SetMinValue(float _value)
	{
		m_minValue = _value;
		if (m_value < m_minValue)
			m_value = m_minValue;
	}

	void RangeWidget::SetMaxValue(float _value)
	{
		m_maxValue = _value;
		if (m_value > m_maxValue)
			m_value = m_maxValue;
	}

	void RangeWidget::SetValue(float _value)
	{
		m_value = _value;
		if (m_value < m_minValue)
			m_value = m_minValue;
		else if (m_value > m_maxValue)
			m_value = m_maxValue;
	}

	void RangeWidget::SetOrientation(RangeWidgetOrientation _orientation)
	{
		m_orientation = _orientation;
	}

	void RangeWidget::SetOnValueChangeFct(const std::function<void(float)>& _fct)
	{
		m_onValueChangeFct = _fct;
	}

	float RangeWidget::GetLittleStep() const
	{
		return m_littleStep;
	}

	float RangeWidget::GetBigStep() const
	{
		return m_bigStep;
	}

	float RangeWidget::GetMinValue() const
	{
		return m_minValue;
	}

	float RangeWidget::GetMaxValue() const
	{
		return m_maxValue;
	}

	float RangeWidget::GetValue() const
	{
		return m_value;
	}

	RangeWidget::RangeWidgetOrientation RangeWidget::GetOrientation() const
	{
		return m_orientation;;
	}

	void RangeWidget::LittleIncrement()
	{
		m_value += m_littleStep;
		if (m_value > m_maxValue)
			m_value = m_maxValue;
	}

	void RangeWidget::LittleDecrement()
	{
		m_value -= m_littleStep;
		if (m_value < m_minValue)
			m_value = m_minValue;
	}

	void RangeWidget::BigIncrement()
	{
		m_value += m_bigStep;
		if (m_value > m_maxValue)
			m_value = m_maxValue;
	}

	void RangeWidget::BigDecrement()
	{
		m_value -= m_bigStep;
		if (m_value < m_minValue)
			m_value = m_minValue;
	}

	bool RangeWidget::HandleEvent(const sf::Event & _event)
	{
		return false;
	}

	void RangeWidget::HandleConfigurationChange()
	{
	}
}