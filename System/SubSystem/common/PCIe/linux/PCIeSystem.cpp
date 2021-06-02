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
// DATE    : 3/28/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "PCIeSystem.h"
#include "SystemMemory.h"
#include <memory.h>
#include <assert.h>

PCIeSystem::PCIeSystem(void){
	m_dwCurrentCardID	= 0;

	m_fd				= open(DEV_PATH, O_RDWR);	// open driver

	memset(&m_tran_reg, 0, sizeof(RC_TRANSACTION));
	memset(&m_tran_mem, 0, sizeof(RC_TRANSACTION));
}

PCIeSystem::~PCIeSystem(void){
	if(m_fd>0) close(m_fd);
}

BOOL PCIeSystem::OnSystemInitialize(void){
	if(m_fd<0) return FALSE;

	{
		if(ioctl(m_fd, IOCTL_RAYCORE_INFOMATION, &m_info))
			return FALSE;
		if(!m_info.card_count) return FALSE;
		m_dwCardCount	= m_info.card_count;
	}

	return TRUE;
}

DWORD PCIeSystem::GetCardCount(void){
	return m_dwCardCount;
}

BOOL PCIeSystem::SetCurrentCard(DWORD dwCardID){
	m_dwCurrentCardID	= dwCardID;
 return TRUE;
}
DWORD PCIeSystem::GetCurrentCardID(void){
	return m_dwCurrentCardID;
}

DWORD PCIeSystem::GetCurrentSubsystemID(void){
	return m_info.card_id[m_dwCurrentCardID];
}

void PCIeSystem::RegReads(DWORD Offset32, DWORD* pData, int iCount){
	m_tran_reg.write_enable	= 0;
	m_tran_reg.device_id	= m_dwCurrentCardID;
	m_tran_reg.offset		= Offset32 << 2;
	m_tran_reg.byte_size	= iCount << 2;
	m_tran_reg.pData		= (void*)pData;
	
	ioctl(m_fd, IOCTL_RAYCORE_TRANSACTION_REG, &m_tran_reg);
}

void PCIeSystem::RegWrites(DWORD Offset32, const DWORD* pData, int iCount){
	m_tran_reg.write_enable	= 1;
	m_tran_reg.device_id	= m_dwCurrentCardID;
	m_tran_reg.offset		= Offset32 << 2;
	m_tran_reg.byte_size	= iCount << 2;
	m_tran_reg.pData		= (void*)pData;

	ioctl(m_fd, IOCTL_RAYCORE_TRANSACTION_REG, &m_tran_reg);
}

void PCIeSystem::MemReads(unsigned long long int dwPhysical, DWORD* pData, DWORD dwByteSize){
	dwByteSize			+= (dwPhysical&0x3F) + 0x3F;	// dwByteSize �� 16*DWORD �� ����̾�� �Ѵ�.
	pData				= (DWORD*)(((BYTE*)pData) - (dwPhysical&0x3F));

	m_tran_mem.write_enable	= 0;
	m_tran_mem.device_id	= m_dwCurrentCardID;
	m_tran_mem.byte_size	= dwByteSize;
	m_tran_mem.pData		= pData;
	//m_tran_mem.phy_address	= (unsigned long lRCMC_API_Lite(GitHub_Final)/ong int)(dwPhysical>>6);
	m_tran_mem.phy_address	= (unsigned long long int)(dwPhysical >> 6);

	ioctl(m_fd, IOCTL_RAYCORE_TRANSACTION_MEM, &m_tran_mem);
}

void PCIeSystem::MemWrites(unsigned long long int dwPhysical, DWORD* pData, DWORD dwByteSize){
	dwByteSize			+= (dwPhysical&0x3F) + 0x3F;	// dwByteSize �� 16*DWORD �� ����̾�� �Ѵ�.
	pData				= (DWORD*)(((BYTE*)pData) - (dwPhysical&0x3F));

	m_tran_mem.write_enable	= 1;
	m_tran_mem.device_id	= m_dwCurrentCardID;
	m_tran_mem.byte_size	= dwByteSize;
	m_tran_mem.pData		= pData;
	m_tran_mem.phy_address	= (unsigned long long int)(dwPhysical >> 6);
	

	ioctl(m_fd, IOCTL_RAYCORE_TRANSACTION_MEM, &m_tran_mem);
}

void PCIeSystem::DMAFlush(BOOL bCopyToPhysical, SystemMemory* pMem, DWORD dwOffsetPhy, DWORD dwOffsetVir, DWORD dwByteSize){
	unsigned long long int dwPhysical		= pMem->Physical() + dwOffsetPhy;
	NativeMemory* pNative	= pMem->GetNativeMemory();

	if(pNative && !bCopyToPhysical){
//	if(0){	//DMA disable
		pNative->Reads(dwPhysical, dwOffsetPhy + dwOffsetVir, dwByteSize);
	}else
	{
		BYTE* pVirtual		= ((BYTE*)pMem->Virtual()) + dwOffsetPhy + dwOffsetVir;

		if(bCopyToPhysical)
			MemWrites(dwPhysical, (DWORD*)pVirtual, dwByteSize);
		else
			MemReads(dwPhysical, (DWORD*)pVirtual, dwByteSize);
	}
}
