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
// DATE    : 7/26/2013 Fri
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "gl_Memory.h"

using namespace GL;

//-----------------------------------------------------------------------------------------------
// Memory
//-----------------------------------------------------------------------------------------------

Memory::Memory(void)
{
	m_pMem			= NULL;
	m_dwByteSize	= 0;
	m_pVirtual		= NULL;
	m_dwPhysical	= 0;
}

Memory::~Memory(void){
	if(m_pMem){
		// free
		Lock();
		m_pMem->Release();
		UnLock();
		m_pMem			= NULL;
		m_pVirtual		= NULL;
		m_dwPhysical	= 0;
		m_dwByteSize	= 0;
	}
}

BOOL Memory::Create(DWORD dwByteSize, DWORD dwByteAlignment, BOOL bForceToDMA){
	Lock();
	if(m_pMem) m_pMem->Release();
	m_pMem				= RCDDK_CreateMemory(dwByteSize, dwByteAlignment, bForceToDMA);
	UnLock();

	if(m_pMem && m_pMem->Virtual()){
		m_dwByteSize	= dwByteSize;
		m_pVirtual		= m_pMem->Virtual();
		m_dwPhysical	= m_pMem->Physical();
#if HW_SIMULATION_DUMP
		memset(m_pVirtual, 0, dwByteSize);
#endif
	}else{
		m_dwByteSize	= 0;
		m_pVirtual		= NULL;
		m_dwPhysical	= 0;
		return FALSE;
	}
	return TRUE;
}

void Memory::Destroy(void){
	if(m_pMem){
		Lock();
		m_pMem->Release();
		m_pMem	= NULL;
		UnLock();
	}
}

//-----------------------------------------------------------------------------------------------
// Memory Pool
//-----------------------------------------------------------------------------------------------
MemoryPool::MemoryPool(void){
	m_dwByteAlignment	= 16;
	m_bResized			= FALSE;
}

MemoryPool::~MemoryPool(void){
}

BOOL MemoryPool::Create(DWORD dwByteSize, DWORD dwByteAlignment, BOOL bForceToDMA){
	m_dwByteAlignment	= dwByteAlignment;
	assert(!bForceToDMA);	// not support growing DMA memory
	m_bResized			= TRUE;
	return Memory::Create(dwByteSize, dwByteAlignment, bForceToDMA);
}

BOOL MemoryPool::Resize(DWORD dwByteSize){
	BOOL	bRet			= FALSE;
	BYTE*	pBuff			= NULL;
	DWORD	dwPrevByteSize	= ByteSize();

	if(dwPrevByteSize){
		pBuff	= new BYTE[dwPrevByteSize];
		memcpy(pBuff, Virtual(), dwPrevByteSize);
	}

	if(Memory::Create(dwByteSize, m_dwByteAlignment)){
		m_bResized			= TRUE;
		if(pBuff){
			memcpy(Virtual(), pBuff, dwPrevByteSize);
			SetDirty(0, dwPrevByteSize);
		}
		bRet	= TRUE;
	}

	SAFE_DELETE_ARRAY(pBuff);
	return bRet;
}
