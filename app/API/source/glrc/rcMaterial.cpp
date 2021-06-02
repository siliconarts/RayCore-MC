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
// DATE    : 8/21/2013 Wed
// Author  : Sang Gon, Kim (sgkim@siliconarts.co.kr)
//
//=======================================================================

#include "rcContext1.h"
#include "rcMaterial.h"
#include "rcTexture.h"

void ContextRC1::Material (RCenum face, RCenum pname, RCfloat param){
	if (face != RC_FRONT_AND_BACK) {
		SetError(RC_INVALID_ENUM);
		return;
	}

	if(pname == RC_SHININESS || pname == RC_SPECULAR_LEVEL){
		if(param < 0) {
			SetError(RC_INVALID_VALUE);
			return;
		}
	}

	switch (pname){
		// roughness
		case RC_REFLECTION:
		case RC_REFRACTION_INDEX:
		case RC_SHININESS:
		case RC_SPECULAR_LEVEL:
		case RC_TRANSMITTANCE:
		case RC_ROUGHNESS:
			// do just one scalar setting filter.
			if(!m_Current.pMaterial->Set(pname, &param)) SetError(RC_INVALID_ENUM);
		break;
		// roughness

		default:
			SetError(RC_INVALID_ENUM);
			break;
	}

	switch(pname){
	case RC_TRANSMITTANCE:
		UdpateTransmitData(m_Current.pMaterial->Name());
		break;
	default:
		break;
	}
}

void ContextRC1::UdpateTransmitData(DWORD name){
	RCfloat transmit[3];
	rcMaterial* pMaterial	= m_Container.Material.Find(name);
	if(!pMaterial) return;
	

	{	// calculate transmittance
		int i;
		RCVECTOR3f* pDiffuse	= pMaterial->GetDiffuse();
		float transmittance		= pMaterial->GetTransmittance();
		// if diffuse color is Grayscale, use original transmittance[0]
		if((pDiffuse->r == pDiffuse->g) && (pDiffuse->r == pDiffuse->b)){
			LOOP(3) transmit[i] = transmittance;
		// if diffuse color is not Grayscale, use recalculated tranmit value;
		}else{
			LOOP(3) transmit[i] = pow(transmittance, 5.f - 4.f * pDiffuse->m[i]);
		}
	}

	{	// set transmittance memory
		RC_TRANSMITTANCE_DATA* pTransmittance = &(((RC_TRANSMITTANCE_DATA*)m_Memory.pTransmit->Virtual())[name]);
		pTransmittance->r	= (WORD)(f32f24(transmit[0])>>4);
		pTransmittance->g	= (WORD)(f32f24(transmit[1])>>4);
		pTransmittance->b	= (WORD)(f32f24(transmit[2])>>4);
		pTransmittance->a	= 0;
		m_Memory.pTransmit->SetDirty(sizeof(RC_TRANSMITTANCE_DATA)*name, sizeof(RC_TRANSMITTANCE_DATA));
	}
}

