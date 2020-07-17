#include "Process.h"
#include <windows.h>
#include <Psapi.h>
#include <iostream>
#include <string>
#include <tchar.h>
#include <vector>
#include <algorithm>
#include <codecvt>

Process::Process() {
	pid = -1;
    name = "<unknown>";
    path = "<unknown>";
}

Process::Process(int pid_int) {
	pid = pid_int;
    name = "<unknown>";
    path = "<unknown>";
    Scan();
    //std::cout << name << "\n";
}

void Process::Scan() {
    std::string ret;
    HANDLE handle = OpenProcess(
        PROCESS_QUERY_LIMITED_INFORMATION,
        FALSE,
        pid
    );

    if (handle) {
        DWORD buffSize = 1024;
        CHAR buffer[1024];
        if (QueryFullProcessImageNameA(handle, 0, buffer, &buffSize)) {
            ret = buffer;
        } else {
            //printf("Error GetModuleBaseNameA : %lu", GetLastError());
        }

        CloseHandle(handle);

    } else {
        //printf("Error OpenProcess : %lu\n", GetLastError());
        return;
    }

    const auto pos = ret.find_last_of("\\");

    name = ret.substr(pos + 1);
    path = ret;

    //std::cout << "Scanned process " << name << " with pid " << pid << "\n";
};

void Process::ScanStrings() {
    HANDLE hProc;
    SYSTEM_INFO si;
    MEMORY_BASIC_INFORMATION mbi;
    LPVOID minAddress, maxAddress;

    GetSystemInfo(&si);
    minAddress = si.lpMinimumApplicationAddress;
    maxAddress = si.lpMaximumApplicationAddress;

    hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);

    if (!hProc) {
        printf("Opening process failed");
        return;
    }

    while (minAddress < maxAddress) {
        if (!VirtualQueryEx(hProc, minAddress, &mbi, sizeof(mbi)))
            printf("VirtualQueryEx() failed. %d\n", GetLastError());

        if (mbi.State == MEM_COMMIT && mbi.Protect == PAGE_READWRITE) {
            printf("Scanning readwrite region. baddr: 0x%p, size: 0x%x\n", mbi.BaseAddress, mbi.RegionSize);

            char* buffer = new char[mbi.RegionSize]{ 0 };
            SIZE_T bytesRead = 0;

            if (ReadProcessMemory(hProc, minAddress, buffer, mbi.RegionSize, &bytesRead)) {
                if (bytesRead) {
                    std::cout << " > Scanning memory " << bytesRead << "\n";
                    std::string str1(buffer);
                    char a[] = { 0xff };
                    std::string needle(a, a + 4);
                    std::string haystack(buffer, buffer + mbi.RegionSize);

                    std::size_t n = haystack.find(needle);

                    if (n != std::string::npos) {
                        std::cout << " > Found string\n";
                    }

                    //std::cout << "========================\n > Scan info: " << haystack.length() << "\n" << haystack << "\n========================\n";

                    // scan from buffer[0] to buffer[bytesRead]
                } else {
                    //scan from buffer[0] to buffer[mbi.RegionSize]
                }
            }
            delete[] buffer;

        } else if (mbi.State == MEM_COMMIT && mbi.Protect == PAGE_READONLY) {
            printf("Scanning readonly region.  baddr: 0x%p, size: 0x%x\n", mbi.BaseAddress, mbi.RegionSize);
            
        } else {
            printf("Skipping region at 0x%p\n", minAddress);
        }

        minAddress = (LPBYTE)mbi.BaseAddress + mbi.RegionSize;
    }


}

BOOL Process::GetDebugPrivileges(void) {
    HANDLE hToken = NULL;
    HANDLE handle = OpenProcess(
        PROCESS_QUERY_LIMITED_INFORMATION,
        FALSE,
        pid
    );
    if (!OpenProcessToken(handle, TOKEN_ADJUST_PRIVILEGES, &hToken))
        return FALSE;

    if (!SetPrivilege(hToken, SE_DEBUG_NAME, TRUE))
        return FALSE;

    return TRUE;
}

BOOL Process::SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege) {
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
        printf("SetPrivilege error: The token does not have the specified privilege. \n");
        return FALSE;
    }

    return TRUE;
}

