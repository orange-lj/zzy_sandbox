#pragma once
#include<windows.h>

class ParseCMD
{
public:
	static ParseCMD& GetInstance() 
	{
		static ParseCMD _instance;
		return _instance;
	}
	ParseCMD();
	virtual ~ParseCMD();

	BOOL ParseCommandLine();
	BOOL StartCtrl();
	LPWSTR GetCommandLineWEx(IN LPWSTR lpCmdLine);
	BOOL INIGetConf(IN LPWSTR Key);
};

