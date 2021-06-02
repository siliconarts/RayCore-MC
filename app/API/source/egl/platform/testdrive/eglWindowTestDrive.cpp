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
// DATE    : 6/4/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#include "eglWindowTestDrive.h"

#ifdef USE_TESTDRIVE
//-----------------------------------------------------------------------------------------
// factory
static eglWindow* __CreateWindowFactoryFunction(EGLNativeWindowType win){
	if(!win || IsWindow(win)) return NULL;
	return new eglWindowTestDrive(win);
}
static eglWindowFactory	__CreateWindowFactory(__CreateWindowFactoryFunction, 2);

//-----------------------------------------------------------------------------------------
// eglWindowTestDrive
eglWindowTestDrive::eglWindowTestDrive(EGLNativeWindowType win){
	m_pWin	= (EGL_WINDOW_SURFACE*)win;
	m_pDisplayConfig		= NULL;

	m_pTDMemory_Main		= TestDriver_GetMemory(SYSTEM_NAME);
	m_pTDMemory_Display		= TestDriver_GetMemory(DISPLAY_NAME);
	if(m_pTDMemory_Display)
		m_pDisplayConfig	= (DisplayConfig*)m_pTDMemory_Display->GetConfig();
}

eglWindowTestDrive::~eglWindowTestDrive(void){
	SAFE_RELEASE(m_pTDMemory_Main);
	SAFE_RELEASE(m_pTDMemory_Display);
}

void eglWindowTestDrive::GetWindowSize(int &iWidth, int &iHeight){
	m_pWin->GetWindowSize(iWidth, iHeight);
}

#ifdef USE_TESTDRIVE
#include "egl/eglSurface.h"
#endif

void eglWindowTestDrive::Present(RCDDK* pDDK, DrawBuffer* pBuffer){
	Memory* pMem	= pBuffer->GetMemory();

	{	// copy to testdrive memory
		void* pSrc		= pMem->Virtual();
		DWORD dwSize	= pBuffer->ByteStride() * pBuffer->Height();
		DWORD dwPhy		= pMem->Physical() - pDDK->GetMemoryBase();
		void* pDst		= m_pTDMemory_Main->GetPointer(dwPhy, dwSize);
		
		if(!pDst){
			dwPhy	= 0;
			pDst	= m_pTDMemory_Main->GetPointer(0, dwSize);
		}
		assert(pDst != NULL);

		if(pSrc != pDst) RtlCopyMemory(pDst, pSrc, dwSize);

		m_pDisplayConfig->Front.dwAddress	= dwPhy;

		m_pDisplayConfig->Front.bUpdate		= TRUE;

		if(m_pDisplayConfig->hWndHandle)	// TestDrive display 가 있을 경우만 기다린다.
		for(;;){
			Sleep(1);
			if(!m_pDisplayConfig->Front.bUpdate){
				if(m_pDisplayConfig->bFramePause && GetKeyState(VK_SHIFT)<0){
					CURRENT_SURFACE->ClearAll();	// 덤프전 framebuffer 를 클리어시킨다.
					pDDK->DumpSystem();
				}
				break;
			}
		}
	}
}
#endif
