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
// DATE    : 8/21/2013 Wed
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "rcDDK_Context.h"

//-----------------------------------------------------------------
// dummy register write �Լ�
void IO_Write(DWORD dwOffset32, DWORD* pData, int iCount){
	ISystem* pSystem	= rcDDKContext::GET_CURRENT()->GetSystem();
	DWORD	dwCount		= pSystem->GetDeviceCount();
	for(DWORD i=0;i<dwCount;i++){
		pSystem->SetDevice(i);
		pSystem->Writes(dwOffset32, pData, iCount);
	}
}

void IO_WriteControl(DWORD dwOffset32, DWORD* pData, int iCount){
	MULTI_LOAD* pLoad	= rcDDKContext::GET_CURRENT()->GetLoadBalance();
	DWORD	dwCount		= pLoad->dwCardCount;

	if(dwCount == 1) IO_Write(dwOffset32, pData, iCount);
	else{
		ISystem*	pSystem	= rcDDKContext::GET_CURRENT()->GetSystem();
		REG_CONTROL* pReg	= (REG_CONTROL*)pData;
		DWORD	dwBlocks	= 0;

		for(DWORD i=0;i<dwCount;i++)
		if(pLoad->Performance[i].dwBlocks){
			pReg->start.y			= f32f24(pLoad->StartPosition.y + (pLoad->PixelSize.height * (dwBlocks<<4)));
			pReg->resolution.height	= (unsigned short)(pLoad->Performance[i].dwBlocks<<4);
			pSystem->SetDevice(i);
			pSystem->Writes(dwOffset32, pData, iCount);
			dwBlocks	+= pLoad->Performance[i].dwBlocks;
		}
	}
}

//-----------------------------------------------------------------
RCDDK_API RCDDK* RCDDK_ENTRY CreateRCDDK(void){
	rcDDKContext* pContext	= new rcDDKContext;
	if(!pContext->IsInitialized())
		SAFE_RELEASE(pContext);
	return pContext;
}

rcDDKContext*	rcDDKContext::m_pCurrent	= NULL;
ISystem*		rcDDKContext::m_pSystem		= NULL;
int				rcDDKContext::m_iRefCount	= 0;

rcDDKContext::rcDDKContext(void){
	if(!m_pSystem) m_pSystem	= CreateSystem();
	m_iRefCount++;

#ifdef USE_TESTDRIVE
	{
		m_pTDRegmap			= NULL;
		m_pTDMemory			= TestDriver_GetMemory("RayCore_Display");
		if(m_pTDMemory)
			m_pTDRegmap			= (TD_REGMAP*)m_pTDMemory->GetPointer();
	}
#endif

	DIRTY_CHAIN_INITIALIZE
	SET_DIRTY(m_Reg.Control);	// control �������͸� ���� �������� ����ϱ� ���ؼ� ���� dirty�� �����Ѵ�.

	for(int i=0;i<RCDDK_MAX_LIGHT_COUNT;i++) m_Reg.Light[i].AddOffset(i<<5);
	for(int i=0;i<RCDDK_MAX_MATERIAL_COUNT;i++){
		REG_MATERIAL* pReg	= m_Reg.Material[i].GetReg();
		memset(pReg, 0, sizeof(REG_MATERIAL));
		m_Reg.Material[i].AddOffset(i*REG_MATERIAL_SIZE);
		
		pReg->normal.repeat_u	=
		pReg->normal.repeat_v	=
		pReg->texture.repeat_u	=
		pReg->texture.repeat_v	= 1;

#ifdef USE_TESTDRIVE	// for fresh setup
		SetMaterialTexture(i, RCDDK_TEXTURE_TYPE_DIFFUSE, FALSE, 0, 0, 0, FALSE);
#endif
	}

	memset(&m_LoadBlance, 0, sizeof(m_LoadBlance));
	memset(&m_Mem, 0, sizeof(m_Mem));

	if(m_pSystem){
		REG_CONFIG* pReg = m_Reg.Config.GetReg();
		//}hjkwon 20190430
		//}32byte address conversion (pReg->base.texture)
		//pReg->base.texture		= m_pSystem->GetMemoryBase();	// texture �ʱ� �ּҸ� �����Ѵ�. ���� �޸� �Ҵ� ���������� �ǹ̾��� �ൿ�̴�.
		pReg->base.texture		= m_pSystem->GetMemoryBase()>>5;	// texture �ʱ� �ּҸ� �����Ѵ�. ���� �޸� �Ҵ� ���������� �ǹ̾��� �ൿ�̴�.
		SET_DIRTY(m_Reg.Config);

		// ī�� ���� �м�
		m_LoadBlance.dwCardCount	= m_pSystem->GetDeviceCount();
		if(m_LoadBlance.dwCardCount){
			for(DWORD i=0;i<m_LoadBlance.dwCardCount;i++){
				m_pSystem->SetDevice(i);
				m_LoadBlance.Performance[i].dwScale	= 4096 / m_pSystem->GetClockMHz();
			}
			m_pSystem->SetDevice(0);
		}

#ifdef USE_TESTDRIVE
		if(m_pTDRegmap){
			m_pTDRegmap->report.device_count	= m_pSystem->GetDeviceCount();
			for(DWORD i = 0;i<m_LoadBlance.dwCardCount;i++){
				m_pSystem->SetDevice(i);
				m_pTDRegmap->report.device[i].id	= m_pSystem->GetDeviceID();
				m_pSystem->Reads(RC_REG_OFFSET_INFO>>2, (DWORD*)&(m_pTDRegmap->report.device[i].info), sizeof(REG_INFO));
			}
		}
#endif
	}else{
#ifdef WIN32
		//MessageBox(NULL, "������ RayCore ����̹� �������̽��� ã�� �� �����ϴ�.\n���� �ý��� �Ǵ� ����̹��� �ùٸ��� ��ġ�Ǿ� �ִ��� Ȯ���Ͻñ� �ٶ��ϴ�.", "���", MB_ICONEXCLAMATION|MB_OK);
		MessageBox(NULL, "Can't find valid RayCore driver interface.\nPlease check your sub-system and driver installation.", "Warning", MB_ICONEXCLAMATION|MB_OK);
#endif
	}
	{
		REG_CONTROL* pReg = m_Reg.Control.GetReg();
		pReg->control.sw_reset		= 1;
		pReg->control.cache_clear	= 1;
		pReg->control.start			= 1;
		// for dynalith
		pReg->status.chip_mask		= (~1)&0xF;
	}
	{
		REG_OPTION* pReg = m_Reg.Option.GetReg();
		pReg->set.normal_map	= 1;
	}
	{
		REG_AST* pReg	= m_Reg.AST.GetReg();
		pReg->MinDist	= 0x300000;
	}
	SetAdaptiveRender(TRUE);

	m_pCurrent	= this;
}

