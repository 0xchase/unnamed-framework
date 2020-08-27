#include "Process.h"

class AntiDebug
{
public:
	static bool CheckAll();

	// github.com/cetfor/AntiDBG
	//bool CheckBeingDebuggedPEB();

	static bool CheckIsDebuggerPresent();
	static bool CheckIsRemoteDebuggerPresent();
	static bool CheckProcessNames(Process* p);
	static bool CheckWindowName();
	static bool CheckHardwareDebugRegisters();
	static bool CheckParentProcess();
	static bool CheckNtGlobalFlag();
	static bool CheckExceptionHandled();
	static bool CheckCloseHandleException();
	static bool CheckDebugString();
	static bool CheckCsrGetProcessId();

	/*
	bool CheckNtQueryInformationProcess();
	bool CheckNtSetInformationThread();
	//bool CheckDebugActiveProcesses(const cahr* cpid);
	bool CheckRDTSC();
	bool CheckPerformanceCounter();
	bool CheckTickCount();
	bool CheckMovSS();
	bool CheckSingleStepException();
	bool CheckInt3();
	bool CheckPrefixHop();
	bool CheckInt2D();
	*/

	// Enumerate installed applications
	// Check if it's a virtual machine


private:

};

