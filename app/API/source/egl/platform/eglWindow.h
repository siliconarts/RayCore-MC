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
// DATE    : 5/3/2013 Fri
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __EGL_WINDOW_H__
#define __EGL_WINDOW_H__
#include "egl/eglCommon.h"
#include "common/DrawBuffer.h"

// EGL Window interface
class eglWindow{
protected:
	virtual ~eglWindow(void);

public:
	eglWindow(void);
	void Release(void);

	virtual void GetWindowSize(int &iWidth, int &iHeight)	= 0;	// 윈도우 크기 얻기
	virtual void Present(RCDDK* pDDK, DrawBuffer* pBuffer)	= 0;	// 버퍼 출력
	virtual void ToogleFullScreen(BOOL bFullScreen);				// 전체화면 갱신
};

// EGL Window factory
typedef eglWindow* (*EGL_WINDOW_FACTORY)(EGLNativeWindowType win);

class eglWindowFactory{
public:
	eglWindowFactory(EGL_WINDOW_FACTORY func, int iPriority = 0);	// 작은 값이 더 높은 우선순위를 나타냄.
	~eglWindowFactory(void);

	static eglWindow* CreateEGLWindow(EGLNativeWindowType win);

private:
	static eglWindowFactory*	m_pHead;
	eglWindowFactory*			m_pNext;
	EGL_WINDOW_FACTORY			m_Function;
	int							m_iPriority;
};

#endif//__EGL_WINDOW_H__
