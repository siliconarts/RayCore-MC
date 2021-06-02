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
// DATE    : 4/27/2013 Sat
// Author  : Sang Gon, Kim (sgkim@siliconarts.co.kr)
//
//=======================================================================

#include <math.h>
#include <memory.h>
#include "define.h"
#include "rcContext1.h"

#define A(row,col)  a[(col<<2)+row]
#define B(row,col)  b[(col<<2)+row]
#define P(row,col)  product[(col<<2)+row]

void ContextRC1::NormalVectorGenerate( float *a, float *b, float *c , float *out){
	float t;
	float v1[3];
	float v2[3];
	float v3[3];

	v1[0] = a[0] - c[0];
	v1[1] = a[1] - c[1];
	v1[2] = a[2] - c[2];

	v2[0] = b[0] - c[0];
	v2[1] = b[1] - c[1];
	v2[2] = b[2] - c[2];

	v3[0] = v1[1]*v2[2] - v1[2]*v2[1];
	v3[1] = v1[0]*v2[2] - v1[2]*v2[0];
	v3[2] = v1[0]*v2[1] - v1[1]*v2[0];

	t = (float) sqrt(v3[0]*v3[0]+v3[1]*v3[1]+v3[2]*v3[2]);

	t = 1/t;

	out[0] = v3[0]*t;
	out[1] = v3[1]*t;
	out[2] = v3[2]*t;
}

void ContextRC1::TransformVertex( float u[4], const float v[4], const float m[16] ) {
	const float v0 = v[0], v1 = v[1], v2 = v[2], v3 = 1;
#define M(row,col)  m[row + col*4]
	u[0] = v0 * M(0,0) + v1 * M(0,1) + v2 * M(0,2) + v3 * M(0,3);
	u[1] = v0 * M(1,0) + v1 * M(1,1) + v2 * M(1,2) + v3 * M(1,3);
	u[2] = v0 * M(2,0) + v1 * M(2,1) + v2 * M(2,2) + v3 * M(2,3);
	//    u[3] = v0 * M(0,3) + v1 * M(1,3) + v2 * M(2,3) + v3 * M(3,3);
#undef M
}

void ContextRC1::TransformNormal( float u[4], const float v[4], const float m[16] ) {
	const float v0 = v[0], v1 = v[1], v2 = v[2], v3 = 1;
#define M(row,col)  m[row + col*4]
	u[0] = v0 * M(0,0) + v1 * M(1,0) + v2 * M(2,0) + v3 * M(3,0);
	u[1] = v0 * M(0,1) + v1 * M(1,1) + v2 * M(2,1) + v3 * M(3,1);
	u[2] = v0 * M(0,2) + v1 * M(1,2) + v2 * M(2,2) + v3 * M(3,2);
	//    u[3] = v0 * M(0,3) + v1 * M(1,3) + v2 * M(2,3) + v3 * M(3,3);
#undef M
}

void ContextRC1::NormalizeV(float *v){
	float one_over_length = 1.f / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

	v[0] = v[0] * one_over_length;
	v[1] = v[1] * one_over_length;
	v[2] = v[2] * one_over_length;
}

void ContextRC1::MatrixCopy( float *product, float *m) {
	int i;
	for(i = 0; i< 16; i++)
		product[i] = m[i];
}

void ContextRC1::MatrixMultf( float *product, const float *a, const float *b ) {
	int i;

	for (i = 0; i < 4; i++) {
		const float ai0=A(i,0),  ai1=A(i,1),  ai2=A(i,2),  ai3=A(i,3);
		P(i,0) = ai0 * B(0,0) + ai1 * B(1,0) + ai2 * B(2,0) + ai3 * B(3,0);
		P(i,1) = ai0 * B(0,1) + ai1 * B(1,1) + ai2 * B(2,1) + ai3 * B(3,1);
		P(i,2) = ai0 * B(0,2) + ai1 * B(1,2) + ai2 * B(2,2) + ai3 * B(3,2);
		P(i,3) = ai0 * B(0,3) + ai1 * B(1,3) + ai2 * B(2,3) + ai3 * B(3,3);
	}
}

void ContextRC1::MatrixMultW( float *product, const float *a, const float w ) {
	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; i++)
			P(i, j) = A(i, j) * w;
	}
}

