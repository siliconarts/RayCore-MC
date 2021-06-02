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
#ifndef KDTREE_H
#define KDTREE_H
#include "Emulation_Common.h"

typedef unsigned int u_int;

typedef struct{
	union {
		RayFloat position[3];
		RayFloat Center[3];
	};
	float fposition[3];
	RayFloat normal[3];
	union{
		RayFloat tex_coord[2];
		RayFloat Radius[2];
	};
}_vertex;

typedef struct{
	_vertex vertex[3];
	RayFloat BaseLod;
	int material_index;
	int fGroup;
}KdTriangle;

struct _BoxField{
	unsigned int UpperGroutHitTemp:1;
	unsigned int UpperGroutHit:1;
	unsigned int StackRtrPtr:6;
	unsigned int RemindCount:24;
};

struct KdStack {
	int flag;

	union{
		int nodeAddress;
		int NextListAddress;
	};
	struct _BoxField BoxField;
	RayFloat	tmin, tmax;
};

#if(MODE==2)
struct _traversal_buffer{
	unsigned int buffer[16];
};
#endif

#endif //KDTREE_H