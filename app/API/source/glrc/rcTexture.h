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
// DATE    : 7/18/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __RC_TEXTURE_H__
#define __RC_TEXTURE_H__
#include "rcCommon.h"
#include <list>

#define MAX_TEXTURE_LEVELS		10
#define MAX_TEXTURE_SIZE		1024

class rcMaterial;

class rcTexture :
	public ObjectT<rcTexture>
{
public:
	rcTexture(void);
	virtual ~rcTexture(void);

	BOOL Create(DWORD dwWidth, DWORD dwHeight, BOOL bMipmap);
	void Destroy(void);
	void CopyLevel(DWORD dwLevel, DWORD* pUser, BOOL bCopyFromUser = TRUE);
	BOOL AddMaterial(rcMaterial* pMaterial);
	void RemoveMaterial(rcMaterial* pMaterial);

	inline Memory* GetBuffer(void)		{return m_pBuffer;}
	
	inline DWORD Width(void)			{return m_dwWidth;}
	inline DWORD Height(void)			{return m_dwHeight;}
	inline DWORD LogWidth(void)			{return m_Log.width;}
	inline DWORD LogHeight(void)		{return m_Log.height;}
	inline DWORD Levels(void)			{return m_dwLevelDepth;}
	inline BOOL IsValid(void)			{return m_pBuffer != NULL;}
	inline BOOL IsMipmap(void)			{return m_bMipmap;}

private:
	void InvalidateMaterials(void);
	Memory*				m_pBuffer;
	DWORD				m_dwWidth, m_dwHeight;
	DWORD				m_dwLevelDepth;
	DWORD				m_dwLevelOffset[MAX_TEXTURE_LEVELS];
	BOOL				m_bMipmap;
	struct{
		DWORD			width, height;
	}m_Log;
	list<rcMaterial*>	m_MaterialList;
};

#endif//__RC_TEXTURE_H__
