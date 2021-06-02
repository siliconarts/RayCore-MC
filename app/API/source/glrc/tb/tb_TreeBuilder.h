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
// DATE    : 6/3/2013 Mon
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __TREEBUILDER_H__
#define __TREEBUILDER_H__
#include "STDInterface.h"
#include <RC1/rc_structs.h>
#include "driver/RCStructures.h"
#include "common/gl_Memory.h"

using namespace GL;

#ifdef __linux
	#include <stdlib.h>
#endif

typedef struct {
	unsigned int		*asSourceIndex;
	union{
		float			*fSplit[2][3];
		unsigned int	*iSplit[2][3];
	};
	int					*Flag[2][3];
} AS_SOURCE_LOCAL;

typedef struct{
	DWORD			dwIndex;
	struct{
		union{
			float		fdata[3];
			DWORD		udata[3];
		};
		int			flag[3];
	}split[2];
}AS_SOURCE;

typedef struct _ScanData{
	int		LeftNum;
	int		RightNum;
	float	Split;
} SCANDATA;

struct _Cost{
	float	Cost;
	float	Split;
	DWORD	PrimNum;
	int		StartEnd;
	int		Flag;
	int		LeftNum;
	int		RightNum;
};

template<typename T>
struct IndexedPoolMemoryT{
	MemoryPoolT<T>*	pMem;
	DWORD			dwCount;
	DWORD			dwStartIndex;
	DWORD			dwIndex;

	inline T* Get(DWORD index){return pMem->GetPool(dwStartIndex + index);}
};

class TreeBuilder{
public:
	TreeBuilder(void);
	~TreeBuilder(void);

	void Initialize(MemoryPoolT<DDK_NODE>* pNode, MemoryPoolT<DDK_LIST>* pList);

	void Setup(RCDDK* pDDK, int primNum, int nodeIndex, int listIndex);

	void AddTriangle(RC_VERTEX *vertex, DWORD index, BOUNDING_BOX *Box);
	void BuildAccelerationStructure(BOUNDING_BOX  *boundBox, DWORD totalPrimitiveNumber);

	inline int GetCurrentTriStartIndex(void)	{return m_iTriStartIndex;}
	inline int GetCurrentNodeStartIndex(void)	{return m_Pool.Node.dwStartIndex;}
	inline int GetCurrentListStartIndex(void)	{return m_Pool.List.dwStartIndex;}

	inline DWORD GetNodeCount(void)		{return m_Pool.Node.dwCount;}
	inline DWORD GetListCount(void)		{return m_Pool.List.dwCount;}

	// private
	void ScanTreeBuild(DWORD *PrimNum, BOUNDING_BOX& kd_bound, unsigned int MaxPrimNum, unsigned int Depth);
	void ScanSetup(SCANDATA ScanData[31], BOUNDING_BOX& kd_bound, DWORD *PrimNum, unsigned int MaxPrimNum, unsigned int axis);
	int ScanCostCal(SCANDATA ScanData[31], BOUNDING_BOX& Box, unsigned int axis, struct _Cost *Best);

	void TreeBuild(DWORD *PrimNum, BOUNDING_BOX kd_bound, unsigned int MaxPrimNum, unsigned int Depth);

	void merge_sort(DWORD *Src, DWORD *Dest, size_t nelem, float *LocalfSplit, int *LocalFlag);
	void KdTreeBuild(DWORD *PrimNum, BOUNDING_BOX& kd_bound, unsigned int MaxPrimNum, unsigned int Depth);
	int KdCostCal(DWORD *StartPrimData, DWORD *EndPrimData, unsigned int MaxPrimNum, BOUNDING_BOX Box, unsigned int axis, struct _Cost *Best);
	void ClassifyPrimitive(unsigned int MaxPrimNum, unsigned int BestAxis, DWORD *PrimNum, float BestSplit, int BestSEFlag, DWORD *ClassifyLeftPrimNum, DWORD *ClassifyRightPrimNum, unsigned int BestPrimNum, unsigned int StartEnd, unsigned int LeftNum, unsigned int RightNum);//dkmodify (modify)

private:
	DWORD		m_dwMaxDepth;
	DWORD		m_dwMaxPrimitive;
	DWORD		m_dwMaxTDN;				// Tree d Number
	DWORD		m_dwMaxLeaf;

	int			m_iTriStartIndex;

	struct{
		IndexedPoolMemoryT<DDK_NODE>			Node;
		IndexedPoolMemoryT<DDK_LIST>			List;
	}m_Pool;

	DWORD		m_dwSplitXCount;
	DWORD		m_dwSplitYCount;
	DWORD		m_dwSplitZCount;

	double		m_fNodeCost;
	double		m_fListCost;
	double		m_fPrimCost;

	// temporary (@todo : 정리 필요)
	DWORD		*m_pDefPrimNum;
	AS_SOURCE_LOCAL			m_LocalSource;
	PoolMemoryT<AS_SOURCE>	m_Src;
	// S/W 관리용
	GrowingMemoryT<NODE>	m_Node;
	DWORD		Saxis, Sminmax;
};

#endif//__TREEBUILDER_H__
