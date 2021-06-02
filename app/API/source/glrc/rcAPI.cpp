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
// DATE    : 7/22/2013 Mon
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "rcCommon.h"
#include "rcContext1.h"

/*------------------------------------------------------------------------*
 * Debug Functions
 *------------------------------------------------------------------------*/

PROC_TABLE_DECLARE_RC
	PROC_TABLE_ITEM_RC(	AlphaFunc							)
	PROC_TABLE_ITEM_RC(	BlendFunc							)
	PROC_TABLE_ITEM_RC(	ClipPlanef							)
	PROC_TABLE_ITEM_RC(	ColorMask							)
	PROC_TABLE_ITEM_RC(	CullFace							)
	PROC_TABLE_ITEM_RC(	DepthRangef							)
	PROC_TABLE_ITEM_RC(	DepthMask							)
	PROC_TABLE_ITEM_RC(	DepthFunc							)
	PROC_TABLE_ITEM_RC(	ClearDepthf							)	
	PROC_TABLE_ITEM_RC(	FrontFace							)
	PROC_TABLE_ITEM_RC(	Scissor								)
	PROC_TABLE_ITEM_RC(	PixelStorei							)
	PROC_TABLE_ITEM_RC(	LineWidth							)
	PROC_TABLE_ITEM_RC(	LogicOp								)
	PROC_TABLE_ITEM_RC(	PointSize							)
	PROC_TABLE_ITEM_RC(	TexEnviv							)
	PROC_TABLE_ITEM_RC(	PolygonOffset						)
	PROC_TABLE_ITEM_RC(	ClearStencil						)
	PROC_TABLE_ITEM_RC(	StencilMask							)
	PROC_TABLE_ITEM_RC(	StencilOp							)
	PROC_TABLE_ITEM_RC(	StencilFunc							)
	PROC_TABLE_ITEM_RC(	TexEnvfv							)
	PROC_TABLE_ITEM_RC(	TexEnvf								)
	PROC_TABLE_ITEM_RC(	CopyTexImage2D						)
	PROC_TABLE_ITEM_RC(	CopyTexSubImage2D					)
	PROC_TABLE_ITEM_RC(	ClearColor							)
	PROC_TABLE_ITEM_RC(	ClearColorx							)
	PROC_TABLE_ITEM_RC(	Color4f								)
	PROC_TABLE_ITEM_RC(	Color4ub							)
	PROC_TABLE_ITEM_RC(	Color4x								)
	PROC_TABLE_ITEM_RC(	Orthof								)
	PROC_TABLE_ITEM_RC(	Frustum								)
	PROC_TABLE_ITEM_RC(	Frustumf							)
	PROC_TABLE_ITEM_RC(	Frustumx							)
	PROC_TABLE_ITEM_RC(	GetFloatv							)
	PROC_TABLE_ITEM_RC(	GetLightfv							)
	PROC_TABLE_ITEM_RC(	GetLightxv							)
	PROC_TABLE_ITEM_RC(	GetMaterialfv						)
	PROC_TABLE_ITEM_RC(	GetMaterialxv						)
	PROC_TABLE_ITEM_RC(	GetTexParameterfv					)
	PROC_TABLE_ITEM_RC(	GetTexParameteriv					)
	PROC_TABLE_ITEM_RC(	GetTexParameterxv					)
	PROC_TABLE_ITEM_RC(	Lightf								)
	PROC_TABLE_ITEM_RC(	Lightfv								)
	PROC_TABLE_ITEM_RC(	Lightx								)
	PROC_TABLE_ITEM_RC(	Lightxv								)
	PROC_TABLE_ITEM_RC(	LoadMatrixf							)
	PROC_TABLE_ITEM_RC(	LoadMatrixx							)
	PROC_TABLE_ITEM_RC(	LoadIdentity						)
	PROC_TABLE_ITEM_RC(	Materialf							)
	PROC_TABLE_ITEM_RC(	Materialfv							)
	PROC_TABLE_ITEM_RC(	Materialx							)
	PROC_TABLE_ITEM_RC(	Materialxv							)
	PROC_TABLE_ITEM_RC(	MultMatrixf							)
	PROC_TABLE_ITEM_RC(	MultMatrixx							)
	PROC_TABLE_ITEM_RC(	Rotatef								)
	PROC_TABLE_ITEM_RC(	Rotatex								)
	PROC_TABLE_ITEM_RC(	Scalef								)
	PROC_TABLE_ITEM_RC(	Scalex								)
	PROC_TABLE_ITEM_RC(	TexParameterf						)
	PROC_TABLE_ITEM_RC(	TexParameterfv						)
	PROC_TABLE_ITEM_RC(	TexParameteri						)
	PROC_TABLE_ITEM_RC(	TexParameteriv						)
	PROC_TABLE_ITEM_RC(	TexParameterx						)
	PROC_TABLE_ITEM_RC(	TexParameterxv						)
	PROC_TABLE_ITEM_RC(	Translatef							)
	PROC_TABLE_ITEM_RC(	Translatex							)
	PROC_TABLE_ITEM_RC(	BindBuffer							)
	PROC_TABLE_ITEM_RC(	BindMaterial						)
	PROC_TABLE_ITEM_RC(	BindTexture							)
	PROC_TABLE_ITEM_RC(	BufferData							)
	PROC_TABLE_ITEM_RC(	Clear								)
	PROC_TABLE_ITEM_RC(	ColorPointer						)
	PROC_TABLE_ITEM_RC(	DeleteBuffers						)
	PROC_TABLE_ITEM_RC(	DeleteMaterials						)
	PROC_TABLE_ITEM_RC(	DeleteTextures						)
	PROC_TABLE_ITEM_RC(	Disable								)
	PROC_TABLE_ITEM_RC(	DisableClientState					)
	PROC_TABLE_ITEM_RC(	DrawArrays							)
	PROC_TABLE_ITEM_RC(	DrawElements						)
	PROC_TABLE_ITEM_RC(	Enable								)
	PROC_TABLE_ITEM_RC(	EnableClientState					)
	PROC_TABLE_ITEM_RC(	Finish								)
	PROC_TABLE_ITEM_RC(	Flush								)
	PROC_TABLE_ITEM_RC(	GetBooleanv							)
	PROC_TABLE_ITEM_RC(	GetBufferParameteriv				)
	PROC_TABLE_ITEM_RC(	GenBuffers							)
	PROC_TABLE_ITEM_RC(	GenMaterials						)
	PROC_TABLE_ITEM_RC(	GenTextures							)
	PROC_TABLE_ITEM_RC(	GetError							)
	PROC_TABLE_ITEM_RC(	GetFixedv							)
	PROC_TABLE_ITEM_RC(	GetIntegerv							)
	PROC_TABLE_ITEM_RC(	GetPointerv							)
	PROC_TABLE_ITEM_RC(	GetString							)
	PROC_TABLE_ITEM_RC(	Hint								)
	PROC_TABLE_ITEM_RC(	IsBuffer							)
	PROC_TABLE_ITEM_RC(	IsEnabled							)
	PROC_TABLE_ITEM_RC(	IsMaterial							)
	PROC_TABLE_ITEM_RC(	IsTexture							)
	PROC_TABLE_ITEM_RC(	MatrixMode							)
	PROC_TABLE_ITEM_RC(	NormalPointer						)
	PROC_TABLE_ITEM_RC(	PopMatrix							)
	PROC_TABLE_ITEM_RC(	ReadPixels							)
	PROC_TABLE_ITEM_RC(	TexCoordPointer						)
	PROC_TABLE_ITEM_RC(	TexImage2D							)
	PROC_TABLE_ITEM_RC(	TexSubImage2D						)
	PROC_TABLE_ITEM_RC(	VertexPointer						)
	PROC_TABLE_ITEM_RC(	Viewport							)
	PROC_TABLE_ITEM_RC(	uPerspective						)
	PROC_TABLE_ITEM_RC(	uLookAt							    )
	PROC_TABLE_ITEM_RC(	SceneAllInit						)
	PROC_TABLE_ITEM_RC(	StaticSceneBegin					)
	PROC_TABLE_ITEM_RC(	StaticSceneEnd						)
	PROC_TABLE_ITEM_RC(	TextureAlpha						)
	PROC_TABLE_ITEM_RC(	DepthBounce							)
	PROC_TABLE_ITEM_RC(	CurrentPaletteMatrixOES				)
	PROC_TABLE_ITEM_RC(	LoadPaletteFromModelViewMatrixOES	)
	PROC_TABLE_ITEM_RC(	MatrixIndexPointerOES				)
	PROC_TABLE_ITEM_RC(	WeightPointerOES					)
