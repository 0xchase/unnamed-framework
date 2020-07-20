// MsgDLL.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "MsgDLL.h"


// This is an example of an exported variable
MSGDLL_API int nMsgDLL=0;

// This is an example of an exported function.
MSGDLL_API int fnMsgDLL(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CMsgDLL::CMsgDLL()
{
    return;
}
