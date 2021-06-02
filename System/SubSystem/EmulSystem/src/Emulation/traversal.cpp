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
// Project : Emulation
// DATE    : 8/29/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#include <stdio.h>
#include <stdlib.h>
#include "Emulation_Common.h"
#include "RayRendering.h"
#include "operate.h"

extern _option*				RtOpt;
extern _material*			RtMaterial;
extern _CountInfo			g_CountInfo;

int box_intersection(RayFloat *minimum, RayFloat *maximum, _ray *ray, RayFloat old_tmax, RayFloat *new_tmin, RayFloat *new_tmax){
	RayFloat tmin[3];
	RayFloat tmax[3];

	RayFloat real_min[3];
	RayFloat real_max[3];

	RayFloat max;
	RayFloat min;

	int i;
	int comp, comp1, comp2;

	for (i=0; i<3; i++){
		if(i==0){
			tmin[i] = fmul(TFAdder( minimum[i], SignConv(ray->o[i]), 0),ray->inv_d[i]);
		}
		else
			tmin[i] = fmul(fadder( minimum[i], ray->o[i], 1),ray->inv_d[i]);
		tmax[i] = fmul(fadder( maximum[i], ray->o[i], 1),ray->inv_d[i]);

		comp=compare(tmin[i],tmax[i]);
		if(comp==2){
			real_min[i] = tmin[i];
			real_max[i] = tmax[i];
		}
		else{
			real_min[i] = tmax[i];
			real_max[i] = tmin[i];
		}
	}

	comp = compare(real_max[0], real_max[1]);
	if (comp==2)   max = real_max[0];
	else max = real_max[1];
	comp = compare(real_max[2], max);
	if (comp==2 ) max = real_max[2];

	comp = compare(real_min[0], real_min[1]);
	if (comp==1)   min = real_min[0];
	else min = real_min[1];
	comp = compare(real_min[2], min);
	if (comp==1) min = real_min[2];

	*new_tmin = min;
	*new_tmax = max;
	comp = compare(max,min);
	comp1 = compare(old_tmax, min);
	comp2 = compare(max,0);
	if((comp!=2)&&(comp1==1)&&(comp2==1)){
		return 1;
	}
	else 
		return 0;   
}

