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

#ifndef __RC_CONTEXT_H__
#define __RC_CONTEXT_H__
#include "rcCommon.h"
#include <RC1/rc_structs.h>
#include "egl/eglSurface.h"

namespace GL{
	class ContextRC :
		public ObjectT<ContextRC>,
		public ErrorHandler<RCenum, RC_NO_ERROR>
	{
	public:
		ContextRC(void);
		virtual ~ContextRC(void);

		// API entries
		virtual void AlphaFunc (RCenum func, RCclampf ref);
		virtual void ClearColor (RCclampf red, RCclampf green, RCclampf blue, RCclampf alpha);
		virtual void ClearDepth (RCclampf depth);
		virtual void ClipPlane (RCenum plane, const RCfloat *equation);
		virtual void Color4 (RCfloat red, RCfloat green, RCfloat blue, RCfloat alpha);
		virtual void DepthRange (RCclampf zNear, RCclampf zFar);
		virtual void Fog (RCenum pname, RCfloat param);
		virtual void Fogv (RCenum pname, const RCfloat *params);
		virtual void Frustum (RCfloat left, RCfloat right, RCfloat bottom, RCfloat top, RCfloat zNear, RCfloat zFar);
		virtual void GetClipPlane (RCenum pname, RCfloat eqn[4]);
		virtual void GetFloatv (RCenum pname, RCfloat *params);
		virtual void GetLightv (RCenum light, RCenum pname, RCfloat *params);
		virtual void GetMaterialv (RCenum face, RCenum pname, RCfloat *params);
		virtual void GetTexEnvfv (RCenum env, RCenum pname, RCfloat *params);
		virtual void GetTexEnviv (RCenum env, RCenum pname, RCint *params);
		virtual void GetTexEnvxv (RCenum env, RCenum pname, RCfixed *params);
		virtual void GetTexParameterv (RCenum target, RCenum pname, RCint *params);
		virtual void LightModel (RCenum pname, RCfloat param);
		virtual void LightModelv (RCenum pname, const RCfloat *params);
		virtual void Light (RCenum light, RCenum pname, RCfloat param);
		virtual void Lightv (RCenum light, RCenum pname, const RCfloat *params);
		virtual void LineWidth (RCfloat width);
		virtual void LoadMatrix (const RCfloat *m);
		virtual void LoadIdentity (void);
		virtual void Material (RCenum face, RCenum pname, RCfloat param);
		virtual void Materialv (RCenum face, RCenum pname, const RCfloat *params);		
		virtual void MultMatrix (const RCfloat *m);
		virtual void MultiTexCoord4 (RCenum target, RCfloat s, RCfloat t, RCfloat r, RCfloat q);
		virtual void Normal3 (RCfloat nx, RCfloat ny, RCfloat nz);
		virtual void Ortho (RCfloat left, RCfloat right, RCfloat bottom, RCfloat top, RCfloat zNear, RCfloat zFar);
		virtual void PointParameterf (RCenum pname, RCfloat param);
		virtual void PointParameterfv (RCenum pname, const RCfloat *params);
		virtual void PointParameterx (RCenum pname, RCfixed param);
		virtual void PointParameterxv (RCenum pname, const RCfixed *params);
		virtual void PointSize (RCfloat size);
		virtual void PolygonOffset (RCfloat factor, RCfloat units);
		virtual void Rotate (RCfloat angle, RCfloat x, RCfloat y, RCfloat z);
		virtual void Scale (RCfloat x, RCfloat y, RCfloat z);
		virtual void TexEnvf (RCenum target, RCenum pname, RCfloat param);
		virtual void TexEnvfv (RCenum target, RCenum pname, const RCfloat *params);		
		virtual void TexEnvi (RCenum target, RCenum pname, RCint param);
		virtual void TexEnviv (RCenum target, RCenum pname, const RCint *params);		
		virtual void TexEnvx (RCenum target, RCenum pname, RCfixed param);
		virtual void TexEnvxv (RCenum target, RCenum pname, const RCfixed *params);
		virtual void TexParameter (RCenum target, RCenum pname, RCint param);
		virtual void TexParameterv (RCenum target, RCenum pname, const RCint *params);
		virtual void Translate (RCfloat x, RCfloat y, RCfloat z);

		/* Available in both Common and Common-Lite profiles */
		virtual void ActiveTexture (RCenum texture);
		virtual void BindBuffer (RCenum target, RCuint buffer);
		virtual void BindMaterial (RCuint material);
		virtual void BindTexture (RCenum target, RCuint texture);
		virtual void BlendFunc (RCenum sfactor, RCenum dfactor);
		virtual void BufferData (RCenum target, RCsizeiptr size, const RCvoid *data, RCenum usage);
		virtual void BufferSubData (RCenum target, RCintptr offset, RCsizeiptr size, const RCvoid *data);
		virtual void Clear (RCbitfield mask);
		virtual void ClearStencil (RCint s);
		virtual void ClientActiveTexture (RCenum texture);
		virtual void ColorMask (RCboolean red, RCboolean green, RCboolean blue, RCboolean alpha);
		virtual void ColorPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer);
		virtual void CompressedTexImage2D (RCenum target, RCint level, RCenum internalformat, RCsizei width, RCsizei height, RCint border, RCsizei imageSize, const RCvoid *data);
		virtual void CompressedTexSubImage2D (RCenum target, RCint level, RCint xoffset, RCint yoffset, RCsizei width, RCsizei height, RCenum format, RCsizei imageSize, const RCvoid *data);
		virtual void CopyTexImage2D (RCenum target, RCint level, RCenum internalformat, RCint x, RCint y, RCsizei width, RCsizei height, RCint border);
		virtual void CopyTexSubImage2D (RCenum target, RCint level, RCint xoffset, RCint yoffset, RCint x, RCint y, RCsizei width, RCsizei height);
		virtual void CullFace (RCenum mode);
		virtual void DeleteBuffers (RCsizei n, const RCuint *buffers);
		virtual void DeleteMaterials (RCsizei n, const RCuint *materials);
		virtual void DeleteTextures (RCsizei n, const RCuint *textures);
		virtual void DepthFunc (RCenum func);
		virtual void DepthMask (RCboolean flag);
		virtual void DisableClientState (RCenum array);
		virtual void DrawArrays (RCenum mode, RCint first, RCsizei count);
		virtual void DrawElements (RCenum mode, RCsizei count, RCenum type, const RCvoid *indices);
		virtual void Enable (RCenum cap, BOOL bEnable = TRUE);
		virtual void EnableClientState (RCenum array);
		virtual void Finish (BOOL bSwapSurface = FALSE);
		virtual void Flush (void);
		virtual void FrontFace (RCenum mode);
		virtual void GetBooleanv (RCenum pname, RCboolean *params);
		virtual void GetBufferParameterv (RCenum target, RCenum pname, RCint *params);
		virtual void GenBuffers (RCsizei n, RCuint *buffers);
		virtual void GenMaterials (RCsizei n, RCuint *materials);
		virtual void GenTextures (RCsizei n, RCuint *textures);
		virtual void GetFixedv (RCenum pname, RCfixed *params);
		virtual void GetIntegerv (RCenum pname, RCint *params);
		virtual void GetPointerv (RCenum pname, RCvoid **params);
		virtual const RCubyte * GetString (RCenum name);
		virtual void Hint (RCenum target, RCenum mode);
		virtual RCboolean IsBuffer (RCuint buffer);
		virtual RCboolean IsEnabled (RCenum cap);
		virtual RCboolean IsMaterial (RCuint material);
		virtual RCboolean IsTexture (RCuint texture);		
		virtual void LogicOp (RCenum opcode);
		virtual void MatrixMode (RCenum mode);
		virtual void NormalPointer (RCenum type, RCsizei stride, const RCvoid *pointer);
		virtual void PixelStorei (RCenum pname, RCint param);
		virtual void PopMatrix (void);
		virtual void PushMatrix (void);
		virtual void ReadPixels (RCint x, RCint y, RCsizei width, RCsizei height, RCenum format, RCenum type, RCvoid *pixels);
		virtual void SampleCoverage (RCclampf value, RCboolean invert);
		virtual void SampleCoveragex (RCclampx value, RCboolean invert);
		virtual void Scissor (RCint x, RCint y, RCsizei width, RCsizei height);
		virtual void ShadeModel (RCenum mode);
		virtual void StencilFunc (RCenum func, RCint ref, RCuint mask);
		virtual void StencilMask (RCuint mask);
		virtual void StencilOp (RCenum fail, RCenum zfail, RCenum zpass);
		virtual void TexCoordPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer);
		virtual void TexImage2D (RCenum target, RCint level, RCint internalformat, RCsizei width, RCsizei height, RCint border, RCenum format, RCenum type, const RCvoid *pixels);
		virtual void TexSubImage2D (RCenum target, RCint level, RCint xoffset, RCint yoffset, RCsizei width, RCsizei height, RCenum format, RCenum type, const RCvoid *pixels);
		virtual void VertexPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer);
		virtual void Viewport (RCint x, RCint y, RCsizei width, RCsizei height);

		virtual void Perspective ( RCfloat fovy, RCfloat aspect, RCfloat zNear, RCfloat zFar);
		virtual void LookAt (RCfloat eyex, RCfloat eyey, RCfloat eyez,
								RCfloat centerx, RCfloat centery, RCfloat centerz,
								RCfloat upx, RCfloat upy, RCfloat upz);

		virtual void CurrentPaletteMatrixOES (RCuint matrixpaletteindex);
		virtual void LoadPaletteFromModelViewMatrixOES (void);
		virtual void MatrixIndexPointerOES(RCint size, RCenum type, RCsizei stride, const RCvoid *pointer);	//Internal(GLExt : GL_OES_matrix_palette group)
		virtual void WeightPointerOES(RCint size, RCenum type, RCsizei stride, const RCvoid *pointer);		//Internal(GLExt : GL_OES_matrix_palette group)

		virtual void SceneAllInit(void);
		virtual void StaticSceneBegin(void);
		virtual void StaticSceneEnd(void);

		virtual void TextureAlpha(RCbyte value);
		virtual void DepthBounce (RCuint value);

		// light radius
		virtual void SetLightRadius(RCfloat value);
		// light radius

		// diffuse generation rate
		virtual void SetDiffuseRate(RCuint radius);
		// diffuse generation rate

		// spp
		virtual void SetSPPCount(RCuint rate);
		// spp

		// EGL Surface
		virtual void SetSurface(_EGLSurface* surface);

		// Current context
		static inline ContextRC* CurrentContext(void)	{return m_pCurrentContext;}
		static ContextRC* SoothfastContext(void);
		void SetCurrent(void);

	private:
		static ContextRC*	m_pCurrentContext;
	};
};

#define CURRENT_RC_CONTEXT		(ContextRC::CurrentContext())
#define SOOTHFAST_RC_CONTEXT	(ContextRC::SoothfastContext())



#endif//__RC_CONTEXT_H__
