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
// DATE    : 11/28/2013 Thu
// Author  : Nobody
//
//=======================================================================

#include "rcDDK_Context.h"
#ifdef CROSS_COMPILE
#include <malloc.h>
#endif

void rcDDKContext::SetCRRNInfo(DWORD number, DWORD baddr){
	REG_AST*	pReg		= m_Reg.AST.GetReg();
	pReg->def_list_num		= number;
	pReg->def_list_addr		= baddr;

#ifdef USE_TESTDRIVE
	if(m_pTDRegmap){
		memcpy(&m_pTDRegmap->AST, pReg, sizeof(REG_AST));
		m_pTDRegmap->update.AST	= TRUE;
	}
#endif
}

BOOL rcDDKContext::RenderStart(void){
	DIRTY_CHAIN_FLUSH	// do render
	SET_DIRTY(m_Reg.Control);	// set 'control' register to last chain

#ifdef USE_TESTDRIVE
		if(m_pTDRegmap){
			m_pTDRegmap->control.status.core_thread_end	= 0;
			m_pTDRegmap->update.control	= TRUE;
		}
#endif
	return TRUE;
}

BOOL rcDDKContext::RenderEnd(RCDDKMemory* pSurface){
	if(pSurface)
	{
		DWORD dwCurBlock	= 0;
		DWORD dwLineBytes	= m_LoadBlance.Screen.width * sizeof(DWORD);
		IMemory* pNative	= ((_RCDDKMemory*)pSurface)->GetNative();


		for(DWORD i = 0;i<m_LoadBlance.dwCardCount;i++){
			DWORD buff[4];
			if(!m_LoadBlance.Performance[i].dwBlocks) continue;
#ifndef CROSS_COMPILE
			// ī�� ����
			m_pSystem->SetDevice(i);
			for(;;){// �������� ���� ������ ���.
				m_pSystem->Reads((RC_REG_OFFSET_CONTROL+0x38)>>2, (DWORD*)buff, 2);	
				//printf("Wait End Signal\n");
				//printf("Buff[1] = %08x\n",buff[1]);
				if(buff[1]&0x4){ 
				break;
				}
			}
			
#else
			for(;;){// �������� ���� ������ ���.
				m_pSystem->Reads((RC_REG_OFFSET_CONTROL+0x3C)>>2, (DWORD*)buff, 1);
				if(buff[0]&0x4) break;
			}
#endif
		
#ifndef CROSS_COMPILE
			// DMA �б�
			
			pNative->SetOffset(dwLineBytes * (dwCurBlock<<4));			
			pNative->SetDirty(0, dwLineBytes * ((m_LoadBlance.Performance[i].dwBlocks)<<4));			
			pNative->Flush(FALSE);			
			pNative->ClearDirty();			
			dwCurBlock	+= m_LoadBlance.Performance[i].dwBlocks;

			// ���� ����
			m_pSystem->Reads(RC_REG_OFFSET_REPORT>>2, buff, 2);	// buff[0] : clock count
			m_LoadBlance.Performance[i].dwElapse	= m_LoadBlance.Performance[i].dwScale * buff[0];
#endif
		}
#ifndef CROSS_COMPILE
		{	// load balancing
			DWORD	fastest_card	= 0;
			DWORD	slowest_card	= 0;
			DWORD	fastest_value	= 0xFFFFFFFF;
			DWORD	slowest_value	= 0;
			for(DWORD i = 0;i<m_LoadBlance.dwCardCount;i++){
				DWORD value	= m_LoadBlance.Performance[i].dwElapse;
				if(value < fastest_value){
					fastest_value	= value;
					fastest_card	= i;
				}
				if(value > slowest_value){
					slowest_value	= value;
					slowest_card	= i;
				}
			}

			m_LoadBlance.Performance[slowest_card].dwBlocks--;
			if(m_LoadBlance.Performance[slowest_card].dwBlocks)
				m_LoadBlance.Performance[fastest_card].dwBlocks++;
			else
				m_LoadBlance.Performance[slowest_card].dwBlocks++;
		
			for(DWORD i = 0;i<m_LoadBlance.dwCardCount;i++)
			if(m_LoadBlance.Performance[i].dwBlocks){
				m_pSystem->SetDevice(i);
				pNative->WaitDone();
			}
		}
#endif
	}

#ifdef USE_TESTDRIVE
	if(m_pTDRegmap){
		m_pTDRegmap->control.control.end	= 1;
		m_pTDRegmap->control.control.start	= 0;
		m_pTDRegmap->update.control	= TRUE;

		// put report
		for(DWORD i = 0;i<m_LoadBlance.dwCardCount;i++){
			m_pSystem->SetDevice(i);
			m_pSystem->Reads(RC_REG_OFFSET_REPORT>>2, (DWORD*)&m_pTDRegmap->report.device[i].report, REG_REPORT_SIZE);
		}
		m_pTDRegmap->update.report	= TRUE;
	}
#endif
	
	return TRUE;
}

