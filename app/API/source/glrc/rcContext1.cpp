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
// DATE    : 9/24/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "rcContext1.h"
#include "define.h"

#include "primitive/primitive.h"

static RCfloat IDENTITY[16] = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};

ContextRC1::ContextRC1(RCDDK* pDDK){
	assert(pDDK != NULL);
	m_pDDK			= pDDK;
	m_bMustDoFinish	= TRUE;

	//--------------------------------------------------------------------------
	// default object initialization
	memset(&m_Current, 0, sizeof(m_Current));
	memset(&m_CurrentStateFlag, 0, sizeof(m_CurrentStateFlag));
	memset(&m_Screen, 0, sizeof(SCREEN));
	
	memset(&m_VertexArray, 0, sizeof(VERTEX_ARRAY));
	memset(&m_NormalArray, 0, sizeof(VERTEX_ARRAY));
	memset(&m_ColorArray, 0, sizeof(VERTEX_ARRAY));
	memset(&m_TexCoordArray, 0, sizeof(VERTEX_ARRAY));
	memset(&m_MatrixIndexArray, 0, sizeof(VERTEX_ARRAY));
	memset(&m_WeightArray, 0, sizeof(VERTEX_ARRAY));

	m_VertexArray.boundBuffer		=
	m_NormalArray.boundBuffer		=
	m_ColorArray.boundBuffer		=
	m_TexCoordArray.boundBuffer		=
	m_MatrixIndexArray.boundBuffer	=
	m_WeightArray.boundBuffer		= 0;

	m_ColorArray.type				= RC_FLOAT;
	m_ColorArray.size				= 4;
	m_ColorArray.stride				= 0;
	
	memset(m_MatrixStackProjection, 0, sizeof(RCMATRIX16F)*RCDDK_MAX_MATRIX_STACK_COUNT);
	m_iMatrixStackIndexProjection	= 0;

	memset(m_MatrixStackModelView, 0, sizeof(RCMATRIX16F)*RCDDK_MAX_MATRIX_STACK_COUNT);
	m_iMatrixStackIndexModelView	= 0;

	memset(m_MatrixStackTexture, 0, sizeof(RCMATRIX16F)*RCDDK_MAX_MATRIX_STACK_COUNT);
	m_iMatrixStackIndexTexture		= 0;

	memset(m_MatrixArray, 0, sizeof(MATRIX_PALETTE)*RCDDK_MAX_MATRIX_PALETTE_COUNT);
	memset(&m_MatrixPalette, 0, sizeof(MATRIX_PALETTE));
	m_iMatrixArrayIndex				= 0;

	memset(&m_StaticRenderData, 0, sizeof(RENDER_DATA));
	memset(&m_DynamicRenderData, 0, sizeof(RENDER_DATA));

	memset(&m_BoundBox, 0, sizeof(BOUNDING_BOX));

	m_dwTriangleIndex		= 0;
	m_dwTriangleMode		= 0;

	memset(m_InputVertexElement, 0, sizeof(VERTEX_ELEMENT)*4);



	m_dwBitFlag				= 0;
	m_dwPrimitiveState		= 0;
	m_dwNextIndex			= 0;

	m_pfnDrawPrimitive		= NULL;


	// Initialize
	SetCurrent();

	{
		m_Memory.pNode				= new MemoryPoolT<DDK_NODE>;
		m_Memory.pList				= new MemoryPoolT<DDK_LIST>;
		m_Memory.pPrimitive			= new MemoryPoolT<DDK_PRIMITIVE>;
		m_Memory.pPrimitiveInfo		= new MemoryPoolT<DDK_PRIMITIVE_INFO>;
		m_Memory.pTransmit			= new Memory;

		m_Memory.pNode->			Create(4096*sizeof(DDK_NODE), 64);
		m_Memory.pList->			Create(4096*sizeof(DDK_LIST), 64);
		m_Memory.pPrimitive->		Create(4096*sizeof(DDK_PRIMITIVE), 64);
		m_Memory.pPrimitiveInfo->	Create(4096*sizeof(DDK_PRIMITIVE_INFO), 64);
		m_Memory.pTransmit->		Create(RCDDK_MAX_MATERIAL_COUNT*sizeof(RC_TRANSMITTANCE_DATA), 64);

		pDDK->SetMemory(RCDDK_MEMORY_TRANSMIT,			m_Memory.pTransmit->GetNative());

		// 안쓰는 Transmit 버퍼 초기화 (@todo : 실제로 사용하는지 확인이 필요함.)
		memset(m_Memory.pTransmit->Virtual(), 0, RCDDK_MAX_MATERIAL_COUNT*sizeof(DWORD)*2);
		m_Memory.pTransmit->SetDirty(0, RCDDK_MAX_MATERIAL_COUNT*sizeof(DWORD)*2);

		m_TreeBuilder.Initialize(m_Memory.pNode, m_Memory.pList);
	}

	BindMaterial(0);					// create default material
	m_Container.Buffer.Create(0);		// create default buffer object
	BindTexture(RC_TEXTURE_2D, 0);		// create default texture
	MatrixMode(RC_MODELVIEW);

	m_dwBitFlag			= 0;

	m_Screen.X			= 0;
	m_Screen.Y			= 0;
	m_Screen.Width		= 800;
	m_Screen.Height		= 480;

	m_Screen.RayCamera.distance		= 1;

//	VECTOR3_SUBSTITUTE(m_Screen.RayCamera.eye.m, 0);	// 위에서 memset 으로 초기화됨.

	VECTOR3_SUBSTITUTE_3(m_Screen.RayCamera.basis[0].m, 1, 0, 0);
	VECTOR3_SUBSTITUTE_3(m_Screen.RayCamera.basis[1].m, 0, 1, 0);
	VECTOR3_SUBSTITUTE_3(m_Screen.RayCamera.basis[2].m, 0, 0, 1);

	m_Screen.info.pixelSizeX		=  0.001380712f;
	m_Screen.info.pixelSizeY		=  0.00172589f;
	m_Screen.info.startPositionX	= -0.000690356f;
	m_Screen.info.startPositionY	= -0.000862945f;

	ClearColor(0.f, 0.f, 0.f, 0.f);
	Color4(1.f, 1.f, 1.f, 1.f);

	memcpy(m_afIdentity, IDENTITY, sizeof(IDENTITY));
	memcpy(&m_MatrixProjection.m, IDENTITY, sizeof(IDENTITY));
	memcpy(&m_MatrixProjectionInverse.m, IDENTITY, sizeof(IDENTITY));
	memcpy(m_MatrixModelToWorld.m, IDENTITY, sizeof(IDENTITY));
	memcpy(m_MatrixModelToWorldInverse.m, IDENTITY, sizeof(IDENTITY));
	memcpy(m_MatrixTexture.m, IDENTITY, sizeof(IDENTITY));
	memcpy(m_MatrixTextureInverse.m, IDENTITY, sizeof(IDENTITY));
		
	/************************************************************************/
	/* Texture Setting                                                      */
	/************************************************************************/
	TexParameter(RC_TEXTURE_2D, RC_TEXTURE_MIN_FILTER, RC_NEAREST_MIPMAP_LINEAR);
	TexParameter(RC_TEXTURE_2D, RC_TEXTURE_MAG_FILTER, RC_LINEAR);
	TexParameter(RC_TEXTURE_2D, RC_GENERATE_MIPMAP, TRUE);
	m_PrimitiveSource.Get(0);	// 미리 초기 메모리 할당.

	TexParameter(RC_TEXTURE_2D, RC_GENERATE_MIPMAP, RC_TRUE);
	TexParameter(RC_TEXTURE_2D_NORMAL, RC_GENERATE_MIPMAP, RC_TRUE);
	
	TextureAlpha(16);

	Enable(RC_USE_COLOR_SHADOW);
	Enable(RC_USE_SHADOW);
	Hint(RC_MIPMAP_HINT, RC_FASTEST);
	Hint(RC_RENDERING_HINT, RC_DONT_CARE);
	
	VECTOR3_SUBSTITUTE(m_BoundBox.maximum.m, FLOAT_MIN);
	VECTOR3_SUBSTITUTE(m_BoundBox.minimum.m, FLOAT_MAX);

	SceneAllInit();
	m_pSurface	= NULL;

	DepthBounce(m_iDepthBounce);
	Hint(RC_RAYBOUNCE_THRESHOLD_HINT, RC_DONT_CARE);

	ELAPSED_TIME_SET(TOTAL);
}

