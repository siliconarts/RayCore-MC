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
// DATE    : 4/18/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#pragma once
#include <windows.h>
#include <string.h>
#include <atlstr.h> 

class ConfigINI
{
public:
	ConfigINI(char* sPath = NULL);
	~ConfigINI(void);

	char* GetString(const char* sApp, const char* sKey, char* default_string = "");
	int GetInt(const char* sApp, const char* sKey, int default_value = -1);

protected:
	CString		m_sPath;
};

extern ConfigINI	g_ConfigINI;
