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

#include "eglSurface.h"

namespace GL{
	_EGLSurface* RetrieveEGLSurface(EGLSurface surfaceId){
		_EGLSurface* pSurfaceId = (_EGLSurface *)surfaceId;
		if(!pSurfaceId || !pSurfaceId->IsValidObject()) {
			eglErrorHandler.SetError(EGL_FALSE);
			return NULL;
		}
		return pSurfaceId;
	}
};

_EGLSurface*	_EGLSurface::m_pCurrent		= NULL;

_EGLSurface::_EGLSurface(void) 
{
	for(int i=0;i<EGL_SURFACE_COUNT;i++) m_pBuffer[i]	= NULL;
	m_iWorkBuffer	= 0;
	m_pWin			= NULL;
}
_EGLSurface::~_EGLSurface(){
	Destroy();
}

BOOL _EGLSurface::Create(_EGLDisplay * pDpy, int config_id, EGLNativeWindowType win){
	if(!pDpy || !win) return FALSE;
	{
		int width, height;
		m_pCurrent	= this;
		Destroy();

		m_pWin	= eglWindowFactory::CreateEGLWindow(win);
		if(!m_pWin) return FALSE;
		m_pWin->GetWindowSize(width, height);
		width	= width & (~0xF);	// 16의 배수로 자른다.
		height	= height & (~0xF);	// 16의 배수로 자른다.
		if(!width || !height) return FALSE;

		for(int i=0;i<EGL_SURFACE_COUNT;i++){
			m_pBuffer[i]	= new DrawBuffer;
			if(!m_pBuffer[i]->Create(width, height, BUFFER_FORMAT_A8B8G8R8)) return FALSE;

			// test 를 위한 패턴 삽입
			BYTE* pCurFrame = (BYTE*)m_pBuffer[i]->GetMemory()->Virtual();
			if(m_pBuffer[i]->PixelBytes()==4){
				DWORD* pBuff = (DWORD*)pCurFrame;
				for(int y=0;y<height;y++)
				for(int x=0;x<width;x++){
					int tt = 0;
					if(!((x^y)&0x1F) || !(((31-x)^y)&0x1F)) tt = 0x20;
					pBuff[x+(y*width)]	= (((x^y)&0x20)^tt) ? 0xFFFFFF : 0xAFAFAF;
				}
			}else{
				WORD* pBuff = (WORD*)pCurFrame;
				for(int y=0;y<height;y++)
				for(int x=0;x<width;x++){
					int tt = 0;
					if(!((x^y)&0x1F) || !(((31-x)^y)&0x1F)) tt = 0x20;
					pBuff[x+(y*width)]	= (((x^y)&0x20)^tt) ? 0xFFFF : ((0xAF>>3)<<11)|((0xAF>>2)<<5)|(0xAF>>3);
				}
			}
		}
		{	// change full screen
			EGLint	config; 
			pDpy->GetConfigAttribute(config_id, EGL_SURFACE_TYPE, &config);
			if(!(config & EGL_WINDOW_BIT)){
				m_pWin->ToogleFullScreen(TRUE);
			}
		}
		
	}
	return TRUE;
}

void _EGLSurface::Destroy(void){
	for(int i=0;i<EGL_SURFACE_COUNT;i++)
	if(m_pBuffer[i]){
		m_pBuffer[i]->Release();
		m_pBuffer[i]	= NULL;
	}
	m_iWorkBuffer	= 0;
	SAFE_RELEASE(m_pWin);
}

DrawBuffer* _EGLSurface::GetCurrentDrawBuffer(void){
	if(!m_pBuffer) return NULL;

	return m_pBuffer[m_iWorkBuffer];
}

void _EGLSurface::Update(void){
	if(m_pBuffer){
		m_iWorkBuffer++;
		if(m_iWorkBuffer>=EGL_SURFACE_COUNT)
			m_iWorkBuffer = 0;
	}
}

BOOL _EGLSurface::CopyBuffers(EGLNativePixmapType target){
	DrawBuffer* pDrawBuffer	= GetCurrentDrawBuffer();
	if(!pDrawBuffer) return FALSE;
	Memory *pMem = pDrawBuffer->GetMemory();
#ifdef WIN32
	{
		LPVOID		lpDIBits;
		struct{
			HDC		hDC;
			HBITMAP	hOldBmp;
		}src, dst;			// 소스랑 목적지 DC

		// HDC 할당
		dst.hDC				= CreateCompatibleDC(NULL);
		src.hDC				= CreateCompatibleDC(NULL);
		dst.hOldBmp			= (HBITMAP)SelectObject(dst.hDC, target);
		
		{
			// 소스 Bitmap 생성
			BITMAPV5HEADER		bmp_header;
			memset(&bmp_header, 0, sizeof(BITMAPV5HEADER));
			bmp_header.bV5Size			= sizeof(BITMAPV5HEADER);
			bmp_header.bV5Width			= pDrawBuffer->Width();
			bmp_header.bV5Height		= pDrawBuffer->Height();
			bmp_header.bV5Planes		= 1;
			bmp_header.bV5Compression	= BI_BITFIELDS;
			bmp_header.bV5BitCount		= 32;
			bmp_header.bV5RedMask		= 0x000000FF;
			bmp_header.bV5GreenMask		= 0x0000FF00;
			bmp_header.bV5BlueMask		= 0x00FF0000;
			bmp_header.bV5AlphaMask		= 0xFF000000;
			bmp_header.bV5SizeImage		= ((pDrawBuffer->Width()*bmp_header.bV5BitCount+31)/32 * 4)*pDrawBuffer->Height();

			src.hOldBmp	= CreateDIBSection(src.hDC, (BITMAPINFO*)&bmp_header, DIB_RGB_COLORS, &lpDIBits, NULL, 0);
			src.hOldBmp	= (HBITMAP)SelectObject(src.hDC, src.hOldBmp);

			// src 복사
			memcpy(lpDIBits, pMem->Virtual(), bmp_header.bV5SizeImage);//pMem->ByteSize());
			// target으로 복사
			BitBlt(dst.hDC, 0,0, bmp_header.bV5Width, bmp_header.bV5Height, src.hDC, 0, 0, SRCCOPY);
		}

		// HDC 해제
		SelectObject(src.hDC, src.hOldBmp);
		SelectObject(dst.hDC, dst.hOldBmp);
		DeleteDC(src.hDC);
		DeleteDC(dst.hDC);
	}
#else
	memcpy(target, pMem->Virtual(), pMem->ByteSize());
#endif
	return TRUE;
}

void _EGLSurface::Present(RCDDK* pDDK){
	DrawBuffer* pBuffer	= GetCurrentDrawBuffer();
	if(pBuffer){
		pBuffer->Lock();
		m_pWin->Present(pDDK, pBuffer);
		pBuffer->UnLock();
		Update();			//backbuffer update
	}
}

void _EGLSurface::ClearAll(void){
	for(int i=0;i<EGL_SURFACE_COUNT;i++)
	if(m_pBuffer[i]){
		memset(m_pBuffer[i]->GetMemory()->Virtual(), 0, m_pBuffer[i]->GetMemory()->ByteSize());
	}
}