void ContextRC1::MatrixWeghitAccum( float *product, const float *a, const float w ) {
	float m[16];
	MatrixMultW(m, a, w);
	MatrixMultf(product, product, m);
	
	//float b[16];
	//float m[16];
	//MatrixMultW(m, a, w);
	//MatrixCopy(b, product);
	//MatrixMultf(product, b, m);
}

//////////////////////////////////////////////////////////////////////////
#define MAT(m,r,c) (m)[(c)*4+(r)]
#define SWAP_ROWS(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }

RCboolean ContextRC1::InverseMatrix( const float *m, float *inv ) {
	float *out = inv;
	float wtmp[4][8];
	float m0, m1, m2, m3, s;
	float *r0, *r1, *r2, *r3;

	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

	r0[0] = MAT(m,0,0), r0[1] = MAT(m,0,1),
		r0[2] = MAT(m,0,2), r0[3] = MAT(m,0,3),
		r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,

		r1[0] = MAT(m,1,0), r1[1] = MAT(m,1,1),
		r1[2] = MAT(m,1,2), r1[3] = MAT(m,1,3),
		r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,

		r2[0] = MAT(m,2,0), r2[1] = MAT(m,2,1),
		r2[2] = MAT(m,2,2), r2[3] = MAT(m,2,3),
		r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,

		r3[0] = MAT(m,3,0), r3[1] = MAT(m,3,1),
		r3[2] = MAT(m,3,2), r3[3] = MAT(m,3,3),
		r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

	/* choose pivot - or die */
	if (fabs(r3[0])>fabs(r2[0])) SWAP_ROWS(r3, r2);
	if (fabs(r2[0])>fabs(r1[0])) SWAP_ROWS(r2, r1);
	if (fabs(r1[0])>fabs(r0[0])) SWAP_ROWS(r1, r0);
	if (0.0 == r0[0])  return FALSE;

	/* eliminate first variable     */
	m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
	s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
	s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
	s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
	s = r0[4];
	if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r0[5];
	if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r0[6];
	if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r0[7];
	if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

	/* choose pivot - or die */
	if (fabs(r3[1])>fabs(r2[1])) SWAP_ROWS(r3, r2);
	if (fabs(r2[1])>fabs(r1[1])) SWAP_ROWS(r2, r1);
	if (0.0 == r1[1])  return FALSE;

	/* eliminate second variable */
	m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1];
	r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
	s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

	/* choose pivot - or die */
	if (fabs(r3[2])>fabs(r2[2])) SWAP_ROWS(r3, r2);
	if (0.0 == r2[2])  return FALSE;

	/* eliminate third variable */
	m3 = r3[2]/r2[2];
	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
		r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
		r3[7] -= m3 * r2[7];

	/* last check */
	if (0.0 == r3[3]) return FALSE;

	s = 1.0F/r3[3];             /* now back substitute row 3 */
	r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

	m2 = r2[3];                 /* now back substitute row 2 */
	s  = 1.0F/r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
		r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
		r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
		r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

	m1 = r1[2];                 /* now back substitute row 1 */
	s  = 1.0F/r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
		r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
		r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

	m0 = r0[1];                 /* now back substitute row 0 */
	s  = 1.0F/r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
		r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

	MAT(out,0,0) = r0[4];
	MAT(out,0,1) = r0[5], MAT(out,0,2) = r0[6];
	MAT(out,0,3) = r0[7], MAT(out,1,0) = r1[4];
	MAT(out,1,1) = r1[5], MAT(out,1,2) = r1[6];
	MAT(out,1,3) = r1[7], MAT(out,2,0) = r2[4];
	MAT(out,2,1) = r2[5], MAT(out,2,2) = r2[6];
	MAT(out,2,3) = r2[7], MAT(out,3,0) = r3[4];
	MAT(out,3,1) = r3[5], MAT(out,3,2) = r3[6];
	MAT(out,3,3) = r3[7];

	return TRUE;
}
#undef MAT
#undef SWAP_ROWS
//////////////////////////////////////////////////////////////////////////

