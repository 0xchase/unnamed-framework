#include "AntiDebug.h"
#include <Windows.h>
#include <tlhelp32.h>
#include <winternl.h>
#include "Process.h"
#include <iostream>
#include <Psapi.h>

bool AntiDebug::CheckAll() {
	return AntiDebug::CheckIsRemoteDebuggerPresent();
}

bool AntiDebug::CheckIsDebuggerPresent() {
	bool found = false;
	found = IsDebuggerPresent();
	return found;
}

bool AntiDebug::CheckIsRemoteDebuggerPresent() {
	BOOL found = FALSE;
	
	CheckRemoteDebuggerPresent(GetCurrentProcess(), &found);

	if (found)
		return true;
	else
		return false;
}

// --------------------------------------------------------------

// Add more process names
bool AntiDebug::CheckProcessNames(Process* p) {
	std::string names[2] = {"ollydbg", "WinDbg"};
	for (int i = 0; i < 1024; i++) {
		for (std::string name : names)
			if (p[i].name.find(name) != std::string::npos)
				return true;
	}
	return false;
}

 // DOESN'T WORK???
bool AntiDebug::CheckWindowName() {
	return FindWindow(L"WinDbg Preview (4)", 0) || FindWindow(L"OLLYDBG", 0) || FindWindow(L"IDA v7.0.190307 hmmm", 0);
}

bool AntiDebug::CheckHardwareDebugRegisters() {
	CONTEXT ctx = { 0 };
	HANDLE hThread = GetCurrentThread();

	ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

	if (GetThreadContext(hThread, &ctx))
		if ((ctx.Dr0 != 0x00) || (ctx.Dr1 != 0x00) || (ctx.Dr2 != 0x00) || (ctx.Dr3 != 0x00) || (ctx.Dr6 != 0x00) || (ctx.Dr7 != 0x00))
			return true;
	
	return false;
}

int GetProcessName(DWORD PID, char* buff, int size) {
	int len = 0;
	HANDLE hProc = NULL;

	if ((hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, PID)) != NULL) {
		len = GetModuleBaseNameA(hProc, NULL, (LPSTR)buff, size);
		CloseHandle(hProc);
	}
		
	return len;
}

bool AntiDebug::CheckParentProcess() {
	std::string debuggerNames[] = { "radare2.exe", "ollydbg.exe", "ida.exe", "ida64.exe", "idag.exe", "idag64.exe", "idaw.exe", "idw64.exe", "idaq.exe", "idaq64.exe", "idau.exe", "idau64.exe", "scylla.exe", "scylla_x64.exe", "scylla_x86.exe", "protection_id.exe", "x64dbg.exe", "x32dbg.exe", "windbg.exe", "windbg.exe" };
	int pid = -1, len;

	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe = { 0 };
	char name[100];
	pe.dwSize = sizeof(PROCESSENTRY32);
	pid = GetCurrentProcessId();

	if (Process32First(handle, &pe)) {
		do {
			if (pe.th32ProcessID == pid) {
				break;
			}
		} while (Process32Next(handle, &pe));
	}

	CloseHandle(handle);
	len = GetProcessName(pe.th32ParentProcessID, name, sizeof(name) - 1);
	name[len] = 0;
	std::string stringfied(name);
	for (int i = 0; i < (sizeof(debuggerNames) / sizeof(*debuggerNames)); i++) {
		if (debuggerNames[i] == stringfied)
			return true;
	}

	return false;
}

// Doesn't work
bool AntiDebug::CheckNtGlobalFlag() {
	typedef unsigned long(__stdcall* pfnZwQueryInformationProcess)
		(
			IN HANDLE,
			IN unsigned int,
			OUT PVOID,
			IN ULONG,
			OUT PULONG
		);
	pfnZwQueryInformationProcess ZwQueryInfoProcess = NULL;
	HMODULE hNtDll = LoadLibrary(TEXT("ntdll.dll"));

	if (hNtDll == NULL) {
		std::cout << "Antidebug failed to get ntdll.dll\n";
		return false;
	}

	ZwQueryInfoProcess = (pfnZwQueryInformationProcess)GetProcAddress(hNtDll, "ZwQueryInformationProcess");
	
	if (ZwQueryInfoProcess == NULL) {
		std::cout << "Antidebug failed to get ZwQueryInformationProcess\n";
		return false;
	}

	unsigned long status;

	DWORD pid = GetCurrentProcessId();

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	PROCESS_BASIC_INFORMATION pbi;
	status = ZwQueryInfoProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), NULL);

	PPEB peb_addr = pbi.PebBaseAddress;
	DWORD ptr = (DWORD) pbi.PebBaseAddress;
	ptr |= 104;
	DWORD temp = ptr;
	std::cout << "Value of temp is: " << temp << " " << ptr << "\n";

	return false;
}

int x = 0, y = 0, noDbg = 0;
LONG WINAPI catchException(struct _EXCEPTION_POINTERS* excInfo) {
	noDbg = 1;
	y++;
	return EXCEPTION_CONTINUE_EXECUTION;
}

bool AntiDebug::CheckExceptionHandled() {
	SetUnhandledExceptionFilter(catchException);
	x = x / y;

	if (!noDbg)
		return true;
	return false;
}

bool AntiDebug::CheckCloseHandleException() {
	HANDLE hInvalid = (HANDLE)0xDEADBEEF;
	bool found = false;

	__try {
		CloseHandle(hInvalid);
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		return true;
	}

	return false;
}

// DOESN'T WORK
bool AntiDebug::CheckDebugString() {
	DWORD val = 123;
	SetLastError(val);

	OutputDebugString(L"Hello, debugger");

	if (GetLastError() == val)
		return true;

	return false;
}

// DOESN'T WORK
bool AntiDebug::CheckCsrGetProcessId() {
	typedef HANDLE (*_CsrGetProcessId)();
	HMODULE nt = GetModuleHandle(L"ntdll.dll");
	_CsrGetProcessId CsrGetProcessId = (_CsrGetProcessId)GetProcAddress(nt, "CsrGetProcessid");
	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD) CsrGetProcessId());

	if (!proc)
		return true;

	return false;
}