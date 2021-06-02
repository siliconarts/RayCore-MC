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
// DATE    : 7/26/2013 Fri
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#ifndef __EMBEDDED_MEMORY_H__
#define __EMBEDDED_MEMORY_H__
#include "../../driver/embedded/vmem/driver_vmem.h"
#include "EmbeddedSystem.h"
#include "driver/SystemHAL.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

class EmbeddedMemory : public IMemory
{
public:
	EmbeddedMemory(DWORD dwByteSize, DWORD dwByteAlignment);
	virtual ~EmbeddedMemory(void);
	
	STDMETHOD_(void*, Virtual)(void);
	STDMETHOD_(DWORD, Physical)(void);
	STDMETHOD_(DWORD, ByteSize)(void);
	STDMETHOD_(void, SetOffset)(DWORD dwOffset);
	STDMETHOD_(void, SetDirty)(DWORD dwOffset, DWORD dwByteSize);
	STDMETHOD_(void, Flush)(BOOL bCopyToPhysical = TRUE);
	STDMETHOD_(void, WaitDone)(void);
	STDMETHOD_(void, ClearDirty)(void);
	
	inline static EmbeddedMemory* GetHead(void)	{return m_pHead;}
	inline EmbeddedMemory* GetNext(void)		{return m_pNext;}

private:
	static EmbeddedMemory*	m_pHead;
	EmbeddedMemory*			m_pNext;
	
	BYTE*				m_pVirtual;
	VMEM_DESC			m_Desc;

	// for driver
	static int			m_iDriver;
	static int			m_iDriverRefCount;
	static VMEM_INFO	m_iDriverInfo;
};

#endif//__EMBEDDED_MEMORY_H__
