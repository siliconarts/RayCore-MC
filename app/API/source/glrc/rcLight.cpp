// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore API v 1.0
// Date : 

#include <math.h>
#include "rcContext1.h"
#include "rcLight.h"

void ContextRC1::Light (RCenum light, RCenum pname, RCfloat param){
	switch(pname) {	
	case RC_SPOT_EXPONENT:
		if(param < 0) {
			SetError(RC_INVALID_VALUE);
			return;
		}
		break;
	case RC_SPOT_INNER_CONE:
	case RC_SPOT_OUTER_CONE:
	case RC_SPOT_CUTOFF:
		if(param != 180 && (param < 0 || param > 90)) {
			SetError(RC_INVALID_VALUE);
			return;
		}
		break;
	case RC_START_ATTENUATION:
	case RC_END_ATTENUATION:
		if(param < 0) {
			SetError(RC_INVALID_VALUE);
			return;
		}
		break;
	}

	switch(pname) {	
	case RC_SPOT_EXPONENT:
	case RC_SPOT_INNER_CONE:
	case RC_SPOT_OUTER_CONE:
	case RC_SPOT_CUTOFF:
	case RC_START_ATTENUATION:
	case RC_END_ATTENUATION:
	case RC_CONSTANT_ATTENUATION:
	case RC_LINEAR_ATTENUATION:
	case RC_QUADRATIC_ATTENUATION:
		if(!m_Light.Set(light, pname, &param))
			SetError(RC_INVALID_ENUM);
		break;
	default:
		SetError(RC_INVALID_ENUM);
		break;
	}
}

void ContextRC1::Lightv (RCenum light, RCenum pname, const RCfloat *params){
	// @todo : Position, Direction의 경우 현재의 ModelViewMatrix를 적용해야 한다?
	if(pname == RC_POSITION) {
		float vert[4];
		TransformVertex(vert, params, m_MatrixModelToWorld.m);
		vert[3] = params[3];
		if(!m_Light.Set(light, pname, vert)) {
			SetError(RC_INVALID_ENUM);
			return;
		}
	} else if(pname == RC_SPOT_DIRECTION) {
		float vert[3];
		TransformVertex(vert, params, m_MatrixModelToWorld.m);
		if(!m_Light.Set(light, pname, vert)) {
			SetError(RC_INVALID_ENUM);
			return;
		}
	} else {
		switch(pname) {	
		case RC_SPOT_EXPONENT:
		case RC_SPOT_INNER_CONE:
		case RC_SPOT_OUTER_CONE:
		case RC_SPOT_CUTOFF:
		case RC_START_ATTENUATION:
		case RC_END_ATTENUATION:
		case RC_CONSTANT_ATTENUATION:
		case RC_LINEAR_ATTENUATION:
		case RC_QUADRATIC_ATTENUATION:
			Light(light, pname, params[0]);
			break;
		default:
			if(!m_Light.Set(light, pname, params)) {
				SetError(RC_INVALID_ENUM);
				return;
			}
			break;
		}
	}
}
//----------------------------------------------------------------------------------------------------------------
// light element
rcLightElement::rcLightElement(void) {
	memset(&m_LightInfo, 0, sizeof(RC_LIGHT));
	m_LightInfo.atten.type = RCDDK_LIGHT_TYPE_POINT;
	VECTOR3_SUBSTITUTE_3(m_LightInfo.position.m, 0, 0, 1);
	VECTOR3_SUBSTITUTE(m_LightInfo.ambient.m, 1.0f);
	VECTOR3_SUBSTITUTE(m_LightInfo.diffuse.m, 0.1f);
	m_LightInfo.spot[1] = 180;
	m_LightInfo.attenWeight[0] = 1;
	
	m_bEnabled = FALSE;
	m_pNext		= NULL;
	m_bDirty	= FALSE;
}

rcLightElement::~rcLightElement(void) {
}

BOOL rcLightElement::Enable(BOOL bEnable, rcLightElement*& pHead) {
	if(m_bEnabled == bEnable) return FALSE;

	m_bEnabled	= bEnable;
	if(bEnable){	// insert to link
		if(!pHead)	pHead	= this;
		else		pHead->SetNext(this);
	}else{			// delete link
		if(m_pNext) m_pNext->SetDirtyChain();
		if(pHead == this) pHead	= m_pNext;
		else{
			rcLightElement* pTerminal = pHead;
			while(pTerminal->m_pNext != this) pTerminal = pTerminal->m_pNext;
			pTerminal->m_pNext	= m_pNext;
		}
		m_pNext	= NULL;
	}
	return TRUE;
}

