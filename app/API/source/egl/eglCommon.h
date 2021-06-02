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
// DATE    : 7/22/2013 Mon
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __EGL_COMMON_H__
#define __EGL_COMMON_H__

#include "common/gl_Util.h"
#include "common/ErrorHandler.h"
#include "common/gl_Object.h"
#include "common/ProcTable.h"
#include "common/ElapsedTimeCheck.h"
#include "dll/MemDebug.h"

#include "EGL/egl.h"
#include "EGL/eglext.h"

namespace GL{
	extern EGLenum eglClinetAPI;
	extern ErrorHandler<EGLint, EGL_SUCCESS>	eglErrorHandler;
};

using namespace GL;
/*
extern "C" {
	// for linux dynamic handle error.
	extern void*	__dso_handle;
};
*/
#endif//__EGL_COMMON_H__
