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
// DATE    : 5/28/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "rcDDK_Context.h"
#include <assert.h>

_RCDDKMemory::_RCDDKMemory(IMemory* pNative){
	assert(pNative != NULL);
	m_pNative	= pNative;
}

_RCDDKMemory::~_RCDDKMemory(void){
	MemoryFree(m_pNative);
}

void _RCDDKMemory::Release(void)								{delete this;}
void* _RCDDKMemory::Virtual(void)								{return m_pNative->Virtual();}
unsigned long long int _RCDDKMemory::Physical(void)								{return m_pNative->Physical();}
DWORD _RCDDKMemory::ByteSize(void)								{return m_pNative->ByteSize();}
void _RCDDKMemory::SetDirty(DWORD dwOffset, DWORD dwByteSize)	{m_pNative->SetDirty(dwOffset, dwByteSize);}
void _RCDDKMemory::Flush(BOOL bCopyToPhysical){
	ISystem* pSystem	= rcDDKContext::GET_CURRENT()->GetSystem();
	DWORD dwCardCount = pSystem->GetDeviceCount();
	for(DWORD i=0;i<dwCardCount;i++){
		pSystem->SetDevice(i);
		m_pNative->Flush(bCopyToPhysical);
		//m_pNative->WaitDone();		// 이 함수가 호출될 때에는 DMA 가 아니므로 실제로 필요가 없음.
	}
	m_pNative->ClearDirty();
}

RCDDK_API RCDDKMemory* RCDDK_ENTRY RCDDK_CreateMemory(DWORD dwByteSize, DWORD dwByteAlignment, BOOL bForceToDMA){
	IMemory* pNative = MemoryAlloc(dwByteSize, dwByteAlignment, bForceToDMA);
	if(pNative) return new _RCDDKMemory(pNative);
	return NULL;
}
