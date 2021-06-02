// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore API v 1.0
// Date : 

#include "rcContext1.h"

void ContextRC1::DrawElements (RCenum mode, RCsizei count, RCenum type, const RCvoid *indices){
	const void*		pData=NULL;
	const BYTE*		ptrb;
	const WORD*		ptrs;
	const DWORD*	ptri;

	switch(mode){
	//case RC_POINTS:		// HW에서 지원하지 않음
	//case RC_LINE_STRIP:	// HW에서 지원하지 않음
	//case RC_LINE_LOOP:	// HW에서 지원하지 않음
	//case RC_LINES:		// HW에서 지원하지 않음
	case RC_TRIANGLES:
	case RC_TRIANGLE_STRIP:
	case RC_TRIANGLE_FAN:
	case RC_QUADS:
		break;
	default:
		SetError(RC_INVALID_ENUM);
		break;
	}

	if (count < 0) {
		SetError(RC_INVALID_VALUE);
		return;
	}

	if (!m_VertexArray.enabled) {
		return;
	}

	
	if (!m_CurrentStateFlag.etc.matrix_modelview_inverse) {
		if (InverseMatrix(m_MatrixModelToWorld.m, m_MatrixModelToWorldInverse.m))
			m_CurrentStateFlag.etc.matrix_modelview_inverse = 1;
	}

	if(m_Current.pElementBuffer){
		if(!m_Current.pElementBuffer->Get()->pValue){
			SetError(RC_INVALID_OPERATION);
			return;
		}

		if(m_Current.pElementBuffer->Get()->size < *(unsigned int *)&pData){
			SetError(RC_INVALID_VALUE);
			return;
		}

		pData = (void *) ((unsigned char *)m_Current.pElementBuffer->Get()->pValue + *(unsigned int *)&indices);
	} else {
		pData = indices;
	}

	if(!pData){
		SetError(RC_INVALID_OPERATION);
		return;
	}

	switch(type){
	case RC_UNSIGNED_BYTE:
	case RC_BYTE:
		{
			ptrb = (BYTE *)(pData);

			Begin(mode);
			while (count-- > 0) {
				(this->*m_pfnDrawPrimitive)(*ptrb++);
			}
			End();
		} 
		break;
	case RC_UNSIGNED_SHORT:
	case RC_SHORT:
		{
			ptrs = (WORD *)(pData);

			Begin(mode);
			while (count-- > 0) {
				(this->*m_pfnDrawPrimitive)(*ptrs++);
			}

			End();
		}
		break;
	case RC_UNSIGNED_INT:
	case RC_INT:
		{
			ptri = (DWORD *)(pData);

			Begin(mode);
			while (count-- > 0) {
				(this->*m_pfnDrawPrimitive)(*ptri++);
			}
			End();
		}
		break;
	default:
		SetError(RC_INVALID_ENUM);
		break;
	}
}

void ContextRC1::DrawArrays (RCenum mode, RCint first, RCsizei count){
	RCsizei ptr = first;

	switch(mode){
	//case RC_POINTS:		// HW에서 지원하지 않음
	//case RC_LINE_STRIP:	// HW에서 지원하지 않음
	//case RC_LINE_LOOP:	// HW에서 지원하지 않음
	//case RC_LINES:		// HW에서 지원하지 않음
	case RC_TRIANGLES:
	case RC_TRIANGLE_STRIP:
	case RC_TRIANGLE_FAN:
	case RC_QUADS:
		break;
	default:
		SetError(RC_INVALID_ENUM);
		break;
	}

	if (count < 0) {
		SetError(RC_INVALID_VALUE);
		return;
	}

	if (!m_VertexArray.enabled) {
		return;
	}

	if (!m_CurrentStateFlag.etc.matrix_modelview_inverse) {
		if (InverseMatrix(m_MatrixModelToWorld.m, m_MatrixModelToWorldInverse.m))
			m_CurrentStateFlag.etc.matrix_modelview_inverse = 1;
	}

	Begin(mode);
	while (count-- > 0) {
		(this->*m_pfnDrawPrimitive)(ptr);
		ptr++;
	}
	End();

}

