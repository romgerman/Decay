#pragma once

#include <cstdlib>

struct CString
{
	char* value;
	size_t length;

	CString()
	{
		value = nullptr;
		length = 0;
	}

	CString(char* str)
	{
		value = str;
		length = _mbstrlen(str);
	}

	CString(const char* str)
	{
		value = const_cast<char*>(str);
		length = _mbstrlen(str);
	}

	CString(char* str, size_t length)
	{
		value = str;
		this->length = length;
	}

	operator char*()
	{
		return value;
	}
};
