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
// DATE    : 5/27/2013 Mon
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#pragma once
#include "PCIeSystem.h"
#include "driver/SystemHAL.h"

class SystemMemory : public IMemoryExtension{
protected:
	~SystemMemory(void);
public:
	SystemMemory(unsigned long long int dwPhysical, DWORD dwByteSize, BOOL bForceToDMA);	

	STDMETHOD_(void, Release)(void);
	// memory object
	STDMETHOD_(PVOID, Virtual)(void);
	STDMETHOD_(unsigned long long int, Physical)(void);
	STDMETHOD_(DWORD, ByteSize)(void);
	STDMETHOD_(void, SetOffset)(DWORD dwOffset);
	// memory validation
	STDMETHOD_(void, SetDirty)(DWORD dwOffset, DWORD dwByteSize);
	STDMETHOD_(void, Flush)(BOOL bCopyToPhysical);
	STDMETHOD_(void, WaitDone)(void);
	STDMETHOD_(void, ClearDirty)(void);

	inline NativeMemory* GetNativeMemory(void)	{return m_pNative;}

private:
	BYTE*				m_pVirtual;
	unsigned long long int				m_dwPhysical;	
	DWORD				m_dwByteSize;
	DWORD				m_dwOffsetVirtual;
	NativeMemory*		m_pNative;

	struct{
		DWORD			dwOffset;			// start offset
		DWORD			dwByteSize;			// byte size
	}m_Dirty;
};