PROC_TABLE_DECLARE_END


RC_API void RC_APIENTRY rcAlphaFunc (RCenum func, RCclampf ref){
	CURRENT_RC_CONTEXT->AlphaFunc(func, ref);
}

RC_API void RC_APIENTRY rcAlphaFuncx (RCenum func, RCclampx ref){
	CURRENT_RC_CONTEXT->AlphaFunc(func, x32f32(ref));
}

RC_API void RC_APIENTRY rcClearColor (RCclampf red, RCclampf green, RCclampf blue, RCclampf alpha){
	CURRENT_RC_CONTEXT->ClearColor(red, green, blue, alpha);
}

RC_API void RC_APIENTRY rcClearColorx (RCclampx red, RCclampx green, RCclampx blue, RCclampx alpha){
	CURRENT_RC_CONTEXT->ClearColor(x32f32(red), x32f32(green), x32f32(blue), x32f32(alpha));
}

RC_API void RC_APIENTRY rcClearDepthf (RCclampf depth){
	CURRENT_RC_CONTEXT->ClearDepth(depth);
}

RC_API void RC_APIENTRY rcClearDepthx (RCclampx depth){
	CURRENT_RC_CONTEXT->ClearDepth(x32f32(depth));
}

RC_API void RC_APIENTRY rcClipPlanef (RCenum plane, const RCfloat *equation){
	CURRENT_RC_CONTEXT->ClipPlane(plane, equation);
}

RC_API void RC_APIENTRY rcClipPlanex (RCenum plane, const RCfixed *equation){
	RCfloat equ[4] = {
		x32f32(equation[0]),
		x32f32(equation[1]),
		x32f32(equation[2]),
		x32f32(equation[3])
	};
	CURRENT_RC_CONTEXT->ClipPlane(plane, equ);
}

RC_API void RC_APIENTRY rcColor4f (RCfloat red, RCfloat green, RCfloat blue, RCfloat alpha){
	CURRENT_RC_CONTEXT->Color4(red, green, blue, alpha);
}

RC_API void RC_APIENTRY rcColor4ub (RCubyte red, RCubyte green, RCubyte blue, RCubyte alpha){
	CURRENT_RC_CONTEXT->Color4(((RCfloat)red)/255, ((RCfloat)green)/255, ((RCfloat)blue)/255, ((RCfloat)alpha)/255);
}

RC_API void RC_APIENTRY rcColor4x (RCfixed red, RCfixed green, RCfixed blue, RCfixed alpha){
	CURRENT_RC_CONTEXT->Color4(x32f32(red), x32f32(green), x32f32(blue), x32f32(alpha));
}

RC_API void RC_APIENTRY rcDepthRangef (RCclampf zNear, RCclampf zFar){
	CURRENT_RC_CONTEXT->DepthRange (zNear, zFar);
}

