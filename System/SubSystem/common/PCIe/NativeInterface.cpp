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
// DATE    : 7/3/2013 Wed
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "NativeInterface.h"

NativeMemory::NativeMemory(void){
	m_pVirtual	= NULL;
}

NativeMemory::~NativeMemory(void){
}

void NativeMemory::Release(void){
	delete this;
}

NativeSystem*	NativeSystem::m_pCurrentSystem	= NULL;

NativeSystem::NativeSystem(void){
	m_pCurrentSystem	= this;
	m_dwCardCount		= 0;
}

NativeSystem::~NativeSystem(void){
	m_pCurrentSystem	= NULL;
}

BOOL NativeSystem::SystemInitialize(void){
	if(!OnSystemInitialize()) return FALSE;

	return TRUE;
}