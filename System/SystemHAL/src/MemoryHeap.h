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
// Project : HAL
// DATE    : 4/18/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __MEMORY_HEAP_H__
#define __MEMORY_HEAP_H__
#include "driver/SystemHAL.h"
#include "CircularLink.h"

class MemoryHeap;
class MemoryImplementation;

typedef	CircularLink<MemoryHeap*, 0>	HeapLink;
typedef	CircularLink<MemoryHeap*, 1>	FreeLink;

class MemoryHeap : public IMemory
{
public:
	MemoryHeap(DWORD dwReqBytes, DWORD dwByteAlignment = 4, BOOL bForceToDMA = FALSE);

	STDMETHOD_(void*, Virtual)(void);
	STDMETHOD_(unsigned long long int, Physical)(void);
	STDMETHOD_(DWORD, ByteSize)(void);
	STDMETHOD_(void, SetOffset)(DWORD dwOffset);
	STDMETHOD_(void, SetDirty)(DWORD dwOffset, DWORD dwByteSize);
	STDMETHOD_(void, Flush)(BOOL bCopyToPhysical = TRUE);
	STDMETHOD_(void, WaitDone)(void);
	STDMETHOD_(void, ClearDirty)(void);

	inline BOOL IsFree(void)			{return m_bFree;}

	BOOL Alloc(MemoryHeap* pHeap, DWORD dwByteSize, DWORD dwByteAlignment = 4, BOOL bForceToDMA = FALSE);
	void Free(void);

protected:
	virtual ~MemoryHeap(void);
	MemoryHeap(BYTE* pVirtual, unsigned long long int dwPhysical, DWORD dwByteSize);

	friend class MemoryImplementation;

private:
	void Initialize(BOOL bFree);

	// linkage
	HeapLink					m_Link;				// all heap node link
	FreeLink					m_Free;				// fast free heap searching link
	// is free
	BOOL						m_bFree;			// empty heap?
	// memory info.
	BYTE*						m_pVirtual;			// virtual memory pointer
	unsigned long long int						m_dwPhysical;		// physical memory address
	DWORD						m_dwByteSize;		// byte size

	IMemoryExtension*			m_pExtension;		// Extension memory management
};

class MemoryImplementation : public IMemoryHive
{
public:
	MemoryImplementation(void);
	~MemoryImplementation(void);

	STDMETHOD_(BOOL, Initialize)(BYTE* pVirtual, unsigned long long int dwPhysical, DWORD dwByteSize, IMemorySubsystem* pMemSubsystem);
	STDMETHOD_(void, Release)(void);

	void Report(void);

private:
};

extern MemoryImplementation	g_MemoryImplementation;

#endif//__MEMORY_HEAP_H__
