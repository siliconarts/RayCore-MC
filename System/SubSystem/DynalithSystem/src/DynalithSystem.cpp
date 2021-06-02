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
// Project : Dynalith system
// DATE    : 7/16/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "DynalithSystem.h"
#include "DynalithMemory.h"
#include "ConfigINI.h"

#include <stdio.h>
#include <assert.h>

#define		SYSTEM_MEMORY_BASE		0x00000000

DynalithSystem*	DynalithSystem::m_pCurrent	= NULL;

DynalithSystem::DynalithSystem(void)
{
#ifdef USE_TESTDRIVE
	m_pDisplayMemory	= TestDriver_GetMemory("RayCore_Display");
	if(m_pDisplayMemory)
			m_pDisplayConfig	= (DisplayConfig*)m_pDisplayMemory->GetConfig();
	else	m_pDisplayConfig	= NULL;
#endif

	m_pMemHive				= NULL;
	m_dwDisplayBaseAddress	= 0;
	m_pCurrent				= this;
}

DynalithSystem::~DynalithSystem(void){
#ifdef USE_TESTDRIVE
	TestDriver_Cleanup();
#endif
	m_pCurrent				= NULL;
}

IMemoryExtension* DynalithSystem::CreateMemory(DWORD dwPhysical, DWORD dwByteSize, BOOL bForceToDMA){
	return new DynalithMemory(dwPhysical, dwByteSize, bForceToDMA);
}

BOOL DynalithSystem::Initialize(IMemoryHive* pMemHive){
	// 메모리 힙 영역 초기화
	m_pMemHive	= pMemHive;
	pMemHive->Initialize(NULL, SYSTEM_MEMORY_BASE, 1024*1024*256, (IMemorySubsystem*)this);
	m_dwChipMask	= ((~g_ConfigINI.GetInt("RayCore", "chip_mask"))&0xF) << 24;
	printf("m_dwChipMask = 0x%X\n", m_dwChipMask);
	_flushall();

	return g_iProve.Initialize();;
}

void DynalithSystem::Release(void){
	// 메모리 하이브 해제
	if(m_pMemHive){
		m_pMemHive->Release();
		m_pMemHive	= NULL;
	}
	delete this;
}

void DynalithSystem::Reset(void){
	// Nothing to do...
}

DWORD DynalithSystem::GetDeviceCount(void){
	return 1;
}

BOOL DynalithSystem::SetDevice(DWORD dwCardID){
	return (dwCardID == 0);
}

DWORD DynalithSystem::GetDeviceID(void){
	return 0;
}

DWORD DynalithSystem::GetClockMHz(void){
	return 84;
}

DWORD DynalithSystem::Read(DWORD Offset32){
/*	switch(Offset32){
	case REG_DISPLAY_WIDTH:			return m_pDisplayConfig->iWidth;
	case REG_DISPLAY_HEIGHT:		return m_pDisplayConfig->iHeight;
	case REG_DISPLAY_FORMAT:		return m_pDisplayConfig->ColorFormat;
	case REG_DISPLAY_BYTE_STRIDE:	return m_pDisplayConfig->dwByteStride;
	case REG_DISPLAY_ADDRESS_FRONT:	return m_pDisplayConfig->Front.dwAddress + SYSTEM_MEMORY_BASE;
	case REG_DISPLAY_ADDRESS_BACK:	return m_pDisplayConfig->Back.dwAddress + SYSTEM_MEMORY_BASE;
	case REG_DISPLAY_UPDATE_FRONT:	return m_pDisplayConfig->Front.bUpdate;
	case REG_DISPLAY_UPDATE_BACK:	return m_pDisplayConfig->Back.bUpdate;
	default:						return RegRead(Offset32);
	}*/
	return 0;
}

void DynalithSystem::Write(DWORD Offset32, DWORD Data){
#ifdef USE_TESTDRIVE
	if(m_pDisplayConfig)
	switch(Offset32){
	case REG_DISPLAY_WIDTH:			m_pDisplayConfig->iWidth			= (int)Data;				break;
	case REG_DISPLAY_HEIGHT:		m_pDisplayConfig->iHeight			= (int)Data;				break;
	case REG_DISPLAY_FORMAT:		m_pDisplayConfig->ColorFormat		= (DISPLAY_FORMAT)Data;		break;
	case REG_DISPLAY_BYTE_STRIDE:	m_pDisplayConfig->dwByteStride		= Data;						break;

		break;
	default:
		break;
	}
#endif
}

void DynalithSystem::Reads(DWORD Offset32, DWORD* pData, int iCount){
	g_iProve.ReadRegister(Offset32, pData, iCount);
}

void DynalithSystem::Writes(DWORD Offset32, const DWORD* pData, int iCount){
	if(Offset32 == (RC_REG_OFFSET_CONTROL>>2)){
		// config 상의 chip mask 삽입
		((DWORD*)pData)[8]	|= m_dwChipMask;
	}
	g_iProve.WriteRegister(Offset32, pData, iCount);
}

void DynalithSystem::RegisterInterruptService(INTRRUPT_SERVICE routine){
	// do something...
}

// system memory
DWORD DynalithSystem::GetMemoryBase(void){
	return SYSTEM_MEMORY_BASE;
}

BOOL DynalithSystem::WriteMemory(DWORD dwPhyAddress, const void* pData, DWORD dwByteSize){
	g_iProve.WriteMemory(dwPhyAddress, (DWORD*)pData, dwByteSize);
	return TRUE;
}

BOOL DynalithSystem::ReadMemory(DWORD dwPhyAddress, void* pData, DWORD dwByteSize){
	g_iProve.ReadMemory(dwPhyAddress, (DWORD*)pData, dwByteSize);
	return TRUE;
}

ISystemImp* CreateSystemImplementation(void){
	return new DynalithSystem();
}
