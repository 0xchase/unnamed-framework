#include <iostream>
#pragma once

class DLL
{
public:
	std::string path;

	DLL(std::string path);

private:
};

class Shellcode
{
public:

	Shellcode(unsigned char* shellcode);
	unsigned char* get_char();
	int length();

private:
	unsigned char* shellcode;
};