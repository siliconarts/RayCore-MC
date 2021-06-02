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
// Project : AlteraV system
// DATE    : 11/19/2013 Tue
// Author  : Nobody
//
//=======================================================================

#include "ArriaVSystem.h"
#include "SystemMemory.h"

#include <stdio.h>
#include <assert.h>

#define		SYSTEM_MEMORY_BASE		0x00000000

ArriaVSystem::ArriaVSystem(void)
{
#ifdef USE_TESTDRIVE
	m_pSystemMemory		= TestDriver_GetMemory("RayCore");
	if(m_pSystemMemory)
			m_pSystemConfig		= (SYSTEM_CONFIG*)m_pSystemMemory->GetConfig();
	else	m_pSystemConfig		= NULL;

	m_pDisplayMemory	= TestDriver_GetMemory("RayCore_Display");
	if(m_pDisplayMemory)
			m_pDisplayConfig	= (DisplayConfig*)m_pDisplayMemory->GetConfig();
	else	m_pDisplayConfig	= NULL;
#endif

	m_pMemHive				= NULL;
	m_dwDisplayBaseAddress	= 0;
}

ArriaVSystem::~ArriaVSystem(void){
#ifdef USE_TESTDRIVE
	TestDriver_Cleanup();
#endif
}

IMemoryExtension* ArriaVSystem::CreateMemory(unsigned long long int dwPhysical, DWORD dwByteSize, BOOL bForceToDMA){
	return new SystemMemory(dwPhysical, dwByteSize, bForceToDMA);
}

BOOL ArriaVSystem::Initialize(IMemoryHive* pMemHive){
	// �޸� �� ���� �ʱ�ȭ
	m_pMemHive	= pMemHive;
	if(!SystemInitialize()) return FALSE;

	{
		DWORD installed_memory_size		= 0xFFFF;
		for(DWORD i=0;i<GetCardCount();i++){
			SetCurrentCard(i);
		
			DWORD cur_memory_size;
			switch(GetDeviceID()){
			default:
			case 0x10121172:	cur_memory_size	= 256;	break;	// arriav
			case 0x10321172:	cur_memory_size	= 512;	break;	// stratixv
			case 0x10721172:	cur_memory_size = 512;	break;	// ArriaX
			}
			if(cur_memory_size<installed_memory_size)
				installed_memory_size	= cur_memory_size;
		}

		pMemHive->Initialize(NULL, SYSTEM_MEMORY_BASE, 1024*1024*installed_memory_size, (IMemorySubsystem*)this);
		SetCurrentCard(0);
	}
	return TRUE;
}

void ArriaVSystem::Release(void){
	// �޸� ���̺� ����
	if(m_pMemHive){
		m_pMemHive->Release();
		m_pMemHive	= NULL;
	}
	delete this;
}

void ArriaVSystem::Reset(void){
	// Nothing to do...
}

DWORD ArriaVSystem::GetDeviceCount(void){
	return GetCardCount();
}

BOOL ArriaVSystem::SetDevice(DWORD dwCardID){
	return SetCurrentCard(dwCardID);
}

DWORD ArriaVSystem::GetDeviceID(void){
	return GetCurrentSubsystemID();
}

DWORD ArriaVSystem::GetClockMHz(void){
	switch(GetCurrentSubsystemID()){
	case 0x10121172:	return 84;
	case 0x10321172:	return 100;
	case 0x10721172:	return 170;
	default: assert(0);
	}
	return 0;
}

DWORD ArriaVSystem::Read(DWORD Offset32){
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

void ArriaVSystem::Write(DWORD Offset32, DWORD Data){
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

void ArriaVSystem::Reads(DWORD Offset32, DWORD* pData, int iCount){
	RegReads(Offset32, pData, iCount);
}

void ArriaVSystem::Writes(DWORD Offset32, const DWORD* pData, int iCount){
	RegWrites(Offset32, pData, iCount);
}

void ArriaVSystem::RegisterInterruptService(INTRRUPT_SERVICE routine){
	// do something...
}

// system memory
unsigned long long ArriaVSystem::GetMemoryBase(void){
	return SYSTEM_MEMORY_BASE;
}

BOOL ArriaVSystem::WriteMemory(DWORD dwPhyAddress, const void* pData, DWORD dwByteSize){
	MemWrites(dwPhyAddress, (DWORD*)pData, dwByteSize);
	return TRUE;
}

BOOL ArriaVSystem::ReadMemory(DWORD dwPhyAddress, void* pData, DWORD dwByteSize){
	MemReads(dwPhyAddress, (DWORD*)pData, dwByteSize);
	return TRUE;
}

ISystemImp* CreateSystemImplementation(void){
	return new ArriaVSystem();
}
