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
// DATE    : 6/17/2013 Mon
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "gl_Util.h"

using namespace GL;

#define USE_MULTIMEDIA_TIMER

#ifndef USE_MULTIMEDIA_TIMER
#pragma comment(lib, "Winmm.lib")
#endif

namespace GL{
#ifndef WIN32
	#include <sys/time.h>
#endif

	BOOL SetChangedArray(DWORD* pDest, const DWORD* pSrc, int iCount){
		BOOL bChaged	= FALSE;
		for(int i=0;i<iCount;i++){
			if(pDest[i] != pSrc[i]){
				bChaged		= TRUE;
				pDest[i]	= pSrc[i];
			}
		}
		return bChaged;
	}

	float GetElapsedTime(void){
		static BOOL		bInitialize = FALSE;
		float			fElapsedTime;

#ifdef WIN32
	#ifdef USE_MULTIMEDIA_TIMER
		{
			static LONGLONG	llPrevTime;
			static LONGLONG llQPFTicksPerSec  = 0;
			LONGLONG		CurTime;

			if(!bInitialize){
				QueryPerformanceFrequency((LARGE_INTEGER*)&llQPFTicksPerSec);
				bInitialize	= TRUE;		
			}
			QueryPerformanceCounter((LARGE_INTEGER*)&CurTime);
			fElapsedTime	= (float)(CurTime - llPrevTime) / llQPFTicksPerSec;
			llPrevTime		= CurTime;
		}
	#else
		{
			static DWORD	dwPrevTime;
			DWORD			dwCurTime;
			if(!bInitialize){
				dwPrevTime	= timeGetTime();
				bInitialize	= TRUE;		
			}
			dwCurTime		= timeGetTime();
			fElapsedTime	= (float)(dwCurTime - dwPrevTime) / 1000.f;
			dwPrevTime		= dwCurTime;
		}
	#endif
#else
		{
			static struct timeval	prev;
			struct timeval	cur;
			if(!bInitialize){
				gettimeofday(&prev, NULL);
				bInitialize	= TRUE;
			}
			gettimeofday(&cur, NULL);
			fElapsedTime	= (cur.tv_sec - prev.tv_sec) + (cur.tv_usec - prev.tv_usec)/1000000.f;
			prev			= cur;
		}
#endif
		return fElapsedTime;
	}
};

//-----------------------------------------------------------
// Semaphore
Semaphore::Semaphore(int iInitValue){
#if defined(__linux__)
	m_Sema = semget(IPC_PRIVATE, 1, IPC_CREAT|0666);
	GL_ASSERT(m_Sema != -1);
	semctl(m_Sema, 0, SETVAL, iInitValue);
#else
	m_Sema	= CreateSemaphore(NULL, iInitValue, 1024, NULL);
#endif
}

#if defined(__linux__)
struct sembuf	Semaphore::m_Down	= { 0, -1, SEM_UNDO };
struct sembuf	Semaphore::m_Up		= { 0,  1, SEM_UNDO };
#endif

Semaphore::~Semaphore(void){
#if defined(__linux__)
	int arg = 1;
	semctl(m_Sema, 0, IPC_RMID, arg);
#else
	CloseHandle(m_Sema);
#endif
}

int Semaphore::Down(void){
#if defined(__linux__)
	return semop(m_Sema, &m_Down, 1);
#else
	return WaitForSingleObject(m_Sema, INFINITE);
#endif
}

int Semaphore::Up(void){
#if defined(__linux__)
	return semop(m_Sema, &m_Up, 1);
#else
	return ReleaseSemaphore(m_Sema, 1, NULL);
#endif
}

//-----------------------------------------------------------
// Mutex
Mutex::Mutex(void)
#if !defined(__linux__)
: Semaphore(1)
#endif
{
#if defined(__linux__)
	pthread_mutex_init(&m_lock, NULL);
	pthread_cond_init(&m_condition, NULL);
#else
	//m_lock = CreateMutex(NULL, FALSE, NULL);
#endif
}

Mutex::~Mutex(void){
#if defined(__linux__)
	pthread_mutex_destroy(&m_lock);
	pthread_cond_destroy(&m_condition);
#else
	//CloseHandle(m_lock);
#endif
}
#if defined(__linux__)
int Mutex::Lock(void){
	return pthread_mutex_lock(&m_lock);
}

int Mutex::UnLock(void){
	return pthread_mutex_unlock(&m_lock);
}
#endif

//-----------------------------------------------------------
// Atomic counter
AtomicCounter::AtomicCounter(int iInitValue){
	m_iCounter = iInitValue;
}

AtomicCounter::~AtomicCounter(void){
}