ContextRC1::~ContextRC1(void){
	// actually it does not need, but we do this for object management conception.
	BindMaterial(0);
	BindTexture(RC_TEXTURE_2D, 0);
	SAFE_RELEASE(m_Current.pMaterial);	// release default material
	SAFE_RELEASE(m_Current.pTexture);	// release default texture
	
	m_Container.Material.DeleteAll();	// 명시적으로 texture 보다 먼저 지워져야 한다.

	// context memory
	SAFE_RELEASE(m_Memory.pNode);
	SAFE_RELEASE(m_Memory.pList);
	SAFE_RELEASE(m_Memory.pPrimitive);
	SAFE_RELEASE(m_Memory.pPrimitiveInfo);
	SAFE_RELEASE(m_Memory.pTransmit);

	if(m_pSurface){
		m_pSurface->Release();
		m_pSurface	= NULL;
	}
}

void ContextRC1::ClearColor (RCclampf red, RCclampf green, RCclampf blue, RCclampf alpha){
	m_Screen.clearRed	= red;
	m_Screen.clearGreen	= green;
	m_Screen.clearBlue	= blue;
	m_Screen.clearAlpha	= alpha;

	m_pDDK->SetBackgroundColor(red, green, blue);
}

void ContextRC1::Color4 (RCfloat red, RCfloat green, RCfloat blue, RCfloat alpha){
	float rgbanull[]={0, 0, 0, 0};

	m_fDefaultColor[0] = red;
	m_fDefaultColor[1] = green;
	m_fDefaultColor[2] = blue;
	m_fDefaultColor[3] = alpha;
	
	// @todo : rcColor와 rcMaterial 중 최근에 호출된 것의 영향을 받는다.
	Materialv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE,	m_fDefaultColor);
	Materialv(RC_FRONT_AND_BACK, RC_SPECULAR,				rgbanull);
}

void ContextRC1::Frustum (RCfloat left, RCfloat right, RCfloat bottom, RCfloat top, RCfloat zNear, RCfloat zFar){
	float ymax;
	float xmax;

	if(zNear <= 0 || zFar <= 0
		|| left == right || bottom == top || zNear == zFar){
		SetError(RC_INVALID_VALUE);
		return;
	}

	ymax = top - bottom;
	xmax = right - left;
	if (ymax < 0)	ymax = -ymax;
	if (xmax < 0)	xmax = -xmax;

	m_Screen.info.pixelSizeX = xmax / m_Screen.Width;	// @todo : Width의 설정 순서의 의존성이 발생된다.
	m_Screen.info.pixelSizeY = ymax / m_Screen.Height;	// @todo : Height의 설정 순서의 의존성이 발생된다.

	m_Screen.info.startPositionX = -xmax / 2.0f;
	m_Screen.info.startPositionY = -ymax / 2.0f;

	//    Distance = zNear;
	m_Screen.RayCamera.distance = zNear; //???????
}

