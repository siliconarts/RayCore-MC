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
// DATE    : 8/29/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "RCFramework.h"
#include <stdio.h>
#include <vector>
static RCFramework*	g_pFramework	= NULL;

// light radius
static constexpr float sMinLightRadius = 0.f;
static constexpr float sMaxLightRadius = 8191.f;
// light radius

// spp count
static constexpr unsigned int sMinSppCount = 1;
static constexpr unsigned int sMaxSppCount = 32;
// spp count

// diffuse rate
static constexpr unsigned int sMinDiffuseRate = 0;
static constexpr  unsigned int sMaxDiffuseRate = 10;
// diffuse rate


RCFramework::RCFramework(const char* sTitleName, int width, int height) 
	: m_roughness(0.f), m_isLOD(TRUE), m_nDepthBounce(10), m_nRenderQuality(1), m_nThresholdQuality(0), m_nSkipFrame(0),
	m_sTitle (sTitleName) 
{
	m_eglDisplay		= EGL_NO_DISPLAY;
	m_eglContext		= EGL_NO_CONTEXT;
	m_eglSurface		= EGL_NO_SURFACE;
	m_sTitle			= sTitleName;
	m_iWidth			= width;
	m_iHeight			= height;
	g_pFramework		= this;

	m_isLOD = TRUE;
	m_nDepthBounce = 10;
	m_nRenderQuality = 1;
	m_nThresholdQuality = 0;
	m_nSkipFrame = 0;
}

RCFramework::~RCFramework(void){
	g_pFramework	= NULL;
}

BOOL RCFramework::Initialize(int argc, char ** argv){
	if(m_eglContext) return FALSE;

	{
		m_Cmd.argc	= argc;
		m_Cmd.argv	= argv;
		if(argc>=4){
			m_iWidth	= (atoi(argv[2])/16) * 16;
			m_iHeight	= (atoi(argv[3])/16) * 16;
			printf("Current resolution : %dx%d\n", m_iWidth, m_iHeight);
		}
	}

	if(!Create(m_sTitle, m_iWidth, m_iHeight)) return FALSE;

	{
		EGLint	attribConfig[] = {
			EGL_RED_SIZE,	8,
			EGL_GREEN_SIZE,	8,
			EGL_BLUE_SIZE,	8,
			EGL_DEPTH_SIZE,	16,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
			EGL_NONE
		};
		EGLint	attribContext[] = {~
			EGL_CONTEXT_CLIENT_VERSION, 1,
			EGL_NONE
		};
		EGLConfig eglConfig;
		EGLint nConfigs = 0;

		m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		if(eglGetError() != EGL_SUCCESS)
		{
			printf("There is an error while the function, eglGetDisplay.\n");
			return FALSE;
		}
		eglInitialize(m_eglDisplay, NULL, NULL);
		if(eglGetError() != EGL_SUCCESS)
		{
			printf("There is an error while the function, eglInitialize.\n");
			return FALSE;
		}
		eglChooseConfig(m_eglDisplay, attribConfig, &eglConfig, 1, &nConfigs);
		if(eglGetError() != EGL_SUCCESS)
		{
			printf("There is an error while the function, eglChooseConfig.\n");
			return FALSE;
		}
		if(nConfigs == 0)
		{
			printf("There is no config selected.\n");
			return FALSE;
		}

		m_eglSurface = eglCreateWindowSurface(m_eglDisplay, eglConfig, GetWindowHandle(), 0);
		if(eglGetError() != EGL_SUCCESS)
		{
			printf("There is an error while the function, eglCreateWindowSurface.\n");
			return FALSE;
		}
		m_eglContext = eglCreateContext(m_eglDisplay, eglConfig, EGL_NO_CONTEXT, attribContext);
		if(eglGetError() != EGL_SUCCESS)
		{
			printf("There is an error while the function, eglCreateContext.\n");
			return FALSE;
		}
		if(eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext) != EGL_TRUE)
		{
			printf("There is an error while the function, eglMakeCurrent.\n");
			return FALSE;
		}
	}

	m_isLOD = TRUE;
	m_nDepthBounce = 3;
	m_nRenderQuality = 1;
	m_nThresholdQuality = 0;
	m_nSkipFrame = 0;

	rcEnable(RC_LIGHTING);
	rcEnable(RC_USE_SHADOW);
	rcHint(RC_MIPMAP_HINT, RC_FASTEST);
	rcHint(RC_RAYBOUNCE_THRESHOLD_HINT, RC_THRESHOLD_LEVEL0);
	
	rcHint(RC_RENDERING_HINT, RC_FASTEST);
	if(m_Cmd.argc >= 5){
		switch(atoi(m_Cmd.argv[4])){
		case 0:rcHint(RC_RENDERING_HINT, RC_DONT_CARE);				break;
		case 1:rcHint(RC_RENDERING_HINT, RC_FASTEST);				break;
		case 2:rcHint(RC_RENDERING_HINT, RC_NICEST);				break;
		case 3:rcHint(RC_RENDERING_HINT, RC_FASTEST_AND_NICEST);	break;
		default:break;
		}
	}
	
	

	
	rcDepthBounce(1);	

	// light radius
	m_lightRadius = 100.0f;
	rcSetLightRadius(m_lightRadius);
	// light radius

	// diffuse rate
	m_diffuseRate = 10;
	rcSetDiffuseRate(m_diffuseRate);
	// diffuse rate

	// spp
	m_sppCount = 8;
	rcSetSPPCount(m_sppCount); 
	// spp

	if(!OnIntialize()) return FALSE;

	return TRUE;
}

