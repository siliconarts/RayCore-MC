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
// DATE    : 5/31/2013 Fri
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "gl_Object.h"
using namespace GL;

//-----------------------------------------------------------------------------------------------
// base Object
Object::Object(void){
	m_bDeleted		= FALSE;
	m_iRefCount		= 0;
}

Object::~Object(void){
}

BOOL Object::AddRef(void){
	Lock();
	if(m_bDeleted){
		return FALSE;
		UnLock();
	}
	m_iRefCount++;
	UnLock();
	return TRUE;
}

void Object::Release(void){
	int iRef;
	Lock();
	iRef = m_iRefCount--;
	GL_ASSERT(iRef >= 0);
	UnLock();
	if(!iRef) delete this;
}

void Object::Delete(void){	// no more reference is not allowed.
	int iRef = 1;
	Lock();
	if(!m_bDeleted){
		iRef = m_iRefCount--;
		GL_ASSERT(iRef >= 0);
		m_bDeleted	= TRUE;
	}
	UnLock();
	if(!iRef) delete this;
}

//-----------------------------------------------------------------------------------------------
// Name table
#define NAMED_TABLE_SIZE	1024
NameTable::NameTable(void){
	m_pTable		= NULL;
	Release();
}
NameTable::~NameTable(void){
	Release();
}

void NameTable::Release(void){
	SAFE_DELETE_ARRAY(m_pTable);
	m_dwLastFree	= 0;
	m_dwTableSize	= 0;
	m_dwItemSize	= 0;
}

void NameTable::ResizeTable(DWORD dwInsertableName){
	if(dwInsertableName==(DWORD)-1) return;
	if(dwInsertableName >= m_dwTableSize){
		// table is too small, increase table
		DWORD dwNewSize		= dwInsertableName + NAMED_TABLE_SIZE;
		void**	pNewTable	= new void*[dwNewSize];

		//memset(&pNewTable[m_dwTableSize], 0, sizeof(void*) * (dwNewSize - m_dwTableSize));
		memset(pNewTable, 0, sizeof(void*) * dwNewSize);
		if(m_pTable){
			memcpy(pNewTable, m_pTable, sizeof(void*) * m_dwTableSize);
			delete [] m_pTable;
		}
		m_dwTableSize	= dwNewSize;
		m_pTable		= pNewTable;
	}
}

DWORD NameTable::Insert(void* pItem){
	if(pItem){
		m_dwItemSize++;
		ResizeTable(m_dwItemSize);
		{	// set item
			DWORD dwCurID	= m_dwLastFree;
			m_pTable[m_dwLastFree++]	= pItem;
			for(;m_dwLastFree<m_dwTableSize && m_pTable[m_dwLastFree];m_dwLastFree++);
			return dwCurID;
		}
	}
	return (DWORD)-1;
}

BOOL NameTable::Insert(DWORD dwName, void* pItem){
	if(!pItem || Find(dwName) || dwName == (DWORD)-1) return FALSE;

	m_dwItemSize++;
	ResizeTable(dwName);
	{
		// set item
		m_pTable[dwName]	= pItem;
		for(;m_dwLastFree<m_dwTableSize && m_pTable[m_dwLastFree];m_dwLastFree++);
	}

	return TRUE;
}

void NameTable::Remove(DWORD dwIndex){
	if(dwIndex < ((DWORD)m_dwTableSize) && m_pTable[dwIndex]){
		m_dwItemSize--;
		m_pTable[dwIndex]		= NULL;
		if(dwIndex < m_dwLastFree) m_dwLastFree = dwIndex;
		if(!m_dwItemSize) Release();
	}
}
