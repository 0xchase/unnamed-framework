#include "Memory.h"

Memory::Memory() {
    ScanProcesses();
    GetDebugPrivileges();
}

void Memory::ScanProcesses(void) {

    DWORD aProcesses[1024];
    DWORD cbNeeded;
    DWORD cProcesses;
    unsigned int i;

    Process temp_processes[1024];
    //Process* temp_processes = new Process[1024];

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        return;

    aProcesses[0];
    cProcesses = cbNeeded / sizeof(DWORD);

    for (i = 0; i < cProcesses; i++)
    {
        temp_processes[i] = Process(aProcesses[i]);
        //std::cout << "Found process " << temp_processes[i].pid << " " << temp_processes[i].IsElevated() << "\n";
    }

    std::copy(temp_processes, temp_processes + sizeof(temp_processes) / sizeof(temp_processes[0]), processes);

    return;
}

Process Memory::GetProcessByName(std::string name) {
    for (auto process : processes) {
        if (process.name == name) {
            return process;
        }
    }

    return NULL;
}

Process Memory::GetProcessByPid(int pid) {
    for (auto process : processes) {
        if (process.pid == pid) {
            return process;
        }
    }

    return NULL;
}

BOOL Memory::GetDebugPrivileges(void) {
    HANDLE hToken = NULL;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
        return FALSE;

    if (!SetPrivilege(hToken, SE_DEBUG_NAME, TRUE))
        return FALSE;

    return TRUE;
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