void tri_intersectionOpt(int PrimNum, _ray *ray, _hit* Hit){
	int modulo[5]={0,1,2,0,1};
	int k,u,v;
	DDK_PRIMITIVE*	pPrimitive	= &g_mem.pPrimitive[PrimNum];

	RayFloat nd, InvNd, t, hu, hv ;
	RayFloat beta, gamma, BetaPluseGamma;
	int betaComp, gammaComp, bgComp;

#ifdef USE_REDUCED_ERROR_CORRECTION
	_DataMargin3 TFadderError0, TFadderError1;
	_DataMargin2 FadderError0, FadderError1;
	_DataMargin2 FmulError0, FmulError1;
	int temp, temp1, tComp;
	RayFloat Bhu,Bhv, Ghu, Ghv;
	RayFloat DataA, DataB;
#endif
	k=pPrimitive->triangle.axis;
	u=modulo[k+1];
	v=modulo[k+2];

	Hit->t = -1;
	Hit->HitStatus=0;
#ifdef USE_REDUCED_ERROR_CORRECTION
	FmulError0.In0 = (pPrimitive->triangle.nu>>16&0x7f) - 0x4f;
	FmulError0.In1 = (ray->d[u]>>16&0x7f) - 0x4e;
	DataA = fmulMargin(pPrimitive->triangle.nu,ray->d[u], &FmulError0);
   
	FmulError1.In0 = (pPrimitive->triangle.nv>>16&0x7f) - 0x4f;
	FmulError1.In1 = (ray->d[v]>>16&0x7f) - 0x4e;
	DataB = fmulMargin(pPrimitive->triangle.nv,ray->d[v], &FmulError1);
   
	TFadderError0.In0 = (ray->d[k]>>16&0x7f) - 0x4e;
	TFadderError0.In1 = FmulError0.Out;
	TFadderError0.In2 = FmulError1.Out;
   
	InvNd = TFAdderMargin(ray->d[k],DataA, DataB, &TFadderError0);
   
	if(InvNd == 0) return;
	nd = fdivider(0x3f0000,InvNd);
	TFadderError0.Out = (nd>>16&0x7f) - ((InvNd>>16&0x7f)-TFadderError0.Out);
	//////////////////////////////////////////////////////////////////
	FmulError0.In1 = (ray->o[u]>>16&0x7f) - 0x4f;
	DataA = fmulMargin(pPrimitive->triangle.nu,ray->o[u], &FmulError0);

	FmulError1.In1 = (ray->o[v]>>16&0x7f) - 0x4f;
	DataB = fmulMargin(pPrimitive->triangle.nv,ray->o[v], &FmulError1);

	FadderError0.In0 = FmulError0.Out;
	FadderError0.In1 = FmulError1.Out;
	DataA = fadderMargin(DataA, DataB, 0, &FadderError0);

	FadderError1.In0 = 0;//pPrimitive.Margin[2];
	FadderError1.In1 = (ray->o[k]>>16&0x7f) - 0x4f;
	DataB = fadderMargin((((DWORD)pPrimitive->triangle.nd_h<<16) | pPrimitive->triangle.nd_l), ray->o[k], 1, &FadderError1);

	FadderError0.In0 = FadderError0.Out;
	FadderError0.In1 = FadderError1.Out;
	DataA = fadderMargin(DataB, DataA, 1, &FadderError0);

	FmulError0.In0 = FadderError0.Out;
	FmulError0.In1 = TFadderError0.Out;
	t  = fmulMargin(DataA, nd, &FmulError0);
	tComp = ((t>>16)&0x7f) - 0x3f;//dk
	if(tComp <= FmulError0.Out)//dk
		return;//dk
	Hit->significantFigure = FmulError0.Out;//dk
	//////////////////////////////////////////////////////////////////
	//FmulError1.In0 = FmulError0.Out;//t Margin
	FmulError1.In0 = (t>>16&0x7f)-0x4f;
	FmulError1.In1 = (ray->d[u]>>16&0x7f) - 0x4e;
	DataA = fmulMargin(t, ray->d[u], &FmulError1);

	FadderError0.In0 = (ray->o[u]>>16&0x7f) - 0x4f;
	FadderError0.In1 = FmulError1.Out;
	hu = fadderMargin(ray->o[u], DataA, 0, &FadderError0);
	//hu = fadder(ray->o[u], fmul(t, ray->d[u]), 0);

	//FmulError1.In0 = FmulError0.Out;//t Margin
	FmulError1.In0 = (t>>16&0x7f)-0x4f;
	FmulError1.In1 = (ray->d[v]>>16&0x7f) - 0x4e;
	DataA = fmulMargin(t, ray->d[v], &FmulError1);

	FadderError1.In0 = (ray->o[v]>>16&0x7f) - 0x3f - 16;
	FadderError1.In1 = FmulError1.Out;
	hv = fadderMargin(ray->o[v], DataA, 0, &FadderError1);
	//hv = fadder(ray->o[v], fmul(t, ray->d[v]), 0);
	////////////////////////////////////////////////////////////////// 
	{
		RCVECTOR3u	CenterPos	= g_mem.PrimitiveCenterPos(PrimNum);
		FmulError0.In0 = FadderError0.Out;
		FmulError0.In1 = (CenterPos.x>>16&0x7f) - 0x4f;
		Bhu = fmulMargin(hu, CenterPos.x, &FmulError0);

		FmulError1.In0 = FadderError1.Out;
		FmulError1.In1 = (CenterPos.y>>16&0x7f) - 0x4f;
		Bhv = fmulMargin(hv, CenterPos.y, &FmulError1);

		TFadderError0.In0 = 0;//pPrimitive.Margin[0];
		TFadderError0.In1 = FmulError0.Out;
		TFadderError0.In2 = FmulError1.Out;   
		beta = TFAdderMargin(CenterPos.z, Bhu, Bhv, &TFadderError0);	
	}

	betaComp = ((beta>>16)&0x7f)-0x3f;
	if(beta&0x800000){
		if(betaComp>TFadderError0.Out)
			return;//no hit
	}

	if((beta&0x800000)==0)
		Hit->HitStatus++;
	//////////////////////////////////////////////////////////////////
	{
		RCVECTOR3u cn	= g_mem.PrimitiveCNuvd(PrimNum);
		FmulError0.In0 = FadderError0.Out;
		FmulError0.In1 = (cn.x>>16&0x7f) - 0x4f;
		Ghu = fmulMargin(hu, cn.x, &FmulError0);

		FmulError1.In0 = FadderError1.Out;
		FmulError1.In1 = (cn.y>>16&0x7f) - 0x4f;
		Ghv = fmulMargin(hv, cn.y, &FmulError1);

		TFadderError1.In0 = 0;//pPrimitive.Margin[1];
		TFadderError1.In1 = FmulError0.Out;
		TFadderError1.In2 = FmulError1.Out;   
		gamma = TFAdderMargin(cn.z, Ghu, Ghv, &TFadderError1);
	}
	gammaComp = ((gamma>>16)&0x7f)-0x3f;
	if(gamma&0x800000){
		if(gammaComp>TFadderError1.Out)
			return;
	}

	if((gamma&0x800000)==0)
		Hit->HitStatus++;

	//////////////////////////////////////////////////////////////////
	FadderError0.In0 = TFadderError0.Out;
	FadderError0.In1 = TFadderError1.Out;

	temp= FadderError0.In0+0x3f+16-((beta>>16)&0x7f);
	temp1= FadderError0.In1+0x3f+16-((gamma>>16)&0x7f);

	//BetaPluseGamma = fadderMargin((beta>>temp)<<temp,(gamma>>temp1)<<temp1,0, &FadderError0); 
	BetaPluseGamma = fadderMargin(beta, gamma, 0, &FadderError0);

	bgComp =((BetaPluseGamma>>16)&0x7f)-0x3f;
	if(bgComp==0){
		temp= FadderError0.Out+0x4f-((BetaPluseGamma>>16)&0x7f);
		if(temp < 0)
			return;

		temp=temp&0xf;
		bgComp=compare(((BetaPluseGamma>>temp)<<temp)&0x7fffff, 0x3f0000);
		if (bgComp == 1) return; //Beta+Gamma > 1
	}
	else if(bgComp > 0)
		return;
	bgComp=compare(BetaPluseGamma, 0x3f0000);
	if (bgComp != 1) 
		Hit->HitStatus++;
#else
	InvNd = TFAdder(ray->d[k], fmul(pPrimitive->triangle.nu, ray->d[u]), fmul(pPrimitive->triangle.nv, ray->d[v]));

	if(InvNd == 0) return;
	nd = fdivider(0x3f0000,InvNd);

	t  = fmul(fadder(fadder(pPrimitive->triangle.nd, ray->o[k], 1),  fadder(fmul(pPrimitive->triangle.nu,ray->o[u]), fmul(pPrimitive->triangle.nv,ray->o[v]), 0), 1), nd);

	hu = fadder(ray->o[u], fmul(t, ray->d[u]), 0);
	hv = fadder(ray->o[v], fmul(t, ray->d[v]), 0);
	beta = TFAdder(fmul(hu, pPrimitive->triangle.bnu), fmul(hv, pPrimitive->triangle.bnv), pPrimitive->triangle.bd);
	betaComp=compare(beta, 0xb80624);
	if (betaComp==2) return;
	if((beta&0x800000)==0)
		Hit->HitStatus++;
	{
		RCVECTOR3u cn	= g_mem.PrimitiveCNuvd(PrimNum);
		gamma = TFAdder(fmul(hu, cn.x), fmul(hv, cn.y), cn.z);
	}
	gammaComp=compare(gamma, 0xb80624);
	if (gammaComp==2) return;
	if((gamma&0x800000)==0)
		Hit->HitStatus++;

	BetaPluseGamma = fadder(beta, gamma, 0);
	bgComp=compare(BetaPluseGamma, 0x3f0000);
	if (bgComp == 1) 
		return;
	if (bgComp != 1) 
		Hit->HitStatus++;
#endif

	Hit->t = t;
	Hit->beta=beta;
	Hit->gamma=gamma;
}

