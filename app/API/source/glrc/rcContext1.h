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
// DATE    : 8/14/2013 Wed
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __RC_CONTEXT1_H__
#define __RC_CONTEXT1_H__

#include "rcContext.h"
#include "rcTexture.h"
#include "rcMaterial.h"
#include "rcLight.h"
#include "rcBuffer.h"

#include <RC1/rc_structs.h>
#include "driver/RCStructures.h"
#include "egl/eglSurface.h"

#include "tb/tb_TreeBuilder.h"
#include "define.h"

typedef struct ScreenSetting {
	struct{
		float   pixelSizeX; 
		float   pixelSizeY; 
		float   startPositionX; 
		float   startPositionY;
	}info;
	int			X, Y;
	int			Width, Height;
	float 		clearRed, clearGreen, clearBlue, clearAlpha;
	struct{
		RCVECTOR3f		eye;
		RCVECTOR3f		basis[3];
		float			distance;
		BYTE			attenFlag;
		float			attenStart, attenEnd;
		float			atten[3];
	}RayCamera;
} SCREEN;

typedef struct VERTEX_ARRAY {
	int					size;
	int					type;
	int					stride;
	const void *		pData;
	BOOL				enabled;
	int					boundBuffer;
} VERTEX_ARRAY;

typedef struct VERTEX_ELEMENT {
    float _v[3];
    float _n[3];
    float _c[3];
    float _t[2];
} VERTEX_ELEMENT;

typedef struct RENDER_DATA {

	BOUNDING_BOX            boundBox;

	int                     startPrimitive; // TriStartIndex
	int                     startList;      // ListStartIndex
	int                     startNode;      // NodeStartIndex

	int                     endPrimitive;
	int                     endList;
	int                     endNode;

	int                     nodeCount;      // NodeCount
	int                     listCount;      // ListCount
	int                     primitiveCount;
	int                     materialCount;

} RENDER_DATA;

typedef struct RCMATRIX16F {
	float   m[16];
} RCMATRIX16F;

typedef struct MATRIX_STACK{
	RCMATRIX16F     V_matrix;
	RCMATRIX16F     N_matrix;
} MATRIX_STACK;

typedef struct MATRIX_PALETTE{
	RCMATRIX16F     matrix;
	RCMATRIX16F     matrixInv;
} MATRIX_PALETTE;

class ContextRC1 : public ContextRC
{
public:
	ContextRC1(RCDDK* pDDK = NULL);
	virtual ~ContextRC1(void);

	// API entries (지원하지 않는 함수는 주석 처리되어 있음.)
	//virtual void AlphaFunc (RCenum func, RCclampf ref);
	virtual void ClearColor (RCclampf red, RCclampf green, RCclampf blue, RCclampf alpha);
	//virtual void ClearDepth (RCclampf depth);
	//virtual void ClipPlane (RCenum plane, const RCfloat *equation);
	virtual void Color4 (RCfloat red, RCfloat green, RCfloat blue, RCfloat alpha);
	//virtual void DepthRange (RCclampf zNear, RCclampf zFar);
	//virtual void Fog (RCenum pname, RCfloat param);
	//virtual void Fogv (RCenum pname, const RCfloat *params);
	virtual void Frustum (RCfloat left, RCfloat right, RCfloat bottom, RCfloat top, RCfloat zNear, RCfloat zFar);
	//virtual void GetClipPlane (RCenum pname, RCfloat eqn[4]);
	virtual void GetFloatv (RCenum pname, RCfloat *params);

