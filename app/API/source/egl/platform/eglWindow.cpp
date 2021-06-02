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

#include "eglWindow.h"

eglWindow::eglWindow(void){}
eglWindow::~eglWindow(void){}

void eglWindow::Release(void){
	delete this;
}

void eglWindow::ToogleFullScreen(BOOL bFullScreen){
}

//-----------------------------------------------------------------------------------------
eglWindowFactory*	eglWindowFactory::m_pHead	= NULL;

eglWindowFactory::eglWindowFactory(EGL_WINDOW_FACTORY func, int iPriority){
	// set-up link
	if(!m_pHead){
SET_TOP_PRIORITY:
		m_pNext		= m_pHead;
		m_pHead		= this;
	}else{
		if(m_pHead->m_iPriority >= iPriority) goto SET_TOP_PRIORITY;
		else{
			eglWindowFactory* pNode	= m_pHead;
			while(pNode->m_pNext){
				eglWindowFactory* pNext	= pNode->m_pNext;
				if(pNext->m_iPriority >= iPriority) break;
				pNode		= pNext;
			}
			m_pNext			= pNode->m_pNext;
			pNode->m_pNext	= this;
		}
	}
	m_iPriority	= iPriority;
	m_Function	= func;
}

eglWindowFactory::~eglWindowFactory(void){
	if(m_pHead == this) m_pHead	= m_pNext;
	else{
		eglWindowFactory* pLink	= m_pHead;
		while(pLink){
			if(pLink->m_pNext == this){
				pLink->m_pNext	= m_pNext;
				return;
			}
			pLink	= pLink->m_pNext;
		}
	}
}

eglWindow* eglWindowFactory::CreateEGLWindow(EGLNativeWindowType win){
	eglWindowFactory*	pLink	= m_pHead;
	eglWindow*				pWin	= NULL;
	while(pLink){
		pWin	= pLink->m_Function(win);
		if(pWin) break;
		pLink	= pLink->m_pNext;
	}
	return pWin;
}