void ContextRC1::Enable(RCenum cap, BOOL bEnable) {


	if (cap >= RC_LIGHT0 && cap <= RC_LIGHT1023)
	{
		if (!m_Light.Enable(cap, bEnable))
		{
			SetError(RC_INVALID_ENUM);
			return;
		}		

		return;
	}

	switch (cap) {
	case RC_ALPHA_TEST:		//rcAlphaFunc
		break;

	case RC_BLEND:			//rcBlendFunc
	case RC_COLOR_LOGIC_OP:	//rcLogicOp
	case RC_CLIP_PLANE0:	//rcClipPlane
	case RC_CLIP_PLANE1:	//rcClipPlane
	case RC_CLIP_PLANE2:	//rcClipPlane
	case RC_CLIP_PLANE3:	//rcClipPlane
	case RC_CLIP_PLANE4:	//rcClipPlane
	case RC_CLIP_PLANE5:	//rcClipPlane
	case RC_COLOR_MATERIAL:	
	case RC_CULL_FACE:		//rcCullFace
	case RC_DEPTH_TEST:		//rcDepthFunc and rcDepthRange
	case RC_DITHER:			
	case RC_FOG:			//rcFog
		break;

	//case RC_LIGHT0:
	//case RC_LIGHT1:
	//case RC_LIGHT2:
	//case RC_LIGHT3:
	//case RC_LIGHT4:
	//case RC_LIGHT5:
	//case RC_LIGHT6:
	//case RC_LIGHT7:
	//case RC_LIGHT8:
	//case RC_LIGHT9:
	//case RC_LIGHT10:
	//case RC_LIGHT11:
	//	if(!m_Light.Enable(cap, bEnable))
	//		SetError(RC_INVALID_ENUM);
	//	break;
	case RC_LIGHTING:
		m_Light.Lighting(bEnable);
		break;

	case RC_LINE_SMOOTH:			//rcLineWidth
	case RC_MULTISAMPLE:			//rcSampleCoverage
	case RC_NORMALIZE:				//rcNormal and rcNormalPointer, RC_RESCALE_NORMAL
	case RC_POINT_SMOOTH:			//rcPointSize
	case RC_POINT_SPRITE_OES:		//rcPointSize and rcTexEnv 
	case RC_POLYGON_OFFSET_FILL:	//rcPolygonOffset
	case RC_RESCALE_NORMAL:			//rcNormal and rcNormalPointer, RC_NORMALIZE
	case RC_SAMPLE_ALPHA_TO_COVERAGE:
	case RC_SAMPLE_ALPHA_TO_ONE:
	case RC_SAMPLE_COVERAGE:		//rcSampleCoverage
	case RC_SCISSOR_TEST:			//rcScissor
	case RC_STENCIL_TEST:			//rcStencilFunc, rcStencilMask, and rcStencilOp.
		break;
		
	case RC_TEXTURE_2D:
		{
			RCDDK_TEXTURE_TYPE oldTextureType = m_Current.pMaterial->GetTextureType();
			m_Current.pMaterial->ActiveTexture(RCDDK_TEXTURE_TYPE_DIFFUSE);
			m_Current.pMaterial->EnableTexture(bEnable);
			if(!bEnable)	m_Current.pMaterial->ActiveTexture(oldTextureType);
		}		
		break;
	case RC_TEXTURE_2D_NORMAL:
		{
			RCDDK_TEXTURE_TYPE oldTextureType = m_Current.pMaterial->GetTextureType();
			m_Current.pMaterial->ActiveTexture(RCDDK_TEXTURE_TYPE_NORMAL);
			m_Current.pMaterial->EnableTexture(bEnable);
			if(!bEnable)	m_Current.pMaterial->ActiveTexture(oldTextureType);
		}		
		break;
	case RC_USE_COLOR_SHADOW:
		m_CurrentStateFlag.option.color_shadow = bEnable;
		m_pDDK->Enable(RCDDK_ENABLE_COLOR_SHADOW, bEnable);
		break;
	case RC_USE_SHADOW:
		m_CurrentStateFlag.option.shadow = bEnable;
		m_pDDK->Enable(RCDDK_ENABLE_SHADOW, bEnable);
		break;
	case RC_USE_TEXTURE_ALPHA_SHADOW:
		m_CurrentStateFlag.primitive.texture_alpha_shadow = bEnable;
		break;
	case RC_USE_TEXTURE_ONLY:
		m_CurrentStateFlag.primitive.texture_only = bEnable;
		break;
	case RC_USE_TRANSMITTANCE_SHADOW:
		m_CurrentStateFlag.primitive.transmittance_shadow = bEnable;
		break;
	case RC_MATRIX_PALETTE_OES:
		m_CurrentStateFlag.etc.matrix_palette = bEnable;
		m_iMatrixArrayIndex = 0;
		break;

	default:
		SetError(RC_INVALID_ENUM);
		return;
	}
}

void ContextRC1::Begin(RCenum mode) {
	rcMaterial*	pMat	= m_Current.pMaterial;
	rcTexture* pTex		= m_Current.pTexture;
	m_dwPrimitiveState = 0;
	m_dwNextIndex = 0;

	switch (mode) {
	case RC_TRIANGLES:		m_pfnDrawPrimitive = &ContextRC1::DrawTriangle;			break;
	case RC_TRIANGLE_STRIP:	m_pfnDrawPrimitive = &ContextRC1::DrawTriangleStrip;	break;
	case RC_QUADS:			m_pfnDrawPrimitive = &ContextRC1::DrawQuards;			break;
	case RC_TRIANGLE_FAN:	m_pfnDrawPrimitive = &ContextRC1::DrawTriangleFan;		break;
	default:				SetError(RC_INVALID_ENUM);								break;
	}
}

void ContextRC1::End() {
	m_pfnDrawPrimitive = NULL;
}

void ContextRC1::SetSurface(_EGLSurface* pSurface) {	
	if(pSurface) pSurface->AddRef();
	if(m_pSurface) m_pSurface->Release();
	m_pSurface = pSurface;
	if(m_pSurface) Viewport(0,0,m_pSurface->GetCurrentDrawBuffer()->Width() ,m_pSurface->GetCurrentDrawBuffer()->Height());
}

