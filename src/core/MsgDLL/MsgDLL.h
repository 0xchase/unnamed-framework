// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the MSGDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// MSGDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MSGDLL_EXPORTS
#define MSGDLL_API __declspec(dllexport)
#else
#define MSGDLL_API __declspec(dllimport)
#endif

// This class is exported from the dll
class MSGDLL_API CMsgDLL {
public:
	CMsgDLL(void);
	// TODO: add your methods here.
};

extern MSGDLL_API int nMsgDLL;

MSGDLL_API int fnMsgDLL(void);