	virtual void GetLightv (RCenum light, RCenum pname, RCfloat *params);
	virtual void GetMaterialv (RCenum face, RCenum pname, RCfloat *params);
	//virtual void GetTexEnvfv (RCenum env, RCenum pname, RCfloat *params);
	//virtual void GetTexEnviv (RCenum env, RCenum pname, RCint *params);
	//virtual void GetTexEnvxv (RCenum env, RCenum pname, RCfixed *params);
	virtual void GetTexParameterv (RCenum target, RCenum pname, RCint *params);
	//virtual void LightModelf (RCenum pname, RCfloat param);
	//virtual void LightModelfv (RCenum pname, const RCfloat *params);
	//virtual void LightModelx (RCenum pname, RCfixed param);
	//virtual void LightModelxv (RCenum pname, const RCfixed *params);
	virtual void Light (RCenum light, RCenum pname, RCfloat param);
	virtual void Lightv (RCenum light, RCenum pname, const RCfloat *params);
	virtual void LoadMatrix (const RCfloat *m);
	virtual void LoadIdentity (void);
	virtual void MultMatrix (const RCfloat *m);
	//virtual void MultiTexCoord4 (RCenum target, RCfloat s, RCfloat t, RCfloat r, RCfloat q);
	//virtual void Normal3 (RCfloat nx, RCfloat ny, RCfloat nz);
	//virtual void Ortho (RCfloat left, RCfloat right, RCfloat bottom, RCfloat top, RCfloat zNear, RCfloat zFar);
	
	//virtual void CullFace (RCenum mode);
	virtual void DeleteBuffers (RCsizei n, const RCuint *buffers);
	virtual void DeleteMaterials (RCsizei n, const RCuint *materials);
	virtual void DeleteTextures (RCsizei n, const RCuint *textures);
	
	//virtual void DepthFunc (RCenum func);
	//virtual void DepthMask (RCboolean flag);
	virtual void DisableClientState (RCenum array);
	virtual void DrawArrays (RCenum mode, RCint first, RCsizei count);
	virtual void DrawElements (RCenum mode, RCsizei count, RCenum type, const RCvoid *indices);
	virtual void Enable(RCenum cap, BOOL bEnable = TRUE);
	virtual void EnableClientState (RCenum array);
	virtual void Finish (BOOL bSwapSurface = FALSE);
	virtual void Flush (void);

	//virtual void FrontFace (RCenum mode);
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
	virtual RCboolean IsMaterial (RCuint Material);
	virtual RCboolean IsTexture (RCuint texture);

	//virtual void LogicOp (RCenum opcode);
	virtual void MatrixMode (RCenum mode);
	virtual void NormalPointer (RCenum type, RCsizei stride, const RCvoid *pointer);

	//virtual void LineWidth (RCfloat width);

	virtual void Material (RCenum face, RCenum pname, RCfloat param);
	virtual void Materialv (RCenum face, RCenum pname, const RCfloat *params);

	//virtual void PointParameterf (RCenum pname, RCfloat param);
	//virtual void PointParameterfv (RCenum pname, const RCfloat *params);
	//virtual void PointParameterx (RCenum pname, RCfixed param);
	//virtual void PointParameterxv (RCenum pname, const RCfixed *params);
	//virtual void PointSize (RCfloat size);
	//virtual void PolygonOffset (RCfloat factor, RCfloat units);
	virtual void Rotate (RCfloat angle, RCfloat x, RCfloat y, RCfloat z);
	virtual void Scale (RCfloat x, RCfloat y, RCfloat z);
	//virtual void TexEnvf (RCenum target, RCenum pname, RCfloat param);
	//virtual void TexEnvfv (RCenum target, RCenum pname, const RCfloat *params);		
	//virtual void TexEnvi (RCenum target, RCenum pname, RCint param);
	//virtual void TexEnviv (RCenum target, RCenum pname, const RCint *params);		
	//virtual void TexEnvx (RCenum target, RCenum pname, RCfixed param);
	//virtual void TexEnvxv (RCenum target, RCenum pname, const RCfixed *params);
	virtual void TexParameter (RCenum target, RCenum pname, RCint param);
	virtual void TexParameterv (RCenum target, RCenum pname, const RCint *params);

	virtual void Translate (RCfloat x, RCfloat y, RCfloat z);

	virtual void Viewport (RCint x, RCint y, RCsizei width, RCsizei height);
	
	virtual void Perspective ( RCfloat fovy, RCfloat aspect, RCfloat zNear, RCfloat zFar);