void ContextRC1::VertexPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer){
	
	if (type != RC_BYTE && type != RC_SHORT && type != RC_FIXED && type != RC_FLOAT) {
		SetError(RC_INVALID_ENUM);
		return;
	}

	//Vertex의 size는 무조건 3이어야 한다. 그렇지 않으면, Tree building에서 오류가 발생한다!
	//if (size < 2 || size > 4) {
	if (size != 3) {
		SetError(RC_INVALID_VALUE);
		return;
	}

	if (stride < 0) {
		SetError(RC_INVALID_VALUE);
		return;
	} else if (stride == 0) {
		switch (type) {
		case RC_BYTE:
			stride = size * sizeof (RCbyte);
			break;

		case RC_SHORT:
			stride = size * sizeof (RCshort);
			break;

		case RC_FIXED:
			stride = size * sizeof (RCfixed);
			break;

		case RC_FLOAT:
			stride = size * sizeof (RCfloat);
			break;
		}
	}

	m_VertexArray.size = size;	
	m_VertexArray.type = type;
	m_VertexArray.stride = stride;

	m_VertexArray.pData = NULL;
	m_VertexArray.boundBuffer = 0;
	if(m_Current.pBuffer){
		if(!m_Current.pBuffer->Get()->pValue){
			SetError(RC_INVALID_OPERATION);
			return;
		}

		if(m_Current.pBuffer->Get()->size <= *(unsigned int *)&pointer){
			SetError(RC_INVALID_VALUE);
			return;
		}

		m_VertexArray.boundBuffer = m_Current.pBuffer->Name();
		m_VertexArray.pData    = (void *) ((unsigned char *)m_Current.pBuffer->Get()->pValue + *(unsigned int *)&pointer);		
	} else {
		m_VertexArray.pData = pointer;
	}
}

void ContextRC1::NormalPointer (RCenum type, RCsizei stride, const RCvoid *pointer){
	RCint size;
	size = 3;
	if (type != RC_BYTE && type != RC_SHORT && type != RC_FIXED && type != RC_FLOAT) {
		SetError(RC_INVALID_ENUM);
		return;
	}

	if (stride < 0) {
		SetError(RC_INVALID_VALUE);
		return;
	} else if (stride == 0) {
		switch (type) {
		case RC_BYTE:
			stride = size * sizeof (RCbyte);
			break;

		case RC_SHORT:
			stride = size * sizeof (RCshort);
			break;

		case RC_FIXED:
			stride = size * sizeof (RCfixed);
			break;

		case RC_FLOAT:
			stride = size * sizeof (RCfloat);
			break;
		}
	} 

	m_NormalArray.size = size;
	m_NormalArray.type = type;
	m_NormalArray.stride = stride;

	m_NormalArray.pData = NULL;
	m_NormalArray.boundBuffer = 0;
	if(m_Current.pBuffer){
		if(!m_Current.pBuffer->Get()->pValue){
			SetError(RC_INVALID_OPERATION);
			return;
		}

		if(m_Current.pBuffer->Get()->size <= *(unsigned int *)&pointer){
			SetError(RC_INVALID_VALUE);
			return;
		}

		m_NormalArray.boundBuffer = m_Current.pBuffer->Name();
		m_NormalArray.pData    = (void *) ((unsigned char *)m_Current.pBuffer->Get()->pValue + *(unsigned int *)&pointer);		
	} else {
		m_NormalArray.pData = pointer;
	}
}

void ContextRC1::TexCoordPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer){
	
	if (type != RC_BYTE && type != RC_SHORT && type != RC_FIXED && type != RC_FLOAT) {
		SetError(RC_INVALID_ENUM);
		return;
	}

	//TexCoord의 size는 무조건 2이어야 한다. 그렇지 않으면, Tree building에서 오류가 발생한다!
	//if (size < 2 || size > 4) {
	if (size != 2) {
		SetError(RC_INVALID_VALUE);
		return;
	}

	if (stride < 0) {
		SetError(RC_INVALID_VALUE);
		return;
	} else if (stride == 0) {
		switch (type) {
		case RC_BYTE:
			stride = size * sizeof (RCbyte);
			break;

		case RC_SHORT:
			stride = size * sizeof (RCshort);
			break;

		case RC_FIXED:
			stride = size * sizeof (RCfixed);
			break;

		case RC_FLOAT:
			stride = size * sizeof (RCfloat);
			break;
		}
	} 

	m_TexCoordArray.size = size;	
	m_TexCoordArray.type = type;
	m_TexCoordArray.stride = stride;

	m_TexCoordArray.pData = NULL;	
	m_TexCoordArray.boundBuffer = 0;
	if(m_Current.pBuffer){
		if(!m_Current.pBuffer->Get()->pValue){
			SetError(RC_INVALID_OPERATION);
			return;
		}

		if(m_Current.pBuffer->Get()->size <= *(unsigned int *)&pointer){
			SetError(RC_INVALID_VALUE);
			return;
		}

		m_TexCoordArray.boundBuffer = m_Current.pBuffer->Name();
		m_TexCoordArray.pData    = (void *) ((unsigned char *)m_Current.pBuffer->Get()->pValue + *(unsigned int *)&pointer);		
	} else {
		m_TexCoordArray.pData = pointer;
	}
}

