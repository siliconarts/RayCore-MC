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

#pragma once
#include "iProveInterface.h"
#include "DynalithMemory.h"
#include "driver/REGMAP.h"

#ifdef USE_TESTDRIVE
#include "TestDriver.h"
#include "VirtualDisplayConfig.h"
#include "driver/SystemConfig.h"
#endif

class DynalithSystem :
	public ISystemImp,
	public IMemorySubsystem
{
public:
	DynalithSystem(void);
	virtual ~DynalithSystem(void);
	// System
	STDMETHOD_(BOOL, Initialize)(IMemoryHive* pMemHive);					// 시스템 생성
	STDMETHOD_(void, Release)(void);										// 시스템 해제
	
	// Memory interface
	STDMETHOD_(DWORD, GetMemoryBase)(void);									// 메모리 시작 주소 얻기
	STDMETHOD_(BOOL, WriteMemory)(DWORD dwPhyAddress, const void* pData, DWORD dwByteSize);	// 메모리 쓰기
	STDMETHOD_(BOOL, ReadMemory)(DWORD dwPhyAddress, void* pData, DWORD dwByteSize);		// 메모리 읽기

	// Hardware interface
	STDMETHOD_(DWORD, GetDeviceCount)(void);								// get device count
	STDMETHOD_(BOOL, SetDevice)(DWORD dwCardID);							// set current device
	STDMETHOD_(DWORD, GetDeviceID)(void);									// device id
	STDMETHOD_(DWORD, GetClockMHz)(void);									// get clock speed
	STDMETHOD_(void, Reset)(void);											// 소프트 리셋
	STDMETHOD_(DWORD, Read)(DWORD Offset32);								// 레지스터 읽기
	STDMETHOD_(void, Write)(DWORD Offset32, DWORD Data);					// 레지스터 쓰기
	STDMETHOD_(void, Reads)(DWORD Offset32, DWORD* pData, int iCount);			// 레지스터들 읽기
	STDMETHOD_(void, Writes)(DWORD Offset32, const DWORD* pData, int iCount);	// 레지스터들 쓰기
	STDMETHOD_(void, RegisterInterruptService)(INTRRUPT_SERVICE routine);	// 인터럽트 서비스 루틴 등록

	// 시스템 메모리 확장 컨트롤
	STDMETHOD_(IMemoryExtension*, CreateMemory)(DWORD dwPhysical, DWORD dwByteSize, BOOL bForceToDMA = FALSE);

	static inline DynalithSystem* GetCurrent(void)		{return m_pCurrent;}

private:
	static DynalithSystem*	m_pCurrent;
#ifdef USE_TESTDRIVE
	ITestDriverMemory*		m_pDisplayMemory;
	DisplayConfig*			m_pDisplayConfig;
#endif
	IMemoryHive*			m_pMemHive;		// system heap memory stack

	DWORD					m_dwDisplayBaseAddress;
	DWORD					m_dwChipMask;
};
