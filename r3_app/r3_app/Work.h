#pragma once

#include"Config.h"
#include"CLoadDriver.h"

#define ProteinBoxDrv_LinkName	"hyDrv"
extern CDriver* g_drv_ProteinBoxDrv;
extern CConfigEx* g_Conf;

BOOL HandlerConf();
BOOL LoadDriver(IN LPSTR szDriverPath, IN LPSTR szDriverLinkName, CDriver** drv);

CDriver* InitDriver(IN LPSTR szDriverPath,IN LPSTR szDriverLinkName);