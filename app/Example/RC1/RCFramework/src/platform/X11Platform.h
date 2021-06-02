// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore Application
// Date : 

#ifndef __WINDOW_PLATFORM_H__
#define __WINDOW_PLATFORM_H__
#include "PlatformBase.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>  //xlib key event
#include <sys/time.h>	//linux system time

//#define Sleep	msleep
#define Sleep(msec)

class WindowPlatform :
	public PlatformBase
{
public:
	WindowPlatform(void);
	virtual ~WindowPlatform(void);

	BOOL Create(const char* sWindowTitle, int& width, int& height);
	void Destroy(void);

	EGLNativeWindowType GetWindowHandle(void);
	BOOL PumpMessage(BOOL bRun = TRUE);
	virtual void OnDrawComplete(void);

private:
	BOOL				m_bInitialized;
	Display*			m_disp;
	Window				m_hWnd;
	int					m_screenNum;
	BOOL				m_bShow;
	Atom				m_WM_DELETE_WINDOW;
	char*				m_sTitle;
	struct{
		struct timeval	start, end;
	}m_Time;
	
};

#endif//__WINDOW_PLATFORM_H__
