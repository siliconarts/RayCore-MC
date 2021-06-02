// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore API v 1.0
// Date : 

#include "AtomicGL.h"

AtomicGL::AtomicGL(void){
	m_bModified	= TRUE;
}

BOOL AtomicGL::Apply(void){
	if(!m_bModified) return FALSE;
	OnApply();
	m_bModified	= FALSE;
	return TRUE;
}