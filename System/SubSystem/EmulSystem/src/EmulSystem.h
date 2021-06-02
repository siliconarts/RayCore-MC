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
// DATE    : 8/7/2013 Wed
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#pragma once
#include "Emul/Emulation.h"

class EmulSystem :
	public ISystemImp,
	public ThreadManager,
	public Emulation
{
public:
	EmulSystem(void);
	virtual ~EmulSystem(void);
	// System
	STDMETHOD_(BOOL, Initialize)(IMemoryHive* pMemHive);					// �ý��� ����
	STDMETHOD_(void, Release)(void);										// �ý��� ����
	STDMETHOD_(void, Reset)(void);											// ����Ʈ ����
	
	// Memory interface
	STDMETHOD_(DWORD, GetMemoryBase)(void);									// �޸� ���� �ּ� ���
	STDMETHOD_(BOOL, WriteMemory)(DWORD dwPhyAddress, const void* pData, DWORD dwByteSize);	// �޸� ����
	STDMETHOD_(BOOL, ReadMemory)(DWORD dwPhyAddress, void* pData, DWORD dwByteSize);		// �޸� �б�

	// Hardware interface
	STDMETHOD_(DWORD, GetDeviceCount)(void);								// get device count
	STDMETHOD_(BOOL, SetDevice)(DWORD dwCardID);							// set current device
	STDMETHOD_(DWORD, GetDeviceID)(void);									// device id
	STDMETHOD_(DWORD, GetClockMHz)(void);									// get clock speed
	STDMETHOD_(DWORD, Read)(DWORD Offset32);								// �������� �б�
	STDMETHOD_(void, Write)(DWORD Offset32, DWORD Data);					// �������� ����
	STDMETHOD_(void, Reads)(DWORD Offset32, DWORD* pData, int iCount);			// �������͵� �б�
	STDMETHOD_(void, Writes)(DWORD Offset32, const DWORD* pData, int iCount);	// �������͵� ����
	STDMETHOD_(void, RegisterInterruptService)(INTRRUPT_SERVICE routine);	// ���ͷ�Ʈ ���� ��ƾ ���

	virtual void UpdateBackFramebuffer(void);

	virtual BYTE* GetPointer(DWORD dwAddress);
	virtual BOOL IsValidAddress(DWORD dwAddress);

protected:
	virtual void MonitorThread(void);
	virtual void OnThreadKill(void);

	virtual void WakeUpWorkThread(void);
	virtual void Lock(void);
	virtual void UnLock(void);

private:
	ITestDriverMemory*		m_pSystemMemory;
	ITestDriverMemory*		m_pDisplayMemory;
	DisplayConfig*			m_pDisplayConfig;
	IMemoryHive*			m_pMemHive;		// system heap memory stack
	

	Semaphore				m_ThreadLock;
	Semaphore				m_BusyLock;
	volatile BOOL			m_bThreadMustQuit;
	DWORD					m_dwDisplayBaseAddress;
};

extern EmulSystem*	g_pEmulSystem;