BOOL rcDDKContext::ReportRegister(int& iDeviceID, EGL_REPORT* pReport){
	if(!m_pSystem) return FALSE;
#ifndef CROSS_COMPILE

	if(!pReport){
		iDeviceID	= m_pSystem->GetDeviceCount();
		return TRUE;
	}

	if(iDeviceID < 0 || (DWORD)iDeviceID >= m_pSystem->GetDeviceCount())
		return FALSE;

	m_pSystem->SetDevice(iDeviceID);
	{
		int core_count	= 0;
		pReport->device_id	= m_pSystem->GetDeviceID();
		switch(pReport->device_id){
		case 0x10121172:	// ArriaV
			pReport->core_count	= 2;
			pReport->clock_mhz	= 84;
			break;
		case 0x10321172:	// Stratix
			pReport->core_count	= 0;	// not supported.
			pReport->clock_mhz	= 100;
			break;
		case 0x10721172:	// ArriaX
			pReport->core_count = 7;
			pReport->clock_mhz = 170;
			break;
		}
		core_count	= pReport->core_count;

		m_pSystem->Reads(RC_REG_OFFSET_REPORT>>2, (DWORD*)&pReport->thread_count, 2);

		m_pSystem->Reads((RC_REG_OFFSET_REPORT+0x20)>>2, (DWORD*)pReport->ray_count, core_count);

		m_pSystem->Reads((RC_REG_OFFSET_REPORT+0x40)>>2, (DWORD*)&pReport->BusReadCount, 2);

		m_pSystem->Reads((RC_REG_OFFSET_REPORT+0x60)>>2, (DWORD*)&pReport->BusWriteCount, 2);

// 		m_pSystem->Reads((RC_REG_OFFSET_REPORT+0x80)>>2, (DWORD*)&pReport->ILL2MissCount, 2 + core_count*sizeof(EGL_REPORT_IL)/4);
// 
// 		m_pSystem->Reads((RC_REG_OFFSET_REPORT+0x100)>>2, (DWORD*)&pReport->ListL2MissCount, 2 + core_count*sizeof(EGL_REPORT_LIST)/4);
// 
// 		m_pSystem->Reads((RC_REG_OFFSET_REPORT+0x180)>>2, (DWORD*)&pReport->TransmitL2MissCount, 2 + core_count*sizeof(EGL_REPORT_TRANSMIT)/4);
// 
// 		m_pSystem->Reads((RC_REG_OFFSET_REPORT+0x200)>>2, (DWORD*)&pReport->TriMemMissCount, (1 + core_count + 1)&(~1));
// 
// 		m_pSystem->Reads((RC_REG_OFFSET_REPORT+0x280)>>2, (DWORD*)&pReport->TexMemMissCount, (1 + core_count + 1)&(~1));
// 
// 		m_pSystem->Reads((RC_REG_OFFSET_REPORT+0x300)>>2, (DWORD*)&pReport->TexNormalMemMissCount, (1 + core_count + 1)&(~1));

		for(int i=0;i<core_count;i++)
			m_pSystem->Reads((RC_REG_OFFSET_REPORT+0x400 + i&0x80)>>2, (DWORD*)&pReport->Render[i], (sizeof(EGL_REPORT_RENDER)/4 + 1)&(~1));
	}

	return TRUE;
#endif
	return FALSE;
}
