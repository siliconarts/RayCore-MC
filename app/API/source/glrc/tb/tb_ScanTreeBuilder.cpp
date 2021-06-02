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
// DATE    : 8/28/2013 Wed
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include "tb_TreeBuilder.h"

#include <rc_util.h>

void TreeBuilder::ScanTreeBuild(DWORD *PrimIndex, BOUNDING_BOX& kd_bound, unsigned int MaxPrimNum, unsigned int Depth){
	SCANDATA ScanData[31];
	int RetValue, DataAddr=0;
	float BestSplit;
	unsigned int LeftNum, RightNum;
	int BestAxis=3;
	BOUNDING_BOX Box;
	struct _Cost Cost;
	int NodeAddr;
	DWORD *LeftNewPrimNum,*RightNewPrimNum;
	unsigned int j, Index;
	float SplitMin, SplitMax;
	int BestPrimNum;
	float x,y,z;
	unsigned int axis[3];
	int LeftIndex, RightIndex;

	DDK_NODE* pNode;
	NODE* pCurNode;

	float DefCost, BestCost;
	int BestSEFlag		= 0;//dkmodify (insert)
	int addr1			= 0;

	BestSplit			= 0;
	LeftNum				= 0;
	RightNum			= 0;
	BestPrimNum			= 0;

	NodeAddr	= m_Pool.Node.dwCount;// + g_TreeBuilder.NodeStartIndex;
	pCurNode	= m_Node[NodeAddr];
	m_Pool.Node.dwCount++;

	x = kd_bound.maximum.x - kd_bound.minimum.x;
	y = kd_bound.maximum.y - kd_bound.minimum.y;
	z = kd_bound.maximum.z - kd_bound.minimum.z;

	if(x>y&&x>z){
		if(y>z){
			axis[0]=0; axis[1]=1; axis[2]=2;
		}
		else{
			axis[0]=0; axis[1]=2; axis[2]=1;
		}
	}
	else if(y>z){
		if(x>z){
			axis[0]=1; axis[1]=0; axis[2]=2;
		}
		else{
			axis[0]=1; axis[1]=2; axis[2]=0;
		}
	}
	else{
		if(x>y){
			axis[0]=2; axis[1]=0; axis[2]=1;
		}
		else{
			axis[0]=2; axis[1]=1; axis[2]=0;
		}
	}

	//DefCost   = (float)(m_fNodeCost + (m_fListCost+m_fPrimCost) * MaxPrimNum);
	DefCost   = (float)(m_fNodeCost + (m_fPrimCost) * MaxPrimNum);
	Cost.Cost = DefCost;
	BestCost  = DefCost;

	if(MaxPrimNum > m_dwMaxLeaf){

		for(Saxis=0; Saxis<3; Saxis++){

			Cost.Cost     = DefCost;
			Cost.LeftNum  = 0;
			Cost.RightNum = 0;

			ScanSetup(ScanData, kd_bound, PrimIndex, MaxPrimNum, axis[Saxis]);
			RetValue=ScanCostCal(ScanData, kd_bound, axis[Saxis], &Cost);

			if(Cost.Cost<(DefCost*0.8f)){
				BestCost    = Cost.Cost;
				BestAxis    = axis[Saxis];
				BestSplit   = Cost.Split;
				LeftNum     = Cost.LeftNum;
				RightNum    = Cost.RightNum;
				BestSEFlag  = Cost.Flag;	// dkmodify (insert)
				break;
			}
			if(Cost.Cost<BestCost){
				BestCost    = Cost.Cost;
				BestAxis    = axis[Saxis];
				BestSplit   = Cost.Split;
				LeftNum     = Cost.LeftNum;
				RightNum    = Cost.RightNum;
				BestSEFlag  = Cost.Flag;	// dkmodify (insert)
			}
		}
	}
	if(BestAxis==3){   /////LeafNode
		pCurNode->listIndex				= m_Pool.List.dwCount+m_Pool.List.dwStartIndex;
		pCurNode->primimiveNumber		= MaxPrimNum;

		SET_NODE_AXIS((*pCurNode), 3);

		{	// set HW data
			pNode			= m_Pool.Node.Get(NodeAddr);
			pNode->axis		= 3;
			pNode->index_l	= pCurNode->listIndex;
			pNode->index_h	= pCurNode->listIndex>>8;
			pNode->number	= pCurNode->primimiveNumber;
		}

		for(Saxis=0;Saxis<MaxPrimNum;Saxis++){
			AS_SOURCE* pSrc			= m_Src.Get(PrimIndex[Saxis]);
			DDK_LIST* pList			= m_Pool.List.Get(m_Pool.List.dwCount+Saxis);
			pList->TriangleIndex	= pSrc->dwIndex/*+g_TreeBuilder.TriStartIndex*/;
		}

		m_Pool.List.dwCount += MaxPrimNum;

		if(m_dwMaxPrimitive<MaxPrimNum)
			m_dwMaxPrimitive = MaxPrimNum;

		return;
	}

	Depth++;

	//Test¿ë...
	//if(m_dwMaxDepth<Depth)
	//	m_dwMaxDepth = Depth;
	//if(BestAxis==0)
	//	m_dwSplitXCount++;
	//if(BestAxis==1)
	//	m_dwSplitYCount++;
	//if(BestAxis==2)
	//	m_dwSplitZCount++;

	pCurNode->split	= BestSplit;

	SET_NODE_AXIS((*pCurNode), BestAxis);
	{	// set HW data
		pNode			= m_Pool.Node.Get(NodeAddr);
		pNode->axis		= BestAxis;
		pNode->number	= f32f24(pCurNode->split);
	}

	RightNewPrimNum = &PrimIndex[MaxPrimNum];
	LeftNewPrimNum  = &PrimIndex[MaxPrimNum+RightNum];

	LeftIndex = 0;
	RightIndex = 0;

	for(j=0;j<MaxPrimNum;j++){
		Index    = PrimIndex[j];
		AS_SOURCE* pSrc	= m_Src.Get(Index);
		SplitMin = pSrc->split[0].fdata[BestAxis];
		SplitMax = pSrc->split[1].fdata[BestAxis];

		if(SplitMax<BestSplit)
			LeftNewPrimNum[LeftIndex++]=Index;
		else if(SplitMax==BestSplit){
			if(SplitMin<BestSplit)
				LeftNewPrimNum[LeftIndex++]=Index;
			else if(BestSEFlag<16)
				LeftNewPrimNum[LeftIndex++]=Index;
			else 
				RightNewPrimNum[RightIndex++]=Index;
		}
		else if(SplitMin<BestSplit){
			LeftNewPrimNum[LeftIndex++]=Index;
			RightNewPrimNum[RightIndex++]=Index;
		}
		else
			RightNewPrimNum[RightIndex++]=Index;
	} 

	if(LeftIndex!=LeftNum) printf("Left error\n");
	if(RightIndex!=RightNum) printf("Right error\n");

	{
		memcpy(&Box, &kd_bound,sizeof(BOUNDING_BOX));
		Box.maximum.m[BestAxis]=BestSplit;
		if(LeftNum>m_dwMaxTDN)
			ScanTreeBuild(LeftNewPrimNum, Box, LeftNum, Depth);
		else{
			TreeBuild(LeftNewPrimNum, Box, LeftNum, Depth);
		}
	}

	SET_NODE_RIGHTCHILD((*pCurNode), m_Pool.Node.dwCount+m_Pool.Node.dwStartIndex);
	{	// set HW data : set right child
		DWORD index			= GET_NODE_RIGHTCHILD((*pCurNode));
		pNode				= m_Pool.Node.Get(NodeAddr);
		pNode->index_l		= index;
		pNode->index_h		= index>>8;
	}
	
	{
		memcpy(&Box, &kd_bound,sizeof(BOUNDING_BOX));
		Box.minimum.m[BestAxis]=BestSplit;
		if(RightNum>m_dwMaxTDN)
			ScanTreeBuild(RightNewPrimNum, Box, RightNum, Depth);
		else{
			TreeBuild(RightNewPrimNum, Box, RightNum, Depth);
		}
	}
}

