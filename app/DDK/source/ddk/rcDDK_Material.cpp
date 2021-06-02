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
// DATE    : 7/18/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "rcDDK_Context.h"

void rcDDKContext::SetMaterialParams(DWORD index, RCDDK_MATERIAL id, const float* pParams){
	if(index>=RCDDK_MAX_MATERIAL_COUNT) return;

	REG_MATERIAL*	pReg	= m_Reg.Material[index].GetReg();

	switch(id){
	case RCDDK_MATERIAL_AMBIENT:
		pReg->ambient.r			= f32f24(pParams[0]);
		pReg->ambient.g			= f32f24(pParams[1]);
		pReg->ambient.b			= f32f24(pParams[2]);
		break;

	case RCDDK_MATERIAL_DIFFUSE:
		pReg->diffuse.r			= f32f24(pParams[0]);
		pReg->diffuse.g			= f32f24(pParams[1]);
		pReg->diffuse.b			= f32f24(pParams[2]);
		break;
	
	case RCDDK_MATERIAL_SPECULAR:
		pReg->specular.r		= f32f24(pParams[0]);
		pReg->specular.g		= f32f24(pParams[1]);
		pReg->specular.b		= f32f24(pParams[2]);
		break;
	
	case RCDDK_MATERIAL_SPECULAR_LEVEL:
		pReg->specular.level	= f32f24(pParams[0]);
		break;
	
	case RCDDK_MATERIAL_EXPONENT:
		pReg->specular.exponent	= f32f24(pParams[0]);
		break;
	
	case RCDDK_MATERIAL_REFRACTION:
		pReg->refraction_index	= f32f24(pParams[0]);
		break;
	
	case RCDDK_MATERIAL_REFLECTION:
		pReg->reflectance		= f32f24(pParams[0]);
		break;
	
	case RCDDK_MATERIAL_TRANSMITTANCE:
		pReg->transmittance		= f32f24(pParams[0]);
		break;
	
	case RCDDK_MATERIAL_ROUGHNESS:
		pReg->ambient.r = pReg->ambient.r | ((f32f24(pParams[0]) & 0xFF) << 24);
		pReg->ambient.g = pReg->ambient.g | (((f32f24(pParams[0]) >> 8) & 0xFF) << 24);
		pReg->ambient.b = pReg->ambient.b | (((f32f24(pParams[0]) >> 16) & 0xFF) << 24);
	break;

	default:
		assert(0);
		break;
	}
	SET_DIRTY(m_Reg.Material[index]);

#ifdef USE_TESTDRIVE
	if(m_pTDRegmap){
		memcpy(&m_pTDRegmap->material[index], pReg, sizeof(REG_MATERIAL));
		m_pTDRegmap->update.material[index]		= TRUE;
		m_pTDRegmap->update.material_all		= TRUE;
	}
#endif
}
//}hjkwon 20190430
//}32->64 bit physical Address;
void rcDDKContext::SetMaterialTexture(DWORD index, RCDDK_TEXTURE_TYPE type, BOOL bEnable, unsigned long long dwPhyAddress, DWORD log_width, DWORD log_height, BOOL bMipmap){
	if(index>=RCDDK_MAX_MATERIAL_COUNT) return;

	REG_MATERIAL*		pReg	= m_Reg.Material[index].GetReg();
	MATERIAL_TEXTURE*	pTex	= (type==RCDDK_TEXTURE_TYPE_DIFFUSE) ? &pReg->texture : &pReg->normal;

	
	pTex->enable	= bEnable;
	//}hjkwon debug point
	//dwPhyAddress: 64bit address
	//(unsigned long long)(m_Reg.Config.GetReg()->base.texture)<<5) //32byte addr -> 1byte addr	
	//}
	if(bEnable){
		pTex->address		= ((dwPhyAddress - ((unsigned long long)(m_Reg.Config.GetReg()->base.texture)<<5)) >> 12);
		pTex->log_width		= log_width;
		pTex->log_height	= log_height;
		pTex->mipmap		= bMipmap;
	}
	SET_DIRTY(m_Reg.Material[index]);

#ifdef USE_TESTDRIVE
	if(m_pTDRegmap){
		memcpy(&m_pTDRegmap->material[index], pReg, sizeof(REG_MATERIAL));
		m_pTDRegmap->update.material[index]		= TRUE;
		m_pTDRegmap->update.material_all		= TRUE;
	}
#endif
}

void rcDDKContext::SetMaterialTextureWrap(DWORD index, RCDDK_TEXTURE_TYPE type, RCDDK_TEXTURE_WRAP mode, BOOL U, BOOL V){
	if(index>=RCDDK_MAX_MATERIAL_COUNT) return;

	REG_MATERIAL*		pReg	= m_Reg.Material[index].GetReg();
	MATERIAL_TEXTURE*	pTex	= (type==RCDDK_TEXTURE_TYPE_DIFFUSE) ? &pReg->texture : &pReg->normal;

	switch(mode){
	case RCDDK_TEXTURE_WRAP_REPEAT:
		pTex->repeat_u		= U;
		pTex->repeat_v		= V;
		break;
	case RCDDK_TEXTURE_WRAP_MIRROR:
		pTex->mirror_u		= U;
		pTex->mirror_v		= V;
		break;
	default:return;
	}

#ifdef USE_TESTDRIVE
	if(m_pTDRegmap){
		memcpy(&m_pTDRegmap->material[index], pReg, sizeof(REG_MATERIAL));
		m_pTDRegmap->update.material[index]		= TRUE;
		m_pTDRegmap->update.material_all		= TRUE;
	}
#endif
}
