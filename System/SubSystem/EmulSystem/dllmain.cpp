// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore DDK v 1.0
// Date : 

#include "stdafx.h"
#include "EmulSystem.h"

#include <stdio.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		break;
	case DLL_THREAD_ATTACH:			break;
	case DLL_THREAD_DETACH:			break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

ISystemImp* CreateSystemImplementation(void){
	return new EmulSystem();
}