RC_API void RC_APIENTRY rcDepthRangex (RCclampx zNear, RCclampx zFar){
	CURRENT_RC_CONTEXT->DepthRange (x32f32(zNear), x32f32(zFar));
}

RC_API void RC_APIENTRY rcFogf (RCenum pname, RCfloat param){
	CURRENT_RC_CONTEXT->Fog(pname, param);
}

RC_API void RC_APIENTRY rcFogfv (RCenum pname, const RCfloat *params){
	CURRENT_RC_CONTEXT->Fogv(pname, params);
}

RC_API void RC_APIENTRY rcFogx (RCenum pname, RCfixed param){
	CURRENT_RC_CONTEXT->Fog(pname, x32f32(param));
}

RC_API void RC_APIENTRY rcFogxv (RCenum pname, const RCfixed *params){
	RCfloat pp[4];
	pp[0]	= x32f32(params[0]);
	if(pname == RC_FOG_COLOR){
		pp[1]	= x32f32(params[1]);
		pp[2]	= x32f32(params[2]);
		pp[3]	= x32f32(params[3]);
	}
	CURRENT_RC_CONTEXT->Fogv(pname, pp);
}

RC_API void RC_APIENTRY rcFrustum (RCdouble left, RCdouble right, RCdouble bottom, RCdouble top, RCdouble zNear, RCdouble zFar){
	CURRENT_RC_CONTEXT->Frustum((RCfloat) left, (RCfloat) right, (RCfloat) bottom, (RCfloat) top, (RCfloat) zNear, (RCfloat) zFar);
}

RC_API void RC_APIENTRY rcFrustumf (RCfloat left, RCfloat right, RCfloat bottom, RCfloat top, RCfloat zNear, RCfloat zFar){
	CURRENT_RC_CONTEXT->Frustum(left, right, bottom, top, zNear, zFar);
}

RC_API void RC_APIENTRY rcFrustumx (RCfixed left, RCfixed right, RCfixed bottom, RCfixed top, RCfixed zNear, RCfixed zFar){
	CURRENT_RC_CONTEXT->Frustum(x32f32(left), x32f32(right), x32f32(bottom), x32f32(top), x32f32(zNear), x32f32(zFar));
}

RC_API void RC_APIENTRY rcGetClipPlanef (RCenum pname, RCfloat eqn[4]){
	CURRENT_RC_CONTEXT->GetClipPlane(pname, eqn);
}

RC_API void RC_APIENTRY rcGetClipPlanex (RCenum pname, RCfixed eqn[4]){
	RCfloat equ[4];
	memset(equ, 0, sizeof(RCfloat)*4);
	CURRENT_RC_CONTEXT->GetClipPlane(pname, equ);
	eqn[0]	= f32x32(equ[0]);
	eqn[1]	= f32x32(equ[1]);
	eqn[2]	= f32x32(equ[2]);
	eqn[3]	= f32x32(equ[3]);
}

RC_API void RC_APIENTRY rcGetFloatv (RCenum pname, RCfloat *params){
	CURRENT_RC_CONTEXT->GetFloatv(pname, params);
}

RC_API void RC_APIENTRY rcGetLightfv (RCenum light, RCenum pname, RCfloat *params){
	CURRENT_RC_CONTEXT->GetLightv (light, pname, params);   
}

RC_API void RC_APIENTRY rcGetLightxv (RCenum light, RCenum pname, RCfixed *params){
	RCfloat paramsf[4];	
	memset(paramsf, 0, sizeof(RCfloat)*4);
	CURRENT_RC_CONTEXT->GetLightv (light, pname, paramsf);	
	params[0] = f32x32 (paramsf[0]);
	switch (pname){
	case RC_ATTENUATION_RANGE:
		params[1] = f32x32 (paramsf[1]);
		break;
	case RC_SPOT_DIRECTION:
		params[1] = f32x32 (paramsf[1]);
		params[2] = f32x32 (paramsf[2]);
		break;
	case RC_AMBIENT:
	case RC_DIFFUSE:
	case RC_SPECULAR:
	case RC_POSITION:
		params[1] = f32x32 (paramsf[1]);
		params[2] = f32x32 (paramsf[2]);
		params[3] = f32x32 (paramsf[3]);
		break;
	}
}

RC_API void RC_APIENTRY rcGetMaterialfv (RCenum face, RCenum pname, RCfloat *params){
	CURRENT_RC_CONTEXT->GetMaterialv (face, pname, params);   
}

RC_API void RC_APIENTRY rcGetMaterialxv (RCenum face, RCenum pname, RCfixed *params){
	RCfloat paramsf[4];	
	memset(paramsf, 0, sizeof(RCfloat)*4);
	CURRENT_RC_CONTEXT->GetMaterialv (face, pname, paramsf); 
	params[0] = f32x32 (paramsf[0]);
	switch(pname){
//	case RC_TRANSMITTANCE:
//	case RC_REFLECTION:
//		params[1] = f32x32 (paramsf[1]);
//		params[2] = f32x32 (paramsf[2]);
//		break;
	case RC_AMBIENT:
	case RC_DIFFUSE:
	case RC_SPECULAR:
	case RC_EMISSION:
		params[1] = f32x32 (paramsf[1]);
		params[2] = f32x32 (paramsf[2]);
		params[3] = f32x32 (paramsf[3]);
		break;
//	case RC_AMBIENT_AND_DIFFUSE:
//		break;
	}	
}

RC_API void RC_APIENTRY rcGetTexEnvfv (RCenum env, RCenum pname, RCfloat *params){
	CURRENT_RC_CONTEXT->GetTexEnvfv (env, pname, params);   
}

