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
// Project : RayCore
// DATE    : 8/29/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#ifndef __EGL_REPORT_REGISTERS_H__
#define __EGL_REPORT_REGISTERS_H__
#ifdef EGL_RAYCORE_Extension
#include <EGL/egl.h>
#endif

#define MAX_CORE_SIZE	8

typedef struct{
	unsigned int		P0ILReqCount;
	unsigned int		P1ILReqCount;
	unsigned int		P0ILL2ReqCount;
	unsigned int		P1ILL2ReqCount;
}EGL_REPORT_IL;

typedef struct{
	unsigned int		P0ListReqCount;
	unsigned int		P1ListReqCount;
	unsigned int		P0ListL2ReqCount;
	unsigned int		P1ListL2ReqCount;
}EGL_REPORT_LIST;

typedef struct{
	unsigned int		P0TransmitReqCount;
	unsigned int		P1TransmitReqCount;
	unsigned int		P0TransmitL2ReqCount;
	unsigned int		P1TransmitL2ReqCount;
}EGL_REPORT_TRANSMIT;

typedef struct{
	unsigned int		RayGenCount0;
	unsigned int		RayGenRealCount0;
	unsigned int		RayGenCount1;
	unsigned int		RayGenRealCount1;
	unsigned int		CullingCount;
	unsigned int		Trav0Count;
	unsigned int		Trav1Count;
	unsigned int		Trav0RealCount;
	unsigned int		Trav1RealCount;
	unsigned int		Trav0RayCount;
	unsigned int		Trav1RayCount;
	unsigned int		Con4Count;
	unsigned int		Con4RealCount;
	unsigned int		Shading0Count;
	unsigned int		Shading0RealCount;
	unsigned int		TexFiFoWaitCount;
	unsigned int		TexFiFoFullCount;
	unsigned int		Shading1Count;
	unsigned int		Shading1RealCount;
	unsigned int		Shading2Count;
	unsigned int		Shading2RealCount;
	unsigned int		Shading3Count;
	unsigned int		Shading3RealCount;
}EGL_REPORT_RENDER;

typedef struct{
	unsigned int		device_id;
	unsigned int		clock_mhz;
	unsigned int		core_count;
	
	unsigned int		thread_count;
	
	unsigned int		ray_count[MAX_CORE_SIZE];
	
	unsigned int		BusReadCount;
	unsigned int		BusWriteCount;
	
	unsigned int		ILL2MissCount;
	EGL_REPORT_IL		IL[MAX_CORE_SIZE];
	
	unsigned int		ListL2MissCount;
	EGL_REPORT_LIST		List[MAX_CORE_SIZE];
	
	unsigned int		TransmitL2MissCount;
	EGL_REPORT_TRANSMIT	Transmit[MAX_CORE_SIZE];
	
	unsigned int		TriMemMissCount;
	unsigned int		TriMemReqCount[MAX_CORE_SIZE];
	
	unsigned int		TexMemMissCount;
	unsigned int		TexMemReqCount[MAX_CORE_SIZE];
	
	unsigned int		TexNormalMemMissCount;
	unsigned int		TexNormalMemReqCount[MAX_CORE_SIZE];
	
	EGL_REPORT_RENDER	Render[MAX_CORE_SIZE];
	unsigned int		reserved;
}EGL_REPORT;

#ifdef EGL_RAYCORE_Extension
typedef EGLBoolean (EGLAPIENTRY *PFNEGLGETREPORTREGISTERPROC) (EGLDisplay dpy, int& iDeviceID, EGL_REPORT* pReport);
#define GET_EGLREPORTREGISTERPROC	(PFNEGLGETREPORTREGISTERPROC)eglGetProcAddress("eglGetReportRegister")
#endif

#endif
