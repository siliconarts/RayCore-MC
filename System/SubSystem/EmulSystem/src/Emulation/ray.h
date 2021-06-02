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
// DATE    : 8/27/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#pragma once
#include "Emulation_Common.h"

typedef struct{
	RayFloat o[3];
	RayFloat d[3];
	RayFloat inv_d[3];
	RayFloat length;
	RayFloat TotLength;
	RayFloat weight[3];//colorº° weight
	RayFloat dielectric[3];
	int depth;
}_ray;

typedef struct{
	RayFloat o[3];
	RayFloat d[3];
	RayFloat weight[3];//colorº° weight
	RayFloat TotLength;
	int depth;
	int SIT_primNum; //dk SIT(self intersection test) primitive number
}_ray_in_buffer;

typedef struct{
	int SIT_primNum[5];
	int count;
}_SIT_primNumBuf;
