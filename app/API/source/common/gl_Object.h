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
// DATE    : 6/3/2013 Mon
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __GL_OBJECT_H__
#define __GL_OBJECT_H__
#include "gl_Util.h"
#include <assert.h>

namespace GL {
	//-----------------------------------------------------------------------------------------------
	// base Object
	class Object :
		protected Mutex
	{
	public:
		Object(void);

		inline int RefCount(void)		{return m_iRefCount;}
		inline BOOL IsDeleted(void)		{return m_bDeleted;}

		virtual BOOL AddRef(void);
		virtual void Release(void);
		virtual void Delete(void);	// no more reference is not allowed.

	protected:
		virtual ~Object(void);		// for class inheritance

	private:
		int		m_iRefCount;		// reference count
		BOOL	m_bDeleted;			// object is deleted. no more reference is not allowed.
	};

	//-----------------------------------------------------------------------------------------------
	// Name table
	class NameTable{
	public:
		NameTable(void);
		virtual ~NameTable(void);

		DWORD Insert(void* pItem);
		BOOL Insert(DWORD dwName, void* pItem);
		void Remove(DWORD dwIndex);
		inline void* Find(DWORD dwName){
			return (dwName < ((DWORD)m_dwTableSize)) ? m_pTable[dwName] : NULL;
		}
		inline DWORD ItemSize(void)		{return m_dwItemSize;}
		inline DWORD TableSize(void)	{return m_dwTableSize;}

	private:
		void ResizeTable(DWORD dwInsertableName);
		void Release(void);

		void**			m_pTable;
		DWORD			m_dwTableSize;
		DWORD			m_dwLastFree;
		int				m_dwItemSize;
	};

	//-----------------------------------------------------------------------------------------------
	// Object ID for validation check of class object pointer
	template <typename T>
	class ObjectID{
	public:
		ObjectID(void){
			m_pCurObjectPointer	= &m_ClassID;
		}
		BOOL IsValidObject(void){
			return (m_pCurObjectPointer == &m_ClassID);
		}

	private:
		static	const int		m_ClassID;
		const int*				m_pCurObjectPointer;
	};

	template <typename T>
	const int		ObjectID<T>::m_ClassID	= 0xC1A551D;	// CLASSID

	//-----------------------------------------------------------------------------------------------
	// Named object template
	template <typename T>
	class ObjectContainerT;
	
	template <typename T>
	class ObjectT :
		public Object,
		public ObjectID<T>
	{
		friend class ObjectContainerT<T>;
	public:
		inline DWORD Name(void)		{return m_dwName;}

	protected:
		ObjectT(void){
			m_pNameTable	= NULL;
		}

		virtual ~ObjectT(void){
			if(m_pNameTable) m_pNameTable->Remove(m_dwName);
		}

	private:
		void SetNameTable(NameTable* pNameTable, DWORD dwName = (DWORD)-1){
			if(pNameTable){
				if((dwName==(DWORD)-1)){
					m_dwName		= pNameTable->Insert(this);
					m_pNameTable	= pNameTable;
				}else{
					if(pNameTable->Insert(dwName, this)){
						m_dwName		= dwName;
						m_pNameTable	= pNameTable;
					}
				}
			}
		}
		NameTable*		m_pNameTable;
		DWORD			m_dwName;
	};

	//-----------------------------------------------------------------------------------------------
	// ObjectT container
	template <typename T>
	class ObjectContainerT :
		public Object
	{
	public:
		ObjectContainerT(void){
		}
		~ObjectContainerT(void){
			// for absent of user's delete action.
			DeleteAll();
			// in this time, item size must be zero.
			assert(!m_Table.ItemSize());
		}

		void DeleteAll(void){
			for(DWORD i=0;i<m_Table.TableSize();i++){
				T* pObject	= (T*)m_Table.Find(i);
				if(pObject && !pObject->IsDeleted()) pObject->Delete();
			}
		}

		inline T* Create(DWORD dwName = (DWORD)-1){
			T* pObject = new T;
			pObject->SetNameTable(&m_Table, dwName);
			return pObject;
		}

		inline T* Find(DWORD dwName){
			return (T*)m_Table.Find(dwName);
		}

		inline DWORD ObjectSize(void){
			return m_Table.ItemSize();
		}
		inline BOOL IsMine(T* pObject)	{return pObject->m_pNameTable == &m_Table;}

	private:
		NameTable		m_Table;
	};

	//-----------------------------------------------------------------------------------------------
	// Growing memory pool
	template <typename T>
	class GrowingMemoryT{
	public:
#define	GROWING_SHIFTS		13
		GrowingMemoryT(void){
			m_pIndices	= NULL;
			m_dwCount	= 0;
			m_dwBase	= 0;
		}
		virtual ~GrowingMemoryT(void){
			if(m_pIndices){
				for(DWORD i=0;i<m_dwCount;i++) SAFE_DELETE_ARRAY(m_pIndices[i]);
				SAFE_DELETE_ARRAY(m_pIndices);
			}
			m_dwCount	= 0;
		}

		inline void SetBase(DWORD dwBase){
			m_dwBase	= dwBase;
		}

		T* Get(int index){
			DWORD new_count	= (index >> GROWING_SHIFTS);
			if(m_dwCount <= new_count){
				T** pNewIndices	= new T*[new_count+1];
				memset(&pNewIndices[m_dwCount], 0, sizeof(T*)*(new_count+1-m_dwCount));
				if(m_pIndices){
					memcpy(pNewIndices, m_pIndices, m_dwCount * sizeof(T*));
					delete [] m_pIndices;
				}
				m_dwCount	= new_count + 1;
				m_pIndices	= pNewIndices;
			}

			if(!m_pIndices[new_count]) m_pIndices[new_count] = new T[1<<GROWING_SHIFTS];

			return &(m_pIndices[new_count][index&((1<<GROWING_SHIFTS)-1)]);
		}

		inline T* operator [](int index){
			return Get(index + m_dwBase);
		}

	protected:
		T**		m_pIndices;
		DWORD	m_dwCount;
		DWORD	m_dwBase;
	};
	//-----------------------------------------------------------------------------------------------
	// Pool memory
	template <typename T>
	class PoolMemoryT{
	public:
#define	POOL_SIZE		(1024*16)
		PoolMemoryT(void){
			m_pPool		= NULL;
			m_dwSize	= 0;
		}
		virtual ~PoolMemoryT(void){
			SAFE_DELETE_ARRAY(m_pPool);
		}

		T* Get(DWORD index){
			if(index >= m_dwSize){
				DWORD	dwNewSize	= (index + POOL_SIZE) & (~(POOL_SIZE-1));
				T*		pNewPool	= new T[dwNewSize];
				if(m_pPool){
					memcpy(pNewPool, m_pPool, sizeof(T)*m_dwSize);
					delete [] m_pPool;
				}
				m_dwSize	= dwNewSize;
				m_pPool		= pNewPool;
			}

			return &(m_pPool[index]);
		}

	protected:
		T*		m_pPool;
		DWORD	m_dwSize;
	};
}

#endif //__GL_OBJECT_H__
