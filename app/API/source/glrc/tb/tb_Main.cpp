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
// Project : API
// DATE    : 6/3/2013 Mon
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "tb_TreeBuilder.h"

// @todo : SW Tree Building에 대한 Library화가 필요함! 
//		(API 공개시 소스 공개없이 lib로만 제공되어야 함!)

void TreeBuilder::AddTriangle( RC_VERTEX *vertex, DWORD index, BOUNDING_BOX *Box ){
	AS_SOURCE* pSrc	= m_Src.Get(index);
	pSrc->dwIndex	= index;

	for(int i=0;i<3;i++){
		if((vertex[0].position.m[i] < vertex[1].position.m[i])&&(vertex[0].position.m[i]<vertex[2].position.m[i]))
			pSrc->split[0].fdata[i] = vertex[0].position.m[i];
		else if(vertex[1].position.m[i] < vertex[2].position.m[i])
			pSrc->split[0].fdata[i] = vertex[1].position.m[i];
		else 
			pSrc->split[0].fdata[i] = vertex[2].position.m[i];

		if((vertex[0].position.m[i] > vertex[1].position.m[i])&&(vertex[0].position.m[i]>vertex[2].position.m[i]))
			pSrc->split[1].fdata[i] = vertex[0].position.m[i];
		else if(vertex[1].position.m[i] > vertex[2].position.m[i])
			pSrc->split[1].fdata[i] = vertex[1].position.m[i];
		else 
			pSrc->split[1].fdata[i] = vertex[2].position.m[i];

		if(Box->minimum.m[i] > pSrc->split[0].fdata[i])
			Box->minimum.m[i] = pSrc->split[0].fdata[i];

		if(Box->maximum.m[i] < pSrc->split[1].fdata[i])
			Box->maximum.m[i] = pSrc->split[1].fdata[i];

		pSrc->split[0].udata[i] &= 0xFFFFFF80;//dk 7bit
		pSrc->split[1].udata[i] &= 0xFFFFFF80;//dk 7bit

		if(pSrc->split[0].fdata[i] == pSrc->split[1].fdata[i]) {
			pSrc->split[0].flag[i]	=
			pSrc->split[1].flag[i]	= 1;
		} else {
			pSrc->split[0].flag[i]	= 2;
			pSrc->split[1].flag[i]	= 0;
		}
	}
}

void TreeBuilder::BuildAccelerationStructure(BOUNDING_BOX *boundBox, DWORD totalPrimitiveNumber){
	{
		double length;

		for(int i=0; i<3; i++){
			length = boundBox->maximum.m[i] - boundBox->minimum.m[i];
			if(length != 0){
				boundBox->minimum.m[i] -= (float)(length*0.0001);
				boundBox->maximum.m[i] += (float)(length*0.0001);
			}else{
				boundBox->minimum.m[i] -= 0.0001f;
				boundBox->maximum.m[i] += 0.0001f;
			}
		}
	}

	{
		//DWORD *PrimNum = new DWORD[0x1000000];	// Index memory + temp memory
		// @todo : 사이즈 계산이 정확하지 않다.
		DWORD *PrimNum = new DWORD[(totalPrimitiveNumber + 1024)*sizeof(RC_PRIMITIVE)];	// Index memory + temp memory

		for(DWORD i=0;i<totalPrimitiveNumber;i++) PrimNum[i] = GetCurrentTriStartIndex() + i;
		for(DWORD i=0;i<1024;i++)	m_pDefPrimNum[i] = i;

		ScanTreeBuild(PrimNum, *boundBox, totalPrimitiveNumber, 0);

		delete [] PrimNum;
	}
}