BOOL Process::IsElevated(void) {
    BOOL fRet = FALSE;
    HANDLE hToken = NULL;

    HANDLE handle = OpenProcess(
        PROCESS_QUERY_LIMITED_INFORMATION,
        FALSE,
        pid
    );

    if (OpenProcessToken(handle, TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof(TOKEN_ELEVATION);
        if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
            fRet = Elevation.TokenIsElevated;
        }
    }

    if (hToken) {
        CloseHandle(hToken);
    }

    return fRet;
}

int Process::ReadInt(HANDLE processHandle, int address) {
    if (address == -1)
        return -1;
    int buffer = 0;
    SIZE_T NumberOfBytesToRead = sizeof(buffer);
    SIZE_T NumberOfBytesActuallyRead;
    BOOL success = ReadProcessMemory(processHandle, (LPCVOID)address, &buffer, NumberOfBytesToRead, &NumberOfBytesActuallyRead);
    if (!success || NumberOfBytesActuallyRead != NumberOfBytesToRead) {
        std::cout << "Memory Error!" << std::endl;
        return -1;
    }
    
    return buffer;
}

int Process::GetPointerAddress(HANDLE processHandle, int startAddress, int offsets[], int offsetCount) {
    if (startAddress == -1)
        return -1;
    int ptr = ReadInt(processHandle, startAddress);
    for (int i = 0; i < offsetCount - 1; i++) {
        ptr += offsets[i];
        ptr = ReadInt(processHandle, ptr);
    }
    ptr += offsets[offsetCount - 1];
    return ptr;
}

int Process::ReadPointerInt(HANDLE processHandle, int startAddress, int offsets[], int offsetCount) {
    if (startAddress == -1)
        return -1;
    return ReadInt(processHandle, GetPointerAddress(processHandle, startAddress, offsets, offsetCount));
}

float Process::ReadFloat(HANDLE processHandle, int address) {
    if (address == -1)
        return -1;
    float buffer = 0.0;
    SIZE_T NumberOfBytesToRead = sizeof(buffer);
    SIZE_T NumberOfBytesActuallyRead;
    BOOL success = ReadProcessMemory(processHandle, (LPCVOID)address, &buffer, NumberOfBytesToRead, &NumberOfBytesActuallyRead);
    if (!success || NumberOfBytesActuallyRead != NumberOfBytesToRead)
        return -1;
    return buffer;
}

float Process::ReadPointerFloat(HANDLE processHandle, int startAddress, int offsets[], int offsetCount) {
    if (startAddress == -1)
        return -1;
    return ReadFloat(processHandle, GetPointerAddress(processHandle, startAddress, offsets, offsetCount));
}

std::string Process::ReadText(long long address) {
    if (address == -1)
        return NULL;
    
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (!processHandle) {
        std::cout << "Failed to open process: " << GetLastError() << "\n";
    }

    char buffer = !0;
    char* stringToRead = new char[128];
    SIZE_T NumberOfBytesToRead = sizeof(buffer);
    SIZE_T NumberOfBytesActuallyRead;
    int i = 0;

    DWORD oldProtect = 0;

    if (!VirtualProtectEx(processHandle, (LPVOID) address, 8, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        std::cout << "Failed to change permissions: " << GetLastError() << " from " << oldProtect << "\n";
    }

    while (buffer != 0) {

        BOOL success = ReadProcessMemory(processHandle, (LPCVOID)address, &buffer, NumberOfBytesToRead, &NumberOfBytesActuallyRead);
        if (!success) {
            std::cout << "Failed here: " << GetLastError() << "\n";
        }
        if (!success || NumberOfBytesActuallyRead != NumberOfBytesToRead) {
            return NULL;
        }
        stringToRead[i] = buffer;
        i++;
        address++;
        
        if (buffer == 0)
            std::cout << "Reached zero in buffer\n";
    }

    std::string ret = stringToRead;
    return ret;
}

std::string Process::ReadPointerText(HANDLE processHandle, int startAddress, int offsets[], int offsetCount) {
    if (startAddress == -1)
        return NULL;
    return ReadText(GetPointerAddress(processHandle, startAddress, offsets, offsetCount));
}