void TreeBuilder::ScanSetup(SCANDATA ScanData[31], BOUNDING_BOX& kd_bound, DWORD *PrimNum, unsigned int MaxPrimNum, unsigned int axis){
	unsigned int i,j;
	float Min, Max;
	float SplitMin, SplitMax;

	Min = kd_bound.minimum.m[axis];
	Max = kd_bound.maximum.m[axis];

	for(i=1;i<16;i++){
		ScanData[i-1].Split=Min+i*(Max-Min)/16;
		ScanData[i-1].LeftNum=0;
		ScanData[i-1].RightNum=0;
	}

	for(j=0;j<MaxPrimNum;j++){
		AS_SOURCE* pSrc	= m_Src.Get(PrimNum[j]);
		SplitMin = pSrc->split[0].fdata[axis];
		SplitMax = pSrc->split[1].fdata[axis];
		//for(i=0;i<31;i++){
		for(i=0;i<15;i++){
			if(SplitMax<ScanData[i].Split)
				ScanData[i].LeftNum++;
			else if(SplitMax==ScanData[i].Split){
				if(SplitMin<ScanData[i].Split)
					ScanData[i].LeftNum++;
				//else if(i<16)
				else if(i<8)
					ScanData[i].LeftNum++;
				else 
					ScanData[i].RightNum++;
			}
			else if(SplitMin<ScanData[i].Split){
				ScanData[i].LeftNum++;
				ScanData[i].RightNum++;
			}
			else
				ScanData[i].RightNum++;
		}
	}
}

