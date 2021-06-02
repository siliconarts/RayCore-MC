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
// DATE    : 1/21/2014 Tue
// Author  : Sang Gon, Kim (sgkim@siliconarts.co.kr)
//
//=======================================================================
#ifndef __RCDDK_H__
#define __RCDDK_H__
#include "STDInterface.h"
#include "rcDDK_Defines.h"
#include "debug/egl_ReportRegisters.h"

#ifdef WIN32
	#ifdef RCDDK_EXPORTS
		#define RCDDK_API		__declspec(dllexport)
	#else
		#define RCDDK_API		__declspec(dllimport)
		#if defined(_WIN64)
			#pragma comment(lib, "RCDDK64.lib")
		#elif defined(_WIN32)
			#pragma comment(lib, "RCDDK.lib")
		#endif
	#endif
	#define RCDDK_ENTRY			__stdcall
#else// linux
	#define RCDDK_API			extern
	#define RCDDK_ENTRY
#endif

typedef enum{
	COLOR_FORMAT_R5G6B5,
	COLOR_FORMAT_A8B8G8R8,
}COLOR_FORMAT;

typedef enum{	// rgba mode
	RCDDK_RGBA_MODE_ABGR,		// mode 0 : A B G R
	RCDDK_RGBA_MODE_BGRA,		// mode 1 : B G R A
	RCDDK_RGBA_MODE_ARGB,		// mode 2 : A R G B
	RCDDK_RGBA_MODE_RGBA,		// mode 3 : R G B A
}RCDDK_RGBA_MODE;

typedef enum{	// light parameters
	RCDDK_LIGHT_POSITION,		// x,y,z
	RCDDK_LIGHT_DIRECTION,		// x,y,z
	RCDDK_LIGHT_SPOT_COS,		// in,out,falloff
	RCDDK_LIGHT_AMBIENT,		// r,g,b
	RCDDK_LIGHT_DIFFUSE,		// r,g,b
	RCDDK_LIGHT_SPECULAR,		// r,g,b
}RCDDK_LIGHT;

typedef enum{	// light type
	RCDDK_LIGHT_TYPE_POINT,			// point light
	RCDDK_LIGHT_TYPE_DIRECTIONAL,	// directional light
	RCDDK_LIGHT_TYPE_SPOT,			// spot light
}RCDDK_LIGHT_TYPE;

typedef enum{	// camera paramters
	RCDDK_CAMERA_EYE,			// x,y,z
	RCDDK_CAMERA_BASIS0,		// x,y,z
	RCDDK_CAMERA_BASIS1,		// x,y,z
	RCDDK_CAMERA_BASIS2,		// x,y,z
	RCDDK_CAMERA_DISTANCE,		// distance
}RCDDK_CAMERA;

typedef enum{	// material parameters
	RCDDK_MATERIAL_AMBIENT,
	RCDDK_MATERIAL_DIFFUSE,
	RCDDK_MATERIAL_SPECULAR,
	RCDDK_MATERIAL_SPECULAR_LEVEL,
	RCDDK_MATERIAL_EXPONENT,
	RCDDK_MATERIAL_REFRACTION,
	RCDDK_MATERIAL_REFLECTION,
	RCDDK_MATERIAL_TRANSMITTANCE,

	// roughness
	RCDDK_MATERIAL_ROUGHNESS
	// roughness

}RCDDK_MATERIAL;

typedef enum{	// texture warp mode
	RCDDK_TEXTURE_TYPE_DIFFUSE,
	RCDDK_TEXTURE_TYPE_NORMAL,
}RCDDK_TEXTURE_TYPE;

typedef enum{	// texture warp mode
	RCDDK_TEXTURE_WRAP_REPEAT,
	RCDDK_TEXTURE_WRAP_MIRROR,
}RCDDK_TEXTURE_WRAP;

typedef enum{	// enable properties
	RCDDK_ENABLE_SHADOW,
	RCDDK_ENABLE_COLOR_SHADOW,
	RCDDK_ENABLE_LOD,
	RCDDK_ENABLE_SUPERSAMPLING,
	RCDDK_ENABLE_NORMALMAP,
}RCDDK_ENABLE;

typedef enum{
	RCDDK_MEMORY_NODE,
	RCDDK_MEMORY_LIST,
	RCDDK_MEMORY_PRIMITIVE,
	RCDDK_MEMORY_PRIMITIVE_INFO,
	RCDDK_MEMORY_TRANSMIT,
	RCDDK_MEMORY_FRAMEBUFFER,
}RCDDK_MEMORY;

// Reserved value
enum ValueType {
	LightRadius = 0,
	DiffuseRate, // diffuse-generation rate
	SppCount,

	ValueCount
};
// Reserved value

typedef struct{
	DWORD	magic_code;		// "SARC" = 0x43524153
	DWORD	series_id		: 16;
	DWORD	/*dummy*/		: 0;
	DWORD	raycore_id		: 16;
	DWORD	/*dummy*/		: 11;
	DWORD	ddr_size		: 4;
	DWORD	/*dummy*/		: 0;
	DWORD	build_day		: 8;
	DWORD	build_month		: 8;
	DWORD	build_year		: 16;
}RCDDK_INFO;

