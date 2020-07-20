# Disk

This class is for functionality related to file/applications on the disk.

## Proposed functionality
 - Easy way to enumerate the file system (for example, using for-each loops)
	- Files in enumeration have types (like DLL, script, etc) that all for easy calls like .injectPayload(), delete(), copyto(), moveto(), etc
 - Enumerate installed apps (via win32 api)
	- Calls to uninstall, etc for each application
	- List application versions, patches, etc