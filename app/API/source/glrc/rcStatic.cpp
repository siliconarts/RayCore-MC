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

#include <math.h>
#include <memory.h>
#include "rcContext1.h"

#define LIST_SIZE 16

void ContextRC1::RenderDataInit(RENDER_DATA *rData) {
	rData->startPrimitive  = 0;
	rData->startList       = 0;
	rData->startNode       = 0;

	rData->boundBox.rootNodeIndex = 0;

	rData->nodeCount       = 0;
	rData->listCount       = 0;
	rData->primitiveCount  = 0;
	rData->materialCount   = 0;
}

void ContextRC1::SceneAllInit(void){
	
	m_CurrentStateFlag.etc.background_set = 0;

	RenderDataInit(&m_DynamicRenderData);
	RenderDataInit(&m_StaticRenderData);

	m_dwTriangleIndex = 0;

	/************************************************************************/
	/* Current State Setting                                                */
	/************************************************************************/
	m_CurrentStateFlag.etc.background_setting = 0;

	rcBindMaterial(0);
}

void ContextRC1::StaticSceneBegin(void){
	
	m_CurrentStateFlag.etc.background_setting = 1;
	m_CurrentStateFlag.etc.background_set = 1;

	m_StaticRenderData.startList   = LIST_SIZE;

//    staticData.listStartA  += LIST_SIZE;
}

void ContextRC1::StaticSceneEnd(void){
	register RENDER_DATA *ptStaticData;
	register RENDER_DATA *ptDynamicData;

	// @todo : Multi Static Scene������ Ȯ��� FLAG_BACKGROUND_SET�� ���� ���䰡 �ʿ���
	if ( !( m_dwTriangleIndex  && m_CurrentStateFlag.etc.background_setting) )
	{
		m_CurrentStateFlag.etc.background_setting = 0;
		m_CurrentStateFlag.etc.background_set = 0;
		return;
	}

	ptStaticData    = &m_StaticRenderData;
	ptDynamicData   = &m_DynamicRenderData;

	ptStaticData->primitiveCount  = m_dwTriangleIndex;

	VECTOR3_COPY(ptStaticData->boundBox.maximum.m, m_BoundBox.maximum.m);
	VECTOR3_COPY(ptStaticData->boundBox.minimum.m, m_BoundBox.minimum.m);

	ptStaticData->endPrimitive    = m_dwTriangleIndex - 1;

	// TREE BUILDER CODE
	m_TreeBuilder.Setup(m_pDDK,
		ptStaticData->startPrimitive,
		ptStaticData->startNode,
		ptStaticData->startList);

	m_TreeBuilder.BuildAccelerationStructure( &ptStaticData->boundBox, ptStaticData->primitiveCount );

	ptStaticData->nodeCount		= m_TreeBuilder.GetNodeCount();
	ptStaticData->listCount		= m_TreeBuilder.GetListCount();
	
	if(ptStaticData->nodeCount)	m_Memory.pNode->Flush();
	if(ptStaticData->listCount) m_Memory.pList->Flush();

	// NEXT TREE DATA INIT

	ptStaticData->endList           = ptStaticData->listCount + ptStaticData->startList;

	ptDynamicData->startNode        = (ptStaticData->nodeCount      + 0x7) & 0xfffffff8;
	ptDynamicData->startList        = (ptStaticData->endList        + 0xf) & 0xfffffff0;
	ptDynamicData->startPrimitive   = (ptStaticData->primitiveCount + 0x1) & 0xfffffffe;

	ptStaticData->endNode           = ptDynamicData->startNode      - 1;
	ptStaticData->endList           = ptDynamicData->startList      - 1;
	ptStaticData->endPrimitive      = ptDynamicData->startPrimitive - 1;

	//Dynamic Scene�� ���� ��� Bound Box ����!
	{
		BOUNDING_BOX optBox;
		optBox.rootNodeIndex  = ptStaticData->boundBox.rootNodeIndex;
		VECTOR3_COPY(optBox.maximum.m, ptStaticData->boundBox.maximum.m);
		VECTOR3_COPY(optBox.minimum.m, ptStaticData->boundBox.minimum.m);

		m_pDDK->SetBoundBox(optBox.rootNodeIndex, (float*)optBox.minimum.m);
	}

	if(m_Memory.pList->IsResized())	// @todo : LoadPrimitive_Temp���� ������ ���� �����ϰ� �ִ�. �������� �Ѵ�.
		m_pDDK->SetMemory(RCDDK_MEMORY_LIST, m_Memory.pList->GetNative());

	VECTOR3_SUBSTITUTE(m_BoundBox.maximum.m, FLOAT_MIN);
	VECTOR3_SUBSTITUTE(m_BoundBox.minimum.m, FLOAT_MAX);

	ptDynamicData->boundBox.rootNodeIndex = ptDynamicData->startNode;

	memcpy( m_MatrixModelToWorld.m, 		m_afIdentity, SIZEOFIDENTITYMATRIX );
	memcpy( m_MatrixModelToWorldInverse.m,  m_afIdentity, SIZEOFIDENTITYMATRIX );
	memcpy( m_MatrixTexture.m, 				m_afIdentity, SIZEOFIDENTITYMATRIX );

	m_CurrentStateFlag.etc.matrix_modelview_inverse = 0;
	m_CurrentStateFlag.etc.background_setting = 0;
}

