#include <iostream>
#include "framework.h"

PCSTR cac_ip = "192.168.1.1";

int main() {
    std::cout << "Starting main\n";

    Process notepad = Memory::GetProcessByName("notepad.exe");
    Memory::GetDebugPrivileges();

    notepad.InjectDLL(DLL("PATH TO DLL"));

    if (AntiDebug::CheckIsDebuggerPresent())
        std::cout << "FOUND DEBUGGER\n";
    else
        std::cout << "No debugger found\n";

    exit(0);
}
