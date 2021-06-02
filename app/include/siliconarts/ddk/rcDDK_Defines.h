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
// DATE    : 5/28/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#ifndef __RCDDK_DEFINES_H__
#define __RCDDK_DEFINES_H__

//}sjpark - 20190408 
//}Max Light Change 
//#define	RCDDK_MAX_LIGHT_COUNT			8
#define	RCDDK_MAX_LIGHT_COUNT				1024
//}sjpark
#define	RCDDK_MAX_MATERIAL_COUNT			512
#define	RCDDK_MAX_MATRIX_STACK_COUNT		32		// @todo : DDK가 아니라 API 쪽으로 옮겨져야 한다.
#define	RCDDK_MAX_MATRIX_PALETTE_COUNT		128		// @todo : DDK가 아니라 API 쪽으로 옮겨져야 한다. 
#define	RCDDK_MAX_VERTEX_UNITS_COUNT		128		// @todo : DDK가 아니라 API 쪽으로 옮겨져야 한다. 
#define	RCDDK_MAX_VIEWPORT_DIMS				2048	// 최대 2048x2048 (Multiples of 16)
#define	RCDDK_MAX_DEPTH_BOUNCE				14		// 최대값 14
#define RCDDK_MAX_THRESHOLD_LEVEL_COUNT		10

#endif//__RCDDK_DEFINES_H__
