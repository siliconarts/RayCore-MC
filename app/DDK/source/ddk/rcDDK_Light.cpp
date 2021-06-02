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
// DATE    : 5/24/2013 Fri
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "rcDDK_Context.h"

void rcDDKContext::SetLightParams(DWORD index, RCDDK_LIGHT id, float* pParams){
	if(index>=RCDDK_MAX_LIGHT_COUNT) return;
	REG_LIGHT*		pReg	= m_Reg.Light[index].GetReg();
	switch(id){
	case RCDDK_LIGHT_POSITION:
		pReg->position.x		= f32f24(pParams[0]);
		pReg->position.y		= f32f24(pParams[1]);
		pReg->position.z		= f32f24(pParams[2]);
		break;
	case RCDDK_LIGHT_DIRECTION:
		pReg->direction.x		= f32f24(pParams[0]);
		pReg->direction.y		= f32f24(pParams[1]);
		pReg->direction.z		= f32f24(pParams[2]);
		break;
	case RCDDK_LIGHT_SPOT_COS:
		pReg->spot.cos_in		= f32f24(pParams[0]);
		pReg->spot.cos_out		= f32f24(pParams[1]);
		pReg->spot.falloff		= f32f24(pParams[2]);
		break;
	case RCDDK_LIGHT_AMBIENT:
		pReg->ambient.r			= f32f24(pParams[0]);
		pReg->ambient.g			= f32f24(pParams[1]);
		pReg->ambient.b			= f32f24(pParams[2]);
		break;
	case RCDDK_LIGHT_DIFFUSE:
		pReg->diffuse.r			= f32f24(pParams[0]);
		pReg->diffuse.g			= f32f24(pParams[1]);
		pReg->diffuse.b			= f32f24(pParams[2]);
		break;
	case RCDDK_LIGHT_SPECULAR:
		pReg->specular.r		= f32f24(pParams[0]);
		pReg->specular.g		= f32f24(pParams[1]);
		pReg->specular.b		= f32f24(pParams[2]);
		break;
	default:
		assert(0);
		break;
	}
	SET_DIRTY(m_Reg.Light[index]);

#ifdef USE_TESTDRIVE
	if(m_pTDRegmap){
		memcpy(&m_pTDRegmap->light, pReg, sizeof(REG_LIGHT));
		m_pTDRegmap->update.light	= TRUE;
	}
#endif
}

void rcDDKContext::SetLightAttenuation(DWORD index, RCDDK_LIGHT_TYPE type, float start, float end, float* pWeight){
	if(index>=RCDDK_MAX_LIGHT_COUNT) return;

	REG_LIGHT*		pReg	= m_Reg.Light[index].GetReg();
	pReg->atten.type	= type;
	pReg->atten.start	= f32f24(start);
	pReg->atten.end	= f32f24(end);

	if(pWeight){
		pReg->atten.enable	= 1;
		pReg->atten.weight[0]	= f32f24(pWeight[0]);
		pReg->atten.weight[1]	= f32f24(pWeight[1]);
		pReg->atten.weight[2]	= f32f24(pWeight[2]);
	}else{
		pReg->atten.enable	= 0;
	}
	SET_DIRTY(m_Reg.Light[index]);

#ifdef USE_TESTDRIVE
	if(m_pTDRegmap){
		memcpy(&m_pTDRegmap->light, pReg, sizeof(REG_LIGHT));
		m_pTDRegmap->update.light	= TRUE;
	}
#endif
}