	//virtual void ActiveTexture (RCenum texture);
	virtual void BindBuffer (RCenum target, RCuint buffer);
	virtual void BindMaterial (RCuint material);
	virtual void BindTexture (RCenum target, RCuint texture);
	//virtual void BlendFunc (RCenum sfactor, RCenum dfactor);
	virtual void BufferData (RCenum target, RCsizeiptr size, const RCvoid *data, RCenum usage);
	virtual void BufferSubData (RCenum target, RCintptr offset, RCsizeiptr size, const RCvoid *data);

	virtual void Clear (RCbitfield mask);
	//virtual void ClearStencil (RCint s);
	//virtual void ClientActiveTexture (RCenum texture);
	//virtual void ColorMask (RCboolean red, RCboolean green, RCboolean blue, RCboolean alpha);
	virtual void ColorPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer);
	//virtual void CompressedTexImage2D (RCenum target, RCint level, RCenum internalformat, RCsizei width, RCsizei height, RCint border, RCsizei imageSize, const RCvoid *data);
	//virtual void CompressedTexSubImage2D (RCenum target, RCint level, RCint xoffset, RCint yoffset, RCsizei width, RCsizei height, RCenum format, RCsizei imageSize, const RCvoid *data);
	//virtual void CopyTexImage2D (RCenum target, RCint level, RCenum internalformat, RCint x, RCint y, RCsizei width, RCsizei height, RCint border);
	//virtual void CopyTexSubImage2D (RCenum target, RCint level, RCint xoffset, RCint yoffset, RCint x, RCint y, RCsizei width, RCsizei height);
	//virtual void PixelStorei (RCenum pname, RCint param);
	virtual void PopMatrix (void);
	virtual void PushMatrix (void);
	virtual void ReadPixels (RCint x, RCint y, RCsizei width, RCsizei height, RCenum format, RCenum type, RCvoid *pixels);
	//virtual void SampleCoverage (RCclampf value, RCboolean invert);
	//virtual void SampleCoveragex (RCclampx value, RCboolean invert);
	//virtual void Scissor (RCint x, RCint y, RCsizei width, RCsizei height);
	//virtual void ShadeModel (RCenum mode);
	//virtual void StencilFunc (RCenum func, RCint ref, RCuint mask);
	//virtual void StencilMask (RCuint mask);
	//virtual void StencilOp (RCenum fail, RCenum zfail, RCenum zpass);

	virtual void TexCoordPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer);
	virtual void TexImage2D (RCenum target, RCint level, RCint internalformat, RCsizei width, RCsizei height, RCint border, RCenum format, RCenum type, const RCvoid *pixels);
	virtual void TexSubImage2D (RCenum target, RCint level, RCint xoffset, RCint yoffset, RCsizei width, RCsizei height, RCenum format, RCenum type, const RCvoid *pixels);

	virtual void VertexPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer);
	virtual void LookAt (RCfloat eyex, RCfloat eyey, RCfloat eyez,
								RCfloat centerx, RCfloat centery, RCfloat centerz,
								RCfloat upx, RCfloat upy, RCfloat upz);

	virtual void CurrentPaletteMatrixOES (RCuint matrixpaletteindex);
	virtual void LoadPaletteFromModelViewMatrixOES (void);
	virtual void MatrixIndexPointerOES(RCint size, RCenum type, RCsizei stride, const RCvoid *pointer);
	virtual void WeightPointerOES(RCint size, RCenum type, RCsizei stride, const RCvoid *pointer);

	virtual void SceneAllInit(void);
	virtual void StaticSceneBegin(void);
	virtual void StaticSceneEnd(void);
	virtual void DynamicSceneEnd(void);

	virtual void TextureAlpha(RCbyte value);
	virtual void DepthBounce(RCuint value);

	// light radius
	virtual void SetLightRadius(RCfloat value) override;
	// light radius

	// diffuse generation rate
	virtual void SetDiffuseRate(RCuint rate) override;
	// diffuse generation rate

	// spp
	virtual void SetSPPCount(RCuint rate) override;
	// spp

	// EGL Surface
	virtual void SetSurface(_EGLSurface* surface);

#ifndef CROSS_COMPILE
	void PrimitiveUpdate(void);
