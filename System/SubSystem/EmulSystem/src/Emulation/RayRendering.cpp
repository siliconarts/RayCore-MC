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
#include "RayRendering.h"

RayRendering::RayRendering(void){
}

void RayRendering::Initialize(int x, int y){
	/*
	// 이전 선언들
	m_iX				= x;
	m_iY				= y;

	RayKind				= RAY_TYPE_PRIMARY;
	light_num			= 0;

	depth				= 0;
	shadow_mode			= 0;
	valid_traversal		= 0;

	ListCount			= 0;
	NodeCount			= 0;
	TriCount			= 0;

	StackCnt			= 0;
//	ray.TotLength		= 0;
	*/

	m_iX			= x;
	m_iY			= y;
	Valid			= 1;
	RayKind			= RAY_TYPE_PRIMARY;
	mode			= 0;
	light_num		= 0;
	depth			= 0;
	shadow_mode		= 0;
	valid_traversal	= 0;
	valid_ray_generation	= 1;
	ListCount		= 0;
	NodeCount		= 0;
	TriCount		= 0;
	StackCnt		= 0;
	ray.TotLength	= 0;

	RayGenInData.ScreenX					= x;
	RayGenInData.ScreenY					= y;
	RayGenInData.valid_ray_generation		= &valid_ray_generation;
	RayGenInData.RayKind					= &RayKind;
	RayGenInData.light_num					= &light_num;
#ifdef USE_REDUCED_ERROR_CORRECTION
	RayGenInData.positionSignificantFigure	= positionSignificantFigure;//dk
#endif
	RayGenInData.Position			= Position;
	RayGenInData.Normal				= Normal;
	RayGenInData.Direction			= Direction;
	RayGenInData.Weight				= Weight;
	RayGenInData.depth				= &depth;
	RayGenInData.reflection_color	= reflection_color;
	RayGenInData.refraction_color	= refraction_color;
	RayGenInData.refraction_index	= &refraction_index;
	RayGenInData.StackData			= StackData;
	RayGenInData.hit				= &RayHit;//dk SIT

	RayGenOutData.valid_traversal	= &valid_traversal;
	RayGenOutData.ray				= &ray;
	RayGenOutData.StackCnt			= &StackCnt;
	RayGenOutData.SpotAtten			= &SpotAtten;
	RayGenOutData.SIT_primNum		= &SIT_primNum;//dk SIT

	TnIInData.valid_traversal		= &valid_traversal;
	TnIInData.RayKind				= &RayKind;
	TnIInData.ray					= &ray;
	TnIInData.hit					= &RayHit;
	TnIInData.SIT_primNum			= &SIT_primNum;//dk SIT

	TnIOutData.hit					= &hit;
	TnIOutData.ShadowRayMinDist		= &ShadowRayMinDist;
}

RayRendering::~RayRendering(void)
{
}
