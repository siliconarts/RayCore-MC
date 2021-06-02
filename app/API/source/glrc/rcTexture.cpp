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
// DATE    : 8/14/2013 Wed
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#include "rcTexture.h"
#include "rcMaterial.h"
#include "rcContext1.h"
#include <stdio.h>

static int GetLogSize(int iSize){
	int iLog = 0;
	while(iSize){
		if(iSize&1) break;
		iLog++;
		iSize>>=1;
	}
	if(iSize!=1) iLog = -1;
	return iLog;
}

void ContextRC1::BindTexture(RCenum target, RCuint texture) {
	
	if (target == RC_TEXTURE_2D) {
		m_Current.pMaterial->ActiveTexture(RCDDK_TEXTURE_TYPE_DIFFUSE);
	} else
	if (target == RC_TEXTURE_2D_NORMAL) {
		m_Current.pMaterial->ActiveTexture(RCDDK_TEXTURE_TYPE_NORMAL);
	} else {
		SetError(RC_INVALID_ENUM);
		return;
	}

	rcTexture*	pTexture	= m_Container.Texture.Find(texture);	// first, find!
	if(!pTexture){	// if texture is not found, we must create one.
		pTexture	= m_Container.Texture.Create(texture);
		if(!pTexture){
			SetError(RC_OUT_OF_MEMORY);
			return;
		}
	}
	// set to current texture
	if(pTexture) m_Current.pTexture	= pTexture;
		
	// set to current material's texture	
	m_Current.pMaterial->SetTexture(pTexture);
}

void ContextRC1::GenTextures(RCsizei n, RCuint *textures) {
	rcTexture*		pTexture;
	if(n<0) SetError(RC_INVALID_VALUE);
	else
	if(textures)
	for(int i=0;i<n;i++){
		pTexture		= m_Container.Texture.Create();
		if(!pTexture){
			SetError(RC_OUT_OF_MEMORY);
			break;
		}
		textures[i]		= pTexture->Name();
	}
}

void ContextRC1::DeleteTextures(RCsizei n, const RCuint *textures){
	if(n<0) SetError(RC_INVALID_VALUE);
	else
	if(textures)
	for(int i=0;i<n;i++){
		rcTexture* pTexture		= m_Container.Texture.Find(textures[i]);
		// 오브젝트가 존재하면서 지워지지 않았어야 하며, default object가 아니여야 합니다.
		if(pTexture && !pTexture->IsDeleted() && pTexture->Name()){
			// current 를 지우면 다시 default 로 대체한다.
			if(m_Current.pTexture == pTexture)
				m_Current.pTexture	= m_Container.Texture.Find(0);
			pTexture->Delete();
		}
	}
}

RCboolean ContextRC1::IsTexture(RCuint texture) {
	return m_Container.Texture.Find(texture) != NULL;
}

void ContextRC1::TexParameter (RCenum target, RCenum pname, RCint param) {
	if (target != RC_TEXTURE_2D && target != RC_TEXTURE_2D_NORMAL) {
		SetError(RC_INVALID_ENUM);
		return;
	}

	// @todo : RayCore에서 지원시 추후 적용해야 함 (filtering, mipmap gen. on/off)
	switch (pname) {
	case RC_TEXTURE_MIN_FILTER:
		m_TextureMinFilter = param - RC_NEAREST;
		break;
	case RC_TEXTURE_MAG_FILTER:
		m_TextureMagFilter = param - RC_NEAREST;
		break;
	case RC_TEXTURE_WRAP_S:
	case RC_TEXTURE_WRAP_T:
		if(!m_Current.pMaterial->SetTexParam(target, pname, param)) SetError(RC_INVALID_ENUM);
		break;
	case RC_GENERATE_MIPMAP:
		m_bGenerateMipmap = (param != 0);
		break;
	default:
		SetError(RC_INVALID_ENUM);
		break;
	}
}

void ContextRC1::TexParameterv (RCenum target, RCenum pname, const RCint *params)
{
	TexParameter(target, pname, params[0]);
}