RC_API void RC_APIENTRY rcGetTexEnviv (RCenum env, RCenum pname, RCint *params){
	CURRENT_RC_CONTEXT->GetTexEnviv (env, pname, params);
}

RC_API void RC_APIENTRY rcGetTexEnvxv (RCenum env, RCenum pname, RCfixed *params){
	CURRENT_RC_CONTEXT->GetTexEnvxv (env, pname, params);
}

RC_API void RC_APIENTRY rcGetTexParameterfv (RCenum target, RCenum pname, RCfloat *params){
	RCint paramsf[4];	
	memset(paramsf, 0, sizeof(RCint)*4);
	CURRENT_RC_CONTEXT->GetTexParameterv (target, pname, paramsf);
	params[0] = (RCfloat) paramsf[0];
	params[1] = (RCfloat) paramsf[1];
	params[2] = (RCfloat) paramsf[2];
	params[3] = (RCfloat) paramsf[3];
}

RC_API void RC_APIENTRY rcGetTexParameteriv (RCenum target, RCenum pname, RCint *params){
	CURRENT_RC_CONTEXT->GetTexParameterv (target, pname, params);
}

RC_API void RC_APIENTRY rcGetTexParameterxv (RCenum target, RCenum pname, RCfixed *params){
	RCint paramsf[4];	
	memset(paramsf, 0, sizeof(RCint)*4);
	CURRENT_RC_CONTEXT->GetTexParameterv (target, pname, paramsf);
	params[0] = (RCfixed) paramsf[0];
	params[1] = (RCfixed) paramsf[1];
	params[2] = (RCfixed) paramsf[2];
	params[3] = (RCfixed) paramsf[3];
}

RC_API void RC_APIENTRY rcLightModelf (RCenum pname, RCfloat param){
	CURRENT_RC_CONTEXT->LightModel (pname, param);   
}

RC_API void RC_APIENTRY rcLightModelfv (RCenum pname, const RCfloat *params){
	CURRENT_RC_CONTEXT->LightModelv (pname, params);   
}

RC_API void RC_APIENTRY rcLightModelx (RCenum pname, RCfixed param){
	CURRENT_RC_CONTEXT->LightModel (pname, x32f32(param));
}

RC_API void RC_APIENTRY rcLightModelxv (RCenum pname, const RCfixed *params){
	RCfloat paramsf[4];
	paramsf[0]	= x32f32(params[0]);
	if(pname == RC_LIGHT_MODEL_AMBIENT){
		paramsf[1]	= x32f32(params[1]);
		paramsf[2]	= x32f32(params[2]);
		paramsf[3]	= x32f32(params[3]);
	}
	CURRENT_RC_CONTEXT->LightModelv(pname, paramsf);
}

RC_API void RC_APIENTRY rcLightf (RCenum light, RCenum pname, RCfloat param){
	CURRENT_RC_CONTEXT->Light (light, pname, param);   
}

RC_API void RC_APIENTRY rcLightfv (RCenum light, RCenum pname, const RCfloat *params){
	CURRENT_RC_CONTEXT->Lightv (light, pname, params);   
}

RC_API void RC_APIENTRY rcLightx (RCenum light, RCenum pname, RCfixed param){
	CURRENT_RC_CONTEXT->Light (light, pname, x32f32(param)); 
}

RC_API void RC_APIENTRY rcLightxv (RCenum light, RCenum pname, const RCfixed *params){
	RCfloat paramsf[4];

	paramsf[0]	= x32f32(params[0]);
	switch (pname){
	case RC_AMBIENT:
	case RC_DIFFUSE:
	case RC_SPECULAR:
	case RC_POSITION:
		paramsf[1]	= x32f32(params[1]);
		paramsf[2]	= x32f32(params[2]);
		paramsf[3]	= x32f32(params[3]);
		break;
	case RC_SPOT_DIRECTION:
		paramsf[1]	= x32f32(params[1]);
		paramsf[2]	= x32f32(params[2]);
		break;
	case RC_ATTENUATION_RANGE:
		paramsf[1]	= x32f32(params[1]);
		break;
	}

	CURRENT_RC_CONTEXT->Lightv (light, pname, paramsf); 
}

RC_API void RC_APIENTRY rcLineWidth (RCfloat width){
	CURRENT_RC_CONTEXT->LineWidth(width);
}

RC_API void RC_APIENTRY rcLineWidthx (RCfixed width){
	CURRENT_RC_CONTEXT->LineWidth(x32f32(width));
}

RC_API void RC_APIENTRY rcLoadMatrixf (const RCfloat *m){
	CURRENT_RC_CONTEXT->LoadMatrix (m);
}

RC_API void RC_APIENTRY rcLoadMatrixx (const RCfixed *m){
	RCfloat mf[16];
	int i;
	LOOP(16) mf[i] = x32f32(m[i]);
	CURRENT_RC_CONTEXT->LoadMatrix (mf);
}

RC_API void RC_APIENTRY rcLoadIdentity (void){
	CURRENT_RC_CONTEXT->LoadIdentity ( );   
}

RC_API void RC_APIENTRY rcMaterialf (RCenum face, RCenum pname, RCfloat param){
	CURRENT_RC_CONTEXT->Material(face, pname, param);   
}

RC_API void RC_APIENTRY rcMaterialfv (RCenum face, RCenum pname, const RCfloat *params){
	CURRENT_RC_CONTEXT->Materialv(face, pname, params);
}

RC_API void RC_APIENTRY rcMaterialx (RCenum face, RCenum pname, RCfixed param){
	CURRENT_RC_CONTEXT->Material(face, pname, x32f32(param));
}