int AtomicCounter::increase(void){
#if defined(__linux__)
	//return __sync_add_and_fetch(&m_iCounter, 1);	// not arm support
	return 0;
#else
	return (int)InterlockedIncrement((volatile LONG*)&m_iCounter);
#endif
}

int AtomicCounter::decrease(void){
#if defined(__linux__)
	//return __sync_sub_and_fetch(&m_iCounter, 1);	// not arm support
	return 0;
#else
	return (int)InterlockedDecrement((volatile LONG*)&m_iCounter);
#endif
}

//-----------------------------------------------------------
// Thread safe fifo
#if defined(__linux__)
static void* thBootStrap(ThreadManager* pManager){
#else
static DWORD WINAPI thBootStrap(ThreadManager* pManager){
#endif
	pManager->MonitorThread();
	return 0;
}

ThreadManager::ThreadManager(void){
#if defined(__linux__)
#else
	m_Thread = 0;
#endif
	m_bThreadRunning	= FALSE;
}

BOOL ThreadManager::RunThread(void){
	if(m_bThreadRunning) return FALSE;
	m_bThreadRunning	= TRUE;
#if defined(__linux__)
	pthread_create(&m_Thread, NULL, ((void* (*)(void *))thBootStrap), (void*)this);
	return TRUE;
#else
	m_Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thBootStrap, this, 0, (LPDWORD)&m_dwThreadID);
	return (m_Thread != NULL);
#endif
}

void ThreadManager::KillThread(void){
	if(!m_bThreadRunning) return;
#if defined(__linux__)
	OnThreadKill();
	pthread_join(m_Thread, NULL);
#else
	if(!m_Thread) return;
	OnThreadKill();

	if(WaitForSingleObject( m_Thread, 10000 ) == WAIT_TIMEOUT){	// 10 초 대기
		//if(MessageBox(NULL, "프로세서가 알 수 없는 이유로\n너무 오랜 시간 동안 대기 상태에 있습니다.\n계속 기다리겠습니까?", "경고", MB_ICONQUESTION|MB_YESNO) == IDYES){
		//WaitForSingleObject( m_Thread, INFINITE );
		//}
	}
	CloseHandle(m_Thread);
	m_Thread	= NULL;
#endif
	m_bThreadRunning	= FALSE;
}

ThreadManager::~ThreadManager(void){
	KillThread();
}

//-----------------------------------------------------------
// Thread safe sync. buffer
ThreadFIFO::ThreadFIFO(int iSize) :
	m_SyncIn(iSize),
	m_SyncOut(0)
{
	GL_ASSERT(iSize>0);
	m_iSize		= iSize;
	m_iIndexIn	= m_iIndexOut	= 0;
	m_pFIFO		= new void*[iSize];
}

ThreadFIFO::~ThreadFIFO(void){
	SAFE_DELETE_ARRAY(m_pFIFO);
}

void ThreadFIFO::Push(void* pData){
	m_SyncIn.Down();
	m_pFIFO[m_iIndexIn] = pData;
	m_SyncOut.Up();
	m_iIndexIn++;
	if(m_iIndexIn==m_iSize) m_iIndexIn = 0;
}

void* ThreadFIFO::Pop(void){
	void* pData;
	m_SyncOut.Down();
	pData = (void*)m_pFIFO[m_iIndexOut];
	m_SyncIn.Up();
	m_iIndexOut++;
	if(m_iIndexOut==m_iSize) m_iIndexOut = 0;
	return pData;
}

void ThreadFIFO::WaitForEmpty(void){
	for(int i=0;i<m_iSize;i++) m_SyncIn.Down();
	for(int i=0;i<m_iSize;i++) m_SyncIn.Up();
}

//#define ThreadFIFO_TEST
#ifdef ThreadFIFO_TEST
pthread_t thread1;
pthread_t thread2;

ThreadFIFO_t<int>	g_ThreadFIFO(4);

void *fun_thread1(void *arg)
{
	for(int i=0;i<100;i++){
		usleep(((DWORD)rand()) & 0xFFFF);
		g_ThreadFIFO.Push((int*)i);
	}
	g_ThreadFIFO.WaitForEmpty();
	printf("Fifo is empty\n");
	return NULL;
}
void *fun_thread2(void *arg)
{
	for(int i=0;i<100;i++){
		int iData = (int)g_ThreadFIFO.Pop();
		usleep(((DWORD)rand()) & 0xFFFF);
		printf("Poped : %d\n", iData);
	}
	return NULL;
}

int main(void){
	pthread_create(&thread2, NULL, fun_thread2, NULL);
	pthread_create(&thread1, NULL, fun_thread1, NULL);
	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);
	return 0;
}
#endif