void ContextRC1::TexImage2D(RCenum target, RCint level, RCint internalformat, RCsizei width, RCsizei height, RCint border, RCenum format, RCenum type, const RCvoid *pixels) {
	int components;
	DWORD* pData	= NULL;
	rcTexture*	pTexture	= m_Current.pTexture;
	int logWidth, logHeight;

	if ((target != RC_TEXTURE_2D && target != RC_TEXTURE_2D_NORMAL) ||
		(format != RC_RGB && format != RC_RGBA)){
		SetError(RC_INVALID_ENUM);
		return;
	}

	if (internalformat != RC_RGB && internalformat != RC_RGBA){
		SetError(RC_INVALID_VALUE);
		return;
	}

	if(internalformat != format){
		SetError(RC_INVALID_OPERATION);
		return;
	}

	if (pixels == NULL) return;
	if(border || level != 0 || width<=0 || height<=0){	// 현재는 level 0 만 허용함 (@todo : 허용할 수 있도록 변경 되어야 함.)
		SetError(RC_INVALID_VALUE);
		return;
	}

	//rcTexture::Create에서 Check하기는 하지만, RC_INVALID_VALUE 처리를 위해 미리 함!
	//(하드웨어 제한으로 16 이상 1024 이하만 허용)
	logWidth	= GetLogSize(width);
	logHeight	= GetLogSize(height);
	if(	logWidth<4  || logWidth>MAX_TEXTURE_LEVELS ||
		logHeight<4 || logHeight>MAX_TEXTURE_LEVELS){
		SetError(RC_INVALID_VALUE);
		return;
	}

	if(!pTexture->Create(width, height, m_bGenerateMipmap)){
		SetError(RC_OUT_OF_MEMORY);
		return;
	}

	switch (format) {
	case RC_RGB:	components = 3;		break;
	case RC_RGBA:	components = 4;		break;
	default:		return;
	}

	// make user level
	BYTE* pUser	= (BYTE*)pixels;
	pData		= new DWORD[width*height];
	switch (type) {
	case RC_BYTE:
	case RC_UNSIGNED_BYTE: 
		{
			DWORD offset = 0;
			if(format == RC_RGBA){
				for(int y=0;y<height;y++)
				for(int x=0;x<width;x++){
					pData[offset]	= (pUser[0]<<24)|(pUser[1]<<16)|(pUser[2]<<8)|(pUser[3]);
					offset++;
					pUser+=4;
				}
			}else{	// RC_RGB
				for(int y=0;y<height;y++)
				for(int x=0;x<width;x++){
					pData[offset]	= (pUser[0]<<24)|(pUser[1]<<16)|(pUser[2]<<8)|m_TextureAlpha;
					offset++;
					pUser+=3;
				}
			}
		}
		break;
	case RC_SHORT:
	case RC_UNSIGNED_SHORT:
		if (format == RC_RGB) {
			DWORD offset = 0;
			union{
				WORD	data;
				struct{
					WORD	b	: 5;
					WORD	g	: 6;
					WORD	r	: 5;
				};
			}color;
			for(int y=0;y<height;y++)
			for(int x=0;x<width;x++){
				color.data	= *(WORD*)pUser;
				DWORD	r	= (color.r<<3) | (color.r>>2);
				DWORD	g	= (color.g<<2) | (color.g>>4);
				DWORD	b	= (color.b<<3) | (color.b>>2);
				pData[offset]	= (r<<24) | (g<<16) | (b<<8) | m_TextureAlpha;
				offset++;
				pUser+=2;
			}
			break;
		}
	default:
		SetError(RC_INVALID_ENUM);
		pTexture->Destroy();
		SAFE_DELETE_ARRAY(pData);
		return;
	}

	// set levels
	pTexture->CopyLevel(0, pData);
	for(DWORD dwLevel=1;dwLevel<pTexture->Levels();dwLevel++){
		DWORD l_width		= width >> 1;
		DWORD l_height		= height >> 1;
		// scale down
		for(DWORD y=0;y<l_height;y++)
		for(DWORD x=0;x<l_width;x++){
			union{
				DWORD data;
				struct{
					BYTE	r,g,b,a;
				};
			}color;
			DWORD r,g,b,a;
			color.data	= pData[(x<<1) + (y<<1)*width];
			r	= color.r;
			g	= color.g;
			b	= color.b;
			a	= color.a;
			color.data	= pData[(x<<1) + 1 + (y<<1)*width];
			r	+= color.r;
			g	+= color.g;
			b	+= color.b;
			a	+= color.a;
			color.data	= pData[(x<<1) + ((y<<1)+1)*width];
			r	+= color.r;
			g	+= color.g;
			b	+= color.b;
			a	+= color.a;
			color.data	= pData[(x<<1) + 1 + ((y<<1)+1)*width];
			r	+= color.r;
			g	+= color.g;
			b	+= color.b;
			a	+= color.a;

			r	>>= 2;
			g	>>= 2;
			b	>>= 2;
			a	>>= 2;
			pData[x+y*l_width] = (a<<24) | (b<<16) | (g<<8) | r;
		}
		pTexture->CopyLevel(dwLevel, pData);
		width	= l_width;
		height	= l_height;
	}
	SAFE_DELETE_ARRAY(pData);
}