#ifndef CROSS_COMPILE
class PrimitiveUpdateThread : public ThreadManager{
public:
	PrimitiveUpdateThread(ContextRC1* pCtx) :
	  m_Lock(0)
	{
		m_pCtx	= pCtx;
	}
	virtual ~PrimitiveUpdateThread(void){}
	virtual void MonitorThread(void){
		m_pCtx->PrimitiveUpdate();
		m_Lock.Up();
	}
	virtual void OnThreadKill(void){
		m_Lock.Down();
	}
protected:
	Semaphore	m_Lock;
	ContextRC1*	m_pCtx;
};

void ContextRC1::PrimitiveUpdate(void){
	m_Memory.pTransmit->Flush();
	m_Memory.pPrimitive->Flush();
	m_Memory.pPrimitiveInfo->Flush();
}
#endif

void ContextRC1::DynamicSceneEnd(void){
	
	register RENDER_DATA *ptDynamicData;
	register RENDER_DATA *ptStaticData;

	RCfloat         *eyePosition = m_Screen.RayCamera.eye.m;

	ptDynamicData = &m_DynamicRenderData;
	ptStaticData  = &m_StaticRenderData;

	//StaticSceneBegine�� ȣ��� �� StaticSceneEnd�� ȣ����� �ʾ��� �� ���� ó��!
	if( m_CurrentStateFlag.etc.background_set && m_CurrentStateFlag.etc.background_setting ){
		m_CurrentStateFlag.etc.background_setting = 0;
		m_CurrentStateFlag.etc.background_set = 0;
		SetError(RC_INVALID_OPERATION);
		return;
	}
		
	//StaticSceneEnd �� DynamicScene�� �������� ���� ���
	if ( m_CurrentStateFlag.etc.background_set && m_dwTriangleIndex == ptStaticData->primitiveCount )
		return;

	VECTOR3_COPY(ptDynamicData->boundBox.maximum.m, m_BoundBox.maximum.m);
	VECTOR3_COPY(ptDynamicData->boundBox.minimum.m, m_BoundBox.minimum.m);

	ptDynamicData->endPrimitive         = m_dwTriangleIndex - 1;

	ptDynamicData->primitiveCount  = ptDynamicData->endPrimitive - ptDynamicData->startPrimitive + 1;

	ELAPSED_TIME_SET(TREE_BUILD);

	// TREE BUILDER CODE
	m_TreeBuilder.Setup(m_pDDK,
		ptDynamicData->startPrimitive,
		ptDynamicData->startNode,
		ptDynamicData->startList);
	
	{
#ifndef CROSS_COMPILE
		PrimitiveUpdateThread	PUP_Thread(this);		
		PUP_Thread.RunThread();
#endif
		m_TreeBuilder.BuildAccelerationStructure( &ptDynamicData->boundBox, ptDynamicData->primitiveCount );
		
#ifndef CROSS_COMPILE
		PUP_Thread.KillThread();
#endif
	}

	ELAPSED_TIME_END(TREE_BUILD);

	ptDynamicData->nodeCount		= m_TreeBuilder.GetNodeCount();
	ptDynamicData->listCount		= m_TreeBuilder.GetListCount();

	if(ptDynamicData->nodeCount) m_Memory.pNode->Flush();
	if(ptDynamicData->listCount) m_Memory.pList->Flush();

	// NEXT TREE DATA INIT
	ptDynamicData->endNode         = ptDynamicData->startNode + ptDynamicData->nodeCount;
	ptDynamicData->endList         = ptDynamicData->startList + ptDynamicData->listCount;

	ptDynamicData->endNode         = ((ptDynamicData->endNode        + 0x7) & 0xfffffff8) - 1;
	ptDynamicData->endList         = ((ptDynamicData->endList        + 0xf) & 0xfffffff0) - 1;
	ptDynamicData->endPrimitive    = ((m_dwTriangleIndex   + 0x1) & 0xfffffffe) - 1;

	if (m_CurrentStateFlag.etc.background_set) {
		// BackGround Set

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Box ���� ����
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		{	// For HARDWARE
			BOUNDING_BOX24  tempBox24[2];
			DDK_PRIMITIVE*	pPrimitive;
			tempBox24[0].rootNodeIndex  = ptStaticData->boundBox.rootNodeIndex;
			VECTOR3_CONVERT_24F(tempBox24[0].maximum, ptStaticData->boundBox.maximum);
			VECTOR3_CONVERT_24F(tempBox24[0].minimum, ptStaticData->boundBox.minimum);

			tempBox24[1].rootNodeIndex  = ptDynamicData->boundBox.rootNodeIndex;
			VECTOR3_CONVERT_24F(tempBox24[1].maximum, ptDynamicData->boundBox.maximum);
			VECTOR3_CONVERT_24F(tempBox24[1].minimum, ptDynamicData->boundBox.minimum);

			pPrimitive	= m_Memory.pPrimitive->GetPool(ptDynamicData->endPrimitive+1);

			pPrimitive->data[0] = PACKING(tempBox24[0].maximum.y, tempBox24[0].maximum.z, 24);
			pPrimitive->data[1] = PACKING(tempBox24[0].minimum.z, tempBox24[0].maximum.y, 16);
			pPrimitive->data[2] = PACKING(					  0x0, tempBox24[0].minimum.z,  8);
			pPrimitive->data[3] = PACKING(tempBox24[0].rootNodeIndex,                  0x0, 24);

			pPrimitive->data[4] = PACKING(tempBox24[0].minimum.y, tempBox24[0].rootNodeIndex, 16);
			pPrimitive->data[5] = PACKING(tempBox24[0].minimum.x, tempBox24[0].minimum.y,     8);
			pPrimitive->data[6] = (tempBox24[0].maximum.x);
			pPrimitive->data[7] = (1<<17)|(0<<12);

			pPrimitive	= m_Memory.pPrimitive->GetPool(ptDynamicData->endPrimitive+2);

			pPrimitive->data[0] = PACKING(tempBox24[1].maximum.y, tempBox24[1].maximum.z, 24);
			pPrimitive->data[1] = PACKING(tempBox24[1].minimum.z, tempBox24[1].maximum.y, 16);
			pPrimitive->data[2] = PACKING(                    0x0, tempBox24[1].minimum.z,  8);
			pPrimitive->data[3] = PACKING(tempBox24[1].rootNodeIndex,                  0x0, 24);

			pPrimitive->data[4] = PACKING(tempBox24[1].minimum.y, tempBox24[1].rootNodeIndex, 16);
			pPrimitive->data[5] = PACKING(tempBox24[1].minimum.x, tempBox24[1].minimum.y,     8);
			pPrimitive->data[6] = (tempBox24[1].maximum.x);
			pPrimitive->data[7] = (1<<17)|(0<<12);

			ptDynamicData->primitiveCount += 2;
		}
		{	// For Software
			RC_PRIMITIVE	*boxPrimitive = m_PrimitiveSource.Get(ptDynamicData->endPrimitive+1);

			VECTOR3_COPY(boxPrimitive[0].vertex[0].position.m, ptStaticData->boundBox.minimum.m);
			VECTOR3_COPY(boxPrimitive[0].vertex[1].position.m, ptStaticData->boundBox.maximum.m);
			boxPrimitive[0].materialIndex = ptStaticData->boundBox.rootNodeIndex;
			boxPrimitive[0].type = 1;

			VECTOR3_COPY(boxPrimitive[1].vertex[0].position.m, ptDynamicData->boundBox.minimum.m);
			VECTOR3_COPY(boxPrimitive[1].vertex[1].position.m, ptDynamicData->boundBox.maximum.m);
			boxPrimitive[1].materialIndex = ptDynamicData->boundBox.rootNodeIndex;
			boxPrimitive[1].type = 1;
		}

		{
			m_Memory.pList->GetPool(0)->TriangleIndex	= ptDynamicData->endPrimitive+1;		//Static boxPrimitive ���� 
			m_Memory.pList->GetPool(1)->TriangleIndex	= ptDynamicData->endPrimitive+2;		//Dynamic boxPrimitive ���� 
			m_Memory.pList->Flush();
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} 
	else {
		// BackGround NOT Set
		BOUNDING_BOX    optBox;
		optBox.rootNodeIndex  = ptDynamicData->boundBox.rootNodeIndex;
		VECTOR3_COPY(optBox.maximum.m, ptDynamicData->boundBox.maximum.m);
		VECTOR3_COPY(optBox.minimum.m, ptDynamicData->boundBox.minimum.m);

		m_pDDK->SetBoundBox(optBox.rootNodeIndex, (float*)optBox.minimum.m);
	}
	
	if(m_Memory.pList->IsResized())	// @todo : LoadPrimitive_Temp���� ������ ���� �����ϰ� �ִ�. �������� �Ѵ�.
		m_pDDK->SetMemory(RCDDK_MEMORY_LIST, m_Memory.pList->GetNative());

	m_dwTriangleIndex  = ptDynamicData->startPrimitive;

	VECTOR3_SUBSTITUTE(m_BoundBox.maximum.m, FLOAT_MIN);
	VECTOR3_SUBSTITUTE(m_BoundBox.minimum.m, FLOAT_MAX); 
	
	memcpy( m_MatrixModelToWorld.m, 		m_afIdentity, SIZEOFIDENTITYMATRIX );
	memcpy( m_MatrixModelToWorldInverse.m,  m_afIdentity, SIZEOFIDENTITYMATRIX );
	memcpy( m_MatrixTexture.m, 		m_afIdentity, SIZEOFIDENTITYMATRIX );

	m_CurrentStateFlag.etc.matrix_modelview_inverse = 0;
}
