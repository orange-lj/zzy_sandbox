#include<iostream>
using namespace std;

#include"Start.h"
#include"ParseCMD.h"


int main() 
{
	int ExitCode = 1;
	BOOL bRet = FALSE;
	Start CMain;

	bRet = ParseCMD::GetInstance().ParseCommandLine();
	
	bRet = ParseCMD::GetInstance().StartCtrl();
	//Æô¶¯PBSrv.exe·þÎñ
	ParseCMD::GetInstance().INIGetConf(L"StartServices");
	return 0;
}