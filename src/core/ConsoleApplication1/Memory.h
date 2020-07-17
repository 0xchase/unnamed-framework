#pragma once
#include "Process.h"
#include <Psapi.h>

class Memory
{
public:
	Process processes[1024];

	Memory();

	void ScanProcesses(void);
	BOOL GetDebugPrivileges(void);
	Process GetProcessByName(std::string name);
	Process GetProcessByPid(int pid);

private:
	BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);

};

