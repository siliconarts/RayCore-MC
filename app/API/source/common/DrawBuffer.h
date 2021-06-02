// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore API v 1.0
// Date : 

#ifndef __DRAW_BUFFER_H__
#define __DRAW_BUFFER_H__
#include "gl_Object.h"
#include "gl_Memory.h"

namespace GL{
	typedef enum{
		BUFFER_FORMAT_NONE,
		BUFFER_FORMAT_A8,
		BUFFER_FORMAT_L8,
		BUFFER_FORMAT_R5G6B5,
		BUFFER_FORMAT_A8B8G8R8,
	}BUFFER_FORMAT;

	class DrawBuffer : public ObjectT<DrawBuffer>
	{
	public:
		DrawBuffer(void);
		virtual ~DrawBuffer(void);

		BOOL Create(int iWidth, int iHeight, BUFFER_FORMAT format, int iByteStride = 0);
		void Destroy(void);

		inline Memory* GetMemory(void)		{return m_pMemory;}
		inline int Width(void)				{return m_iWidth;}
		inline int Height(void)				{return m_iHeight;}
		inline int ByteStride(void)			{return m_iByteStride;}
		inline int PixelBytes(void)			{return m_iPixelBytes;}
		inline BUFFER_FORMAT Format(void)	{return m_Format;}

		inline int Lock(void)				{return Mutex::Lock();}
		inline int UnLock(void)				{return Mutex::UnLock();}

	private:
		Memory*			m_pMemory;
		int				m_iWidth, m_iHeight, m_iByteStride, m_iPixelBytes, m_iSizzle;
		BUFFER_FORMAT	m_Format;
	};
};

#endif//__DRAW_BUFFER_H__
