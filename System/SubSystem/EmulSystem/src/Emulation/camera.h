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
#ifndef CAMERA_H
#define CAMERA_H
#include "Emulation_Common.h"

typedef struct{
	float eye[3]; // Position of the eye.
	float up[3]; // Up direction.
	float basis[3][3]; // Orthonormal basis [right,up,gaze] (u,v,w)
	float distance; // Distance from the eye to the image plane.
	float lookat[3];
	float ScreenWidth;
}_fcamera;

#endif //CAMERA_H