void disk_intersection(int PrimNum, _ray *ray, _hit* Hit){
	int modulo[5]={0,1,2,0,1};
	int k,u,v;
	DDK_PRIMITIVE* pPrimitive	= &g_mem.pPrimitive[PrimNum];

	RayFloat nd, InvNd, t;
	RayFloat P[3], CP[3];
	RayFloat Dist;
	int HitComp;

	k=pPrimitive->triangle.axis;
	u=modulo[k+1];
	v=modulo[k+2];

	Hit->t = -1;
	Hit->HitStatus=0;

	InvNd = TFAdder(ray->d[k], fmul(pPrimitive->triangle.nu,ray->d[u]), fmul(pPrimitive->triangle.nv,ray->d[v]));
	if(InvNd == 0) return;
	nd = fdivider(0x3f0000,InvNd);
	t  = fmul(fadder(fadder(((DWORD)pPrimitive->triangle.nd_h<<16 | pPrimitive->triangle.nd_l), ray->o[k], 1),  fadder(fmul(pPrimitive->triangle.nu,ray->o[u]), fmul(pPrimitive->triangle.nv,ray->o[v]), 0), 1), nd);

	P[0] = fadder(ray->o[0], fmul(t, ray->d[0]), 0);
	P[1] = fadder(ray->o[1], fmul(t, ray->d[1]), 0);
	P[2] = fadder(ray->o[2], fmul(t, ray->d[2]), 0);
	{
		RCVECTOR3u CenterPos	= g_mem.PrimitiveCenterPos(PrimNum);
		CP[0] = fadder(CenterPos.x,P[0],1);
		CP[1] = fadder(CenterPos.y,P[1],1);
		CP[2] = fadder(CenterPos.z,P[2],1);
	}

	Dist = TFAdder(fmul(CP[0],CP[0]), fmul(CP[1],CP[1]), fmul(CP[2],CP[2]));
	HitComp = compare(Dist, pPrimitive->triangle.cnu);	// radius
	if(HitComp==1)
		return;

	Hit->t = t;
	Hit->beta=0;
	Hit->gamma=0;
}

