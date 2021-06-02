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
#include <RC1/rc_structs.h>
#include <rc_util.h>


void TreeBuilder::KdTreeBuild( DWORD *PrimNum, BOUNDING_BOX& kd_bound, unsigned int MaxPrimNum, unsigned int Depth ) {
	int Num, RetValue=0;

	unsigned int DMaxPrimNum, DataAddr=0;
	float BestSplit;
	unsigned int LeftNum, RightNum;
	int BestAxis=3;
	BOUNDING_BOX Box;
	struct _Cost Cost;
	int NodeAddr;
	DWORD *LeftNewPrimNum,*RightNewPrimNum;
	int StartEnd;
	unsigned int BestPrimNum;
	float x,y,z;
	unsigned int axis[3];

	DDK_NODE* pNode;
	NODE* pCurNode;

	float DefCost, BestCost;
	int BestSEFlag;//dkmodify (insert)
	int addr1=0;

	BestSplit	= 0;
	StartEnd	= 0;
	LeftNum		= 0;
	RightNum	= 0;
	BestSEFlag	= 0;//dkmodify (insert)
	BestPrimNum	= 0;

	NodeAddr	= m_Pool.Node.dwCount;// + g_TreeBuilder->NodeStartIndex;
	pCurNode	= m_Node[NodeAddr];
	m_Pool.Node.dwCount++;

	DMaxPrimNum=2*MaxPrimNum;

	x = kd_bound.maximum.x-kd_bound.minimum.x;
	y = kd_bound.maximum.y-kd_bound.minimum.y;
	z = kd_bound.maximum.z-kd_bound.minimum.z;

	if(x>y&&x>z) {
		if(y>z) {
			axis[0]=0; axis[1]=1; axis[2]=2;
		} else {
			axis[0]=0; axis[1]=2; axis[2]=1;
		}
	}
	else if(y>z) {
		if(x>z) {
			axis[0]=1; axis[1]=0; axis[2]=2;
		} else {
			axis[0]=1; axis[1]=2; axis[2]=0;
		}
	}
	else {
		if(x>y) {
			axis[0]=2; axis[1]=0; axis[2]=1;
		} else {
			axis[0]=2; axis[1]=1; axis[2]=0;
		}
	}

	//DefCost     = (float)(m_fNodeCost + (m_fListCost+m_fPrimCost) *MaxPrimNum);
	DefCost     = (float)(m_fNodeCost + (m_fPrimCost) *MaxPrimNum);
	Cost.Cost   = DefCost;
	BestCost    = DefCost;

	if(MaxPrimNum>m_dwMaxLeaf) {

		for(Saxis=0; Saxis<3; Saxis++) {
			Cost.Cost       = DefCost;
			Cost.LeftNum    = 0;
			Cost.RightNum   = 0;

			RetValue=KdCostCal(&PrimNum[DataAddr+DMaxPrimNum*axis[Saxis]], &PrimNum[DataAddr+DMaxPrimNum*axis[Saxis]+MaxPrimNum], MaxPrimNum, kd_bound, axis[Saxis], &Cost);

			if(Cost.Cost<(DefCost*0.8f)){
				BestCost    = Cost.Cost;
				BestAxis    = axis[Saxis];
				BestSplit   = Cost.Split;
				BestPrimNum = Cost.PrimNum;
				StartEnd    = Cost.StartEnd;
				LeftNum     = Cost.LeftNum;
				RightNum    = Cost.RightNum;
				BestSEFlag  = Cost.Flag;//dkmodify (insert)
				break;
			}
			if(Cost.Cost<BestCost) {
				BestCost    = Cost.Cost;
				BestAxis    = axis[Saxis];
				BestSplit   = Cost.Split;
				BestPrimNum = Cost.PrimNum;
				StartEnd    = Cost.StartEnd;
				LeftNum     = Cost.LeftNum;
				RightNum    = Cost.RightNum;
				BestSEFlag  = Cost.Flag;//dkmodify (insert)
			}
		}
	}

	if(BestAxis==3){   /////LeafNode
		pCurNode->listIndex			= m_Pool.List.dwCount+m_Pool.List.dwStartIndex;
		pCurNode->primimiveNumber	= MaxPrimNum;

		SET_NODE_AXIS((*pCurNode), 3);
		{	// set HW data
			DWORD index		= m_Pool.List.dwCount+m_Pool.List.dwStartIndex;
			pNode			= m_Pool.Node.Get(NodeAddr);
			pNode->axis		= 3;
			pNode->index_l	= index;
			pNode->index_h	= index>>8;
			pNode->number	= pCurNode->primimiveNumber;
		}

		for(Saxis=0;Saxis<MaxPrimNum;Saxis++) {
			Num=PrimNum[Saxis];
			DDK_LIST* pList			= m_Pool.List.Get(m_Pool.List.dwCount+Saxis);
			pList->TriangleIndex	= m_LocalSource.asSourceIndex[Num]/*+g_TreeBuilder.TriStartIndex*/;
		}

		m_Pool.List.dwCount+=MaxPrimNum;
		if(m_dwMaxPrimitive < MaxPrimNum)
			m_dwMaxPrimitive = MaxPrimNum;

		return;
	}

	Depth++;

	//Test¿ë...
	//if(m_dwMaxDepth<Depth)	m_dwMaxDepth = Depth;
	//if(BestAxis==0)			m_dwSplitXCount++;
	//if(BestAxis==1)			m_dwSplitYCount++;
	//if(BestAxis==2)			m_dwSplitZCount++;

	pCurNode->split	= BestSplit;

	SET_NODE_AXIS((*pCurNode), BestAxis);

	{	// set HW data
		pNode			= m_Pool.Node.Get(NodeAddr);
		pNode->axis		= BestAxis;
		pNode->number	= f32f24(pCurNode->split);
	}

	RightNewPrimNum = &PrimNum[6*MaxPrimNum];
	LeftNewPrimNum  = &PrimNum[6*MaxPrimNum+6*(MaxPrimNum-RightNum)];

	//dk modified classify ////////////////////////////////////////////////////////////////////////////
	if(LeftNum==0)
		memcpy(RightNewPrimNum, PrimNum,sizeof(unsigned int)*(MaxPrimNum-RightNum)*6);
	else if((MaxPrimNum-RightNum)==0)
		memcpy(LeftNewPrimNum, PrimNum,sizeof(unsigned int)*LeftNum*6);
	else
		ClassifyPrimitive(MaxPrimNum, BestAxis, PrimNum, BestSplit, BestSEFlag, LeftNewPrimNum, RightNewPrimNum, BestPrimNum, StartEnd, LeftNum, (MaxPrimNum-RightNum));//dkmodify (modify)
	///////////////////////////////////////////////////////////////////////////////////////////////////
	{
		memcpy(&Box, &kd_bound,sizeof(BOUNDING_BOX));
		Box.maximum.m[BestAxis]=BestSplit;
		KdTreeBuild(LeftNewPrimNum, Box, LeftNum, Depth);
	}

	SET_NODE_RIGHTCHILD((*pCurNode), m_Pool.Node.dwCount+m_Pool.Node.dwStartIndex);
	{	// set HW data : set right child
		pNode			= m_Pool.Node.Get(NodeAddr);
		//DWORD index		= GET_NODE_RIGHTCHILD(m_pCurNode[NodeAddr]);
		DWORD index		= GET_NODE_RIGHTCHILD((*pCurNode));
		pNode->index_l	= index;
		pNode->index_h	= index>>8;
	}

	{
		unsigned int RightSize=MaxPrimNum-RightNum;
		memcpy(&Box, &kd_bound,sizeof(BOUNDING_BOX));
		Box.minimum.m[BestAxis]=BestSplit;
		KdTreeBuild(RightNewPrimNum, Box, RightSize, Depth);
	}
}

