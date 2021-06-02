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
// DATE    : 7/16/2013 Tue
// Author  : Sang Gon, Kim (sgkim@siliconarts.co.kr)
//
//=======================================================================

#include "rcContext1.h"

RCint ContextRC1::GetRoundInt(RCfloat f){
	RCint ret=0;
	RCfloat dist;

	dist = f - (RCfloat)(RCint)f;

	if(f < 0){
		if(dist < -0.5)	ret = (RCint)(f-0.5);
		else			ret = (RCint)f;
	}else{
		if(dist < 0.5)	ret = (RCint)f;
		else			ret = (RCint)(f+0.5);
	}

	return ret;
}

void ContextRC1::GetFloatv (RCenum pname, RCfloat *params){
	int i=0;

	switch(pname){
	case RC_ACTIVE_TEXTURE:				//rcActiveTexture
		params[0] = RC_TEXTURE0;
		break;
	case RC_ALIASED_POINT_SIZE_RANGE:	//rcPointSIze
	case RC_ALIASED_LINE_WIDTH_RANGE:	//rcLineWidth
	case RC_ALPHA_BITS:					//the number of alpha bitplanes in the color buffer
	case RC_ALPHA_TEST:					//rcAlphaFunc
	case RC_ALPHA_TEST_FUNC:			//rcAlphaFunc
	case RC_ALPHA_TEST_REF:				//rcAlphaFunc
		params[0] = 0.0;
		break;
	case RC_ARRAY_BUFFER_BINDING:
		params[0] = 0.0;
		if(m_Current.pBuffer) params[0] = (RCfloat)m_Current.pBuffer->Name();
		break;
	case RC_BLEND:						//rcBlendFunc, rcLogiOp
	case RC_BLEND_DST:					//rcBlendFunc
	case RC_BLEND_SRC:					//rcBlendFunc
	case RC_BLUE_BITS:					//the number of blue bitplanes in the color buffer
		params[0] = 0.0;
		break;
	case RC_CLIENT_ACTIVE_TEXTURE:		//rcClientActiveTexture
		params[0] = RC_TEXTURE0;
		break;
	case RC_CLIP_PLANE0:				//rcClipPlane
	case RC_CLIP_PLANE1:				//rcClipPlane
	case RC_CLIP_PLANE2:				//rcClipPlane
	case RC_CLIP_PLANE3:				//rcClipPlane
	case RC_CLIP_PLANE4:				//rcClipPlane
	case RC_CLIP_PLANE5:				//rcClipPlane
		params[0] = 0.0;
		break;
	case RC_COLOR_ARRAY:
		params[0] = (m_ColorArray.enabled)? 1.f:0.f;
		break;
	case RC_COLOR_ARRAY_BUFFER_BINDING:
		params[0] = (RCfloat)m_ColorArray.boundBuffer;
		break;
	case RC_COLOR_ARRAY_SIZE:
		params[0] = (RCfloat)m_ColorArray.size;
		break;
	case RC_COLOR_ARRAY_STRIDE:
		params[0] = (RCfloat)m_ColorArray.stride;
		break;
	case RC_COLOR_ARRAY_TYPE:
		params[0] = (RCfloat)m_ColorArray.type;
		break;
	case RC_COLOR_CLEAR_VALUE:
		params[0] = m_Screen.clearRed;
		params[1] = m_Screen.clearGreen;
		params[2] = m_Screen.clearBlue;
		params[4] = m_Screen.clearAlpha;
		break;
	case RC_COLOR_LOGIC_OP:				//rcLogicOp
	case RC_COLOR_MATERIAL:				//rcMaterial, rcEnable(RC_COLOR_MATERIAL)
	case RC_COLOR_WRITEMASK:			//rcColorMask
	case RC_COMPRESSED_TEXTURE_FORMATS:	//rcCompressedTexImage2D, rcCompressedTexSubImage2D
	case RC_CULL_FACE:					//rcCullFace
		params[0] = 0.0;
		break;
	case RC_CULL_FACE_MODE:				//rcCullFace
		params[0] = RC_BACK;
		break;
	case RC_CURRENT_COLOR:
		LOOP(4) params[i] = (m_fDefaultColor[i] < 0)? -1:m_fDefaultColor[i];
		break;
	case RC_CURRENT_AMBIENT:
		{
			RCVECTOR3f* pColor		= m_Current.pMaterial->GetAmbient();
			params[0] = pColor->r;
			params[1] = pColor->g;
			params[2] = pColor->b;
			params[3] = 1.0;
		}
		break;
	case RC_CURRENT_DIFFUSE:
		{
			RCVECTOR3f* pColor		= m_Current.pMaterial->GetDiffuse();
			params[0] = pColor->r;
			params[1] = pColor->g;
			params[2] = pColor->b;
			params[3] = 1.0;
		}
		break;
	case RC_CURRENT_SPECULAR:
		{
			RCVECTOR3f* pColor		= m_Current.pMaterial->GetSpecular();
			params[0] = pColor->r;
			params[1] = pColor->g;
			params[2] = pColor->b;
			params[3] = 1.0;
		}
		break;
	case RC_CURRENT_NORMAL:				//rcNormal
		params[0] = 0.0f;
		params[1] = 0.0f;
		params[2] = 1.0f;
		break;
	case RC_CURRENT_TEXTURE_COORDS:		//rcMultiTexCoord
	case RC_DEPTH_CLEAR_VALUE:			//rcClearDepth
	case RC_DEPTH_BITS:					//the number of bitplanes in the depth buffer
	case RC_DEPTH_FUNC:					//rcDepthFunc
	case RC_DEPTH_TEST:					//rcDepthFunc, rcDepthRange
	case RC_DEPTH_RANGE:				//rcDepthRange
	case RC_DEPTH_WRITEMASK:			//rcDepthMask
	case RC_DITHER:
		params[0] = 0.0;
		break;
	case RC_ELEMENT_ARRAY_BUFFER_BINDING:
		params[0] = 0;
		if(m_Current.pElementBuffer) params[0] = (RCfloat)m_Current.pElementBuffer->Name();
		break;
	case RC_FOG:						//rcFog
	case RC_FOG_COLOR:					//rcFog
	case RC_FOG_DENSITY:				//rcFog
	case RC_FOG_END:					//rcFog
	case RC_FOG_HINT:					//rcHint
	case RC_FOG_MODE:					//rcFog
	case RC_FOG_START:					//rcFog
	case RC_FRONT_FACE:					//rcFrontFace
	case RC_GREEN_BITS:					//the number of green bitplanes in the color buffer
	case RC_IMPLEMENTATION_COLOR_READ_FORMAT_OES:	//rcReadPixels
	case RC_IMPLEMENTATION_COLOR_READ_TYPE_OES:		//rcReadPixels
	case RC_LIGHT_MODEL_AMBIENT:		//rcLightModel
	case RC_LIGHT_MODEL_TWO_SIDE:		//rcLightModel
		params[0] = 0.0;
		break;
	case RC_LIGHT0:
	case RC_LIGHT1:
	case RC_LIGHT2:
	case RC_LIGHT3:
	case RC_LIGHT4:
	case RC_LIGHT5:
	case RC_LIGHT6:
	case RC_LIGHT7:
		{
			RCboolean isEnable;
			if(!m_Light.IsEnable(pname, &isEnable))
				SetError(RC_INVALID_ENUM);
			params[0] = (isEnable)? 1.f:0.f;
		}
		break;
	case RC_LIGHTING:
		params[0] = (m_Light.IsLighting())? 1.f:0.f;
		break;
	case RC_LINE_SMOOTH:				//rcLineWidth
	case RC_LINE_SMOOTH_HINT:			//rcHint
	case RC_LINE_WIDTH:					//rcLineWidth
	case RC_LOGIC_OP_MODE:				//rcLogicOp
		params[0] = 0.f;
		break;
	case RC_MATERIAL_BINDING:
		params[0] = (RCfloat)m_Current.pMaterial->Name();
		break;
	case RC_MAX_CLIP_PLANES:			//rcClipPlane
		params[0] = 0.f;
		break;
	case RC_MAX_LIGHTS:
		params[0] = RCDDK_MAX_LIGHT_COUNT;
		break;
	case RC_MAX_MODELVIEW_STACK_DEPTH:
		params[0] = RCDDK_MAX_MATRIX_STACK_COUNT;
		break;
	case RC_MAX_PALETTE_MATRICES_OES:
		params[0] = RCDDK_MAX_MATRIX_PALETTE_COUNT;
		break;
	case RC_MAX_PROJECTION_STACK_DEPTH:
		params[0] = RCDDK_MAX_MATRIX_STACK_COUNT;
		break;
	case RC_MAX_TEXTURE_SIZE:
		params[0] = MAX_TEXTURE_SIZE;
		break;
	case RC_MAX_TEXTURE_STACK_DEPTH:
		params[0] = RCDDK_MAX_MATRIX_STACK_COUNT;
		break;
	case RC_MAX_TEXTURE_UNITS:			//rcActiveTexture
		params[0] = 1.f;
		// @todo : multi texture 지원시 5(RC_TEXTURE0 ~ RC_TEXTURE4)?
		break;
	case RC_MAX_THRESHOLD_LEVELS:
		params[0] = RCDDK_MAX_THRESHOLD_LEVEL_COUNT;
		break;
	case RC_MAX_VERTEX_UNITS_OES:
		params[0] = RCDDK_MAX_VERTEX_UNITS_COUNT;
		break;
	case RC_MAX_VIEWPORT_DIMS:
		params[0] = RCDDK_MAX_VIEWPORT_DIMS;
		params[1] = RCDDK_MAX_VIEWPORT_DIMS;
		break;
	case RC_MATRIX_INDEX_ARRAY_OES:
		params[0] = m_MatrixIndexArray.enabled ? 1.f : 0.f;
		break;
	case RC_MATRIX_INDEX_ARRAY_BUFFER_BINDING_OES:
		params[0] = (RCfloat)m_MatrixIndexArray.boundBuffer;
		break;
	case RC_MATRIX_INDEX_ARRAY_SIZE_OES:
		params[0] = (RCfloat)m_MatrixIndexArray.size;
		break;
	case RC_MATRIX_INDEX_ARRAY_STRIDE_OES:
		params[0] = (RCfloat)m_MatrixIndexArray.stride;
		break;
	case RC_MATRIX_INDEX_ARRAY_TYPE_OES:
		params[0] = (RCfloat)m_MatrixIndexArray.type;
		break;
	case RC_MATRIX_MODE:
		params[0] = (RCfloat)m_dwCurrentMatrixMode;
		break;
	case RC_MATRIX_PALETTE_OES:
		params[0] = (m_CurrentStateFlag.etc.matrix_palette)? 1.f:0.f;
		break;
	case RC_MODELVIEW_MATRIX:
		memset(params, 0, SIZEOFIDENTITYMATRIX);
		if(m_iMatrixStackIndexModelView >= 0)
			memcpy(params, m_MatrixStackModelView[m_iMatrixStackIndexModelView].V_matrix.m, SIZEOFIDENTITYMATRIX);
		break;
	case RC_MODELVIEW_STACK_DEPTH:
		params[0] = (RCfloat)(m_iMatrixStackIndexModelView + 1);
		break;
	case RC_MULTISAMPLE:				//whether multisampling is enabled
		params[0] = 1.f;
		break;
	case RC_NORMAL_ARRAY:
		params[0] = (m_NormalArray.enabled)? 1.f:0.f;
		break;
	case RC_NORMAL_ARRAY_BUFFER_BINDING:
		params[0] = (RCfloat)m_NormalArray.boundBuffer;
		break;
	case RC_NORMAL_ARRAY_STRIDE:
		params[0] = (RCfloat)m_NormalArray.stride;
		break;
	case RC_NORMAL_ARRAY_TYPE:
		params[0] = (RCfloat)m_NormalArray.type;
		break;
	case RC_NORMALIZE:						//rcNormal
	case RC_NUM_COMPRESSED_TEXTURE_FORMATS:	//rcCompressedTexImage2D
	case RC_PACK_ALIGNMENT:					//rcPixelStorei
	case RC_PERSPECTIVE_CORRECTION_HINT:	//rcHint
	case RC_POINT_DISTANCE_ATTENUATION:		//rcPointParameter
	case RC_POINT_FADE_THRESHOLD_SIZE:		//rcPointParameter
	case RC_POINT_SIZE:						//rcPointSize
	case RC_POINT_SIZE_ARRAY_BUFFER_BINDING_OES:	//rcPointSizePointerOES
	case RC_POINT_SIZE_ARRAY_OES:					//rcPointSizePointerOES
	case RC_POINT_SIZE_ARRAY_STRIDE_OES:			//rcPointSizePointerOES
	case RC_POINT_SIZE_ARRAY_TYPE_OES:				//rcPointSizePointerOES
	case RC_POINT_SIZE_MAX:				//rcPointParameter
	case RC_POINT_SIZE_MIN:				//rcPointParameter
	case RC_POINT_SMOOTH:				//rcPointSize
	case RC_POINT_SMOOTH_HINT:			//rcHint
	case RC_POINT_SPRITE_OES:			//rcTGexEnv
	case RC_POLYGON_OFFSET_FACTOR:		//rcPolygonOffset
	case RC_POLYGON_OFFSET_FILL:		//rcPolygonOffset
	case RC_POLYGON_OFFSET_UNITS:		//rcPolygonOffset
		params[0] = 0.f;
		break;
	case RC_PROJECTION_MATRIX:
		memset(params, 0, SIZEOFIDENTITYMATRIX);
		if(m_iMatrixStackIndexProjection >= 0)
			memcpy(params, m_MatrixStackProjection[m_iMatrixStackIndexProjection].V_matrix.m, SIZEOFIDENTITYMATRIX);
		break;
	case RC_PROJECTION_STACK_DEPTH:
		params[0] = (RCfloat)(m_iMatrixStackIndexProjection + 1);
		break;
	case RC_RED_BITS:					//the number of red bitplanes in each color buffer
	case RC_RESCALE_NORMAL:				//rcNormal
	case RC_SAMPLE_ALPHA_TO_COVERAGE:	//rcSampleCoverage
	case RC_SAMPLE_ALPHA_TO_ONE:		//rcSampleCoverage
	case RC_SAMPLE_BUFFERS:				//rcSampleCoverage
	case RC_SAMPLE_COVERAGE:			//rcSampleCoverage
	case RC_SAMPLE_COVERAGE_INVERT:		//rcSampleCoverage
	case RC_SAMPLE_COVERAGE_VALUE:		//rcSampleCoverage
	case RC_SAMPLES:					//rcSampleCoverage
	case RC_SHADE_MODEL:				//rcShadeModel
	case RC_SCISSOR_BOX:				//rcScissor
	case RC_SCISSOR_TEST:				//rcScissor
	case RC_SMOOTH_LINE_WIDTH_RANGE:	//rcLineWidth
	case RC_SMOOTH_POINT_SIZE_RANGE:	//rcPointSize
	case RC_STENCIL_BITS:				//the number of bitplanes in the stencil buffer
	case RC_STENCIL_CLEAR_VALUE:		//rcClearStencil
	case RC_STENCIL_FAIL:				//rcStencilOp
	case RC_STENCIL_FUNC:				//rcStencilFunc
	case RC_STENCIL_PASS_DEPTH_FAIL:	//rcStencilOp
	case RC_STENCIL_PASS_DEPTH_PASS:	//rcStencilOp
	case RC_STENCIL_REF:				//rcStencilFunc
	case RC_STENCIL_TEST:				//rcStencilFunc, rcStencilOp
	case RC_STENCIL_VALUE_MASK:			//rcStencilFunc
	case RC_STENCIL_WRITEMASK:			//rcStencilMask
	case RC_SUBPIXEL_BITS:				//an estimate of the number of bits of subpixel resolution
		params[0] = 0.f;
		break;
	case RC_TEXTURE_2D:
		//params[0] = (m_Current.pMaterial->IsTextureEnable())? 1.f:0.f;
		params[0] = (m_Current.pMaterial->IsTextureMapEnable())? 1.f:0.f;
		break;
	case RC_TEXTURE_2D_NORMAL:
		params[0] = (m_Current.pMaterial->IsNormalMapEnable())? 1.f:0.f;
		break;
	case RC_TEXTURE_BINDING_2D:
		params[0] = (RCfloat)m_Current.pTexture->Name();
		break;
	case RC_TEXTURE_COORD_ARRAY:
		params[0] = (m_TexCoordArray.enabled)? 1.f:0.f;
		break;		
	case RC_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
		params[0] = (RCfloat)m_TexCoordArray.boundBuffer;
		break;
	case RC_TEXTURE_COORD_ARRAY_SIZE:
		params[0] = (RCfloat)m_TexCoordArray.size;
		break;
	case RC_TEXTURE_COORD_ARRAY_STRIDE:
		params[0] = (RCfloat)m_TexCoordArray.stride;
		break;
	case RC_TEXTURE_COORD_ARRAY_TYPE:
		params[0] = (RCfloat)m_TexCoordArray.type;
		break;
	case RC_TEXTURE_MATRIX:
		memset(params, 0, SIZEOFIDENTITYMATRIX);
		if(m_iMatrixStackIndexTexture >= 0)
			memcpy(params, m_MatrixStackTexture[m_iMatrixStackIndexTexture].V_matrix.m, SIZEOFIDENTITYMATRIX);
		break;
	case RC_TEXTURE_STACK_DEPTH:
		params[0] = (RCfloat)(m_iMatrixStackIndexTexture + 1);
		break;
	case RC_UNPACK_ALIGNMENT:			//rcPixelStorei
		params[0] = 0.f;
		break;
	case RC_USE_COLOR_SHADOW:
		params[0] = (m_CurrentStateFlag.option.color_shadow)? 1.f:0.f;
		break;
	case RC_USE_SHADOW:
		params[0] = (m_CurrentStateFlag.option.shadow)? 1.f:0.f;
		break;
	case RC_USE_TEXTURE_ALPHA_SHADOW:
		params[0] = (m_CurrentStateFlag.primitive.texture_alpha_shadow)? 1.f:0.f;
		break;
	case RC_USE_TEXTURE_ONLY:
		params[0] = (m_CurrentStateFlag.primitive.texture_only)? 1.f:0.f;
		break;
	case RC_USE_TRANSMITTANCE_SHADOW:
		params[0] = (m_CurrentStateFlag.primitive.transmittance_shadow)? 1.f:0.f;
		break;		
	case RC_VERTEX_ARRAY:
		params[0] = (m_VertexArray.enabled)? 1.f:0.f;
		break;
	case RC_VERTEX_ARRAY_BUFFER_BINDING:
		params[0] = (RCfloat)m_VertexArray.boundBuffer;
		break;
	case RC_VERTEX_ARRAY_SIZE:
		params[0] = (RCfloat)m_VertexArray.size;
		break;
	case RC_VERTEX_ARRAY_STRIDE:
		params[0] = (RCfloat)m_VertexArray.stride;
		break;
	case RC_VERTEX_ARRAY_TYPE:
		params[0] = (RCfloat)m_VertexArray.type;
		break;
	case RC_VIEWPORT:
		params[0] = (RCfloat)m_Screen.X;
		params[1] = (RCfloat)m_Screen.Y;
		params[2] = (RCfloat)m_Screen.Width;
		params[3] = (RCfloat)m_Screen.Height;
		break;
	case RC_WEIGHT_ARRAY_OES:
		params[0] = m_WeightArray.enabled ? 1.f : 0.f;
		break;
	case RC_WEIGHT_ARRAY_BUFFER_BINDING_OES:
		params[0] = (RCfloat)m_WeightArray.boundBuffer;
		break;
	case RC_WEIGHT_ARRAY_SIZE_OES:
		params[0] = (RCfloat)m_WeightArray.size;
		break;
	case RC_WEIGHT_ARRAY_STRIDE_OES:
		params[0] = (RCfloat)m_WeightArray.stride;
		break;
	case RC_WEIGHT_ARRAY_TYPE_OES:
		params[0] = (RCfloat)m_WeightArray.type;
		break;
	default:
		SetError(RC_INVALID_ENUM);
		break;
	}
}

