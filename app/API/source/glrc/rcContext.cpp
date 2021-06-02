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

#include "rcContext.h"

static ContextRC	__dummy_context;	// 더미 컨텍스트
ContextRC*	ContextRC::m_pCurrentContext	= &__dummy_context;

ContextRC::ContextRC(void){
}

ContextRC::~ContextRC(void){
	if(m_pCurrentContext == this) m_pCurrentContext = &__dummy_context;
}

void ContextRC::SetCurrent(void){
	m_pCurrentContext	= this;
}

ContextRC* ContextRC::SoothfastContext(void){
	if(m_pCurrentContext == &__dummy_context) return NULL;
	return m_pCurrentContext;
}

// dummy operation
void ContextRC::AlphaFunc (RCenum func, RCclampf ref){SetError(RC_INVALID_OPERATION);}
void ContextRC::ClearColor (RCclampf red, RCclampf green, RCclampf blue, RCclampf alpha){SetError(RC_INVALID_OPERATION);}
void ContextRC::ClearDepth (RCclampf depth){SetError(RC_INVALID_OPERATION);}
void ContextRC::ClipPlane (RCenum plane, const RCfloat *equation){SetError(RC_INVALID_OPERATION);}
void ContextRC::Color4 (RCfloat red, RCfloat green, RCfloat blue, RCfloat alpha){SetError(RC_INVALID_OPERATION);}
void ContextRC::DepthRange (RCclampf zNear, RCclampf zFar){SetError(RC_INVALID_OPERATION);}
void ContextRC::Fog (RCenum pname, RCfloat param){SetError(RC_INVALID_OPERATION);}
void ContextRC::Fogv (RCenum pname, const RCfloat *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::Frustum (RCfloat left, RCfloat right, RCfloat bottom, RCfloat top, RCfloat zNear, RCfloat zFar){SetError(RC_INVALID_OPERATION);}
void ContextRC::GetClipPlane (RCenum pname, RCfloat eqn[4]){SetError(RC_INVALID_OPERATION);}
void ContextRC::GetFloatv (RCenum pname, RCfloat *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::GetLightv (RCenum light, RCenum pname, RCfloat *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::GetMaterialv (RCenum face, RCenum pname, RCfloat *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::GetTexEnvfv (RCenum env, RCenum pname, RCfloat *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::GetTexEnviv (RCenum env, RCenum pname, RCint *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::GetTexEnvxv (RCenum env, RCenum pname, RCfixed *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::GetTexParameterv (RCenum target, RCenum pname, RCint *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::LightModel (RCenum pname, RCfloat param){SetError(RC_INVALID_OPERATION);}
void ContextRC::LightModelv (RCenum pname, const RCfloat *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::Light (RCenum light, RCenum pname, RCfloat param){SetError(RC_INVALID_OPERATION);}
void ContextRC::Lightv (RCenum light, RCenum pname, const RCfloat *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::LineWidth (RCfloat width){SetError(RC_INVALID_OPERATION);}
void ContextRC::LoadMatrix (const RCfloat *m){SetError(RC_INVALID_OPERATION);}
void ContextRC::LoadIdentity (void){SetError(RC_INVALID_OPERATION);}
void ContextRC::Material (RCenum face, RCenum pname, RCfloat param){SetError(RC_INVALID_OPERATION);}
void ContextRC::Materialv (RCenum face, RCenum pname, const RCfloat *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::MultMatrix (const RCfloat *m){SetError(RC_INVALID_OPERATION);}
void ContextRC::MultiTexCoord4 (RCenum target, RCfloat s, RCfloat t, RCfloat r, RCfloat q){SetError(RC_INVALID_OPERATION);}
void ContextRC::Normal3 (RCfloat nx, RCfloat ny, RCfloat nz){SetError(RC_INVALID_OPERATION);}
void ContextRC::Ortho (RCfloat left, RCfloat right, RCfloat bottom, RCfloat top, RCfloat zNear, RCfloat zFar){SetError(RC_INVALID_OPERATION);}
void ContextRC::PointParameterf (RCenum pname, RCfloat param){SetError(RC_INVALID_OPERATION);}
void ContextRC::PointParameterfv (RCenum pname, const RCfloat *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::PointParameterx (RCenum pname, RCfixed param){SetError(RC_INVALID_OPERATION);}
void ContextRC::PointParameterxv (RCenum pname, const RCfixed *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::PointSize (RCfloat size){SetError(RC_INVALID_OPERATION);}
void ContextRC::PolygonOffset (RCfloat factor, RCfloat units){SetError(RC_INVALID_OPERATION);}
void ContextRC::Rotate (RCfloat angle, RCfloat x, RCfloat y, RCfloat z){SetError(RC_INVALID_OPERATION);}
void ContextRC::Scale (RCfloat x, RCfloat y, RCfloat z){SetError(RC_INVALID_OPERATION);}
void ContextRC::TexEnvf (RCenum target, RCenum pname, RCfloat param){SetError(RC_INVALID_OPERATION);}
void ContextRC::TexEnvfv (RCenum target, RCenum pname, const RCfloat *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::TexEnvi (RCenum target, RCenum pname, RCint param){SetError(RC_INVALID_OPERATION);}
void ContextRC::TexEnviv (RCenum target, RCenum pname, const RCint *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::TexEnvx (RCenum target, RCenum pname, RCfixed param){SetError(RC_INVALID_OPERATION);}
void ContextRC::TexEnvxv (RCenum target, RCenum pname, const RCfixed *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::TexParameter (RCenum target, RCenum pname, RCint param){SetError(RC_INVALID_OPERATION);}
void ContextRC::TexParameterv (RCenum target, RCenum pname, const RCint *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::Translate (RCfloat x, RCfloat y, RCfloat z){SetError(RC_INVALID_OPERATION);}
void ContextRC::ActiveTexture (RCenum texture){SetError(RC_INVALID_OPERATION);}
void ContextRC::BindBuffer (RCenum target, RCuint buffer){SetError(RC_INVALID_OPERATION);}
void ContextRC::BindMaterial (RCuint material){SetError(RC_INVALID_OPERATION);}
void ContextRC::BindTexture (RCenum target, RCuint texture){SetError(RC_INVALID_OPERATION);}
void ContextRC::BlendFunc (RCenum sfactor, RCenum dfactor){SetError(RC_INVALID_OPERATION);}
void ContextRC::BufferData (RCenum target, RCsizeiptr size, const RCvoid *data, RCenum usage){SetError(RC_INVALID_OPERATION);}
void ContextRC::BufferSubData (RCenum target, RCintptr offset, RCsizeiptr size, const RCvoid *data){SetError(RC_INVALID_OPERATION);}
void ContextRC::Clear (RCbitfield mask){SetError(RC_INVALID_OPERATION);}
void ContextRC::ClearStencil (RCint s){SetError(RC_INVALID_OPERATION);}
void ContextRC::ClientActiveTexture (RCenum texture){SetError(RC_INVALID_OPERATION);}
void ContextRC::ColorMask (RCboolean red, RCboolean green, RCboolean blue, RCboolean alpha){SetError(RC_INVALID_OPERATION);}
void ContextRC::ColorPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer){SetError(RC_INVALID_OPERATION);}
void ContextRC::CompressedTexImage2D (RCenum target, RCint level, RCenum internalformat, RCsizei width, RCsizei height, RCint border, RCsizei imageSize, const RCvoid *data){SetError(RC_INVALID_OPERATION);}
void ContextRC::CompressedTexSubImage2D (RCenum target, RCint level, RCint xoffset, RCint yoffset, RCsizei width, RCsizei height, RCenum format, RCsizei imageSize, const RCvoid *data){SetError(RC_INVALID_OPERATION);}
void ContextRC::CopyTexImage2D (RCenum target, RCint level, RCenum internalformat, RCint x, RCint y, RCsizei width, RCsizei height, RCint border){SetError(RC_INVALID_OPERATION);}
void ContextRC::CopyTexSubImage2D (RCenum target, RCint level, RCint xoffset, RCint yoffset, RCint x, RCint y, RCsizei width, RCsizei height){SetError(RC_INVALID_OPERATION);}
void ContextRC::CullFace (RCenum mode){SetError(RC_INVALID_OPERATION);}
void ContextRC::DeleteBuffers (RCsizei n, const RCuint *buffers){SetError(RC_INVALID_OPERATION);}
void ContextRC::DeleteMaterials (RCsizei n, const RCuint *materials){SetError(RC_INVALID_OPERATION);}
void ContextRC::DeleteTextures (RCsizei n, const RCuint *textures){SetError(RC_INVALID_OPERATION);}
void ContextRC::DepthFunc (RCenum func){SetError(RC_INVALID_OPERATION);}
void ContextRC::DepthMask (RCboolean flag){SetError(RC_INVALID_OPERATION);}
void ContextRC::DisableClientState (RCenum array){SetError(RC_INVALID_OPERATION);}
void ContextRC::DrawArrays (RCenum mode, RCint first, RCsizei count){SetError(RC_INVALID_OPERATION);}
void ContextRC::DrawElements (RCenum mode, RCsizei count, RCenum type, const RCvoid *indices){SetError(RC_INVALID_OPERATION);}
void ContextRC::Enable (RCenum cap, BOOL bEnable){SetError(RC_INVALID_OPERATION);}
void ContextRC::EnableClientState (RCenum array){SetError(RC_INVALID_OPERATION);}
void ContextRC::Finish (BOOL bSwapSurface){SetError(RC_INVALID_OPERATION);}
void ContextRC::Flush (void){SetError(RC_INVALID_OPERATION);}
void ContextRC::FrontFace (RCenum mode){SetError(RC_INVALID_OPERATION);}
void ContextRC::GetBooleanv (RCenum pname, RCboolean *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::GetBufferParameterv (RCenum target, RCenum pname, RCint *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::GenBuffers (RCsizei n, RCuint *buffers){SetError(RC_INVALID_OPERATION);}
void ContextRC::GenMaterials (RCsizei n, RCuint *materials){SetError(RC_INVALID_OPERATION);}
void ContextRC::GenTextures (RCsizei n, RCuint *textures){SetError(RC_INVALID_OPERATION);}
void ContextRC::GetFixedv (RCenum pname, RCfixed *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::GetIntegerv (RCenum pname, RCint *params){SetError(RC_INVALID_OPERATION);}
void ContextRC::GetPointerv (RCenum pname, RCvoid **params){SetError(RC_INVALID_OPERATION);}
const RCubyte * ContextRC::GetString (RCenum name){SetError(RC_INVALID_OPERATION);return 0;}
void ContextRC::Hint (RCenum target, RCenum mode){SetError(RC_INVALID_OPERATION);}
RCboolean ContextRC::IsBuffer (RCuint buffer){SetError(RC_INVALID_OPERATION);return 0;}
RCboolean ContextRC::IsEnabled (RCenum cap){SetError(RC_INVALID_OPERATION);return 0;}
RCboolean ContextRC::IsMaterial (RCuint material){SetError(RC_INVALID_OPERATION);return 0;}
RCboolean ContextRC::IsTexture (RCuint texture){SetError(RC_INVALID_OPERATION);return 0;}
void ContextRC::LogicOp (RCenum opcode){SetError(RC_INVALID_OPERATION);}
void ContextRC::MatrixMode (RCenum mode){SetError(RC_INVALID_OPERATION);}
void ContextRC::NormalPointer (RCenum type, RCsizei stride, const RCvoid *pointer){SetError(RC_INVALID_OPERATION);}
void ContextRC::PixelStorei (RCenum pname, RCint param){SetError(RC_INVALID_OPERATION);}
void ContextRC::PopMatrix (void){SetError(RC_INVALID_OPERATION);}
void ContextRC::PushMatrix (void){SetError(RC_INVALID_OPERATION);}
void ContextRC::ReadPixels (RCint x, RCint y, RCsizei width, RCsizei height, RCenum format, RCenum type, RCvoid *pixels){SetError(RC_INVALID_OPERATION);}
void ContextRC::SampleCoverage (RCclampf value, RCboolean invert){SetError(RC_INVALID_OPERATION);}
void ContextRC::SampleCoveragex (RCclampx value, RCboolean invert){SetError(RC_INVALID_OPERATION);}
void ContextRC::Scissor (RCint x, RCint y, RCsizei width, RCsizei height){SetError(RC_INVALID_OPERATION);}
void ContextRC::ShadeModel (RCenum mode){SetError(RC_INVALID_OPERATION);}
void ContextRC::StencilFunc (RCenum func, RCint ref, RCuint mask){SetError(RC_INVALID_OPERATION);}
void ContextRC::StencilMask (RCuint mask){SetError(RC_INVALID_OPERATION);}
void ContextRC::StencilOp (RCenum fail, RCenum zfail, RCenum zpass){SetError(RC_INVALID_OPERATION);}
void ContextRC::TexCoordPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer){SetError(RC_INVALID_OPERATION);}
void ContextRC::TexImage2D (RCenum target, RCint level, RCint internalformat, RCsizei width, RCsizei height, RCint border, RCenum format, RCenum type, const RCvoid *pixels){SetError(RC_INVALID_OPERATION);}
void ContextRC::TexSubImage2D (RCenum target, RCint level, RCint xoffset, RCint yoffset, RCsizei width, RCsizei height, RCenum format, RCenum type, const RCvoid *pixels){SetError(RC_INVALID_OPERATION);}
void ContextRC::VertexPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer){SetError(RC_INVALID_OPERATION);}
void ContextRC::Viewport (RCint x, RCint y, RCsizei width, RCsizei height){SetError(RC_INVALID_OPERATION);}

void ContextRC::Perspective ( RCfloat fovy, RCfloat aspect, RCfloat zNear, RCfloat zFar){SetError(RC_INVALID_OPERATION);}
void ContextRC::LookAt (RCfloat eyex, RCfloat eyey, RCfloat eyez,
	RCfloat centerx, RCfloat centery, RCfloat centerz,
	RCfloat upx, RCfloat upy, RCfloat upz){SetError(RC_INVALID_OPERATION);}

void ContextRC::CurrentPaletteMatrixOES (RCuint matrixpaletteindex){SetError(RC_INVALID_OPERATION);}
void ContextRC::LoadPaletteFromModelViewMatrixOES (void){SetError(RC_INVALID_OPERATION);}
void ContextRC::MatrixIndexPointerOES(RCint size, RCenum type, RCsizei stride, const RCvoid *pointer){SetError(RC_INVALID_OPERATION);}
void ContextRC::WeightPointerOES(RCint size, RCenum type, RCsizei stride, const RCvoid *pointer){SetError(RC_INVALID_OPERATION);}

void ContextRC::SceneAllInit(void){SetError(RC_INVALID_OPERATION);}
void ContextRC::StaticSceneBegin(void){SetError(RC_INVALID_OPERATION);}
void ContextRC::StaticSceneEnd(void){SetError(RC_INVALID_OPERATION);}

void ContextRC::TextureAlpha(RCbyte value){SetError(RC_INVALID_OPERATION);}
void ContextRC::DepthBounce(RCuint value) { SetError(RC_INVALID_OPERATION); }

// light radius
void ContextRC::SetLightRadius(RCfloat radius){SetError(RC_INVALID_OPERATION);}
// light radius

// diffuse generation rate
void ContextRC::SetDiffuseRate(RCuint radius) { SetError(RC_INVALID_OPERATION); }
// diffuse generation rate

// spp
void ContextRC::SetSPPCount(RCuint rate) { SetError(RC_INVALID_OPERATION); }
// spp

void ContextRC::SetSurface(_EGLSurface* surface){SetError(RC_INVALID_OPERATION);}