rcDDKContext::~rcDDKContext(void){
	m_iRefCount--;
	if(!m_iRefCount){
		SAFE_RELEASE(m_pSystem);
#ifdef USE_TESTDRIVE
		SAFE_RELEASE(m_pTDMemory);
#endif
	}
	if(m_pCurrent==this) m_pCurrent = NULL;
}

void rcDDKContext::Release(void){
	delete this;
}

BOOL rcDDKContext::GetInformation(RCDDK_INFO* pInfo){
	if(!m_pSystem) return FALSE;
	m_pSystem->Reads((RC_REG_OFFSET_INFO)>>2, (DWORD*)pInfo, sizeof(RCDDK_INFO)>>2);
	return TRUE;
}

BOOL rcDDKContext::Enable(RCDDK_ENABLE id, BOOL bEnable){
	REG_OPTION* pReg = m_Reg.Option.GetReg();
	switch(id){
	case RCDDK_ENABLE_SHADOW:
		if(pReg->set.shadow == bEnable) return TRUE;
		pReg->set.shadow			= bEnable;
		break;
	case RCDDK_ENABLE_COLOR_SHADOW:
		if(pReg->set.color_shadow == bEnable) return TRUE;
		pReg->set.color_shadow		= bEnable;
		break;
	case RCDDK_ENABLE_LOD:
		if(pReg->set.lod == bEnable) return TRUE;
		pReg->set.lod				= bEnable;
		break;
	case RCDDK_ENABLE_SUPERSAMPLING:
		if(pReg->set.super_sampling == bEnable) return TRUE;
		pReg->set.super_sampling	= bEnable;
		break;
	case RCDDK_ENABLE_NORMALMAP:
		if(pReg->set.normal_map == bEnable) return TRUE;
		pReg->set.normal_map		= bEnable;
	default:
		return FALSE;
	}
	SET_DIRTY(m_Reg.Option);	
	return TRUE;

#ifdef USE_TESTDRIVE
	if(m_pTDRegmap){
		memcpy(&m_pTDRegmap->option, pReg, sizeof(REG_OPTION));
		m_pTDRegmap->update.option	= TRUE;
	}
#endif
}

void rcDDKContext::SetMemory(RCDDK_MEMORY id, RCDDKMemory* pMemory){
	REG_CONFIG* pReg = m_Reg.Config.GetReg();
	if(!pMemory) return;
	//}
	//19.4.6 - sjpark
	//hardware addressing modification 
	//old : pMemory->Physical(), 1byte address
	//current : pMemory->Physical() >> shiftBit, 32byte address
	//}
	unsigned int shiftBit = 5;
	//}sjpark
	//printf("[RAYCORE] SetMemory[%d] 0x%llx 0x%llx \n", id, pMemory->Physical(), pMemory->Physical() >> shiftBit);
	//}sjpark

	switch(id){
	case RCDDK_MEMORY_NODE:
		pReg->base.node = pMemory->Physical() >> shiftBit;
		break;
	case RCDDK_MEMORY_LIST:
		pReg->base.list = pMemory->Physical() >> shiftBit;
		break;
	case RCDDK_MEMORY_PRIMITIVE:
		pReg->base.primitive = pMemory->Physical() >> shiftBit;
		break;
	case RCDDK_MEMORY_PRIMITIVE_INFO:
		pReg->base.primitive_info = pMemory->Physical() >> shiftBit;
		break;
	case RCDDK_MEMORY_TRANSMIT:
		pReg->base.transmit = pMemory->Physical() >> shiftBit;
		break;
	case RCDDK_MEMORY_FRAMEBUFFER:
		pReg->base.framebuffer_0 =
		pReg->base.framebuffer_1 = pMemory->Physical() >> shiftBit;
		break;
	default:
		return;
	}
	m_Mem.pMemory[id]	= ((_RCDDKMemory*)pMemory)->GetNative();

	SET_DIRTY(m_Reg.Config);

#ifdef USE_TESTDRIVE
	if(m_pTDRegmap){
		memcpy(&m_pTDRegmap->config, pReg, sizeof(REG_CONFIG));
		m_pTDRegmap->update.config	= TRUE;
	}
#endif
}

unsigned long long rcDDKContext::GetMemoryBase(void){
	return m_pSystem->GetMemoryBase();
}
