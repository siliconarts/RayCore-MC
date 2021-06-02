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
// DATE    : 7/16/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#pragma once
#include "PCIeSystem.h"
#include "driver/REGMAP.h"

#ifdef USE_TESTDRIVE
#include "TestDriver.h"
#include "VirtualDisplayConfig.h"
#include "driver/SystemConfig.h"
#endif

class ArriaVSystem :
	public ISystemImp,
	public IMemorySubsystem,
	public PCIeSystem
{
public:
	ArriaVSystem(void);
	virtual ~ArriaVSystem(void);
	// System
	STDMETHOD_(BOOL, Initialize)(IMemoryHive* pMemHive);					// �ý��� ����
	STDMETHOD_(void, Release)(void);										// �ý��� ����
	
	// Memory interface
	STDMETHOD_(unsigned long long, GetMemoryBase)(void);									// �޸� ���� �ּ� ���
	STDMETHOD_(BOOL, WriteMemory)(DWORD dwPhyAddress, const void* pData, DWORD dwByteSize);	// �޸� ����
	STDMETHOD_(BOOL, ReadMemory)(DWORD dwPhyAddress, void* pData, DWORD dwByteSize);		// �޸� �б�

	// Hardware interface
	STDMETHOD_(DWORD, GetDeviceCount)(void);								// get device count
	STDMETHOD_(BOOL, SetDevice)(DWORD dwCardID);							// set current device
	STDMETHOD_(DWORD, GetDeviceID)(void);									// device id
	STDMETHOD_(DWORD, GetClockMHz)(void);									// get clock speed
	STDMETHOD_(void, Reset)(void);											// ����Ʈ ����
	STDMETHOD_(DWORD, Read)(DWORD Offset32);								// �������� �б�
	STDMETHOD_(void, Write)(DWORD Offset32, DWORD Data);					// �������� ����
	STDMETHOD_(void, Reads)(DWORD Offset32, DWORD* pData, int iCount);			// �������͵� �б�
	STDMETHOD_(void, Writes)(DWORD Offset32, const DWORD* pData, int iCount);	// �������͵� ����
	STDMETHOD_(void, RegisterInterruptService)(INTRRUPT_SERVICE routine);	// ���ͷ�Ʈ ���� ��ƾ ���
	
	// �ý��� �޸� Ȯ�� ��Ʈ��
	STDMETHOD_(IMemoryExtension*, CreateMemory)(unsigned long long int dwPhysical, DWORD dwByteSize, BOOL bForceToDMA = FALSE);

private:
#ifdef USE_TESTDRIVE
	ITestDriverMemory*		m_pSystemMemory;
	ITestDriverMemory*		m_pDisplayMemory;
	DisplayConfig*			m_pDisplayConfig;
	SYSTEM_CONFIG*			m_pSystemConfig;
#endif
	IMemoryHive*			m_pMemHive;		// system heap memory stack

	DWORD					m_dwDisplayBaseAddress;
};