RC_API void RC_APIENTRY rcMaterialxv (RCenum face, RCenum pname, const RCfixed *params){
	RCfloat paramf[4];
	paramf[0] = x32f32 (params[0]);
	paramf[1] = x32f32 (params[1]);
	paramf[2] = x32f32 (params[2]);
	paramf[3] = x32f32 (params[3]);
	CURRENT_RC_CONTEXT->Materialv(face, pname, paramf);
}

RC_API void RC_APIENTRY rcMultMatrixf (const RCfloat *m){
	CURRENT_RC_CONTEXT->MultMatrix (m);   
}

RC_API void RC_APIENTRY rcMultMatrixx (const RCfixed *m){
	RCfloat mf[16];
	int i;
	LOOP(16) mf[i] = x32f32(m[i]);
	CURRENT_RC_CONTEXT->MultMatrix (mf);
}

RC_API void RC_APIENTRY rcMultiTexCoord4f (RCenum target, RCfloat s, RCfloat t, RCfloat r, RCfloat q){
	CURRENT_RC_CONTEXT->MultiTexCoord4 (target, s, t, r, q);   
}

RC_API void RC_APIENTRY rcMultiTexCoord4x (RCenum target, RCfixed s, RCfixed t, RCfixed r, RCfixed q){
	CURRENT_RC_CONTEXT->MultiTexCoord4 (target, x32f32(s), x32f32(t), x32f32(r), x32f32(q));
}

RC_API void RC_APIENTRY rcNormal3f (RCfloat nx, RCfloat ny, RCfloat nz){
	CURRENT_RC_CONTEXT->Normal3 (nx, ny, nz);   
}

RC_API void RC_APIENTRY rcNormal3x (RCfixed nx, RCfixed ny, RCfixed nz){
	CURRENT_RC_CONTEXT->Normal3 (x32f32(nx), x32f32(ny), x32f32(nz));   
}

RC_API void RC_APIENTRY rcOrthof (RCfloat left, RCfloat right, RCfloat bottom, RCfloat top, RCfloat zNear, RCfloat zFar){
	CURRENT_RC_CONTEXT->Ortho (left, right, bottom, top, zNear, zFar);   
}

RC_API void RC_APIENTRY rcOrthox (RCfixed left, RCfixed right, RCfixed bottom, RCfixed top, RCfixed zNear, RCfixed zFar){
	CURRENT_RC_CONTEXT->Ortho (x32f32(left), x32f32(right), x32f32(bottom), x32f32(top), x32f32(zNear), x32f32(zFar));   
}

RC_API void RC_APIENTRY rcPointParameterf (RCenum pname, RCfloat param){
	CURRENT_RC_CONTEXT->PointParameterf(pname, param);
}

RC_API void RC_APIENTRY rcPointParameterfv (RCenum pname, const RCfloat *params){
	CURRENT_RC_CONTEXT->PointParameterfv(pname, params);
}

RC_API void RC_APIENTRY rcPointParameterx (RCenum pname, RCfixed param){
	CURRENT_RC_CONTEXT->PointParameterx (pname, param);   
}

RC_API void RC_APIENTRY rcPointParameterxv (RCenum pname, const RCfixed *params){
	CURRENT_RC_CONTEXT->PointParameterxv (pname, params);   
}

RC_API void RC_APIENTRY rcPointSize (RCfloat size){
	CURRENT_RC_CONTEXT->PointSize(size);
}

RC_API void RC_APIENTRY rcPointSizex (RCfixed size){
	CURRENT_RC_CONTEXT->PointSize(x32f32(size));
}

RC_API void RC_APIENTRY rcPolygonOffset (RCfloat factor, RCfloat units){
	CURRENT_RC_CONTEXT->PolygonOffset(factor, units);
}

RC_API void RC_APIENTRY rcPolygonOffsetx (RCfixed factor, RCfixed units){
	CURRENT_RC_CONTEXT->PolygonOffset(x32f32(factor), x32f32(units));
}

RC_API void RC_APIENTRY rcRotatef (RCfloat angle, RCfloat x, RCfloat y, RCfloat z){
	CURRENT_RC_CONTEXT->Rotate(angle, x, y, z);
}

RC_API void RC_APIENTRY rcRotatex (RCfixed angle, RCfixed x, RCfixed y, RCfixed z){
	CURRENT_RC_CONTEXT->Rotate(x32f32(angle),x32f32(x), x32f32(y), x32f32(z));
}

RC_API void RC_APIENTRY rcScalef (RCfloat x, RCfloat y, RCfloat z){
	CURRENT_RC_CONTEXT->Scale(x, y, z);
}

RC_API void RC_APIENTRY rcScalex (RCfixed x, RCfixed y, RCfixed z){
	CURRENT_RC_CONTEXT->Scale(x32f32(x), x32f32(y), x32f32(z));   
}

RC_API void RC_APIENTRY rcTexEnvf (RCenum target, RCenum pname, RCfloat param){
	CURRENT_RC_CONTEXT->TexEnvf (target, pname, param);   
}

RC_API void RC_APIENTRY rcTexEnvfv (RCenum target, RCenum pname, const RCfloat *params){
	CURRENT_RC_CONTEXT->TexEnvfv (target, pname, params);   
}

RC_API void RC_APIENTRY rcTexEnvi (RCenum target, RCenum pname, RCint param){
	CURRENT_RC_CONTEXT->TexEnvi (target, pname, param);
}

RC_API void RC_APIENTRY rcTexEnviv (RCenum target, RCenum pname, const RCint *params){
	CURRENT_RC_CONTEXT->TexEnviv (target, pname, params);   
}

RC_API void RC_APIENTRY rcTexEnvx (RCenum target, RCenum pname, RCfixed param){
	CURRENT_RC_CONTEXT->TexEnvx (target, pname, param);
}

RC_API void RC_APIENTRY rcTexEnvxv (RCenum target, RCenum pname, const RCfixed *params){
	CURRENT_RC_CONTEXT->TexEnvxv (target, pname, params);   
}

