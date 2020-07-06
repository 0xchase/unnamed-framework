
#include <iostream>
#include <windows.h>
#include <psapi.h>
#include "Process.h"
#include "utils.h"

PCSTR cac_ip = "192.168.1.1";
Process processes[1024];

// Initialize the core module
int main()
{
    std::cout << "Hello World!\n";
    ScanProcesses();
    
}

int ScanProcesses(void)
{

    DWORD aProcesses[1024];
    DWORD cbNeeded;
    DWORD cProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        return 1;

    aProcesses[0];
    cProcesses = cbNeeded / sizeof(DWORD);

    for (i = 0; i < cProcesses; i++)
    {
        //processes[i] = new Process();
        std::cout << "Process ID: " << aProcesses[i] << "\n";
    }

    return 0;
}