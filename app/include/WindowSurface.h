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
// DATE    : 3/22/2013 Fri
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __WINDOW_SURFACE_H__
#define __WINDOW_SURFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct EGL_WINDOW_SURFACE_t{
	void (*GetWindowSize)(int& width, int& height);
}EGL_WINDOW_SURFACE, *LPEGL_WINDOW_SURFACE;

#ifdef __cplusplus
}
#endif

#endif//_SYSTEM_HAL_H_
