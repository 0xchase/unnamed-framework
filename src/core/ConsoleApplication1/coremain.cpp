
#include <iostream>
#include <windows.h>
#include <psapi.h>
#include "Process.h"
#include "utils.h"

PCSTR cac_ip = "192.168.1.1";
Process processes[1024];

void ScanProcesses(void);

// Initialize the core module
int main()
{
    std::cout << "Hello World!\n";
    ScanProcesses();

    for (auto process : processes) {
        if (process.name == "notepad.exe") {
            std::cout << "Found notepad (pid " << process.pid << ")\n";
            process.scanStrings();
        }
    }
}

void ScanProcesses(void)
{

    DWORD aProcesses[1024];
    DWORD cbNeeded;
    DWORD cProcesses;
    unsigned int i;

    Process temp_processes[1024];

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        return;

    aProcesses[0];
    cProcesses = cbNeeded / sizeof(DWORD);

    for (i = 0; i < cProcesses; i++)
    {
        temp_processes[i] = Process(aProcesses[i]);
    }

    std::copy(temp_processes, temp_processes + sizeof(temp_processes) / sizeof(temp_processes[0]), processes);

    return;
}