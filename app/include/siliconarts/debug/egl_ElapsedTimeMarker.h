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
// DATE    : 7/22/2013 Mon
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#ifndef __EGL_ELAPSED_TIME_MARKER_H__
#define __EGL_ELAPSED_TIME_MARKER_H__
#include <EGL/egl.h>

#define EGL_ELAPSED_TIME_CHECK_FORCED	0	// set to 1 this value, if you want to check elapsed time forced.

typedef enum{
	EGL_ELAPSED_TIME_MARKER_RENDER,			// only h/w rendering time
	EGL_ELAPSED_TIME_MARKER_TREE_BUILD,		// tree build time
	EGL_ELAPSED_TIME_MARKER_TOTAL,			// total render time
}EGL_ELAPSED_TIME_MARKER;

typedef float (EGLAPIENTRY *PFNEGLGETELAPSEDTIMEPROC) (EGL_ELAPSED_TIME_MARKER id);
#define GET_EGLGETELAPSEDTIMEPROC	(PFNEGLGETELAPSEDTIMEPROC)eglGetProcAddress("eglGetElapsedTime")

#endif
