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
// DATE    : 4/23/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#pragma once
#include <Windows.h>
#include "iprove.h"

class iProveInterface
{
public:
	iProveInterface(void);
	~iProveInterface(void);

	BOOL IsInitialized(void);
	BOOL Initialize(DWORD dwCardID = 0);
	void Release(void);

	void WriteMemory(DWORD dwAddress, DWORD* pData, DWORD dwSize);
	void ReadMemory(DWORD dwAddress, DWORD *pData, DWORD dwSize);

	void WriteRegister(DWORD Offset32, const DWORD* pData, int iCount);
	void ReadRegister(DWORD Offset32, DWORD* pData, int iCount);
	void WriteRegister(DWORD Offset32, DWORD dwData);
	DWORD ReadRegister(DWORD Offset32);


protected:
	BOOL				m_bInitialized;
	DWORD				m_dwCardID;
	iPROVE_Module		m_iProveHandle;
	LONGLONG			m_lElpasedTime;

	BOOL DDRInitialize(void);
};

extern iProveInterface		g_iProve;
