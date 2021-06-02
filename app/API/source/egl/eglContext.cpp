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
// DATE    : 7/22/2013 Mon
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "eglContext.h"


_EGLContext*	_EGLContext::m_pCurrent	= NULL;

namespace GL {
	_EGLContext* RetrieveEGLContext(EGLContext contextId){
		_EGLContext* pContext = (_EGLContext *)contextId;
		if(!pContext || !pContext->IsValidObject()) {
			eglErrorHandler.SetError(EGL_BAD_CONTEXT);
			return NULL;
		}
		return pContext;
	}
}

_EGLContext::_EGLContext(void)
{
	m_config			= NULL;
	m_client_API		= 0;
	m_client_Version	= 0;
	m_config_id			= 0;
	m_pSurfaceDraw		=
	m_pSurfaceRead		= NULL;
	m_pCtx				= NULL;
	

}

_EGLContext::~_EGLContext(void)
{
	ELAPSED_TIME_CLEANUP();
	Destroy();
	SetSurface(NULL, NULL);
	if(m_pCurrent == this) m_pCurrent = NULL;
}
#include "glrc/rcContext1.h"	// @todo : 나중에 없어져야 함.

BOOL _EGLContext::Create(_EGLDisplay* pDisplay, EGLConfig config, _EGLContext* pContext, const EGLint *attr_list)
{
	Destroy();

	m_client_API		= EGL_OPENGL_ES_API;
	m_client_Version	= 1;
	m_config_id			= config;
	m_pCurrent			= this;
	
// 	if(!share_context) {
// 		//no texture object
// 	}
// 	else {
// 		//draw texture container
// 		
// 		//link ctx from share_context
// 		m_pCurrent		= (_EGLContext *)share_context;
// 
// 	}

	// @todo : 후에 ContextRC1에 의존성이 없어야 한다.
	m_pCtx	= new ContextRC1(pDisplay->GetDDK());

	return EGL_TRUE;
}
void _EGLContext::Destroy(void) {
	if(m_pCtx){
		m_pCtx->Delete();
		m_pCtx	= NULL;
	}
}

void _EGLContext::SetSurface(_EGLSurface* pDraw, _EGLSurface* pRead) {
	if(pDraw) pDraw->AddRef();
	if(pRead) pRead->AddRef();

	if(m_pSurfaceDraw) m_pSurfaceDraw->Release();
	if(m_pSurfaceRead) m_pSurfaceRead->Release();

	m_pSurfaceDraw	= pDraw;
	m_pSurfaceRead	= pRead;
}

BOOL _EGLContext::MakeCurrent(_EGLDisplay* pDisplay, _EGLSurface* draw, _EGLSurface* read){
	if(draw != read) return FALSE;

	SetSurface(draw, read);
	pDisplay->SetCurrentSurface(draw);

	m_pCurrent	= this;
	((ContextRC1*)m_pCtx)->SetSurface(draw);		// @todo : 구현에 대한 의존성 해결이 필요하다.
	return TRUE;
}

BOOL _EGLContext::DoFinish(void){
	((ContextRC1*)m_pCtx)->Finish(TRUE);
	return TRUE;
}

BOOL _EGLContext::QueryAttribute(EGLint attribute, EGLint *value){
	switch(attribute){
	case EGL_CONFIG_ID:	
		*value = *(EGLint *)&m_config_id;
		break;
	case EGL_CONTEXT_CLIENT_TYPE:
		*value = m_client_API;
		break;
	case EGL_CONTEXT_CLIENT_VERSION:
		*value = m_client_Version;
		break;
	case EGL_RENDER_BUFFER:
		*value = EGL_BACK_BUFFER;  //pbuffer surface rendering buffer
		break;
	default:
		eglErrorHandler.SetError(EGL_BAD_PARAMETER);
		return FALSE;
	}
	
	return TRUE;
}