void sphere_intersection(int PrimNum, _ray *ray, _hit* Hit){
	DDK_PRIMITIVE* pPrimitive	= &g_mem.pPrimitive[PrimNum];

	RayFloat A[3];
	RayFloat DdotA, DoubleA, DisCri;
	int mode, HitComp;
	RayFloat t;

	Hit->t = -1;
	Hit->HitStatus=0;

	{
		RCVECTOR3u CenterPos	= g_mem.PrimitiveCenterPos(PrimNum);
		A[0]= fadder(CenterPos.x, ray->o[0],1);
		A[1]= fadder(CenterPos.y, ray->o[1],1);
		A[2]= fadder(CenterPos.z, ray->o[2],1);
	}

	DdotA = TFAdder(fmul(ray->d[0],A[0]),fmul(ray->d[1],A[1]),fmul(ray->d[2],A[2]));
	DoubleA = TFAdder(fmul(A[0],A[0]),fmul(A[1],A[1]),fmul(A[2],A[2]));

	DisCri = TFAdder(fmul(DdotA,DdotA), SignConv(DoubleA),pPrimitive->triangle.cnu);	// radius

	HitComp = compare(DisCri, 0);
	if(HitComp==2)
		return;

	HitComp = compare(DoubleA, pPrimitive->triangle.cnu);	// radius
	if(HitComp==1)
		mode = 1;
	else 
		mode = 0;

	t= fadder(DdotA, SQRT(DisCri),mode);

	Hit->t = t;
	Hit->beta=0;
	Hit->gamma=0;
}

int NodeTraversal(RayFloat ro, RayFloat rd, DDK_NODE node, RayFloat *tmin, RayFloat *tmax, RayFloat PrimDist, int *NodeAddr, struct KdStack *Stack){
	RayFloat split, tplane;
	int firstChild, secondChild;
	int firstEmpty, secondEmpty;
	int LeftEmpty, RightEmpty;
	int comp1, comp2, comp3, comp4, comp5;
	int StackPush=0;
	RayFloat node_index	= (((DWORD)node.index_h)<<8) | node.index_l;

	split = node.number;
	tplane=fmul(fadder( split, ro, 1),rd);

	if((node.axis>>2)) // 1 : left child node is empty node
		LeftEmpty = 1;
	else
		LeftEmpty = 0;

	if(node_index==0xffffff) // if all bits of right child is 1, right child node is empty node
		RightEmpty = 1;
	else
		RightEmpty =0;

	comp1 = compare(ro, split);
	comp2 = compare(rd, 0);

	if(comp1==2){//ro<split
		firstChild = *NodeAddr + 1;
		secondChild = node_index;
		firstEmpty = LeftEmpty;
		secondEmpty = RightEmpty;
	}
	else if(comp1==1){//ro>split
		firstChild = node_index;
		secondChild = *NodeAddr + 1;
		firstEmpty = RightEmpty;
		secondEmpty = LeftEmpty;
	}
	//ro==split
	else if(comp2==2){
		firstChild = *NodeAddr + 1;
		secondChild = node_index;
		firstEmpty = LeftEmpty;
		secondEmpty = RightEmpty;
	}
	else if(comp2==1){//Rigth First
		firstChild = node_index;
		secondChild = *NodeAddr + 1;
		firstEmpty = RightEmpty;
		secondEmpty = LeftEmpty;
	}
	else{//Left First
		firstChild = *NodeAddr + 1;
		secondChild = node_index;
		firstEmpty = LeftEmpty;
		secondEmpty = RightEmpty;
	}
	// Advance to next child node, possibly enqueue other child
	comp1 = compare(tplane, TmaxGap1); //x>y : 1
	comp2 = compare(tplane, 0);
	comp3 = compare(tplane, TminGap1);
	//comp3 = compare(tplane, *tmin);
	comp4 = compare(tplane, PrimDist);
	//add
	comp5 = compare(tplane, *tmin);
	//comp5=2;

	if (comp1==1 || comp2!=1){//only First
		if(firstEmpty)
			StackPush=-1;
		else 
			*NodeAddr = firstChild;
	}
	else if (comp3==2){//only Second
		if(secondEmpty)
			StackPush=-1;
		if(StackPush!=-1)
			*NodeAddr = secondChild;
	}
	else {  //First & Second
		if(firstEmpty){ 
			*NodeAddr = secondChild;
			if(comp5==1)
				*tmin = tplane;
			if(comp4!=2)  //Tree Out
				StackPush = -1;
		}
		else if(secondEmpty){
			*NodeAddr = firstChild;
			*tmax = tplane;
		}
		else{
			Stack->nodeAddress = secondChild;
			//add
			if(comp5==1)
				Stack->tmin = tplane;
			else
				Stack->tmin = *tmin;
			Stack->tmax = *tmax;
			Stack->flag=0;
			StackPush=1;
			*NodeAddr = firstChild;
			*tmax = tplane;
			if(comp4!=2)  
				StackPush = 0;
		}
	}
	return StackPush;
}

