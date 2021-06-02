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

#ifndef __EGL_SURFACE_H__
#define __EGL_SURFACE_H__

#include "eglCommon.h"
#include "eglDisplay.h"
#include "common/DrawBuffer.h"
#include "platform/eglWindow.h"

namespace GL{
	#define EGL_SURFACE_COUNT		2  //double buffer

	class _EGLDisplay;

	class _EGLSurface : public ObjectT<_EGLSurface>
	{
		virtual ~_EGLSurface(void);
	public : 
		_EGLSurface(void);
		
		inline static _EGLSurface* GetCurrent(void) {return m_pCurrent;}
		BOOL Create(_EGLDisplay * pDpy, int config_id, EGLNativeWindowType win);
		void Destroy(void);
		DrawBuffer* GetCurrentDrawBuffer(void);
		void Update(void);
		void Present(RCDDK* pDDK);
		void ClearAll(void);	// for debug
		BOOL CopyBuffers(EGLNativePixmapType target);

		inline int Lock(void)		{return Mutex::Lock();}
		inline int UnLock(void)		{return Mutex::UnLock();}

	private:
		static _EGLSurface*		m_pCurrent;
		DrawBuffer*				m_pBuffer[EGL_SURFACE_COUNT];
		int						m_iWorkBuffer;  //count of m_pBuffer
		eglWindow*				m_pWin;
	};

	#define CURRENT_SURFACE			_EGLSurface::GetCurrent()
	_EGLSurface* RetrieveEGLSurface(EGLSurface surfaceId);
};

#endif//__EGL_SURFACE_H__