RC_API void RC_APIENTRY rcTexParameterf (RCenum target, RCenum pname, RCfloat param){
	CURRENT_RC_CONTEXT->TexParameter (target, pname, RCint(param));
}

RC_API void RC_APIENTRY rcTexParameterfv (RCenum target, RCenum pname, const RCfloat *params){
	RCint paramsf[4];
	paramsf[0] = (RCint) params[0];
	paramsf[1] = (RCint) params[1];
	paramsf[2] = (RCint) params[2];
	paramsf[3] = (RCint) params[3];
	CURRENT_RC_CONTEXT->TexParameterv (target, pname, paramsf);
}

RC_API void RC_APIENTRY rcTexParameteri (RCenum target, RCenum pname, RCint param){
	CURRENT_RC_CONTEXT->TexParameter (target, pname, param);
}

RC_API void RC_APIENTRY rcTexParameteriv (RCenum target, RCenum pname, const RCint *params){
	CURRENT_RC_CONTEXT->TexParameterv (target, pname, params);   
}

RC_API void RC_APIENTRY rcTexParameterx (RCenum target, RCenum pname, RCfixed param){
	CURRENT_RC_CONTEXT->TexParameter (target, pname, RCint(param));
}

RC_API void RC_APIENTRY rcTexParameterxv (RCenum target, RCenum pname, const RCfixed *params){
	RCint paramsf[4];	
	paramsf[0] = params[0];
	paramsf[1] = params[1];
	paramsf[2] = params[2];
	paramsf[3] = params[3];
	CURRENT_RC_CONTEXT->TexParameterv (target, pname, paramsf);
}

RC_API void RC_APIENTRY rcTranslatef (RCfloat x, RCfloat y, RCfloat z){
	CURRENT_RC_CONTEXT->Translate (x, y, z);   
}

RC_API void RC_APIENTRY rcTranslatex (RCfixed x, RCfixed y, RCfixed z){
	CURRENT_RC_CONTEXT->Translate (x32f32(x), x32f32(y), x32f32(z));   
}

/*****************************************************/
/* Available in both Common and Common-Lite profiles */
RC_API void RC_APIENTRY rcActiveTexture (RCenum texture){
	CURRENT_RC_CONTEXT->ActiveTexture(texture);
}

RC_API void RC_APIENTRY rcBindBuffer (RCenum target, RCuint buffer){
	CURRENT_RC_CONTEXT->BindBuffer (target, buffer);   
}

RC_API void RC_APIENTRY rcBindMaterial (RCuint material){
	CURRENT_RC_CONTEXT->BindMaterial (material);   
}

RC_API void RC_APIENTRY rcBindTexture (RCenum target, RCuint texture){
	CURRENT_RC_CONTEXT->BindTexture (target, texture);   
}

RC_API void RC_APIENTRY rcBlendFunc (RCenum sfactor, RCenum dfactor){
	CURRENT_RC_CONTEXT->BlendFunc (sfactor, dfactor);   
}

RC_API void RC_APIENTRY rcBufferData (RCenum target, RCsizeiptr size, const RCvoid *data, RCenum usage){
	CURRENT_RC_CONTEXT->BufferData (target, size, data, usage);   
}

RC_API void RC_APIENTRY rcBufferSubData (RCenum target, RCintptr offset, RCsizeiptr size, const RCvoid *data){
	CURRENT_RC_CONTEXT->BufferSubData (target, offset, size, data);   
}

RC_API void RC_APIENTRY rcClear (RCbitfield mask){
	CURRENT_RC_CONTEXT->Clear(mask);
}

RC_API void RC_APIENTRY rcClearStencil (RCint s){
	CURRENT_RC_CONTEXT->ClearStencil(s);
}

RC_API void RC_APIENTRY rcClientActiveTexture (RCenum texture){
	CURRENT_RC_CONTEXT->ClientActiveTexture (texture);   
}

RC_API void RC_APIENTRY rcColorMask (RCboolean red, RCboolean green, RCboolean blue, RCboolean alpha){
	CURRENT_RC_CONTEXT->ColorMask (red, green, blue, alpha);   
}

RC_API void RC_APIENTRY rcColorPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer){
	CURRENT_RC_CONTEXT->ColorPointer (size, type, stride, pointer);   
}

RC_API void RC_APIENTRY rcCompressedTexImage2D (RCenum target, RCint level, RCenum internalformat, RCsizei width, RCsizei height, RCint border, RCsizei imageSize, const RCvoid *data){
	CURRENT_RC_CONTEXT->CompressedTexImage2D (target, level, internalformat, width, height, border, imageSize, data);   
}

RC_API void RC_APIENTRY rcCompressedTexSubImage2D (RCenum target, RCint level, RCint xoffset, RCint yoffset, RCsizei width, RCsizei height, RCenum format, RCsizei imageSize, const RCvoid *data){
	CURRENT_RC_CONTEXT->CompressedTexSubImage2D (target, level, xoffset, yoffset, width, height, format, imageSize, data);   
}

RC_API void RC_APIENTRY rcCopyTexImage2D (RCenum target, RCint level, RCenum internalformat, RCint x, RCint y, RCsizei width, RCsizei height, RCint border){
	CURRENT_RC_CONTEXT->CopyTexImage2D (target, level, internalformat, x, y, width, height, border);   
}

RC_API void RC_APIENTRY rcCopyTexSubImage2D (RCenum target, RCint level, RCint xoffset, RCint yoffset, RCint x, RCint y, RCsizei width, RCsizei height){
	CURRENT_RC_CONTEXT->CopyTexSubImage2D (target, level, xoffset, yoffset, x, y, width, height);   
}