double ContextRC1::MipMapPreProcess(float tex_coord[3][2], RCuint usingMaterial){
	rcMaterial*	pMat		= m_Container.Material.Find(usingMaterial);
	rcTexture*	pTex		= pMat ? pMat->GetTexture() : NULL;
	if(pMat && pTex && pTex->IsValid()){
		double Tn, Pta; 
		Pta = (( tex_coord[0][0] * tex_coord[1][1] + tex_coord[1][0] * tex_coord[2][1] + tex_coord[2][0] * tex_coord[0][1]) -
			( tex_coord[1][0] * tex_coord[0][1] + tex_coord[2][0] * tex_coord[1][1] + tex_coord[0][0] * tex_coord[2][1]) ); //// bound include.
		if(Pta<0)
			Pta = -Pta;

		Tn = Pta * 
			pTex->Width() *
			pTex->Height();

		if(Tn==0)
			return 0;
		else
			return (1 / (m_Screen.info.pixelSizeX * m_Screen.info.pixelSizeY * Tn)); //LodPreProcess = 65536./Tn;
	}
	return 0;
}

void ContextRC1::MakePrimitiveByTriangle(RC_VERTEX *vertex){
	float       tex_coord[3][2];
	double      normal[3], InvNormal;
	double      Vb[3], Vc[3];
	double      n[3];
	int         k, u, v;
	int         axis[5]={0,1,2,0,1};
	int         index;
	RC_PRIMITIVE       *primPt;

	index = m_dwTriangleIndex;
	primPt = m_PrimitiveSource.Get(index);

	VECTOR2_COPY(tex_coord[0], primPt->vertex[0].texcoord.m);
	VECTOR2_COPY(tex_coord[1], primPt->vertex[1].texcoord.m);
	VECTOR2_COPY(tex_coord[2], primPt->vertex[2].texcoord.m);

	primPt->materialIndex = m_Current.pMaterial->Name();
	primPt->baseLOD = (float) MipMapPreProcess(tex_coord, primPt->materialIndex);

	VECTOR3_OPERATION(Vb, vertex[2].position.m, SUB, vertex[0].position.m);
	VECTOR3_OPERATION(Vc, vertex[1].position.m, SUB, vertex[0].position.m);
	VECTOR3_CROSS_PRODUCT(normal, Vb, Vc);

	if(normal[0]<0)     n[0] = -normal[0];
	else                n[0] =  normal[0];

	if(normal[1]<0)     n[1] = -normal[1];
	else                n[1] =  normal[1];

	if(normal[2]<0)     n[2] = -normal[2];
	else                n[2] =  normal[2];

	if( (n[0]>n[1]) && (n[0]>n[2]) ) {  k=0;    u=1;    v=2;   }
	else if( n[1]>n[2] )             {  k=1;    u=2;    v=0;   }
	else                             {  k=2;    u=0;    v=1;   }

	primPt->axis			= k;

	if(normal[k] != 0){
		InvNormal				= (float) (1. / normal[k]);
		primPt->nu				= (float) (normal[u] * InvNormal);
		primPt->nv				= (float) (normal[v] * InvNormal);
		primPt->nd				= (float) (vertex[0].position.m[u] * primPt->nu + vertex[0].position.m[v] * primPt->nv + vertex[0].position.m[k]);

		primPt->triangle.bnu	= (float) (-(Vb[v] * InvNormal));
		primPt->triangle.bnv	= (float) (Vb[u] * InvNormal);
		primPt->triangle.bd		= (float) (0 - vertex[0].position.m[u] * primPt->triangle.bnu - vertex[0].position.m[v] * primPt->triangle.bnv);

		primPt->triangle.cnu	= (float)  (Vc[v] * InvNormal);
		primPt->cnv				= (float) -(Vc[u] * InvNormal);
		primPt->cd				= (float) (0 - vertex[0].position.m[v] * primPt->cnv - vertex[0].position.m[u] * primPt->triangle.cnu); 
	}else{
		primPt->nu				= 0.0f;
		primPt->nv				= 0.0f;
		primPt->nd				= 10000.0f;

		primPt->triangle.bnu	= 0.0f;;
		primPt->triangle.bnv	= 0.0f;
		primPt->triangle.bd		= 10000.0f;

		primPt->triangle.cnu	= 0.0f;
		primPt->cnv				= 0.0f;
		primPt->cd				= 10000.0f;
	}
	
	primPt->type = 0;
	primPt->Flag	= m_CurrentStateFlag.primitive.flags;

	///****///
	rcTexture*	pTexture	= m_Current.pMaterial->GetTexture();
	if(pTexture && pTexture->IsValid()){
		double Pa,Tb;

		Pa = sqrt(normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2]);
		Tb = sqrt(Pa * primPt->baseLOD);
		if(Tb==0)
			primPt->baseLOD = 1.0f;
		else{
			//primPt->baseLOD = (float)(1.8661f/Tb);	// @todo : Emulator!
			primPt->baseLOD = (float)(1.0f/Tb);
		}
	}
	else {
		primPt->baseLOD = 1.0f;
	}

	if(m_Current.pMaterial->IsNormalMapEnable())
	{
		float Uvector[2];
		float Vvector[2];
		float Pvector[2][3];
		float tbn_invdeno;
		float t_normal[3], t_tangent[3], t_binormal[3];
		float test_normal;
//		float normal_temp;
//		float newNormal[3],newTriNormal[3];
		float normalLength;

		Vb[0]= ((primPt->vertex[2].position.x)-(primPt->vertex[0].position.x));
		Vc[0]= ((primPt->vertex[1].position.x)-(primPt->vertex[0].position.x));
		Vb[1]= ((primPt->vertex[2].position.y)-(primPt->vertex[0].position.y));
		Vc[1]= ((primPt->vertex[1].position.y)-(primPt->vertex[0].position.y));
		Vb[2]= ((primPt->vertex[2].position.z)-(primPt->vertex[0].position.z));
		Vc[2]= ((primPt->vertex[1].position.z)-(primPt->vertex[0].position.z));

		Uvector[0] = primPt->vertex[1].texcoord.u -	primPt->vertex[0].texcoord.u;
		Uvector[1] = primPt->vertex[2].texcoord.u -	primPt->vertex[0].texcoord.u;
		Vvector[0] = primPt->vertex[1].texcoord.v -	primPt->vertex[0].texcoord.v;
		Vvector[1] = primPt->vertex[2].texcoord.v -	primPt->vertex[0].texcoord.v;
			
		Pvector[0][0] = primPt->vertex[1].position.x - primPt->vertex[0].position.x;
		Pvector[1][0] = primPt->vertex[2].position.x - primPt->vertex[0].position.x;
		Pvector[0][1] = primPt->vertex[1].position.y - primPt->vertex[0].position.y;
		Pvector[1][1] = primPt->vertex[2].position.y - primPt->vertex[0].position.y;
		Pvector[0][2] = primPt->vertex[1].position.z - primPt->vertex[0].position.z;
		Pvector[1][2] = primPt->vertex[2].position.z - primPt->vertex[0].position.z;

		tbn_invdeno = 1.0f / ((Uvector[0] * Vvector[1]) - (Vvector[0] * Uvector[1]));
		for(int i = 0 ; i <3 ; i++) {
			t_tangent[i]	= (tbn_invdeno * (((Vvector[1] * Pvector[0][i]) - (Vvector[0] * Pvector[1][i]))));
			t_binormal[i]	= (tbn_invdeno * (((Uvector[0] * Pvector[1][i]) - (Uvector[1] * Pvector[0][i]))));
		}

		t_normal[0] = (t_tangent[1] * t_binormal[2]) - (t_tangent[2] * t_binormal[1]);
		t_normal[1] = (t_tangent[2] * t_binormal[0]) - (t_tangent[0] * t_binormal[2]);
		t_normal[2] = (t_tangent[0] * t_binormal[1]) - (t_tangent[1] * t_binormal[0]);

		normalLength = sqrt(((t_normal[0] * t_normal[0]) + (t_normal[1] * t_normal[1]) + (t_normal[2] * t_normal[2])));
		normalLength = sqrt(normalLength);

		t_normal[0] = t_normal[0] / normalLength;
		t_normal[1] = t_normal[1] / normalLength;
		t_normal[2] = t_normal[2] / normalLength;

		test_normal = (float) ((t_normal[0] * normal[0]) + (t_normal[1] * normal[1]) + (t_normal[2] * normal[2]));
		test_normal = -(test_normal);

		
		if(test_normal > 0) {
			t_normal[0]		= -(t_normal[0]);
			t_normal[1]		= -(t_normal[1]);
			t_normal[2]		= -(t_normal[2]);
			t_tangent[0]	= -(t_tangent[0]);
			t_tangent[1]	= -(t_tangent[1]);
			t_tangent[2]	= -(t_tangent[2]);
		}

		primPt->vertex[0].normal.x = t_tangent[0];
		primPt->vertex[0].normal.y = t_tangent[1];
		primPt->vertex[0].normal.z = t_tangent[2];
		primPt->vertex[1].normal.x = t_binormal[0];
		primPt->vertex[1].normal.y = t_binormal[1];
		primPt->vertex[1].normal.z = t_binormal[2];
		primPt->vertex[2].normal.x = t_normal[0];
		primPt->vertex[2].normal.y = t_normal[1];
		primPt->vertex[2].normal.z = t_normal[2];
	}

	{	// 하드웨어 primitive 설정.
		PRIMITIVE24			temp24;
		DDK_PRIMITIVE		*primPos;
		DDK_PRIMITIVE_INFO	*primInfo;
		
		primPos  = m_Memory.pPrimitive->GetPool(index);
		primInfo = m_Memory.pPrimitiveInfo->GetPool(index);

		temp24.axis     = primPt->axis;
		temp24.baseLOD  = f32f24(primPt->baseLOD);
		temp24.type     = 0;

		temp24.materialIndex = primPt->materialIndex;

		VECTOR3_CONVERT_24F(temp24.vertex[0].normal, primPt->vertex[0].normal);
		VECTOR3_CONVERT_24F(temp24.vertex[1].normal, primPt->vertex[1].normal);
		VECTOR3_CONVERT_24F(temp24.vertex[2].normal, primPt->vertex[2].normal);

		VECTOR2_CONVERT_24F(temp24.vertex[0].textureCoordinate, primPt->vertex[0].texcoord);
		VECTOR2_CONVERT_24F(temp24.vertex[1].textureCoordinate, primPt->vertex[1].texcoord);
		VECTOR2_CONVERT_24F(temp24.vertex[2].textureCoordinate, primPt->vertex[2].texcoord);

		temp24.nu = f32f24(primPt->nu);
		temp24.nv = f32f24(primPt->nv);
		temp24.nd = f32f24(primPt->nd);

		temp24.bnu = f32f24(primPt->triangle.bnu);
		temp24.bnv = f32f24(primPt->triangle.bnv);
		temp24.bd  = f32f24(primPt->triangle.bd);

		temp24.cnu = f32f24(primPt->triangle.cnu);
		temp24.cnv = f32f24(primPt->cnv);
		temp24.cd  = f32f24(primPt->cd);

		PACKING_PRIMITIVE_POSITION(temp24, m_CurrentStateFlag.primitive.flags, primPos);
		PACKING_PRIMITIVE_INFORMATION(temp24, m_CurrentStateFlag.primitive.flags, primInfo);
	}
}

