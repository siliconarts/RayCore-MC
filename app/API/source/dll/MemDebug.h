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
// DATE    : 4/19/2013 Fri
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __MEM_DEBUG_H__
#define __MEM_DEBUG_H__
#include "STDInterface.h"

#if defined(WIN32) && (defined(DEBUG) || defined(_DEBUG))
	#define _CRTDBG_MAP_ALLOC
	#define _CRTDBG_MAP_ALLOC_NEW
	#include <crtdbg.h>

	#define CHECK_MEMORY_LEAK				_ASSERTE( _CrtCheckMemory( ) );
	#define SET_MEMORY_ASSERT_BREAK(idx)	_CrtSetBreakAlloc(idx)

	#if __cplusplus
		extern "C" {
	#endif
		void MemoryDebug_Initialize(void);
		void MemoryDebug_Uninitialize(void);
	#if __cplusplus
		};
	#endif
#else
	#define CHECK_MEMORY_LEAK
	#define SET_MEMORY_ASSERT_BREAK(idx)

// #if defined(USE_EGL_DEBUG) && defined(WIN32)	// for memory debugging
// #include "mmgr/mmgr.h"
// #endif
	#define MemoryDebug_Initialize()
	#define MemoryDebug_Uninitialize()
#endif


#endif //__MEM_DEBUG_H__
