// Add utility to get handle to function from ntdll to 
#include <Windows.h>
#include <iostream>
#include "Utilities.h"

std::wstring Utilities::string_to_wstring(std::string s) {
	int len;
	int slength = (int) s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	std::wstring ret = r;
	return ret;
}

LPVOID Utilities::GetLibAddress(std::string s) {
	HMODULE hKernel32 = GetModuleHandle(L"kernel32.dll");
	if (hKernel32 == NULL) {
		std::cout << "Failed to get handle to kernel32.dll\n";
		return nullptr;
	}

	LPVOID llBaseAddress = (LPVOID)GetProcAddress(hKernel32, s.c_str());
	if (llBaseAddress == NULL) {
		std::cout << "Failed to get LoadLibraryW base address\n";
		return nullptr;
	}
}