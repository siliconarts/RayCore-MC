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
// DATE    : 5/24/2013 Fri
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "RegisterChain.h"

_DirtyChainDummy	g_DirtyChainDummy;
void _DirtyChainDummy::OnFlushDirty(void){}

_DirtyChain::_DirtyChain(void){
	m_pNextDirty	= NULL;
}

void _DirtyChain::SetDirty(_DirtyChain** pHead){
	if(!m_pNextDirty){
		m_pNextDirty	= *pHead;
		*pHead			= this;
	}
}

void _DirtyChain::FlushDirty(_DirtyChain** pHead){
	if(*pHead != &g_DirtyChainDummy){
		_DirtyChain* pChain = *pHead;
		_DirtyChain* pNext;
		while(pChain != &g_DirtyChainDummy){
			pChain->OnFlushDirty();
			pNext	= pChain->m_pNextDirty;
			pChain->m_pNextDirty	= NULL;
			pChain = pNext;
		}
		*pHead	= &g_DirtyChainDummy;
	}
}
