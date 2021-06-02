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
// DATE    : 4/29/2013 Mon
// Author  : Sang Gon, Kim (sgkim@siliconarts.co.kr)
//
//=======================================================================

#include "rcContext1.h"

#define PI_OVER_360	 0.0087266462599716478846184538424431	// = PI / 360

void ContextRC1::Perspective ( RCfloat fovy, RCfloat aspect, RCfloat zNear, RCfloat zFar){
	float ymax;
	float xmax;    

	if(zNear <= 0 || zFar <= 0 || zNear == zFar){
		SetError(RC_INVALID_VALUE);
		return;
	}

	ymax = 2.0f*(float) (zNear * tan(fovy * PI_OVER_360));
	xmax = ymax * aspect;

	m_Screen.info.pixelSizeX   = xmax/m_Screen.Width;	// @todo : Width의 설정 순서의 의존성이 발생된다.
	m_Screen.info.pixelSizeY   = ymax/m_Screen.Height;	// @todo : Height의 설정 순서의 의존성이 발생된다.

	m_Screen.info.startPositionX = -xmax/2.0f;
	m_Screen.info.startPositionY = -ymax/2.0f;

	//    Distance = zNear;
	m_Screen.RayCamera.distance = zNear; //???????
}

void ContextRC1::LookAt (	RCfloat eyex, RCfloat eyey, RCfloat eyez,
				RCfloat centerx, RCfloat centery, RCfloat centerz,
				RCfloat upx, RCfloat upy, RCfloat upz){

	int i;
	
	float pos[3];
	float up[3];
	float look[3];
	double a[3];


	pos[0] = eyex;
	pos[1] = eyey;
	pos[2] = eyez;

	up[0] = upx;
	up[1] = upy;
	up[2] = upz;

	look[0] = centerx;
	look[1] = centery;
	look[2] = centerz;

	LOOP(3) {
		m_Screen.RayCamera.eye.m[i]			= pos[i];
		m_Screen.RayCamera.basis[1].m[i]	= up[i];
		m_Screen.RayCamera.basis[2].m[i]	= m_Screen.RayCamera.eye.m[i] - look[i];
	}

	NormalizeV(m_Screen.RayCamera.basis[2].m);

	VECTOR3_OPERATION(a, pos, SUB, look);

	m_Screen.RayCamera.basis[0].m[0] = (float)(( up[1] * a[2] ) - ( up[2] * a[1] ));
	m_Screen.RayCamera.basis[0].m[1] = (float)(( up[2] * a[0] ) - ( up[0] * a[2] ));
	m_Screen.RayCamera.basis[0].m[2] = (float)(( up[0] * a[1] ) - ( up[1] * a[0] ));

	NormalizeV(m_Screen.RayCamera.basis[0].m);
	NormalizeV(m_Screen.RayCamera.basis[1].m);
}

MATRIX_PALETTE* ContextRC1::GetMatrixPaletteIndex(RCuint index) {
	if(index > RCDDK_MAX_MATRIX_PALETTE_COUNT - 1)
		return NULL;

	return &m_MatrixArray[index];
}

void ContextRC1::SetMatrixPaletteIndex(RCuint index, const float *m){
	MATRIX_PALETTE *mtx=GetMatrixPaletteIndex(index);

	if(!mtx){
		SetError(RC_INVALID_VALUE);
		return;
	}

	memcpy(mtx->matrix.m, m, SIZEOFIDENTITYMATRIX);
}

void ContextRC1::CalculateMatrixPalette(int index){
	int i, n;
	RCubyte *vpt_bm;
	RCuint *vpt_im;
	RCfloat *vpt_w;
	MATRIX_PALETTE *mtx=NULL;
	float *pData=&m_MatrixPalette.matrix.m[0];

	memset(pData, 0, sizeof(float)*16);
	for (n = 0; n < m_MatrixIndexArray.size; n++) {
		mtx = NULL;
		switch (m_MatrixIndexArray.type) {
		case RC_UNSIGNED_BYTE:
			vpt_bm = (RCubyte *) ((unsigned char *)m_MatrixIndexArray.pData + (m_MatrixIndexArray.stride * index) + sizeof(RCubyte)*n);
			mtx = GetMatrixPaletteIndex((RCuint)*vpt_bm);
			break;
		case RC_UNSIGNED_INT:
			vpt_im = (RCuint *) ((unsigned char *)m_MatrixIndexArray.pData + (m_MatrixIndexArray.stride * index) + sizeof(RCuint)*n);
			mtx = GetMatrixPaletteIndex((RCuint)*vpt_im);
			break;
		}
		if(!mtx) continue;
		vpt_w = (RCfloat *) ((unsigned char *)m_WeightArray.pData + (m_WeightArray.stride * index) + sizeof(RCfloat)*n);

		LOOP(16) pData[i] += (mtx->matrix.m[i] * (*vpt_w));
	}

	//Calculate the inverse matrix of matrix palette
	InverseMatrix(m_MatrixPalette.matrix.m, m_MatrixPalette.matrixInv.m);
}

void ContextRC1::CurrentPaletteMatrixOES (RCuint matrixpaletteindex){
	
	if(!m_CurrentStateFlag.etc.matrix_palette){
		SetError(RC_INVALID_OPERATION);
		return;
	}

	if(matrixpaletteindex > RCDDK_MAX_MATRIX_PALETTE_COUNT - 1){
		SetError(RC_INVALID_VALUE);
		return;
	}

	m_iMatrixArrayIndex = matrixpaletteindex;
}

