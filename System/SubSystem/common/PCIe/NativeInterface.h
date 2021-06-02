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
// DATE    : 10/1/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __PCIE_SYSTEM_INTERFACE_H__
#define __PCIE_SYSTEM_INTERFACE_H__
#if defined(WIN32) || defined(WIN64)
	#include <winsock2.h>
#endif
#include "driver/SystemHAL.h"

typedef union{
	UINT64	addr;
	struct{
		DWORD low;
		DWORD hi;
	};
}PAGE_ADDRESS;

class SystemMemory;

class NativeMemory{
protected:
	virtual ~NativeMemory(void);

public:
	NativeMemory(void);
	void Release(void);

	STDMETHOD_(void, Reads)(unsigned long long int dwPhysical, DWORD dwOffset, DWORD dwByteSize) PURE;
	STDMETHOD_(void, Wait)(void) PURE;
	inline PVOID Virtual(void)		{return m_pVirtual;}

protected:
	STDMETHOD_(BOOL, Alloc)(DWORD dwByteSize) PURE;

	PVOID		m_pVirtual;
};

NativeMemory* CreateNativeMemory(DWORD dwByteSize);


class NativeSystem
{
public:
	NativeSystem(void);
	virtual ~NativeSystem(void);

	BOOL SystemInitialize(void);
 	STDMETHOD_(DWORD, GetCardCount)(void) PURE;
	STDMETHOD_(BOOL, SetCurrentCard)(DWORD dwCardID) PURE;
	STDMETHOD_(DWORD, GetCurrentCardID)(void) PURE;
	STDMETHOD_(DWORD, GetCurrentSubsystemID)(void) PURE;
	STDMETHOD_(void, RegReads)(DWORD Offset32, DWORD* pData, int iCount) PURE;
	STDMETHOD_(void, RegWrites)(DWORD Offset32, const DWORD* pData, int iCount) PURE;
	STDMETHOD_(void, MemReads)(unsigned long long int dwPhysical, DWORD* pData, DWORD dwByteSize) PURE;
	STDMETHOD_(void, MemWrites)(unsigned long long int dwPhysical, DWORD* pData, DWORD dwByteSize) PURE;
	STDMETHOD_(void, DMAFlush)(BOOL bCopyToPhysical, SystemMemory* pMem, DWORD dwOffsetPhy, DWORD dwOffsetVir, DWORD dwByteSize) PURE;

	static inline NativeSystem* GetSystem(void)	{return m_pCurrentSystem;}

protected:
	STDMETHOD_(BOOL, OnSystemInitialize)(void) PURE;

	static NativeSystem*	m_pCurrentSystem;
	DWORD					m_dwCardCount;
};

#endif//__PCIE_SYSTEM_INTERFACE_H__
