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
// DATE    : 6/19/2013 Wed
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#include "EmbeddedSystem.h"
#include "../../driver/embedded/raycore/driver_raycore.h"
#include "../../driver/embedded/driver_config.h"

ISystem* CreateSystem(void){
	EmbeddedSystem* pSystem	= new EmbeddedSystem();
	if(!pSystem->IsInitialized()){
		delete pSystem;
		pSystem	= NULL;
	}
	return pSystem;
}

EmbeddedSystem::EmbeddedSystem(void){
	m_iDriver	= open(RAYCORE_DEV_PATH,  O_RDWR);
}

EmbeddedSystem::~EmbeddedSystem(void){
	if(m_iDriver>=0) close(m_iDriver);
}

void EmbeddedSystem::Release(void){
	delete this;
}

void EmbeddedSystem::Reset(void){
	// no operation
}
	
DWORD EmbeddedSystem::GetMemoryBase(void){
	return VMEM_PHYSICAL_BASE;
}

BOOL EmbeddedSystem::WriteMemory(DWORD dwAddress, const void* pData, DWORD dwByteSize){
	return FALSE;	// don't care
}
	
BOOL EmbeddedSystem::ReadMemory(DWORD dwAddress, void* pData, DWORD dwByteSize){
	return FALSE;	// don't care
}

DWORD EmbeddedSystem::GetDeviceCount(void){
	return 1;		// don't care
}

BOOL EmbeddedSystem::SetDevice(DWORD dwCardID){
	return (dwCardID==0);	// don't care
}

DWORD EmbeddedSystem::GetDeviceID(void){
	return 0;		// don't care
}

DWORD EmbeddedSystem::GetClockMHz(void){
	return 100;		// don't care
}

DWORD EmbeddedSystem::Read(DWORD Offset32){
	return 0;		// don't care
}

void EmbeddedSystem::Write(DWORD Offset32, DWORD Data){
	// don't care
}

void EmbeddedSystem::Reads(DWORD Offset32, DWORD* pData, int iCount){
	RAYCORE_REG		reg;
	reg.bWrite		= 0;
	reg.dwCount		= iCount;
	reg.pReg		= (void*)pData;
	reg.dwOffset32	= Offset32;
	ioctl(m_iDriver, IOCTL_RAYCORE_TRANSACTION_REG, &reg);
}

void EmbeddedSystem::Writes(DWORD Offset32, const DWORD* pData, int iCount){
	RAYCORE_REG		reg;
	reg.bWrite		= 1;
	reg.dwCount		= iCount;
	reg.pReg		= (void*)pData;
	reg.dwOffset32	= Offset32;
	ioctl(m_iDriver, IOCTL_RAYCORE_TRANSACTION_REG, &reg);
}

void EmbeddedSystem::RegisterInterruptService(INTRRUPT_SERVICE routine){
}

// deprecated
void EmbeddedSystem::T_SetKDNode(void *ddkkd_node){}
void EmbeddedSystem::T_SetPrimitive(void *ddkPrePrimitive){}
