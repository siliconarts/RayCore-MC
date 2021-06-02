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
// DATE    : 7/16/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>

#include <rc_define.h>
#include <RC1/rc_structs.h>

#include "tb_TreeBuilder.h"

void TreeBuilder::TreeBuild(DWORD *PrimIndex, BOUNDING_BOX kd_bound, unsigned int MaxPrimNum, unsigned int Depth){
	unsigned int i;

	for(i=0;i<MaxPrimNum;i++){
		AS_SOURCE* pSrc	= m_Src.Get(PrimIndex[i]);
		m_LocalSource.fSplit[0][0][i]	= pSrc->split[0].fdata[0];//dk min
		m_LocalSource.fSplit[0][1][i]	= pSrc->split[0].fdata[1];
		m_LocalSource.fSplit[0][2][i]	= pSrc->split[0].fdata[2];

		m_LocalSource.fSplit[1][0][i]	= pSrc->split[1].fdata[0];//dk max
		m_LocalSource.fSplit[1][1][i]	= pSrc->split[1].fdata[1];
		m_LocalSource.fSplit[1][2][i]	= pSrc->split[1].fdata[2];

		m_LocalSource.asSourceIndex[i]	= pSrc->dwIndex;

		m_LocalSource.Flag[0][0][i]		= pSrc->split[0].flag[0];
		m_LocalSource.Flag[0][1][i]		= pSrc->split[0].flag[1];
		m_LocalSource.Flag[0][2][i]		= pSrc->split[0].flag[2];
		m_LocalSource.Flag[1][0][i]		= pSrc->split[1].flag[0];
		m_LocalSource.Flag[1][1][i]		= pSrc->split[1].flag[1];
		m_LocalSource.Flag[1][2][i]		= pSrc->split[1].flag[2];
	}

	{	// dk merge sort optimize
		merge_sort(m_pDefPrimNum, &PrimIndex[0], MaxPrimNum, m_LocalSource.fSplit[0][0], m_LocalSource.Flag[0][0]);
		merge_sort(m_pDefPrimNum, &PrimIndex[MaxPrimNum],MaxPrimNum, m_LocalSource.fSplit[1][0], m_LocalSource.Flag[1][0]);
		merge_sort(m_pDefPrimNum, &PrimIndex[2*MaxPrimNum],MaxPrimNum, m_LocalSource.fSplit[0][1], m_LocalSource.Flag[0][1]);
		merge_sort(m_pDefPrimNum, &PrimIndex[3*MaxPrimNum],MaxPrimNum, m_LocalSource.fSplit[1][1], m_LocalSource.Flag[1][1]);
		merge_sort(m_pDefPrimNum, &PrimIndex[4*MaxPrimNum],MaxPrimNum, m_LocalSource.fSplit[0][2], m_LocalSource.Flag[0][2]);
		merge_sort(m_pDefPrimNum, &PrimIndex[5*MaxPrimNum],MaxPrimNum, m_LocalSource.fSplit[1][2], m_LocalSource.Flag[1][2]);
	}
	KdTreeBuild(PrimIndex, kd_bound, MaxPrimNum, Depth);
}

