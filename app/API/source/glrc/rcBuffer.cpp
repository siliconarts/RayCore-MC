// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore API v 1.0
// Date : 

#include "rcContext1.h"

void ContextRC1::ResetVertexArray (VERTEX_ARRAY* pVertexArray){
	pVertexArray->pData = NULL;
	pVertexArray->boundBuffer = 0;
	pVertexArray->size = 0;
	pVertexArray->stride = 0;
	pVertexArray->type = 0;
}

void ContextRC1::ReleaseVertexArrays (RCuint buffer){
	// 모든 VertexArray(Vertex, Normal, TexCoord, Color, MatrixIndex, Weight)에 대해서
	// Buffer Object가 연결되어 있는 경우 관련 정보를 모두 초기화해야 한다.
	if(buffer == 0) return;	

	if(m_VertexArray.boundBuffer == buffer)			ResetVertexArray(&m_VertexArray);
	if(m_NormalArray.boundBuffer == buffer)			ResetVertexArray(&m_NormalArray);
	if(m_ColorArray.boundBuffer == buffer)			ResetVertexArray(&m_ColorArray);
	if(m_TexCoordArray.boundBuffer == buffer)		ResetVertexArray(&m_TexCoordArray);
	if(m_MatrixIndexArray.boundBuffer == buffer)	ResetVertexArray(&m_MatrixIndexArray);
	if(m_WeightArray.boundBuffer == buffer)			ResetVertexArray(&m_WeightArray);
}

void ContextRC1::BindBuffer (RCenum target, RCuint buffer){
	if(target != RC_ARRAY_BUFFER && target != RC_ELEMENT_ARRAY_BUFFER){
		SetError(RC_INVALID_ENUM);
		return;
	}

	if(buffer == 0){
		switch (target){
		case RC_ARRAY_BUFFER:
			if(m_Current.pBuffer)	ReleaseVertexArrays(m_Current.pBuffer->Name());
			m_Current.pBuffer = NULL;
			break;
		case RC_ELEMENT_ARRAY_BUFFER:
			if(m_Current.pElementBuffer)	ReleaseVertexArrays(m_Current.pElementBuffer->Name());
			m_Current.pElementBuffer = NULL;
			break;
		}
		return;
	}

	rcBuffer* pBuffer	= m_Container.Buffer.Find(buffer);	// first, find!
	if(!pBuffer){	// if buffer is not found, we must create one.
		pBuffer = m_Container.Buffer.Create(buffer);
		if(!pBuffer){
			SetError(RC_OUT_OF_MEMORY);			
		}
	}

	// set to current buffer
	if(pBuffer){
		pBuffer->Set(target);

		switch (target){
		case RC_ARRAY_BUFFER:
			m_Current.pBuffer = pBuffer;
			break;
		case RC_ELEMENT_ARRAY_BUFFER:
			m_Current.pElementBuffer = pBuffer;
			break;
		}
	}
}

void ContextRC1::BufferData (RCenum target, RCsizeiptr size, const RCvoid *data, RCenum usage){
	rcBuffer* pBuffer=NULL;

	if(target != RC_ARRAY_BUFFER && target != RC_ELEMENT_ARRAY_BUFFER){
		SetError(RC_INVALID_ENUM);
		return;
	}

	if(usage != RC_STATIC_DRAW && usage != RC_DYNAMIC_DRAW){
		SetError(RC_INVALID_ENUM);
		return;
	}

	if(size <= 0 || data == NULL){
		SetError(RC_INVALID_VALUE);
		return;
	}

	switch (target){
	case RC_ARRAY_BUFFER:
		pBuffer = m_Current.pBuffer;
		break;
	case RC_ELEMENT_ARRAY_BUFFER:
		pBuffer = m_Current.pElementBuffer;
		break;
	}

	if(!pBuffer){
		SetError(RC_INVALID_OPERATION);
		return;
	}

	ReleaseVertexArrays(pBuffer->Name());
	pBuffer->BufferData(target, size, data, usage);
	// @todo : 새로운 BufferData로 연결 정보를 다시 설정해 주어야 할까?
}

