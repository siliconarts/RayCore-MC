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
// DATE    : 5/3/2013 Fri
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "eglConfig.h"
#include <stdarg.h>

_EGLConfig::_EGLConfig(_EGLConfig* pNext){
	m_pNext			= pNext;
	m_iItemCount	= 0;
	m_iAttribCount	= 0;
	m_Criterion		= CONFIG_CRITERION_EXACT;
	m_pDesc			= NULL;
}

_EGLConfig::~_EGLConfig(void){
	if(m_pNext) delete m_pNext;

	if(m_pDesc){
		for(int i=0;i<m_iAttribCount;i++) delete [] m_pDesc[i].pTable;
		delete [] m_pDesc;
	}
}

void _EGLConfig::MakeConfigFromUserAttribute(int* pTable, EGLint const * attrib_list){
	if(!pTable || !m_pDesc) return;

	for(int i=0;i<m_iAttribCount;i++){
		int index	= m_pDesc[i].config_index;
		int attrib	= m_pDesc[i].attribute_id;
		int value	= m_pDesc[i].default_value;

		if(attrib_list)
		for(int t=0;attrib_list[t*2]!=EGL_NONE;t++){
			if(attrib_list[t*2] == attrib)
				value	= attrib_list[t*2 + 1];
		}
		pTable[index]	= value;
	}

	if(m_pNext) m_pNext->MakeConfigFromUserAttribute(pTable, attrib_list);
}

void _EGLConfig::SetAttribute(int AttribCount, int ItemCount, CONFIG_CRITERION criterion, ...){
	m_iAttribCount	= AttribCount;
	m_iItemCount	= ItemCount;
	m_Criterion		= criterion;
	{
		m_pDesc = new CONFIG_TABLE_DESC[AttribCount];
		for(int i=0;i<AttribCount;i++){
			m_pDesc[i].pTable = new int[ItemCount];
		}
	}
	{
		va_list ap;

		va_start(ap, criterion);
		for(int i=0;i<AttribCount;i++){
			m_pDesc[i].config_index		= (CONFIG_INDEX) va_arg(ap, int);
			m_pDesc[i].attribute_id		= (EGLenum) va_arg(ap, int);
			m_pDesc[i].default_value	= (int)va_arg(ap, int);
		}
		va_end(ap);
	}
}

void _EGLConfig::SetItem(int index, ...){
	va_list ap;

	va_start(ap, index);
	for(int i=0;i<m_iAttribCount;i++){
		m_pDesc[i].pTable[index] = va_arg(ap, int);
	}
	va_end(ap);
}

int _EGLConfig::ConfigSize(void){
	return m_pNext ? m_pNext->ConfigSize() * m_iItemCount : m_iItemCount;
}

BOOL _EGLConfig::CheckConfig(int item, int *pTable){
	BOOL bCheck = TRUE;
	for(int t=0;t<m_iAttribCount && bCheck;t++){
		switch(m_Criterion){
		case CONFIG_CRITERION_EXACT:
			if(pTable[m_pDesc[t].config_index] != m_pDesc[t].pTable[item]) bCheck = FALSE;
			break;
		case CONFIG_CRITERION_ATLEAST:
			if(pTable[m_pDesc[t].config_index] > m_pDesc[t].pTable[item]) bCheck = FALSE;
			break;
		case CONFIG_CRITERION_MASK:
			if( !pTable[m_pDesc[t].config_index] ||
				(pTable[m_pDesc[t].config_index] & (~m_pDesc[t].pTable[item])) ) bCheck = FALSE;
			break;
		}
	}
	return bCheck;
}


void _EGLConfig::FindConfig(int *pTable, EGLConfig *pConfig, int ConfigSize, int& config_index, int id, int step){
	for(int i=0;i<m_iItemCount;i++){
		if(config_index==ConfigSize) return;
		if(CheckConfig(i, pTable)){
			if(m_pNext){
				m_pNext->FindConfig(pTable, pConfig, ConfigSize, config_index, id, step * m_iItemCount);

			}else{
				if(pConfig) pConfig[config_index] = (EGLConfig)(id + 1);
				config_index++;
			}
		}
		id += step;
	}
}

BOOL _EGLConfig::MakeTable(int id, int *pTable){
	int index = id % m_iItemCount;
	id = id / m_iItemCount;

	for(int i=0;i<m_iAttribCount;i++)
		pTable[m_pDesc[i].config_index] = m_pDesc[i].pTable[index];
	
	if(m_pNext) m_pNext->MakeTable(id, pTable);
	else if(id) return FALSE;

	return TRUE;
}

CONFIG_INDEX _EGLConfig::GetIndexFromAttributeID(EGLint attribute_id){
	for(int i=0;i<m_iAttribCount;i++){
		if(m_pDesc[i].attribute_id == attribute_id)
			return m_pDesc[i].config_index;
	}
	if(m_pNext) return m_pNext->GetIndexFromAttributeID(attribute_id);
	return (CONFIG_INDEX)-1;
}
