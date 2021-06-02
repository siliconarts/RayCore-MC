// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore API v 1.0
// Date : 

#include <stdio.h>
#include <string.h>
#include "MemDebug.h"

#include <assert.h>

#ifdef _WIN32
	#include <atlstr.h>
#endif

extern "C" {
#if (defined(DEBUG) || defined(_DEBUG)) && defined(WIN32)

enum{
	MEMORY_DEBUG_WARNING,
	MEMORY_DEBUG_ERROR,
	MEMORY_DEBUG_ASSERT,
	MEMORY_DEBUG_SIZE
};

int __cdecl my_crt_report_hook(int reportType, char* message, int *returnValue)
{
	switch(reportType){
	case _CRT_WARN:
		printf("CRT_WARNING : %s\n", message);
		_flushall();
		break;
	case _CRT_ERROR:
		printf("_CRT_ERROR : %s\n", message);
		_flushall();
		break;
	case _CRT_ASSERT:
		printf("_CRT_ASSERT : %s\n", message);
		_flushall();
		break;
	}
	return *returnValue;
}

void MemoryDebug_Initialize(void){
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);	//_CRTDBG_MODE_FILE
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);

	_CrtSetReportHook(my_crt_report_hook);
}

void MemoryDebug_Uninitialize(void){
	CHECK_MEMORY_LEAK
	_CrtDumpMemoryLeaks();
	_CrtSetReportFile(_CRT_WARN,	_CRTDBG_FILE_STDERR);
	_CrtSetReportFile(_CRT_ERROR,	_CRTDBG_FILE_STDERR);
	_CrtSetReportFile(_CRT_ASSERT,	_CRTDBG_FILE_STDERR);
	_CrtSetReportHook(NULL);
}

#endif

};