void ContextRC1::CalcStartPixelbyProjection(const float *m){
	
	float newNear;
	float A, beta;
	float x, y;
	float xmax, ymax;

	x = m[0];
	y = m[5];

	A = (m[10]-1) / (m[10]+1);
	beta =  m[14];

	newNear = 1-A;
	newNear = newNear * beta;
	newNear =  newNear / (2*A);


	xmax = 2 * newNear / x;
	ymax = 2 * newNear / y;

	m_Screen.RayCamera.distance = newNear;

	m_Screen.info.pixelSizeX   = -xmax/m_Screen.Width;
	m_Screen.info.pixelSizeY   = -ymax/m_Screen.Height;

	m_Screen.info.startPositionX = xmax/2.0f;
	m_Screen.info.startPositionY = ymax/2.0f;
}

void ContextRC1::CalcStartPixelbyProjectionInLHCoordSys(const float *m){
	
	float newNear;
	float A, beta;
	float x, y;
	float xmax, ymax;

	x = m[0];
	y = m[5];

	A = (m[10] - 1) / (m[10]+1);
	beta =  m[14];

	newNear = 1-A;
	newNear = newNear * beta;
	newNear =  newNear / (2*A);


	xmax = - (2 * newNear / x);
	ymax = - (2 * newNear / y);

	m_Screen.RayCamera.distance = newNear;

	m_Screen.info.pixelSizeX   = xmax/m_Screen.Width;
	m_Screen.info.pixelSizeY   = ymax/m_Screen.Height;

	m_Screen.info.startPositionX = -xmax/2.0f;
	m_Screen.info.startPositionY = -ymax/2.0f;
}

void ContextRC1::CalculateInverseViewMatrix(float *inverseViewMatrix){
	float viewMatrix[16];

	InverseMatrix(inverseViewMatrix, viewMatrix);

	m_Screen.RayCamera.basis[0].m[0] = viewMatrix[0];
	m_Screen.RayCamera.basis[0].m[1] = viewMatrix[4];
	m_Screen.RayCamera.basis[0].m[2] = viewMatrix[8];

	m_Screen.RayCamera.basis[1].m[0] = viewMatrix[1];
	m_Screen.RayCamera.basis[1].m[1] = viewMatrix[5];
	m_Screen.RayCamera.basis[1].m[2] = viewMatrix[9];

	m_Screen.RayCamera.basis[2].m[0] = - viewMatrix[2];
	m_Screen.RayCamera.basis[2].m[1] = - viewMatrix[6];
	m_Screen.RayCamera.basis[2].m[2] = - viewMatrix[10];
}

//---------------------------------------------------------------------------------------------
// extra Matrix API entries
//---------------------------------------------------------------------------------------------
#define M_PI        3.14159265358979323846
#define DEGREE      0.01745329251994329577

