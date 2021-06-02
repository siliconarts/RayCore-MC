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
// DATE    : 6/17/2013 Mon
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include <assert.h>
#include <stdio.h>
#include "MemoryHeap.h"

static IMemorySubsystem* __MEM_SUBSYSTEM	= NULL;
#ifdef _DEBUG
//#define USE_MEMORY_DEBUG

typedef struct{
	DWORD		peek, current;
}MEMORY_CONSUME;

static MEMORY_CONSUME	g_MemoryConsume = {0, 0};

#endif

#ifdef USE_MEMORY_DEBUG
	#define MEMORY_REPORT		g_MemoryImplementation.Report();
#else
	#define MEMORY_REPORT
#endif

void MemoryHeap::Initialize(BOOL bFree){
	m_bFree				= bFree;
	m_pExtension		= NULL;
	m_pVirtual			= NULL;
	m_dwPhysical		=
	m_dwByteSize		= 0;

	m_Link.SetObject(this);
	m_Free.SetObject(this);

	if(bFree){
		m_Link.Link();
		m_Free.Link();
	}
}

void ShowByteSize(DWORD dwSize);
MemoryHeap::MemoryHeap(DWORD dwReqBytes, DWORD dwByteAlignment, BOOL bForceToDMA){
	Initialize(FALSE);

	assert(dwReqBytes != 0);
	assert(dwByteAlignment != 0);

	{	// 빈 힙에서 공간 찾기
		FreeLink*		pFree	= m_Free.Head();
		while(pFree){
			if(pFree->Item()->Alloc(this, dwReqBytes, dwByteAlignment, bForceToDMA)){
				MEMORY_REPORT
				return;
			}
			pFree	= pFree->Next();
		}
	}
	printf("*E : Sorry! No more linear physical heap memory : ");
	ShowByteSize(dwReqBytes);
	printf("\n");
#ifdef WIN32
	_flushall();
#endif
}

MemoryHeap::~MemoryHeap(void){}

MemoryHeap::MemoryHeap(BYTE* pVirtual, unsigned long long int dwPhysical, DWORD dwByteSize){
	Initialize(TRUE);
	m_pVirtual			= pVirtual;
	m_dwPhysical		= dwPhysical;
	m_dwByteSize		= dwByteSize;
}

void MemoryHeap::Free(void){
#ifdef _DEBUG
	if(!m_bFree) g_MemoryConsume.current	-= m_dwByteSize;
#endif
	m_bFree	= TRUE;

	if(m_pExtension){
		SAFE_RELEASE(m_pExtension);
		m_pVirtual		= NULL;
	}
	{
		MemoryHeap* pCurHeap	= this;
		MemoryHeap*	pPrev		= m_Link.Prev() ? m_Link.Prev()->Item() : NULL;
		MemoryHeap*	pNext		= m_Link.Next() ? m_Link.Next()->Item() : NULL;
	
		if(pPrev && pPrev->IsFree()){
			pPrev->m_dwByteSize	+= m_dwByteSize;
			m_dwByteSize		= 0;
			pCurHeap			= pPrev;
		}
		if(pNext && pNext->IsFree()){
			pCurHeap->m_dwByteSize	+= pNext->m_dwByteSize;
			delete pNext;
		}
		if(!m_dwByteSize) delete this;
		else m_Free.Link();	// set to free link stack
	}
	MEMORY_REPORT
}

BOOL MemoryHeap::Alloc(MemoryHeap* pHeap, DWORD dwByteSize, DWORD dwByteAlignment, BOOL bForceToDMA){
	// adjust align size
	dwByteSize				+= (m_dwPhysical + m_dwByteSize - dwByteSize) & (dwByteAlignment-1);
	// check free size
	if(!m_bFree || m_dwByteSize < dwByteSize) return FALSE;

#ifdef _DEBUG
	g_MemoryConsume.current	+= dwByteSize;
	if(g_MemoryConsume.peek < g_MemoryConsume.current)
		g_MemoryConsume.peek = g_MemoryConsume.current;
#endif

	// set link (this >> pHeap(alloc) >> m_Link.pNext)
	pHeap->m_Link.Link(&m_Link);

	// set size
	pHeap->m_dwByteSize		= dwByteSize;
	m_dwByteSize			-= dwByteSize;

	// set physical
	pHeap->m_dwPhysical		= m_dwPhysical + m_dwByteSize;

	// set virtual
	if(m_pVirtual){
		pHeap->m_pVirtual		= m_pVirtual + m_dwByteSize;
	}else{
		if(!__MEM_SUBSYSTEM) return FALSE;
		pHeap->m_pExtension		= __MEM_SUBSYSTEM->CreateMemory(pHeap->m_dwPhysical, pHeap->m_dwByteSize, bForceToDMA);
		if(!pHeap->m_pExtension) return FALSE;
		pHeap->m_pVirtual		= (BYTE*)pHeap->m_pExtension->Virtual();
	}

	// set not free heap to new heap
	pHeap->m_bFree			= FALSE;

	return TRUE;
}

void MemoryHeap::SetOffset(DWORD dwOffset){
	if(m_pExtension) m_pExtension->SetOffset(dwOffset);
}

void MemoryHeap::SetDirty(DWORD dwOffset, DWORD dwByteSize){
	if(m_pExtension) m_pExtension->SetDirty(dwOffset, dwByteSize);
}

