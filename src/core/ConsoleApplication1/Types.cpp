#include "Types.h"

DLL::DLL(std::string path) {
	this->path = path;
}

Shellcode::Shellcode(unsigned char* s) {
	this->shellcode = s;
}

unsigned char* Shellcode::get_char() {
	return this->shellcode;
}

int Shellcode::length() {
	return 504;
}