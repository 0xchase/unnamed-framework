// TestDll.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "TestDll.h"
#include <iostream>
#include <Windows.h>
#include <Shellapi.h>


// This is an example of an exported variable
TESTDLL_API int nTestDll=0;

// This is an example of an exported function.
TESTDLL_API int fnTestDll(void)
{
    std::cout << "Calling fnTestDll in TestDLL\n";
    STARTUPINFO si = { 0 };
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi = { 0 };

    //CreateProcess(L"notepad.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

    ShellExecute(NULL, L"notepad.exe", NULL, NULL, NULL, SW_SHOW);

    return 0;
}

// This is the constructor of a class that has been exported.
CTestDll::CTestDll()
{
    return;
}
