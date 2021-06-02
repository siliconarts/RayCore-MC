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
// Project : DDK
// DATE    : 6/24/2013 Mon
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include <string.h>

#include "rcDDK_Context.h"

void rcDDKContext::SetCameraParams(RCDDK_CAMERA id, float* pParams){
	REG_OPTION* pReg	= m_Reg.Option.GetReg();
	switch(id){
	case RCDDK_CAMERA_EYE:
		pReg->camera.eye.x			= f32f24(pParams[0]);
		pReg->camera.eye.y			= f32f24(pParams[1]);
		pReg->camera.eye.z			= f32f24(pParams[2]);
		break;
	case RCDDK_CAMERA_BASIS0:
		pReg->camera.basis[0].x		= f32f24(pParams[0]);
		pReg->camera.basis[0].y		= f32f24(pParams[1]);
		pReg->camera.basis[0].z		= f32f24(pParams[2]);
		break;
	case RCDDK_CAMERA_BASIS1:
#ifdef WIN32
		pReg->camera.basis[1].x		= f32f24(pParams[0]);
		pReg->camera.basis[1].y		= f32f24(pParams[1]);
		pReg->camera.basis[1].z		= f32f24(pParams[2]);
#else		// 상하 반전을 위해서는 이 값이 음수로 들어가야 한다.
		pReg->camera.basis[1].x		= f32f24(-pParams[0]);
		pReg->camera.basis[1].y		= f32f24(-pParams[1]);
		pReg->camera.basis[1].z		= f32f24(-pParams[2]);
#endif
		break;
	case RCDDK_CAMERA_BASIS2:
		pReg->camera.basis[2].x		= f32f24(pParams[0]);
		pReg->camera.basis[2].y		= f32f24(pParams[1]);
		pReg->camera.basis[2].z		= f32f24(pParams[2]);
		break;
	case RCDDK_CAMERA_DISTANCE:
		pReg->camera.distance		= f32f24(pParams[0]);
		break;
	default:
		assert(0);
		break;
	}

	SET_DIRTY(m_Reg.Option);

#ifdef USE_TESTDRIVE
	if(m_pTDRegmap){
		memcpy(&m_pTDRegmap->option, pReg, sizeof(REG_OPTION));
		m_pTDRegmap->update.option	= TRUE;
	}
#endif
}

void rcDDKContext::SetCameraAttenuation(float start, float end, float* pWeight){
	REG_OPTION* pReg	= m_Reg.Option.GetReg();
	pReg->camera.atten.start		= f32f24(start);
	pReg->camera.atten.end			= f32f24(end);
	pReg->camera.atten.weight[0]	= f32f24(pWeight[0]);
	pReg->camera.atten.weight[1]	= f32f24(pWeight[1]);
	pReg->camera.atten.weight[2]	= f32f24(pWeight[2]);
	SET_DIRTY(m_Reg.Option);

#ifdef USE_TESTDRIVE
	if(m_pTDRegmap){
		memcpy(&m_pTDRegmap->option, pReg, sizeof(REG_OPTION));
		m_pTDRegmap->update.option	= TRUE;
	}
#endif
}
