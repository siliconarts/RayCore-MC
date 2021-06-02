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
// Project : DynalithSystem
// DATE    : 4/18/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#pragma once
#include "driver/SystemHAL.h"

class DynalithMemory : public IMemoryExtension{
protected:
	~DynalithMemory(void);
public:
	DynalithMemory(DWORD dwPhysical, DWORD dwByteSize, BOOL bForceToDMA);

	STDMETHOD_(void, Release)(void);
	// memory object
	STDMETHOD_(PVOID, Virtual)(void);
	STDMETHOD_(DWORD, Physical)(void);
	STDMETHOD_(DWORD, ByteSize)(void);
	STDMETHOD_(void, SetOffset)(DWORD dwOffset);
	// memory validation
	STDMETHOD_(void, SetDirty)(DWORD dwOffset, DWORD dwByteSize);
	STDMETHOD_(void, Flush)(BOOL bCopyToPhysical);
	STDMETHOD_(void, WaitDone)(void);
	STDMETHOD_(void, ClearDirty)(void);

private:
	BYTE*				m_pVirtual;
	DWORD				m_dwPhysical;
	DWORD				m_dwByteSize;
	DWORD				m_dwOffsetVirtual;

	struct{
		DWORD			dwOffset;			// start offset
		DWORD			dwByteSize;			// byte size
	}m_Dirty;
};