void RCFramework::Release(void){
	OnRelease();
	{
		if(m_eglDisplay) eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if(m_eglSurface != EGL_NO_SURFACE) eglDestroySurface(m_eglDisplay, m_eglSurface);
		if(m_eglContext != EGL_NO_CONTEXT) eglDestroyContext(m_eglDisplay, m_eglContext);
		if(m_eglDisplay != EGL_NO_DISPLAY) eglTerminate(m_eglDisplay);
		m_eglDisplay	= EGL_NO_DISPLAY;
		m_eglContext	= EGL_NO_CONTEXT;
		m_eglSurface	= EGL_NO_SURFACE;
	}
	Destroy();
}

void RCFramework::SetDepthBounce(unsigned int depth)
{
	if(depth > 14) depth = 14;
	rcDepthBounce(depth);
	m_nDepthBounce = depth;
}
void RCFramework::SetLightRadius(float radius){
	rcSetLightRadius(radius);
}
void RCFramework::SetSPPCount(unsigned int nSpp) {
	rcSetSPPCount(nSpp);
}
void RCFramework::SetDiffuseRate(unsigned int rate) {
	rcSetDiffuseRate(rate);
}
BOOL RCFramework::DrawScene(void){
	if(!PumpMessage()) return FALSE;	
	if(!OnDraw()) return FALSE;			

	if(rcGetError() == RC_NO_ERROR)
	{
		rcFinish();

		eglSwapBuffers(m_eglDisplay, m_eglSurface);

		if(!PumpMessage()) return FALSE;
	}else{
		printf("There was an error while drawing scene.\n");
		return FALSE;
	}

	OnDrawComplete();
	if(!OnPostDraw()) return FALSE;

	return TRUE;
}

