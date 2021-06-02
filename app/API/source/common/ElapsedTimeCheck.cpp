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
// DATE    : 7/22/2013 Mon
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#include "ElapsedTimeCheck.h"

#ifdef ELAPSED_TIME_CHECK
ElapsedTimeCheck*	ElapsedTimeCheck::m_pHead	= NULL;

ElapsedTimeCheck::ElapsedTimeCheck(EGL_ELAPSED_TIME_MARKER ID){
	m_ID			= ID;
	m_fElapsedTime	= 0.f;
	memset(&m_PrevTime, 0, sizeof(m_PrevTime));
	// set link
	m_pNext			= m_pHead;
	m_pHead			= this;
	// set initial time
	TimeCheck();
}

ElapsedTimeCheck::~ElapsedTimeCheck(void){
	// unlink
	if(m_pHead == this){
		m_pHead	= m_pNext;
	}else{
		ElapsedTimeCheck* pNode = m_pHead;
		while(pNode->m_pNext != this) pNode = pNode->m_pNext;
		pNode->m_pNext	= m_pNext;
	}
}

void ElapsedTimeCheck::TimeCheck(BOOL bCheck){
#ifdef WIN32
	{
		static LONGLONG llQPFTicksPerSec	= 0;

		if(!llQPFTicksPerSec){
			QueryPerformanceFrequency((LARGE_INTEGER*)&llQPFTicksPerSec);
		}
		if(bCheck){
			LONGLONG		CurTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&CurTime);
			m_fElapsedTime	= (float)(CurTime - m_PrevTime) / llQPFTicksPerSec;
			m_PrevTime		= CurTime;
		}else{
			QueryPerformanceCounter((LARGE_INTEGER*)&m_PrevTime);
		}
	}
#else
	{
		struct timeval	cur;

		if(bCheck){
			gettimeofday(&cur, NULL);
			m_fElapsedTime	= (cur.tv_sec - m_PrevTime.tv_sec) + (cur.tv_usec - m_PrevTime.tv_usec)/1000000.f;
			m_PrevTime		= cur;
		}else{
			gettimeofday(&m_PrevTime, NULL);
		}
	}
#endif
}

ElapsedTimeCheck* ElapsedTimeCheck::Find(EGL_ELAPSED_TIME_MARKER ID){
	ElapsedTimeCheck* pNode	= m_pHead;
	while(pNode && (pNode->m_ID != ID)) pNode = pNode->m_pNext;
	return pNode;
}

void ElapsedTimeCheck::Set(EGL_ELAPSED_TIME_MARKER ID){
	ElapsedTimeCheck* pNode	= Find(ID);
	if(!pNode) pNode = new ElapsedTimeCheck(ID);
	pNode->TimeCheck();
}

void ElapsedTimeCheck::End(EGL_ELAPSED_TIME_MARKER ID){
	ElapsedTimeCheck* pNode	= Find(ID);
	if(!pNode) pNode = new ElapsedTimeCheck(ID);
	else pNode->TimeCheck(TRUE);
}

float ElapsedTimeCheck::Get(EGL_ELAPSED_TIME_MARKER ID){
	ElapsedTimeCheck* pNode	= Find(ID);
	if(pNode) return pNode->m_fElapsedTime;
	return 0.f;
}

void ElapsedTimeCheck::Cleanup(void){
	while(m_pHead) delete m_pHead;
}

#endif