void ContextRC1::Rotate (RCfloat angle, RCfloat x, RCfloat y, RCfloat z){
	RCfloat xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c, s, c;
	RCfloat m[16];
	RCboolean optimized;
	RCdouble theta;//
	RCfloat mag;

	if (m_dwCurrentMatrixMode != RC_MODELVIEW
		&& m_dwCurrentMatrixMode != RC_PROJECTION
		&& m_dwCurrentMatrixMode != RC_TEXTURE)
		return;

	theta = (RCdouble)angle * DEGREE;

	s = (RCfloat) sin( theta );
	c = (RCfloat) cos( theta );

	memcpy(m, m_afIdentity, sizeof(RCfloat)*16);
	optimized = FALSE;

#define M(row,col)  m[col*4+row]
	if (x == 0.0F) {
		if (y == 0.0F) {
			if (z != 0.0F) {
				optimized = TRUE;
				/* rotate only around z-axis */
				M(0,0) = c;
				M(1,1) = c;
				if (z < 0.0F) {
					M(0,1) = s;
					M(1,0) = -s;
				}
				else {
					M(0,1) = -s;
					M(1,0) = s;
				}
			}
		}
		else if (z == 0.0F) {
			optimized = TRUE;
			/* rotate only around y-axis */
			M(0,0) = c;
			M(2,2) = c;
			if (y < 0.0F) {
				M(0,2) = -s;
				M(2,0) = s;
			}
			else {
				M(0,2) = s;
				M(2,0) = -s;
			}
		}
	}
	else if (y == 0.0F) {
		if (z == 0.0F) {
			optimized = TRUE;
			/* rotate only around x-axis */
			M(1,1) = c;
			M(2,2) = c;
			if (x < 0.0F) {
				M(1,2) = s;
				M(2,1) = -s;
			}
			else {
				M(1,2) = -s;
				M(2,1) = s;
			}
		}
	}

	if (!optimized) {
		mag = (RCfloat) sqrt(x * x + y * y + z * z);

		if (mag <= 1.0e-4) {
			/* no rotation, leave mat as-is */
			return;
		}

		x /= mag;
		y /= mag;
		z /= mag;

		xx = x * x;
		yy = y * y;
		zz = z * z;
		xy = x * y;
		yz = y * z;
		zx = z * x;
		xs = x * s;
		ys = y * s;
		zs = z * s;
		one_c = 1.0F - c;

		/* We already hold the identity-matrix so we can skip some statements */
		M(0,0) = (one_c * xx) + c;
		M(0,1) = (one_c * xy) - zs;
		M(0,2) = (one_c * zx) + ys;
		/*      M(0,3) = 0.0F; */

		M(1,0) = (one_c * xy) + zs;
		M(1,1) = (one_c * yy) + c;
		M(1,2) = (one_c * yz) - xs;
		/*      M(1,3) = 0.0F; */

		M(2,0) = (one_c * zx) - ys;
		M(2,1) = (one_c * yz) + xs;
		M(2,2) = (one_c * zz) + c;
		/*      M(2,3) = 0.0F; */

		/*
		M(3,0) = 0.0F;
		M(3,1) = 0.0F;
		M(3,2) = 0.0F;
		M(3,3) = 1.0F;
		*/
	}

#undef M

	switch(m_dwCurrentMatrixMode ){
	case RC_MODELVIEW:
		MatrixMultf( m_MatrixModelToWorld.m, m_MatrixModelToWorld.m, m );
		m_CurrentStateFlag.etc.matrix_modelview_inverse = 0;	
		break;
	case RC_PROJECTION:
		MatrixMultf( m_MatrixProjection.m, m_MatrixProjection.m, m );	
		break;
	case RC_TEXTURE:
		MatrixMultf( m_MatrixTexture.m, m_MatrixTexture.m, m );
		break;
	}
}

void ContextRC1::Scale (RCfloat x, RCfloat y, RCfloat z){
	RCfloat *m = NULL;

	if (m_dwCurrentMatrixMode != RC_MODELVIEW
		&& m_dwCurrentMatrixMode != RC_PROJECTION
		&& m_dwCurrentMatrixMode != RC_TEXTURE)
		return;

	switch(m_dwCurrentMatrixMode ){
	case RC_MODELVIEW:
		m = m_MatrixModelToWorld.m;
		m_CurrentStateFlag.etc.matrix_modelview_inverse = 0;
		break;
	case RC_PROJECTION:
		m = m_MatrixProjection.m;
		break;
	case RC_TEXTURE:
		m = m_MatrixTexture.m;
		break;
	}

	m[0] *= x;   m[4] *= y;   m[8]  *= z;
	m[1] *= x;   m[5] *= y;   m[9]  *= z;
	m[2] *= x;   m[6] *= y;   m[10] *= z;
	m[3] *= x;   m[7] *= y;   m[11] *= z;
}

void ContextRC1::Translate (RCfloat x, RCfloat y, RCfloat z){
	RCfloat *m = NULL;

	if (m_dwCurrentMatrixMode != RC_MODELVIEW
		&& m_dwCurrentMatrixMode != RC_PROJECTION
		&& m_dwCurrentMatrixMode != RC_TEXTURE)
		return;

	switch(m_dwCurrentMatrixMode ){
	case RC_MODELVIEW:
		m = m_MatrixModelToWorld.m;
		m_CurrentStateFlag.etc.matrix_modelview_inverse = 0;
		break;
	case RC_PROJECTION:
		m = m_MatrixProjection.m;
		break;
	case RC_TEXTURE:
		m = m_MatrixTexture.m;
		break;
	}

	m[12] = m[0] * x + m[4] * y + m[8]  * z + m[12];
	m[13] = m[1] * x + m[5] * y + m[9]  * z + m[13];
	m[14] = m[2] * x + m[6] * y + m[10] * z + m[14];
	m[15] = m[3] * x + m[7] * y + m[11] * z + m[15];
}

