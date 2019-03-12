#pragma once

#include <vector>

template<typename T>
class CircularBuffer
{
public:
	CircularBuffer(size_t length = 0) : m_index(0)
	{
		resize(length);
	}

	void put(T item)
	{
		m_instance[m_index++] = item;

		if (m_index == m_instance.size())
			m_index = 0;
	}

	void resize(size_t length)
	{
		m_instance.resize(length, T());
		
		if (m_index >= length - 1)
			m_index = 0;
	}

	T& operator[] (size_t index)
	{		
		return m_instance[index];
	}

	T& current()
	{
		return m_instance[m_index];
	}

private:
	std::vector<T> m_instance;
	size_t m_index;
};
