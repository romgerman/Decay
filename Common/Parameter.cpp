#include "Parameter.h"

void Parameter::SetValue(float value)
{
	m_value = ClampValue(value, m_min, m_max);
}

void Parameter::SetNormalizedValue(float value)
{
	m_value = m_min + ClampValue(value, 0.f, 1.f) * (m_max - m_min);
}

void Parameter::SetInternalValue(int value)
{
	SetNormalizedValue(value / (float)MAX_PARAM_VALUE);
}

float Parameter::GetValue() const
{
	return m_value;
}

int Parameter::GetInternalValue() const
{
	return (int)(GetNormalizedValue() / (float)MAX_PARAM_VALUE);
}

float Parameter::GetNormalizedValue() const
{
	return (m_value - m_min) / (m_max - m_min);
}

CString Parameter::GetName() const
{
	return m_name;
}

CString Parameter::GetUnits() const
{
	return m_units;
}

int Parameter::GetIndex() const
{
	return m_index;
}
