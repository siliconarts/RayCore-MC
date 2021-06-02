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
// DATE    : 8/27/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __EGL_DISPLAY_H__
#define __EGL_DISPLAY_H__

#include "eglCommon.h"
#include "eglSurface.h"
#include "eglConfig.h"
#include "ddk/rcDDK.h"
#include "debug/egl_ReportRegisters.h"

namespace GL{

	class _EGLSurface;

	class _EGLDisplay : public ObjectT<_EGLDisplay>
	{
		virtual ~_EGLDisplay(void);
	public : 
		_EGLDisplay(void);

		void SetCurrent(void);
		inline static _EGLDisplay* GetCurrent(void)	{return m_pCurrent;}
		void SwapBuffers(_EGLSurface* pSurface);
		

		BOOL Initialize(EGLint *major, EGLint *minor);
		BOOL IsInitialized(void);

		const char * QueryString(EGLint name);

		inline _EGLConfig* Config(void)	{return m_pConfig;}

		BOOL ChooseConfig(const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config);
		BOOL GetConfigAttribute(int config_id, EGLint attribute, EGLint *value);

		_EGLSurface* CreateSurface(EGLConfig config_id, EGLNativeWindowType win, const EGLint *attr_list);
		BOOL SetCurrentSurface(_EGLSurface* pSurface);

		inline RCDDK* GetDDK(void)	{return m_pDDK;}

		void DumpSystem(const char* filename);
		BOOL GetReportRegister(int& iDeviceID, EGL_REPORT* pReport);

	private:
		static _EGLDisplay*		m_pCurrent;
		int						m_iWidth, m_iHeight;
		_EGLConfig*				m_pConfig;
		RCDDK*					m_pDDK;
	};

	#define CURRENT_DISPLAY		_EGLDisplay::GetCurrent()
	_EGLDisplay* RetrieveEGLDisplay(EGLDisplay dpy);

};

#endif//__EGL_DISPLAY_H__
