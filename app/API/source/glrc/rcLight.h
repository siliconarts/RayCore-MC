// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore API v 1.0
// Date : 

#ifndef __RC_LIGHT_H__
#define __RC_LIGHT_H__
#include "rcCommon.h"

class rcLight;

class rcLightElement{
public:
	rcLightElement(void);
	virtual ~rcLightElement(void);

	inline BOOL IsDirty(void)		{return m_bDirty;}
	inline void SetDirty(void)		{m_bDirty	= TRUE;}
	inline void ClearDirty(void)	{m_bDirty	= FALSE;}

	inline RC_LIGHT* GetLight(void)	{return &m_LightInfo;}
	inline BOOL IsAttenuation(void)	{return m_LightInfo.atten.enable;}
	inline BOOL IsEnable(void)		{return m_bEnabled;}

	DWORD Update(DWORD index, RCDDK* pDDK);
	BOOL Set(rcLight* pLight, RCenum pname, const RCfloat *params);
	BOOL Get(RCenum pname, RCfloat *params);
	BOOL Enable(BOOL bEnable, rcLightElement*& pHead);
	void UpdateType(void);
	void SetDirtyChain(void);

	void SetNext(rcLightElement* pNext);
	void NormalizeV(float *v);

private:
	BOOL			m_bEnabled;
	RC_LIGHT		m_LightInfo;
	BOOL			m_bDirty;
	rcLightElement	*m_pNext;
};

class rcLight
{
public:
	rcLight(void);
	virtual ~rcLight(void);

	BOOL Set(RCenum light, RCenum pname, const RCfloat *params);
	BOOL Get(RCenum light, RCenum pname, RCfloat *params);
	BOOL Enable(RCenum light, BOOL bEnable);
	BOOL IsEnable(RCenum light, RCboolean *params);
	void Lighting(BOOL bUse);
	inline BOOL IsLighting(void)	{return m_bLighting;}

	void Update(RCDDK* pDDK);

	inline void SetDirty(void)		{m_bDirty	= TRUE;}
	inline void ClearDirty(void)	{m_bDirty	= FALSE;}
	inline BOOL IsDirty(void)		{return m_bDirty;}

private:
	rcLightElement	*m_pHead;
	rcLightElement	m_Element[RCDDK_MAX_LIGHT_COUNT];

	BOOL			m_bLighting;
	BOOL			m_bDirty;
};


#endif//__RC_MATERIAL_H__