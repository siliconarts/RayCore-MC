// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore API v 1.0
// Date : 

#ifndef __ATOMIC_GL_H__
#define __ATOMIC_GL_H__
#include "gl_Util.h"

// atomic GL operation
class AtomicGL
{
public:
	AtomicGL(void);

	BOOL Apply(void);

	inline BOOL IsModified(void)	{return m_bModified;}
	inline void SetModified(void)	{m_bModified = TRUE;}

protected:
	virtual void OnApply(void)	= 0;

private:
	BOOL		m_bModified;
};

#endif//__ATOMIC_GL_H__
