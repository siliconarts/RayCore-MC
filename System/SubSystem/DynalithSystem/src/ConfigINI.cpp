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
#include "ConfigINI.h"

ConfigINI	g_ConfigINI;

ConfigINI::ConfigINI(char* sPath){
	TCHAR full_path[1024];
	if(sPath){
		GetFullPathName(CString(sPath), 1024, full_path, NULL);
		m_sPath	= full_path;
	}else{
		GetCurrentDirectory(1024, full_path);
		m_sPath = full_path;
		m_sPath	+= _T("\\dynalith\\config.ini");
	}
}

ConfigINI::~ConfigINI(void){
}

char* ConfigINI::GetString(const char* sApp, const char* sKey, char* default_string){
	static CStringA ms_string;
	TCHAR string_data[1024];
	GetPrivateProfileString(CString(sApp), CString(sKey), CString(default_string), string_data, 1024, m_sPath);
	ms_string = string_data;
	return ms_string.GetBuffer();
}

int ConfigINI::GetInt(const char* sApp, const char* sKey, int default_value){
	return (int)GetPrivateProfileInt(CString(sApp), CString(sKey), default_value, m_sPath);
}