void ContextRC1::Materialv (RCenum face, RCenum pname, const RCfloat *params){
	if (face != RC_FRONT_AND_BACK){
		SetError(RC_INVALID_ENUM);
		return;
	}

	switch (pname){
	case RC_REFLECTION:
	case RC_REFRACTION_INDEX:
	case RC_SHININESS:
	case RC_SPECULAR_LEVEL:
	case RC_TRANSMITTANCE:
		Material(face, pname, params[0]);
		break;
	default:
		if(!m_Current.pMaterial->Set(pname, params))
			SetError(RC_INVALID_ENUM);
		break;
	}

	switch(pname){
	case RC_AMBIENT_AND_DIFFUSE:
	case RC_DIFFUSE:
		UdpateTransmitData(m_Current.pMaterial->Name());
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
RCboolean ContextRC1::IsMaterial(RCuint material) {
	return m_Container.Material.Find(material) != NULL;
}

void ContextRC1::BindMaterial (RCuint material){
	// it's hardware limitation, (YoY)
	if(material>=RCDDK_MAX_MATERIAL_COUNT){
		SetError(RC_OUT_OF_MEMORY);
		return;
	}
	rcMaterial* pMaterial	= m_Container.Material.Find(material);	// first, find!
	if(!pMaterial){	// if material is not found, we must create one.
		pMaterial = m_Container.Material.Create(material);
		if(pMaterial) pMaterial->SetDDK(m_pDDK);
		else{
			SetError(RC_OUT_OF_MEMORY);
			return;
		}
		pMaterial->CopyFromMaterial(m_Current.pMaterial);
		UdpateTransmitData(pMaterial->Name());
	}
	// set to current material
	if(pMaterial) m_Current.pMaterial	= pMaterial;

	// set current texture from current material
	m_Current.pTexture	= pMaterial->GetTexture();
	if(!m_Current.pTexture)
		m_Current.pTexture	= m_Container.Texture.Find(0);
	
	// activate default diffuse texture....
	pMaterial->ActiveTexture(RCDDK_TEXTURE_TYPE_DIFFUSE);
}

void ContextRC1::GenMaterials (RCsizei n, RCuint *materials){
	rcMaterial*		pMaterial;
	if(n<0) SetError(RC_INVALID_VALUE);
	else
	if(materials)
	for(int i=0;i<n;i++){
		pMaterial		= m_Container.Material.Create();
		if(!pMaterial){
			SetError(RC_OUT_OF_MEMORY);
			break;
		}
		pMaterial->SetDDK(m_pDDK);
		materials[i]	= pMaterial->Name();
		// hardware does not support material binding.
		// so we use only direct material registers. 512ea
		if(materials[i]>=RCDDK_MAX_MATERIAL_COUNT){	// max 512ea, no more...--;
			m_Container.Material.Find(materials[i])->Release();
			SetError(RC_OUT_OF_MEMORY);
			break;
		}
		pMaterial->CopyFromMaterial(m_Current.pMaterial);
		UdpateTransmitData(pMaterial->Name());
	}
}

void ContextRC1::DeleteMaterials (RCsizei n, const RCuint *materials){
	if(n<0) SetError(RC_INVALID_VALUE);
	else
	if(materials)
	for(int i=0;i<n;i++){
		rcMaterial* pMaterial	= m_Container.Material.Find(materials[i]);
		// 오브젝트가 존재하면서 지워지지 않았어야 하며, default object가 아니여야 합니다.
		if(pMaterial && !pMaterial->IsDeleted() && pMaterial->Name()){
			// current 를 지우면 다시 default 로 대체한다.
			if(m_Current.pMaterial == pMaterial)
				m_Current.pMaterial	= m_Container.Material.Find(0);
			pMaterial->Delete();
		}
	}
}

rcMaterial::rcMaterial(void) 
	// roughness
	: m_roughness(0.f) {
	// roughness
	memset(m_Texture, 0, sizeof(MATERIAL_TEXTURE)*2);
	memset(&m_Color, 0, sizeof(m_Color));

	m_Transmittance		= 0.f;
	m_Reflection		= 0.f;
	m_fExponent			= 0;
	m_fSpecularLevel	= 0;
	m_fRefraction		= 1.f;

	m_Color.ambient.r	=
	m_Color.ambient.g	=
	m_Color.ambient.b	= 0.2f;

	m_Color.diffuse.r	=
	m_Color.diffuse.g	=
	m_Color.diffuse.b	= 0.8f;

	m_Color.specular.r	=
	m_Color.specular.g	=
	m_Color.specular.b	= 0.8f;
	
	ActiveTexture(RCDDK_TEXTURE_TYPE_DIFFUSE);

	m_Texture[RCDDK_TEXTURE_TYPE_DIFFUSE].wrap[RCDDK_TEXTURE_WRAP_REPEAT].u	=
	m_Texture[RCDDK_TEXTURE_TYPE_DIFFUSE].wrap[RCDDK_TEXTURE_WRAP_REPEAT].v	= 1;
	m_Texture[RCDDK_TEXTURE_TYPE_DIFFUSE].wrap[RCDDK_TEXTURE_WRAP_MIRROR].u	=
	m_Texture[RCDDK_TEXTURE_TYPE_DIFFUSE].wrap[RCDDK_TEXTURE_WRAP_MIRROR].v	= 0;

	m_Texture[RCDDK_TEXTURE_TYPE_NORMAL].wrap[RCDDK_TEXTURE_WRAP_REPEAT].u	=
	m_Texture[RCDDK_TEXTURE_TYPE_NORMAL].wrap[RCDDK_TEXTURE_WRAP_REPEAT].v	= 1;
	m_Texture[RCDDK_TEXTURE_TYPE_NORMAL].wrap[RCDDK_TEXTURE_WRAP_MIRROR].u	=
	m_Texture[RCDDK_TEXTURE_TYPE_NORMAL].wrap[RCDDK_TEXTURE_WRAP_MIRROR].v	= 0;
}

rcMaterial::~rcMaterial(void){
	ActiveTexture(RCDDK_TEXTURE_TYPE_DIFFUSE);
	SetTexture(NULL);
	ActiveTexture(RCDDK_TEXTURE_TYPE_NORMAL);
	SetTexture(NULL);
}

void rcMaterial::SetDDK(RCDDK* pDDK){
	float refraction	= 0;
	m_pDDK				= pDDK;

	// HW에는 굴절률 역수를 설정해 주어야 한다!
	if(m_fRefraction != 0)	refraction = 1.0f/m_fRefraction;

	m_pDDK->SetMaterialParams(Name(), RCDDK_MATERIAL_AMBIENT, (const float*)&m_Color.ambient);
	m_pDDK->SetMaterialParams(Name(), RCDDK_MATERIAL_DIFFUSE, (const float*)&m_Color.diffuse);
	m_pDDK->SetMaterialParams(Name(), RCDDK_MATERIAL_SPECULAR, (const float*)&m_Color.specular);
	m_pDDK->SetMaterialParams(Name(), RCDDK_MATERIAL_REFLECTION, (const float*)&m_Reflection);
	m_pDDK->SetMaterialParams(Name(), RCDDK_MATERIAL_TRANSMITTANCE, (const float*)&m_Transmittance);
	m_pDDK->SetMaterialParams(Name(), RCDDK_MATERIAL_EXPONENT, (const float*)&m_fExponent);
	m_pDDK->SetMaterialParams(Name(), RCDDK_MATERIAL_SPECULAR_LEVEL, (const float*)&m_fSpecularLevel);
	m_pDDK->SetMaterialParams(Name(), RCDDK_MATERIAL_REFRACTION, (const float*)&refraction);
	m_pDDK->SetMaterialTextureWrap(Name(), RCDDK_TEXTURE_TYPE_DIFFUSE, RCDDK_TEXTURE_WRAP_REPEAT,
		m_Texture[RCDDK_TEXTURE_TYPE_DIFFUSE].wrap[RCDDK_TEXTURE_WRAP_REPEAT].u,
		m_Texture[RCDDK_TEXTURE_TYPE_DIFFUSE].wrap[RCDDK_TEXTURE_WRAP_REPEAT].v);
	m_pDDK->SetMaterialTextureWrap(Name(), RCDDK_TEXTURE_TYPE_DIFFUSE, RCDDK_TEXTURE_WRAP_MIRROR,
		m_Texture[RCDDK_TEXTURE_TYPE_DIFFUSE].wrap[RCDDK_TEXTURE_WRAP_MIRROR].u,
		m_Texture[RCDDK_TEXTURE_TYPE_DIFFUSE].wrap[RCDDK_TEXTURE_WRAP_MIRROR].v);
	m_pDDK->SetMaterialTextureWrap(Name(), RCDDK_TEXTURE_TYPE_NORMAL, RCDDK_TEXTURE_WRAP_REPEAT,
		m_Texture[RCDDK_TEXTURE_TYPE_NORMAL].wrap[RCDDK_TEXTURE_WRAP_REPEAT].u,
		m_Texture[RCDDK_TEXTURE_TYPE_NORMAL].wrap[RCDDK_TEXTURE_WRAP_REPEAT].v);
	m_pDDK->SetMaterialTextureWrap(Name(), RCDDK_TEXTURE_TYPE_NORMAL, RCDDK_TEXTURE_WRAP_MIRROR,
		m_Texture[RCDDK_TEXTURE_TYPE_NORMAL].wrap[RCDDK_TEXTURE_WRAP_MIRROR].u,
		m_Texture[RCDDK_TEXTURE_TYPE_NORMAL].wrap[RCDDK_TEXTURE_WRAP_MIRROR].v);
	InvalidateTexture();
}

void rcMaterial::CopyFromMaterial(rcMaterial* pMaterial){
	if(!pMaterial) return;
	{
		Set(RC_AMBIENT,				(const RCfloat*)&pMaterial->m_Color.ambient);
		Set(RC_DIFFUSE,				(const RCfloat*)&pMaterial->m_Color.diffuse);
		Set(RC_SPECULAR,			(const RCfloat*)&pMaterial->m_Color.specular);
		Set(RC_REFLECTION,			&pMaterial->m_Reflection);
		Set(RC_TRANSMITTANCE,		&pMaterial->m_Transmittance);
		Set(RC_SHININESS,			&pMaterial->m_fExponent);
		Set(RC_SPECULAR_LEVEL,		&pMaterial->m_fSpecularLevel);
		Set(RC_REFRACTION_INDEX,	&pMaterial->m_fRefraction);
		{
			BOOL U,V;
			pMaterial->GetWarp(RCDDK_TEXTURE_TYPE_DIFFUSE, RCDDK_TEXTURE_WRAP_REPEAT, U, V);
			SetWrap(RCDDK_TEXTURE_TYPE_DIFFUSE, RCDDK_TEXTURE_WRAP_REPEAT, U, V);
			pMaterial->GetWarp(RCDDK_TEXTURE_TYPE_DIFFUSE, RCDDK_TEXTURE_WRAP_MIRROR, U, V);
			SetWrap(RCDDK_TEXTURE_TYPE_DIFFUSE, RCDDK_TEXTURE_WRAP_MIRROR, U, V);

			pMaterial->GetWarp(RCDDK_TEXTURE_TYPE_NORMAL, RCDDK_TEXTURE_WRAP_REPEAT, U, V);
			SetWrap(RCDDK_TEXTURE_TYPE_NORMAL, RCDDK_TEXTURE_WRAP_REPEAT, U, V);
			pMaterial->GetWarp(RCDDK_TEXTURE_TYPE_NORMAL, RCDDK_TEXTURE_WRAP_MIRROR, U, V);
			SetWrap(RCDDK_TEXTURE_TYPE_NORMAL, RCDDK_TEXTURE_WRAP_MIRROR, U, V);
		}

		////////////////////////////////////////////////////////////
		//EnableTexture(pMaterial->IsTextureEnable());
		//if(IsTextureEnable())
		//	SetTexture(pMaterial->GetTexture());
		////////////////////////////////////////////////////////////
		RCDDK_TEXTURE_TYPE oldTextureType = pMaterial->GetTextureType();

		pMaterial->ActiveTexture(RCDDK_TEXTURE_TYPE_DIFFUSE);
		ActiveTexture(RCDDK_TEXTURE_TYPE_DIFFUSE);
		EnableTexture(pMaterial->IsTextureEnable());
		if(IsTextureEnable())
			SetTexture(pMaterial->GetTexture());

		pMaterial->ActiveTexture(RCDDK_TEXTURE_TYPE_NORMAL);
		ActiveTexture(RCDDK_TEXTURE_TYPE_NORMAL);
		EnableTexture(pMaterial->IsTextureEnable());
		if(IsTextureEnable())
			SetTexture(pMaterial->GetTexture());

		pMaterial->ActiveTexture(oldTextureType);
		////////////////////////////////////////////////////////////
	}
}

void rcMaterial::EnableTexture(BOOL bEnable){
	if(m_Texture[m_TextureType].bEnable == bEnable) return;
	m_Texture[m_TextureType].bEnable	= bEnable;
	InvalidateTexture();
}

void rcMaterial::ActiveTexture(RCDDK_TEXTURE_TYPE type){
	m_TextureType	= type;
}

void rcMaterial::SetTexture(rcTexture* pTexture){
	MATERIAL_TEXTURE*	pMatTex	= GetActiveTexture();
	if(pTexture == pMatTex->pObject) return;

	if(pMatTex->pObject) pMatTex->pObject->RemoveMaterial(this);
	if(pTexture){
		if(!pTexture->AddMaterial(this))
			pTexture = NULL;
	}
	pMatTex->pObject	= pTexture;

	InvalidateTexture();
}

void rcMaterial::InvalidateTexture(void){
	for(int i=0;i<2;i++){
		MATERIAL_TEXTURE*	pMatTex	= &m_Texture[i];
		rcTexture*			pTex	= pMatTex->pObject;
		if(pMatTex->bEnable && pTex && pTex->IsValid())
			m_pDDK->SetMaterialTexture(Name(), (RCDDK_TEXTURE_TYPE)i, TRUE, pTex->GetBuffer()->Physical(), pTex->LogWidth(), pTex->LogHeight(), pTex->IsMipmap());
		else
			m_pDDK->SetMaterialTexture(Name(), (RCDDK_TEXTURE_TYPE)i, FALSE, 0, 0, 0, FALSE);
	}
}

BOOL rcMaterial::Set(RCenum pname, const RCfloat* params){
	switch(pname){
	case RC_AMBIENT:
		if(SET_CHANGED_VECTOR3(&m_Color.ambient, params))
			m_pDDK->SetMaterialParams(Name(), RCDDK_MATERIAL_AMBIENT, params);
		break;
	case RC_DIFFUSE:
		if(SET_CHANGED_VECTOR3(&m_Color.diffuse, params))
			m_pDDK->SetMaterialParams(Name(), RCDDK_MATERIAL_DIFFUSE, params);
		break;
	case RC_AMBIENT_AND_DIFFUSE:
		if( SET_CHANGED_VECTOR3(&m_Color.ambient, params) ||
			SET_CHANGED_VECTOR3(&m_Color.diffuse, params)){
			m_pDDK->SetMaterialParams(Name(), RCDDK_MATERIAL_AMBIENT, params);
			m_pDDK->SetMaterialParams(Name(), RCDDK_MATERIAL_DIFFUSE, params);
		}
		break;
	case RC_SPECULAR:
		if(SET_CHANGED_VECTOR3(&m_Color.specular, params))
			m_pDDK->SetMaterialParams(Name(), RCDDK_MATERIAL_SPECULAR, params);
		break;
	case RC_REFLECTION:
		if(SET_CHANGED_SCALAR(&m_Reflection, params))
			m_pDDK->SetMaterialParams(Name(), RCDDK_MATERIAL_REFLECTION, params);
		break;
	case RC_TRANSMITTANCE:
		if(SET_CHANGED_SCALAR(&m_Transmittance, params))
			m_pDDK->SetMaterialParams(Name(), RCDDK_MATERIAL_TRANSMITTANCE, params);
		break;
	case RC_SHININESS:
		if(SET_CHANGED_SCALAR(&m_fExponent, params)) {
			//RCfloat shine;
			//shine = pow(2.0, params[0] * 7.99);
			//m_pDDK->SetMaterialParams(Name(), RCDDK_MATERIAL_EXPONENT, &shine);
			m_pDDK->SetMaterialParams(Name(), RCDDK_MATERIAL_EXPONENT, params);
		}
		break;
	case RC_SPECULAR_LEVEL:
		if(SET_CHANGED_SCALAR(&m_fSpecularLevel, params)) {
			m_pDDK->SetMaterialParams(Name(), RCDDK_MATERIAL_SPECULAR_LEVEL, params);
		}
		break;
	case RC_REFRACTION_INDEX:
		{
			// HW에는 굴절률을 역수로 설정해 주어야 한다!
			float refraction	= 0;
			if(params[0] != 0)	refraction = 1.0f/params[0];
			if(SET_CHANGED_SCALAR(&m_fRefraction, params))
				m_pDDK->SetMaterialParams(Name(), RCDDK_MATERIAL_REFRACTION, &refraction);
		}
		break;
	case RC_EMISSION:		// @todo : 추후 지원시 구현
		break;

	// roughness
	case RC_ROUGHNESS:
		if (SET_CHANGED_SCALAR(&m_roughness, params)) {
			m_pDDK->SetMaterialParams(Name(), RCDDK_MATERIAL_ROUGHNESS, params);
		}
	break;
	// roughness

	default:
		return FALSE;
	}
	return TRUE;
}

BOOL rcMaterial::SetTexParam(RCenum target, RCenum pname, RCint param){
	RCDDK_TEXTURE_TYPE	type;
	int					uv_sel;
	BOOL				bMirror, bRepeat;

	switch(target) {
	case RC_TEXTURE_2D:			type	= RCDDK_TEXTURE_TYPE_DIFFUSE;	break;
	case RC_TEXTURE_2D_NORMAL:	type	= RCDDK_TEXTURE_TYPE_NORMAL;	break;
	default:					return FALSE;
	}

	switch(pname){
	case RC_TEXTURE_WRAP_S:		uv_sel	= 0;	break;
	case RC_TEXTURE_WRAP_T:		uv_sel	= 1;	break;
	default:					return FALSE;
	}

	switch(param) {
	case RC_CLAMP_TO_EDGE:
		bMirror	= FALSE;
		bRepeat	= FALSE;
		break;
	case RC_MIRRORED_REPEAT:
		bMirror	= TRUE;
		bRepeat	= TRUE;
		break;
	case RC_REPEAT:
		bMirror	= FALSE;
		bRepeat	= TRUE;
		break;
	default:
		return FALSE;
	}
	{
		MATERIAL_TEXTURE*	pMatTex	= &m_Texture[type];

		if(pMatTex->wrap[RCDDK_TEXTURE_WRAP_MIRROR].m[uv_sel] != bMirror){
			pMatTex->wrap[RCDDK_TEXTURE_WRAP_MIRROR].m[uv_sel]	= bMirror;
			m_pDDK->SetMaterialTextureWrap(Name(), type, RCDDK_TEXTURE_WRAP_MIRROR, pMatTex->wrap[RCDDK_TEXTURE_WRAP_MIRROR].u, pMatTex->wrap[RCDDK_TEXTURE_WRAP_MIRROR].v);
		}
		if(pMatTex->wrap[RCDDK_TEXTURE_WRAP_REPEAT].m[uv_sel] != bRepeat){
			pMatTex->wrap[RCDDK_TEXTURE_WRAP_REPEAT].m[uv_sel]	= bRepeat;
			m_pDDK->SetMaterialTextureWrap(Name(), type, RCDDK_TEXTURE_WRAP_REPEAT, pMatTex->wrap[RCDDK_TEXTURE_WRAP_REPEAT].u, pMatTex->wrap[RCDDK_TEXTURE_WRAP_REPEAT].v);
		}
	}
	return TRUE;
}

RCint rcMaterial::GetTexParam(RCenum target, RCenum pname) {
	RCDDK_TEXTURE_TYPE	type;
	int					uv_sel;
	RCint				param;

	switch(target) {
	case RC_TEXTURE_2D:			type	= RCDDK_TEXTURE_TYPE_DIFFUSE;	break;
	case RC_TEXTURE_2D_NORMAL:	type	= RCDDK_TEXTURE_TYPE_NORMAL;	break;
	default:					return FALSE;
	}

	switch(pname){
	case RC_TEXTURE_WRAP_S:		uv_sel	= 0;	break;
	case RC_TEXTURE_WRAP_T:		uv_sel	= 1;	break;
	default:					return FALSE;
	}

	{
		MATERIAL_TEXTURE*	pMatTex	= &m_Texture[type];
		BOOL				bMirror, bRepeat;

		bMirror	= pMatTex->wrap[RCDDK_TEXTURE_WRAP_MIRROR].m[uv_sel];
		bRepeat	= pMatTex->wrap[RCDDK_TEXTURE_WRAP_REPEAT].m[uv_sel];

		if(!bRepeat)
			param	= RC_CLAMP_TO_EDGE;
		else
			param	= bMirror ? RC_MIRRORED_REPEAT : RC_REPEAT;
	}

	return param;
}

void rcMaterial::SetWrap(RCDDK_TEXTURE_TYPE type, RCDDK_TEXTURE_WRAP mode, BOOL U, BOOL V){
	MATERIAL_TEXTURE* pMatTex	= &m_Texture[type];
	if(	(pMatTex->wrap[mode].u == U) &&
		(pMatTex->wrap[mode].v == V)) return;
	pMatTex->wrap[mode].u	= U;
	pMatTex->wrap[mode].v	= V;
	m_pDDK->SetMaterialTextureWrap(Name(), type, mode, U, V);
}
