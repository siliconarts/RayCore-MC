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
// Project : Example
// DATE    : 7/24/2013 Wed
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __PLATFORM_BASE_H__
#define __PLATFORM_BASE_H__
#include <EGL/egl.h>
#include <RC1/rc1.h>
#include <string.h>
#include <debug/egl_ElapsedTimeMarker.h>
#include "STDInterface.h"

#define	MOUSE_BTN_NULL			0
#define	MOUSE_BTN_LEFT			1
#define	MOUSE_BTN_RIGHT			(1<<1)
#define	MOUSE_BTN_MIDDLE		(1<<2)

class PlatformBase{
public:
	PlatformBase(void){
		peglGetElapsedTime	= GET_EGLGETELAPSEDTIMEPROC;
	}
	virtual BOOL Create(const char* sWindowTitle, int& width, int& height) = 0;
	virtual void Destroy(void) = 0;

	inline int Width(void)		{return m_iWidth;}
	inline int Height(void)		{return m_iHeight;}

	virtual EGLNativeWindowType GetWindowHandle(void) = 0;
	virtual BOOL PumpMessage(BOOL bRun = TRUE) = 0;
	virtual void KeyDownEvent(char cKey) = 0;
	virtual void OnMouseEvent(float x, float y, DWORD dwBtn = MOUSE_BTN_NULL) = 0;
	virtual void OnDrawComplete(void) = 0;

protected:
	void CastElapsedTime(char* msg){
		if(peglGetElapsedTime){
			char sDebugInfo[256];
			sprintf(sDebugInfo, " : Render(%f), TreeBuild(%f), Total(%f)",
				peglGetElapsedTime(EGL_ELAPSED_TIME_MARKER_RENDER),
				peglGetElapsedTime(EGL_ELAPSED_TIME_MARKER_TREE_BUILD),
				peglGetElapsedTime(EGL_ELAPSED_TIME_MARKER_TOTAL));
			strcat(msg, sDebugInfo);
		}
	}

	int				m_iWidth, m_iHeight;

private:
	PFNEGLGETELAPSEDTIMEPROC	peglGetElapsedTime;
};

#endif//__PLATFORM_BASE_H__
