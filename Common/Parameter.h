#pragma once

#include "CString.h"

// min = 0, max = 65536

template<typename T>
T ClampValue(T val, T min, T max)
{
	if (val > max)
		return max;
	else if (val < min)
		return min;

	return val;
}

const int MAX_PARAM_VALUE = 65536;

class Parameter
{
public:
	Parameter() : Parameter(0, "") {}
	Parameter(int index, const char* name, float value = 0, const char* units = "", float min = 0, float max = 1)
	{
		m_name = name;
		m_units = units;
		m_min = min;
		m_max = max;
		m_index = index;
		SetValue(value);
	}

	void SetValue(float value);
	void SetNormalizedValue(float value);
	void SetInternalValue(int value);
	// Real value
	float GetValue() const;
	// FL Studio internal value
	int GetInternalValue() const;
	// Value between 0.0 - 1.0
	float GetNormalizedValue() const;
	CString GetName() const;
	CString GetUnits() const;
	int GetIndex() const;

private:
	float m_value;
	float m_min;
	float m_max;
	CString m_name;
	CString m_units;
	int m_index;
};
