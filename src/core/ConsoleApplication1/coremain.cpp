#include <iostream>
#include <windows.h>
#include "Process.h"
#include "Memory.h"
#include "AntiDebug.h"
#include "utils.h"
#include "Info.h"

PCSTR cac_ip = "192.168.1.1";

int main() {
    std::cout << "Starting main\n";

    Memory memory = Memory();
    //Info info = Info();
    AntiDebug antidebug = AntiDebug(false);

    memory.GetDebugPrivileges();
    Process notepad = memory.GetProcessByName("notepad.exe");

    std::cout << "Inejcting DLL\n";
    notepad.InjectDll();
    


    if (antidebug.CheckIsDebuggerPresent())
        std::cout << "FOUND DEBUGGER\n";
    else
        std::cout << "No debugger found\n";


    exit(0);
}
