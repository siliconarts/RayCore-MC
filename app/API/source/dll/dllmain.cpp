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
// Project : API
// DATE    : 5/23/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "common/gl_Util.h"
#include "MemDebug.h"
#include <stdio.h>

HMODULE	g_hDll	= NULL;

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpRes)
{
	switch( dwReason )
	{
	case DLL_PROCESS_ATTACH:
		g_hDll	= hModule;
		MemoryDebug_Initialize();		// �޸� ����� �ʱ�ȭ
		SET_MEMORY_ASSERT_BREAK(-1);		// �����Ǵ� �޸� üũ ��ȣ�� ���⿡ �־� Ȯ���Ѵ�. (-1�� ������ Ȯ���� ���õȴ�.)
		break;
	case DLL_PROCESS_DETACH:
		MemoryDebug_Uninitialize();
		break;
	}
	return TRUE;
}
