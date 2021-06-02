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
// DATE    : 7/16/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#ifndef __TD_REGMAP_H__
#define __TD_REGMAP_H__

#include "STDInterface.h"
#include "driver/REGMAP.h"
#include "siliconarts/ddk/rcDDK_Defines.h"

typedef struct{
	struct{
		volatile BOOL		config, control, option, AST, light, material_all, material[RCDDK_MAX_MATERIAL_COUNT], report;
	}update;

	REG_CONFIG		config;
	REG_CONTROL		control;
	REG_OPTION		option;
	REG_AST			AST;
	REG_LIGHT		light[RCDDK_MAX_LIGHT_COUNT];
	REG_MATERIAL	material[RCDDK_MAX_MATERIAL_COUNT];

	struct{
		DWORD		device_count;
		struct{
			DWORD		id;
			REG_REPORT	report;
			REG_INFO	info;
		}device[32];
	}report;
}TD_REGMAP;

#endif//__TD_REGMAP_H__