BOOL rcLightElement::Set(rcLight* pLight, RCenum pname, const RCfloat *params){
	BOOL bChange	= FALSE;
	switch(pname) {
	case RC_AMBIENT:				bChange	= SET_CHANGED_VECTOR3(m_LightInfo.ambient.m, params);	break;
	case RC_DIFFUSE:				bChange	= SET_CHANGED_VECTOR3(m_LightInfo.diffuse.m, params);	break;
	case RC_SPECULAR:				bChange	= SET_CHANGED_VECTOR3(m_LightInfo.specular.m, params);	break;
	case RC_POSITION:
		bChange	= SET_CHANGED_VECTOR3(m_LightInfo.position.m, params);
		bChange	|= SET_CHANGED_SCALAR(&m_LightInfo.positionWeight, &params[3]);
		break;
	case RC_SPOT_DIRECTION:			bChange	= SET_CHANGED_VECTOR3(m_LightInfo.direction.m, params);	break;
	case RC_SPOT_EXPONENT:			bChange	= SET_CHANGED_SCALAR(&m_LightInfo.spot[2], params); break;
	case RC_SPOT_INNER_CONE:		bChange	= SET_CHANGED_SCALAR(&m_LightInfo.spot[0], params); break;
	case RC_SPOT_OUTER_CONE:		bChange	= SET_CHANGED_SCALAR(&m_LightInfo.spot[1], params); break;
	case RC_SPOT_CUTOFF:			bChange	= SET_CHANGED_SCALAR(&m_LightInfo.spot[1], params); break;
	case RC_ATTENUATION_RANGE:
		bChange	= SET_CHANGED_SCALAR(&m_LightInfo.attenStart, &params[0]);
		bChange	|= SET_CHANGED_SCALAR(&m_LightInfo.attenEnd, &params[1]);
		break;
	case RC_START_ATTENUATION:		bChange	= SET_CHANGED_SCALAR(&m_LightInfo.attenStart, params); break;
	case RC_END_ATTENUATION:		bChange	= SET_CHANGED_SCALAR(&m_LightInfo.attenEnd, params); break;
	case RC_CONSTANT_ATTENUATION:	bChange	= SET_CHANGED_SCALAR(&m_LightInfo.attenWeight[0], params); break;
	case RC_LINEAR_ATTENUATION:		bChange	= SET_CHANGED_SCALAR(&m_LightInfo.attenWeight[1], params); break;
	case RC_QUADRATIC_ATTENUATION:	bChange	= SET_CHANGED_SCALAR(&m_LightInfo.attenWeight[2], params); break;
	default:	return FALSE;
	}
	if(bChange){
		UpdateType();
		if(IsEnable()){
			SetDirty();
			pLight->SetDirty();
		}
	}
	return TRUE;
}

