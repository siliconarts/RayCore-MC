//=======================================================================
//
// This confidential and proprietary software may be used only as
// authorized by a licensing agreement from Siliconarts Inc.
//
//   (C) Copyright 2010 ~ 2013  Siliconarts Inc. ALL RIGHTS RESERVED
//
// The entire notice above must be reproduced on all authorized
// copies and copies may only be made to the extent permitted
// by a licensing agreement from Siliconarts.
//
// Project : DynalithSystem
// DATE    : 4/18/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "DynalithSystem.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:		break;
	case DLL_THREAD_ATTACH:			break;
	case DLL_THREAD_DETACH:			break;
	case DLL_PROCESS_DETACH:		break;
	}
	return TRUE;
}
