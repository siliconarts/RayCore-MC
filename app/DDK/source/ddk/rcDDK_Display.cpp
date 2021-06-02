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
// DATE    : 6/25/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "rcDDK_Context.h"
#ifdef USE_TESTDRIVE
#include "VirtualDisplayConfig.h"
#endif

BOOL rcDDKContext::DisplaySetup(int width, int height, COLOR_FORMAT format, int byte_stride){
	if(!width || !height || width%8 || height%8) return false;
#ifdef USE_TESTDRIVE
	{
		DISPLAY_FORMAT display_format;
		switch(format){
		case COLOR_FORMAT_R5G6B5:	display_format	= DISPLAY_FORMAT_RGB_565;		break;
		case COLOR_FORMAT_A8B8G8R8:	display_format	= DISPLAY_FORMAT_ABGR_8888;		break;
		default:					return false;
		}
		m_pSystem->Write(REG_DISPLAY_FORMAT,	display_format);	// 색상 포멧
	}

	m_pSystem->Write(REG_DISPLAY_WIDTH,			width);			// 너비
	m_pSystem->Write(REG_DISPLAY_HEIGHT,		height);		// 높이
	m_pSystem->Write(REG_DISPLAY_BYTE_STRIDE,	byte_stride);	// 너비 메모리 폭
#endif
	{	// register setup
		REG_CONTROL* pReg	= m_Reg.Control.GetReg();
		pReg->fb_stride			= width;
		pReg->resolution.width	= width;
		pReg->resolution.height	= height;

#ifdef USE_TESTDRIVE
		if(m_pTDRegmap){
			memcpy(&m_pTDRegmap->config, pReg, sizeof(REG_CONTROL));
			m_pTDRegmap->update.config	= TRUE;
		}
#endif
	}

	if(	m_LoadBlance.Screen.width != width ||
		m_LoadBlance.Screen.height != height)
	{
		m_LoadBlance.Screen.width	= width;
		m_LoadBlance.Screen.height	= height;
		// 초기 랜더링 배분
		DWORD dwRestBlocks			= height >> 4;	// /16
		DWORD dwBlocks				= dwRestBlocks / m_LoadBlance.dwCardCount;
		for(DWORD i=0;i<m_LoadBlance.dwCardCount;i++){
			m_LoadBlance.Performance[i].dwBlocks	= dwBlocks;
			dwRestBlocks	-= dwBlocks;
		}
		m_LoadBlance.Performance[m_LoadBlance.dwCardCount-1].dwBlocks	+= dwRestBlocks;
	}

	return TRUE;
}
