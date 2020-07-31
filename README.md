# Unnamed Framework

A WIP framework for post exploitation, malware research, learning win32 and the native api, and more.

I have created a number of other `.md` files that propose various classes and functions.

## Current progress

 - Partial implementation of memory and process classes. Users can quickly search for processes, scan their memory, and do basic DLL injection. 
 - Implementation of a few anti-debug techniques in the antidebug class.

---

## Example

The goal of this framework is for people to be able to do things like below.

```
#include "unnamed-framework.h"

int main() {
    if (AntiDebug().CheckIsDebuggerPresent())
        exit(0);
	
    Memory memory = Memory();
    Process notepad = memory.GetProcessByName("notepad.exe");
    notepad.InjectDLL(DLL("C:\MaliciousDll.dll"));
	
    exit(0);
}
```

---

## Future projects/research

As the framework nears completion, it could be used for various projects like:
 - A challenge binary for anti-debugging techniques
 - Build more advanced meterpreter
 - Research which injection techniques are reliable and stealthy, evaluate against different anti-virus products
 - Usable for advanced game hacking
 - Extreme anti-debugging techniques (more in anti-debugging.md)
 
---

## Resources
 - Large list of malware sample source code: github.com/RamadhanAmizudin/malware
 - Interesting techniques in Springer "An emerging threat Fileless malware"
 
 