#ifdef EGL_RAYCORE_Extension
#include "debug/egl_ReportRegisters.h"
void ReportRegister(EGLDisplay dpy){
	PFNEGLGETREPORTREGISTERPROC	GetReportRegister	= GET_EGLREPORTREGISTERPROC;
	if(!GetReportRegister){
		printf("Can't find 'eglGetReportRegister'.\n");
		return;
	}
	{
		int			device_count;
		EGL_REPORT	report;

		GetReportRegister(dpy, device_count, NULL);

		for(int i=0;i<device_count;i++){
			if(!GetReportRegister(dpy, i, &report)) break;

			printf("--------------------------------------------------------------\n");
			printf("device(%d/%d) : ID(0x%X)\n", i+1, device_count, report.device_id);
			printf("	core_count                    = %d\n", report.core_count);
			printf("	H/W FPS                       = %.2f\n", (report.clock_mhz * 1000000.f)/ report.thread_count);

			{
				DWORD total_count	= 0;
				printf("	ray_count                     =");
				
				for(unsigned int t=0;t<report.core_count;t++){
					if(t) printf(" +");
					printf(" %d", report.ray_count[t]);
					total_count	+= report.ray_count[t];
				}
				printf(" : total = %d\n", total_count);
			}

			printf("	BusReadCount                  = %d\n", report.BusReadCount);
			printf("	BusWriteCount                 = %d\n", report.BusWriteCount);


			for(unsigned int t=0;t<report.core_count;t++){
				DWORD total_count;
				printf("	Core #%d Cache Miss Rate (%%)\n", t);

				// Primitive node
				total_count	= (report.IL[t].P0ILReqCount + report.IL[t].P1ILReqCount) * 100;
				if(!total_count) total_count = 1;
				printf("	    PrimitiveNode L1 Cache    = %f%%\n", (float)(report.IL[t].P0ILL2ReqCount + report.IL[t].P1ILL2ReqCount) / total_count);
				total_count	= (report.IL[t].P0ILL2ReqCount + report.IL[t].P1ILL2ReqCount) * 100;
				if(!total_count) total_count = 1;
				printf("	    PrimitiveNode L1 Cache    = %f%%\n", (float)(report.ILL2MissCount) / total_count);

				// List
				total_count	= (report.List[t].P0ListReqCount + report.List[t].P1ListReqCount) * 100;
				if(!total_count) total_count = 1;
				printf("	    List L1 Cache             = %f%%\n", (float)(report.List[t].P0ListL2ReqCount + report.List[t].P1ListL2ReqCount) / total_count);
				total_count	= (report.List[t].P0ListL2ReqCount + report.List[t].P1ListL2ReqCount) * 100;
				if(!total_count) total_count = 1;
				printf("	    List L1 Cache             = %f%%\n", (float)(report.ListL2MissCount) / total_count);

				// Transmit
				total_count	= (report.Transmit[t].P0TransmitReqCount + report.Transmit[t].P1TransmitReqCount) * 100;
				if(!total_count) total_count = 1;
				printf("	    Transmit L1 Cache         = %f%%\n", (float)(report.Transmit[t].P0TransmitL2ReqCount + report.Transmit[t].P1TransmitL2ReqCount) / total_count);
				total_count	= (report.Transmit[t].P0TransmitL2ReqCount + report.Transmit[t].P1TransmitL2ReqCount) * 100;
				if(!total_count) total_count = 1;
				printf("	    Transmit L1 Cache         = %f%%\n", (float)(report.TransmitL2MissCount) / total_count);

				// PrimitiveInfo Memory
				total_count	= (report.Transmit[t].P0TransmitReqCount + report.Transmit[t].P1TransmitReqCount) * 100;
				if(!total_count) total_count = 1;
			}

			{
				DWORD total_count;
				// Primitive info.
				total_count	= 0;
				for(unsigned int t=0;t<report.core_count;t++) total_count += report.TriMemReqCount[t];
				if(total_count) total_count *= 100; else total_count = 1;
				printf("	PrimitiveInfo $ Miss Rate     = %f%%\n", (float)report.TriMemMissCount / total_count);

				// Texture memory
				total_count	= 0;
				for(unsigned int t=0;t<report.core_count;t++) total_count += report.TexMemReqCount[t];
				if(total_count) total_count *= 100; else total_count = 1;
				printf("	Texture $ Miss Rate           = %f%%\n", (float)report.TexMemMissCount / total_count);

				// Normal Texture memory
				total_count	= 0;
				for(unsigned int t=0;t<report.core_count;t++) total_count += report.TexNormalMemReqCount[t];
				if(total_count) total_count *= 100; else total_count = 1;
				printf("	Normal Texture $ Miss Rate    = %f%%\n", (float)report.TexNormalMemMissCount / total_count);

			}

			for(unsigned int t=0;t<report.core_count;t++){
				EGL_REPORT_RENDER*	pRender	= &report.Render[t];
				DWORD	ray_count	= report.ray_count[t] ? report.ray_count[t] : 1;

				printf("	Core #%d Performance (clock/ray)\n", t);
				printf("	    Ray Generation #1          = %.3f\n", (float)pRender->RayGenCount0 / ray_count);
				printf("	    Ray Generation #2          = %.3f\n", (float)pRender->RayGenCount1 / ray_count);
				printf("	    Ray Generation Real #1     = %.3f\n", (float)pRender->RayGenRealCount0 / ray_count);
				printf("	    Ray Generation Real #2     = %.3f\n", (float)pRender->RayGenRealCount1 / ray_count);
				printf("	    Culling                    = %.3f\n", (float)pRender->CullingCount / ray_count);
				printf("	    Traversal #1               = %.3f\n", (float)pRender->Trav0Count / ray_count);
				printf("	    Traversal #2               = %.3f\n", (float)pRender->Trav1Count / ray_count);
				printf("	    Traversal Real #1          = %.3f\n", (float)pRender->Trav0RealCount / ray_count);
				printf("	    Traversal Real #2          = %.3f\n", (float)pRender->Trav1RealCount / ray_count);
				printf("	    Traversal Ray Count #1     = %d\n", pRender->Trav0RayCount);
				printf("	    Traversal Ray Count #2     = %d\n", pRender->Trav1RayCount);
				printf("	    Control                    = %.3f\n", (float)pRender->Con4Count / ray_count);
				printf("	    Control Real               = %.3f\n", (float)pRender->Con4RealCount / ray_count);
				printf("	    Shading #1                 = %.3f\n", (float)pRender->Shading0Count / ray_count);
				printf("	    Shading #2                 = %.3f\n", (float)pRender->Shading1Count / ray_count);
				printf("	    Shading #3                 = %.3f\n", (float)pRender->Shading2Count / ray_count);
				printf("	    Shading #4                 = %.3f\n", (float)pRender->Shading3Count / ray_count);
				printf("	    Shading Real #1            = %.3f\n", (float)pRender->Shading0RealCount / ray_count);
				printf("	    Shading Real #2            = %.3f\n", (float)pRender->Shading1RealCount / ray_count);
				printf("	    Shading Real #3            = %.3f\n", (float)pRender->Shading2RealCount / ray_count);
				printf("	    Shading Real #4            = %.3f\n", (float)pRender->Shading3RealCount / ray_count);
				printf("	    Texture FiFo Wait          = %.3f\n", (float)pRender->TexFiFoWaitCount / ray_count);
				printf("	    Texture FiFo Full          = %.3f\n", (float)pRender->TexFiFoFullCount / ray_count);
			}
		}
		_flushall();
	}
}
#endif

