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
// Project : HAL
// DATE    : 7/15/2013 Mon
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#pragma once

template <typename T, int idx>
class CircularLink
{
public:
	CircularLink(void){
		pPrev	= NULL;
		pNext	= NULL;
	}

	virtual ~CircularLink(void){
		UnLink();
	}

	void SetObject(T object){
		Object	= object;
	}

	void Link(CircularLink<T, idx>* pPrevLink = NULL){
		UnLink();
		if(pPrevLink){
			pPrev				= pPrevLink;
			pNext				= pPrevLink->pNext;
			pPrevLink->pNext	= this;
			if(pNext)
				pNext->pPrev	= this;
		}else{
			if(pHead) pHead->pPrev	= this;
			pNext			= pHead;
			pHead			= this;
		}
	}

	void UnLink(void){
		if(pHead == this)
			pHead			= pNext;
		if(pPrev)
			pPrev->pNext	= pNext;
		if(pNext)
			pNext->pPrev	= pPrev;
		
		pPrev			= NULL;
		pNext			= NULL;
	}

	static inline CircularLink<T, idx>* Head(void)	{return pHead;}
	inline CircularLink<T, idx>* Prev(void)	{return pPrev;}
	inline CircularLink<T, idx>* Next(void)	{return pNext;}
	inline T Item(void)						{return Object;}

private:
	static CircularLink<T, idx>*	pHead;
	CircularLink<T, idx>*			pPrev;
	CircularLink<T, idx>*			pNext;
	T								Object;
};

template <typename T, int idx>
CircularLink<T, idx>*	CircularLink<T, idx>::pHead		= NULL;