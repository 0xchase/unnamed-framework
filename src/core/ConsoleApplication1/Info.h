#pragma once
#include <Windows.h>

class Info
{
public:
	Info();
	struct HardwareInfo {
		DWORD pageSize;
		LPVOID minimumApplicationAddress;
		LPVOID maximumApplicationAddress;
		int numberOfProcessors;
		WORD processorLevel;
		WORD processorRevision;
	};

	HardwareInfo hardwareInfo;

private:
};