void RCFramework::KeyDownEvent(char cKey){
	switch(cKey){
	case 'I':	//Initialize
		{
			m_isLOD = TRUE;
			m_nDepthBounce = 1;
			m_nRenderQuality = 1;
			m_nThresholdQuality = 0;
			m_nSkipFrame = 0;
			m_diffuseRate = 10;
			m_sppCount = 8;	
			m_lightRadius = 100.0f;




			rcEnable(RC_LIGHTING);
			rcEnable(RC_USE_SHADOW);
			rcHint(RC_MIPMAP_HINT, RC_FASTEST);
			rcHint(RC_RAYBOUNCE_THRESHOLD_HINT, RC_THRESHOLD_LEVEL0);
			rcHint(RC_RENDERING_HINT, RC_FASTEST);
			rcDepthBounce(m_nDepthBounce);
			rcSetDiffuseRate(m_diffuseRate);
			rcSetSPPCount(m_sppCount);
			rcSetLightRadius(m_lightRadius);			

			OnKeyDown(cKey);
		}
		break;
#ifdef EGL_RAYCORE_Extension
	case 'P':	// report register
		ReportRegister(m_eglDisplay);
		break;
#endif
	case 'S':	//Shadow On/Off
		{
			if(rcIsEnabled(RC_USE_SHADOW))	
				rcDisable(RC_USE_SHADOW);
			else							
				rcEnable(RC_USE_SHADOW);
		}
		break;
	case 'X':	//Texture LOD On/Off
		{
			if(m_isLOD)		rcHint(RC_MIPMAP_HINT, RC_NICEST);
			else			rcHint(RC_MIPMAP_HINT, RC_FASTEST);
			m_isLOD = !m_isLOD;	
		}
		break;
	case 'Q':	//RayBounce Threshold Level Increse
		{
			if(m_nThresholdQuality < 8) m_nThresholdQuality++;

			if(m_nThresholdQuality > 0)
				rcHint(RC_RAYBOUNCE_THRESHOLD_HINT, RC_THRESHOLD_LEVEL0 + m_nThresholdQuality - 1);
			else
				rcHint(RC_RAYBOUNCE_THRESHOLD_HINT, RC_DONT_CARE);
		}
		break;
	case 'W':	//RayBounce Threshold Level Decrease
		{
			if(m_nThresholdQuality > 0) m_nThresholdQuality--;

			if(m_nThresholdQuality > 0)
				rcHint(RC_RAYBOUNCE_THRESHOLD_HINT, RC_THRESHOLD_LEVEL0 + m_nThresholdQuality - 1);
			else
				rcHint(RC_RAYBOUNCE_THRESHOLD_HINT, RC_DONT_CARE);
		}
		break;
	// light radius
	case 'H':
		if (m_lightRadius > sMinLightRadius) {
			m_lightRadius -= 1.0f;
			rcSetLightRadius(m_lightRadius);
			printf("LightRadius:%f\n",m_lightRadius);
		}
	break;
	
	case 'J':
		if (m_lightRadius < sMaxLightRadius) {
			m_lightRadius += 1.0f;
			rcSetLightRadius(m_lightRadius);
			printf("LightRadius:%f\n",m_lightRadius);
		}
	break;
	// light radius
	// diffuse rate
	case 'K':
		if (m_diffuseRate > sMinDiffuseRate) {
			--m_diffuseRate;
			rcSetDiffuseRate(m_diffuseRate);
			printf("DiffuseRate:%d\n",m_diffuseRate);
		}
	break;
	
	case 'L':
		if (m_diffuseRate < sMaxDiffuseRate) {
			++m_diffuseRate;
			rcSetDiffuseRate(m_diffuseRate);
			printf("DiffuseRate:%d\n",m_diffuseRate);
		}
	break;
	// diffuse rate
	// spp
	case 'T':
		{
			if(m_sppCount > sMinSppCount){	
				--m_sppCount;				
				rcSetSPPCount(m_sppCount);
				printf("Spp:%d\n",m_sppCount);
			}
		}
	break;

	case 'Y':
		{
			if(m_sppCount < sMaxSppCount){
				++m_sppCount;				
				rcSetSPPCount(m_sppCount);
				printf("Spp:%d\n",m_sppCount);
			}
		}
		break;
	// spp
	case 91://+	//Depth Bounce Increase
		{
			if(m_nDepthBounce < 14)	++m_nDepthBounce;
			printf("DepthBounce:%d\n",m_nDepthBounce);
			rcDepthBounce(m_nDepthBounce);
		}
		break;
	case 92://-	//Depth Bounce Decrease
		{
			if(m_nDepthBounce > 0)	--m_nDepthBounce;
			printf("DepthBounce:%d\n",m_nDepthBounce);
			rcDepthBounce(m_nDepthBounce);
		}
		break;

	case 95://\A1\E7//Frame Speed Down
		if(m_nSkipFrame > 1) m_nSkipFrame--;
		break;
	case 96://\A1\E6//Frame Speed Up
		if(m_nSkipFrame < 3) m_nSkipFrame++;
		break;
	default:
		OnKeyDown(cKey);
		break;
	}
}

int RCFramework::SkipFrame(void)
{
	int frameStep=1;
	
	for(DWORD i=0; i<m_nSkipFrame; i++)
		frameStep *= 2;

	return frameStep;
}

BOOL RCFramework::OnIntialize(void){
	return TRUE;
}

void RCFramework::OnRelease(void){
}

BOOL RCFramework::OnDraw(void){
	return TRUE;
}

BOOL RCFramework::OnPostDraw(void){
	return TRUE;
}

void RCFramework::OnKeyDown(char cKey){
}

void RCFramework::OnMouseEvent(float x, float y, DWORD dwBtn){
}

int main(int argc, char ** argv)
{
	if(g_pFramework->Initialize(argc, argv))
		while(g_pFramework->DrawScene());
	
	g_pFramework->Release();

	return 0;
}
