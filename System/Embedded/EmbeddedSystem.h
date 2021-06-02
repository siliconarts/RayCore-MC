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
// DATE    : 6/18/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#ifndef __EMBEDDED_SYSTEM_H__
#define __EMBEDDED_SYSTEM_H__

#include "driver/SystemHAL.h"
#include "EmbeddedMemory.h"

class EmbeddedSystem : public ISystem
{
public:
	EmbeddedSystem(void);
	virtual ~EmbeddedSystem(void);

// System
	STDMETHOD_(void, Release)(void);
	STDMETHOD_(void, Reset)(void);
	
	// Memory interface
	STDMETHOD_(DWORD, GetMemoryBase)(void);
	STDMETHOD_(BOOL, WriteMemory)(DWORD dwAddress, const void* pData, DWORD dwByteSize);
	STDMETHOD_(BOOL, ReadMemory)(DWORD dwAddress, void* pData, DWORD dwByteSize);

	// Hardware interface
	STDMETHOD_(DWORD, GetDeviceCount)(void);
	STDMETHOD_(BOOL, SetDevice)(DWORD dwCardID);
	STDMETHOD_(DWORD, GetDeviceID)(void);
	STDMETHOD_(DWORD, GetClockMHz)(void);
	STDMETHOD_(DWORD, Read)(DWORD Offset32);
	STDMETHOD_(void, Write)(DWORD Offset32, DWORD Data);
	STDMETHOD_(void, Reads)(DWORD Offset32, DWORD* pData, int iCount);
	STDMETHOD_(void, Writes)(DWORD Offset32, const DWORD* pData, int iCount);
	STDMETHOD_(void, RegisterInterruptService)(INTRRUPT_SERVICE routine);

	// deprecated
	STDMETHOD_(void, T_SetKDNode)(void *ddkkd_node);
	STDMETHOD_(void, T_SetPrimitive)(void *ddkPrePrimitive);

	BOOL inline IsInitialized(void)		{return (m_iDriver>=0);}

protected:
	int			m_iDriver;
};

#endif//__EMBEDDED_SYSTEM_H__
