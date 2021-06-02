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

#include "eglAPI.h"
#include "eglContext.h"

#ifdef EGL_RAYCORE_Extension
EGLAPI EGLBoolean EGLAPIENTRY eglGetReportRegister(EGLDisplay dpy, int& iDeviceID, EGL_REPORT* pReport);
#endif

#ifdef ELAPSED_TIME_CHECK
float EGLAPIENTRY eglGetElapsedTime(EGL_ELAPSED_TIME_MARKER id);
#endif

PROC_TABLE_DECLARE_EGL
	PROC_TABLE_ITEM_EGL( GetError						)
	PROC_TABLE_ITEM_EGL( GetDisplay						)
	PROC_TABLE_ITEM_EGL( Initialize						)
	PROC_TABLE_ITEM_EGL( Terminate						)
	PROC_TABLE_ITEM_EGL( QueryString					)
	PROC_TABLE_ITEM_EGL( GetConfigs						)
	PROC_TABLE_ITEM_EGL( ChooseConfig					)
	PROC_TABLE_ITEM_EGL( GetConfigAttrib				)
	PROC_TABLE_ITEM_EGL( CreateWindowSurface			)
	PROC_TABLE_ITEM_EGL( CreatePbufferSurface			)
	PROC_TABLE_ITEM_EGL( CreatePixmapSurface			)
	PROC_TABLE_ITEM_EGL( DestroySurface					)
	PROC_TABLE_ITEM_EGL( QuerySurface					)
	PROC_TABLE_ITEM_EGL( BindAPI						)
	PROC_TABLE_ITEM_EGL( QueryAPI						)
	PROC_TABLE_ITEM_EGL( WaitClient						)
	PROC_TABLE_ITEM_EGL( ReleaseThread					)
	PROC_TABLE_ITEM_EGL( CreatePbufferFromClientBuffer	)
	PROC_TABLE_ITEM_EGL( SurfaceAttrib					)
	PROC_TABLE_ITEM_EGL( BindTexImage					)
	PROC_TABLE_ITEM_EGL( ReleaseTexImage				)
	PROC_TABLE_ITEM_EGL( SwapInterval					)
	PROC_TABLE_ITEM_EGL( CreateContext					)
	PROC_TABLE_ITEM_EGL( DestroyContext					)
	PROC_TABLE_ITEM_EGL( MakeCurrent					)
	PROC_TABLE_ITEM_EGL( GetCurrentContext				)
	PROC_TABLE_ITEM_EGL( GetCurrentSurface				)
	PROC_TABLE_ITEM_EGL( GetCurrentDisplay				)
	PROC_TABLE_ITEM_EGL( QueryContext					)
	PROC_TABLE_ITEM_EGL( WaitGL							)
	PROC_TABLE_ITEM_EGL( WaitNative						)
	PROC_TABLE_ITEM_EGL( SwapBuffers					)
	PROC_TABLE_ITEM_EGL( CopyBuffers					)
	PROC_TABLE_ITEM_EGL( GetProcAddress					)

#ifdef EGL_RAYCORE_Extension
	PROC_TABLE_ITEM_EGL( DumpSystemRC					)
	PROC_TABLE_ITEM_EGL( GetReportRegister				)
#endif

#ifdef ELAPSED_TIME_CHECK
	PROC_TABLE_ITEM_EGL( GetElapsedTime					)
#endif

	// extension
	// 	PROC_TABLE_ITEM_EGL( LockSurfaceKHR					)
	// 	PROC_TABLE_ITEM_EGL( UnlockSurfaceKHR				)
	// 	PROC_TABLE_ITEM_EGL( CreateImageKHR					)
	// 	PROC_TABLE_ITEM_EGL( DestroyImageKHR				)
	// 	PROC_TABLE_ITEM_EGL( CreateSyncKHR					)
	// 	PROC_TABLE_ITEM_EGL( DestroySyncKHR					)
	// 	PROC_TABLE_ITEM_EGL( ClientWaitSyncKHR				)
	// 	PROC_TABLE_ITEM_EGL( SignalSyncKHR					)
	// 	PROC_TABLE_ITEM_EGL( GetSyncAttribKHR				)
	// 	PROC_TABLE_ITEM_EGL( CreateFenceSyncNV				)
	// 	PROC_TABLE_ITEM_EGL( DestroySyncNV					)
	// 	PROC_TABLE_ITEM_EGL( FenceNV						)
	// 	PROC_TABLE_ITEM_EGL( ClientWaitSyncNV				)
	// 	PROC_TABLE_ITEM_EGL( SignalSyncNV					)
	// 	PROC_TABLE_ITEM_EGL( GetSyncAttribNV				)
	// 	PROC_TABLE_ITEM_EGL( CreatePixmapSurfaceHI			)
