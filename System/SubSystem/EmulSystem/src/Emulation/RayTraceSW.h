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
#pragma once
#include "kdtree.h"
#include "option.h"
#include "camera.h"
#include "material.h"
#include "texture.h"
#include "Emulation_Common.h"
#include "ray.h"
#include "RayRendering.h"

class RayTraceSW
{
public:
	RayTraceSW(void);
	~RayTraceSW(void);

	int load(char folderName[256]);
	void trace_blm(void);

	_option opt;
	_material material[MAX_MATERIAL];
	_texture_id  texture_id[MAX_MATERIAL];
	_texture texture[MAX_TEXTURE];
	_texture normalmap[MAX_TEXTURE];//dk normal mapping
	_texture_index texture_index;
	_texture_index normalmap_index;//dk normal mapping

	int MaterialFlag[1024];

	int *PrimNum;
};
