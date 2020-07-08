# malware-framework
A WIP framework for learning malware development and detection.

## Todo
 - Add functionality ideas from Windows System Programming book
 - Process class
   - Scan for processes, adding to global array
   - Print process information
   - Kill process
   - Monitor process
 - Python script to generate writeups from code base

---

# Design

## Core
 - Contains some key functionality, local cmdline interface, networked cmdline interface
 - Contains an array of plugin objects
 
### Process Manipulation
 - Get basic process information
 - Implement various memory scanning techniques, rewrite strings in memory
 - Suspend and resume process execution
 - Implement various process injection techniques: github.com/SafeBreach-Labs/pinjectra and the corresponding whitepaper. Detailed comments.
 - Implement various process hiding techniques
 - Visualize heap memory
 - Monitor system calls
 - Build hooks for various system calls, can enable these hooks

## Plugins
 - Plugins can be written in C, C++, Python, Rust, Powershell
 - Plugins can be user mode, kernel mode, etc

# Resources
 - Mirai code: github.com/0x27/linux.mirai
 