void ContextRC1::GetLightv (RCenum light, RCenum pname, RCfloat *params){
	if(!m_Light.Get(light, pname, params))
		SetError(RC_INVALID_ENUM);
}

void ContextRC1::GetMaterialv (RCenum face, RCenum pname, RCfloat *params){

	if (face != RC_FRONT_AND_BACK && face != RC_FRONT && face != RC_BACK) {
		SetError(RC_INVALID_ENUM);
		return;
	}

	switch(pname){
	case RC_AMBIENT:
		VECTOR3_COPY(params, ((float*)m_Current.pMaterial->GetAmbient()));
		params[3] = 1.f;
		break;
	case RC_DIFFUSE:
		VECTOR3_COPY(params, ((float*)m_Current.pMaterial->GetDiffuse()));
		params[3] = 1.f;
		break;
	case RC_SPECULAR:
		VECTOR3_COPY(params, ((float*)m_Current.pMaterial->GetSpecular()));
		params[3] = 1.f;
		break;
	case RC_EMISSION:	// @todo : 추후 지원시 구현
		break;
	case RC_TRANSMITTANCE:
		params[0]	= m_Current.pMaterial->GetTransmittance();
		break;
	case RC_REFLECTION:
		params[0]	= m_Current.pMaterial->GetReflection();
		break;
	case RC_SHININESS:
		params[0] = m_Current.pMaterial->GetExponent();
		break;
	case RC_SPECULAR_LEVEL:
		params[0] = m_Current.pMaterial->GetSpecularLevel();
		break;
	case RC_REFRACTION_INDEX:
		params[0] = m_Current.pMaterial->GetRefractionIndex();
		break;
	default:
		SetError(RC_INVALID_ENUM);
		break;
	}
}

