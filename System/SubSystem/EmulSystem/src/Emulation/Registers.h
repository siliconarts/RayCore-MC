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
#include "driver/RCStructures.h"
#include "driver/REGMAP.h"
#include "primitive/primitive.h"
#include "as/as_node.h"
#include "as/as_list.h"

class Registers{
public:
	Registers(void);
	~Registers(void);

	REG_CONFIG		config;
	REG_CONTROL		control;
	REG_OPTION		option;
	REG_AST			ast;
	REG_LIGHT		light[8];
	REG_MATERIAL	material[512];

};

class Memories{
public:
	Memories(void);
	~Memories(void);

	RCVECTOR3u PrimitiveCenterPos(int index);
	RCVECTOR3u PrimitiveCNuvd(int index);

	RCVECTOR3u PrimitiveInfoNormal(int index, int element);
	RCVECTOR2u PrimitiveInfoTexCoord(int index, int element);

	DWORD*					pFramebuffer;
	RC_TRANSMITTANCE_DATA*	pTransmittance;
	DDK_LIST*				pList;
	DDK_NODE*				pNode;
	DDK_PRIMITIVE*			pPrimitive;
	DDK_PRIMITIVE_INFO*		pPrimitiveInfo;
	BYTE*					pTexture;

	int						LodTable[12];
};

extern Registers	g_reg;
extern Memories		g_mem;
