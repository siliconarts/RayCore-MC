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
// DATE    : 3/27/2013 Wed
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __EGL_CONTEXT_H__
#define __EGL_CONTEXT_H__

#include "eglCommon.h"
#include "eglConfig.h"
#include "eglDisplay.h"
#include "eglSurface.h"
#include "common/DrawBuffer.h"
#include "glrc/rcContext.h"

namespace GL{
	class _EGLContext :
		public ObjectT<_EGLContext>
	{
	public :
		_EGLContext(void);

		//releated width _EGLSurface
		void SetSurface(_EGLSurface* pDraw, _EGLSurface* pRead);  //Using that makes 
		BOOL Create(_EGLDisplay* pDisplay, EGLConfig config, _EGLContext* pContext, const EGLint *attr_list);
		BOOL MakeCurrent(_EGLDisplay* pDisplay, _EGLSurface* draw, _EGLSurface* read);
		BOOL DoFinish(void);

		void Destroy(void);
		inline static _EGLContext* GetCurrent()	{return m_pCurrent;}

		BOOL QueryAttribute(EGLint attribute, EGLint *value);
	protected:
		virtual ~_EGLContext(void);

	protected:
		static _EGLContext*		m_pCurrent;
		_EGLConfig*				m_config;
		EGLenum					m_client_API;
		EGLint					m_client_Version;
		EGLConfig				m_config_id;

		_EGLSurface*			m_pSurfaceDraw;
		_EGLSurface*			m_pSurfaceRead;
	
		ContextRC*				m_pCtx;
	};

	#define CURRENT_EGL_CONTEXT		_EGLContext::GetCurrent()
	_EGLContext* RetrieveEGLContext(EGLContext contextId);
};

#endif//__EGL_CONTEXT_H__