PROC_TABLE_DECLARE_END

//----------------------------------------------------------------------------------------------------
// function proc address list manager
//----------------------------------------------------------------------------------------------------
EGLAPI __eglMustCastToProperFunctionPointerType EGLAPIENTRY EGL_DECLARE(GetProcAddress)(const char *procname){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglGetProcAddress.html
	return FIND_PROC_TABLE(procname);
}

//----------------------------------------------------------------------------------------------------
// maintenance
//----------------------------------------------------------------------------------------------------
EGLAPI EGLint EGLAPIENTRY EGL_DECLARE(GetError)(void){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglGetError.html
	return eglErrorHandler.GetError();
}

EGLAPI const char * EGLAPIENTRY EGL_DECLARE(QueryString)(EGLDisplay dpy, EGLint name){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglQueryString.html
	_EGLDisplay* pDisplay = RetrieveEGLDisplay(dpy);

	if(!pDisplay || !pDisplay->IsInitialized()) return NULL;
	return pDisplay->QueryString(name);
}

//----------------------------------------------------------------------------------------------------
// display
//----------------------------------------------------------------------------------------------------
EGLAPI EGLDisplay EGLAPIENTRY EGL_DECLARE(GetDisplay)(EGLNativeDisplayType display_id){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglGetDisplay.html
	if(display_id == EGL_DEFAULT_DISPLAY){
		_EGLDisplay* pDisplay	= new _EGLDisplay;
		return (EGLDisplay)pDisplay;
	}
	return EGL_NO_DISPLAY;
}

EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(Initialize)(EGLDisplay dpy, EGLint *major, EGLint *minor){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglInitialize.html
	_EGLDisplay* pDisplay = RetrieveEGLDisplay(dpy);
	if(!pDisplay || !pDisplay->Initialize(major, minor)) return EGL_FALSE;
	return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(Terminate)(EGLDisplay dpy){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglTerminate.html
	_EGLDisplay* pDisplay = RetrieveEGLDisplay(dpy);
	if(!pDisplay) return EGL_FALSE;
	pDisplay->Delete();
	
	return EGL_TRUE;
}

EGLAPI EGLDisplay EGLAPIENTRY EGL_DECLARE(GetCurrentDisplay)(void){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglGetCurrentDisplay.html
	return CURRENT_DISPLAY;
}

EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(SwapInterval)(EGLDisplay dpy, EGLint interval){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglSwapInterval.html
	// don't care this time...
	return EGL_FALSE;
}

EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(WaitGL)(void){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglWaitGL.html
	// don't care this time...
	return EGL_FALSE;
}

EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(WaitNative)(EGLint engine){
	// http://www.khronos.org/opengles/documentation/opengles1_0/html/eglWaitNative.html
	// don't care this time...
	return EGL_FALSE;
}

//----------------------------------------------------------------------------------------------------
// config
//----------------------------------------------------------------------------------------------------
EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(GetConfigs)(EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config){
	// http://www.khronos.org/opengles/documentation/opengles1_0/html/eglGetConfigs.html

	return eglChooseConfig(dpy, NULL, configs, config_size, num_config);
}

EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(ChooseConfig)(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config){
	// http://www.khronos.org/opengles/documentation/opengles1_0/html/eglChooseConfig.html
	_EGLDisplay* pDisplay = RetrieveEGLDisplay(dpy);
	if(!pDisplay || !pDisplay->IsInitialized()){
		eglErrorHandler.SetError(EGL_NOT_INITIALIZED);
		return EGL_FALSE;
	}

	if(!num_config){
		eglErrorHandler.SetError(EGL_BAD_PARAMETER);
		return EGL_FALSE;
	}
	if(!pDisplay->ChooseConfig(attrib_list, configs, config_size, num_config)) return EGL_FALSE;

	return EGL_TRUE;

}

EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(GetConfigAttrib)(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value){
	// http://www.khronos.org/opengles/documentation/opengles1_0/html/eglGetConfigAttrib.html
	_EGLDisplay* pDisplay = RetrieveEGLDisplay(dpy);
	if(!pDisplay || !pDisplay->IsInitialized()){
		eglErrorHandler.SetError(EGL_NOT_INITIALIZED);
		return EGL_FALSE;
	}

	if(!pDisplay->GetConfigAttribute(*((int*)&config), attribute, value)) return EGL_FALSE;
	
	return EGL_TRUE;
}

//----------------------------------------------------------------------------------------------------
// surface
//----------------------------------------------------------------------------------------------------
EGLAPI EGLSurface EGLAPIENTRY EGL_DECLARE(CreateWindowSurface)(EGLDisplay dpy, EGLConfig config_id, EGLNativeWindowType win, const EGLint *attr_list){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglCreateWindowSurface.html
	_EGLDisplay* pDisplay = RetrieveEGLDisplay(dpy);

	if(!pDisplay || !pDisplay->IsInitialized() || !win){
		eglErrorHandler.SetError(EGL_NOT_INITIALIZED);
		return EGL_FALSE;
	}

	_EGLSurface* pSurface = pDisplay->CreateSurface(config_id, win, attr_list);

	return 	pSurface;
}

EGLAPI EGLSurface EGLAPIENTRY EGL_DECLARE(CreatePbufferSurface)(EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglCreatePbufferSurface.html
	return EGL_NO_SURFACE;
}

EGLAPI EGLSurface EGLAPIENTRY EGL_DECLARE(CreatePixmapSurface)(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglCreatePixmapSurface.html
	return EGL_NO_SURFACE;
}

EGLAPI EGLSurface EGLAPIENTRY EGL_DECLARE(CreatePbufferFromClientBuffer)(EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer,	EGLConfig config, const EGLint *attrib_list){
	return EGL_NO_SURFACE;
}

EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(DestroySurface)(EGLDisplay dpy, EGLSurface surfaceId){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglDestroySurface.html
	_EGLDisplay *pDisplay = RetrieveEGLDisplay(dpy);
	_EGLSurface *pSurface = RetrieveEGLSurface(surfaceId);

	if(!pSurface) return EGL_FALSE;
	pSurface->Delete();
	return EGL_TRUE;

}

EGLAPI EGLSurface EGLAPIENTRY EGL_DECLARE(GetCurrentSurface)(EGLint readdraw){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglGetCurrentSurface.html
	// don't care this time...
	//@todo : eglMakeCurrent
	return CURRENT_SURFACE;
}


EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(QuerySurface)(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglQuerySurface.html
	// don't care this time...
	return EGL_FALSE;
}

EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(SurfaceAttrib)(EGLDisplay dpy, EGLSurface surface,	EGLint attribute, EGLint value){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglSurfaceAttrib.html
	return EGL_FALSE;
}
EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(BindTexImage)(EGLDisplay dpy, EGLSurface surface, EGLint buffer){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglBindTexImage.html
	// don't care this time...
	return EGL_FALSE;
}

EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(ReleaseTexImage)(EGLDisplay dpy, EGLSurface surface, EGLint buffer){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglReleaseTexImage.html
	// don't care this time...
	return EGL_FALSE;
}

EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(SwapBuffers)(EGLDisplay dpy, EGLSurface surface){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglSwapBuffers.html
 	_EGLDisplay* pDisplay = RetrieveEGLDisplay(dpy);
 	if(!pDisplay || !pDisplay->IsInitialized()) return EGL_FALSE;
	_EGLSurface* pSurface	= RetrieveEGLSurface(surface);
	_EGLContext* pContext	= CURRENT_EGL_CONTEXT;
	if(!pSurface || !pContext || !pContext->DoFinish()) return EGL_FALSE;
	pDisplay->SwapBuffers(pSurface);
	return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(CopyBuffers)(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglCopyBuffers.html
	_EGLDisplay* pDisplay = RetrieveEGLDisplay(dpy);
	if(!pDisplay || !pDisplay->IsInitialized()){
		eglErrorHandler.SetError(EGL_NOT_INITIALIZED);
		return EGL_FALSE;
	}
	_EGLSurface* pSurface = RetrieveEGLSurface(surface);
	if(!pSurface || !target) return EGL_FALSE;

	pSurface->CopyBuffers(target);
	return EGL_TRUE;
}

//----------------------------------------------------------------------------------------------------
// context
//----------------------------------------------------------------------------------------------------
EGLAPI EGLContext EGLAPIENTRY EGL_DECLARE(CreateContext)(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list){
	//http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglCreateContext.html
	_EGLDisplay *pDisplay = RetrieveEGLDisplay(dpy);
	if(!pDisplay || !pDisplay->IsInitialized()) return NULL;
	_EGLContext *pSharedContext = (_EGLContext*)share_context;
	if(pSharedContext){
		if(!pSharedContext->IsValidObject()){
			eglErrorHandler.SetError(EGL_BAD_CONTEXT);
			return NULL;
		}
	}
	_EGLContext *pContext = new _EGLContext;
 	if(!pContext->Create(pDisplay, config, pSharedContext, attrib_list)){
 		pContext->Delete();
 		pContext	= NULL;
 	}

	return pContext;
}

EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(DestroyContext)(EGLDisplay dpy, EGLContext ctx){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglDestroyContext.html
	_EGLDisplay* pDisplay = RetrieveEGLDisplay(dpy);

	if(!pDisplay) return FALSE;
	if(!pDisplay->IsInitialized()){
		eglErrorHandler.SetError(EGL_NOT_INITIALIZED);
		return EGL_FALSE;
	}
	if(!ctx) return EGL_TRUE;

	_EGLContext* pContext = RetrieveEGLContext(ctx);
	if(!pContext) return EGL_FALSE;
	pContext->Delete();

	return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(MakeCurrent)(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglMakeCurrent.html

	_EGLDisplay* pDisplay = RetrieveEGLDisplay(dpy);
	if(!pDisplay) return FALSE;
	if(!pDisplay->IsInitialized()){
		eglErrorHandler.SetError(EGL_NOT_INITIALIZED);
		return EGL_FALSE;
	}
	
	_EGLContext* pCtx	= ctx ? RetrieveEGLContext(ctx) : NULL;
	_EGLSurface* pDraw	= draw ? RetrieveEGLSurface(draw) : NULL;
	_EGLSurface* pRead	= read ? RetrieveEGLSurface(read) : NULL;
	if(!pCtx) return EGL_FALSE;
	
	pCtx->MakeCurrent(pDisplay, pDraw, pRead);
	return EGL_TRUE;
}

EGLAPI EGLContext EGLAPIENTRY EGL_DECLARE(GetCurrentContext)(void){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglGetCurrentContext.html
	return CURRENT_EGL_CONTEXT;
}

EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(QueryContext)(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglQueryContext.html
	_EGLDisplay* pDisp	= RetrieveEGLDisplay(dpy);
	_EGLContext* pCtx	= RetrieveEGLContext(ctx);
	 
	if(!pCtx->QueryAttribute(attribute, value)) return EGL_FALSE;

	return EGL_TRUE;
}

//----------------------------------------------------------------------------------------------------
// API
//----------------------------------------------------------------------------------------------------
EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(BindAPI)(EGLenum api){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglBindAPI.html
	switch(api){
	case EGL_OPENGL_API:		// not supported
	case EGL_OPENGL_ES_API:
	case EGL_OPENVG_API:		// not supported
 	//case EGL_OPENRC_API:
		eglClinetAPI	= api;
		break;
	default:
		eglErrorHandler.SetError(EGL_BAD_PARAMETER);
		return EGL_FALSE;
	}
	return EGL_TRUE;
}

EGLAPI EGLenum EGLAPIENTRY EGL_DECLARE(QueryAPI)(void){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglQueryAPI.html
	return eglClinetAPI;
}

EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(WaitClient)(void){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglWaitClient.html
	// don't care this time...
	return EGL_FALSE;
}

EGLAPI EGLBoolean EGLAPIENTRY EGL_DECLARE(ReleaseThread)(void){
	// http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglReleaseThread.html
	// don't care this time...
	return EGL_FALSE;
}

#ifdef EGL_RAYCORE_Extension
EGLAPI void EGLAPIENTRY eglDumpSystemRC(EGLDisplay dpy, const char* filename){
	_EGLDisplay* pDisp	= RetrieveEGLDisplay(dpy);
	if(!pDisp) return;
	pDisp->DumpSystem(filename);
}

EGLAPI EGLBoolean EGLAPIENTRY eglGetReportRegister(EGLDisplay dpy, int& iDeviceID, EGL_REPORT* pReport){
	_EGLDisplay* pDisp	= RetrieveEGLDisplay(dpy);
	if(!pDisp) return EGL_FALSE;
	return pDisp->GetReportRegister(iDeviceID, pReport);
}
#endif

#ifdef ELAPSED_TIME_CHECK
float EGLAPIENTRY eglGetElapsedTime(EGL_ELAPSED_TIME_MARKER id){
	return ELAPSED_TIME_GET(id);
}
#endif
