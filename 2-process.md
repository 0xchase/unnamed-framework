# Process

This class provides functions for creating, querying, and manipulating processes.

## Proposed functionality

### Basics
 - Get processes by name or pid
 - Print other information about process
 - Pause, resume, kill, and restart process
 - Return corresponding program on the disk

### Memory manipulation
 - Functions to search for strings, integers, etc in memory
 - Funtions to write data to memory, replace strings in memory, etc

### DLL injection
 - Implement all/most techniques
 - github.com/SafeBreach-Labs/pinjectra and the corresponding whitepaper
 - This method: https://github.com/D4stiny/ThreadJect/blob/master/ThreadJect/main.cpp
 
### Shellcode injection
 - Write shellcode to memory
 - Trampolines
 - https://googleprojectzero.blogspot.com/2018/10/injecting-code-into-windows-protected.html
 - Inject into kernel process
 
### Hooking
 - Todo

### Other
 - Visualize heap memory
 - Monitor system calls
 - Create custom process classes that extend process, like edge, tskmgr, chrome, explorer with custom injections, manipulations, etc
 
