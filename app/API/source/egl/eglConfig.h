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
// DATE    : 5/3/2013 Fri
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef _EGL_CONFIG_
#define _EGL_CONFIG_

#include "eglCommon.h"
#include <assert.h>

namespace GL{
	typedef enum{
		CONFIG_INDEX_RED_SIZE,
		CONFIG_INDEX_GREEN_SIZE,
		CONFIG_INDEX_BLUE_SIZE,
		CONFIG_INDEX_DEPTH_SIZE,
		CONFIG_INDEX_SURFACE_TYPE,
		CONFIG_INDEX_SIZE
	}CONFIG_INDEX;

	typedef enum{
		CONFIG_CRITERION_EXACT,
		CONFIG_CRITERION_ATLEAST,
		CONFIG_CRITERION_MASK,
		CONFIG_CRITERION_SPECIAL,
	}CONFIG_CRITERION;

	typedef enum {
		//TYPE
		ATTRIB_TYPE_INTEGER,
		ATTRIB_TYPE_BOOLEAN,
		ATTRIB_TYPE_BITMASK,
		ATTRIB_TYPE_ENUM,
		ATTRIB_TYPE_PSEUDO,
		ATTRIB_TYPE_PLATFORM,
	}_CONFIG_TYPE;

	typedef struct{
		CONFIG_INDEX	config_index;
		EGLenum			attribute_id;
		int				default_value;
		int*			pTable;
	}CONFIG_TABLE_DESC;


	class _EGLConfig
	{
	public :
		_EGLConfig(_EGLConfig* pNext);
		virtual ~_EGLConfig(void);

	public:
		BOOL CheckConfig(int item, int *pTable);
		void FindConfig(int *pTable, EGLConfig *pConfig, int ConfigSize, int& config_index, int id = 0, int step = 1);
		BOOL MakeTable(int id, int *pTable);	// 주어진 ID로 테이블 생성하기

		int ConfigSize(void);

		void MakeConfigFromUserAttribute(int* pTable, EGLint const * attrib_list);  //match test with api chooseConfig  by BufferSize

		void SetAttribute(int AttribCount, int ItemCount, CONFIG_CRITERION criterion, ...);
		void SetItem(int index, ...);

		CONFIG_INDEX GetIndexFromAttributeID(EGLint attribute_id);


	private:
		_EGLConfig*			m_pNext;
		int					m_iAttribCount;
		int					m_iItemCount;
		CONFIG_CRITERION	m_Criterion;
		CONFIG_TABLE_DESC*	m_pDesc;
		const EGLint		*attrib_list;

	};

};

#endif  //_EGL_CONFIG_