void MemoryHeap::Flush(BOOL bCopyToPhysical){
	if(m_pExtension) m_pExtension->Flush(bCopyToPhysical);
}

void MemoryHeap::WaitDone(void){
	if(m_pExtension) m_pExtension->WaitDone();
}

void MemoryHeap::ClearDirty(void){
	if(m_pExtension) m_pExtension->ClearDirty();
}

void ShowByteSize(DWORD dwSize){
	BOOL bShow = FALSE;

	if(dwSize>1024*1024*1024){
		DWORD	b_size	= dwSize / (1024*1024*1024);
		dwSize	-= b_size * (1024*1024*1024);
		//if(bShow) printf(" ");
		printf("%dGB", b_size);
		bShow	= TRUE;
	}

	if(dwSize>1024*1024){
		DWORD	b_size	= dwSize / (1024*1024);
		dwSize	-= b_size * (1024*1024);
		if(bShow) printf(" ");
		printf("%dMB", b_size);
		bShow	= TRUE;
	}

	if(dwSize>1024){
		DWORD	b_size	= dwSize / (1024);
		dwSize	-= b_size * (1024);
		if(bShow) printf(" ");
		printf("%dKB", b_size);
		bShow	= TRUE;
	}

	if(dwSize){
		if(bShow) printf(" ");
		printf("%d Byte", dwSize);
	}
}

void* MemoryHeap::Virtual(void){
	return m_pVirtual;
}

unsigned long long int MemoryHeap::Physical(void){
	return m_dwPhysical;
}

DWORD MemoryHeap::ByteSize(void){
	return m_dwByteSize;
}

IMemory* MemoryAlloc(DWORD dwByteSize, DWORD dwByteAlignment, BOOL bForceToDMA){
	return new MemoryHeap(dwByteSize, dwByteAlignment, bForceToDMA);
}

void MemoryFree(IMemory* pMemory){
	MemoryHeap* pHeap = (MemoryHeap*)pMemory;
	assert(pHeap);
	pHeap->Free();
}

void MemoryEnumerate(MEMORY_ENUMERATE_CALLBACK enum_func, void* pPrivate){
	HeapLink*	pLink	= HeapLink::Head();

	if(!enum_func || !pLink)	return;

	while(pLink){
		MemoryHeap*	pHeap	= pLink->Item();
		if(pHeap && !pHeap->IsFree()) enum_func(pHeap, pPrivate);
		pLink	= pLink->Next();
	}
	g_MemoryImplementation.Report();
}

//-----------------------------------------------------------
// Memory implementation
//-----------------------------------------------------------
MemoryImplementation	g_MemoryImplementation;
MemoryImplementation::MemoryImplementation(void){}
MemoryImplementation::~MemoryImplementation(void){}

BOOL MemoryImplementation::Initialize(BYTE* pVirtual, unsigned long long int dwPhysical, DWORD dwByteSize, IMemorySubsystem* pMemSubsystem){
	Release();
	__MEM_SUBSYSTEM	= pMemSubsystem;
	new MemoryHeap(pVirtual, dwPhysical, dwByteSize);
	return TRUE;
}

void MemoryImplementation::Release(void){
	// 해제되지 않은 메모리가 있으면 메모리 리포트한다.
	Report();

	// 모든 힙을 제거한다.
	while(HeapLink::Head()) delete HeapLink::Head()->Item();
}

void MemoryImplementation::Report(void){
#ifdef _DEBUG
	if(g_MemoryConsume.peek){
		printf("\n[HAL] Peek memory consume size : ");
		ShowByteSize(g_MemoryConsume.peek);
		printf("\n");
	}
#endif

	if(!HeapLink::Head()) return;

	if(HeapLink::Head()->Next()){
		// 2개 이상의 heap 이 존재하면 해제되지 않은 메모리가 존재하는 것이므로 자세히 출력한다.
		int i_not_free_count	= 0;
		HeapLink*	pLink		= HeapLink::Head();
		printf("\n------------------------------------------------------------------\nSystem heap memory stack status\n"\
			"    ID   IsFree    Address(Phy.)     Pointer       ByteSize\n");
		for(int i=0;pLink;i++){
			MemoryHeap* pHeap = pLink->Item();
			printf(
				"    %-4d %c         0x%08X     0x%08p   %12u (", i, pHeap->IsFree() ? 'O' : 'X', pHeap->Physical(), pHeap, pHeap->ByteSize());
			ShowByteSize(pHeap->ByteSize());
			printf(")\n");
			if(!pHeap->IsFree()) i_not_free_count++;
			pLink = pLink->Next();
		}

		if(i_not_free_count)
			printf(	"*Warning : Non-free heap count : %d\n", i_not_free_count);

#ifdef USE_MEMORY_DEBUG
		{
			printf("Free Link List\n");
			FreeLink* pFree	= FreeLink::Head();
			while(pFree){
				MemoryHeap* pHeap = pFree->Item();
				printf("	0x%p (%12u : ", pHeap, pHeap->ByteSize());
				ShowByteSize(pHeap->ByteSize());
				printf(")\n");
				pFree	= pFree->Next();
			}
		}
#endif

		printf(	"------------------------------------------------------------------\n");
	}
#ifdef WIN32
	_flushall();
#endif
}