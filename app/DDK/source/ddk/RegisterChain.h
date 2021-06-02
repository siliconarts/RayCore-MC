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
// Project : DDK
// DATE    : 4/15/2013 Mon
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __REGISTER_CHAIN_H__
#define __REGISTER_CHAIN_H__

#include <string.h>

#include "STDInterface.h"
#include "driver/REGMAP.h"

//-----------------------------------------------------------------
// base dirty chain class for register chain template
class _DirtyChain{
public:
	_DirtyChain(void);

	void SetDirty(_DirtyChain** pHead);
	static void FlushDirty(_DirtyChain** pHead);

	inline BOOL IsDirty(void)	{return (m_pNextDirty != NULL);}

protected:
	virtual void OnFlushDirty(void) = 0;

private:
	_DirtyChain*		m_pNextDirty;
};

//-----------------------------------------------------------------
// dirty chain dummy class for initial value
class _DirtyChainDummy : public _DirtyChain{
protected:
	virtual void OnFlushDirty(void);
};
extern _DirtyChainDummy		g_DirtyChainDummy;

// dirty chain defines
#define DIRTY_CHAIN_DECLARE			_DirtyChain*		m_pDirtyChainHead;
#define DIRTY_CHAIN_INITIALIZE		m_pDirtyChainHead	= &g_DirtyChainDummy;
#define DIRTY_CHAIN_FLUSH			_DirtyChain::FlushDirty(&m_pDirtyChainHead);
#define SET_DIRTY(x)				x.SetDirty(&m_pDirtyChainHead)

//-----------------------------------------------------------------
// register chain template

typedef void (*WRITE_REG_FUNC)(DWORD dwOffset32, DWORD* pData, int iCount);

template <unsigned int OFFSET, typename REG_T, WRITE_REG_FUNC reg_write_func>
class RegChainT : public _DirtyChain
{
public:
	RegChainT(void){
		m_dwOffset	= (OFFSET)>>2;
		memset(&m_Reg, 0, sizeof(REG_T));
	}
	inline REG_T* GetReg(void)				{return &m_Reg;}
	inline void AddOffset(DWORD offset)		{m_dwOffset += offset;}
	inline DWORD GetOffset(void)			{return m_dwOffset<<2;}
	inline DWORD GetSize(void)				{return sizeof(REG_T);}

protected:
	virtual void OnFlushDirty(void){
		reg_write_func(m_dwOffset, (DWORD*)&m_Reg, sizeof(REG_T)/sizeof(int));
	}

private:
	DWORD		m_dwOffset;
	REG_T		m_Reg;
};

#endif//__REGISTER_CHAIN_H__
