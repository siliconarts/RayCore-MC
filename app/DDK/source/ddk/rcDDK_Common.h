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
// DATE    : 6/24/2013 Mon
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#ifndef __RCDDK_COMMON_H__
#define __RCDDK_COMMON_H__
#include <assert.h>
#include "ddk/rcDDK.h"
#include "driver/SystemHAL.h"
#include "RegisterChain.h"

#include "rc_util.h"
// @todo : 없애야 할 것...	: SystemHAL.h 가 이를 대체한다.
#include "RC1/rc_structs.h"
#include "driver/RCStructures.h"

#ifdef USE_TESTDRIVE
#include "TestDriver.h"
#include "TestDrive/TD_Regmap.h"
#include "VirtualDisplayConfig.h"
#endif

// memories group
typedef union{
	IMemory*	pMemory[6];
	struct{
		IMemory*	pNode;				// node buffer
		IMemory*	pList;				// list buffer
		IMemory*	pPrimitive;			// primitive buffer
		IMemory*	pPrimitiveInfo;		// primitive information
		IMemory*	pTransmit;			// transmittance
		IMemory*	pFramebuffer;		// frame back-buffer
	};
}RCDDK_MEMORIES;

#define MAX_DEVICE_COUNT	16			// maximum multi-card support count

typedef struct{
	DWORD				dwCardCount;
	
	struct{
		DWORD			width, height;
	}Screen;

	struct{
		float			width, height;
	}PixelSize;
	
	struct{
		float			x,y;
	}StartPosition;

	struct{									// card performance analysis
		DWORD			dwScale;			// clock scale
		DWORD			dwElapse;			// clock * clcok scale
		DWORD			dwBlocks;			// rendered blocks (height = blocks * 16)
	}Performance[MAX_DEVICE_COUNT];
}MULTI_LOAD;

// I/O function group
void IO_Write(DWORD dwOffset32, DWORD* pData, int iCount);
void IO_WriteControl(DWORD dwOffset32, DWORD* pData, int iCount);

typedef struct{
	RegChainT<RC_REG_OFFSET_CONFIG, REG_CONFIG, IO_Write>			Config;								// configuration
	RegChainT<RC_REG_OFFSET_CONTROL, REG_CONTROL, IO_WriteControl>	Control;							// control
	RegChainT<RC_REG_OFFSET_OPTION, REG_OPTION, IO_Write>			Option;								// option
	RegChainT<RC_REG_OFFSET_AST, REG_AST, IO_Write>					AST;								// AST
	RegChainT<RC_REG_OFFSET_LIGHT, REG_LIGHT, IO_Write>				Light[RCDDK_MAX_LIGHT_COUNT];		// light
	RegChainT<RC_REG_OFFSET_MATERIAL, REG_MATERIAL, IO_Write>		Material[RCDDK_MAX_MATERIAL_COUNT];	// materials
}RCDDK_REGISTERS;

#endif//__RCDDK_COMMON_H__
