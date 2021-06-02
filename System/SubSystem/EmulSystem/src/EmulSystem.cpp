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
// Project : Emulation
// DATE    : 8/28/2013 Wed
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "StdAfx.h"
#include "EmulSystem.h"

#define		SYSTEM_MEMORY_BASE		0x00000000

EmulSystem*	g_pEmulSystem	= NULL;


EmulSystem::EmulSystem(void) :
	m_ThreadLock(0),
	m_BusyLock(1)
{
	m_pMemHive			= NULL;
	m_pSystemMemory		= TestDriver_GetMemory("RayCore");
	m_pDisplayMemory	= TestDriver_GetMemory("RayCore_Display");

	if(m_pDisplayMemory){
		m_pDisplayConfig	= (DisplayConfig*)m_pDisplayMemory->GetConfig();
		m_pDisplayConfig->bReverse			= TRUE;
		m_pDisplayConfig->ColorFormat		= DISPLAY_FORMAT_ABGR_8888;
		m_pDisplayConfig->Back.dwAddress	= 0;
		g_mem.pFramebuffer					= (DWORD*)m_pSystemMemory->GetPointer();
	}else
		m_pDisplayConfig	= NULL;

	m_bThreadMustQuit	= FALSE;
	g_pEmulSystem		= this;
}

EmulSystem::~EmulSystem(void){
	TestDriver_Cleanup();
	g_pEmulSystem		= NULL;
}

BOOL EmulSystem::Initialize(IMemoryHive* pMemHive){
	if(!m_pSystemMemory || !m_pDisplayMemory) return FALSE;

	// 메모리 힙 영역 초기화
	m_pMemHive	= pMemHive;
	pMemHive->Initialize(m_pSystemMemory->GetPointer(), SYSTEM_MEMORY_BASE, m_pSystemMemory->GetSize(), NULL);

	DoInit();
	RunThread();
	return TRUE;
}

void EmulSystem::Release(void){
	KillThread();
	// 메모리 하이브 해제
	if(m_pMemHive){
		m_pMemHive->Release();
		m_pMemHive	= NULL;
	}
	delete this;
}

void EmulSystem::MonitorThread(void){
	for(;;){
		m_ThreadLock.Down();
		if(m_bThreadMustQuit) break;
		DoWork();
	}
}

void EmulSystem::OnThreadKill(void){
	m_bThreadMustQuit	= TRUE;
	m_ThreadLock.Up();
}

void EmulSystem::Reset(void){
	DoReset();
}

DWORD EmulSystem::GetDeviceCount(void){
	return 1;
}

BOOL EmulSystem::SetDevice(DWORD dwCardID){
	return (dwCardID == 0);
}

DWORD EmulSystem::GetDeviceID(void){
	return 0x10000000;
}

DWORD EmulSystem::GetClockMHz(void){
	return 100;
}

DWORD EmulSystem::Read(DWORD Offset32){
	switch(Offset32){
	case REG_DISPLAY_WIDTH:			return m_pDisplayConfig->iWidth;
	case REG_DISPLAY_HEIGHT:		return m_pDisplayConfig->iHeight;
	case REG_DISPLAY_FORMAT:		return m_pDisplayConfig->ColorFormat;
	case REG_DISPLAY_BYTE_STRIDE:	return m_pDisplayConfig->dwByteStride;
	case REG_DISPLAY_ADDRESS_FRONT:	return m_pDisplayConfig->Front.dwAddress + SYSTEM_MEMORY_BASE;
	case REG_DISPLAY_ADDRESS_BACK:	return m_pDisplayConfig->Back.dwAddress + SYSTEM_MEMORY_BASE;
	case REG_DISPLAY_UPDATE_FRONT:	return m_pDisplayConfig->Front.bUpdate;
	case REG_DISPLAY_UPDATE_BACK:	return m_pDisplayConfig->Back.bUpdate;
	default:						return RegRead(Offset32);
	}
	return 0;
}

void EmulSystem::Write(DWORD Offset32, DWORD Data){
	switch(Offset32){
	case REG_DISPLAY_WIDTH:			m_pDisplayConfig->iWidth	= (int)Data;				break;
	case REG_DISPLAY_HEIGHT:		m_pDisplayConfig->iHeight	= (int)Data;				break;
	case REG_DISPLAY_FORMAT:		m_pDisplayConfig->ColorFormat	= (DISPLAY_FORMAT)Data;	break;
	case REG_DISPLAY_BYTE_STRIDE:	m_pDisplayConfig->dwByteStride	= Data;					break;
	case REG_DISPLAY_ADDRESS_FRONT:	m_pDisplayConfig->Front.dwAddress	= Data - SYSTEM_MEMORY_BASE;
	case REG_DISPLAY_UPDATE_FRONT:	m_pDisplayConfig->Front.bUpdate		= TRUE;				break;
	case REG_DISPLAY_ADDRESS_BACK:	m_pDisplayConfig->Back.dwAddress	= Data - SYSTEM_MEMORY_BASE;
									g_mem.pFramebuffer					= (DWORD*)GetPointer(Data);
	case REG_DISPLAY_UPDATE_BACK:	m_pDisplayConfig->Back.bUpdate		= TRUE;				break;
	default:						RegWrite(Offset32, Data);
	}
}

void EmulSystem::Reads(DWORD Offset32, DWORD* pData, int iCount){
	RegReads(Offset32, pData, iCount);
}

void EmulSystem::Writes(DWORD Offset32, const DWORD* pData, int iCount){
	RegWrites(Offset32, pData, iCount);
}

void EmulSystem::RegisterInterruptService(INTRRUPT_SERVICE routine){
	// do something...
}

void EmulSystem::WakeUpWorkThread(void){
	m_ThreadLock.Up();
}

void EmulSystem::Lock(void){
	m_BusyLock.Down();
}

void EmulSystem::UnLock(void){
	m_BusyLock.Up();
}

// system memory
DWORD EmulSystem::GetMemoryBase(void){
	return SYSTEM_MEMORY_BASE;
}

BOOL EmulSystem::WriteMemory(DWORD dwPhyAddress, const void* pData, DWORD dwByteSize){
	BYTE* pMem	= m_pSystemMemory->GetPointer(dwPhyAddress - SYSTEM_MEMORY_BASE, dwByteSize);
	if(pMem && pData){
		memcpy(pMem, pData, dwByteSize);
		return TRUE;
	}
	return FALSE;
}

BOOL EmulSystem::ReadMemory(DWORD dwPhyAddress, void* pData, DWORD dwByteSize){
	BYTE* pMem	= m_pSystemMemory->GetPointer(dwPhyAddress - SYSTEM_MEMORY_BASE, dwByteSize);
	if(pMem && pData){
		memcpy(pData, pMem, dwByteSize);
		return TRUE;
	}
	return FALSE;
}

BYTE* EmulSystem::GetPointer(DWORD dwAddress){
	if(dwAddress<SYSTEM_MEMORY_BASE) return NULL;
	dwAddress	-= SYSTEM_MEMORY_BASE;
	return m_pSystemMemory->GetPointer(dwAddress);
}

BOOL EmulSystem::IsValidAddress(DWORD dwAddress){
	dwAddress	-= SYSTEM_MEMORY_BASE;
	return m_pSystemMemory->IsValidAddress(dwAddress);
}

void EmulSystem::UpdateBackFramebuffer(void){
	m_pDisplayConfig->Back.bUpdate	= TRUE;
}
