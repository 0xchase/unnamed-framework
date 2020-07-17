#pragma once
#include <windows.h>
#include <string>

class Process
{
public:
	int pid;
	std::string name;
	std::string path;

	Process();
	Process(int pid_num);

	void Scan();
	void ScanStrings();

	BOOL GetDebugPrivileges(void);
	BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
	BOOL IsElevated();

    int ReadInt(HANDLE processHandle, int address);
    int GetPointerAddress(HANDLE processHandle, int startAddress, int offsets[], int offsetCount);
    int ReadPointerInt(HANDLE processHandle, int startAddress, int offsets[], int offsetCount);
    float ReadFloat(HANDLE processHandle, int address);
    float ReadPointerFloat(HANDLE processHandle, int startAddress, int offsets[], int offsetCount);
    std::string ReadText(long long address);
    std::string ReadPointerText(HANDLE processHandle, int startAddress, int offsets[], int offsetCount);

	bool InjectDllNewThread();
	bool InjectDll();

private:
};
