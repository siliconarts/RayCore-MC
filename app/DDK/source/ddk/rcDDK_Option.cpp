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
// DATE    : 1/21/2014 Tue
// Author  : Sang Gon, Kim (sgkim@siliconarts.co.kr)
//
//=======================================================================

#include "rcDDK_Context.h"

void rcDDKContext::SetRGBAMode(RCDDK_RGBA_MODE mode){
	REG_OPTION* pReg = m_Reg.Option.GetReg();
	pReg->set.rgba_mode		= mode;
	SET_DIRTY(m_Reg.Option);

#ifdef USE_TESTDRIVE
	if(m_pTDRegmap){
		memcpy(&m_pTDRegmap->option, pReg, sizeof(REG_OPTION));
		m_pTDRegmap->update.option	= TRUE;
	}
#endif
}

void rcDDKContext::SetLightCount(DWORD count){
	REG_OPTION* pReg = m_Reg.Option.GetReg();
	pReg->set.light_number		= count;
	SET_DIRTY(m_Reg.Option);

#ifdef USE_TESTDRIVE
	if(m_pTDRegmap){
		memcpy(&m_pTDRegmap->option, pReg, sizeof(REG_OPTION));
		m_pTDRegmap->update.option	= TRUE;
	}
#endif
}

void rcDDKContext::SetBoundDepth(DWORD depth){
	REG_OPTION* pReg = m_Reg.Option.GetReg();
	pReg->set.ray_bounce		= depth;
	SET_DIRTY(m_Reg.Option);

#ifdef USE_TESTDRIVE
	if(m_pTDRegmap){
		memcpy(&m_pTDRegmap->option, pReg, sizeof(REG_OPTION));
		m_pTDRegmap->update.option	= TRUE;
	}
#endif
}

void rcDDKContext::SetAdaptiveRender(BOOL bEnable, DWORD size){
	REG_OPTION* pReg = m_Reg.Option.GetReg();
	pReg->set.adaptive_render	= bEnable;
	pReg->set.adaptive_size		= (size>63) ? 63 : size;
	SET_DIRTY(m_Reg.Option);

#ifdef USE_TESTDRIVE
	if(m_pTDRegmap){
		memcpy(&m_pTDRegmap->option, pReg, sizeof(REG_OPTION));
		m_pTDRegmap->update.option	= TRUE;
	}
#endif
}

void rcDDKContext::SetPixelSize(float width, float height){
	REG_CONTROL* pReg = m_Reg.Control.GetReg();
	pReg->pixel_size.x				= f32f24(width);
	pReg->pixel_size.y				= f32f24(height); 
	m_LoadBlance.PixelSize.width	= width;
	m_LoadBlance.PixelSize.height	= height;
	SET_DIRTY(m_Reg.Control);

#ifdef USE_TESTDRIVE
	if(m_pTDRegmap){
		memcpy(&m_pTDRegmap->control, pReg, sizeof(REG_CONTROL));
		m_pTDRegmap->update.control	= TRUE;
	}
#endif
}

void rcDDKContext::SetStartPosition(float x, float y){
	REG_CONTROL* pReg = m_Reg.Control.GetReg();
	pReg->start.x			= f32f24(x);
	pReg->start.y			= f32f24(y); 
	m_LoadBlance.StartPosition.x	= x;
	m_LoadBlance.StartPosition.y	= y;
	SET_DIRTY(m_Reg.Control);

#ifdef USE_TESTDRIVE
	if(m_pTDRegmap){
		memcpy(&m_pTDRegmap->control, pReg, sizeof(REG_CONTROL));
		m_pTDRegmap->update.control	= TRUE;
	}
#endif
}

void rcDDKContext::SetBoundBox(DWORD RootNodeID, float* pMinMax){
	REG_AST* pReg = m_Reg.AST.GetReg();

	pReg->bound_box.index	= RootNodeID;

	// maximum - camera eye position
	pReg->bound_box.min.x	= f32f24(pMinMax[0]);
	pReg->bound_box.min.y	= f32f24(pMinMax[1]);
	pReg->bound_box.min.z	= f32f24(pMinMax[2]);

	// minimum - camera eye position
	pReg->bound_box.max.x	= f32f24(pMinMax[3]);
	pReg->bound_box.max.y	= f32f24(pMinMax[4]);
	pReg->bound_box.max.z	= f32f24(pMinMax[5]);

	SET_DIRTY(m_Reg.AST);

#ifdef USE_TESTDRIVE
	if(m_pTDRegmap){
		memcpy(&m_pTDRegmap->AST, pReg, sizeof(REG_AST));
		m_pTDRegmap->update.AST	= TRUE;
	}
#endif
}

void rcDDKContext::SetRayThresholdWeight(float threshold){
	REG_OPTION* pReg = m_Reg.Option.GetReg();
	pReg->threshold_weight	= f32f24(threshold);
	SET_DIRTY(m_Reg.Option);

#ifdef USE_TESTDRIVE
	if(m_pTDRegmap){
		memcpy(&m_pTDRegmap->option, pReg, sizeof(REG_OPTION));
		m_pTDRegmap->update.option	= TRUE;
	}
#endif
}

void rcDDKContext::SetBackgroundColor(float red, float green, float blue){
	REG_CONTROL* pReg = m_Reg.Control.GetReg();
	pReg->background.r	= f32f24(red);
	pReg->background.g	= f32f24(green); 
	pReg->background.b	= f32f24(blue);
	SET_DIRTY(m_Reg.Control);

#ifdef USE_TESTDRIVE
	if(m_pTDRegmap){
		memcpy(&m_pTDRegmap->control, pReg, sizeof(REG_CONTROL));
		m_pTDRegmap->update.control	= TRUE;
	}
#endif
}

void rcDDKContext::SetCoreMask(int count)
{
	if (count <= 0) return;
	REG_CONTROL* pReg = m_Reg.Control.GetReg();
	pReg->core_mask = (0b1111111 << count) & 0xff;
	SET_DIRTY(m_Reg.Control);
}

// From the right-to-left order: 6 bits(#spp) - 4 bits(diffuse rate) - 3 unused bits - 13 bits(light radius)
void rcDDKContext::SetReservedValue(unsigned int value, unsigned int type)
{
	REG_CONTROL* pReg = m_Reg.Control.GetReg();

	switch (type) {
		case DiffuseRate:
			pReg->reserved = (pReg->reserved & 0xFFF0FFFF) | ((value & 0xF) << 16);
			break;

		case SppCount:
			pReg->reserved = (pReg->reserved & 0xFC0FFFFF) | ((value & 0x3F) << 20);
			break;

		default: // LightRadius
			pReg->reserved = (pReg->reserved & 0xFFFF0000) | (value & 0x1FFF); // 13 bits
			break;
	}
	SET_DIRTY(m_Reg.Control);
}