RC_API void RC_APIENTRY rcCullFace (RCenum mode){
	CURRENT_RC_CONTEXT->CullFace(mode);
}

RC_API void RC_APIENTRY rcDeleteBuffers (RCsizei n, const RCuint *buffers){
	CURRENT_RC_CONTEXT->DeleteBuffers (n, buffers);   
}

RC_API void RC_APIENTRY rcDeleteMaterials (RCsizei n, RCuint *materials){
	CURRENT_RC_CONTEXT->DeleteMaterials (n, materials);
}

RC_API void RC_APIENTRY rcDeleteTextures (RCsizei n, const RCuint *textures){
	CURRENT_RC_CONTEXT->DeleteTextures (n, textures);   
}

RC_API void RC_APIENTRY rcDepthFunc (RCenum func){
	CURRENT_RC_CONTEXT->DepthFunc(func);
}

RC_API void RC_APIENTRY rcDepthMask (RCboolean flag){
	CURRENT_RC_CONTEXT->DepthMask(flag);
}

RC_API void RC_APIENTRY rcDisable (RCenum cap){
	CURRENT_RC_CONTEXT->Enable (cap, FALSE);
}

RC_API void RC_APIENTRY rcDisableClientState (RCenum array){
	CURRENT_RC_CONTEXT->DisableClientState (array);   
}

RC_API void RC_APIENTRY rcDrawArrays (RCenum mode, RCint first, RCsizei count){
	CURRENT_RC_CONTEXT->DrawArrays (mode, first, count);   
}

RC_API void RC_APIENTRY rcDrawElements (RCenum mode, RCsizei count, RCenum type, const RCvoid *indices){
	CURRENT_RC_CONTEXT->DrawElements (mode, count, type, indices);   
}

RC_API void RC_APIENTRY rcEnable (RCenum cap){
	CURRENT_RC_CONTEXT->Enable (cap, TRUE);   
}

RC_API void RC_APIENTRY rcEnableClientState (RCenum array){
	CURRENT_RC_CONTEXT->EnableClientState (array);   
}

RC_API void RC_APIENTRY rcFinish (void){
	CURRENT_RC_CONTEXT->Finish ();
}

RC_API void RC_APIENTRY rcFlush (void){
	CURRENT_RC_CONTEXT->Flush ();
}

RC_API void RC_APIENTRY rcFrontFace (RCenum mode){
	CURRENT_RC_CONTEXT->FrontFace(mode);
}

RC_API void RC_APIENTRY rcGetBooleanv (RCenum pname, RCboolean *params){
	CURRENT_RC_CONTEXT->GetBooleanv (pname, params);   
}

RC_API void RC_APIENTRY rcGetBufferParameteriv (RCenum target, RCenum pname, RCint *params){
	CURRENT_RC_CONTEXT->GetBufferParameterv (target, pname, params);   
}

RC_API void RC_APIENTRY rcGenBuffers (RCsizei n, RCuint *buffers){
	CURRENT_RC_CONTEXT->GenBuffers (n, buffers);   
}

RC_API void RC_APIENTRY rcGenMaterials (RCsizei n, RCuint *materials){
	CURRENT_RC_CONTEXT->GenMaterials (n, materials);
}

RC_API void RC_APIENTRY rcGenTextures (RCsizei n, RCuint *textures){
	CURRENT_RC_CONTEXT->GenTextures (n, textures);   
}

RC_API RCenum RC_APIENTRY rcGetError (void){
	return CURRENT_RC_CONTEXT->GetError();
}

RC_API void RC_APIENTRY rcGetFixedv (RCenum pname, RCfixed *params){
	CURRENT_RC_CONTEXT->GetFixedv (pname, params);   
}

RC_API void RC_APIENTRY rcGetIntegerv (RCenum pname, RCint *params){
	CURRENT_RC_CONTEXT->GetIntegerv (pname, params);   
}

RC_API void RC_APIENTRY rcGetPointerv (RCenum pname, RCvoid **params){
	CURRENT_RC_CONTEXT->GetPointerv (pname, params);   
}

RC_API const RCubyte * RC_APIENTRY rcGetString (RCenum name){
	return CURRENT_RC_CONTEXT->GetString( name ); 
}

RC_API void RC_APIENTRY rcHint (RCenum target, RCenum mode){
	CURRENT_RC_CONTEXT->Hint (target, mode);   
}

RC_API RCboolean RC_APIENTRY rcIsBuffer (RCuint buffer){
	return CURRENT_RC_CONTEXT->IsBuffer( buffer );
}

RC_API RCboolean RC_APIENTRY rcIsEnabled (RCenum cap){
	return CURRENT_RC_CONTEXT->IsEnabled( cap );
}

RC_API RCboolean RC_APIENTRY rcIsMaterial (RCuint material){
	return CURRENT_RC_CONTEXT->IsMaterial( material );
}

RC_API RCboolean RC_APIENTRY rcIsTexture (RCuint texture){
	return CURRENT_RC_CONTEXT->IsTexture( texture );
}

RC_API void RC_APIENTRY rcLogicOp (RCenum opcode){
	CURRENT_RC_CONTEXT->LogicOp (opcode);   
}

RC_API void RC_APIENTRY rcMatrixMode (RCenum mode){
	CURRENT_RC_CONTEXT->MatrixMode (mode);   
}

RC_API void RC_APIENTRY rcNormalPointer (RCenum type, RCsizei stride, const RCvoid *pointer){
	CURRENT_RC_CONTEXT->NormalPointer (type, stride, pointer);   
}

RC_API void RC_APIENTRY rcPixelStorei (RCenum pname, RCint param){
	CURRENT_RC_CONTEXT->PixelStorei (pname, param);   
}