void ContextRC1::DrawTriangle(int index) {

	if(!SelectArrayElement(index)) return;

	m_dwNextIndex += 1;
	if (m_dwNextIndex == 3) {
		RenderTriangle(&m_InputVertexElement[0], &m_InputVertexElement[1], &m_InputVertexElement[2]);
		m_dwNextIndex = 0;
	}
}

void ContextRC1::DrawQuards(int index) {

	if(!SelectArrayElement(index)) return;

	if (m_dwPrimitiveState == 0) {
		m_dwPrimitiveState = 1;
		m_dwNextIndex = 1;
	} else if (m_dwPrimitiveState == 1) {
		m_dwPrimitiveState = 2;
		m_dwNextIndex = 2;
	} else if (m_dwPrimitiveState == 2) {
		RenderTriangle(&m_InputVertexElement[0], &m_InputVertexElement[1], &m_InputVertexElement[2]);
		m_dwPrimitiveState = 3;
		m_dwNextIndex = 3;
	} else {
		RenderTriangle(&m_InputVertexElement[0], &m_InputVertexElement[2], &m_InputVertexElement[3]);
		m_dwPrimitiveState = 0;
		m_dwNextIndex = 0;
	}
}

void ContextRC1::DrawTriangleStrip(int index) {

	int prevIndex, prevIndex2;
	
	if(!SelectArrayElement(index)) return;

	if (m_dwPrimitiveState == 3) {
		// even triangle
		prevIndex = (m_dwNextIndex - 1) > m_dwNextIndex ? m_dwNextIndex + 2 : m_dwNextIndex - 1;
		prevIndex2 = (m_dwNextIndex - 2) > m_dwNextIndex ? m_dwNextIndex + 1 : m_dwNextIndex - 2;
		RenderTriangle(&m_InputVertexElement[prevIndex], &m_InputVertexElement[prevIndex2], &m_InputVertexElement[m_dwNextIndex]);
		m_dwPrimitiveState = 2;
	} else if (m_dwPrimitiveState == 2) {
		// odd triangle
		prevIndex = (m_dwNextIndex - 1) > m_dwNextIndex ? m_dwNextIndex + 2 : m_dwNextIndex - 1;
		prevIndex2 = (m_dwNextIndex - 2) > m_dwNextIndex ? m_dwNextIndex + 1 : m_dwNextIndex - 2;
		RenderTriangle(&m_InputVertexElement[prevIndex2], &m_InputVertexElement[prevIndex], &m_InputVertexElement[m_dwNextIndex]);
		m_dwPrimitiveState = 3;
	} else {
		// remember seen a vertex
		m_dwPrimitiveState += 1;
	}

	m_dwNextIndex += 1;
	if (m_dwNextIndex == 3)
		m_dwNextIndex = 0;
}

