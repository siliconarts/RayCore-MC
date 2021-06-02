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
// Project : PCIe common
// DATE    : 7/11/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include <stdio.h>
#include <assert.h>
#include "SystemMemory.h"

SystemMemory::SystemMemory(unsigned long long int dwPhysical, DWORD dwByteSize, BOOL bForceToDMA){
	m_dwPhysical		= dwPhysical;
	m_dwByteSize		= dwByteSize;
	m_Dirty.dwOffset	=
	m_Dirty.dwByteSize	= 0;
	m_dwOffsetVirtual	= 0;
	if(bForceToDMA){
		m_pNative		= CreateNativeMemory(dwByteSize);
		if(!m_pNative){
			printf("*E : DMA allocation request is failed! (%d bytes). We try to normal memory allocation.\n", dwByteSize);
			goto TRY_NONE_DMA_ALLOC;
		}
		m_pVirtual		= (BYTE*)m_pNative->Virtual();
	}else{
TRY_NONE_DMA_ALLOC:
		m_pNative		= NULL;
		m_pVirtual		= new BYTE[dwByteSize];
	}
}

SystemMemory::~SystemMemory(void){
	if(m_pNative){
		m_pNative->Release();
		m_pNative	= NULL;
	}else{
		SAFE_DELETE_ARRAY(m_pVirtual);
	}
}

void SystemMemory::Release(void){
	delete this;
}
// memory object
PVOID SystemMemory::Virtual(void){
	return m_pVirtual;
}

unsigned long long int SystemMemory::Physical(void){
	return m_dwPhysical;
}

DWORD SystemMemory::ByteSize(void){
	return m_dwByteSize;
}

void SystemMemory::SetOffset(DWORD dwOffset){
	m_dwOffsetVirtual	= dwOffset;
}
// memory validation
void SystemMemory::SetDirty(DWORD dwOffset, DWORD dwByteSize){
	if(m_Dirty.dwByteSize){
		DWORD	mem_low, mem_high;
		mem_low	= m_Dirty.dwOffset < dwOffset ? m_Dirty.dwOffset : dwOffset;
		{
			DWORD org_high	= m_Dirty.dwOffset + m_Dirty.dwByteSize;
			DWORD new_high	= dwOffset + dwByteSize;
			mem_high		= org_high > new_high ? org_high : new_high;
		}

		m_Dirty.dwOffset	= mem_low;
		m_Dirty.dwByteSize	= mem_high - mem_low;
	}else{
		m_Dirty.dwOffset	= dwOffset;
		m_Dirty.dwByteSize	= dwByteSize;
	}
}

void SystemMemory::Flush(BOOL bCopyToPhysical){
	if(!m_Dirty.dwByteSize) return;
	
	PCIeSystem::GetSystem()->DMAFlush(bCopyToPhysical,
		this,
		m_Dirty.dwOffset,
		m_dwOffsetVirtual,
		m_Dirty.dwByteSize);
}

void SystemMemory::WaitDone(void){
	if(m_pNative) m_pNative->Wait();
}

void SystemMemory::ClearDirty(void){
	m_Dirty.dwByteSize	= 0;
}