rcTexture::rcTexture(void){
	m_pBuffer		= NULL;
	Destroy();
}

rcTexture::~rcTexture(void){
	Destroy();
}

BOOL rcTexture::Create(DWORD dwWidth, DWORD dwHeight, BOOL bMipmap){
	DWORD	dwByteSize;
	Destroy();
	m_bMipmap		= bMipmap;
	m_Log.width		= GetLogSize(dwWidth);
	m_Log.height	= GetLogSize(dwHeight);
	// 하드웨어 제한으로 16 이상 1024 이하만 허용함.
	if(	m_Log.width<4  || m_Log.width>MAX_TEXTURE_LEVELS ||
		m_Log.height<4 || m_Log.height>MAX_TEXTURE_LEVELS){
		Destroy();
		return FALSE;
	}
	{	// level calculation
		// set level #0
		m_dwWidth			= dwWidth;
		m_dwHeight			= dwHeight;
		dwByteSize			= dwWidth * dwHeight * sizeof(DWORD);
		m_dwLevelDepth		= 1;
		m_dwLevelOffset[0]	= 0;
		// calculate level #N
		if(IsMipmap())
		while(dwWidth > 8 && dwHeight > 8){	// 하드웨어가 16x16 이상의 패턴만 사용한다. @todo : 하드웨어 제약사항이 풀어져야 한다.
			dwWidth				>>= 1;
			dwHeight			>>= 1;
			m_dwLevelOffset[m_dwLevelDepth]	= dwByteSize;
			dwByteSize			+= dwWidth * dwHeight * sizeof(DWORD);
			m_dwLevelDepth++;
		}
	}

	// memory allocation
	m_pBuffer	= new Memory;
	if(!m_pBuffer->Create(dwByteSize, 1<<13)){	// @todo : 하드웨어 제약으로 byte alignment이 2^13 byte alignment 를 유지하고 있다.
		Destroy();
		return FALSE;
	}
	InvalidateMaterials();
	return TRUE;
}

void rcTexture::Destroy(void){
	SAFE_RELEASE(m_pBuffer);
	m_Log.width		=
	m_Log.height	= 0;
	m_dwWidth		=
	m_dwHeight		= 0;
	m_dwLevelDepth	= 0;
	InvalidateMaterials();
}

void rcTexture::CopyLevel(DWORD dwLevel, DWORD* pUser, BOOL bCopyFromUser){
	DWORD dwWidth	= m_dwWidth;
	DWORD dwHeight	= m_dwHeight;
	DWORD* pBuffer	= (DWORD*)m_pBuffer->Virtual();

	if(dwLevel>=Levels() || !m_pBuffer) return;
	{
		DWORD dwOffset	= 0;
		for(;dwLevel;dwLevel--){
			dwOffset	+= dwWidth*dwHeight;
			dwWidth		>>= 1;
			dwHeight	>>= 1;
		}
		pBuffer	+= dwOffset;
		m_pBuffer->SetDirty(dwOffset*sizeof(DWORD), dwWidth*dwHeight*sizeof(DWORD));
	}

	for(DWORD y = 0; y<dwHeight ;y+=8)
	for(DWORD x = 0; x<dwWidth  ;x+=8){
		for(DWORD y1=0; y1<8; y1+=4)
		for(DWORD x1=0; x1<8; x1+=4){
			for(DWORD y2=0; y2<4; y2++)
			for(DWORD x2=0; x2<4; x2++){
				DWORD addr	= (y + y1 + y2) * dwWidth + (x + x1 + x2);
				if(bCopyFromUser){
					*pBuffer	= pUser[addr];	// copy from user

				}else{
					pUser[addr]	= *pBuffer;		// copy to user
				}
				pBuffer++;
			}
		}
	}
	m_pBuffer->Flush();
}

BOOL rcTexture::AddMaterial(rcMaterial* pMaterial){
	if(!pMaterial || !AddRef()) return FALSE;
	m_MaterialList.push_front(pMaterial);
	return TRUE;
}

void rcTexture::RemoveMaterial(rcMaterial* pMaterial){
	if(!pMaterial) return;
	m_MaterialList.remove(pMaterial);	// @todo : 잘못된 사용
	Release();
}

void rcTexture::InvalidateMaterials(void){
	for(list<rcMaterial*>::iterator iter = m_MaterialList.begin();iter != m_MaterialList.end();iter++){
		(*iter)->InvalidateTexture();
	}
}