void ContextRC1::LoadPaletteFromModelViewMatrixOES (void){
	MATRIX_PALETTE *mtx=GetMatrixPaletteIndex(m_iMatrixArrayIndex);

	if(!m_CurrentStateFlag.etc.matrix_palette || m_iMatrixArrayIndex < 0 || !mtx){
		SetError(RC_INVALID_OPERATION);
		return;
	}

	memcpy(mtx->matrix.m, m_MatrixModelToWorld.m, SIZEOFIDENTITYMATRIX);
}

void ContextRC1::MatrixIndexPointerOES(RCint size, RCenum type, RCsizei stride, const RCvoid *pointer){

	if(!m_CurrentStateFlag.etc.matrix_palette) {
		SetError(RC_INVALID_OPERATION);
		return;
	}
	
	if (type != RC_UNSIGNED_BYTE && type != RC_UNSIGNED_INT) {
		SetError(RC_INVALID_ENUM);
		return;
	}

	if (size < 1 || size > RCDDK_MAX_VERTEX_UNITS_COUNT) {
		SetError(RC_INVALID_VALUE);
		return;
	}

	if (stride < 0) {
		SetError(RC_INVALID_VALUE);
		return;
	} else if (stride == 0) {
		switch (type) {
		case RC_UNSIGNED_BYTE:
			stride = size * sizeof (RCubyte);
			break;
		case RC_UNSIGNED_INT:
			stride = size * sizeof (RCuint);
			break;
		}
	} 
	
	m_MatrixIndexArray.size = size;	
	m_MatrixIndexArray.type = type;
	m_MatrixIndexArray.stride = stride;

	m_MatrixIndexArray.pData = NULL;
	m_MatrixIndexArray.boundBuffer = 0;
	if(m_Current.pBuffer){
		if(!m_Current.pBuffer->Get()->pValue){
			SetError(RC_INVALID_OPERATION);
			return;
		}

		if(m_Current.pBuffer->Get()->size <= *(unsigned int *)&pointer){
			SetError(RC_INVALID_VALUE);
			return;
		}

		m_MatrixIndexArray.boundBuffer = m_Current.pBuffer->Name();
		m_MatrixIndexArray.pData    = (void *) ((unsigned char *)m_Current.pBuffer->Get()->pValue + *(unsigned int *)&pointer);
	} else {
		m_MatrixIndexArray.pData = pointer;
	}
}

void ContextRC1::WeightPointerOES(RCint size, RCenum type, RCsizei stride, const RCvoid *pointer){
	
	if(!m_CurrentStateFlag.etc.matrix_palette) {
		SetError(RC_INVALID_OPERATION);
		return;
	}

	if (type != RC_FIXED && type != RC_FLOAT) {
		SetError(RC_INVALID_ENUM);
		return;
	}

	if (size < 1 || size > RCDDK_MAX_VERTEX_UNITS_COUNT) {
		SetError(RC_INVALID_VALUE);
		return;
	}

	if (stride < 0) {
		SetError(RC_INVALID_VALUE);
		return;
	} else if (stride == 0) {
		stride = size;
		switch (type) {
		case RC_FIXED:
			stride = size * sizeof (RCfixed);
			break;
		case RC_FLOAT:
			stride = size * sizeof (RCfloat);
			break;
		}
	}

	m_WeightArray.size = size;	
	m_WeightArray.type = type;
	m_WeightArray.stride = stride;

	m_WeightArray.pData = NULL;
	m_WeightArray.boundBuffer = 0;
	if(m_Current.pBuffer){
		if(!m_Current.pBuffer->Get()->pValue){
			SetError(RC_INVALID_OPERATION);
			return;
		}

		if(m_Current.pBuffer->Get()->size <= *(unsigned int *)&pointer){
			SetError(RC_INVALID_VALUE);
			return;
		}

		m_WeightArray.boundBuffer = m_Current.pBuffer->Name();
		m_WeightArray.pData    = (void *) ((unsigned char *)m_Current.pBuffer->Get()->pValue + *(unsigned int *)&pointer);		
	} else {
		m_WeightArray.pData = pointer;
	}
}

void ContextRC1::TextureAlpha(RCbyte value){
	m_TextureAlpha = value;
}

void ContextRC1::DepthBounce(RCuint value){
	m_iDepthBounce	= (value>RCDDK_MAX_DEPTH_BOUNCE) ? RCDDK_MAX_DEPTH_BOUNCE : value;
	m_pDDK->SetBoundDepth(m_iDepthBounce);
}

void ContextRC1::SetLightRadius(RCfloat radius) {
	// User inputs a floating point radius
	//m_lightRadius = min(MaxLightRadius, max(radius, MinLightRadius));
	m_lightRadius = radius;

	//m_pDDK->SetReservedValue((unsigned)m_lightRadius, 0); // light radius
	m_pDDK->SetReservedValue((unsigned)m_lightRadius, 0); // light radius
}

void ContextRC1::SetDiffuseRate(RCuint rate) {
	m_diffuseRate = rate;
	m_pDDK->SetReservedValue(m_diffuseRate, 1); // diffuse rate
}

void ContextRC1::SetSPPCount(RCuint nSpp) {
	m_sppCount = nSpp;
	m_pDDK->SetReservedValue((unsigned)m_sppCount, 2); // spp count
}
// spp