void ContextRC1::DrawTriangleFan(int index) {

	if(!SelectArrayElement(index)) return;

	if (m_dwPrimitiveState == 3) {
		// even triangle
		RenderTriangle(&m_InputVertexElement[0], &m_InputVertexElement[2], &m_InputVertexElement[1]);
		m_dwPrimitiveState = 2;
		m_dwNextIndex = 2;
	} else if (m_dwPrimitiveState == 2) {
		// odd triangle
		RenderTriangle(&m_InputVertexElement[0], &m_InputVertexElement[1], &m_InputVertexElement[2]);
		m_dwPrimitiveState = 3;
		m_dwNextIndex = 1;
	} else if (m_dwPrimitiveState == 1) {
		// remember seen second vertex
		m_dwPrimitiveState = 2;
		m_dwNextIndex = 2;
	} else if (m_dwPrimitiveState == 0) {
		// remember seen first vertex
		m_dwPrimitiveState = 1;
		m_dwNextIndex = 1;
	}
}

void ContextRC1::RenderTriangle(VERTEX_ELEMENT *a, VERTEX_ELEMENT *b, VERTEX_ELEMENT *c) {
	RC_VERTEX   *vInfopt;
	SetModified();
	vInfopt = m_PrimitiveSource.Get(m_dwTriangleIndex)->vertex;

	if (m_VertexArray.enabled){
		VECTOR3_COPY(vInfopt[0].position.m, a->_v);
		VECTOR3_COPY(vInfopt[1].position.m, b->_v);
		VECTOR3_COPY(vInfopt[2].position.m, c->_v);
	}

	if (m_NormalArray.enabled){
		VECTOR3_COPY(vInfopt[0].normal.m, a->_n);
		VECTOR3_COPY(vInfopt[1].normal.m, b->_n);
		VECTOR3_COPY(vInfopt[2].normal.m, c->_n);
	}else{
		NormalVectorGenerate(a->_v, b->_v, c->_v, vInfopt[0].normal.m);
		VECTOR3_COPY(vInfopt[1].normal.m, vInfopt[0].normal.m);
		VECTOR3_COPY(vInfopt[2].normal.m, vInfopt[0].normal.m);
	}

	if ( m_TexCoordArray.enabled ){
		VECTOR2_COPY(vInfopt[0].texcoord.m, a->_t);
		VECTOR2_COPY(vInfopt[1].texcoord.m, b->_t);
		VECTOR2_COPY(vInfopt[2].texcoord.m, c->_t);
	}else{
		VECTOR2_SUBSTITUTE(vInfopt[0].texcoord.m, 0);
		VECTOR2_SUBSTITUTE(vInfopt[1].texcoord.m, 0);
		VECTOR2_SUBSTITUTE(vInfopt[2].texcoord.m, 0);
	}

	MakePrimitiveByTriangle(vInfopt);
	m_TreeBuilder.AddTriangle(vInfopt, m_dwTriangleIndex, &m_BoundBox);

	m_dwTriangleIndex++;
}