void ContextRC1::PopMatrix (void){
	switch(m_dwCurrentMatrixMode ){
	case RC_MODELVIEW:
		if (m_iMatrixStackIndexModelView > 0){			
			m_iMatrixStackIndexModelView--;
			memcpy(m_MatrixModelToWorld.m, 			m_MatrixStackModelView[m_iMatrixStackIndexModelView].V_matrix.m, SIZEOFIDENTITYMATRIX);
			memcpy(m_MatrixModelToWorldInverse.m,	m_MatrixStackModelView[m_iMatrixStackIndexModelView].N_matrix.m, SIZEOFIDENTITYMATRIX);
		}
		else
			SetError(RC_STACK_UNDERFLOW);
		break;
	case RC_PROJECTION:
		if (m_iMatrixStackIndexProjection > 0){			
			m_iMatrixStackIndexProjection--;
			memcpy(m_MatrixProjection.m, 			m_MatrixStackProjection[m_iMatrixStackIndexProjection].V_matrix.m, SIZEOFIDENTITYMATRIX);
			memcpy(m_MatrixProjectionInverse.m,		m_MatrixStackProjection[m_iMatrixStackIndexProjection].N_matrix.m, SIZEOFIDENTITYMATRIX);
		}
		else
			SetError(RC_STACK_UNDERFLOW);
		break;
	case RC_TEXTURE:
		if (m_iMatrixStackIndexTexture > 0){
			m_iMatrixStackIndexTexture--;
			memcpy(m_MatrixTexture.m, 			m_MatrixStackTexture[m_iMatrixStackIndexTexture].V_matrix.m, SIZEOFIDENTITYMATRIX);
			memcpy(m_MatrixTextureInverse.m,	m_MatrixStackTexture[m_iMatrixStackIndexTexture].N_matrix.m, SIZEOFIDENTITYMATRIX);
		}
		else
			SetError(RC_STACK_UNDERFLOW);
		break;
	}
}

void ContextRC1::PushMatrix (void){
	switch(m_dwCurrentMatrixMode ){
	case RC_MODELVIEW:
		if (m_iMatrixStackIndexModelView < RCDDK_MAX_MATRIX_STACK_COUNT - 1) {
			memcpy(m_MatrixStackModelView[m_iMatrixStackIndexModelView].V_matrix.m, m_MatrixModelToWorld.m, 	   SIZEOFIDENTITYMATRIX);
			memcpy(m_MatrixStackModelView[m_iMatrixStackIndexModelView].N_matrix.m, m_MatrixModelToWorldInverse.m, SIZEOFIDENTITYMATRIX);
			m_iMatrixStackIndexModelView += 1;
		}
		else
			SetError(RC_STACK_OVERFLOW);
		break;
	case RC_PROJECTION:
		if (m_iMatrixStackIndexProjection < RCDDK_MAX_MATRIX_STACK_COUNT - 1) {
			memcpy(m_MatrixStackProjection[m_iMatrixStackIndexProjection].V_matrix.m, m_MatrixProjection.m,			SIZEOFIDENTITYMATRIX);
			memcpy(m_MatrixStackProjection[m_iMatrixStackIndexProjection].N_matrix.m, m_MatrixProjectionInverse.m,	SIZEOFIDENTITYMATRIX);
			m_iMatrixStackIndexProjection += 1;
		}
		else
			SetError(RC_STACK_OVERFLOW);
		break;
	case RC_TEXTURE:
		if (m_iMatrixStackIndexTexture < RCDDK_MAX_MATRIX_STACK_COUNT - 1) {
			memcpy(m_MatrixStackTexture[m_iMatrixStackIndexTexture].V_matrix.m, m_MatrixTexture.m,			SIZEOFIDENTITYMATRIX);
			memcpy(m_MatrixStackTexture[m_iMatrixStackIndexTexture].N_matrix.m, m_MatrixTextureInverse.m,	SIZEOFIDENTITYMATRIX);
			m_iMatrixStackIndexTexture += 1;
		}
		else
			SetError(RC_STACK_OVERFLOW);
		break;
	}
}

