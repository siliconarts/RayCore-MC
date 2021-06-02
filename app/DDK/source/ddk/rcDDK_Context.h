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
// Project : DDK
// DATE    : 1/21/2014 Tue
// Author  : Sang Gon, Kim (sgkim@siliconarts.co.kr)
//
//=======================================================================

#ifndef __RCDDK_CONTEXT_H__
#define __RCDDK_CONTEXT_H__
#include "rcDDK_Common.h"

class _RCDDKMemory :
	public RCDDKMemory
{
protected:
	~_RCDDKMemory(void);

public:
	_RCDDKMemory(IMemory* pNative);
	
	STDMETHOD_(void, Release)(void);

	STDMETHOD_(void*, Virtual)(void);
	STDMETHOD_(unsigned long long int, Physical)(void);
	STDMETHOD_(DWORD, ByteSize)(void);
	STDMETHOD_(void, SetDirty)(DWORD dwOffset, DWORD dwByteSize);
	STDMETHOD_(void, Flush)(BOOL bCopyToPhysical = TRUE);

	inline IMemory* GetNative(void)		{return m_pNative;}

private:
	IMemory*		m_pNative;
};

class rcDDKContext :
	public RCDDK
{
protected:
	virtual ~rcDDKContext(void);

public:
	rcDDKContext(void);

	// life cycle
	STDMETHOD_(void, Release)(void);

	// Display
	STDMETHOD_(BOOL, DisplaySetup)(int width, int height, COLOR_FORMAT format = COLOR_FORMAT_A8B8G8R8, int byte_stride = 0);

	// RGBA Mode
	STDMETHOD_(void, SetRGBAMode)(RCDDK_RGBA_MODE mode = RCDDK_RGBA_MODE_ABGR);

	// RayCore
	STDMETHOD_(BOOL, GetInformation)(RCDDK_INFO* pInfo);
	STDMETHOD_(BOOL, Enable)(RCDDK_ENABLE id, BOOL bEnable = TRUE);
	STDMETHOD_(void, SetMemory)(RCDDK_MEMORY id, RCDDKMemory* pMemory);
	STDMETHOD_(unsigned long long, GetMemoryBase)(void);

	// Render
	STDMETHOD_(BOOL, RenderStart)(void);
	STDMETHOD_(BOOL, RenderEnd)(RCDDKMemory* pSurface);

	// Option
	STDMETHOD_(void, SetCRRNInfo)(DWORD number, DWORD baddr);
	STDMETHOD_(void, SetPixelSize)(float width, float height);
	STDMETHOD_(void, SetStartPosition)(float x, float y);
	STDMETHOD_(void, SetBoundBox)(DWORD RootNodeID, float* pMinMax);
	STDMETHOD_(void, SetBackgroundColor)(float red, float green, float blue);
	STDMETHOD_(void, SetRayThresholdWeight)(float threshold);
	STDMETHOD_(void, SetLightCount)(DWORD count);
	STDMETHOD_(void, SetBoundDepth)(DWORD depth);
	STDMETHOD_(void, SetAdaptiveRender)(BOOL bEnable, DWORD size = 10);
	//// light radius
	//STDMETHOD_(void, SetLightRadius)(unsigned int pow = 9);
	//// light radius

	//// diffuse generation rate
	//STDMETHOD_(void, SetDiffuseRate)(unsigned int rate = 10);
	//// diffuse generation rate

	// Reserved value
	// Currently, this function is used to set the light radius or the diffuse-generation rate
	// type = 0: light radius
	// type = 1: diffuse-geration rate
	STDMETHOD_(void, SetReservedValue)(unsigned int value = 9, unsigned int type = LightRadius);
	// Reserved value

	// Camera
	STDMETHOD_(void, SetCameraParams)(RCDDK_CAMERA id, float* pParams);
	STDMETHOD_(void, SetCameraAttenuation)(float start, float end, float* pWeight);

	// Light
	STDMETHOD_(void, SetLightParams)(DWORD index, RCDDK_LIGHT id, float* pParams);
	STDMETHOD_(void, SetLightAttenuation)(DWORD index, RCDDK_LIGHT_TYPE type, float start, float end, float* pWeight);

	// Material
	STDMETHOD_(void, SetMaterialParams)(DWORD index, RCDDK_MATERIAL id, const float* pParams);
	STDMETHOD_(void, SetMaterialTexture)(DWORD index, RCDDK_TEXTURE_TYPE type, BOOL bEnable, unsigned long long dwPhyAddress, DWORD log_width, DWORD log_height, BOOL bMipmap);
	STDMETHOD_(void, SetMaterialTextureWrap)(DWORD index, RCDDK_TEXTURE_TYPE type, RCDDK_TEXTURE_WRAP mode, BOOL U, BOOL V);

	// System dump save/load
	STDMETHOD_(BOOL, DumpSystem)(const char* file_name, PFUNC_DUMPSYSTEM dump_func = NULL);
	STDMETHOD_(BOOL, ReportRegister)(int& iDeviceID, EGL_REPORT* pReport);

	BOOL LoadDumpSystem(const char* file_name);

	inline BOOL IsInitialized(void)			{return m_pSystem != NULL;}
	inline static ISystem* GetSystem(void)	{return m_pSystem;}
	inline MULTI_LOAD* GetLoadBalance(void)	{return &m_LoadBlance;}
	static rcDDKContext* GET_CURRENT(void)	{return m_pCurrent;}

	void SetCoreMask(int count);

private:
	static rcDDKContext*	m_pCurrent;		// current DDK context
	static ISystem*			m_pSystem;		// system driver interface
	static int				m_iRefCount;	// system driver reference count

	// register dirty chain declare
	DIRTY_CHAIN_DECLARE
	RCDDK_REGISTERS			m_Reg;			// registers
	RCDDK_MEMORIES			m_Mem;			// memories

	MULTI_LOAD				m_LoadBlance;	// load balance

#ifdef USE_TESTDRIVE
	ITestDriverMemory*		m_pTDMemory;
	TD_REGMAP*				m_pTDRegmap;
#endif
};

#endif//__RCDDK_CONTEXT_H__
