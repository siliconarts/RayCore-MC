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


void rcDDKContext::SetTextureMapInfo(DWORD index, RCDDK_TEXTUREMAPINFO id, const DWORD pParams) {
	if(index>=RCDDK_MAX_MATERIAL_COUNT) return;

	DWORD dwPhyAddress;

	REG_MATERIAL*		pReg	= m_Reg.Material[index].GetReg();
	REG_TEXMAPINFO*		pTex	= m_Reg.TextureMapInfo[pReg->TextureInfo.TextureMapInfo].GetReg();

	switch(id){
	case RCDDK_REPEAT_U	: 
		pTex->repeat_u = pParams;
		break;
	case RCDDK_REPEAT_V	: 
		pTex->repeat_u = pParams;
		break;
	case RCDDK_MIRROR_U	: 
		pTex->mirror_u = pParams;
		break;
	case RCDDK_MIRROR_V	: 
		pTex->mirror_v = pParams;
		break;
	case RCDDK_BASEADDRESS : 
		dwPhyAddress = pParams;
		pTex->baseaddress = ((dwPhyAddress - m_Reg.Config.GetReg()->base.texture) >> 12);
		break;
	case RCDDK_CONSTANTVALUE : 
		pTex->ConstantValue = pParams;
		break;
	case RCDDK_WIDTH : 
		pTex->Width = pParams;
		break;
	case RCDDK_HEIGHT : 
		pTex->Height = pParams;
		break;
	case RCDDK_RGB_BLENDMODE_ARG0 : 
		pTex->RGB_BlendMode_arg0 = pParams;
		break;
	case RCDDK_RGB_BLENDMODE_ARG1 : 
		pTex->RGB_BlendMode_arg1 = pParams;
		break;
	case RCDDK_RGB_BLENDMODE_ARG2 : 
		pTex->RGB_BlendMode_arg2 = pParams;
		break;
	case RCDDK_RGB_BLENDMODE_FUNC : 
		pTex->RGB_BlendMode_func = pParams;
		break;
	case RCDDK_A_BLENDMODE_ARG0 : 
		pTex->A_BlendMode_arg0 = pParams;
		break;
	case RCDDK_A_BLENDMODE_ARG1 : 
		pTex->A_BlendMode_arg1 = pParams;
		break;
	case RCDDK_A_BLENDMODE_ARG2 : 
		pTex->A_BlendMode_arg2 = pParams;
		break;
	case RCDDK_A_BLENDMODE_FUNC : 
		pTex->A_BlendMode_func = pParams;
		break;
	case RCDDK_COMPRESSION : 
		pTex->CompressionMode = pParams;
		break;
	case RCDDK_MAXLOD : 
		pTex->maxLod = pParams;
		break;
	case RCDDK_REMAP : 
		pTex->RemapMode = pParams;
		break;
	default:
		assert(0);
		break;
	}
	SET_DIRTY(m_Reg.TextureMapInfo[pReg->TextureInfo.TextureMapInfo]);
}


/*
void rcDDKContext::SetMaterialTexture(DWORD index, RCDDK_TEXTURE_TYPE type, BOOL bEnable, DWORD dwPhyAddress, DWORD log_width, DWORD log_height, BOOL bMipmap){
	if(index>=RCDDK_MAX_MATERIAL_COUNT) return;

	REG_MATERIAL*		pReg	= m_Reg.Material[index].GetReg();
//	MATERIAL_TEXTURE*	pTex	= (type==RCDDK_TEXTURE_TYPE_DIFFUSE) ? pReg->TextureInfo.TextureEnable : pReg->TextureInfo.NormalEnable;

	
	pTex->enable	= bEnable;
	if(bEnable){
		pTex->address		= ((dwPhyAddress - m_Reg.Config.GetReg()->base.texture) >> 12);
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
	MATERIAL_TEXTURE*	pTex	= (type==RCDDK_TEXTURE_TYPE_DIFFUSE) ? pReg->TextureInfo.TextureEnable : pReg->TextureInfo.NormalEnable;

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
*/