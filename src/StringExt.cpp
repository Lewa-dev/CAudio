#include "StringExt.h"
#include <Windows.h>//used for wstring <> utf8 conversion
#include <stringapiset.h>

std::wstring StringExt::to_wstring(const std::string& strUtf8)
{
	int length = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), strUtf8.length(), NULL, 0);

	if (length == 0)
		return std::wstring();

	std::wstring wstr;
	wstr.resize(length);
	MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), strUtf8.length(), &wstr[0], length);

	return wstr;
}
