#include "String.h"
#include "String.h"
#include <cstdlib>

wchar_t* CharToWchar(const char* source, size_t length)
{
	wchar_t* dest = new wchar_t[length + 1];
	mbstowcs(dest, source, length + 1);
	return dest;
}