BOOL ContextRC1::SelectArrayElement(int index){
	int i;
	RCfloat     vert[3], vertTmp[3];
	RCfloat     *vpt_f;
	RCubyte     *vpt_ub;
	RCshort     *vpt_s;
	RCfixed     *vpt_fx;
	RCboolean	isMatrixPalette=false;

	if (m_VertexArray.enabled)
		if(!m_VertexArray.pData)	return FALSE;
	if (m_NormalArray.enabled)
		if(!m_NormalArray.pData)	return FALSE;
	if (m_TexCoordArray.enabled)
		if(!m_TexCoordArray.pData)	return FALSE;
	// @todo : Vertex Color를 지원하지 않으므로, 추후 정리가 필요함
	if (m_ColorArray.enabled)
		if(!m_ColorArray.pData)		return FALSE;

	
	if (m_CurrentStateFlag.etc.matrix_palette
			&& m_MatrixIndexArray.size > 0 
			&& m_MatrixIndexArray.pData && m_WeightArray.pData) {
		CalculateMatrixPalette(index);
		isMatrixPalette = true;
	}

	if (m_VertexArray.enabled && m_VertexArray.pData) {
		switch(m_VertexArray.type) {
		case RC_BYTE:
			vpt_ub = (RCbyte *)((unsigned char *)m_VertexArray.pData + (m_VertexArray.stride * index));
			LOOP(m_VertexArray.size) vert[i] = (float) vpt_ub[i];
			break;

		case RC_SHORT:
			vpt_s= (RCshort *)((unsigned char *)m_VertexArray.pData + (m_VertexArray.stride * index));
			LOOP(m_VertexArray.size) vert[i] = (float) vpt_s[i];
			break;

		case RC_FLOAT:
			vpt_f = (float*) ((unsigned char *)m_VertexArray.pData + (m_VertexArray.stride * index));
			//LOOP(m_VertexArray.size) vert[i] = (float) vpt_f[i];
			memcpy(vert, vpt_f, sizeof(RCfloat)*m_VertexArray.size);
			break;

		case RC_FIXED:
			vpt_fx = (RCfixed *) ((unsigned char *)m_VertexArray.pData + (m_VertexArray.stride * index));
			LOOP(m_VertexArray.size) vert[i] = x32f32(vpt_fx[i]);
			break;
		}

		if (m_CurrentStateFlag.etc.matrix_palette && isMatrixPalette) {			
			// @todo : 여기서 Model View Matrix를 적용해 주는 것이 맞는 걸까?			
			if (m_CurrentStateFlag.etc.matrix_modelview_inverse) {
				TransformVertex(vertTmp, vert, m_MatrixModelToWorld.m);
				TransformVertex(m_InputVertexElement[m_dwNextIndex]._v, vertTmp, m_MatrixPalette.matrix.m);
			}
			else
				TransformVertex(m_InputVertexElement[m_dwNextIndex]._v, vert, m_MatrixPalette.matrix.m);
		}
		else if (m_CurrentStateFlag.etc.matrix_modelview_inverse)
			TransformVertex(m_InputVertexElement[m_dwNextIndex]._v, vert, m_MatrixModelToWorld.m);
		else {
			/*for (i = m_VertexArray.size; i;)
			{
				i--;
				m_InputVertexElement[m_dwNextIndex]._v[i] = vert[i];
			}*/
			memcpy(m_InputVertexElement[m_dwNextIndex]._v, vert, sizeof(float)*m_VertexArray.size);
		}
	}

	if (m_NormalArray.enabled && m_NormalArray.pData) {
		float *npt = (float*) m_NormalArray.pData;
		npt = (float *)((unsigned char *)npt + (m_NormalArray.stride * index));

		if (m_CurrentStateFlag.etc.matrix_palette && isMatrixPalette) {
			if (m_CurrentStateFlag.etc.matrix_modelview_inverse) {
				TransformNormal(vertTmp, npt, m_MatrixModelToWorldInverse.m);
				TransformNormal(m_InputVertexElement[m_dwNextIndex]._n, vertTmp, m_MatrixPalette.matrixInv.m);
			}
			else
				TransformNormal(m_InputVertexElement[m_dwNextIndex]._n, npt, m_MatrixPalette.matrixInv.m);
		}
		else if (m_CurrentStateFlag.etc.matrix_modelview_inverse)
			TransformNormal(m_InputVertexElement[m_dwNextIndex]._n, npt, m_MatrixModelToWorldInverse.m);
		else {
			/*for(i = m_NormalArray.size; i;){
				i--;
				m_InputVertexElement[m_dwNextIndex]._n[i] = npt[i];
			}*/
			memcpy(m_InputVertexElement[m_dwNextIndex]._n, npt, sizeof(float)*m_NormalArray.size);
		}
	}

	if (m_TexCoordArray.enabled && m_TexCoordArray.pData) {
		switch(m_TexCoordArray.type) {
		case RC_FLOAT:
			{
				float *tpt = (float*) ((unsigned char *)m_TexCoordArray.pData  + (m_TexCoordArray.stride * index));

				/*for(i = m_TexCoordArray.size; i;)
				{
					i--;
					m_InputVertexElement[m_dwNextIndex]._t[i] = tpt[i];
				}*/
				memcpy(m_InputVertexElement[m_dwNextIndex]._t, tpt, sizeof(float)*m_TexCoordArray.size);
			}
			break;
		case RC_FIXED:
			{
				int *tpt = (int *) ((unsigned char *)m_TexCoordArray.pData  + (m_TexCoordArray.stride * index));

				for(i = m_TexCoordArray.size; i;)
				{
					i--;
					m_InputVertexElement[m_dwNextIndex]._t[i] = (float) tpt[i];
				}
			}
			break;
		case RC_SHORT:
			{
				short *tpt = (short *) ((unsigned char *)m_TexCoordArray.pData  + (m_TexCoordArray.stride * index));

				for(i = m_TexCoordArray.size; i;)
				{
					i--;
					m_InputVertexElement[m_dwNextIndex]._t[i] = (float) tpt[i];
				}
			}
			break;
		case RC_BYTE:
			{
				RCbyte *tpt = (RCbyte *) ((unsigned char *)m_TexCoordArray.pData  + (m_TexCoordArray.stride * index));

				for(i = m_TexCoordArray.size; i;)
				{
					i--;
					m_InputVertexElement[m_dwNextIndex]._t[i] = (float) tpt[i];
				}
			}
			break;
		}

		m_InputVertexElement[m_dwNextIndex]._t[0] = m_InputVertexElement[m_dwNextIndex]._t[0]*m_MatrixTexture.m[0] + m_InputVertexElement[m_dwNextIndex]._t[1]*m_MatrixTexture.m[4];
		m_InputVertexElement[m_dwNextIndex]._t[1] = m_InputVertexElement[m_dwNextIndex]._t[0]*m_MatrixTexture.m[1] + m_InputVertexElement[m_dwNextIndex]._t[1]*m_MatrixTexture.m[5];
	}

	// @todo : Vertex Color를 지원하지 않으므로, 추후 정리가 필요함
	if (m_ColorArray.enabled && m_ColorArray.pData){
		float cpt[4];
		RCfloat     *cpt_f;
		RCubyte     *cpt_ub;
		RCshort     *cpt_s;
		RCushort    *cpt_us;
		RCfixed     *cpt_fx;

		memset(cpt, 0, sizeof(float)*4);
		switch(m_ColorArray.type) {
		case RC_BYTE:
		case RC_UNSIGNED_BYTE:
			cpt_ub = (RCbyte *)((unsigned char *)m_ColorArray.pData + (m_ColorArray.stride * index));
			LOOP(4) cpt[i] = (float) cpt_ub[i] / 256.f;
			break;
		case RC_SHORT:
			cpt_s= (RCshort *)((unsigned char *)m_ColorArray.pData + (m_ColorArray.stride * index));
			LOOP(4) cpt[i] = (float) cpt_s[i] / 65536.f;
			break;
		case RC_UNSIGNED_SHORT:
			cpt_us= (RCushort *)((unsigned char *)m_ColorArray.pData + (m_ColorArray.stride * index));
			LOOP(4) cpt[i] = (float) cpt_us[i] / 65536.f;
			break;
		case RC_FLOAT:
			cpt_f = (float*) ((unsigned char *)m_ColorArray.pData + (m_ColorArray.stride * index));
			memcpy(cpt, cpt_f, sizeof(RCfloat)*m_ColorArray.size);
			break;
		case RC_FIXED:
			cpt_fx = (RCfixed *) ((unsigned char *)m_ColorArray.pData + (m_ColorArray.stride * index));
			LOOP(4) cpt[i] = x32f32(cpt_fx[i]);
			break;
		}

		memcpy(m_InputVertexElement[m_dwNextIndex]._c, &cpt[0], sizeof(RCfloat)*3);
	}

	return TRUE;
}

