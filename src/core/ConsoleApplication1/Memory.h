#pragma once
#include "Process.h"
#include <Psapi.h>
#include <vector>

class Memory
{
public:
	static bool GetDebugPrivileges(void);
	static std::vector<Process> GetProcesses(void);
	static Process GetProcessByName(std::string name);
	static Process GetProcessByPid(int pid);

private:
	static BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
};

