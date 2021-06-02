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
// DATE    : 1/21/2014 Tue
// Author  : ay
//
//=======================================================================

#include "eglWindowX11.h"
#include <assert.h>

//-----------------------------------------------------------------------------------------
// factory
static eglWindow* __CreateWindowFactoryFunction(EGLNativeWindowType win){
	if(!win) return NULL;
	return new eglWindowX11(win);
}
static eglWindowFactory	__CreateWindowFactory(__CreateWindowFactoryFunction);

//-----------------------------------------------------------------------------------------
// eglWindowX11
eglWindowX11::eglWindowX11(EGLNativeWindowType win){
	m_sDisplayName	= NULL;
	m_pXdpy			= XOpenDisplay(m_sDisplayName);
	assert(m_pXdpy != NULL);
	m_Wnd			= (Window)win;
	XGetWindowAttributes(m_pXdpy, m_Wnd, &m_WindowAttributes);
	memset(m_pXImage, 0, sizeof(XImage*)*EGL_WINDOWX_BUFFER_COUNT);
	m_pXImage[0] =
	m_pXImage[1] = NULL;
	m_iCurrentBuffer	= 0;
	GetWindowSize(m_iWidth, m_iHeight);

	switch(m_WindowAttributes.depth){
	case 16:	// 16 bit
	case 24:	// 24 bit
	case 32:	// 32 bit
		break;
	default:
		printf("This screen depth(%d) is not supported.\n", m_WindowAttributes.depth);
	}
	m_iPixelDepth	= m_WindowAttributes.depth;

#ifdef __LP64__  //64bit
	for(int i=0;i<EGL_WINDOWX_BUFFER_COUNT;i++) m_pBuffer[i] = NULL;
#endif
}

eglWindowX11::~eglWindowX11(void){
	for(int i=0;i<EGL_WINDOWX_BUFFER_COUNT;i++)
		if(m_pXImage[i]){
			//XDestroyImage(m_pXImage[i]);	// @todo : Is it needed?
			m_pXImage[i]	= NULL;
		}

#if defined(__LP64__) || defined(CROSS_COMPILE)
		for(int i=0;i<EGL_WINDOWX_BUFFER_COUNT;i++){
			if(m_pBuffer[i]){
				free(m_pBuffer[i]);
				m_pBuffer[i]	= NULL;
			}
		}
#endif
	
	if(m_pXdpy){
		XCloseDisplay(m_pXdpy);
		m_pXdpy	= NULL;
	}
}

void eglWindowX11::GetWindowSize(int &iWidth, int &iHeight){

	assert(XGetWindowAttributes(m_pXdpy, m_Wnd, &m_WindowAttributes));

	iWidth		= m_WindowAttributes.width;
	iHeight		= m_WindowAttributes.height;
}

void eglWindowX11::Present(RCDDK* pDDK, DrawBuffer* pBuffer){
	if(m_Wnd) {
		unsigned int dwCopyBytes	= pBuffer->Width() * pBuffer->Height() * sizeof(unsigned int);
		Memory*	pMem				= pBuffer->GetMemory();

#ifdef __LP64__  //64bit
		if(!m_pXImage[m_iCurrentBuffer]){
			m_pBuffer[m_iCurrentBuffer] = (char *)malloc(m_iWidth * m_iHeight * sizeof(unsigned int));
			XImage* pImage	= XCreateImage(m_pXdpy, DefaultVisual(m_pXdpy, 0), 24, ZPixmap, 0, m_pBuffer[m_iCurrentBuffer], m_iWidth, m_iHeight, 32, 0);
			m_pXImage[m_iCurrentBuffer]	= pImage;
			pImage->byte_order	= MSBFirst;
		}
		
		if(m_pXImage[m_iCurrentBuffer]){
			memcpy(((char *)m_pXImage[m_iCurrentBuffer]->data), (char *)pBuffer->GetMemory()->Virtual(), m_iWidth * 4 * m_iHeight -1);

			XPutImage(m_pXdpy, m_Wnd, DefaultGC(m_pXdpy, 0), m_pXImage[m_iCurrentBuffer], 0, 0, 0, 0, m_iWidth, m_iHeight);
		}
#else	//32bit
		
		if(!m_pXImage[m_iCurrentBuffer]){
#ifdef CROSS_COMPILE
			XImage* pImage	= NULL;
			
			switch(m_iPixelDepth){
			case 16:
				m_pBuffer[m_iCurrentBuffer] = (char *)malloc(m_iWidth * m_iHeight * sizeof(WORD));		// 16 bit
				pImage	= XCreateImage(m_pXdpy, DefaultVisual(m_pXdpy, 0), 16, ZPixmap, 0, m_pBuffer[m_iCurrentBuffer], m_iWidth, m_iHeight, 16, 0);
				break;
			case 32:
			case 24:
				m_pBuffer[m_iCurrentBuffer] = (char *)malloc(m_iWidth * m_iHeight * sizeof(DWORD));		// 32 bit
				pImage	= XCreateImage(m_pXdpy, DefaultVisual(m_pXdpy, 0), m_iPixelDepth, ZPixmap, 0, m_pBuffer[m_iCurrentBuffer], m_iWidth, m_iHeight, 32, 0);
				pImage->byte_order	= MSBFirst;
				break;
			}
			
			m_pXImage[m_iCurrentBuffer]	= pImage;
#else
			// buffer
			XImage* pImage	= XCreateImage(m_pXdpy, DefaultVisual(m_pXdpy, 0), 24, ZPixmap, 0, (char*)pBuffer->GetMemory()->Virtual(), m_iWidth, m_iHeight, 32, 0);
			m_pXImage[m_iCurrentBuffer]	= pImage;
			pImage->byte_order	= MSBFirst;
#endif
		}

		if(m_pXImage[m_iCurrentBuffer]){
#ifdef CROSS_COMPILE
			switch(m_iPixelDepth){
			case 16:
				{	// 16 bit
					VECTOR4c*	pSrc	= (VECTOR4c*)pBuffer->GetMemory()->Virtual();
					WORD*		pDst	= (WORD*)m_pBuffer[m_iCurrentBuffer];
					for(DWORD y=0;y<m_iHeight;y++)
					for(DWORD x=0;x<m_iWidth;x++){
						//*pDst	= ((WORD)(pSrc->b>>3)<<11)|((WORD)(pSrc->g>>2)<<5)|(pSrc->r>>3);
						*pDst	= ((WORD)(pSrc->r>>3)<<11)|((WORD)(pSrc->g>>2)<<5)|(pSrc->b>>3);
						pDst++;
						pSrc++;
					}
				}
				break;
			case 24:
			case 32:
				{	// 32 bit
					DWORD*	pSrc	= (DWORD*)pBuffer->GetMemory()->Virtual();
					DWORD*	pDst	= (DWORD*)m_pBuffer[m_iCurrentBuffer];
					for(DWORD y=0;y<m_iHeight;y++)
					for(DWORD x=0;x<m_iWidth;x++){
						*pDst		= (*pSrc) << 8;
						pDst++;
						pSrc++;
					}
				}
				break;
			}
#endif
			XPutImage(m_pXdpy, m_Wnd, DefaultGC(m_pXdpy, 0), m_pXImage[m_iCurrentBuffer], 0, 0, 0, 0, m_iWidth, m_iHeight);
		}
#endif
		
		m_iCurrentBuffer++;
		
		if(m_iCurrentBuffer>=EGL_WINDOWX_BUFFER_COUNT) m_iCurrentBuffer = 0;
		XFlush(m_pXdpy);
	}
}
