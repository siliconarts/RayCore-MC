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
// Project : Embedded system
// DATE    : 7/26/2013 Fri
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include <stdio.h>
#include <assert.h>
#include "EmbeddedMemory.h"

int			EmbeddedMemory::m_iDriver			= -1;
int			EmbeddedMemory::m_iDriverRefCount	= 0;
VMEM_INFO	EmbeddedMemory::m_iDriverInfo;

EmbeddedMemory*	EmbeddedMemory::m_pHead			= NULL;
		
IMemory* MemoryAlloc(DWORD dwByteSize, DWORD dwByteAlignment, BOOL bForceToDMA){
	EmbeddedMemory* pMem	= new EmbeddedMemory(dwByteSize, dwByteAlignment);
	if(pMem && !pMem->Virtual()){
		delete pMem;
		pMem	= NULL;
	}
	return pMem;
}

void MemoryFree(IMemory* pMemory){
	EmbeddedMemory* pMem	= (EmbeddedMemory*)pMemory;
	if(pMem) delete pMem;
}

void MemoryEnumerate(MEMORY_ENUMERATE_CALLBACK enum_func, void* pPrivate){
	EmbeddedMemory* pNode = EmbeddedMemory::GetHead();
	while(pNode){
		enum_func(pNode, pPrivate);
		pNode	= pNode->GetNext();
	}
}


EmbeddedMemory::EmbeddedMemory(DWORD dwByteSize, DWORD dwByteAlignment){
	m_Desc.pPrivate		= NULL;
	m_pVirtual			= NULL;
	if(m_iDriver<0){		// open driver
		m_iDriver	= open(VMEM_DEV_PATH,  O_RDWR);
		ioctl(m_iDriver, IOCTL_RAYCORE_VMEM_INFO, &m_iDriverInfo);
	}
	m_iDriverRefCount++;
	
	if(m_iDriver>=0){
		m_Desc.dwByteSize		= dwByteSize;
		m_Desc.dwAlignment		= dwByteAlignment;
		ioctl(m_iDriver, IOCTL_RAYCORE_VMEM_ALLOC, &m_Desc);
		if(m_Desc.pPrivate){
			m_pVirtual	= (BYTE*)mmap(0, m_Desc.dwByteSize, PROT_READ|PROT_WRITE, MAP_SHARED, m_iDriver, m_Desc.dwPhysical);
			if(!m_pVirtual) printf("Failed alloc memory : %d\n", dwByteSize);
		}
	}
	{	// link
		m_pNext		= m_pHead;
		m_pHead		= this;
	}
}
EmbeddedMemory::~EmbeddedMemory(void){
	{	// unlink
		if(m_pHead == this) m_pHead = m_pNext;
		else{
			EmbeddedMemory* pNode = m_pHead;
			while(pNode->m_pNext != this) pNode	= pNode->m_pNext;
			pNode->m_pNext	= m_pNext;
		}
	}
	if(m_pVirtual){
		munmap(m_pVirtual, m_Desc.dwByteSize);
		m_pVirtual	= NULL;
	}
	if(m_Desc.pPrivate){
		ioctl(m_iDriver, IOCTL_RAYCORE_VMEM_FREE, &m_Desc);
		m_Desc.pPrivate	= NULL;
	}
	m_iDriverRefCount--;
	if(!m_iDriverRefCount){
		if(m_iDriver>=0){	// release driver
			close(m_iDriver);
			m_iDriver	= -1;
		}
	}
}

void* EmbeddedMemory::Virtual(void){
	return m_pVirtual;
}

DWORD EmbeddedMemory::Physical(void){
	return m_Desc.dwPhysical;
}

DWORD EmbeddedMemory::ByteSize(void){
	return m_Desc.dwByteSize;
}

// not use in embedded system
void EmbeddedMemory::SetOffset(DWORD dwOffset){}
void EmbeddedMemory::SetDirty(DWORD dwOffset, DWORD dwByteSize){}
void EmbeddedMemory::Flush(BOOL bCopyToPhysical){
	//ioctl(m_iDriver, IOCTL_RAYCORE_VMEM_FLUSH, 0);
	//msync(m_pVirtual, m_Desc.dwByteSize, MS_INVALIDATE);
}
void EmbeddedMemory::WaitDone(void){}
void EmbeddedMemory::ClearDirty(void){}