int TreeBuilder::ScanCostCal(SCANDATA ScanData[31], BOUNDING_BOX& Box, unsigned int axis, struct _Cost *Best){


	float NodeCost, PrimitiveCost;
	float SA, SB, SC;
	float Sa;
	float InvSuA;
	float SASBSC;

	float XLength, YLength,ZLength;
	float Length;
	float BoxMin;

	float Cost;
	float LeftSA, RightSA;
	float LeftCost, RightCost;

	int x, y, z;
	int axis_data[5] = {0,1,2,0,1};
	int RetValue=0;
	int i;

	x = axis;
	y = axis_data[axis+1];
	z = axis_data[axis+2];

	XLength = Box.maximum.m[x] - Box.minimum.m[x];
	YLength = Box.maximum.m[y] - Box.minimum.m[y];
	ZLength = Box.maximum.m[z] - Box.minimum.m[z];

	BoxMin = Box.minimum.m[x];

	SA = XLength * YLength;
	SB = XLength * ZLength;
	SC = YLength * ZLength;
	if(SA==0||SB==0||SC==0)
		return 0;

	SASBSC=SA + SB + SC;

	InvSuA= 1.0f/SASBSC;

	NodeCost = (float)(m_fNodeCost);
	//PrimitiveCost = (float)(m_fListCost+m_fPrimCost);
	PrimitiveCost = (float)(m_fPrimCost);

	//for(i=0;i<31;i++){
	for(i=0;i<15;i++){
		Length = ScanData[i].Split - BoxMin;
		Sa = (YLength + ZLength)*Length;
		LeftSA = Sa + SC;
		RightSA = SASBSC - Sa;

		LeftCost  = LeftSA  * (NodeCost + PrimitiveCost * ScanData[i].LeftNum);
		RightCost = RightSA * (NodeCost + PrimitiveCost * ScanData[i].RightNum);

		Cost = (LeftCost + RightCost) * InvSuA + NodeCost;

		if(Cost<=Best->Cost){
			Best->Cost = Cost;
			Best->LeftNum = ScanData[i].LeftNum;
			Best->RightNum = ScanData[i].RightNum;
			Best->Flag = i;//dkmodify (insert)
			Best->Split = ScanData[i].Split;
			RetValue = 1;
		}
	}
	return RetValue;
}
