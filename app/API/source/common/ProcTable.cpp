// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore API v 1.0
// Date : 

#include "ProcTable.h"

using namespace GL;

ProcTable*	ProcTable::m_pHead		= NULL;

ProcTable::ProcTable(const char* sPreFix, const PROCTABLE* pTable){
	// set prefix & table
	m_sPreFix	= sPreFix;
	m_pTable	= pTable;
	// add linked table
	m_pNext		= m_pHead;
	m_pHead		= this;
}

ProcTable::~ProcTable(void){
	// remove linked table
	if(m_pHead == this){
		m_pHead = m_pNext;
	}else{
		ProcTable* pTable	= m_pHead;
		while(pTable->m_pNext){
			if(pTable->m_pNext == this){
				pTable->m_pNext = m_pNext;
				break;
			}
			pTable = pTable->m_pNext;
		}
	}
}

PROCFUNC ProcTable::Find(const char* sName){
	ProcTable*	pTable	= m_pHead;
	PROCFUNC	pFunc;
	while(pTable){
		// check all table
		if((pFunc = pTable->LocalFind(sName))) return pFunc;
		pTable = pTable->m_pNext;
	}
	return NULL;
}

PROCFUNC ProcTable::LocalFind(const char* sName){
	// check prefix
	if(strstr(sName, m_sPreFix) != sName) return NULL;
	sName += strlen(m_sPreFix);		// point to name
	
	// local find all
	const PROCTABLE* pTable = m_pTable;
	while(pTable->sName){
		if(!strcmp(pTable->sName, sName))
			return pTable->pFunc;
		pTable++;
	}
	
	// no proc. is found
	return NULL;
}
