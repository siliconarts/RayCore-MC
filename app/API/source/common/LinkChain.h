// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore API v 1.0
// Date : 

#ifndef __LINK_CHAIN_H__
#define __LINK_CHAIN_H__

template <typename T, int idx = 0>
class LinkChain
{
public:
	LinkChain(void){
		m_pPrev		= 
		m_pNext		= NULL;
		m_pObject	= NULL;
	}

	virtual ~LinkChain(void){
		UnLink();
	}

	void SetObject(T* pObject){
		m_pObject	= pObject;
	}

	void Link(LinkChain<T, idx>* pPrevLink = NULL){
		UnLink();
		if(pPrevLink){
			m_pPrev				= pPrevLink;
			m_pNext				= pPrevLink->m_pNext;
			pPrevLink->m_pNext	= this;
			if(m_pNext)
				m_pNext->m_pPrev	= this;
		}else{
			m_pNext			= m_pHead;
			m_pHead			= this;
		}
	}

	void UnLink(void){
		if(m_pHead == this)
			m_pHead				= m_pNext;
		if(m_pPrev)
			m_pPrev->m_pNext	= m_pNext;
		if(m_pNext)
			m_pNext->m_pPrev	= m_pPrev;

		m_pPrev			= NULL;
		m_pNext			= NULL;
	}

	static inline LinkChain<T, idx>* Head(void)		{return m_pHead;}
	inline LinkChain<T, idx>* Prev(void)			{return m_pPrev;}
	inline LinkChain<T, idx>* Next(void)			{return m_pNext;}
	inline T* Object(void)							{return m_pObject;}

private:
	static LinkChain<T, idx>*	m_pHead;
	LinkChain<T, idx>*			m_pPrev;
	LinkChain<T, idx>*			m_pNext;
	T*							m_pObject;
};

template <typename T, int idx>
LinkChain<T, idx>*	LinkChain<T, idx>::m_pHead		= NULL;

#endif//__LINK_CHAIN_H__
