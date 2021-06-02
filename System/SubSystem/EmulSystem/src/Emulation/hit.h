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
#ifndef __HIT_H__
#define __HIT_H__
#include "Emulation_Common.h"

typedef struct{
	int			prim_num;
	int			hit_result; //bool
	RayFloat	t;
	RayFloat	beta;
	RayFloat	gamma;
	int			HitStatus;
	int			significantFigure;//dk significant figure of hit.t
}_hit;
   
#endif//__HIT_H__