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

#ifndef __GL_UTIL_H__
#define __GL_UTIL_H__
#include <memory.h>
#if defined(__linux__)
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>        /* for open/close */
#include <fcntl.h>         /* for O_RDWR */
#include <sys/ioctl.h>     /* for ioctl */
#include <sys/types.h>     /* for mmap options */
#include <sys/mman.h>      /* for mmap options */
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>         /* error */
#else
#include <assert.h>
#endif
#include <stdio.h>
#include <math.h>
#include "STDInterface.h"

using namespace std;

#define USE_GL_DEBUG	// for debugging
//#define HW_SIMULATION_DUMP		1		// set to 1 for H/W simulation dump

namespace GL {
	typedef enum{
		RETV_NO_ERROR			= 0,		// GL_NO_ERROR
		RETV_INVALID_ENUM		= 0x0500,	// GL_INVALID_ENUM
		RETV_INVALID_VALUE		= 0x0501,	// GL_INVALID_VALUE
		RETV_INVALID_OPERATION	= 0x0502,	// GL_INVALID_OPERATION
		RETV_STACK_OVERFLOW		= 0x0503,
		RETV_STACK_UNDERFLOW	= 0x0504,
		RETV_OUT_OF_MEMORY		= 0x0505,	// GL_OUT_OF_MEMORY
	}RETV;

	typedef union {	float	m[4];	struct{	float	x, y, z, w;	};	struct{	float	r, g, b, a;	};	}VECTOR4f;
	typedef union {	DWORD	m[4];	struct{	DWORD	x, y, z, w;	};	struct{	DWORD	r, g, b, a;	};	}VECTOR4u;
	typedef union {	int		m[4];	struct{	int		x, y, z, w;	};	struct{	int		r, g, b, a;	};	}VECTOR4i;
	typedef union {	short	m[4];	struct{	short	x, y, z, w;	};	struct{	short	r, g, b, a;	};	}VECTOR4s;
	typedef union {	BYTE	m[4];	struct{	BYTE	x, y, z, w;	};	struct{	BYTE	r, g, b, a;	};	}VECTOR4c;

	typedef union {	float	m[3];	struct{	float	x, y, z;	};	struct{	float	r, g, b;	};	}VECTOR3f;
	typedef union {	DWORD	m[3];	struct{	DWORD	x, y, z;	};	struct{	DWORD	r, g, b;	};	}VECTOR3u;
	typedef union {	int		m[3];	struct{	int		x, y, z;	};	struct{	int		r, g, b;	};	}VECTOR3i;
	typedef union {	short	m[3];	struct{	short	x, y, z;	};	struct{	short	r, g, b;	};	}VECTOR3s;
	typedef union {	BYTE	m[3];	struct{	BYTE	x, y, z;	};	struct{	BYTE	r, g, b;	};	}VECTOR3c;

	typedef union {	float	m[2];	struct{	float	x, y;		};	}VECTOR2f;
	typedef union {	DWORD	m[2];	struct{	DWORD	x, y;		};	}VECTOR2u;
	typedef union {	int		m[2];	struct{	int		x, y;		};	}VECTOR2i;
	typedef union {	short	m[2];	struct{	short	x, y;		};	}VECTOR2s;
	typedef union {	BYTE	m[2];	struct{	BYTE	x, y;		};	}VECTOR2c;


	typedef union {	float	m[16];	struct{	float	m11, m21, m31, m41;		// M's transposed memory structure on OGL
											float	m12, m22, m32, m42;
											float	m13, m23, m33, m43;
											float	m14, m24, m34, m44;};}	MATRIX4f;