//dk Src : default primitive number, Dest : primitive number
void TreeBuilder::merge_sort(DWORD *Src, DWORD *Dest, size_t nelem, float *LocalfSplit, int *LocalFlag){
	unsigned int temp1[1024],temp2[1024];
	int first, second, toggle;
	unsigned int i, j, k, size;
	unsigned int *base, *b;
	float Split1, Split2;
	int Flag1, Flag2;
	//dk # of element > 4//////////////
	if(nelem<4){
		for(i=0;i<nelem;i++)
			Dest[i]=Src[i];
		return;
	}
	//////////////////////////////////////
	b=temp1;
	//dk input(level 1)///////////////////////////////////////////////////////////////////
	for(i=0,k=0;i<nelem-1;i+=2){

		Split1 = LocalfSplit[Src[i]];//dk editing: i, i+1?
		Split2 = LocalfSplit[Src[i+1]];
		Flag1 = LocalFlag[Src[i]];
		Flag2 = LocalFlag[Src[i+1]];

		if(Split1>Split2){//dk a[i] > a[j] split
			b[k++] = Src[i+1];
			b[k++] = Src[i];
		}
		else if(Split1<Split2){//dk a[i] < a[j] split
			b[k++] = Src[i];
			b[k++] = Src[i+1];
		}
		else{//dk a[i] == a[j] split
			if(Flag1>Flag2){
				b[k++] = Src[i+1];
				b[k++] = Src[i];
			}
			else{
				b[k++] = Src[i];
				b[k++] = Src[i+1];
			}
		}
	}

	if(nelem&1) //dk last data in # of element is odd
		b[k]=Src[k];

	base	= b;
	b		= temp2;
	toggle	= 0;
	////////////////////////////////////////////////////////////////////////////////
	//dk merge sort//////////////////////////////////////////////////////////////
	for (size = 2; (size<<1) < nelem; size <<= 1) { //dk level 2 ~ n-1
		first = -2 * size;
		second = first + size;
		while (second + size * 2 < nelem) {
			first = second + size;
			second = first + size;
			i = first;
			j = second;
			k = first;
			Split1 = LocalfSplit[base[i]];
			Split2 = LocalfSplit[base[j]];
			Flag1 = LocalFlag[base[i]];
			Flag2 = LocalFlag[base[j]];
			//while ((i <( first + size)) || ((j < (second + size)) && (j < nelem))) {
			while (1) {
				if(Split1>Split2){//dk a[i] > a[j] split
					b[k++] = base[j++];
					if (j < second + size && j < nelem){
						Split2 = LocalfSplit[base[j]];
						Flag2 = LocalFlag[base[j]];
					}
					else{
						for(i;i<first+size;i++)
							b[k++] = base[i];
						break;
					}
				}
				else if(Split1<Split2){//dk a[i] < a[j] split
					b[k++] = base[i++];
					if (i < first + size){
						Split1 = LocalfSplit[base[i]];
						Flag1 = LocalFlag[base[i]];
					}
					else{
						for(j;j < second + size && j < nelem;j++)
							b[k++] = base[j];
						break;
					}
				}
				else{//dk a[i] == a[j] split
					if(Flag1>Flag2){
						b[k++] = base[j++];
						if (j < second + size && j < nelem){
							Split2 = LocalfSplit[base[j]];
							Flag2 = LocalFlag[base[j]];
						}
						else{
							for(i;i<first+size;i++)
								b[k++] = base[i];
							break;
						}
					}
					else{
						b[k++] = base[i++];
						if (i < first + size){
							Split1 = LocalfSplit[base[i]];
							Flag1 = LocalFlag[base[i]];
						}
						else{
							for(j;j < second + size && j < nelem;j++)
								b[k++] = base[j];
							break;
						}
					}
				}
			}
		}
		for(i=second+size; i<nelem;i++){
			b[i]=base[i];
		}
		base=b;
		if(toggle){
			b=temp2;
			toggle^= 1;
		}
		else{
			b=temp1;
			toggle^= 1;
		}		
	}
	/////////////////////////////////////////////////////////////////////////////////
	//dk last level//////////////////////////////////////////////////////////////////
	first = 0;
	second = size;
	i = 0;
	j = second;
	k = 0;
	Split1 = LocalfSplit[base[i]];
	Split2 = LocalfSplit[base[j]];
	Flag1 = LocalFlag[base[i]];
	Flag2 = LocalFlag[base[j]];
	while (1) {
		if(Split1>Split2){//dk a[i] > a[j] split
			Dest[k++] = base[j++];
			if (j < second + size && j < nelem){
				Split2 = LocalfSplit[base[j]];
				Flag2 = LocalFlag[base[j]];
			}
			else{
				for(i;i<first+size;i++)
					Dest[k++] = base[i];
				break;
			}
		}
		else if(Split1<Split2){//dk a[i] < a[j] split
			Dest[k++] = base[i++];
			if (i < first + size){
				Split1 = LocalfSplit[base[i]];
				Flag1 = LocalFlag[base[i]];
			}
			else{
				for(j;j < second + size && j < nelem;j++)
					Dest[k++] = base[j];
				break;
			}
		}
		else{//dk a[i] == a[j] split
			if(Flag1>Flag2){
				Dest[k++] = base[j++];
				if (j < second + size && j < nelem){
					Split2 = LocalfSplit[base[j]];
					Flag2 = LocalFlag[base[j]];
				}
				else{
					for(i;i<first+size;i++)
						Dest[k++] = base[i];
					break;
				}
			}
			else{
				Dest[k++] = base[i++];
				if (i < first + size){
					Split1 = LocalfSplit[base[i]];
					Flag1 = LocalFlag[base[i]];
				}
				else{
					for(j;j < second + size && j < nelem;j++)
						Dest[k++] = base[j];
					break;
				}
			}
		}
	}
}

TreeBuilder::TreeBuilder(void){
	memset(&m_Pool, 0, sizeof(m_Pool));

	m_LocalSource.asSourceIndex = new unsigned int[1024];
	for(int i=0;i<2;i++){
		for(int j=0;j<3;j++){
			m_LocalSource.fSplit[i][j] = new float[1024];
			m_LocalSource.Flag[i][j]   = new int[1024];		//dkmodify (insert)
		}
	}
	m_pDefPrimNum = new DWORD[1024];
}

TreeBuilder::~TreeBuilder(void){
	SAFE_DELETE_ARRAY(m_LocalSource.asSourceIndex);
	for(int i=0;i<2;i++)
	for(int j=0;j<3;j++){
		SAFE_DELETE_ARRAY(m_LocalSource.fSplit[i][j]);
		SAFE_DELETE_ARRAY(m_LocalSource.Flag[i][j]);
	}
	
	SAFE_DELETE_ARRAY(m_pDefPrimNum);
}

void TreeBuilder::Initialize(MemoryPoolT<DDK_NODE>* pNode, MemoryPoolT<DDK_LIST>* pList){
	m_Pool.Node.pMem	= pNode;
	m_Pool.List.pMem	= pList;
}

void TreeBuilder::Setup(RCDDK* pDDK, int primNum, int nodeIndex, int listIndex){
	m_fListCost					= 0.0068;
	m_fNodeCost					= 1.7072;
	m_fPrimCost					= 3.4623+m_fListCost; //dk 2.4623

	m_dwMaxDepth				= 0;
	m_dwSplitXCount				= 0;
	m_dwSplitYCount				= 0;
	m_dwSplitZCount				= 0;
	m_Pool.Node.dwCount			= 0;	// start NodeAddress
	m_Pool.List.dwCount			= 0;	// Start ListAddress
	m_dwMaxPrimitive			= 0;
	m_dwMaxTDN					= 512;
	m_dwMaxLeaf					= 8;	//16;	// Leaf Node Maximum Primitive °¹¼ö

	m_iTriStartIndex			= primNum;
	m_Pool.Node.dwStartIndex	= nodeIndex;
	m_Pool.List.dwStartIndex	= listIndex;

	m_Node.SetBase(nodeIndex);
}
