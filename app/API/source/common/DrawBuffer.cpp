// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore API v 1.0
// Date : 

#include "DrawBuffer.h"

using namespace GL;

DrawBuffer::DrawBuffer(void){
	m_pMemory		= NULL;
	m_iWidth		=
	m_iHeight		=
	m_iPixelBytes	=
	m_iSizzle		=
	m_iByteStride	= 0;
	m_Format		= BUFFER_FORMAT_NONE;
}

DrawBuffer::~DrawBuffer(void){
	Destroy();
}

BOOL DrawBuffer::Create(int iWidth, int iHeight, BUFFER_FORMAT format, int iByteStride){
	{	// buffer allocation
		int iPixelBytes, iLineBytes;
		switch(format){
		case BUFFER_FORMAT_A8:
		case BUFFER_FORMAT_L8:			iPixelBytes	= 1;	break;
		case BUFFER_FORMAT_R5G6B5:		iPixelBytes	= 2;	break;
		case BUFFER_FORMAT_A8B8G8R8:	iPixelBytes	= 4;	break;
		default:return FALSE;
		}

		iLineBytes	= iPixelBytes * iWidth;
		if(iLineBytes<iByteStride) iLineBytes	= iByteStride;
		// 메모리 크기가 다르면 다시 할당
		if(	m_iByteStride != iLineBytes || m_iHeight != iHeight){
			Destroy();
			m_iByteStride	= iLineBytes;
			m_iHeight		= iHeight;
			m_pMemory	= new Memory;
			m_pMemory->Create(iLineBytes * iHeight, 1<<13, TRUE);
		}
		m_iWidth		= iWidth;
		m_iPixelBytes	= iPixelBytes;
		m_Format		= format;
	}
	return TRUE;
}

void DrawBuffer::Destroy(void){
	if(m_pMemory){
		m_pMemory->Delete();
		m_pMemory		= NULL;
		m_iWidth		=
		m_iHeight		=
		m_iPixelBytes	=
		m_iSizzle		=
		m_iByteStride	= 0;
		m_Format		= BUFFER_FORMAT_NONE;
	}
}
