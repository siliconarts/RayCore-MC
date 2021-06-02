// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore API v 1.0
// Date : 

#ifndef __EGL_WINDOW_TESTDRIVE_H__
#define __EGL_WINDOW_TESTDRIVE_H__
#include "../eglWindow.h"

#ifdef USE_TESTDRIVE
#include "WindowSurface.h"
#include "TestDriver.h"
#include "driver/SystemConfig.h"
#include "VirtualDisplayConfig.h"

class eglWindowTestDrive :
	public eglWindow
{
public:
	eglWindowTestDrive(EGLNativeWindowType win);
	virtual ~eglWindowTestDrive(void);

	virtual void GetWindowSize(int &iWidth, int &iHeight);
	virtual void Present(RCDDK* pDDK, DrawBuffer* pBuffer);

private:
	EGL_WINDOW_SURFACE*		m_pWin;
	ITestDriverMemory*		m_pTDMemory_Main;
	ITestDriverMemory*		m_pTDMemory_Display;
	SYSTEM_CONFIG*			m_pTDSystemConfig;
	DisplayConfig*			m_pDisplayConfig;
};
#endif

#endif//__EGL_WINDOW_TESTDRIVE_H__
