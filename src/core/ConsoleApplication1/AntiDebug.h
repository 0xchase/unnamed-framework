#include "Process.h"

class AntiDebug
{
public:
	bool exitImmediatley;

	AntiDebug(bool exitImmediatley);

	bool CheckAll();

	// github.com/cetfor/AntiDBG
	//bool CheckBeingDebuggedPEB();

	bool CheckIsDebuggerPresent();
	bool CheckIsRemoteDebuggerPresent();
	bool CheckProcessNames(Process* p);
	bool CheckWindowName();
	bool CheckHardwareDebugRegisters();
	bool CheckParentProcess();
	bool CheckNtGlobalFlag();
	bool CheckExceptionHandled();
	bool CheckCloseHandleException();
	bool CheckDebugString();
	bool CheckCsrGetProcessId();

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