void ContextRC1::LoadIdentity (void){
	// Identity matrix.
	switch(m_dwCurrentMatrixMode ){
	case RC_MODELVIEW:
		memcpy( m_MatrixModelToWorld.m, m_afIdentity, SIZEOFIDENTITYMATRIX );
		memcpy( m_MatrixModelToWorldInverse.m, m_afIdentity, SIZEOFIDENTITYMATRIX );
		m_CurrentStateFlag.etc.matrix_modelview_inverse = 0;
		break;
	case RC_PROJECTION:
		memcpy( m_MatrixProjection.m, m_afIdentity, SIZEOFIDENTITYMATRIX );
		memcpy( m_MatrixProjectionInverse.m, m_afIdentity, SIZEOFIDENTITYMATRIX );
		break;
	case RC_TEXTURE:
		memcpy( m_MatrixTexture.m, m_afIdentity, SIZEOFIDENTITYMATRIX );
		memcpy( m_MatrixTextureInverse.m, m_afIdentity, SIZEOFIDENTITYMATRIX );
		break;
	case RC_MATRIX_PALETTE_OES:
		{
			if (!m_CurrentStateFlag.etc.matrix_palette
				|| m_iMatrixArrayIndex < 0 
				|| m_iMatrixArrayIndex > RCDDK_MAX_MATRIX_PALETTE_COUNT - 1){
					SetError(RC_INVALID_OPERATION);
					return;
			}

			SetMatrixPaletteIndex(m_iMatrixArrayIndex, m_afIdentity);
		}
		break;
	}
}

void ContextRC1::LoadMatrix (const RCfloat *m){
	switch(m_dwCurrentMatrixMode ){
	case RC_MODELVIEW:
		memcpy ( m_MatrixModelToWorld.m, m, SIZEOFIDENTITYMATRIX);
		m_CurrentStateFlag.etc.matrix_modelview_inverse = 0;
		break;
	case RC_PROJECTION:
		memcpy ( m_MatrixProjection.m, m, SIZEOFIDENTITYMATRIX);
		break;
	case RC_TEXTURE:
		memcpy ( m_MatrixTexture.m, m, SIZEOFIDENTITYMATRIX);
		break;
	case RC_MATRIX_PALETTE_OES:
		{
			if (!m_CurrentStateFlag.etc.matrix_palette
				|| m_iMatrixArrayIndex < 0 
				|| m_iMatrixArrayIndex > RCDDK_MAX_MATRIX_PALETTE_COUNT - 1){
					SetError(RC_INVALID_OPERATION);
					return;
			}

			SetMatrixPaletteIndex(m_iMatrixArrayIndex, m);
		}
		break;
	}
}

void ContextRC1::MultMatrix (const RCfloat *m){
	switch(m_dwCurrentMatrixMode ){
	case RC_MODELVIEW:
		MatrixMultf(m_MatrixModelToWorld.m, m_MatrixModelToWorld.m, m);
		m_CurrentStateFlag.etc.matrix_modelview_inverse = 0;
		break;
	case RC_PROJECTION:
		MatrixMultf(m_MatrixProjection.m, m_MatrixProjection.m, m);
		break;
	case RC_TEXTURE:
		MatrixMultf(m_MatrixTexture.m, m_MatrixTexture.m, m);
		break;
	case RC_MATRIX_PALETTE_OES:
		{
			if (!m_CurrentStateFlag.etc.matrix_palette
				|| m_iMatrixArrayIndex < 0 
				|| m_iMatrixArrayIndex > RCDDK_MAX_MATRIX_PALETTE_COUNT - 1){
				SetError(RC_INVALID_OPERATION);
				return;
			}

			MATRIX_PALETTE *mtx=GetMatrixPaletteIndex(m_iMatrixArrayIndex);
			MatrixMultf(mtx->matrix.m, mtx->matrix.m, m);
		}
		break;
	}
}

void ContextRC1::MatrixMode (RCenum mode){
	switch(mode ){
	case RC_MODELVIEW:
	case RC_PROJECTION:
	case RC_TEXTURE:
		m_dwCurrentMatrixMode = mode;
		break;
	case RC_MATRIX_PALETTE_OES:
		if (!m_CurrentStateFlag.etc.matrix_palette){
			SetError(RC_INVALID_OPERATION);
			return;
		}
		m_dwCurrentMatrixMode = mode;
		break;
	default:
		SetError(RC_INVALID_ENUM);
		break;
	}
}