void traversal(_TnIInput *InData, _TnIOutput *OutData){
	struct KdStack Stack[TravStackSize];
	struct KdStack StackData;
	int StackPos, StackPosGroup;
	int StackPop, StackPush;

	DDK_NODE node;
	KdTriangle triangle;
	int NodeAddr, ListAddr, PrimNum, PreNodeAddr, Second; //dk Second는 debugging용임
	int BoxHit, Mode, GroupMode;
	int EndTraversal, i, axis;

	RayFloat SectionMinDist, SectionMaxDist, PrimDist, CurrMaxDist, InitMinDist;
	RayFloat BoxMinDist, BoxMaxDist, CurrPrimDist;
#ifdef USE_REDUCED_ERROR_CORRECTION
	int currentSignificantFigure;//dk 130524 added
#endif
	_hit hit_triangle;
	int HitTemp;
	int comp, HitStatus;
	unsigned int RayKind;
	//initialize
	EndTraversal = 0;
	Mode = 0;
	Second = 0;
	HitStatus = 0;
	if((!g_reg.option.set.shadow)||*InData->valid_traversal){
		InData->ray->dielectric[0] =f32f24(1.f);
		InData->ray->dielectric[1] =f32f24(1.f);
		InData->ray->dielectric[2] =f32f24(1.f);
	}
	else{
		InData->ray->dielectric[0]=0;
		InData->ray->dielectric[1]=0;
		InData->ray->dielectric[2]=0;
	}

	StackPos		= 0;
	StackPosGroup	= -1;
	StackPop		= 0;
	StackPush		= 0;
	GroupMode		= 0;   
	OutData->hit->prim_num=0;
	PrimNum			= g_reg.ast.def_list_num;
	ListAddr		= g_reg.ast.def_list_addr;
	i=0;
	HitTemp=0;
	OutData->hit->hit_result=0;
	CurrPrimDist = 0;
#ifdef USE_REDUCED_ERROR_CORRECTION
	currentSignificantFigure = 0;//dk 130524 added
#endif

	if(*InData->valid_traversal==0){
		return;
	}
	if (*InData->RayKind == RAY_TYPE_TEXTURE_SHADOW){
		SectionMinDist = *OutData->ShadowRayMinDist+6;//dk tminGap에서 빼는 수보다 커야함
		InitMinDist = *OutData->ShadowRayMinDist+6;
	}
	else {
#ifdef USE_REDUCED_ERROR_CORRECTION
		SectionMinDist	= 0x250000;   //dk 0x370000, 0x300000, 0x250000 ok
		InitMinDist		= 0x250000;
#else
		SectionMinDist	= 0x330000;
		InitMinDist		= 0x330000;
#endif
	}
	if((*InData->RayKind == RAY_TYPE_TEXTURE_SHADOW)||(*InData->RayKind == RAY_TYPE_SHADOW))
		RayKind = RAY_TYPE_SHADOW;
	else
		RayKind = *InData->RayKind;

	if (RayKind == RAY_TYPE_SHADOW) {
		SectionMaxDist = InData->ray->length; 
		PrimDist = InData->ray->length; 
	}
	else {
		SectionMaxDist = InitTmax;
		PrimDist = InitTmax;
	}
	CurrMaxDist = InitTmax;
	BoxHit = 0;

	while(EndTraversal!=1){
		switch(Mode){
			case 0 : {
				if(RayKind!=RAY_TYPE_SHADOW)
					g_CountInfo.RayBoxCount++;
				else
					g_CountInfo.ShadowBoxCount++;

				
				BoxHit=box_intersection((RayFloat*)g_reg.ast.bound_box.min.m, (RayFloat*)g_reg.ast.bound_box.max.m, InData->ray, CurrMaxDist, &BoxMinDist, &BoxMaxDist);  
				if(BoxHit){
					StackPush=1;
					Mode = 2;
					StackData.flag=1;
					StackData.NextListAddress=ListAddr+1;
					StackData.tmax=SectionMaxDist;
					StackData.tmin=SectionMinDist;
					StackData.BoxField.UpperGroutHit=OutData->hit->hit_result;
					StackData.BoxField.UpperGroutHitTemp=HitTemp;
					StackData.BoxField.StackRtrPtr = StackPosGroup;
					StackData.BoxField.RemindCount = PrimNum-(i+1);
					StackPosGroup=StackPos;
					
					NodeAddr = g_reg.ast.bound_box.index&0x1fffff;
					node = g_mem.pNode[NodeAddr];
					HitTemp=0;

					comp=compare(SectionMinDist,BoxMinDist);  //x>y : 1
					if(comp==2)
						SectionMinDist = BoxMinDist;
					comp=compare(SectionMaxDist,BoxMaxDist);
					if(comp==1)
						SectionMaxDist = BoxMaxDist;
				}
				else{
					Mode = 3;
					i++;
					ListAddr++;
				}
				break;
			} 
			case 1 : {
				g_CountInfo.TriCount++;
				if(RayKind!=RAY_TYPE_SHADOW)
					g_CountInfo.RayTriCount++;
				else
					g_CountInfo.ShadowTriCount++;

				// 항상 triangle 로 시전
//				if(RtPrimitive[g_mem.pList[ListAddr].TriangleIndex].Type==0)
					tri_intersectionOpt(g_mem.pList[ListAddr].TriangleIndex, InData->ray, &hit_triangle);
// 				else if(RtPrimitive[g_mem.pList[ListAddr].TriangleIndex].Type==2)
// 					disk_intersection(g_mem.pList[ListAddr].TriangleIndex, InData->ray, &hit_triangle);
// 				else if(RtPrimitive[g_mem.pList[ListAddr].TriangleIndex].Type==3)
// 					sphere_intersection(g_mem.pList[ListAddr].TriangleIndex, InData->ray, &hit_triangle);

				comp = compare(hit_triangle.t,InitMinDist);//dk 130514 test
				//comp = compare(hit_triangle.t,TminGap);//dk 130514 test
				if(RayKind != 1 && (*InData->SIT_primNum == g_mem.pList[ListAddr].TriangleIndex)){//dk 130604 SIT modified
					comp = 2;
					//g_CountInfo.SITCount++;
				}
				else
					//g_CountInfo.SITnoCount++;
				if(comp!=2){  /////////////Primitive Hit
					int minComp, maxComp;
					if(RayKind==RAY_TYPE_SHADOW){
						maxComp=compare(hit_triangle.t, InData->ray->length);
						if(maxComp!=1){//Hit
						//if(maxComp!=1&&triangle.Flag!=1){//Hit
							RC_TRANSMITTANCE_DATA* pTransmit	= &g_mem.pTransmittance[triangle.material_index];
							if(!pTransmit->r && !pTransmit->g && pTransmit->b){
								InData->ray->dielectric[0]	= 0;
								InData->ray->dielectric[1]	= 0;
								InData->ray->dielectric[2]	= 0;
								OutData->hit->hit_result	= 0;
								EndTraversal = 1;
							}
							g_CountInfo.SITCount++;//dk 130503
						}
						else
							g_CountInfo.SITnoCount++;//dk 130503

						if(OutData->hit->hit_result)
							maxComp=compare(hit_triangle.t, PrimDist);
						else
							maxComp=compare(hit_triangle.t, InData->ray->length);

						if(maxComp!=1){//Hit   
							/*if(triangle.Flag==1){// texture flag
								maxComp=compare(hit_triangle.t, PrimDist);
								if(maxComp==2){
									PrimDist = hit_triangle.t;
									OutData->hit->hit_result = 1;
									OutData->hit->t = PrimDist;
									OutData->hit->prim_num = g_mem.pList[ListAddr].TriangleIndex;
									OutData->hit->beta=hit_triangle.beta;
									OutData->hit->gamma=hit_triangle.gamma;
									HitStatus = hit_triangle.HitStatus;
#ifdef USE_REDUCED_ERROR_CORRECTION
									OutData->hit->significantFigure = hit_triangle.significantFigure;//dk
#endif
								}
								else if(maxComp==0){
									if(hit_triangle.HitStatus>HitStatus){
										PrimDist = hit_triangle.t;
										CurrPrimDist = PrimDist;
#ifdef USE_REDUCED_ERROR_CORRECTION
										currentSignificantFigure = hit_triangle.significantFigure;//dk 130524 added
#endif
										OutData->hit->hit_result = 1;
										OutData->hit->t = PrimDist;
										OutData->hit->prim_num = g_mem.pList[ListAddr].TriangleIndex;
										OutData->hit->beta=hit_triangle.beta;
										OutData->hit->gamma=hit_triangle.gamma;
										HitStatus = hit_triangle.HitStatus;
#ifdef USE_REDUCED_ERROR_CORRECTION
										OutData->hit->significantFigure = hit_triangle.significantFigure;//dk
#endif
									}
								}
							}
							else*/
							{
								minComp=compare(hit_triangle.t, TminGap);
								maxComp=compare(hit_triangle.t, TmaxGap);
								if((minComp!=2)&&(maxComp==2)){  //Section Hit
#ifdef USE_REDUCED_ERROR_CORRECTION//dk 130524 added
									int temp;
									int SFG;//dk 130524 significant figure gap
									temp = CurrPrimDist - hit_triangle.t;
									if(temp < 0)
										temp = -temp;
									SFG = currentSignificantFigure - hit_triangle.significantFigure;
									if(SFG < 0)
										SFG = -SFG;
									temp = temp>>SFG<<SFG;
									if(CurrPrimDist == 0 || temp > 15){
#else
									if(CurrPrimDist!=hit_triangle.t){
#endif
										CurrPrimDist = hit_triangle.t;
#ifdef USE_REDUCED_ERROR_CORRECTION
										currentSignificantFigure = hit_triangle.significantFigure;//dk 130524 added
#endif
										{
											RC_TRANSMITTANCE_DATA* pTransmit	= &g_mem.pTransmittance[triangle.material_index];
											DWORD r	= ((DWORD)pTransmit->r)<<4;
											DWORD g	= ((DWORD)pTransmit->g)<<4;
											DWORD b	= ((DWORD)pTransmit->b)<<4;
											if(r) r	|= 0x300000;
											if(g) g	|= 0x300000;
											if(b) b	|= 0x300000;
											InData->ray->dielectric[0]=fmul(InData->ray->dielectric[0],r);
											InData->ray->dielectric[1]=fmul(InData->ray->dielectric[1],g);
											InData->ray->dielectric[2]=fmul(InData->ray->dielectric[2],b);
										}
									}
								}
							}
						}
					}
					else{
						maxComp=compare(hit_triangle.t, PrimDist);
						if(maxComp==2){
							PrimDist = hit_triangle.t;
							OutData->hit->hit_result = 1;
							OutData->hit->t = PrimDist;
							OutData->hit->prim_num = g_mem.pList[ListAddr].TriangleIndex;
							OutData->hit->beta=hit_triangle.beta;
							OutData->hit->gamma=hit_triangle.gamma;
							HitStatus = hit_triangle.HitStatus;
#ifdef USE_REDUCED_ERROR_CORRECTION
							OutData->hit->significantFigure = hit_triangle.significantFigure;//dk
#endif
						}
						else if(maxComp==0){
							if(hit_triangle.HitStatus>HitStatus){
								PrimDist = hit_triangle.t;
								CurrPrimDist = PrimDist;
#ifdef USE_REDUCED_ERROR_CORRECTION
								currentSignificantFigure = hit_triangle.significantFigure;//dk 130524 added
#endif
								OutData->hit->hit_result = 1;
								OutData->hit->t = PrimDist;
								OutData->hit->prim_num = g_mem.pList[ListAddr].TriangleIndex;
								OutData->hit->beta=hit_triangle.beta;
								OutData->hit->gamma=hit_triangle.gamma;
								HitStatus = hit_triangle.HitStatus;
#ifdef USE_REDUCED_ERROR_CORRECTION
								OutData->hit->significantFigure = hit_triangle.significantFigure;//dk
#endif
							}
						}
					}
				}

				Mode=3;
				i++;
				ListAddr++;
				break;
			}
			case 2 : {
				axis = node.axis;
				g_CountInfo.NodeCount++;
				//if((*InData->RayKind!=SHADOW_RAY)&&(*InData->RayKind!=SHADOW_RAY))
				if(RayKind!=RAY_TYPE_SHADOW)
					g_CountInfo.RayNodeCount++;
				else
					g_CountInfo.ShadowNodeCount++;

				if(axis !=3){
					PreNodeAddr=NodeAddr;
					StackPush = NodeTraversal(InData->ray->o[axis], InData->ray->inv_d[axis], node, &SectionMinDist, &SectionMaxDist, PrimDist, &NodeAddr, &StackData);
					node= g_mem.pNode[NodeAddr];               

					if(StackPush==-1){
						StackPush = 0;
						StackPop = 1;
					}
					else if(Second==0){
						if(((PreNodeAddr+1)==NodeAddr)&&((PreNodeAddr&3)!=3)){
							Second=1;
						}
					}
					else 
						Second=0;
				}
				else {
					Mode=3;
					ListAddr=(((DWORD)node.index_h)<<8) | node.index_l;
					PrimNum=node.number;
					i=0;
					//CurrPrimDist= 0;//dk 130415 deleted
				}
				comp=compare(PrimDist,SectionMinDist);
				if(comp!=1){
					Mode=5;
				}
				break;
			}
			case 3 : {
				Second=0;
				if(i<PrimNum){
					g_CountInfo.ListCount++;
					triangle = g_mem.pPrimitiveInfo[g_mem.pList[ListAddr].TriangleIndex];

					if(triangle.fGroup == 1){
						Mode = 4;
					}
					else{
						Mode = 1;
					}
				}
				else {
					StackPop=1;
				}
				break;
			}
			case 4 : {
				comp=compare(PrimDist,TmaxGap);    //x>y : 1           
				CurrMaxDist=(comp==1)?TmaxGap:PrimDist;
				if(RayKind!=RAY_TYPE_SHADOW)
					g_CountInfo.RayBoxCount++;
				else
					g_CountInfo.ShadowBoxCount++;

				BoxHit = box_intersection(triangle.vertex[0].position, triangle.vertex[1].position, InData->ray, CurrMaxDist, &BoxMinDist, &BoxMaxDist);
            
				if(BoxHit){
					StackPush=1;
					Mode = 2;
					StackData.flag=1;
					StackData.NextListAddress=ListAddr+1;
					StackData.tmax=SectionMaxDist;
					StackData.tmin=SectionMinDist;
					StackData.BoxField.UpperGroutHit=OutData->hit->hit_result;
					StackData.BoxField.UpperGroutHitTemp=HitTemp;
					StackData.BoxField.StackRtrPtr = StackPosGroup;
					StackData.BoxField.RemindCount = PrimNum-(i+1);
					StackPosGroup=StackPos;
               
					NodeAddr = triangle.material_index&0x1fffff;
					node = g_mem.pNode[NodeAddr];
               
					comp=compare(SectionMinDist,BoxMinDist);
					if(comp==2)
						SectionMinDist = BoxMinDist;
					comp=compare(SectionMaxDist,BoxMaxDist);
					if(comp==1)
						SectionMaxDist = BoxMaxDist;
				}
				else{
					Mode = 3;
					i++;
					ListAddr++;
				}            
				break;
			}
			case 5 : {
				Second=0;
				EndTraversal = 1;
				StackPop=1;
			}
		}//switch

		if(StackPush){
			StackPush=0;
			Stack[StackPos]=StackData;
			++StackPos;
		}

		if(StackPop){
			Second=0;
			StackPop=0;
			if(StackPos>0){
				StackPos--;
				if(EndTraversal&&((StackPosGroup&0x3f)!=0x3f)){
					StackPos=StackPosGroup;
					EndTraversal = 0;
					Mode=3;
					ListAddr = Stack[StackPosGroup].NextListAddress;
					SectionMaxDist = Stack[StackPosGroup].tmax;
					SectionMinDist = Stack[StackPosGroup].tmin;
					OutData->hit->hit_result= OutData->hit->hit_result|Stack[StackPosGroup].BoxField.UpperGroutHit;
					HitTemp = HitTemp|Stack[StackPosGroup].BoxField.UpperGroutHitTemp;
					PrimNum = Stack[StackPosGroup].BoxField.RemindCount;
					StackPosGroup = Stack[StackPosGroup].BoxField.StackRtrPtr;
					i=0;
				}
				else if(EndTraversal){
				}
				else if(Stack[StackPos].flag){  //miss
					EndTraversal = 0;
					Mode=3;
					ListAddr = Stack[StackPos].NextListAddress;
					OutData->hit->hit_result= HitTemp|OutData->hit->hit_result|Stack[StackPos].BoxField.UpperGroutHit;
					HitTemp = Stack[StackPosGroup].BoxField.UpperGroutHitTemp;
					PrimNum = Stack[StackPos].BoxField.RemindCount;
					SectionMaxDist = Stack[StackPos].tmax;
					SectionMinDist = Stack[StackPos].tmin;
					i=0;
					StackPosGroup = Stack[StackPos].BoxField.StackRtrPtr;
				}
				else {
					Mode=2;
					NodeAddr = Stack[StackPos].nodeAddress;
					node = g_mem.pNode[NodeAddr];
					SectionMinDist = Stack[StackPos].tmin;
					SectionMaxDist = Stack[StackPos].tmax;
					comp=compare(PrimDist,SectionMinDist);
					if(comp!=1)
						Mode=5;
				}
			}
			else {
				EndTraversal = 1;
			}
		}
	}//while
}
