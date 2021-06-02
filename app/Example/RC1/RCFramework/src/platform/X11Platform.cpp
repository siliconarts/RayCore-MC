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
// DATE    : 7/25/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "X11Platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

DWORD					g_dwMouseButton		= 0;
float					g_fMouseX			= 0;
float					g_fMouseY			= 0;

WindowPlatform::WindowPlatform(void){
	m_bInitialized	= FALSE;
	m_hWnd			= NULL;
	m_disp			= NULL;
}

WindowPlatform::~WindowPlatform(void){
}

BOOL WindowPlatform::Create(const char* sWindowTitle, int& width, int& height){
	m_bShow			= FALSE;
	m_bInitialized	= TRUE;
	
	m_disp			= XOpenDisplay(NULL);
	assert(m_disp  != NULL);
	m_sTitle		= (char *)sWindowTitle;

	m_screenNum		= DefaultScreen(m_disp);

	m_hWnd			= XCreateSimpleWindow(m_disp, RootWindow(m_disp, m_screenNum), 0, 0, width, height, 2, BlackPixel (m_disp,0), WhitePixel(m_disp,0));

	XMapWindow(m_disp, m_hWnd);
	
	{
		m_WM_DELETE_WINDOW = XInternAtom(m_disp, "WM_DELETE_WINDOW", False);
		XSetWMProtocols(m_disp, m_hWnd, &m_WM_DELETE_WINDOW, 1);
	}

	XStoreName(m_disp, m_hWnd, m_sTitle);
	XSelectInput(m_disp, m_hWnd, KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask/*| KeyReleaseMask*/);
	gettimeofday(&m_Time.start, NULL);	//timer start

	{	// Áß¾Ó¿¡ 'Loading...' Ãâ·Â
		const char sLoading[]	= "Loading...";
		GC			gc = XCreateGC(m_disp, m_hWnd, 0, NULL);
		XDrawString(m_disp, m_hWnd, gc, (width - 8*(sizeof(sLoading)-1))/2, (height - 16)/2, sLoading, sizeof(sLoading) - 1);
		XFreeGC(m_disp, gc);
	}

	XFlush(m_disp);
	return TRUE;
}

void WindowPlatform::Destroy(void){
	if(!m_bInitialized) return;
	if(m_hWnd){
		XDestroyWindow(m_disp, m_hWnd);
		m_hWnd	= NULL;
	}
	if(m_disp){
		XCloseDisplay(m_disp);
		m_disp	= NULL;
	}
	m_bInitialized	= FALSE;
}


EGLNativeWindowType WindowPlatform::GetWindowHandle(void){
		return (EGLNativeWindowType)m_hWnd; 
}

BOOL WindowPlatform::PumpMessage(BOOL bRun){
	XEvent ev;

	while(XPending(m_disp)){
		XNextEvent(m_disp, &ev);
		switch(ev.type){
		case Expose:		m_bShow	= TRUE;		break;
		case NoExpose:		m_bShow	= FALSE;	break;
		case ClientMessage:
			if(ev.xclient.data.l[0] == m_WM_DELETE_WINDOW)
				return FALSE;
			break;
		case DestroyNotify:	return FALSE;
		case ButtonPress:
			switch(ev.xbutton.button){ //press
			case Button1: g_dwMouseButton |= (1<<0);	break;	//left mouse button
			case Button2: g_dwMouseButton |= (1<<2);	break;	//middle mouse button
			case Button3: g_dwMouseButton |= (1<<1);	break;	//right mouse button
			}
			goto UPDATE_BUTTON_STATUS;
		case ButtonRelease:
			switch(ev.xbutton.button){ //press
			case Button1: g_dwMouseButton &= ~(1<<0);	break;	//left mouse button
			case Button2: g_dwMouseButton &= ~(1<<2);	break;	//middle mouse button
			case Button3: g_dwMouseButton &= ~(1<<1);	break;	//right mouse button
			}
			goto UPDATE_BUTTON_STATUS;
		case MotionNotify:
		case LeaveNotify:
			g_fMouseX = ((float)ev.xmotion.x) / m_iWidth;
			g_fMouseY = ((float)ev.xmotion.y) / m_iHeight;
UPDATE_BUTTON_STATUS:
			OnMouseEvent(g_fMouseX, g_fMouseY, g_dwMouseButton);
			break;
		case KeyPress:
			{
				int r, code;

				code = XLookupKeysym(&ev.xkey, 0);

				switch(code){
				case XK_KP_Add:	//+
					KeyDownEvent(0x5A + 1);
				break;
				case XK_KP_Subtract:	//-
					KeyDownEvent(0x5A + 2);
					break;
				case XK_Up:	//up 
					KeyDownEvent(0x5A + 3);
					break;
				case XK_Down:	//down
					KeyDownEvent(0x5A + 4);
					break;
				case XK_Left:	//¡çleft
					KeyDownEvent(0x5A + 5);
					break;
				case XK_Right:	//¡æright
					KeyDownEvent(0x5A + 6);
					break;
				case XK_KP_0:
				case XK_KP_1:
				case XK_KP_2:
				case XK_KP_3:
				case XK_KP_4:
				case XK_KP_5:
				case XK_KP_6:
				case XK_KP_7:
				case XK_KP_8:
				case XK_KP_9:
					KeyDownEvent(0x30 + (code - XK_KP_0));
					break;
				case XK_Escape:
					return FALSE;
				default:
					if((code >= '0' && code <= '9')) {
						KeyDownEvent(code);
					}
					if((code>= 'a' && code <= 'z')) {
						KeyDownEvent(code - ('a' - 'A'));
					}
					break;
				}
			}
		default:
			break;
		}
	}

	return bRun;
}

void WindowPlatform::OnDrawComplete(void){
	//fps
	static int		render_count	= 0;

	render_count++;

	if(render_count == 10) {
		gettimeofday(&m_Time.end, NULL);	//timer end
		long elapsed_time_us = (1000000*(m_Time.end.tv_sec - m_Time.start.tv_sec)) + (m_Time.end.tv_usec - m_Time.start.tv_usec);
		float fps = 10000000.f/elapsed_time_us;
		char sTitle[100];
		sprintf(sTitle, "%s : %.2f fps", m_sTitle, fps);
		CastElapsedTime(sTitle);
		XStoreName(m_disp, m_hWnd, sTitle);

		m_Time.start	= m_Time.end;

		render_count	= 0;
	}
}
