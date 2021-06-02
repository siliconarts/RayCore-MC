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
#ifndef __MATERIAL_H__
#define __MATERIAL_H__
#include "Emulation_Common.h"

typedef struct {
	RayFloat self_illumination;
}_material;

typedef struct{
	int texture_num;
	int normalmap_num;
}_texture_id;

#endif//__MATERIAL_H__
