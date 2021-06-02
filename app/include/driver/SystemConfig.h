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
// Project : RayCore
// DATE    : 4/18/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#ifndef __SYSTEM_CONFIG_H__
#define __SYSTEM_CONFIG_H__
#include "STDInterface.h"

typedef enum{
	SYSTEM_TYPE_EMULATION,			// S/W ���ķ��̼�
	SYSTEM_TYPE_SIMULATION,			// H/W �ùķ��̼�
	SYSTEM_TYPE_ALTERA,				// altera �ý���
	SYSTEM_TYPE_DYNALITH,			// dynalith �ý���
	SYSTEM_TYPE_SIZE
}SYSTEM_TYPE;

#ifdef USE_TESTDRIVE
typedef struct{
	DWORD				dwMemorySize;			// �޸� ũ��
	DWORD				dwSystemType;			// �ý��� Ÿ��
	BOOL				bUseTestDriveDisplay;	// ������ �ڵ� ���
	struct{
		BOOL			bOveride;
		int				iWidth;
		int				iHeight;
	}screen;
}SYSTEM_CONFIG;
#endif

#define	SYSTEM_NAME		"RayCore"
#define	DISPLAY_NAME	"RayCore_Display"

#endif//__SYSTEM_CONFIG_H__