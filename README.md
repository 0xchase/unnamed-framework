# malware-framework
A WIP framework for learning malware research, mitigation, and development.

## Todo
 - Add functionality ideas from Windows System Programming book
 - Add disk class
 - Add network class
 - Add payload class
 - Add application class, enumerate and create list in disk class
 - Add anti-debug class
 - Implement techniques in mitre attack framework
 - Python API
 - Metasploit integration???
 - Features from post-exploitation frameworks here: awesomeopensource.com/projects/post-exploitation

---

# Design

## Core
 - Contains some key functionality, local cmdline interface, networked cmdline interface
 - Contains an array of plugin objects
 - Make it useable as a library, to make writing mini malwares simple
 
### Process Manipulation
 - Get basic process information
 - Implement various memory scanning techniques, rewrite strings in memory
 - Suspend, resume, dump, and kill process
 - Implement various process injection techniques: github.com/SafeBreach-Labs/pinjectra and the corresponding whitepaper. Detailed comments.
 - Implement various process hiding techniques
 - Implement DLL injection/hijacking techniques
 - Inject code into files on disk
 - Visualize heap memory
 - Monitor system calls
 - Build hooks for various system calls, can enable these hooks
 - Create custom process classes that extend process, like edge, tskmgr, chrome, explorer with custom injections, manipulations, etc
 
### Code injection
 - Process spawning techniques (start legit executable, but modify before it starts running) (noisy, not stealthy)
 - Inject during process initialization (requires UAC elevation, mitigated by Extension Point Disable Policy)
 - Alter files/DLLs on disk
 - True process injection
   - Work on techniques in github injectAllTheThings

### Info
 - Gather system information, ip address, etc
 - Gather hashes, network passwords, etc
 - Enumerate applications and versions
 - Keylogger (user mode or kernel mode)
 - Enable mimikatz vuln, then on restart, run mimikatz to get passwords
 - Get all information from docs.microsoft.com/.../system-information-functions

### Networking
 - Scan network for other hosts
 - Create a proxy into the network
 - Create netcat reverse shell, bind shell, metasploit reverse shell, custom command line reverse shell
 - Send information back and forth
 - Wireshark like network capture
 - Use of tor network
 - Capture windwos passwords on the network Penetration Testing on Windows book

### Persistance
 - Various user mode techniques
 - Various kernel mode techniques
 - Advanced techniques: overwrite firmware
 
### Anti-forensics
 - Various techniques for hiding from forensic tools
 - Avoid writing to disk as much as possible
 - Document in code what traces each function leaves. Can print a report of this.
 - Detect memory dump or virus scan. Immediatley obfuscate or kill processes
 
### Disk
 - Functions and data structures for iterating through directories, finding files
 - Functions and data structures for application objects

### Applications
 - Can list applications, updates, patches, add/remove patches and more
 
### Anti-debug
 - Methods in github.com/zer0fl4g/DebugDetector
 - Methods in resources in above repository
 - Methods in github.com/revsic/AntiDebugging
 - Methods in github.com/cetfor/AntiDBG
 - Mess with debugger process if is detected: inject into address space
 - Mess with debugger installation if detected: inject into installation, make it crash if opens malware
 - All techniques in The Ultimate Anti-Reversing Reference pdf
 - Create a list of anti-debug reversing challenges
 
### Anti-analysis
 - Load libraries dynamically instead of using imports

## Powershell
 - Find powershell for penetration testers book
 - Can enable powershell, check if enabled
 - Store a set of powershell commands in memory
 - Can run arbitrary powershell commands, return status code as integer and output as std::string array
 - Find penetration testing with powershell book
 - Best Resources:
   - PowerSploit
   - Empire Framework??? Says better projects exist
   - Silver 
 - Other resources:
   - Everything in github.com/samratashok/nishang
   - Empire framework
   - Use PsInject (By EmpireProject github) to inject powershell into any process
   - Apfell, Covenant, 
 
---

## Plugins
 - Plugins can be written in C, C++, Python, Rust, Powershell
 - Plugins can be user mode, kernel mode, etc

# Resources
 - Large list of malware sample source code: github.com/RamadhanAmizudin/malware
 - Interesting techniques in Springer "An emerging threat Fileless malware"
 
---

## Privilege Escalation

### Methods
 - Online OSCP guides for privilege escalation
 - Inject code into file on disk
 - Various DLL injection methods
 - Privesc using process injectioncc
   - ???

### Payloads
 - Start a process at max privilege level
 