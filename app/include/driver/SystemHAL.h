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
// Project : RayCore
// DATE    : 11/14/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#ifndef __SYSTEM_HAL_H__
#define __SYSTEM_HAL_H__
#include "STDInterface.h"

//------------------------------------------------------------------------------------------------------------------
// system control interface
//------------------------------------------------------------------------------------------------------------------
typedef void (*INTRRUPT_SERVICE)(void);

interface ISystem{
	// System
	STDMETHOD_(void, Release)(void) PURE;													// system release
	STDMETHOD_(void, Reset)(void) PURE;														// soft reset (not implemented.)
	
	// Memory interface
	STDMETHOD_(unsigned long long, GetMemoryBase)(void) PURE;											// get memory start address
	STDMETHOD_(BOOL, WriteMemory)(DWORD dwAddress, const void* pData, DWORD dwByteSize) PURE;	// direct memory write
	STDMETHOD_(BOOL, ReadMemory)(DWORD dwAddress, void* pData, DWORD dwByteSize) PURE;			// direct memory read

	// Hardware interface
	STDMETHOD_(DWORD, GetDeviceCount)(void) PURE;											// get device count
	STDMETHOD_(BOOL, SetDevice)(DWORD dwCardID) PURE;										// set current device
	STDMETHOD_(DWORD, GetDeviceID)(void) PURE;												// device id
	STDMETHOD_(DWORD, GetClockMHz)(void) PURE;												// get clock speed
	STDMETHOD_(DWORD, Read)(DWORD Offset32) PURE;											// register read
	STDMETHOD_(void, Write)(DWORD Offset32, DWORD Data) PURE;								// register write
	STDMETHOD_(void, Reads)(DWORD Offset32, DWORD* pData, int iCount) PURE;					// multi-register read
	STDMETHOD_(void, Writes)(DWORD Offset32, const DWORD* pData, int iCount) PURE;			// multi-register write
	STDMETHOD_(void, RegisterInterruptService)(INTRRUPT_SERVICE routine) PURE;				// register interrupt service routine
};

#if defined(SYSTEM_IMPLEMENTATION) || defined(SYSTEM_HAL)	// System memory management
	interface IMemoryExtension{	// System memory extended management
		// life cycle
		STDMETHOD_(void, Release)(void) PURE;
		// memory object
		STDMETHOD_(PVOID, Virtual)(void) PURE;
		STDMETHOD_(unsigned long long int, Physical)(void) PURE;
		STDMETHOD_(DWORD, ByteSize)(void) PURE;
		STDMETHOD_(void, SetOffset)(DWORD dwOffset) PURE;
		// memory validation
		STDMETHOD_(void, SetDirty)(DWORD dwOffset, DWORD dwByteSize) PURE;
		STDMETHOD_(void, Flush)(BOOL bCopyToPhysical) PURE;
		STDMETHOD_(void, WaitDone)(void) PURE;
		STDMETHOD_(void, ClearDirty)(void) PURE;
	};

	interface IMemorySubsystem{	// System memory sub system
		STDMETHOD_(IMemoryExtension*, CreateMemory)(unsigned long long int dwPhysical, DWORD dwByteSize, BOOL bForceToDMA = FALSE) PURE;		// system memory creation		
	};

	interface IMemoryHive{		// System memory hive : lifecycle management
		STDMETHOD_(BOOL, Initialize)(BYTE* pVirtual, unsigned long long int dwPhysical, DWORD dwByteSize, IMemorySubsystem* pMemSubsystem) PURE;
		STDMETHOD_(void, Release)(void) PURE;
	};
	
	interface ISystemImp : public ISystem{	// System implementation
		STDMETHOD_(BOOL, Initialize)(IMemoryHive* pMemHive) PURE;			// system intialization
	};
#endif

#ifndef SYSTEM_IMPLEMENTATION	// HAL & DDK interface
	#ifdef WIN32
		#ifdef SYSTEM_HAL
			#define SYSTEM_API __declspec(dllexport)
		#else
			#define SYSTEM_API __declspec(dllimport)		
		#endif
	#else
		#define SYSTEM_API extern
	#endif

	#ifdef SYSTEM_HAL
		#ifdef WIN32
			typedef ISystemImp* (*CreateSystemImplementationFunction)(void);
		#else
			extern ISystemImp* CreateSystemImplementation(void);
		#endif
	#else
		#if defined(_WIN64)
			#pragma comment(lib, "SystemHAL64.lib")
		#elif defined(_WIN32)
			#pragma comment(lib, "SystemHAL.lib")
		#endif
	#endif

	SYSTEM_API ISystem* CreateSystem(void);		// system creation

	interface IMemory{
		STDMETHOD_(void*, Virtual)(void) PURE;									// virtual memory pointer
		STDMETHOD_(unsigned long long int, Physical)(void) PURE;									// physical memory address
		STDMETHOD_(DWORD, ByteSize)(void) PURE;									// aligned allocation size
		STDMETHOD_(void, SetOffset)(DWORD dwOffset) PURE;						// virtual address offset
		STDMETHOD_(void, SetDirty)(DWORD dwOffset, DWORD dwByteSize) PURE;		// set dirty memory range
		STDMETHOD_(void, Flush)(BOOL bCopyToPhysical = TRUE) PURE;				// Flush memory
		STDMETHOD_(void, WaitDone)(void) PURE;									// wait flush memory
		STDMETHOD_(void, ClearDirty)(void) PURE;								// clear dirty
	};

	typedef void (*MEMORY_ENUMERATE_CALLBACK)(IMemory* pMem, void* pPrivate);

	// no thread safe system memory allocation function
	SYSTEM_API	IMemory*	MemoryAlloc(DWORD dwByteSize, DWORD dwByteAlignment = 8, BOOL bForceToDMA = FALSE);	// dwByteAlignment must be 2^n.
	SYSTEM_API	void		MemoryFree(IMemory* pMemory);
	SYSTEM_API	void		MemoryEnumerate(MEMORY_ENUMERATE_CALLBACK enum_func, void* pPrivate = NULL);				// memory enumerate function

#endif

#endif//__SYSTEM_HAL_H__