	typedef union {	float	m[9];	struct{	float	m11, m21, m31;			// M's transposed memory structure on OGL
											float	m12, m22, m32;
											float	m13, m23, m33;};}		MATRIX3f;

#if defined(USE_GL_DEBUG)
	#if defined(__linux__)
		#define GL_ASSERT(expr)	if (!(expr)) { printf("ASSERT, %s:%d\n", __FUNCTION__, __LINE__); while (1); }
	#else
		#define GL_ASSERT(expr)	if (!(expr)) { printf("ASSERT, %s:%d\n", __FUNCTION__, __LINE__); assert((expr)); }
	#endif
	#define PRINT_MARK	printf("%s : %d line\n", __FUNCTION__, __LINE__);

#ifdef USE_TESTDRIVE
	#define NOT_IMPLEMENTED		{printf("%s(%d) : NOT_IMPLEMENTED!\n", __FUNCTION__, __LINE__);_flushall();assert(0);}
#else
	#define NOT_IMPLEMENTED		{printf("%s(%d) : NOT_IMPLEMENTED!\n", __FUNCTION__, __LINE__); while (1);}
#endif
#else
	#define GL_ASSERT(expr)
	#define PRINT_MARK
	#define NOT_IMPLEMENTED
#endif
	#define DEBUG_MSG(msg, ...)				printf(msg, __VA_ARGS__)
	#define DEBUG_MSG_CON(cond, msg, ...)	if(cond) { \
												printf(msg, __VA_ARGS__); \
											}

	BOOL SetChangedArray(DWORD* pDest, const DWORD* pSrc, int iCount);
	#define SET_CHANGED_ARRAY(v0, v1, n)	SetChangedArray((DWORD*)(v0), (const DWORD*)(v1), n)
	#define SET_CHANGED_VECTOR4(v0, v1)		SetChangedArray((DWORD*)(v0), (const DWORD*)(v1), 4)
	#define SET_CHANGED_VECTOR3(v0, v1)		SetChangedArray((DWORD*)(v0), (const DWORD*)(v1), 3)
	#define SET_CHANGED_VECTOR2(v0, v1)		SetChangedArray((DWORD*)(v0), (const DWORD*)(v1), 2)
	#define SET_CHANGED_SCALAR(v0, v1)		SetChangedArray((DWORD*)(v0), (const DWORD*)(v1), 1)

	//-----------------------------------------------------------
	// Semaphore
	class Semaphore{
#if defined(__linux__)
		static struct sembuf	m_Down;
		static struct sembuf	m_Up;
		int					m_Sema;
#else
		HANDLE				m_Sema;
#endif
	public:
		Semaphore(int iInitValue);
		virtual ~Semaphore(void);
		int Down(void);
		int Up(void);
	};

	//-----------------------------------------------------------
	// Mutex
	class Mutex
#if !defined(__linux__)
		: private Semaphore
#endif
	{
#if defined(__linux__)
		pthread_cond_t		m_condition;
		pthread_mutex_t		m_lock;
#else
		//HANDLE				m_lock;
#endif
	public:
		Mutex(void);
		virtual ~Mutex(void);
#if defined(__linux__)
		int Lock(void);
		int UnLock(void);
#else
		inline int Lock(void)	{return Down();}
		inline int UnLock(void)	{return Up();}
#endif
	};

	//-----------------------------------------------------------
	// Atomic counter
	class AtomicCounter{
		volatile int		m_iCounter;
	public:
		AtomicCounter(int iInitValue);
		virtual ~AtomicCounter(void);
		int increase(void);
		int decrease(void);
	};

	//-----------------------------------------------------------
	// Thread safe fifo
	class ThreadManager{
#if defined(__linux__)
		pthread_t			m_Thread;
#else
		HANDLE				m_Thread;
		DWORD				m_dwThreadID;
#endif
		BOOL				m_bThreadRunning;
	public:
		ThreadManager(void);
		virtual ~ThreadManager(void);
		BOOL RunThread(void);
		void KillThread(void);

		virtual void MonitorThread(void)	= 0;					// 모니터 스레드
		virtual void OnThreadKill(void)		= 0;					// 스레드 킬 전 이벤트
	};

	//-----------------------------------------------------------
	// Thread safe fifo
	class ThreadFIFO{
		int			m_iSize;
		int			m_iIndexIn;
		int			m_iIndexOut;
		Semaphore	m_SyncIn;
		Semaphore	m_SyncOut;
		void**		m_pFIFO;
	public:
		ThreadFIFO(int iSize);
		virtual ~ThreadFIFO(void);
		void Push(void* pData);
		void* Pop(void);
		void WaitForEmpty(void);
	};

	// template override
	template <typename T>
	class ThreadFIFO_t : public ThreadFIFO
	{
	public:
		inline ThreadFIFO_t(int iSize) : ThreadFIFO(iSize){}
		inline void Push(T* pData){ThreadFIFO::Push((void*)pData);}
		inline T* Pop(void){return (T*)ThreadFIFO::Pop();}
	};

	//-----------------------------------------------------------
	// performance check
	float GetElapsedTime(void);
};

#endif //__GL_UTIL_H__
