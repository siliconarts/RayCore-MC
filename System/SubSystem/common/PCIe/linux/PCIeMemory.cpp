// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore DDK v 1.0
// Date : 

#include "PCIeMemory.h"
#include "PCIeSystem.h"
#include <memory.h>
#include <assert.h>

NativeMemory* CreateNativeMemory(DWORD dwByteSize){
	PCIeMemory* pMem	= new PCIeMemory();
	if(!pMem->Alloc(dwByteSize)){
		pMem->Release();
		pMem	= NULL;
	}
	return pMem;
}

PCIeMemory::PCIeMemory(void){
	m_fd		= ((PCIeSystem*)NativeSystem::GetSystem())->GetDeviceHandle();
	memset(&m_dma, 0, sizeof(DMA_MEMORY_USER));
	memset(&m_trans, 0, sizeof(RC_TRANSACTION));
	//trans.write_enable	= 0;
}

PCIeMemory::~PCIeMemory(void){
	if(m_pVirtual){
		munmap(m_pVirtual, m_dma.byte_size);
		m_pVirtual	= NULL;
	}
	if(m_dma.kernel_dma) ioctl(m_fd, IOCTL_RAYCORE_DMA_FREE, &m_dma);
}

BOOL PCIeMemory::Alloc(DWORD dwByteSize){
	dwByteSize = (dwByteSize + 4095) & (~4095);
	
	m_dma.byte_size	= dwByteSize;
	if(ioctl(m_fd, IOCTL_RAYCORE_DMA_ALLOC, &m_dma)) return FALSE;
	
	m_pVirtual	= (void*)mmap(0, m_dma.byte_size, PROT_READ|PROT_WRITE, MAP_SHARED, m_fd, m_dma.phy_addr);
	
	m_trans.pData	= m_dma.kernel_dma;
	
	return TRUE;
}

void PCIeMemory::Reads(unsigned long long int dwPhysical, DWORD dwOffset, DWORD dwByteSize){
	NativeSystem*	pSystem		= PCIeSystem::GetSystem();	
	assert((dwPhysical&(32 - 1)) == 0);			// must 32 byte(128bit) align
	assert((dwOffset&(8 - 1)) == 0);			// must 8 byte(64bit) align
	
	m_trans.device_id	= pSystem->GetCurrentCardID();
	m_trans.offset		= dwOffset;
	m_trans.byte_size	= dwByteSize;
	m_trans.phy_address	= (unsigned long long int)(dwPhysical >> 6);	
	//m_trans.phy_address	= (unsigned long long int)(dwPhysical);	


	ioctl(m_fd, IOCTL_RAYCORE_TRANSACTION_DMA, &m_trans);
}

void PCIeMemory::Wait(void){
	// wait DMA done signal.
	NativeSystem*	pSystem		= PCIeSystem::GetSystem();
	
	m_trans.device_id	= pSystem->GetCurrentCardID();
	m_trans.byte_size	= 0;
	ioctl(m_fd, IOCTL_RAYCORE_TRANSACTION_DMA, &m_trans);
}