interface RCDDKMemory{	// system memory interface
	STDMETHOD_(void, Release)(void) PURE;									// release this memory

	STDMETHOD_(void*, Virtual)(void) PURE;									// virtual memory pointer
	//final
	STDMETHOD_(unsigned long long int, Physical)(void) PURE;									// physical memory address
	STDMETHOD_(DWORD, ByteSize)(void) PURE;									// byte allocation size
	STDMETHOD_(void, SetDirty)(DWORD dwOffset, DWORD dwByteSize) PURE;		// set dirty memory range
	STDMETHOD_(void, Flush)(BOOL bCopyToPhysical = TRUE) PURE;				// Flush memory
};

typedef void (*PFUNC_DUMPSYSTEM)(BOOL bMemory, DWORD dwPhyAddress, DWORD dwByteSize, PVOID pData);

interface RCDDK{
	// life cycle
	STDMETHOD_(void, Release)(void) PURE;

	// Display
	STDMETHOD_(BOOL, DisplaySetup)(int width, int height, COLOR_FORMAT format = COLOR_FORMAT_A8B8G8R8, int byte_stride = 0) PURE;	// setup display device

	// RGBA Mode
	STDMETHOD_(void, SetRGBAMode)(RCDDK_RGBA_MODE mode = RCDDK_RGBA_MODE_ABGR) PURE;

	// RayCore
	STDMETHOD_(BOOL, GetInformation)(RCDDK_INFO* pInfo) PURE;
	STDMETHOD_(BOOL, Enable)(RCDDK_ENABLE id, BOOL bEnable = TRUE) PURE;
	STDMETHOD_(void, SetMemory)(RCDDK_MEMORY id, RCDDKMemory* pMemory) PURE;
	STDMETHOD_(unsigned long long, GetMemoryBase)(void) PURE;

	// Render
	STDMETHOD_(BOOL, RenderStart)(void) PURE;
	STDMETHOD_(BOOL, RenderEnd)(RCDDKMemory* pSurface) PURE;

	// Option
	STDMETHOD_(void, SetCRRNInfo)(DWORD number, DWORD baddr) PURE;
	STDMETHOD_(void, SetPixelSize)(float width, float height) PURE;
	STDMETHOD_(void, SetStartPosition)(float x, float y) PURE;
	STDMETHOD_(void, SetBoundBox)(DWORD RootNodeID, float* pMinMax) PURE;
	STDMETHOD_(void, SetBackgroundColor)(float red, float green, float blue) PURE;
	STDMETHOD_(void, SetRayThresholdWeight)(float threshold) PURE;
	STDMETHOD_(void, SetBoundDepth)(DWORD depth) PURE;
	STDMETHOD_(void, SetAdaptiveRender)(BOOL bEnable, DWORD size = 16) PURE;	// large size is more faster. (0 ~ 63)
	
	// Reserved value
	STDMETHOD_(void, SetReservedValue)(unsigned value = 9, unsigned type = 0) PURE;
	// Reserved value

	// Camera
	STDMETHOD_(void, SetCameraParams)(RCDDK_CAMERA id, float* pParams) PURE;
	STDMETHOD_(void, SetCameraAttenuation)(float start, float end, float* pWeight) PURE;

	// Light
	STDMETHOD_(void, SetLightCount)(DWORD count) PURE;
	STDMETHOD_(void, SetLightParams)(DWORD index, RCDDK_LIGHT id, float* pParams) PURE;
	STDMETHOD_(void, SetLightAttenuation)(DWORD index, RCDDK_LIGHT_TYPE type, float start, float end, float* pWeight) PURE;

	// Material
	STDMETHOD_(void, SetMaterialParams)(DWORD index, RCDDK_MATERIAL id, const float* pParams) PURE;
	STDMETHOD_(void, SetMaterialTexture)(DWORD index, RCDDK_TEXTURE_TYPE type, BOOL bEnable, unsigned long long dwPhyAddress, DWORD log_width, DWORD log_height, BOOL bMipmap) PURE;
	STDMETHOD_(void, SetMaterialTextureWrap)(DWORD index, RCDDK_TEXTURE_TYPE type, RCDDK_TEXTURE_WRAP mode, BOOL U, BOOL V) PURE;

	STDMETHOD_(BOOL, DumpSystem)(const char* file_name = NULL, PFUNC_DUMPSYSTEM dump_func = NULL) PURE;
	STDMETHOD_(BOOL, ReportRegister)(int& iDeviceID, EGL_REPORT* pReport) PURE;
};

RCDDK_API RCDDK* RCDDK_ENTRY CreateRCDDK(void);
RCDDK_API BOOL RCDDK_ENTRY LoadDumpScene(const char* file_name);

RCDDK_API RCDDKMemory* RCDDK_ENTRY RCDDK_CreateMemory(DWORD dwByteSize, DWORD dwByteAlignment = 8, BOOL bForceToDMA = FALSE);

#endif//__RCDDK_H__
