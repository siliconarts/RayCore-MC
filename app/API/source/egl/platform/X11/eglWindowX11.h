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
// DATE    : 7/8/2013 Mon
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __EGL_WINDOW_X11_H__
#define __EGL_WINDOW_X11_H__
#include "../eglWindow.h"

#include "STDInterface.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define EGL_WINDOWX_BUFFER_COUNT		2  //double buffer

class eglWindowX11 :
	public eglWindow
{
public:
	eglWindowX11(EGLNativeWindowType win);
	virtual ~eglWindowX11(void);

	virtual void GetWindowSize(int &iWidth, int &iHeight);
	virtual void Present(RCDDK* pDDK, DrawBuffer* pBuffer);

private:
	Display*		m_pXdpy;
	Window			m_Wnd;
	int				m_iWidth;
	int				m_iHeight;
	char*			m_sDisplayName;
	XImage*			m_pXImage[EGL_WINDOWX_BUFFER_COUNT];
	XWindowAttributes	m_WindowAttributes;
	int				m_iPixelDepth;
	int				m_iCurrentBuffer;
#if defined(__LP64__) || defined(CROSS_COMPILE) //64bit or versatile board
	char*			m_pBuffer[EGL_WINDOWX_BUFFER_COUNT];
#endif
};

#endif//__EGL_WINDOW_X11_H__
