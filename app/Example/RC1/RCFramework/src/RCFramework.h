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
// DATE    : 11/14/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __EGL_FRAMEWORK_H__
#define __EGL_FRAMEWORK_H__
#ifdef WIN32
	#include "platform/WindowsPlatform.h"
	#ifndef RC_FRAMEWORK
		#if defined(_WIN64)
			#pragma comment(lib, "RCFramework64.lib")
		#elif defined(_WIN32)
			#pragma comment(lib, "RCFramework.lib")
		#endif
	#endif
#else
	#include <stdlib.h>
	#include "platform/X11Platform.h"
#endif

class RCFramework :
	protected WindowPlatform
{
protected:
	virtual ~RCFramework(void);

public:
	RCFramework(const char* sTitleName, int width, int height);

	BOOL Initialize(int argc, char ** argv);
	void Release(void);
	BOOL DrawScene(void);	
	virtual void KeyDownEvent(char cKey);
	virtual int SkipFrame(void);
	
	inline EGLDisplay CurrentDisplay(void)	{return m_eglDisplay;}
	inline EGLContext CurrentContext(void)	{return m_eglContext;}
	inline EGLSurface CurrentSurface(void)	{return m_eglSurface;}

	void SetDepthBounce(unsigned int depth);
	void SetLightRadius(float radius);	
	void SetSPPCount(unsigned int nSpp);	
	void SetDiffuseRate(unsigned int rate);

protected:
	virtual BOOL OnIntialize(void);
	virtual void OnRelease(void);
	virtual BOOL OnDraw(void);
	virtual BOOL OnPostDraw(void);
	virtual void OnKeyDown(char cKey);
	virtual void OnMouseEvent(float x, float y, DWORD dwBtn = MOUSE_BTN_NULL);

	struct{
		int			argc;
		char**		argv;
	}m_Cmd;

private:
	EGLDisplay		m_eglDisplay;
	EGLContext		m_eglContext;
	EGLSurface		m_eglSurface;
	
	const char*		m_sTitle;

	BOOL			m_isLOD;
	DWORD			m_nDepthBounce;
	DWORD			m_nRenderQuality;
	DWORD			m_nThresholdQuality;
	DWORD			m_nSkipFrame;

	RCfloat			m_lightRadius; // range 0 ~ 8191
	RCuint			m_diffuseRate; // range 0 ~ 10
	RCuint			m_sppCount;    // range 1 ~ 32
	// roughness
	float			m_roughness;
	// roughness
	
};

#endif//__EGL_FRAMEWORK_H__
