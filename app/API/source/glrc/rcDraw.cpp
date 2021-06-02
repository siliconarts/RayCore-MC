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
// DATE    : 7/26/2013 Fri
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "rcContext1.h"
#include <malloc.h>
#include <memory.h>

/*
#ifdef WIN32
#include <windows.h>

#define CHECKTIME(Task) \
	LARGE_INTEGER startTime, endTime, freq; \
	\
	QueryPerformanceFrequency(&freq);       \
	QueryPerformanceCounter(&startTime);    \
	Task;                                   \
	QueryPerformanceCounter(&endTime);      \
	printf("[OpenRC] RayCore Render Time ");      \
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED| FOREGROUND_INTENSITY);          \
	printf("[%6f] ", (float)(endTime.QuadPart-startTime.QuadPart)/freq.QuadPart);   \
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);      \
	printf("seconds\n"); \
	_flushall()

#else   // WIN32
#define CHECKTIME(Task)	\
	Task;
#endif  // WIN32
*/

// @todo : Display Resolution 제한	- 최대 2048x2048 (Multiples of 16)

void ContextRC1::Viewport (RCint x, RCint y, RCsizei width, RCsizei height) {

	if(width < 0 || height < 0){
		SetError(RC_INVALID_VALUE);
		return;
	}

	if(width*height > RCDDK_MAX_VIEWPORT_DIMS*RCDDK_MAX_VIEWPORT_DIMS){
		SetError(RC_INVALID_VALUE);
		return;
	}

	m_Screen.X = x;
	m_Screen.Y = y;
	m_Screen.Width	= width;
	m_Screen.Height	= height;
}

#if HW_SIMULATION_DUMP
static struct{
	FILE*	fMap;
	FILE*	fReg;
}__dump;

static void __DumpSystemCallBack(BOOL bMemory, DWORD dwPhyAddress, DWORD dwByteSize, PVOID pData){
	if(bMemory){
		char filename[128];
		{	// 이름 지정
			static int findex = 0;
			sprintf(filename, "mem_%03d.txt", findex);
			printf("dump memory : %s,  addr(0x%X), byte_size(%d)\n", filename, dwPhyAddress, dwByteSize);
			findex++;
			fprintf(__dump.fMap, "%s\t%08X\t%08X\n", filename, dwPhyAddress, dwByteSize);
		}
		{	// 쓰기
			FILE* fp = fopen(filename, "wt");
			for(DWORD i=0;i<dwByteSize/4;i++)
				fprintf(fp, "%08X\n", ((DWORD*)pData)[i]);
			fclose(fp);
		}
	}else{
		printf("dump register : addr(0x%X), byte_size(%d)\n", dwPhyAddress, dwByteSize);
		for(DWORD i=0;i<dwByteSize/4;i++)
			fprintf(__dump.fReg, "sfr_write(32'h%08X,32'h%08X);\n", dwPhyAddress + (i*4), ((DWORD*)pData)[i]);
	}
}
#endif

void ContextRC1::Finish(BOOL bSwapSurface) {
	if(!m_pSurface) return;
	{
		BOOL bDoFinish	= m_bMustDoFinish;
		m_bMustDoFinish	= bSwapSurface;

		if(!bDoFinish) return;
	}

	DynamicSceneEnd();
	
	m_pSurface->Lock();

	// @todo : Multi Static/Dynamic Scene으로의 확장시 추가적인 boxPrimitive 처리가 필요함
	//
	{	// Set Camera
		m_pDDK->SetCameraParams(RCDDK_CAMERA_EYE, m_Screen.RayCamera.eye.m);
		m_pDDK->SetCameraParams(RCDDK_CAMERA_BASIS0, m_Screen.RayCamera.basis[0].m);
		m_pDDK->SetCameraParams(RCDDK_CAMERA_BASIS1, m_Screen.RayCamera.basis[1].m);
		m_pDDK->SetCameraParams(RCDDK_CAMERA_BASIS2, m_Screen.RayCamera.basis[2].m);
		m_pDDK->SetCameraParams(RCDDK_CAMERA_DISTANCE, &m_Screen.RayCamera.distance);
		m_pDDK->SetCameraAttenuation(
			m_Screen.RayCamera.attenFlag ? -m_Screen.RayCamera.attenStart : m_Screen.RayCamera.attenStart,
			m_Screen.RayCamera.attenEnd,
			m_Screen.RayCamera.atten);
	}
	{
		// Set Light
		m_Light.Update(m_pDDK);
	}

	//StaticScene 설정 and DynamicScene 미설정 확인
	if (m_CurrentStateFlag.etc.background_set && m_DynamicRenderData.primitiveCount != 0)
		m_pDDK->SetCRRNInfo(2, 0);
	else
		m_pDDK->SetCRRNInfo(1, 0);
	
	// dynamic 이 없을 경우, static 에 대한 bound box 를 다시 설정한다.
	/*if(CheckFlag(FLAG_BACKGROUND_SET)){
		BOUNDING_BOX    *optBox	= &m_StaticRenderData.boundBox;
		m_pDDK->SetBoundBox(optBox->rootNodeIndex, (float*)optBox->minimum.m);		
	}else{
		BOUNDING_BOX    *optBox	= &m_DynamicRenderData.boundBox;
		m_pDDK->SetBoundBox(optBox->rootNodeIndex, (float*)optBox->minimum.m);
	}*/
	
	m_pDDK->SetPixelSize(m_Screen.info.pixelSizeX, m_Screen.info.pixelSizeY);
	m_pDDK->SetStartPosition(m_Screen.info.startPositionX, m_Screen.info.startPositionY);

	{	// 메모리 갱신하기
		if(m_Memory.pNode->IsResized())
			m_pDDK->SetMemory(RCDDK_MEMORY_NODE, m_Memory.pNode->GetNative());
		if(m_Memory.pList->IsResized())
			m_pDDK->SetMemory(RCDDK_MEMORY_LIST, m_Memory.pList->GetNative());
		if(m_Memory.pPrimitive->IsResized())
			m_pDDK->SetMemory(RCDDK_MEMORY_PRIMITIVE, m_Memory.pPrimitive->GetNative());
		if(m_Memory.pPrimitiveInfo->IsResized())
			m_pDDK->SetMemory(RCDDK_MEMORY_PRIMITIVE_INFO, m_Memory.pPrimitiveInfo->GetNative());

		m_Memory.pTransmit->Flush();
		m_Memory.pPrimitive->Flush();
		m_Memory.pPrimitiveInfo->Flush();
	}

#if HW_SIMULATION_DUMP
	{
		__dump.fMap	= fopen("memory_map.txt", "wt");
		__dump.fReg	= fopen("Register.txt", "wt");
		m_pDDK->DumpSystem(NULL, __DumpSystemCallBack);
		fclose(__dump.fMap);
		fclose(__dump.fReg);
		printf("H/W simulation system dump is done!\n");
		exit(1);
	}
#endif

	ELAPSED_TIME_SET(RENDER);

	// 렌더링 하기
	m_pDDK->RenderStart();
	m_pDDK->RenderEnd(m_pSurface->GetCurrentDrawBuffer()->GetMemory()->GetNative());

	ELAPSED_TIME_END(RENDER);
	ELAPSED_TIME_END(TOTAL);

	m_pSurface->UnLock();

	VECTOR3_SUBSTITUTE(m_BoundBox.maximum.m, FLOAT_MIN);
	VECTOR3_SUBSTITUTE(m_BoundBox.minimum.m, FLOAT_MAX);
}

void ContextRC1::Flush(void) {
    Finish();
}
