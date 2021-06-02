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
// DATE    : 4/16/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#ifndef __RCDDK_DUMP_H__
#define __RCDDK_DUMP_H__
#include "STDInterface.h"

//-----------------------------------
// DUMP_HEADER
// DUMP_DESCRIPTION(#0)
//      dump binary(#0)
// DUMP_DESCRIPTION(#1)
//      dump binary(#1)
// ...
// DUMP_DESCRIPTION(#n)			// n(0 ~ (DUMP_HEADER.item_count-1))
//      dump binary(#n)
//-----------------------------------

typedef struct{
	DWORD	magic_code;			// 0xDEADDEEF
	DWORD	version;
	DWORD	item_count;
	struct{
		DWORD	dwAddress;
		WORD	width, height;
	}framebuffer;
	DWORD	reserved[6];
}DUMP_HEADER, *PDUMP_HEADER;

typedef struct{
	BOOL	bMemory;
	DWORD	dwAddress;
	DWORD	dwSize;
	DWORD	dwCompressedSize;	// if it's compressed with zlib, this value will be not a zero.
}DUMP_DESCRIPTION;

#endif//__RCDDK_DUMP_H__
