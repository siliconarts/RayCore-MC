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
// DATE    : 7/15/2013 Mon
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __RC_MATERIAL_H__
#define __RC_MATERIAL_H__
#include "rcCommon.h"

typedef struct{			// texture setup
	BOOL		bEnable;
	DWORD		phy_address, log_width, log_height;
	union{
		BOOL	m[2];
		struct{
			BOOL u, v;
		};
	}wrap[2];
	rcTexture*	pObject;
}MATERIAL_TEXTURE;

class rcTexture;

class rcMaterial :
	public ObjectT<rcMaterial>
{
public:
	rcMaterial(void);
	virtual ~rcMaterial(void);
	void SetDDK(RCDDK* pDDK);
	void CopyFromMaterial(rcMaterial* pMaterial);

	BOOL Set(RCenum pname, const RCfloat* params);
	BOOL SetTexParam(RCenum target, RCenum pname, RCint param);
	RCint GetTexParam(RCenum target, RCenum pname);
	void SetWrap(RCDDK_TEXTURE_TYPE type, RCDDK_TEXTURE_WRAP mode, BOOL U, BOOL V);
	void EnableTexture(BOOL bEnable = TRUE);
	void ActiveTexture(RCDDK_TEXTURE_TYPE type);
	void SetTexture(rcTexture* pTexture);
	void InvalidateTexture(void);

	inline MATERIAL_TEXTURE* GetActiveTexture(void)		{return &(m_Texture[m_TextureType]);}
	inline RCVECTOR3f* GetAmbient(void)					{return &m_Color.ambient;}
	inline RCVECTOR3f* GetDiffuse(void)					{return &m_Color.diffuse;}
	inline RCVECTOR3f* GetSpecular(void)				{return &m_Color.specular;}
	inline float& GetReflection(void)					{return m_Reflection;}
	inline float& GetTransmittance(void)				{return m_Transmittance;}
	inline float GetExponent(void)						{return m_fExponent;}
	inline float GetSpecularLevel(void)					{return m_fSpecularLevel;}
	inline float GetRefractionIndex(void)				{return m_fRefraction;}
	inline rcTexture* GetTexture(void)					{return GetActiveTexture()->pObject;}
	inline BOOL IsTextureEnable(void)					{return GetActiveTexture()->bEnable;}

	// roughness - implicit inline
	const float GetRoughness() const noexcept			{ return m_roughness; }
	// roughness

	inline void GetWarp(RCDDK_TEXTURE_TYPE type, RCDDK_TEXTURE_WRAP mode, BOOL& U, BOOL& V)	{U = m_Texture[type].wrap[mode].u;V = m_Texture[type].wrap[mode].v;}

	inline RCDDK_TEXTURE_TYPE GetTextureType(void)		{return m_TextureType;}

	inline BOOL IsTextureMapEnable(void)    {return m_Texture[RCDDK_TEXTURE_TYPE_DIFFUSE].bEnable;}
	inline BOOL IsNormalMapEnable(void)    {return m_Texture[RCDDK_TEXTURE_TYPE_NORMAL].bEnable;}

private:
	RCDDK*			m_pDDK;

	struct{			// ambient, diffuse, specular color
		RCVECTOR3f		ambient, diffuse, specular;
	}m_Color;
	float			m_Reflection;		// reflection factor
	float			m_Transmittance;	// transmittance factor
	float			m_fExponent;		// exponent of specular highlight
	float			m_fSpecularLevel;	// specular level
	float			m_fRefraction;		// refraction index

	// roughness
	float			m_roughness;
	// roughness

	MATERIAL_TEXTURE		m_Texture[2];
	RCDDK_TEXTURE_TYPE		m_TextureType;
};

#endif//__RC_MATERIAL_H__
