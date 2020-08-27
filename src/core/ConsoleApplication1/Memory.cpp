#include "Memory.h"

std::vector<Process> Memory::GetProcesses(void) {

    DWORD aProcesses[1024];
    DWORD cbNeeded;
    DWORD cProcesses;
    unsigned int i;

    std::vector<Process> processes;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        throw std::exception("In Memory::GetProcesses(void):\t Failed to enumerate processes");

    aProcesses[0];
    cProcesses = cbNeeded / sizeof(DWORD);

    for (i = 0; i < cProcesses; i++)
    {
        processes.push_back(Process(aProcesses[i]));
    }

    return processes;
}

Process Memory::GetProcessByName(std::string name) {
    for (auto process : GetProcesses()) {
        if (process.name == name) {
            return process;
        }
    }

    return NULL;
}

Process Memory::GetProcessByPid(int pid) {
    for (auto process : GetProcesses()) {
        if (process.pid == pid) {
            return process;
        }
    }

    return NULL;
}

bool Memory::GetDebugPrivileges(void) {
    HANDLE hToken = NULL;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
        return false;

    if (!SetPrivilege(hToken, SE_DEBUG_NAME, TRUE))
        return false;

    return true;
}

BOOL Memory::SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege) {
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if (!LookupPrivilegeValue(
        NULL,
        lpszPrivilege,
        &luid))
    {
        printf("LookupPrivilegeValue error: %u\n", GetLastError());
        return FALSE;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if (bEnablePrivilege)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    if (!AdjustTokenPrivileges(
        hToken,
        FALSE,
        &tp,
        sizeof(TOKEN_PRIVILEGES),
        (PTOKEN_PRIVILEGES)NULL,
        (PDWORD)NULL))
    {
        printf("AdjustTokenPrivileges error: %u\n", GetLastError());
        return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

    {
        printf("The token does not have the specified privilege. \n");
        return FALSE;
    }

    return TRUE;
}