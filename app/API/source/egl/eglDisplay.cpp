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
// Author  : Sang Gon, Kim (sgkim@siliconarts.co.kr)
//
//=======================================================================

#include "eglDisplay.h"


static const char*	__STRING_VENDOR		= "Siliconarts";
static const char*	__STRING_VERSION	= "EGL 1.3";
static const char*	__STRING_EXTENSIONS	= "";

_EGLDisplay*	_EGLDisplay::m_pCurrent	= NULL;

namespace GL{
	_EGLDisplay* RetrieveEGLDisplay(EGLDisplay dpy){
		_EGLDisplay* pDisplay	= (_EGLDisplay*)dpy;
		if(!pDisplay || !pDisplay->IsValidObject()){
			eglErrorHandler.SetError(EGL_BAD_DISPLAY);
			return NULL;
		}
		return pDisplay;
	}
};

_EGLDisplay::_EGLDisplay(void){
	m_pDDK			= CreateRCDDK();
	m_pConfig		= NULL;
	m_iWidth		=
	m_iHeight		= 0;
}

_EGLDisplay::~_EGLDisplay(void){
	if(m_pCurrent == this) m_pCurrent = NULL;
	if(m_pConfig) delete m_pConfig;
	SAFE_RELEASE(m_pDDK);
}

void _EGLDisplay::SetCurrent(void){
	m_pCurrent	= this;
}

BOOL _EGLDisplay::Initialize(EGLint *major, EGLint *minor){
	// 이미 동일한 디스플레이 객체가 이미 할당되었다면 여기서 EGL_FALSE 를 반환해야 한다.
	if(!m_pDDK){
		eglErrorHandler.SetError(EGL_NOT_INITIALIZED);
		return FALSE;
	}

	// EGL version 1.3
	if(major) *major = 1;
	if(minor) *minor = 3;

	// m_pConfig 생성
	{
		m_pConfig = new _EGLConfig(m_pConfig);
		m_pConfig->SetAttribute(3, 2,  //SetItem 매개변수 뒤쪽 3개, rgb 세팅한 갯수(565 1개, 8882개)
			CONFIG_CRITERION_ATLEAST,
			// index id					attribute id		default value
			CONFIG_INDEX_RED_SIZE,		EGL_RED_SIZE,		5,
			CONFIG_INDEX_GREEN_SIZE,	EGL_GREEN_SIZE,		6,
			CONFIG_INDEX_BLUE_SIZE,		EGL_BLUE_SIZE,		5);
		m_pConfig->SetItem(0, 5,6,5);  //index, conf1, conf2, conf3
		m_pConfig->SetItem(1, 8,8,8);
	}
	
	{
		m_pConfig = new _EGLConfig(m_pConfig);
		m_pConfig->SetAttribute(1, 2,
			CONFIG_CRITERION_ATLEAST,
			CONFIG_INDEX_DEPTH_SIZE,	EGL_DEPTH_SIZE,		0);
		m_pConfig->SetItem(0, 0);
		m_pConfig->SetItem(1, 16);
	}
	
	{	// Set the RGBA Mode
#ifdef WIN32
		m_pDDK->SetRGBAMode(RCDDK_RGBA_MODE_ABGR);
#else// linux
		m_pDDK->SetRGBAMode(RCDDK_RGBA_MODE_BGRA);
#endif
	}

	/*{	// surface TYPE
		m_pConfig = new _EGLConfig(m_pConfig);
		m_pConfig->SetAttribute(1, 2,
			CONFIG_CRITERION_MASK,
			CONFIG_INDEX_SURFACE_TYPE,	EGL_SURFACE_TYPE,	EGL_WINDOW_BIT);
		m_pConfig->SetItem(0, EGL_WINDOW_BIT | EGL_PIXMAP_BIT);
	}*/

	{	// surface TYPE
		m_pConfig = new _EGLConfig(m_pConfig);
		m_pConfig->SetAttribute(1, 3,
			CONFIG_CRITERION_EXACT,
			CONFIG_INDEX_SURFACE_TYPE,	EGL_SURFACE_TYPE,	EGL_WINDOW_BIT);
		m_pConfig->SetItem(0, EGL_WINDOW_BIT | EGL_PIXMAP_BIT);
		m_pConfig->SetItem(1, EGL_WINDOW_BIT);
		m_pConfig->SetItem(2, EGL_PIXMAP_BIT);
	}

	return TRUE;
}

