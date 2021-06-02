// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore API v 1.0
// Date : 

#ifndef __PROC_TABLE_H__
#define __PROC_TABLE_H__
#include "gl_Util.h"

namespace GL{
	typedef void (*PROCFUNC)(void);
//	#define PROCFUNC	__eglMustCastToProperFunctionPointerType

	typedef struct{
		const char*			sName;
		PROCFUNC			pFunc;
	}PROCTABLE;

	class ProcTable{
	public:
		ProcTable(const char* sPreFix, const PROCTABLE* pTable);
		~ProcTable(void);

		static PROCFUNC Find(const char* sName);
	
	protected:
		PROCFUNC LocalFind(const char* sName);

	private:
		static ProcTable*	m_pHead;
		ProcTable*			m_pNext;

		const char*			m_sPreFix;
		const PROCTABLE*	m_pTable;
	};

	// table declaration macro
	#define PROC_TABLE_DECLARE(prefix)		static const char* const	__ProcPrefix = #prefix;\
											static const PROCTABLE		__ProcTable[] = {
	#define PROC_TABLE_ITEM(prefix, name)	{#name, (PROCFUNC)prefix##name},
	#define PROC_TABLE_DECLARE_END			{NULL, NULL}};\
											static ProcTable			__ProcTableContainer(__ProcPrefix, __ProcTable);
	#define FIND_PROC_TABLE(name)			ProcTable::Find(name)

	// for convenience
	#define EGL_DECLARE(name)				egl##name
	#define PROC_TABLE_DECLARE_EGL			PROC_TABLE_DECLARE(egl)
	#define PROC_TABLE_ITEM_EGL(name)		PROC_TABLE_ITEM(egl, name)
	#define PROC_TABLE_DECLARE_GL			PROC_TABLE_DECLARE(gl)
	#define PROC_TABLE_ITEM_GL(name)		PROC_TABLE_ITEM(gl, name)
	#define PROC_TABLE_DECLARE_RC			PROC_TABLE_DECLARE(rc)
	#define PROC_TABLE_ITEM_RC(name)		PROC_TABLE_ITEM(rc, name)	
	#define PROC_TABLE_DECLARE_VG			PROC_TABLE_DECLARE(vg)
	#define PROC_TABLE_ITEM_VG(name)		PROC_TABLE_ITEM(vg, name)
	#define PROC_TABLE_DECLARE_CL			PROC_TABLE_DECLARE(cl)
	#define PROC_TABLE_ITEM_CL(name)		PROC_TABLE_ITEM(cl, name)
};

#endif//__PROC_TABLE_H__