void ContextRC1::BufferSubData (RCenum target, RCintptr offset, RCsizeiptr size, const RCvoid *data){
	rcBuffer* pBuffer=NULL;

	if(target != RC_ARRAY_BUFFER && target != RC_ELEMENT_ARRAY_BUFFER){
		SetError(RC_INVALID_ENUM);
		return;
	}

	if(offset < 0 || size <= 0 || data == NULL){
		SetError(RC_INVALID_VALUE);
		return;
	}
	
	switch (target){
	case RC_ARRAY_BUFFER:
		pBuffer = m_Current.pBuffer;
		break;
	case RC_ELEMENT_ARRAY_BUFFER:
		pBuffer = m_Current.pElementBuffer;
		break;
	}

	if(!pBuffer 
		|| pBuffer->Get()->usage != RC_DYNAMIC_DRAW 
		|| !pBuffer->Get()->flag 
		|| !pBuffer->Get()->pValue){
		SetError(RC_INVALID_OPERATION);
		return;
	}

	if(pBuffer->Get()->size < (DWORD)(offset+size)){
		SetError(RC_INVALID_VALUE);
		return;
	}

	pBuffer->BufferSubData(target, offset, size, data);
}

void ContextRC1::DeleteBuffers (RCsizei n, const RCuint *buffers){
	rcBuffer* pBuffer=NULL;

	if(n<0) SetError(RC_INVALID_VALUE);
	else if(buffers){
		for(int i=0;i<n;i++){
			pBuffer = m_Container.Buffer.Find(buffers[i]);
			// 오브젝트가 존재하면서 지워지지 않았어야 하며, default object가 아니여야 합니다.
			if(pBuffer && !pBuffer->IsDeleted() && pBuffer->Name()){
				if(m_Current.pBuffer == pBuffer)
					m_Current.pBuffer = 0;
				else if(m_Current.pElementBuffer == pBuffer)
					m_Current.pElementBuffer = 0;
				ReleaseVertexArrays(pBuffer->Name());
				pBuffer->Delete();
			}
		}
	}
}

void ContextRC1::GenBuffers (RCsizei n, RCuint *buffers){
	rcBuffer* pBuffer=NULL;

	if(n<0) SetError(RC_INVALID_VALUE);
	else if(buffers){
		for(int i=0;i<n;i++){
			pBuffer = m_Container.Buffer.Create();
			if(!pBuffer){
				SetError(RC_OUT_OF_MEMORY);
				break;
			}
			buffers[i] = pBuffer->Name();
		}
	}
}

rcBuffer::rcBuffer(void){
	memset(&m_Status, 0, sizeof(BUFFER_OBJECT));
	m_Status.usage = RC_STATIC_DRAW;
	m_Status.target = RC_ARRAY_BUFFER;
	m_Status.pValue = NULL;

	m_bChanged	= TRUE;
}

rcBuffer::~rcBuffer(void){
	if(m_Status.pValue)
		free(m_Status.pValue);
	m_Status.pValue = NULL;
}

BOOL rcBuffer::Set(RCenum target){
	switch (target){
	case RC_ARRAY_BUFFER:
	case RC_ELEMENT_ARRAY_BUFFER:
		m_Status.target = target;
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

BOOL rcBuffer::BufferData (RCenum target, RCsizeiptr size, const RCvoid *data, RCenum usage){
	switch (target){
	case RC_ARRAY_BUFFER:
	case RC_ELEMENT_ARRAY_BUFFER:
		{
			// @todo : Memory 관리가 필요할까?
			m_Status.flag = TRUE;
			if(m_Status.pValue)
				free(m_Status.pValue);
			m_Status.pValue = NULL;

			m_Status.usage = usage;			
			m_Status.size = size;
			if(size > 0){
				m_Status.pValue = malloc(size);
				if(data != NULL)	
					memcpy(m_Status.pValue, data, size);
			}
		}
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

BOOL rcBuffer::BufferSubData (RCenum target, RCintptr offset, RCsizeiptr size, const RCvoid *data){
	void* pData=NULL;

	switch (target){
	case RC_ARRAY_BUFFER:
	case RC_ELEMENT_ARRAY_BUFFER:
		{
			if(offset < 0)							return FALSE;
			if(size <= 0)							return FALSE;
			if(m_Status.usage != RC_DYNAMIC_DRAW)	return FALSE;
			if(!m_Status.flag || !m_Status.pValue)	return FALSE;
			if(m_Status.size < (DWORD)(offset+size))	return FALSE;
			
			pData = (void *) ((unsigned char *)m_Status.pValue + offset);
			memcpy(pData, data, size);
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}