RC_API void RC_APIENTRY rcPopMatrix (void){
	CURRENT_RC_CONTEXT->PopMatrix ();   
}

RC_API void RC_APIENTRY rcPushMatrix (void){
	CURRENT_RC_CONTEXT->PushMatrix ();   
}

RC_API void RC_APIENTRY rcReadPixels (RCint x, RCint y, RCsizei width, RCsizei height, RCenum format, RCenum type, RCvoid *pixels){
	CURRENT_RC_CONTEXT->ReadPixels (x, y, width, height, format, type, pixels);   
}

RC_API void RC_APIENTRY rcSampleCoverage (RCclampf value, RCboolean invert){
	CURRENT_RC_CONTEXT->SampleCoverage (value, invert);   
}

RC_API void RC_APIENTRY rcSampleCoveragex (RCclampx value, RCboolean invert){
	CURRENT_RC_CONTEXT->SampleCoveragex (value, invert);   
}

RC_API void RC_APIENTRY rcScissor (RCint x, RCint y, RCsizei width, RCsizei height){
	CURRENT_RC_CONTEXT->Scissor (x, y, width, height);   
}

RC_API void RC_APIENTRY rcShadeModel (RCenum mode){
	CURRENT_RC_CONTEXT->ShadeModel (mode);   
}

RC_API void RC_APIENTRY rcStencilFunc (RCenum func, RCint ref, RCuint mask){
	CURRENT_RC_CONTEXT->StencilFunc (func, ref, mask);   
}

RC_API void RC_APIENTRY rcStencilMask (RCuint mask){
	CURRENT_RC_CONTEXT->StencilMask (mask);   
}

RC_API void RC_APIENTRY rcStencilOp (RCenum fail, RCenum zfail, RCenum zpass){
	CURRENT_RC_CONTEXT->StencilOp (fail, zfail, zpass);   
}

RC_API void RC_APIENTRY rcTexCoordPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer){
	CURRENT_RC_CONTEXT->TexCoordPointer (size, type, stride, pointer);   
}

RC_API void RC_APIENTRY rcTexImage2D (RCenum target, RCint level, RCint internalformat, RCsizei width, RCsizei height, RCint border, RCenum format, RCenum type, const RCvoid *pixels){
	CURRENT_RC_CONTEXT->TexImage2D (target, level, internalformat, width, height, border, format, type, pixels);   
}

RC_API void RC_APIENTRY rcTexSubImage2D (RCenum target, RCint level, RCint xoffset, RCint yoffset, RCsizei width, RCsizei height, RCenum format, RCenum type, const RCvoid *pixels){
	CURRENT_RC_CONTEXT->TexSubImage2D (target, level, xoffset, yoffset, width, height, format, type, pixels);   
}

RC_API void RC_APIENTRY rcVertexPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer){
	CURRENT_RC_CONTEXT->VertexPointer (size, type, stride, pointer);   
}

RC_API void RC_APIENTRY rcViewport (RCint x, RCint y, RCsizei width, RCsizei height){
	CURRENT_RC_CONTEXT->Viewport(x, y, width, height);
}

/************************************************************************/
/* Extended Functions                                                   */
/************************************************************************/
RC_API void RC_APIENTRY rcuLookAt(RCfloat eyex, RCfloat eyey, RCfloat eyez, 
	RCfloat centerx, RCfloat centery, RCfloat centerz, 
	RCfloat upx, RCfloat upy, RCfloat upz) {
		CURRENT_RC_CONTEXT->LookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

RC_API void RC_APIENTRY rcCurrentPaletteMatrixOES (RCuint matrixpaletteindex){
	CURRENT_RC_CONTEXT->CurrentPaletteMatrixOES(matrixpaletteindex);
}

RC_API void RC_APIENTRY rcLoadPaletteFromModelViewMatrixOES (void){
	CURRENT_RC_CONTEXT->LoadPaletteFromModelViewMatrixOES();
}

RC_API void RC_APIENTRY rcMatrixIndexPointerOES (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer){
	CURRENT_RC_CONTEXT->MatrixIndexPointerOES(size, type, stride, pointer);
}

RC_API void RC_APIENTRY rcWeightPointerOES (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer){
	CURRENT_RC_CONTEXT->WeightPointerOES(size, type, stride, pointer);
}

RC_API void RC_APIENTRY rcTextureAlpha(RCbyte value){
	CURRENT_RC_CONTEXT->TextureAlpha(value); 
}

RC_API void RC_APIENTRY rcSceneAllInit(void ){
	CURRENT_RC_CONTEXT->SceneAllInit(); 
}

RC_API void RC_APIENTRY rcStaticSceneBegin(void){
	CURRENT_RC_CONTEXT->StaticSceneBegin(); 
}

RC_API void RC_APIENTRY rcStaticSceneEnd(void){
	CURRENT_RC_CONTEXT->StaticSceneEnd(); 
}

RC_API void RC_APIENTRY rcuPerspective ( RCfloat fovy, RCfloat aspect, RCfloat zNear, RCfloat zFar) {
	CURRENT_RC_CONTEXT->Perspective(fovy, aspect, zNear, zFar);
}

RC_API void RC_APIENTRY rcDepthBounce (RCuint value){
	CURRENT_RC_CONTEXT->DepthBounce(value);
}

RC_API void RC_APIENTRY rcSetLightRadius(RCfloat radius) {
	CURRENT_RC_CONTEXT->SetLightRadius(radius);
}

RC_API void RC_APIENTRY rcSetDiffuseRate(RCuint rate) {
	CURRENT_RC_CONTEXT->SetDiffuseRate(rate);	
}

// count = 1 ~ 32
RC_API void RC_APIENTRY rcSetSPPCount(RCuint count) {
	CURRENT_RC_CONTEXT->SetSPPCount(count);
}