int TreeBuilder::KdCostCal(DWORD *StartPrimData, 
	DWORD *EndPrimData, 
	unsigned int MaxPrimNum, 
	BOUNDING_BOX Box, 
	unsigned int axis, 
	struct _Cost *Best) {

	float NodeCost, PrimitiveCost;
	float SA, SB, SC;
	float Sa;
	float InvSuA;
	float SASBSC;

	float XLength, YLength,ZLength;
	float Length;
	float BoxMin, BoxMax;

	float Cost;
	int PrimNumStart, PrimNumEnd, FlagStart, FlagEnd;//dkmodify (insert)
	float SplitStart, SplitEnd, SplitTemp, SplitStartTemp, SplitEndTemp;
	int SplitSame;
	float BoundSplit=0;

	float LeftSA, RightSA;
	float LeftCost, RightCost;

	unsigned int i,j,k;
	int N1, N2;
	int StartEnd;
	int RetValue=0;
	int axis_data[5] = {0,1,2,0,1};
	int addr1=0;

	int x, y, z;
	if(MaxPrimNum==0)return 0;

	x = axis;
	y = axis_data[axis+1];
	z = axis_data[axis+2];

	XLength = Box.maximum.m[x] - Box.minimum.m[x];
	YLength = Box.maximum.m[y] - Box.minimum.m[y];
	ZLength = Box.maximum.m[z] - Box.minimum.m[z];

	BoxMin = Box.minimum.m[x];
	BoxMax = Box.maximum.m[x];

	SA = XLength * YLength;
	SB = XLength * ZLength;
	SC = YLength * ZLength;

	if(SA==0||SB==0||SC==0)
		printf("Length Error\n");

	SASBSC=SA + SB + SC;

	InvSuA= 1.0f/SASBSC;

	NodeCost = (float)(m_fNodeCost);
	//PrimitiveCost = (float)(m_fListCost+m_fPrimCost);
	PrimitiveCost = (float)(m_fPrimCost);

	j=0;k=0;

	N1=0;
	N2=MaxPrimNum;
	PrimNumStart = StartPrimData[0];
	PrimNumEnd   = EndPrimData[0];
	SplitStart   = m_LocalSource.fSplit[0][x][PrimNumStart];
	SplitEnd     = m_LocalSource.fSplit[1][x][PrimNumEnd];
	FlagStart    = m_LocalSource.Flag[0][x][PrimNumStart];//dkmodify (insert)
	FlagEnd      = m_LocalSource.Flag[1][x][PrimNumEnd];//dkmodify (insert)
	SplitTemp    = SplitStart - 1;
	SplitSame    = 0;

	//Normal Cost
	for(i=0;i<MaxPrimNum*2;i++) {

		//printf("%d %f %f ", i, SplitStart, SplitEnd);
		if(SplitStart<SplitEnd) {
			//Start 
			StartEnd = 0;
			SplitStartTemp=SplitStart;
		}
		else if(SplitStart>SplitEnd) {
			//End
			StartEnd = 1;
			SplitEndTemp=SplitEnd;
		}
		//dkmodify (insert)///////////////
		else if(FlagStart > FlagEnd) {
			StartEnd = 1;
			SplitEndTemp = SplitEnd;
		}
		///////////////////////////////////
		else if(PrimNumStart>PrimNumEnd) {
			StartEnd = 1;
			SplitEndTemp=SplitEnd;
		}
		else{
			StartEnd = 0;
			SplitStartTemp=SplitStart;
		}

		if(StartEnd){
			if(SplitTemp!=SplitEndTemp) {
				SplitTemp=SplitEndTemp;
				SplitSame = 1;
			}
			else SplitSame = 0;
		}
		else{
			if(SplitTemp!=SplitStartTemp) {
				SplitTemp=SplitStartTemp;
				SplitSame = 1;
			}
			else SplitSame = 0;
		}

		if(StartEnd==1)N2--;
		if(SplitTemp>=BoxMax)
			break;
		if(SplitTemp>BoxMin){
			Length = SplitTemp - BoxMin;
			Sa = (YLength + ZLength)*Length;

			LeftSA = Sa + SC;
			RightSA = SASBSC - Sa;

			LeftCost  = LeftSA  * (NodeCost + PrimitiveCost * N1);
			RightCost = RightSA * (NodeCost + PrimitiveCost * N2);

			Cost = (LeftCost + RightCost) * InvSuA + NodeCost;

			if(Cost<=Best->Cost) {
				Best->Cost = Cost;
				Best->LeftNum = j;
				Best->RightNum = k+StartEnd;
				Best->StartEnd = StartEnd;

				if(StartEnd) {
					Best->PrimNum	= PrimNumEnd;
					Best->Split		= SplitEnd;
					Best->Flag		= FlagEnd;//dkmodify (insert)
				} else {
					Best->PrimNum	= PrimNumStart;
					Best->Split		= SplitStart;
					Best->Flag		= FlagStart;//dkmodify (insert)
				}
				RetValue = 1;
			}
		}
		if(StartEnd==0)N1++;

		if(StartEnd==1) {	// right

			if((k+1)<MaxPrimNum) {
				k++;
				PrimNumEnd		= EndPrimData[k];
				SplitEnd		= m_LocalSource.fSplit[1][x][PrimNumEnd];
				FlagEnd			= m_LocalSource.Flag[1][x][PrimNumEnd];//dkmodify (insert)
			}
			else {}
		} else {	// left
			if((j+1)<MaxPrimNum) {
				j++;
				PrimNumStart	= StartPrimData[j];
				SplitStart		= m_LocalSource.fSplit[0][x][PrimNumStart];
				FlagStart		= m_LocalSource.Flag[0][x][PrimNumStart];//dkmodify (insert)
			} else {
				j++;
				PrimNumStart	= EndPrimData[MaxPrimNum-1];//end
				SplitStart		= m_LocalSource.fSplit[1][x][PrimNumStart]+1;
			}
		}
	}
	return RetValue;   
}