void ContextRC1::Clear (RCbitfield mask){
	//do something...
}

void ContextRC1::Hint (RCenum target, RCenum mode){
	switch(target){
	case RC_FOG_HINT:
	case RC_GENERATE_MIPMAP_HINT:
	case RC_LINE_SMOOTH_HINT:
	case RC_PERSPECTIVE_CORRECTION_HINT:
	case RC_POINT_SMOOTH_HINT:
		break;
	case RC_MIPMAP_HINT:
		{
			switch(mode){
			case RC_DONT_CARE:
			case RC_FASTEST:
				m_CurrentStateFlag.option.lod = 1;
				m_pDDK->Enable(RCDDK_ENABLE_LOD, TRUE);
				break;
			case RC_NICEST:
				m_CurrentStateFlag.option.lod = 0;
				m_pDDK->Enable(RCDDK_ENABLE_LOD, FALSE);
				break;
			default:
				SetError(RC_INVALID_ENUM);
				break;
			}
		}
		break;
	case RC_RAYBOUNCE_THRESHOLD_HINT:
		{
			switch(mode){
			case RC_DONT_CARE:
				m_fRayThresholdWeight = 0.0f;
				break;
			case RC_THRESHOLD_LEVEL0:
			case RC_THRESHOLD_LEVEL1:
			case RC_THRESHOLD_LEVEL2:
			case RC_THRESHOLD_LEVEL3:
			case RC_THRESHOLD_LEVEL4:
			case RC_THRESHOLD_LEVEL5:
			case RC_THRESHOLD_LEVEL6:
			case RC_THRESHOLD_LEVEL7:
			case RC_THRESHOLD_LEVEL8:
			case RC_THRESHOLD_LEVEL9:
				{
					DWORD index = (mode - RC_THRESHOLD_LEVEL0);
					m_fRayThresholdWeight = (index + 1)*0.01f;
				}
				break;
			default:
				SetError(RC_INVALID_ENUM);
				break;
			}
			m_pDDK->SetRayThresholdWeight(m_fRayThresholdWeight);
		}
		break;
	case RC_RENDERING_HINT:
		{
			switch(mode){
			case RC_DONT_CARE:
				m_CurrentStateFlag.option.super_sampling = 0;
				m_CurrentStateFlag.option.adaptive_render = 0;
				m_iAdaptiveSize = 0;
				break;
			case RC_FASTEST:
				m_CurrentStateFlag.option.super_sampling = 0;
				m_CurrentStateFlag.option.adaptive_render = 1;
				m_iAdaptiveSize = 16;
				break;
			case RC_NICEST:
				m_CurrentStateFlag.option.super_sampling = 1;
				m_CurrentStateFlag.option.adaptive_render = 0;
				m_iAdaptiveSize = 0;
				
				break;
			case RC_FASTEST_AND_NICEST:
				m_CurrentStateFlag.option.super_sampling = 1;
				m_CurrentStateFlag.option.adaptive_render = 1;
				m_iAdaptiveSize = 16;
				break;
			default:
				SetError(RC_INVALID_ENUM);
				return;
			}
			m_pDDK->Enable(RCDDK_ENABLE_SUPERSAMPLING,	m_CurrentStateFlag.option.super_sampling);
			m_pDDK->SetAdaptiveRender(m_CurrentStateFlag.option.adaptive_render, m_iAdaptiveSize);
		}
		break;
	default:
		SetError(RC_INVALID_ENUM);
		break;
	}
}

void ContextRC1::SetModified(void){
	m_bMustDoFinish	= TRUE;
}

void ContextRC1::ReadPixels (RCint x, RCint y, RCsizei width, RCsizei height, RCenum format, RCenum type, RCvoid *pixels){
	//do something...
	SetError(RC_INVALID_OPERATION);
}

void ContextRC1::TexSubImage2D (RCenum target, RCint level, RCint xoffset, RCint yoffset, RCsizei width, RCsizei height, RCenum format, RCenum type, const RCvoid *pixels){
	//do something...
	SetError(RC_INVALID_OPERATION);
}