#endif

private:
	typedef void (ContextRC1::*DrawPrimitiveFunction)(int index);

	//
	RCint GetRoundInt(RCfloat f);

	// 
	void Begin(RCenum mode);
	void End();
	void RenderDataInit(RENDER_DATA *rData);

	//
	void ReleaseVertexArrays(RCuint buffer);
	void ResetVertexArray(VERTEX_ARRAY* pVertexArray);

	// Matrix Function
	void UdpateTransmitData(DWORD name);
	void NormalVectorGenerate( float *a, float *b, float *c , float *out);
	void TransformNormal( float u[4], const float v[4], const float m[16] );
	void TransformVertex( float u[4], const float v[4], const float m[16] );
	void NormalizeV(float *v);
	void MatrixCopy( float *product, float *m);
	void MatrixMultf(float *product, const float *a, const float *b);
	void MatrixMultW(float *product, const float *a, const float w);
	RCboolean InverseMatrix(const float *m, float *inv);
	void MatrixWeghitAccum(float *product, const float *a, const float w);
	void CalculateInverseViewMatrix(float *inverseViewMatrix);
	void CalcStartPixelbyProjection(const float *m);
	void CalcStartPixelbyProjectionInLHCoordSys(const float *m);

	MATRIX_PALETTE* GetMatrixPaletteIndex(RCuint index);
	void SetMatrixPaletteIndex(RCuint index, const float *m);
	void CalculateMatrixPalette(int index);
	
	// Triangle Draw Function
	void RenderTriangle(VERTEX_ELEMENT *a, VERTEX_ELEMENT *b, VERTEX_ELEMENT *c);
	void DrawTriangleStrip(int index);
	void DrawTriangleFan(int index);
	void DrawTriangle(int index);
	void DrawQuards(int index);
	BOOL SelectArrayElement(int index);
	void SetModified(void);
	// 
	double MipMapPreProcess(float tex_coord[3][2], RCuint usingMaterial);
	void MakePrimitiveByTriangle(RC_VERTEX *triangle);
	
