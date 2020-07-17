#include "Info.h"
#include <Windows.h>
#include <iostream>

#ifndef UNICODE
typedef std::string String;
#else
typedef std::wstring String;
#endif

Info::Info() {
	std::cout << "Getting system info:\n";

	TCHAR computerName[128 + 1];
	DWORD size = sizeof(computerName) / sizeof(computerName[0]);
	GetComputerName(computerName, &size);
	std::wstring temp = computerName;
	std::cout << "Host name: " << temp.c_str() << "\n";
	//GetComputerName(buffer, 128);

	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	hardwareInfo.pageSize = sysinfo.dwPageSize;
	hardwareInfo.numberOfProcessors = sysinfo.dwNumberOfProcessors;
	hardwareInfo.maximumApplicationAddress = sysinfo.lpMaximumApplicationAddress;
	hardwareInfo.minimumApplicationAddress = sysinfo.lpMinimumApplicationAddress;
	hardwareInfo.processorLevel = sysinfo.wProcessorLevel;
	hardwareInfo.processorRevision = sysinfo.wProcessorRevision;

}