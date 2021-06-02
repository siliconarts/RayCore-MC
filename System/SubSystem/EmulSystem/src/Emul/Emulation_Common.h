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
// Project : Emulation
// DATE    : 8/28/2013 Wed
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __EMUL_COMMON_H__
#define __EMUL_COMMON_H__
#include "TestDriver.h"
#include "VirtualDisplayConfig.h"
#include "driver/SystemHAL.h"
#include <math.h>
#include "driver/REGMAP.h"
#include "driver/SystemHAL.h"
#include "ddk/rcDDK_defines.h"
#include "common/gl_Object.h"
using namespace GL;

#include "Registers.h"

#define RT24
#define USE_REDUCED_ERROR_CORRECTION

#define RAY_TYPE_NULL				0
#define RAY_TYPE_PRIMARY			(1<<0)
#define RAY_TYPE_REFLECTION			(1<<1)
#define RAY_TYPE_REFRACTION			(1<<2)
#define RAY_TYPE_SHADOW				(1<<3)
#define RAY_TYPE_TEXTURE_SHADOW		(1<<4)
#define RAY_TYPE_SKYBOX				(1<<5)
#define RAY_TYPE_NORMAL				(1<<6)

#define MAX_PRIM					600000
#define MAX_LIGHT					8
#define MAX_MATERIAL				1000
#define MAX_TEXTURE					1000
#define StackSize					4
#define TravStackSize				64

#define RamSize						0x500000

// Data Type
#if defined(RT32)
	typedef float RayFloat;
#else
	typedef unsigned int RayFloat;
#endif

	// Operator
#ifndef RT24
	#define fadder(x, y, mode) ((mode==1) ? (x-y) : (x+y))
	#define TFAdder( x, y, z) ((x)+(y)+(z))
	#define fdivider(x, y) (x)/(y)
	#define fmul(x, y) ((x)*(y))
	#define SQRT(x) ((float)sqrt(x)) 
	#define compare(x, y) ((x) > (y)) ? 1 : ((x) < (y)) ? 2 : 0
	#define fexp(a, b) pow(a, b)
	#define nmod(INFP) mod(INFP)
	#define f32f24(x)	(x)
	#define f32f16(x) (x)
	#define f24f32(x)   (x)
	#define CalculateLod(x, y) log(x)/log(2.)+log(y)/log(2.)
#endif

#ifdef RT24
	#define TmaxGap SectionMaxDist+10
	#define TminGap (SectionMinDist>0x25000a) ? SectionMinDist-5 : 0x250000
	#define TmaxGap1 *tmax+5
	#define TminGap1 *tmin-5

	#define InitTmax 0x7ffff5
	#define SignConv(m) ((m==0) ? 0 : (((~m)&0x800000)|(m&0x7fffff)))
	#include "rc_util.h"
#else
	#define TmaxGap (float)(SectionMaxDist + fabs(SectionMaxDist)*0.0001)
	#define TminGap (float)(SectionMinDist - fabs(SectionMinDist)*0.0001)
	#define TmaxGap1 *tmax + fabs(*tmax)*0.0001
	#define TminGap1 *tmin - fabs(*tmin)*0.0001

	#define InitTmax 2e30f
	#define SignConv(m) (-m)
#endif

// MODE 1 : RayGen
// MODE 2 : Traversal
// MODE 3 : Shading
// MODE 4 : ray trace data file

#ifdef RT24
	#define MODE 2
#else
	#define MODE 0
#endif

// @todo : 지워져야 함.
#include <RC1/rc_structs.h>

typedef struct{
	int NodeCount;
	int ListCount;
	int TriCount;
	int RayCount;
	int RayBoxCount;
	int RayNodeCount;
	int RayTriCount;
	int ShadowCount;
	int ShadowBoxCount;
	int ShadowNodeCount;
	int ShadowTriCount;
	int SITCount;//dk SIT
	int SITnoCount;//dk SIT
}_CountInfo;

#endif//__EMUL_COMMON_H__
