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
// DATE    : 11/14/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#pragma once
#include "driver/SystemHAL.h"
#include "driver/SystemConfig.h"

class SystemHALConfig
{
public:
	SystemHALConfig(void);
	~SystemHALConfig(void);

	void Initialize(void);
	void Release(void);
	ISystem* CreateSystem(void);

private:
#ifdef WIN32
	HMODULE			m_hDllSystemImp;
	SYSTEM_TYPE		m_SystemType;
#endif
};
