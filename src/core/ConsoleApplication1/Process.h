#pragma once
#include <windows.h>
#include <string>

class Process
{
public:
	int pid;
	std::string name;
	std::string path;

	Process();
	Process(int pid_num);

	void scan();
	void scanStrings();
private:
};