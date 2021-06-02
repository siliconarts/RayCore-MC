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

#include "SystemHALConfig.h"
#include "MemoryHeap.h"
#include <stdio.h>
#include <assert.h>

#ifdef USE_TESTDRIVE
	#include "TestDriver.h"
#endif

static const char*	__sSubSystemFileName[SYSTEM_TYPE_SIZE]	= {
#if defined(_WIN64)
	"EmulSystem64.dll",
	"SimSystem64.dll",
	"ArriaVSystem64.dll",
	"DynalithSystem64.dll"
#elif defined(_WIN32)
	"EmulSystem.dll",
	"SimSystem.dll",
	"ArriaVSystem.dll",
	"DynalithSystem.dll"
#endif
};


SystemHALConfig::SystemHALConfig(void){
#ifdef WIN32
	m_hDllSystemImp	= NULL;
#endif
	Initialize();
}

SystemHALConfig::~SystemHALConfig(void){
	Release();
}

void SystemHALConfig::Initialize(void){
#ifdef WIN32
#ifdef USE_TESTDRIVE
	{	// get id
		ITestDriverMemory*	pMemory = TestDriver_GetMemory(SYSTEM_NAME);
		if(pMemory){
			SYSTEM_CONFIG* pConfig	= (SYSTEM_CONFIG*)pMemory->GetConfig();
			m_SystemType	= (SYSTEM_TYPE)pConfig->dwSystemType;
			if(m_SystemType>=SYSTEM_TYPE_SIZE){
				assert("System type is invalid...\n" == 0);
			}
			printf("HAL(Debug) : Current sub-system = %s\n", __sSubSystemFileName[m_SystemType]);
			_flushall();
			pMemory->Release();
		}else{
			m_SystemType	= SYSTEM_TYPE_ALTERA;
		}
	}
#else
	m_SystemType	= SYSTEM_TYPE_ALTERA;
#endif
#endif
}
void SystemHALConfig::Release(void){
#ifdef WIN32
	if(m_hDllSystemImp){
		FreeLibrary(m_hDllSystemImp);
		m_hDllSystemImp	= NULL;
	}
#endif
}

ISystem* SystemHALConfig::CreateSystem(void){
	ISystemImp* pImp	= NULL;

	// 하위 모듈 열기
#ifdef WIN32
	if(!m_hDllSystemImp){
		m_hDllSystemImp = LoadLibrary(__sSubSystemFileName[m_SystemType]);
	}
#endif

#ifdef WIN32
	if(m_hDllSystemImp)
#endif
	{
		// 하위 시스템 생성
#ifdef WIN32
		CreateSystemImplementationFunction	CreateSystemImplementation		= (CreateSystemImplementationFunction)GetProcAddress(m_hDllSystemImp, "CreateSystemImplementation");

		if(CreateSystemImplementation)
#endif
		{
			pImp = CreateSystemImplementation();

			if(pImp){
				// 하위 시스템 초기화
				if(!pImp->Initialize(&g_MemoryImplementation)){
					pImp->Release();
					pImp	= NULL;
					g_MemoryImplementation.Release();
				}
			}
		}
#ifdef USE_TESTDRIVE
		else{
			printf("*E : 지정된 서브 시스템(%s)에서 CreateSystemImplementation 구현을 찾을 수 없습니다.\n", __sSubSystemFileName[m_SystemType]);
		}
#endif
	}
#ifdef USE_TESTDRIVE
	else{
		printf("*E : 존재하지 않는 서브 시스템입니다. - %s\n", __sSubSystemFileName[m_SystemType]);
	}
#endif
	
	return pImp;
}

static SystemHALConfig	__HALConfig;

SYSTEM_API ISystem* CreateSystem(void){
	return __HALConfig.CreateSystem();
}
