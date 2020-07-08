#include "Process.h"
#include <windows.h>
#include <Psapi.h>
#include <iostream>
#include <string>
#include <tchar.h>

Process::Process() {
	pid = -1;
    name = "<unknown>";
    path = "<unknown>";
}

Process::Process(int pid_int) {
	pid = pid_int;
    name = "<unknown>";
    path = "<unknown>";
    scan();
}

// Use VirtualQueryEx to figure out what parts of memory are in use
// Function to collect strings. Function to replace strings. 
// View heap memory
void Process::scanStrings() {
    constexpr unsigned CHUNK_SIZE = 0x100000;
    constexpr unsigned MAX_ADDRESS = 0x7fffffff;

    HANDLE handle = OpenProcess(
        PROCESS_QUERY_LIMITED_INFORMATION,
        FALSE,
        pid
    );

    CHAR buffer[CHUNK_SIZE];

    for (unsigned i = 0; i < MAX_ADDRESS; i += CHUNK_SIZE) {
        if (ReadProcessMemory(handle, (LPVOID)i, buffer, sizeof(buffer), nullptr)) {
            for (int j = 0; j <= CHUNK_SIZE - sizeof(int); ++j) {
                int something;
                memcpy(&something, buffer + j, sizeof(int));
                
            }
        }
    }
}

void Process::scan() {
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
