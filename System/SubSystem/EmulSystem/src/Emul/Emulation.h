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
// Project : Emulation
// DATE    : 8/28/2013 Wed
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#pragma once
#include "Emulation_Common.h"

class Emulation
{
public:
	Emulation(void);
	virtual ~Emulation(void);

	BOOL DoInit(void);
	BOOL DoWork(void);
	void DoReset(void);

	DWORD RegRead(DWORD Offset32);					// 레지스터 읽기
	void RegWrite(DWORD Offset32, DWORD Data);		// 레지스터 쓰기

	void RegReads(DWORD Offset32, DWORD* pData, int iCount);
	void RegWrites(DWORD Offset32, const DWORD* pData, int iCount);

	void RenderScene(void);

	virtual void UpdateBackFramebuffer(void) = 0;		// 뒤쪽 프레임 버퍼 갱신

	// 테스트 드라이버 인터페이스로 변환 필요
	virtual BYTE* GetPointer(DWORD dwAddress) = 0;		// 시스템 메모리의 virtual 포인터 얻기
	virtual BOOL IsValidAddress(DWORD dwAddress) = 0;	// 유효한 주소인지 확인

protected:
	virtual void WakeUpWorkThread(void) = 0;
	virtual void Lock(void) = 0;
	virtual void UnLock(void) = 0;

private:
	volatile BOOL		m_bReset;
};
