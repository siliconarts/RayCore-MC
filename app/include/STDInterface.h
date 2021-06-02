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
// DATE    : 5/29/2013 Wed
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __STD_INTERFACE_H__
#define __STD_INTERFACE_H__
#include <stdio.h>

#ifdef WIN32
	#include <windows.h>
	#include <ObjBase.h>
#else
	#define interface					struct
	#define STDMETHOD_(type,method)		virtual type method
	#define PURE						= 0

	#ifndef PVOID
		typedef void *PVOID;
	#endif
	#if !defined(INT64)
		#define INT64		long long
	#endif
	#if !defined(UINT64)
		#define UINT64		unsigned INT64
	#endif
	#if !defined(LONGLONG)
		#define LONGLONG	long long
	#endif
	#if !defined(LONG)
		#define LONG		long
	#endif
	#if !defined(DWORD)
		#define DWORD		unsigned int
	#endif
	#if !defined(WORD)
		#define WORD		unsigned short
	#endif
	#if !defined(BYTE)
		#define BYTE		unsigned char
	#endif
	#if !defined(BOOL)
		#define BOOL		DWORD
		#define TRUE		1
		#define FALSE		0
	#endif
	#if !defined(NULL)
		#define	NULL		0
	#endif
	
	#if defined(KERNEL_64BIT)
		typedef INT64 KPTR;
	#else
		typedef DWORD KPTR;
	#endif
#endif

// Safe memory deletion
#define SAFE_DELETE(x)			if(x){delete x;			(x) = NULL;}
#define SAFE_DELETE_ARRAY(x)	if(x){delete [] x;		(x) = NULL;}
#define SAFE_RELEASE(x)			if(x){(x)->Release();	(x) = NULL;}

#endif//__STD_INTERFACE_H__