private:
	/************************************************************************/
	/* Global Setting Values                                                */
	/************************************************************************/

	// light radius
	RCfloat					m_lightRadius;
	// light radius

	// diffuse generation rate
	RCushort				m_diffuseRate;
	// diffuse generation rate

	// spp
	RCushort				m_sppCount;
	// spp

	RCuint			        m_iDepthBounce;
	RCuint			        m_iAdaptiveSize;

	SCREEN				    m_Screen;
	rcLight					m_Light;
	
	VERTEX_ARRAY			m_VertexArray;
	VERTEX_ARRAY			m_NormalArray;
	VERTEX_ARRAY			m_ColorArray;
	VERTEX_ARRAY			m_TexCoordArray;

	VERTEX_ARRAY			m_MatrixIndexArray;
	VERTEX_ARRAY            m_WeightArray;

	float					m_fDefaultColor[4];         // color info [r, g, b, a]

	float					m_fRayThresholdWeight;		// ThWeight ( 0 ~ 1.0 )

	/************************************************************************/
	/* Matrix Transform                                                     */
	/************************************************************************/

	RCMATRIX16F             m_MatrixProjection;
	RCMATRIX16F             m_MatrixProjectionInverse;
	RCMATRIX16F             m_MatrixModelToWorld;
	RCMATRIX16F             m_MatrixModelToWorldInverse;
	RCMATRIX16F             m_MatrixTexture;
	RCMATRIX16F             m_MatrixTextureInverse;
	MATRIX_STACK            m_MatrixStackModelView[RCDDK_MAX_MATRIX_STACK_COUNT];
	MATRIX_STACK            m_MatrixStackProjection[RCDDK_MAX_MATRIX_STACK_COUNT];
	MATRIX_STACK            m_MatrixStackTexture[RCDDK_MAX_MATRIX_STACK_COUNT];

	int                     m_iMatrixArrayIndex;
	MATRIX_PALETTE          m_MatrixArray[RCDDK_MAX_MATRIX_PALETTE_COUNT];
	MATRIX_PALETTE          m_MatrixPalette;

	int 				    m_iMatrixStackIndexModelView;
	int 				    m_iMatrixStackIndexProjection;
	int 				    m_iMatrixStackIndexTexture;

	/************************************************************************
	* Scene Render Data                                                    *
	************************************************************************/

	RENDER_DATA             m_StaticRenderData;
	RENDER_DATA             m_DynamicRenderData;

	/************************************************************************
	*    Hardware Ready Data                                               *
	************************************************************************/
	struct{
		MemoryPoolT<DDK_NODE>*			pNode;
		MemoryPoolT<DDK_LIST>*				pList;
		MemoryPoolT<DDK_PRIMITIVE>*				pPrimitive;
		MemoryPoolT<DDK_PRIMITIVE_INFO>*	pPrimitiveInfo;
		Memory*			pTransmit;
	}m_Memory;
	
	BOUNDING_BOX		m_BoundBox;

	DWORD				m_dwTriangleIndex;   // triangle Counting for every Object
	DWORD				m_dwTriangleMode;


	VERTEX_ELEMENT		m_InputVertexElement[4];			// for primitive rendering


	DWORD				m_dwCurrentMatrixMode;

	/************************************************************************
	*    Acceleration Structure Data                                       *
	************************************************************************/
	TreeBuilder				m_TreeBuilder;
	
	PoolMemoryT<RC_PRIMITIVE>	m_PrimitiveSource;	// for S/W 관리.

	/**************************************************************************/
	/*  FLAG SIGNAL                                                           */
	/**************************************************************************/

	DWORD 		            m_dwBitFlag;

	DWORD					m_dwPrimitiveState;
	DWORD					m_dwNextIndex;
		
	DrawPrimitiveFunction	m_pfnDrawPrimitive;

	float                   m_afIdentity[16];

	//--------------------------------------------------------------------------
	// object container
	//--------------------------------------------------------------------------
	struct{
		ObjectContainerT<rcMaterial>	Material;
		ObjectContainerT<rcTexture>		Texture;
		ObjectContainerT<rcBuffer>		Buffer;
	}m_Container;
	//--------------------------------------------------------------------------
	// current object
	//--------------------------------------------------------------------------
	struct{
		rcMaterial*		pMaterial;
		rcTexture*		pTexture;
		rcBuffer*		pBuffer;
		rcBuffer*		pElementBuffer;
	}m_Current;

	struct{
		union{
			DWORD			flags;
			struct{
				DWORD			shadow				: 1;
				DWORD			color_shadow		: 1;
				DWORD			lod					: 1;
				DWORD			super_sampling		: 1;
				DWORD			adaptive_render		: 1;	// enable adaptive rendering
				DWORD			/*dummy*/			: 0;
			};
		}option;

		union{
			DWORD			flags;
			struct{
				DWORD			transmittance_shadow	: 1;
				DWORD			texture_only			: 1;
				DWORD			texture_alpha_shadow	: 1;
				DWORD			/*dummy*/				: 0;
			};
		}primitive;

		union{
			DWORD			flags;
			struct{
				DWORD			mipmap						: 1;
				DWORD			background_set				: 1;
				DWORD			background_setting			: 1;
				DWORD			matrix_modelview_inverse	: 1;
				DWORD			matrix_palette				: 1;
				DWORD			/*dummy*/					: 0;
			};
		}etc;
	}m_CurrentStateFlag;
	
	//--------------------------------------------------------------------------
	// texture interface
	//--------------------------------------------------------------------------
	unsigned int		m_TextureMinFilter;	// not implemented yet.
	unsigned int		m_TextureMagFilter;	// not implemented yet.
	BYTE				m_TextureAlpha;
	BOOL				m_bGenerateMipmap;
	BOOL				m_bMustDoFinish;

	/**************************************************************************/
	/*  EGL & DDK                                                             */
	/**************************************************************************/

	_EGLSurface*			m_pSurface;
	RCDDK*					m_pDDK;			// DDK interface
};

#endif//__RC_CONTEXT1_H__

