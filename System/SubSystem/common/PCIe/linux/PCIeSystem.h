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

#ifndef __PCIE_SYSTEM_H__
#define __PCIE_SYSTEM_H__
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "NativeInterface.h"

#include "PCIeMemory.h"

#include "SystemMemory.h"
class SystemMemory;

class PCIeSystem :
	public NativeSystem
{
public:
	PCIeSystem(void);
	virtual ~PCIeSystem(void);

	STDMETHOD_(DWORD, GetCardCount)(void);
	STDMETHOD_(BOOL, SetCurrentCard)(DWORD dwCardID);
	STDMETHOD_(DWORD, GetCurrentCardID)(void);
	STDMETHOD_(DWORD, GetCurrentSubsystemID)(void);
	STDMETHOD_(void, RegReads)(DWORD Offset32, DWORD* pData, int iCount);
	STDMETHOD_(void, RegWrites)(DWORD Offset32, const DWORD* pData, int iCount);
	STDMETHOD_(void, MemReads)(unsigned long long int dwPhysical, DWORD* pData, DWORD dwByteSize);
	STDMETHOD_(void, MemWrites)(unsigned long long int dwPhysical, DWORD* pData, DWORD dwByteSize);
	STDMETHOD_(void, DMAFlush)(BOOL bCopyToPhysical, SystemMemory* pMem, DWORD dwOffsetPhy, DWORD dwOffsetVir, DWORD dwByteSize);

	inline int GetDeviceHandle(void)	{return m_fd;}

protected:
	STDMETHOD_(BOOL, OnSystemInitialize)(void);

private:
	int					m_fd;
	RC_DRIVER_INFO		m_info;
	RC_TRANSACTION		m_tran_reg;
	RC_TRANSACTION		m_tran_mem;
	DWORD				m_dwCurrentCardID;
};

#endif//__PCIE_SYSTEM_H__
