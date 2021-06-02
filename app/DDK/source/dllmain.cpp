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
// Project : DDK
// DATE    : 5/24/2013 Fri
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "STDInterface.h"

BOOL WINAPI DllMain( HANDLE /*hInst*/, DWORD dwReason, LPVOID /*lpRes*/ )
{
	switch( dwReason ){
	case DLL_PROCESS_ATTACH:		break;
	case DLL_PROCESS_DETACH:		break;
	}
	return TRUE;
}