BOOL rcLightElement::Get(RCenum pname, RCfloat *params){
	switch (pname){
	case RC_AMBIENT:
		params[0] = m_LightInfo.ambient.r;
		params[1] = m_LightInfo.ambient.g;
		params[2] = m_LightInfo.ambient.b;
		params[3] = 1.0;
		break;
	case RC_DIFFUSE:
		params[0] = m_LightInfo.diffuse.r;
		params[1] = m_LightInfo.diffuse.g;
		params[2] = m_LightInfo.diffuse.b;
		params[3] = 0.0;
		break;
	case RC_SPECULAR:
		params[0] = m_LightInfo.specular.r;
		params[1] = m_LightInfo.specular.g;
		params[2] = m_LightInfo.specular.b;
		params[3] = 0.0;
		break;
	case RC_POSITION:
		params[0] = m_LightInfo.position.x;
		params[1] = m_LightInfo.position.y;
		params[2] = m_LightInfo.position.z;
		params[3] = m_LightInfo.positionWeight;
		break;
	case RC_SPOT_DIRECTION:
		params[0] = m_LightInfo.direction.x;
		params[1] = m_LightInfo.direction.y;
		params[2] = m_LightInfo.direction.z;
		break;
	case RC_SPOT_EXPONENT:	
		params[0] = m_LightInfo.spot[2];
		break;
	case RC_SPOT_INNER_CONE:
		params[0] = m_LightInfo.spot[0];
		break;
	case RC_SPOT_OUTER_CONE:
	case RC_SPOT_CUTOFF:
		params[0] = m_LightInfo.spot[1];
		break;
	case RC_ATTENUATION_RANGE:
		params[0] = m_LightInfo.attenStart;
		params[1] = m_LightInfo.attenEnd;
	case RC_START_ATTENUATION:
		params[0] = m_LightInfo.attenStart;
		break;
	case RC_END_ATTENUATION:
		params[0] = m_LightInfo.attenEnd;
		break;
	case RC_CONSTANT_ATTENUATION:
		params[0] = m_LightInfo.attenWeight[0];
		break;
	case RC_LINEAR_ATTENUATION:
		params[0] = m_LightInfo.attenWeight[1];
		break;
	case RC_QUADRATIC_ATTENUATION:
		params[0] = m_LightInfo.attenWeight[2];
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

void rcLightElement::UpdateType(void) {
	//Light Type
	if(m_LightInfo.positionWeight == 0) {
		m_LightInfo.atten.type = RCDDK_LIGHT_TYPE_DIRECTIONAL;
	} else {
		if(m_LightInfo.direction.m[0] != 0
			|| m_LightInfo.direction.m[1] != 0
			|| m_LightInfo.direction.m[2] != 0) {
			m_LightInfo.atten.type = RCDDK_LIGHT_TYPE_SPOT;
		} else {
			m_LightInfo.atten.type = RCDDK_LIGHT_TYPE_POINT;
		}
	}

	//If the spot cutoff(out-cone) is 180, resulting in uniform light distribution.
	if(m_LightInfo.atten.type == RCDDK_LIGHT_TYPE_SPOT && m_LightInfo.spot[1] == 180)
		m_LightInfo.atten.type = RCDDK_LIGHT_TYPE_POINT;
	
	//If the attenuation factors are (1, 0, 0) or (0, 0, 0), resulting in no attenuation.
	if((m_LightInfo.attenWeight[0] == 1 || m_LightInfo.attenWeight[0] == 0)
		&& m_LightInfo.attenWeight[1] == 0
		&& m_LightInfo.attenWeight[2] == 0) {
		m_LightInfo.atten.enable = FALSE;
	} else {
		m_LightInfo.atten.enable = TRUE;
	}
}

void rcLightElement::SetNext(rcLightElement* pNext){
	if(m_pNext) m_pNext->SetNext(pNext);
	else m_pNext	= pNext;
}

void rcLightElement::SetDirtyChain(void){
	SetDirty();
	if(m_pNext) m_pNext->SetDirtyChain();
}

#define M_PI        3.14159265358979323846
#define DEGREE      0.01745329251994329577

DWORD rcLightElement::Update(DWORD index, RCDDK* pDDK){
	float direction[3];
	float spot[3];
	RCdouble theta;
	int i=0;

	VECTOR3_COPY(spot, m_LightInfo.spot);
	LOOP(2) {
		theta = (RCdouble)spot[i] * DEGREE;
		spot[i] = (RCfloat) cos( theta );
	}

	VECTOR3_COPY(direction, m_LightInfo.direction.m);	
	NormalizeV(direction);
	
	pDDK->SetLightParams(index, RCDDK_LIGHT_POSITION,	m_LightInfo.position.m);
	pDDK->SetLightParams(index, RCDDK_LIGHT_DIRECTION,	direction);
	pDDK->SetLightParams(index, RCDDK_LIGHT_SPOT_COS,	spot);
	pDDK->SetLightParams(index, RCDDK_LIGHT_AMBIENT,	m_LightInfo.ambient.m);
	pDDK->SetLightParams(index, RCDDK_LIGHT_DIFFUSE,	m_LightInfo.diffuse.m);
	pDDK->SetLightParams(index, RCDDK_LIGHT_SPECULAR,	m_LightInfo.specular.m);
	pDDK->SetLightAttenuation(index,
		(RCDDK_LIGHT_TYPE)m_LightInfo.atten.type,
		m_LightInfo.attenStart, m_LightInfo.attenEnd,
		IsAttenuation() ? m_LightInfo.attenWeight : NULL);

	ClearDirty();
	// chain update, return light count
	if(m_pNext) return m_pNext->Update(index+1, pDDK) + 1;
	return 1;
}

void rcLightElement::NormalizeV(float *v){
	float size = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	float one_over_length = 0.0f;

	if(size != 0){
		one_over_length = 1.f / size;
		v[0] = v[0] * one_over_length;
		v[1] = v[1] * one_over_length;
		v[2] = v[2] * one_over_length;
	}
}

//----------------------------------------------------------------------------------------------------------------
// light
rcLight::rcLight(void){
	m_pHead		= NULL;
	m_bLighting	= FALSE;
	SetDirty();
}

rcLight::~rcLight(void) {

}

void rcLight::Lighting(BOOL bUse){
	if(bUse == m_bLighting) return;
	m_bLighting	= bUse;
	if(bUse && m_pHead){
		m_pHead->SetDirtyChain();
	}
	SetDirty();
}

BOOL rcLight::Set(RCenum light, RCenum pname, const RCfloat *params) {
	DWORD index = (light - RC_LIGHT0);
	if (index >= RCDDK_MAX_LIGHT_COUNT) return FALSE;
	return m_Element[index].Set(this, pname, params);
}

BOOL rcLight::Get(RCenum light, RCenum pname, RCfloat *params) {
	unsigned int index = (light - RC_LIGHT0);
	if (index >= RCDDK_MAX_LIGHT_COUNT) return FALSE;

	return m_Element[index].Get(pname, params);
}

BOOL rcLight::Enable(RCenum light, BOOL bEnable) {
	unsigned int index = (light - RC_LIGHT0);
	if (index >= RCDDK_MAX_LIGHT_COUNT) return FALSE;
	if(m_Element[index].Enable(bEnable, m_pHead))
		SetDirty();

	return TRUE;
}

BOOL rcLight::IsEnable(RCenum light, RCboolean *params) {
	unsigned int index = (light - RC_LIGHT0);
	if (index >= RCDDK_MAX_LIGHT_COUNT) return FALSE;
	params[0] = m_Element[index].IsEnable();

	return TRUE;
}

void rcLight::Update(RCDDK* pDDK) {
	if(!IsDirty()) return;
	if(m_bLighting && m_pHead) pDDK->SetLightCount(m_pHead->Update(0, pDDK));
	else{// set default light
		// 빛을 적용하지 않을 때 색상이 나올 수 있도록 방향성 없는 빛을 지정한다.
		RC_LIGHT localLight;
		localLight.position.x = 0;
		localLight.position.y = 0;
		localLight.position.z = 1;

		VECTOR3_SUBSTITUTE(localLight.direction.m, 0);
		VECTOR3_SUBSTITUTE(localLight.spot,        0);
		VECTOR3_SUBSTITUTE(localLight.ambient.m,   1.0f);
		VECTOR3_SUBSTITUTE(localLight.diffuse.m,   0.1f);
		VECTOR3_SUBSTITUTE(localLight.specular.m,  0);

		localLight.attenStart	= 0;
		localLight.attenEnd		= 0;
		VECTOR3_SUBSTITUTE_3(localLight.attenWeight, 1, 0, 0);

		pDDK->SetLightParams(0, RCDDK_LIGHT_POSITION,		localLight.position.m);
		pDDK->SetLightParams(0, RCDDK_LIGHT_DIRECTION,		localLight.direction.m);
		pDDK->SetLightParams(0, RCDDK_LIGHT_SPOT_COS,		localLight.spot);
		pDDK->SetLightParams(0, RCDDK_LIGHT_AMBIENT,		localLight.ambient.m);
		pDDK->SetLightParams(0, RCDDK_LIGHT_DIFFUSE,		localLight.diffuse.m);
		pDDK->SetLightParams(0, RCDDK_LIGHT_SPECULAR,		localLight.specular.m);
		pDDK->SetLightAttenuation(0, RCDDK_LIGHT_TYPE_POINT, 
			localLight.attenStart, localLight.attenEnd, NULL);

		pDDK->SetLightCount(1);
	}
	ClearDirty();
}