BOOL _EGLDisplay::ChooseConfig(const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config){
	if(!m_pConfig) return FALSE;

	{
		// attrib_list 로 부터 모든 cofiguration table 만들기
		int user_config[CONFIG_INDEX_SIZE];
		m_pConfig->MakeConfigFromUserAttribute(user_config, attrib_list);

		// 해당 config id 찾기
		*num_config = 0;
		m_pConfig->FindConfig(user_config, configs, config_size, *num_config);
	}

	return TRUE;
}

BOOL _EGLDisplay::GetConfigAttribute(int config_id, EGLint attribute, EGLint *value){
	config_id--;
	if(config_id<0 || !m_pConfig || config_id >= m_pConfig->ConfigSize()) return FALSE;
	int index = m_pConfig->GetIndexFromAttributeID(attribute);
	
	if(index<0) return FALSE;

	{
		int user_config[CONFIG_INDEX_SIZE];
		m_pConfig->MakeTable(config_id, user_config);
		*value	= user_config[index];
	}
	return TRUE;
}

BOOL _EGLDisplay::IsInitialized(void){
	if(!m_pDDK) eglErrorHandler.SetError(EGL_NOT_INITIALIZED);
	return m_pDDK != NULL;
}

const char * _EGLDisplay::QueryString(EGLint name){
	switch(name){
	case EGL_VENDOR:		return __STRING_VENDOR;
	case EGL_VERSION:		return __STRING_VERSION;
	case EGL_EXTENSIONS:	return __STRING_EXTENSIONS;
	default:				eglErrorHandler.SetError(EGL_BAD_PARAMETER);
	}
	return NULL;
}

void _EGLDisplay::SwapBuffers(_EGLSurface* pSurface){
	pSurface->Present(m_pDDK);
	SetCurrentSurface(pSurface);								//buffering debug
}

_EGLSurface* _EGLDisplay::CreateSurface(EGLConfig config_id, EGLNativeWindowType win, const EGLint *attr_list){
	_EGLSurface *pSurface = new _EGLSurface;

	if(pSurface) {
		if(!(pSurface->Create(this, *(int*)(&config_id), win))) {
			pSurface->Delete();
			pSurface=NULL;
		}
	}

	return pSurface;
}

BOOL _EGLDisplay::SetCurrentSurface(_EGLSurface* pSurface){
	if(!pSurface) return FALSE;
	{
		DrawBuffer* pBuffer = pSurface->GetCurrentDrawBuffer();
		
		{// present to front display buffer
			int iWidth	= pBuffer->Width();
			int iHeight	= pBuffer->Height();
		
			if((iWidth!=m_iWidth) || (iHeight!=m_iHeight)) {
				m_pDDK->DisplaySetup(iWidth, iHeight,
					pBuffer->Format() == BUFFER_FORMAT_A8B8G8R8 ? COLOR_FORMAT_A8B8G8R8 : COLOR_FORMAT_R5G6B5);
				m_iWidth	= iWidth;
				m_iHeight	= iHeight;
			}
		}
		
		// set work framebuffer
		m_pDDK->SetMemory(RCDDK_MEMORY_FRAMEBUFFER, pBuffer->GetMemory()->GetNative());
	}
	return TRUE;
}

void _EGLDisplay::DumpSystem(const char* filename){
	if(m_pDDK) m_pDDK->DumpSystem(filename);
}

BOOL _EGLDisplay::GetReportRegister(int& iDeviceID, EGL_REPORT* pReport){
	if(m_pDDK) return m_pDDK->ReportRegister(iDeviceID, pReport);
	return FALSE;
}