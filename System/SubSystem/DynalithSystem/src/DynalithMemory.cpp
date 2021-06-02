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
// Project : DynalithSystem
// DATE    : 5/29/2013 Wed
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#include <stdio.h>
#include <assert.h>
#include "DynalithMemory.h"
#include "DynalithSystem.h"

DynalithMemory::DynalithMemory(DWORD dwPhysical, DWORD dwByteSize, BOOL bForceToDMA){
	m_dwPhysical		= dwPhysical;
	m_dwByteSize		= dwByteSize;
	m_Dirty.dwOffset	=
	m_Dirty.dwByteSize	= 0;
	m_dwOffsetVirtual	= 0;
	m_pVirtual			= new BYTE[dwByteSize];
}

DynalithMemory::~DynalithMemory(void){
	SAFE_DELETE_ARRAY(m_pVirtual);
}

void DynalithMemory::Release(void){
	delete this;
}
// memory object
PVOID DynalithMemory::Virtual(void){
	return m_pVirtual;
}

DWORD DynalithMemory::Physical(void){
	return m_dwPhysical;
}

DWORD DynalithMemory::ByteSize(void){
	return m_dwByteSize;
}

void DynalithMemory::SetOffset(DWORD dwOffset){
	m_dwOffsetVirtual	= dwOffset;
}
// memory validation
void DynalithMemory::SetDirty(DWORD dwOffset, DWORD dwByteSize){
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

void DynalithMemory::Flush(BOOL bCopyToPhysical){
	if(!m_Dirty.dwByteSize) return;

	DynalithSystem* pSystem	= DynalithSystem::GetCurrent();
	if(pSystem){
		if(bCopyToPhysical)
			pSystem->WriteMemory(m_dwPhysical + m_Dirty.dwOffset, (m_pVirtual + m_Dirty.dwOffset), m_Dirty.dwByteSize);
		else
			pSystem->ReadMemory(m_dwPhysical + m_Dirty.dwOffset, (m_pVirtual + m_Dirty.dwOffset), m_Dirty.dwByteSize);
	}
}

void DynalithMemory::WaitDone(void){
}

void DynalithMemory::ClearDirty(void){
	m_Dirty.dwByteSize	= 0;
}