void ContextRC1::GetBooleanv (RCenum pname, RCboolean *params){
	int i = 0;

	if (pname >= RC_LIGHT0 && pname <= RC_LIGHT1023)
	{
		if (!m_Light.IsEnable(pname, params)) {
			SetError(RC_INVALID_ENUM);
		}
		return;
	}


	switch(pname){
	case RC_ACTIVE_TEXTURE:				//rcActiveTexture
		params[0] = (RC_TEXTURE0 == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_ALIASED_POINT_SIZE_RANGE:	//rcPointSIze
	case RC_ALIASED_LINE_WIDTH_RANGE:	//rcLineWidth
	case RC_ALPHA_BITS:					//the number of alpha bitplanes in the color buffer
	case RC_ALPHA_TEST:					//rcAlphaFunc
	case RC_ALPHA_TEST_FUNC:			//rcAlphaFunc
	case RC_ALPHA_TEST_REF:				//rcAlphaFunc
		params[0] = RC_FALSE;
		break;
	case RC_ARRAY_BUFFER_BINDING:
		params[0] = RC_FALSE;
		if(m_Current.pBuffer) params[0] = (m_Current.pBuffer->Name() == 0)? FALSE:RC_TRUE;
		break;
	case RC_BLEND:						//rcBlendFunc, rcLogiOp
		params[0] = RC_FALSE;
		break;
	case RC_BLEND_DST:					//rcBlendFunc
	case RC_BLEND_SRC:					//rcBlendFunc
	case RC_BLUE_BITS:					//the number of blue bitplanes in the color buffer
		params[0] = RC_FALSE;
		break;
	case RC_CLIENT_ACTIVE_TEXTURE:			//rcClientActiveTexture
		params[0] = (RC_TEXTURE0 == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_CLIP_PLANE0:				//rcClipPlane
	case RC_CLIP_PLANE1:				//rcClipPlane
	case RC_CLIP_PLANE2:				//rcClipPlane
	case RC_CLIP_PLANE3:				//rcClipPlane
	case RC_CLIP_PLANE4:				//rcClipPlane
	case RC_CLIP_PLANE5:				//rcClipPlane
		params[0] = RC_FALSE;
		break;
	case RC_COLOR_ARRAY:
		params[0] = m_ColorArray.enabled;
		break;
	case RC_COLOR_ARRAY_BUFFER_BINDING:
		params[0] = (m_ColorArray.boundBuffer == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_COLOR_ARRAY_SIZE:
		params[0] = (m_ColorArray.size == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_COLOR_ARRAY_STRIDE:
		params[0] = (m_ColorArray.stride == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_COLOR_ARRAY_TYPE:
		params[0] = (m_ColorArray.type == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_COLOR_CLEAR_VALUE:
		params[0] = (m_Screen.clearRed == 0)? RC_FALSE:RC_TRUE;
		params[1] = (m_Screen.clearGreen == 0)? RC_FALSE:RC_TRUE;
		params[2] = (m_Screen.clearBlue == 0)? RC_FALSE:RC_TRUE;
		params[4] = (m_Screen.clearAlpha == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_COLOR_LOGIC_OP:				//rcLogicOp
		params[0] = RC_FALSE;
		break;
	case RC_COLOR_MATERIAL:				//rcMaterial, rcEnable(RC_COLOR_MATERIAL)
		params[0] = RC_FALSE;
		break;
	case RC_COLOR_WRITEMASK:			//rcColorMask
		params[0] = RC_FALSE;
		break;
	case RC_COMPRESSED_TEXTURE_FORMATS:	//rcCompressedTexImage2D, rcCompressedTexSubImage2D
		params[0] = RC_FALSE;
		break;
	case RC_CULL_FACE:					//rcCullFace
		params[0] = RC_FALSE;
		break;
	case RC_CULL_FACE_MODE:				//rcCullFace
		params[0] = (RC_BACK == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_CURRENT_COLOR:
		LOOP(4) params[i] = (m_fDefaultColor[i] == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_CURRENT_AMBIENT:
		{
			RCVECTOR3f* pColor		= m_Current.pMaterial->GetAmbient();
			params[0] = (pColor->r == 0)? RC_FALSE:RC_TRUE;
			params[1] = (pColor->g == 0)? RC_FALSE:RC_TRUE;
			params[2] = (pColor->b == 0)? RC_FALSE:RC_TRUE;
			params[3] = RC_TRUE;
		}
		break;
	case RC_CURRENT_DIFFUSE:
		{
			RCVECTOR3f* pColor		= m_Current.pMaterial->GetDiffuse();
			params[0] = (pColor->r == 0)? RC_FALSE:RC_TRUE;
			params[1] = (pColor->g == 0)? RC_FALSE:RC_TRUE;
			params[2] = (pColor->b == 0)? RC_FALSE:RC_TRUE;
			params[3] = RC_TRUE;
		}
		break;
	case RC_CURRENT_SPECULAR:
		{
			RCVECTOR3f* pColor		= m_Current.pMaterial->GetSpecular();
			params[0] = (pColor->r == 0)? RC_FALSE:RC_TRUE;
			params[1] = (pColor->g == 0)? RC_FALSE:RC_TRUE;
			params[2] = (pColor->b == 0)? RC_FALSE:RC_TRUE;
			params[3] = RC_TRUE;
		}
		break;
	case RC_CURRENT_NORMAL:				//rcNormal
		params[0] = RC_FALSE;
		params[1] = RC_FALSE;
		params[2] = RC_TRUE;
		break;
	case RC_CURRENT_TEXTURE_COORDS:		//rcMultiTexCoord
		break;
	case RC_DEPTH_CLEAR_VALUE:			//rcClearDepth
	case RC_DEPTH_FUNC:					//rcDepthFunc
	case RC_DEPTH_TEST:					//rcDepthFunc, rcDepthRange
	case RC_DEPTH_WRITEMASK:			//rcDepthMask
	case RC_DEPTH_RANGE:				//rcDepthRange
	case RC_DITHER:
		params[0] = RC_FALSE;
		break;
	case RC_ELEMENT_ARRAY_BUFFER_BINDING:
		params[0] = RC_FALSE;
		if(m_Current.pElementBuffer) params[0] = (m_Current.pElementBuffer->Name() == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_FOG:						//rcFog
	case RC_FOG_COLOR:					//rcFog
	case RC_FOG_DENSITY:				//rcFog
	case RC_FOG_END:					//rcFog
	case RC_FOG_HINT:					//rcHint
	case RC_FOG_MODE:					//rcFog
	case RC_FOG_START:					//rcFog
	case RC_FRONT_FACE:					//rcFrontFace
	case RC_GREEN_BITS:					//the number of green bitplanes in the color buffer
	case RC_LIGHT_MODEL_TWO_SIDE:		//rcLightModel
	case RC_IMPLEMENTATION_COLOR_READ_FORMAT_OES:	//rcReadPixels
	case RC_IMPLEMENTATION_COLOR_READ_TYPE_OES:		//rcReadPixels
	case RC_LIGHT_MODEL_AMBIENT:		//rcLightModel
		params[0] = RC_FALSE;
		break;
	//case RC_LIGHT0:
	//case RC_LIGHT1:
	//case RC_LIGHT2:
	//case RC_LIGHT3:
	//case RC_LIGHT4:
	//case RC_LIGHT5:
	//case RC_LIGHT6:
	//case RC_LIGHT7:
	//	if(!m_Light.IsEnable(pname, params))
	//		SetError(RC_INVALID_ENUM);
	//	break;
	case RC_LIGHTING:
		params[0] = m_Light.IsLighting();
		break;
	case RC_LINE_SMOOTH:				//rcLineWidth
	case RC_LINE_WIDTH:					//rcLineWidth
	case RC_LOGIC_OP_MODE:				//rcLogicOp
		params[0] = RC_FALSE;
		break;
	case RC_MATERIAL_BINDING:
		params[0] = (m_Current.pMaterial->Name() == 0)? RC_FALSE:RC_TRUE;		
		break;
	case RC_MAX_CLIP_PLANES:			//rcClipPlane
		params[0] = RC_FALSE;
		break;
	case RC_MAX_LIGHTS:
		params[0] = (RCDDK_MAX_LIGHT_COUNT == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_MAX_MODELVIEW_STACK_DEPTH:
		params[0] = (RCDDK_MAX_MATRIX_STACK_COUNT == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_MAX_PALETTE_MATRICES_OES:
		params[0] = (RCDDK_MAX_MATRIX_PALETTE_COUNT == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_MAX_PROJECTION_STACK_DEPTH:
		params[0] = (RCDDK_MAX_MATRIX_STACK_COUNT == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_MAX_TEXTURE_SIZE:
		params[0] = (MAX_TEXTURE_SIZE == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_MAX_TEXTURE_STACK_DEPTH:
		params[0] = (RCDDK_MAX_MATRIX_STACK_COUNT == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_MAX_TEXTURE_UNITS:			//rcActiveTexture
		params[0] = RC_TRUE;
		// @todo : multi texture 지원시 5(RC_TEXTURE0 ~ RC_TEXTURE4)?
		break;
	case RC_MAX_THRESHOLD_LEVELS:
		params[0] = (RCDDK_MAX_THRESHOLD_LEVEL_COUNT == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_MAX_VERTEX_UNITS_OES:
		params[0] = (RCDDK_MAX_VERTEX_UNITS_COUNT == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_MAX_VIEWPORT_DIMS:
		params[0] = (RCDDK_MAX_VIEWPORT_DIMS == 0)? RC_FALSE:RC_TRUE;
		params[1] = (RCDDK_MAX_VIEWPORT_DIMS == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_MATRIX_INDEX_ARRAY_OES:
		params[0] = m_MatrixIndexArray.enabled;
		break;
	case RC_MATRIX_INDEX_ARRAY_BUFFER_BINDING_OES:
		params[0] = (m_MatrixIndexArray.boundBuffer == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_MATRIX_INDEX_ARRAY_SIZE_OES:
		params[0] = (m_MatrixIndexArray.size == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_MATRIX_INDEX_ARRAY_STRIDE_OES:
		params[0] = (m_MatrixIndexArray.stride == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_MATRIX_INDEX_ARRAY_TYPE_OES:
		params[0] = (m_MatrixIndexArray.type == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_MATRIX_MODE:
		params[0] = (m_dwCurrentMatrixMode == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_MATRIX_PALETTE_OES:
		params[0] = m_CurrentStateFlag.etc.matrix_palette;
		break;
	case RC_MODELVIEW_MATRIX:
		if(m_iMatrixStackIndexModelView >= 0)
			LOOP(16) params[i] = (m_MatrixStackModelView[m_iMatrixStackIndexModelView].V_matrix.m[16] == 0) ? RC_FALSE : RC_TRUE;
		else
			LOOP(16) params[i] = RC_FALSE;
		break;
	case RC_MODELVIEW_STACK_DEPTH:
		params[0] = (m_iMatrixStackIndexModelView + 1 == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_MULTISAMPLE:				//whether multisampling is enabled
		params[0] = RC_TRUE;
		break;
	case RC_NORMAL_ARRAY:
		params[0] = m_NormalArray.enabled;
		break;
	case RC_NORMAL_ARRAY_BUFFER_BINDING:
		params[0] = (m_NormalArray.boundBuffer == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_NORMAL_ARRAY_STRIDE:
		params[0] = (m_NormalArray.stride == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_NORMAL_ARRAY_TYPE:
		params[0] = (m_NormalArray.type == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_NORMALIZE:						//rcNormal
	case RC_NUM_COMPRESSED_TEXTURE_FORMATS:	//rcCompressedTexImage2D
	case RC_PACK_ALIGNMENT:					//rcPixelStorei
	case RC_PERSPECTIVE_CORRECTION_HINT:	//rcHint
	case RC_POINT_DISTANCE_ATTENUATION:		//rcPointParameter
	case RC_POINT_FADE_THRESHOLD_SIZE:		//rcPointParameter
	case RC_POINT_SIZE:						//rcPointSize
	case RC_POINT_SIZE_ARRAY_BUFFER_BINDING_OES:	//rcPointSizePointerOES
	case RC_POINT_SIZE_ARRAY_OES:					//rcPointSizePointerOES
	case RC_POINT_SIZE_ARRAY_STRIDE_OES:			//rcPointSizePointerOES
	case RC_POINT_SIZE_ARRAY_TYPE_OES:				//rcPointSizePointerOES
	case RC_POINT_SIZE_MAX:					//rcPointParameter
	case RC_POINT_SIZE_MIN:					//rcPointParameter
	case RC_POINT_SMOOTH_HINT:				//rcHint
	case RC_POINT_SMOOTH:					//rcPointSize
	case RC_POINT_SPRITE_OES:				//rcTGexEnv
	case RC_POLYGON_OFFSET_FACTOR:			//rcPolygonOffset
	case RC_POLYGON_OFFSET_FILL:			//rcPolygonOffset
	case RC_POLYGON_OFFSET_UNITS:			//rcPolygonOffset
		params[0] = RC_FALSE;
		break;
	case RC_PROJECTION_MATRIX:
		if(m_iMatrixStackIndexProjection >= 0)
			LOOP(16) params[i] = (m_MatrixStackProjection[m_iMatrixStackIndexProjection].V_matrix.m[16] == 0)? RC_FALSE : RC_TRUE;
		else
			LOOP(16) params[i] = RC_FALSE;
		break;
	case RC_PROJECTION_STACK_DEPTH:
		params[0] = (m_iMatrixStackIndexProjection + 1 == 0)? RC_FALSE : RC_TRUE;
		break;
	case RC_RED_BITS:					//the number of red bitplanes in each color buffer
	case RC_RESCALE_NORMAL:				//rcNormal
	case RC_SAMPLE_ALPHA_TO_COVERAGE:	//rcSampleCoverage
	case RC_SAMPLE_ALPHA_TO_ONE:		//rcSampleCoverage
	case RC_SAMPLE_BUFFERS:				//rcSampleCoverage
	case RC_SAMPLE_COVERAGE_VALUE:		//rcSampleCoverage
	case RC_SAMPLES:					//rcSampleCoverage
	case RC_SAMPLE_COVERAGE:			//rcSampleCoverage
	case RC_SAMPLE_COVERAGE_INVERT:		//rcSampleCoverage
	case RC_SCISSOR_BOX:				//rcScissor
	case RC_SCISSOR_TEST:				//rcScissor
	case RC_SHADE_MODEL:				//rcShadeModel
	case RC_SMOOTH_LINE_WIDTH_RANGE:	//rcLineWidth
	case RC_SMOOTH_POINT_SIZE_RANGE:	//rcPointSize
	case RC_STENCIL_BITS:				//the number of bitplanes in the stencil buffer
	case RC_STENCIL_CLEAR_VALUE:		//rcClearStencil
	case RC_STENCIL_FAIL:				//rcStencilOp
	case RC_STENCIL_FUNC:				//rcStencilFunc
	case RC_STENCIL_PASS_DEPTH_FAIL:	//rcStencilOp
	case RC_STENCIL_PASS_DEPTH_PASS:	//rcStencilOp
	case RC_STENCIL_REF:				//rcStencilFunc
	case RC_STENCIL_TEST:				//rcStencilFunc, rcStencilOp
	case RC_STENCIL_VALUE_MASK:			//rcStencilFunc
	case RC_STENCIL_WRITEMASK:			//rcStencilMask
	case RC_SUBPIXEL_BITS:				//an estimate of the number of bits of subpixel resolution
		params[0] = RC_FALSE;
		break;
	case RC_TEXTURE_2D:
		//params[0] = m_Current.pMaterial->IsTextureEnable();
		params[0] = m_Current.pMaterial->IsTextureMapEnable();
		break;
	case RC_TEXTURE_2D_NORMAL:
		//params[0] = m_Current.pMaterial->GetTextureType() == RCDDK_TEXTURE_TYPE_NORMAL;
		params[0] = m_Current.pMaterial->IsNormalMapEnable();
		break;
	case RC_TEXTURE_BINDING_2D:
		params[0] = (m_Current.pTexture->Name() == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_TEXTURE_COORD_ARRAY:
		params[0] = m_TexCoordArray.enabled;
		break;		
	case RC_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
		params[0] = (m_TexCoordArray.boundBuffer == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_TEXTURE_COORD_ARRAY_SIZE:
		params[0] = (m_TexCoordArray.size == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_TEXTURE_COORD_ARRAY_STRIDE:
		params[0] = (m_TexCoordArray.stride == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_TEXTURE_COORD_ARRAY_TYPE:
		params[0] = (m_TexCoordArray.type == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_TEXTURE_MATRIX:
		if(m_iMatrixStackIndexTexture >= 0)
			LOOP(16) params[i] = (m_MatrixStackTexture[m_iMatrixStackIndexTexture].V_matrix.m[16] == 0)? RC_FALSE : RC_TRUE;
		else
			LOOP(16) params[i] = RC_FALSE;
		break;
	case RC_TEXTURE_STACK_DEPTH:
		params[0] = (m_iMatrixStackIndexTexture + 1 == 0)? RC_FALSE:RC_TRUE;
		break;
		break;
	case RC_UNPACK_ALIGNMENT:				//rcPixelStorei
		break;
	case RC_USE_COLOR_SHADOW:
		params[0] = m_CurrentStateFlag.option.color_shadow;
		break;
	case RC_USE_SHADOW:
		params[0] = m_CurrentStateFlag.option.shadow;
		break;
	case RC_USE_TEXTURE_ALPHA_SHADOW:
		params[0] = m_CurrentStateFlag.primitive.texture_alpha_shadow;
		break;
	case RC_USE_TEXTURE_ONLY:
		params[0] = m_CurrentStateFlag.primitive.texture_only;
		break;
	case RC_USE_TRANSMITTANCE_SHADOW:
		params[0] = m_CurrentStateFlag.primitive.transmittance_shadow;
		break;		
	case RC_VERTEX_ARRAY:
		params[0] = m_VertexArray.enabled;
		break;
	case RC_VERTEX_ARRAY_BUFFER_BINDING:
		params[0] = (m_VertexArray.boundBuffer == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_VERTEX_ARRAY_SIZE:
		params[0] = (m_VertexArray.size == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_VERTEX_ARRAY_STRIDE:
		params[0] = (m_VertexArray.stride == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_VERTEX_ARRAY_TYPE:
		params[0] = (m_VertexArray.type == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_VIEWPORT:
		params[0] = (m_Screen.X == 0)? RC_FALSE:RC_TRUE;
		params[1] = (m_Screen.Y == 0)? RC_FALSE:RC_TRUE;
		params[2] = (m_Screen.Width == 0)? RC_FALSE:RC_TRUE;
		params[3] = (m_Screen.Height == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_WEIGHT_ARRAY_OES:
		params[0] = m_WeightArray.enabled;
		break;
	case RC_WEIGHT_ARRAY_BUFFER_BINDING_OES:
		params[0] = (m_WeightArray.boundBuffer == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_WEIGHT_ARRAY_SIZE_OES:
		params[0] = (m_WeightArray.size == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_WEIGHT_ARRAY_STRIDE_OES:
		params[0] = (m_WeightArray.stride == 0)? RC_FALSE:RC_TRUE;
		break;
	case RC_WEIGHT_ARRAY_TYPE_OES:
		params[0] = (m_WeightArray.type == 0)? RC_FALSE:RC_TRUE;
		break;
	default:
		SetError(RC_INVALID_ENUM);
		break;
	}
}

void ContextRC1::GetFixedv (RCenum pname, RCfixed *params){
	int i;
	RCfloat paramsf[16];
	RCint parami[4];
	RCboolean paramb;

	switch(pname){
	case RC_ACTIVE_TEXTURE:
	case RC_ALPHA_BITS:
	case RC_ARRAY_BUFFER_BINDING:
	case RC_BLEND_DST:
	case RC_BLEND_SRC:
	case RC_BLUE_BITS:
	case RC_CLIENT_ACTIVE_TEXTURE:
	case RC_COLOR_ARRAY_BUFFER_BINDING:
	case RC_COLOR_ARRAY_SIZE:
	case RC_COLOR_ARRAY_STRIDE:
	case RC_COLOR_ARRAY_TYPE:
	case RC_COMPRESSED_TEXTURE_FORMATS:
	case RC_CULL_FACE_MODE:
	case RC_DEPTH_BITS:
	case RC_DEPTH_FUNC:
	case RC_ELEMENT_ARRAY_BUFFER_BINDING:
	case RC_FOG_HINT:
	case RC_FOG_MODE:
	case RC_FRONT_FACE:
	case RC_GREEN_BITS:
	case RC_IMPLEMENTATION_COLOR_READ_FORMAT_OES:
	case RC_IMPLEMENTATION_COLOR_READ_TYPE_OES:
	case RC_LINE_SMOOTH_HINT:
	case RC_LOGIC_OP_MODE:
	case RC_MATERIAL_BINDING:
	case RC_MATRIX_MODE:
	case RC_MAX_CLIP_PLANES:
	case RC_MATRIX_INDEX_ARRAY_BUFFER_BINDING_OES:
	case RC_MATRIX_INDEX_ARRAY_SIZE_OES:
	case RC_MATRIX_INDEX_ARRAY_STRIDE_OES:
	case RC_MATRIX_INDEX_ARRAY_TYPE_OES:
	case RC_MAX_LIGHTS:
	case RC_MAX_MODELVIEW_STACK_DEPTH:
	case RC_MAX_PALETTE_MATRICES_OES:
	case RC_MAX_PROJECTION_STACK_DEPTH:
	case RC_MAX_TEXTURE_SIZE:
	case RC_MAX_TEXTURE_STACK_DEPTH:
	case RC_MAX_TEXTURE_UNITS:
	case RC_MAX_THRESHOLD_LEVELS:
	case RC_MAX_VERTEX_UNITS_OES:
	case RC_MAX_VIEWPORT_DIMS:
	case RC_MODELVIEW_STACK_DEPTH:
	case RC_NORMAL_ARRAY_BUFFER_BINDING:
	case RC_NORMAL_ARRAY_STRIDE:
	case RC_NORMAL_ARRAY_TYPE:
	case RC_NUM_COMPRESSED_TEXTURE_FORMATS:
	case RC_PACK_ALIGNMENT:
	case RC_PERSPECTIVE_CORRECTION_HINT:
	case RC_POINT_SIZE_ARRAY_BUFFER_BINDING_OES:
	case RC_POINT_SIZE_ARRAY_STRIDE_OES:
	case RC_POINT_SIZE_ARRAY_TYPE_OES:
	case RC_POINT_SMOOTH_HINT:
	case RC_PROJECTION_STACK_DEPTH:
	case RC_RED_BITS:
	case RC_SAMPLE_BUFFERS:
	case RC_SAMPLES:
	case RC_SHADE_MODEL:
	case RC_STENCIL_BITS:
	case RC_STENCIL_CLEAR_VALUE:
	case RC_STENCIL_FAIL:
	case RC_STENCIL_FUNC:
	case RC_STENCIL_PASS_DEPTH_FAIL:
	case RC_STENCIL_PASS_DEPTH_PASS:
	case RC_STENCIL_REF:
	case RC_STENCIL_VALUE_MASK:
	case RC_STENCIL_WRITEMASK:
	case RC_SUBPIXEL_BITS:
	case RC_TEXTURE_BINDING_2D:
	case RC_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
	case RC_TEXTURE_COORD_ARRAY_SIZE:
	case RC_TEXTURE_COORD_ARRAY_STRIDE:
	case RC_TEXTURE_COORD_ARRAY_TYPE:
	case RC_TEXTURE_STACK_DEPTH:
	case RC_UNPACK_ALIGNMENT:
	case RC_VIEWPORT:
	case RC_VERTEX_ARRAY_BUFFER_BINDING:
	case RC_VERTEX_ARRAY_SIZE:
	case RC_VERTEX_ARRAY_STRIDE:
	case RC_VERTEX_ARRAY_TYPE:
	case RC_WEIGHT_ARRAY_BUFFER_BINDING_OES:
	case RC_WEIGHT_ARRAY_SIZE_OES:
	case RC_WEIGHT_ARRAY_STRIDE_OES:
	case RC_WEIGHT_ARRAY_TYPE_OES:
		{
			GetIntegerv(pname, parami);	
			params[0] = parami[0];
			if(pname == RC_MAX_VIEWPORT_DIMS)
				params[1] = parami[1];
			else if(pname == RC_VIEWPORT) {
				params[1] = parami[1];
				params[2] = parami[2];
				params[3] = parami[3];
			}
		}
		break;

	case RC_ALPHA_TEST:
	case RC_BLEND:
	case RC_CLIP_PLANE0:
	case RC_CLIP_PLANE1:
	case RC_CLIP_PLANE2:
	case RC_CLIP_PLANE3:
	case RC_CLIP_PLANE4:
	case RC_CLIP_PLANE5:
	case RC_COLOR_ARRAY:
	case RC_COLOR_LOGIC_OP:
	case RC_COLOR_MATERIAL:
	case RC_CULL_FACE:
	case RC_DEPTH_TEST:
	case RC_DEPTH_WRITEMASK:
	case RC_DITHER:
	case RC_FOG:
	case RC_LIGHT_MODEL_TWO_SIDE:
	case RC_LIGHT0:
	case RC_LIGHT1:
	case RC_LIGHT2:
	case RC_LIGHT3:
	case RC_LIGHT4:
	case RC_LIGHT5:
	case RC_LIGHT6:
	case RC_LIGHT7:
	case RC_LIGHTING:
	case RC_LINE_SMOOTH:
	case RC_MATRIX_INDEX_ARRAY_OES:
	case RC_MATRIX_PALETTE_OES:
	case RC_MULTISAMPLE:
	case RC_NORMAL_ARRAY:
	case RC_NORMALIZE:
	case RC_POINT_SIZE_ARRAY_OES:
	case RC_POINT_SMOOTH:
	case RC_POINT_SPRITE_OES:
	case RC_POLYGON_OFFSET_FILL:
	case RC_RESCALE_NORMAL:
	case RC_SAMPLE_ALPHA_TO_COVERAGE:
	case RC_SAMPLE_ALPHA_TO_ONE:
	case RC_SAMPLE_COVERAGE:
	case RC_SAMPLE_COVERAGE_INVERT:
	case RC_SCISSOR_TEST:
	case RC_STENCIL_TEST:
	case RC_TEXTURE_2D:
	case RC_TEXTURE_2D_NORMAL:
	case RC_TEXTURE_COORD_ARRAY:
	case RC_USE_COLOR_SHADOW:
	case RC_USE_SHADOW:
	case RC_USE_TEXTURE_ALPHA_SHADOW:
	case RC_USE_TEXTURE_ONLY:
	case RC_USE_TRANSMITTANCE_SHADOW:	
	case RC_VERTEX_ARRAY:
	case RC_WEIGHT_ARRAY_OES:
		GetBooleanv(pname, &paramb);
		params[0] = (RCfixed)paramb;
		break;

	default:
		{
			GetFloatv(pname, paramsf);

			switch(pname){
			case RC_ALPHA_TEST_FUNC:
			case RC_ALPHA_TEST_REF:
			case RC_DEPTH_CLEAR_VALUE:
			case RC_FOG_DENSITY:
			case RC_FOG_END:
			case RC_FOG_START:
			case RC_LINE_WIDTH:
			case RC_NUM_COMPRESSED_TEXTURE_FORMATS:
			case RC_POINT_FADE_THRESHOLD_SIZE:
			case RC_POINT_SIZE:
			case RC_POINT_SIZE_MAX:
			case RC_POINT_SIZE_MIN:
			case RC_POLYGON_OFFSET_FACTOR:
			case RC_POLYGON_OFFSET_UNITS:
			case RC_SAMPLE_COVERAGE_VALUE:
				params[0] = (RCfixed) f32x32(paramsf[0]);
				break;
			case RC_ALIASED_POINT_SIZE_RANGE:
			case RC_ALIASED_LINE_WIDTH_RANGE:
			case RC_DEPTH_RANGE:
			case RC_SMOOTH_LINE_WIDTH_RANGE:
			case RC_SMOOTH_POINT_SIZE_RANGE:
				LOOP(2) params[i] = (RCfixed) f32x32(paramsf[i]);
				break;
			case RC_CURRENT_NORMAL:
			case RC_POINT_DISTANCE_ATTENUATION:
				LOOP(3) params[i] = (RCfixed) f32x32(paramsf[i]);
				break;
			case RC_MODELVIEW_MATRIX:
			case RC_PROJECTION_MATRIX:
			case RC_TEXTURE_MATRIX:
				LOOP(16) params[i] = (RCfixed) f32x32(paramsf[i]);
				break;
			default:
				LOOP(4) params[i] = (RCfixed) f32x32(paramsf[i]);
				break;
			}
		}
		break;
	}
}

void ContextRC1::GetIntegerv (RCenum pname, RCint *params){
	int i = 0;
	switch(pname){
	case RC_ACTIVE_TEXTURE:				//rcActiveTexture
		params[0] = RC_TEXTURE0;
		break;
	case RC_ALIASED_POINT_SIZE_RANGE:	//rcPointSIze
	case RC_ALIASED_LINE_WIDTH_RANGE:	//rcLineWidth
	case RC_ALPHA_BITS:					//the number of alpha bitplanes in the color buffer
	case RC_ALPHA_TEST:					//rcAlphaFunc
	case RC_ALPHA_TEST_FUNC:			//rcAlphaFunc
	case RC_ALPHA_TEST_REF:				//rcAlphaFunc
		params[0] = 0;
		break;
	case RC_ARRAY_BUFFER_BINDING:
		params[0] = 0;
		if(m_Current.pBuffer) params[0] = m_Current.pBuffer->Name();
		break;
	case RC_BLEND:						//rcBlendFunc, rcLogiOp
	case RC_BLEND_DST:					//rcBlendFunc
	case RC_BLEND_SRC:					//rcBlendFunc
	case RC_BLUE_BITS:					//the number of blue bitplanes in the color buffer
		params[0] = 0;
		break;
	case RC_CLIENT_ACTIVE_TEXTURE:		//rcClientActiveTexture
		params[0] = RC_TEXTURE0;
		break;
	case RC_CLIP_PLANE0:				//rcClipPlane
	case RC_CLIP_PLANE1:				//rcClipPlane
	case RC_CLIP_PLANE2:				//rcClipPlane
	case RC_CLIP_PLANE3:				//rcClipPlane
	case RC_CLIP_PLANE4:				//rcClipPlane
	case RC_CLIP_PLANE5:				//rcClipPlane
		params[0] = 0;
		break;
	case RC_COLOR_ARRAY:
		params[0] = m_ColorArray.enabled;
		break;
	case RC_COLOR_ARRAY_BUFFER_BINDING:
		params[0] = m_ColorArray.boundBuffer;
		break;
	case RC_COLOR_ARRAY_SIZE:
		params[0] = m_ColorArray.size;
		break;
	case RC_COLOR_ARRAY_STRIDE:
		params[0] = m_ColorArray.stride;
		break;
	case RC_COLOR_ARRAY_TYPE:
		params[0] = m_ColorArray.type;
		break;
	case RC_COLOR_CLEAR_VALUE:
		params[0] = (m_Screen.clearRed < 0) ? 0 : (RCint)(255.f * m_Screen.clearRed);
		params[1] = (m_Screen.clearGreen < 0) ? 0 : (RCint)(255.f * m_Screen.clearGreen);
		params[2] = (m_Screen.clearBlue < 0) ? 0 : (RCint)(255.f * m_Screen.clearBlue);
		params[4] = (m_Screen.clearAlpha < 0) ? 0 : (RCint)(255.f * m_Screen.clearAlpha);
		break;
	case RC_COLOR_LOGIC_OP:				//rcLogicOp
	case RC_COLOR_MATERIAL:				//rcMaterial, rcEnable(RC_COLOR_MATERIAL)		
	case RC_COLOR_WRITEMASK:			//rcColorMask
	case RC_COMPRESSED_TEXTURE_FORMATS:	//rcCompressedTexImage2D, rcCompressedTexSubImage2D
	case RC_CULL_FACE:					//rcCullFace
		params[0] = 0;
		break;
	case RC_CULL_FACE_MODE:				//rcCullFace
		params[0] = RC_BACK;
		break;
	case RC_CURRENT_COLOR:
		LOOP(4) params[i] = (m_fDefaultColor[i] < 0) ? 0 : (RCint)(255.f * m_fDefaultColor[i]);
		break;
	case RC_CURRENT_AMBIENT:
		{
			RCVECTOR3f* pColor		= m_Current.pMaterial->GetAmbient();
			params[0] = (pColor->r < 0) ? 0 : (RCint)(255.f * pColor->r);
			params[1] = (pColor->g < 0) ? 0 : (RCint)(255.f * pColor->g);
			params[2] = (pColor->b < 0) ? 0 : (RCint)(255.f * pColor->b);
			params[3] = 255;
		}
		break;
	case RC_CURRENT_DIFFUSE:
		{
			RCVECTOR3f* pColor		= m_Current.pMaterial->GetDiffuse();
			params[0] = (pColor->r < 0) ? 0 : (RCint)(255.f * pColor->r);
			params[1] = (pColor->g < 0) ? 0 : (RCint)(255.f * pColor->g);
			params[2] = (pColor->b < 0) ? 0 : (RCint)(255.f * pColor->b);
			params[3] = 255;
		}
		break;
	case RC_CURRENT_SPECULAR:
		{
			RCVECTOR3f* pColor		= m_Current.pMaterial->GetSpecular();
			params[0] = (pColor->r < 0) ? 0 : (RCint)(255.f * pColor->r);
			params[1] = (pColor->g < 0) ? 0 : (RCint)(255.f * pColor->g);
			params[2] = (pColor->b < 0) ? 0 : (RCint)(255.f * pColor->b);
			params[3] = 255;
		}
		break;
	case RC_CURRENT_NORMAL:				//rcNormal
		params[0] = 0;
		params[1] = 0;
		params[2] = 0;
		break;
	case RC_CURRENT_TEXTURE_COORDS:		//rcMultiTexCoord
	case RC_DEPTH_BITS:					//the number of bitplanes in the depth buffer
	case RC_DEPTH_CLEAR_VALUE:			//rcClearDepth
	case RC_DEPTH_FUNC:					//rcDepthFunc
	case RC_DEPTH_RANGE:				//rcDepthRange
	case RC_DEPTH_TEST:					//rcDepthFunc, rcDepthRange
	case RC_DEPTH_WRITEMASK:			//rcDepthMask
	case RC_DITHER:
		params[0] = 0;
		break;
	case RC_ELEMENT_ARRAY_BUFFER_BINDING:
		params[0] = 0;
		if(m_Current.pElementBuffer) params[0] = m_Current.pElementBuffer->Name();
		break;
	case RC_FOG:						//rcFog
	case RC_FOG_COLOR:					//rcFog
	case RC_FOG_DENSITY:				//rcFog
	case RC_FOG_END:					//rcFog
	case RC_FOG_HINT:					//rcHint
	case RC_FOG_MODE:					//rcFog
	case RC_FOG_START:					//rcFog
	case RC_FRONT_FACE:					//rcFrontFace
	case RC_GREEN_BITS:					//the number of green bitplanes in the color buffer
	case RC_IMPLEMENTATION_COLOR_READ_FORMAT_OES:	//rcReadPixels
	case RC_IMPLEMENTATION_COLOR_READ_TYPE_OES:		//rcReadPixels
	case RC_LIGHT_MODEL_AMBIENT:		//rcLightModel
	case RC_LIGHT_MODEL_TWO_SIDE:		//rcLightModel
		params[0] = 0;
		break;
	case RC_LIGHT0:
	case RC_LIGHT1:
	case RC_LIGHT2:
	case RC_LIGHT3:
	case RC_LIGHT4:
	case RC_LIGHT5:
	case RC_LIGHT6:
	case RC_LIGHT7:
		{
			RCboolean isEnable;
			if(!m_Light.IsEnable(pname, &isEnable))
				SetError(RC_INVALID_ENUM);
			params[0] = isEnable;
		}
		break;
	case RC_LIGHTING:
		params[0] = m_Light.IsLighting();
		break;
	case RC_LINE_SMOOTH:				//rcLineWidth
	case RC_LINE_SMOOTH_HINT:			//rcHint
	case RC_LINE_WIDTH:					//rcLineWidth
	case RC_LOGIC_OP_MODE:				//rcLogicOp
		params[0] = 0;
		break;
	case RC_MATERIAL_BINDING:
		params[0] = m_Current.pMaterial->Name();
		break;
	case RC_MAX_CLIP_PLANES:			//rcClipPlane
		params[0] = 0;
		break;
	case RC_MAX_LIGHTS:
		params[0] = RCDDK_MAX_LIGHT_COUNT;
		break;
	case RC_MAX_MODELVIEW_STACK_DEPTH:
		params[0] = RCDDK_MAX_MATRIX_STACK_COUNT;
		break;
	case RC_MAX_PALETTE_MATRICES_OES:
		params[0] = RCDDK_MAX_MATRIX_PALETTE_COUNT;
		break;
	case RC_MAX_PROJECTION_STACK_DEPTH:
		params[0] = RCDDK_MAX_MATRIX_STACK_COUNT;
		break;
	case RC_MAX_TEXTURE_SIZE:
		params[0] = MAX_TEXTURE_SIZE;
		break;
	case RC_MAX_TEXTURE_STACK_DEPTH:
		params[0] = RCDDK_MAX_MATRIX_STACK_COUNT;
		break;
	case RC_MAX_TEXTURE_UNITS:			//rcActiveTexture
		params[0] = 1;
		// @todo : multi texture 지원시 5(RC_TEXTURE0 ~ RC_TEXTURE4)?
		break;
	case RC_MAX_THRESHOLD_LEVELS:
		params[0] = RCDDK_MAX_THRESHOLD_LEVEL_COUNT;
		break;
	case RC_MAX_VERTEX_UNITS_OES:
		params[0] = RCDDK_MAX_VERTEX_UNITS_COUNT;
		break;
	case RC_MAX_VIEWPORT_DIMS:
		params[0] = RCDDK_MAX_VIEWPORT_DIMS;
		params[1] = RCDDK_MAX_VIEWPORT_DIMS;
		break;
	case RC_MATRIX_MODE:
		params[0] = m_dwCurrentMatrixMode;
		break;
	case RC_MATRIX_INDEX_ARRAY_OES:
		params[0] = m_MatrixIndexArray.enabled;
		break;
	case RC_MATRIX_INDEX_ARRAY_BUFFER_BINDING_OES:
		params[0] = m_MatrixIndexArray.boundBuffer;
		break;
	case RC_MATRIX_INDEX_ARRAY_SIZE_OES:
		params[0] = m_MatrixIndexArray.size;
		break;
	case RC_MATRIX_INDEX_ARRAY_STRIDE_OES:
		params[0] = m_MatrixIndexArray.stride;
		break;
	case RC_MATRIX_INDEX_ARRAY_TYPE_OES:
		params[0] = m_MatrixIndexArray.type;
		break;
	case RC_MATRIX_PALETTE_OES:
		params[0] = m_CurrentStateFlag.etc.matrix_palette;
		break;
	case RC_MODELVIEW_MATRIX:
		memset(params, 0, SIZEOFIDENTITYMATRIX);
		if(m_iMatrixStackIndexModelView >= 0)
			LOOP(16) params[i] = GetRoundInt(m_MatrixStackModelView[m_iMatrixStackIndexModelView].V_matrix.m[i]);
		break;
	case RC_MODELVIEW_STACK_DEPTH:
		params[0] = m_iMatrixStackIndexModelView + 1;
		break;
	case RC_MULTISAMPLE:				//whether multisampling is enabled
		params[0] = 0;
		break;
	case RC_NORMAL_ARRAY:
		params[0] = m_NormalArray.enabled;
		break;
	case RC_NORMAL_ARRAY_BUFFER_BINDING:
		params[0] = m_NormalArray.boundBuffer;
		break;
	case RC_NORMAL_ARRAY_STRIDE:
		params[0] = m_NormalArray.stride;
		break;
	case RC_NORMAL_ARRAY_TYPE:
		params[0] = m_NormalArray.type;
		break;
	case RC_NORMALIZE:						//rcNormal
	case RC_NUM_COMPRESSED_TEXTURE_FORMATS:	//rcCompressedTexImage2D
	case RC_PACK_ALIGNMENT:					//rcPixelStorei
	case RC_PERSPECTIVE_CORRECTION_HINT:	//rcHint
	case RC_POINT_DISTANCE_ATTENUATION:		//rcPointParameter
	case RC_POINT_FADE_THRESHOLD_SIZE:		//rcPointParameter
	case RC_POINT_SIZE:						//rcPointSize
	case RC_POINT_SIZE_ARRAY_BUFFER_BINDING_OES:	//rcPointSizePointerOES
	case RC_POINT_SIZE_ARRAY_OES:			//rcPointSizePointerOES
	case RC_POINT_SIZE_ARRAY_STRIDE_OES:	//rcPointSizePointerOES
	case RC_POINT_SIZE_ARRAY_TYPE_OES:		//rcPointSizePointerOES
	case RC_POINT_SIZE_MAX:					//rcPointParameter
	case RC_POINT_SIZE_MIN:					//rcPointParameter
	case RC_POINT_SMOOTH:					//rcPointSize
	case RC_POINT_SMOOTH_HINT:				//rcHint
	case RC_POINT_SPRITE_OES:				//rcTGexEnv		
	case RC_POLYGON_OFFSET_FACTOR:			//rcPolygonOffset
	case RC_POLYGON_OFFSET_FILL:			//rcPolygonOffset
	case RC_POLYGON_OFFSET_UNITS:			//rcPolygonOffset
		params[0] = 0;
		break;
	case RC_PROJECTION_MATRIX:
		memset(params, 0, SIZEOFIDENTITYMATRIX);
		if(m_iMatrixStackIndexProjection >= 0)
			LOOP(16) params[i] = GetRoundInt(m_MatrixStackProjection[m_iMatrixStackIndexProjection].V_matrix.m[i]);
		break;
	case RC_PROJECTION_STACK_DEPTH:
		params[0] = m_iMatrixStackIndexProjection + 1;
		break;
	case RC_RED_BITS:					//the number of red bitplanes in each color buffer
	case RC_SAMPLE_BUFFERS:				//rcSampleCoverage
	case RC_SAMPLES:					//rcSampleCoverage
	case RC_RESCALE_NORMAL:				//rcNormal
	case RC_SHADE_MODEL:				//rcShadeModel
	case RC_SAMPLE_ALPHA_TO_COVERAGE:	//rcSampleCoverage
	case RC_SAMPLE_ALPHA_TO_ONE:		//rcSampleCoverage
	case RC_SAMPLE_COVERAGE:			//rcSampleCoverage
	case RC_SAMPLE_COVERAGE_INVERT:		//rcSampleCoverage
	case RC_SAMPLE_COVERAGE_VALUE:		//rcSampleCoverage
	case RC_SCISSOR_BOX:				//rcScissor
	case RC_SCISSOR_TEST:				//rcScissor
	case RC_SMOOTH_LINE_WIDTH_RANGE:	//rcLineWidth
	case RC_SMOOTH_POINT_SIZE_RANGE:	//rcPointSize
	case RC_STENCIL_BITS:				//the number of bitplanes in the stencil buffer
	case RC_STENCIL_CLEAR_VALUE:		//rcClearStencil
	case RC_STENCIL_FAIL:				//rcStencilOp
	case RC_STENCIL_TEST:				//rcStencilFunc, rcStencilOp
	case RC_STENCIL_FUNC:				//rcStencilFunc
	case RC_STENCIL_PASS_DEPTH_FAIL:	//rcStencilOp
	case RC_STENCIL_PASS_DEPTH_PASS:	//rcStencilOp
	case RC_STENCIL_REF:				//rcStencilFunc
	case RC_STENCIL_VALUE_MASK:			//rcStencilFunc
	case RC_STENCIL_WRITEMASK:			//rcStencilMask
	case RC_SUBPIXEL_BITS:				//an estimate of the number of bits of subpixel resolution
		params[0] = 0;
		break;
	case RC_TEXTURE_2D:
		//params[0] = m_Current.pMaterial->IsTextureEnable();
		params[0] = m_Current.pMaterial->IsTextureMapEnable();
		break;
	case RC_TEXTURE_2D_NORMAL:
		params[0] = m_Current.pMaterial->IsNormalMapEnable();
		break;
	case RC_TEXTURE_BINDING_2D:
		params[0] = m_Current.pTexture->Name();
		break;
	case RC_TEXTURE_COORD_ARRAY:
		params[0] = m_TexCoordArray.enabled;
		break;
	case RC_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
		params[0] = m_TexCoordArray.boundBuffer;
		break;
	case RC_TEXTURE_COORD_ARRAY_SIZE:
		params[0] = m_TexCoordArray.size;
		break;
	case RC_TEXTURE_COORD_ARRAY_STRIDE:
		params[0] = m_TexCoordArray.stride;
		break;
	case RC_TEXTURE_COORD_ARRAY_TYPE:
		params[0] = m_TexCoordArray.type;
		break;
	case RC_TEXTURE_MATRIX:
		memset(params, 0, SIZEOFIDENTITYMATRIX);
		if(m_iMatrixStackIndexTexture >= 0)
			LOOP(16) params[i] = GetRoundInt(m_MatrixStackTexture[m_iMatrixStackIndexTexture].V_matrix.m[i]);
		break;
	case RC_TEXTURE_STACK_DEPTH:
		params[0] = m_iMatrixStackIndexTexture + 1;
		break;
	case RC_UNPACK_ALIGNMENT:			//rcPixelStorei
		params[0] = 0;
		break;
	case RC_USE_COLOR_SHADOW:
		params[0] = m_CurrentStateFlag.option.color_shadow;
		break;
	case RC_USE_SHADOW:
		params[0] = m_CurrentStateFlag.option.shadow;
		break;
	case RC_USE_TEXTURE_ALPHA_SHADOW:
		params[0] = m_CurrentStateFlag.primitive.texture_alpha_shadow;
		break;
	case RC_USE_TEXTURE_ONLY:
		params[0] = m_CurrentStateFlag.primitive.texture_only;
		break;
	case RC_USE_TRANSMITTANCE_SHADOW:
		params[0] = m_CurrentStateFlag.primitive.transmittance_shadow;
		break;		
	case RC_VIEWPORT:
		params[0] = m_Screen.X;
		params[1] = m_Screen.Y;
		params[2] = m_Screen.Width;
		params[3] = m_Screen.Height;
		break;
	case RC_VERTEX_ARRAY:
		params[0] = m_VertexArray.enabled;
		break;
	case RC_VERTEX_ARRAY_BUFFER_BINDING:
		params[0] = m_VertexArray.boundBuffer;
		break;
	case RC_VERTEX_ARRAY_SIZE:
		params[0] = m_VertexArray.size;
		break;
	case RC_VERTEX_ARRAY_STRIDE:
		params[0] = m_VertexArray.stride;
		break;
	case RC_VERTEX_ARRAY_TYPE:
		params[0] = m_VertexArray.type;
		break;
	case RC_WEIGHT_ARRAY_OES:
		params[0] = m_WeightArray.enabled;
		break;
	case RC_WEIGHT_ARRAY_BUFFER_BINDING_OES:
		params[0] = m_WeightArray.boundBuffer;
		break;
	case RC_WEIGHT_ARRAY_SIZE_OES:
		params[0] = m_WeightArray.size;
		break;
	case RC_WEIGHT_ARRAY_STRIDE_OES:
		params[0] = m_WeightArray.stride;
		break;
	case RC_WEIGHT_ARRAY_TYPE_OES:
		params[0] = m_WeightArray.type;
		break;
	default:
		SetError(RC_INVALID_ENUM);
		break;
	}
}

void ContextRC1::GetPointerv (RCenum pname, RCvoid **params)
{
	switch (pname) {
	case RC_COLOR_ARRAY_POINTER:
		params[0] = const_cast<void *>(m_ColorArray.pData);
		break;
	case RC_MATRIX_INDEX_ARRAY_POINTER_OES:
		params[0] = const_cast<void *>(m_MatrixIndexArray.pData);
		break;
	case RC_NORMAL_ARRAY_POINTER:
		params[0] = const_cast<void *>(m_NormalArray.pData);
		break;
	case RC_POINT_SIZE_ARRAY_POINTER_OES:		
		params[0] = 0;
		break;
	case RC_TEXTURE_COORD_ARRAY_POINTER:
		// @todo : 추후 Multi Texture를 지원할 경우 texCoordArray[m_clientActiveTexture]의 형태로 수정해야 함
		params[0] = const_cast<void *>(m_TexCoordArray.pData);
		break;
	case RC_VERTEX_ARRAY_POINTER:
		params[0] = const_cast<void *>(m_VertexArray.pData);
		break;
	case RC_WEIGHT_ARRAY_POINTER_OES:
		params[0] = const_cast<void *>(m_WeightArray.pData);
		break;
	default:
		SetError(RC_INVALID_ENUM);
		break;
	}
}

void ContextRC1::GetTexParameterv (RCenum target, RCenum pname, RCint *params)
{
	if (target != RC_TEXTURE_2D && target != RC_TEXTURE_2D_NORMAL) {
		SetError(RC_INVALID_ENUM);
		return;
	}

	// @todo : RayCore에서 지원시 추후 적용해야 함 (filtering, mipmap gen. on/off)
	switch (pname) {
	case RC_GENERATE_MIPMAP:
		params[0] = m_bGenerateMipmap;
		break;
	case RC_TEXTURE_MIN_FILTER:
		params[0] = m_TextureMinFilter + RC_NEAREST;
		break;
	case RC_TEXTURE_MAG_FILTER:
		params[0] = m_TextureMagFilter + RC_NEAREST;
		break;
	case RC_TEXTURE_WRAP_S:
	case RC_TEXTURE_WRAP_T:
		params[0] = m_Current.pMaterial->GetTexParam(target, pname);
		break;
	default:
		SetError(RC_INVALID_ENUM);
		break;
	}
}

void ContextRC1::GetBufferParameterv (RCenum target, RCenum pname, RCint *params)
{
	rcBuffer *pBuffer=NULL;

	switch (target){
	case RC_ARRAY_BUFFER:
		pBuffer = m_Current.pBuffer;
		break;
	case RC_ELEMENT_ARRAY_BUFFER:
		pBuffer = m_Current.pElementBuffer;
		break;
	default:
		SetError(RC_INVALID_ENUM);
		break;
	}	

	if (m_Container.Buffer.ObjectSize() == 0 || !pBuffer) {
		SetError(RC_INVALID_OPERATION);
		return;
	}

	switch (pname) {
	case RC_BUFFER_SIZE:
		params[0] = pBuffer->Get()->size;
		break;
	case RC_BUFFER_USAGE:
		params[0] = pBuffer->Get()->usage;
		break;		
	default:
		SetError(RC_INVALID_ENUM);
		return;
	}
}

static const char*	__STRING_VENDOR		= "Siliconarts Inc.";
static const char*	__STRING_RENDERER	= "RayCore";
static const char*	__STRING_VERSION	= "RayCore API 1.0";
static const char*	__STRING_EXTENSIONS	= "";

const RCubyte * ContextRC1::GetString (RCenum name){
	switch(name){
	case RC_VENDOR:		return (RCubyte*)__STRING_VENDOR;
	case RC_RENDERER:	return (RCubyte*)__STRING_RENDERER;
	case RC_VERSION:	return (RCubyte*)__STRING_VERSION;
	case RC_EXTENSIONS:	return (RCubyte*)__STRING_EXTENSIONS;
	default:			SetError(RC_INVALID_ENUM);
	}
	return NULL;
}

RCboolean ContextRC1::IsBuffer (RCuint buffer){	
	return m_Container.Buffer.Find(buffer) != NULL;
}

RCboolean ContextRC1::IsEnabled (RCenum cap){
	switch(cap){
	case RC_ALPHA_TEST:
	case RC_BLEND:
	case RC_CLIP_PLANE0:
	case RC_CLIP_PLANE1:
	case RC_CLIP_PLANE2:
	case RC_CLIP_PLANE3:
	case RC_CLIP_PLANE4:
	case RC_CLIP_PLANE5:
		return 0;
		break;
	case RC_COLOR_ARRAY:
		return m_ColorArray.enabled;
		break;
	case RC_COLOR_LOGIC_OP:
	case RC_COLOR_MATERIAL:
	case RC_CULL_FACE:
	case RC_DEPTH_TEST:
	case RC_DITHER:
	case RC_FOG:
		return 0;
		break;
	case RC_LIGHT0:
	case RC_LIGHT1:
	case RC_LIGHT2:
	case RC_LIGHT3:
	case RC_LIGHT4:
	case RC_LIGHT5:
	case RC_LIGHT6:
	case RC_LIGHT7:
		{
			RCboolean isEnable=0;
			if(m_Light.IsEnable(cap, &isEnable))
				return isEnable;
			else
				SetError(RC_INVALID_ENUM);
		}
		break;
	case RC_LIGHTING:
		return m_Light.IsLighting();
	case RC_LINE_SMOOTH:
		break;
	case RC_MATRIX_INDEX_ARRAY_OES:
		return m_MatrixIndexArray.enabled;
		break;
	case RC_MATRIX_PALETTE_OES:
		return m_CurrentStateFlag.etc.matrix_palette;
		break;
	case RC_MULTISAMPLE:
		return 0;
		break;
	case RC_NORMAL_ARRAY:
		return m_NormalArray.enabled;
	case RC_NORMALIZE:
	case RC_POINT_SIZE_ARRAY_OES:
	case RC_POINT_SMOOTH:
	case RC_POINT_SPRITE_OES:
	case RC_POLYGON_OFFSET_FILL:
	case RC_RESCALE_NORMAL:
	case RC_SAMPLE_ALPHA_TO_COVERAGE:
	case RC_SAMPLE_ALPHA_TO_ONE:
	case RC_SAMPLE_COVERAGE:
	case RC_SCISSOR_TEST:
	case RC_STENCIL_TEST:
		return 0;
		break;
	case RC_TEXTURE_2D:
		//return m_Current.pMaterial->IsTextureEnable();
		return m_Current.pMaterial->IsTextureMapEnable();
		break;
	case RC_TEXTURE_2D_NORMAL:
		return m_Current.pMaterial->IsNormalMapEnable();
		break;
	case RC_TEXTURE_COORD_ARRAY:
		return m_TexCoordArray.enabled;
		break;
	case RC_USE_COLOR_SHADOW:
		return m_CurrentStateFlag.option.color_shadow;
		break;
	case RC_USE_SHADOW:
		return m_CurrentStateFlag.option.shadow;
		break;
	case RC_USE_TEXTURE_ALPHA_SHADOW:
		return m_CurrentStateFlag.primitive.texture_alpha_shadow;
		break;
	case RC_USE_TEXTURE_ONLY:
		return m_CurrentStateFlag.primitive.texture_only;
		break;
	case RC_USE_TRANSMITTANCE_SHADOW:
		return m_CurrentStateFlag.primitive.transmittance_shadow;
		break;
	case RC_VERTEX_ARRAY:
		return m_VertexArray.enabled;
		break;
	case RC_WEIGHT_ARRAY_OES:
		return m_WeightArray.enabled;
		break;
	default:
		SetError(RC_INVALID_ENUM);
		break;
	}
	return 0;
}
