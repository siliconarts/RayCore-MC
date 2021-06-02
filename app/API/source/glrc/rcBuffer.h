// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore API v 1.0
// Date : 

#ifndef __RC_BUFFER_H__
#define __RC_BUFFER_H__
#include "rcCommon.h"

typedef struct BUFFER_OBJECT{
	unsigned char 	flag;
	WORD		 	target;
	DWORD		 	usage;
	DWORD		    size;
	void*  			pValue;	
}BUFFER_OBJECT;

class rcBuffer :
	public ObjectT<rcBuffer>
{
public:
	rcBuffer(void);
	virtual ~rcBuffer(void);
	inline BUFFER_OBJECT* Get(void)	{return &m_Status;}	// temporary...
	BOOL Set(RCenum target);
	BOOL BufferData (RCenum target, RCsizeiptr size, const RCvoid *data, RCenum usage);	
	BOOL BufferSubData (RCenum target, RCintptr offset, RCsizeiptr size, const RCvoid *data);
	
	inline BOOL IsDirty(void)		{return m_bChanged;}
	inline void SetDirty(void)		{m_bChanged	= TRUE;}
	inline void ClearDirty(void)	{m_bChanged	= FALSE;}
private:
	BUFFER_OBJECT	m_Status;	
	BOOL			m_bChanged;
};

#endif//__RC_BUFFER_H__
