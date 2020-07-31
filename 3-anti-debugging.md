# Anti-debug

This class will implement a relativley comprehensive list of anti-debugging techniques

## Proposed functionality

### Anti debugging techniques
 - Methods in github.com/zer0fl4g/DebugDetector
 - Methods in resources in above repository
 - Methods in github.com/revsic/AntiDebugging
 - Methods in github.com/cetfor/AntiDBG
 - All techniques in The Ultimate Anti-Reversing Reference pdf

### Extreme Anti-debugging
 - Inject into debuggers address space (Use frida to trace function call)
 - Make stepping decrease rip
 - When stack is printed, return 0xdeadbeef
 - When some commands are run, print "You don't have permission to perform this operation"
 - Open message box that says "Give up now"
 - Change labels in debug window
 - Hook attachment system calls, reject attachment
 - Redirect attachment to dummy process
 - Make disassembly look like it's movfuscated, or make it mis-read the strings
 - Sweep and remove breakpoints

 