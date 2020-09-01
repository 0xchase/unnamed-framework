#pragma once

class Utilities {
public:
	static std::wstring string_to_wstring(std::string s);
	static LPVOID GetLibAddress(std::string s);
};