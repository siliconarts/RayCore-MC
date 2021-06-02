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
// DATE    : 7/23/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#ifndef __ELAPSED_TIME_CHECK_H__
#define __ELAPSED_TIME_CHECK_H__
#include "gl_Object.h"
#include "debug/egl_ElapsedTimeMarker.h"

#if defined(DEBUG) || defined(_DEBUG) || EGL_ELAPSED_TIME_CHECK_FORCED
	#define ELAPSED_TIME_CHECK		// API 내에서 오류를 검출하려면 이 정의를 사용한다.
#endif

#ifdef ELAPSED_TIME_CHECK
	class ElapsedTimeCheck
	{
	public:
		ElapsedTimeCheck(EGL_ELAPSED_TIME_MARKER ID);
		virtual ~ElapsedTimeCheck(void);

		static void Set(EGL_ELAPSED_TIME_MARKER ID);
		static void End(EGL_ELAPSED_TIME_MARKER ID);
		static float Get(EGL_ELAPSED_TIME_MARKER ID);
		static void Cleanup(void);

	protected:
		static ElapsedTimeCheck* Find(EGL_ELAPSED_TIME_MARKER ID);
		void TimeCheck(BOOL bCheck = FALSE);

	private:
		float						m_fElapsedTime;
		static	ElapsedTimeCheck*	m_pHead;
		ElapsedTimeCheck*			m_pNext;
		EGL_ELAPSED_TIME_MARKER		m_ID;

#ifdef WIN32
		LONGLONG					m_PrevTime;
#else
		struct timeval				m_PrevTime;
#endif
	};
	#define ELAPSED_TIME_SET(id)	ElapsedTimeCheck::Set(EGL_ELAPSED_TIME_MARKER_##id)	// set start
	#define ELAPSED_TIME_END(id)	ElapsedTimeCheck::End(EGL_ELAPSED_TIME_MARKER_##id)	// set end
	#define ELAPSED_TIME_GET(id)	ElapsedTimeCheck::Get(id)	// get elapsed time.
	#define ELAPSED_TIME_CLEANUP()	ElapsedTimeCheck::Cleanup()
#else
	#define ELAPSED_TIME_SET(id)
	#define ELAPSED_TIME_END(id)
	#define ELAPSED_TIME_GET(id)	(0.f)
	#define ELAPSED_TIME_CLEANUP()
#endif

#endif //__ELAPSED_TIME_CHECK_H__