void TreeBuilder::ClassifyPrimitive(unsigned int MaxPrimNum, unsigned int BestAxis, DWORD *PrimNum, float BestSplit, int BestSEFlag, DWORD *ClassifyLeftPrimNum, DWORD *ClassifyRightPrimNum, unsigned int BestPrimNum, unsigned int StartEnd, unsigned int LeftNum, unsigned int RightNum){
	register unsigned int i, j;
	DWORD *Index;
	float *ClassifySplitMax, *ClassifySplitMin;
	int *ClassifyFlagMax, *ClassifyFlagMin;//dkmodify (insert)
	float SplitMin, SplitMax;
	int FlagMin, FlagMax;//dkmodify (insert)
	unsigned int ClassfyLeftRight[1024];
	DWORD *LeftStart, *RightStart;

	//1 : Left, 2 : right 3 : left and right
	unsigned int LeftRight;

	ClassifySplitMin	= m_LocalSource.fSplit[0][BestAxis];
	ClassifySplitMax	= m_LocalSource.fSplit[1][BestAxis];

	ClassifyFlagMin		= m_LocalSource.Flag[0][BestAxis];//dkmodify (insert)
	ClassifyFlagMax		= m_LocalSource.Flag[1][BestAxis];//dkmodify (insert)

	Index = PrimNum;
	LeftStart			= ClassifyLeftPrimNum;
	RightStart			= ClassifyRightPrimNum;

	//printf("%f %d\n", BestSplit, BestPrimNum);
	//dkmodify (modify)/////////////////////////////////////////////////////////////////////////////////////
	for(j=0;j<MaxPrimNum;j++) {

		SplitMin = ClassifySplitMin[*Index];
		SplitMax = ClassifySplitMax[*Index];

		FlagMin = ClassifyFlagMin[*Index];//dkmodify (insert)
		FlagMax = ClassifyFlagMax[*Index];//dkmodify (insert)

		if((SplitMax<BestSplit) || ((SplitMax==BestSplit) && (FlagMax<BestSEFlag))){//dkmodify (modify)
			ClassfyLeftRight[*Index] = 1;
		}
		else if((SplitMax==BestSplit) && (FlagMax==BestSEFlag)){//dkmodify (modify)

			if(*Index<BestPrimNum) {
				ClassfyLeftRight[*Index] = 1;
			}
			else if(*Index>BestPrimNum) {
				ClassfyLeftRight[*Index] = 2;
			}
			else{   //self
				if(StartEnd) {
					ClassfyLeftRight[*Index] = 1;
				}
				else {
					ClassfyLeftRight[*Index] = 2;
				}
			}
		}
		else if((SplitMin>BestSplit) || ((SplitMin==BestSplit) && (FlagMin>BestSEFlag))) {//dkmodify (modify)
			ClassfyLeftRight[*Index] = 2;
		}
		else if((SplitMin==BestSplit) && (FlagMin==BestSEFlag)) {//dkmodify (modify)

			if(*Index<BestPrimNum) {
				ClassfyLeftRight[*Index] = 1;
			}
			else if(*Index>BestPrimNum) {
				ClassfyLeftRight[*Index] = 2;
			}
			else {
				ClassfyLeftRight[*Index]	= StartEnd ? 1 : 2;
			}
		}
		else {
			ClassfyLeftRight[*Index] = 3;
		}
		Index++;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Index = PrimNum;
	for(i=0;i<6;i++) {

		for(j=0;j<MaxPrimNum;j++) {
			LeftRight	= ClassfyLeftRight[*Index];

			if(LeftRight == 1) {
				*ClassifyLeftPrimNum	= *Index;
				ClassifyLeftPrimNum++;
			}
			else if(LeftRight == 2) {
				*ClassifyRightPrimNum	= *Index;
				ClassifyRightPrimNum++;
			}
			else {
				*ClassifyLeftPrimNum	= *Index;
				ClassifyLeftPrimNum++;
				*ClassifyRightPrimNum	= *Index;
				ClassifyRightPrimNum++;
			}
			Index++;
		}
	}
	if((LeftNum*6)!=(ClassifyLeftPrimNum-LeftStart))		printf("left Num error\n");
	if((RightNum*6)!=(ClassifyRightPrimNum-RightStart))		printf("right Num error\n");
}
