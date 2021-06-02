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
#include "Registers.h"
#include <math.h>

Registers	g_reg;

Registers::Registers(void){
	memset(&config, 0, sizeof(REG_CONFIG));
	memset(&control, 0, sizeof(REG_CONTROL));
	memset(&option, 0, sizeof(REG_OPTION));
	memset(&ast, 0, sizeof(REG_AST));
	memset(light, 0, sizeof(REG_LIGHT)*8);
	memset(material, 0, sizeof(REG_MATERIAL)*512);
}

Registers::~Registers(void){
}

Memories	g_mem;

Memories::Memories(void){
	pFramebuffer	= NULL;
	pTransmittance	= NULL;
	pList			= NULL;
	pNode			= NULL;
	pPrimitive		= NULL;
	pPrimitiveInfo	= NULL;
	pTexture		= NULL;

	for(int i=0;i<12;i++){
		LodTable[i]=(int)((pow(4.,(int)i)-1)/3);
	}
}

Memories::~Memories(void){

}

RCVECTOR3u Memories::PrimitiveCenterPos(int index){
	DDK_PRIMITIVE* pPrim	= &pPrimitive[index];
	RCVECTOR3u pos;
	pos.x	= ((((DWORD)pPrim->triangle.bnu_h)<<16) | pPrim->triangle.bnu_l);
	pos.y	= ((DWORD)pPrim->triangle.bnv);
	pos.z	= ((DWORD)pPrim->triangle.bd);
	return pos;
}

RCVECTOR3u Memories::PrimitiveCNuvd(int index){
	DDK_PRIMITIVE* pPrim	= &pPrimitive[index];
	RCVECTOR3u pos;
	pos.x	= ((DWORD)pPrim->triangle.cnu);
	pos.y	= ((((DWORD)pPrim->triangle.cnv_h)<<8) | pPrim->triangle.cnv_l);
	pos.z	= ((((DWORD)pPrim->triangle.cd_h)<<8) | pPrim->triangle.cd_l);
	return pos;
}

RCVECTOR3u Memories::PrimitiveInfoNormal(int index, int element){
	DDK_PRIMITIVE_INFO* pPrimInfo = &pPrimitiveInfo[index];
	RCVECTOR3u pos;

	switch(element){
	case 0:
		pos.x	= pPrimInfo->v0_norm_x;
		pos.y	= ((DWORD)pPrimInfo->v0_norm_y_h<<8) | pPrimInfo->v0_norm_y_l;
		pos.z	= ((DWORD)pPrimInfo->v0_norm_z_h<<16) | pPrimInfo->v0_norm_z_l;
		break;
	case 1:
		pos.x	= pPrimInfo->v1_norm_x;
		pos.y	= pPrimInfo->v1_norm_y;
		pos.z	= ((DWORD)pPrimInfo->v1_norm_z_h<<8) | pPrimInfo->v1_norm_z_l;
		break;
	case 2:
		pos.x	= ((DWORD)pPrimInfo->v2_norm_x_h<<8) | pPrimInfo->v2_norm_x_l;
		pos.y	= pPrimInfo->v2_norm_y;
		pos.z	= pPrimInfo->v2_norm_z;
		break;
	}
	return pos;
}

RCVECTOR2u Memories::PrimitiveInfoTexCoord(int index, int element){
	DDK_PRIMITIVE_INFO* pPrimInfo = &pPrimitiveInfo[index];
	RCVECTOR2u pos;

	switch(element){
	case 0:
		pos.u	= ((DWORD)pPrimInfo->v0_tex_u_h<<8) | pPrimInfo->v0_tex_u_l;
		pos.v	= ((DWORD)pPrimInfo->v0_tex_v_h<<16) | pPrimInfo->v0_tex_v_l;
		break;
	case 1:
		pos.u	= pPrimInfo->v1_tex_u;
		pos.v	= pPrimInfo->v1_tex_v;
		break;
	case 2:
		pos.u	= ((DWORD)pPrimInfo->v2_tex_u_h<<8) | pPrimInfo->v2_tex_u_l;
		pos.v	= ((DWORD)pPrimInfo->v2_tex_v_h<<16) | pPrimInfo->v2_tex_v_l;
		break;
	}
	return pos;
}