// @todo : Vertex Color를 지원하지 않으므로, 추후 정리가 필요함
void ContextRC1::ColorPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer){
	
	if (type != RC_BYTE && type != RC_UNSIGNED_BYTE
		&& type != RC_SHORT && type != RC_UNSIGNED_SHORT
		&& type != RC_FIXED && type != RC_FLOAT) {
		SetError(RC_INVALID_ENUM);
		return;
	}

	if (size != 4) {
		SetError(RC_INVALID_VALUE);
		return;
	}

	if (stride < 0) {
		SetError(RC_INVALID_VALUE);
		return;
	} else if (stride == 0) {
		switch (type) {
		case RC_BYTE:
		case RC_UNSIGNED_BYTE:
			stride = sizeof (RCbyte) * size;
			break;
		case RC_SHORT:
		case RC_UNSIGNED_SHORT:
			stride = sizeof (RCshort) * size;
			break;
		case RC_FIXED:
			stride = sizeof (RCfixed) * size;
			break;
		case RC_FLOAT:
			stride = sizeof (RCfloat) * size;
			break;
		}
	}

	m_ColorArray.size = size;	
	m_ColorArray.type = type;
	m_ColorArray.stride = stride;

	m_ColorArray.pData = NULL;
	m_ColorArray.boundBuffer = 0;
	if(m_Current.pBuffer){
		if(!m_Current.pBuffer->Get()->pValue){
			SetError(RC_INVALID_OPERATION);
			return;
		}

		if(m_Current.pBuffer->Get()->size <= *(unsigned int *)&pointer){
			SetError(RC_INVALID_VALUE);
			return;
		}

		m_ColorArray.boundBuffer = m_Current.pBuffer->Name();
		m_ColorArray.pData    = (void *) ((unsigned char *)m_Current.pBuffer->Get()->pValue + *(unsigned int *)&pointer);		
	} else {
		m_ColorArray.pData = pointer;
	}
}

void ContextRC1::DisableClientState (RCenum array){
	switch(array) {
	case RC_COLOR_ARRAY:
		m_ColorArray.enabled = FALSE;
		break;
	case RC_MATRIX_INDEX_ARRAY_OES:
		m_MatrixIndexArray.enabled = FALSE;
		break;
	case RC_NORMAL_ARRAY:
		m_NormalArray.enabled = FALSE;
		break;
	case RC_POINT_SIZE_ARRAY_OES:	//rcPointSize : 지원하지 않음
		break;
	case RC_TEXTURE_COORD_ARRAY:
		m_TexCoordArray.enabled = FALSE;
		break;
	case RC_VERTEX_ARRAY:
		m_VertexArray.enabled = FALSE;
		break;
	case RC_WEIGHT_ARRAY_OES:
		m_WeightArray.enabled = FALSE;
		break;
	default:
		SetError(RC_INVALID_ENUM);
		break;
	}
}

void ContextRC1::EnableClientState (RCenum array){
	switch(array) {
	case RC_COLOR_ARRAY:
		m_ColorArray.enabled = TRUE;
		break;
	case RC_MATRIX_INDEX_ARRAY_OES:
		m_MatrixIndexArray.enabled = TRUE;
		break;
	case RC_NORMAL_ARRAY:
		m_NormalArray.enabled = TRUE;
		break;
	case RC_POINT_SIZE_ARRAY_OES:	//rcPointSize : 지원하지 않음
		break;
	case RC_TEXTURE_COORD_ARRAY:
		m_TexCoordArray.enabled = TRUE;
		break;
	case RC_VERTEX_ARRAY:
		m_VertexArray.enabled = TRUE;
		break;
	case RC_WEIGHT_ARRAY_OES:
		m_WeightArray.enabled = TRUE;
		break;
	default:
		SetError(RC_INVALID_ENUM);
		break;
	}
}
