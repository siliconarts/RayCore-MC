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
// Project : API
// DATE    : 5/29/2013 Wed
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __GL_MEMORY_H__
#define __GL_MEMORY_H__
#include "gl_Object.h"
#include "ddk/rcDDK.h"

// error handler
namespace GL {
	//-----------------------------------------------------------------------------------------------
	// Memory
	//-----------------------------------------------------------------------------------------------
	class Memory :
		public ObjectT<Memory>
	{
	public:
		Memory(void);
		virtual ~Memory(void);

		virtual BOOL Create(DWORD dwByteSize, DWORD dwByteAlignment = (1<<8), BOOL bForceToDMA = FALSE);
		void Destroy(void);

		inline DWORD ByteSize(void)			{return m_dwByteSize;}
		inline void* Virtual(void)			{return m_pVirtual;}
		inline unsigned long long int Physical(void)			{return m_dwPhysical;}

		inline RCDDKMemory* GetNative(void)	{return m_pMem;}
		inline void SetDirty(DWORD dwOffset, DWORD dwByteSize) {if(m_pMem) m_pMem->SetDirty(dwOffset, dwByteSize);}
		inline void Flush(BOOL bCopyToPhysical = TRUE) {if(m_pMem) m_pMem->Flush(bCopyToPhysical);}

	protected:
		void*				m_pVirtual;
		unsigned long long int				m_dwPhysical;
		DWORD				m_dwByteSize;
		RCDDKMemory*		m_pMem;
	};

	//-----------------------------------------------------------------------------------------------
	// Memory pool
	//-----------------------------------------------------------------------------------------------
	class MemoryPool : public Memory
	{
	public:
		MemoryPool(void);
		virtual ~MemoryPool(void);

		virtual BOOL Create(DWORD dwByteSize, DWORD dwByteAlignment = (1<<8), BOOL bForceToDMA = FALSE);
		BOOL Resize(DWORD dwByteSize);
		inline BOOL IsResized(void)	{
			if(m_bResized){
				m_bResized	= FALSE;
				return TRUE;
			}
			return FALSE;
		}

	protected:
		DWORD				m_dwByteAlignment;
		BOOL				m_bResized;
	};

	//-----------------------------------------------------------------------------------------------
	// Memory pool template
	//-----------------------------------------------------------------------------------------------
	template<typename T>
	class MemoryPoolT : public MemoryPool
	{
	public:
		MemoryPoolT(void){}
		virtual ~MemoryPoolT(void){}

		T* GetPool(DWORD dwIndex){
			{	// check grow
				DWORD nBytePos	= dwIndex * sizeof(T);
				if((nBytePos + (sizeof(T) * 4096)) >= ByteSize())
					Resize(ByteSize() + (sizeof(T) * 32768));	// grow
				SetDirty(nBytePos, sizeof(T));
			}
			return &(((T*)(Virtual()))[dwIndex]);
		}
	};
};

#endif //__GL_MEMORY_H__
