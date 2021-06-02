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
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#ifndef __ERROR_HANDLER_H__
#define __ERROR_HANDLER_H__

#if defined(DEBUG) || defined(_DEBUG)
	#define CATCH_ERROR_HANDLER		// API 내에서 오류를 검출하려면 이 정의를 사용한다.
#endif

// error handler
template <typename T, const T default_error_code>
class ErrorHandler
{
public:
	ErrorHandler(void){
		m_LastError		= default_error_code;	// initial no error
	}
	inline T GetError(void){
		T err = m_LastError;
		m_LastError		= default_error_code;	// set no error
		return err;
	}

#ifdef CATCH_ERROR_HANDLER
	#define SetError(err)	__SetError(err, __FUNCTION__)
	inline void __SetError(T err, const char* func){
		printf("Error occurred : %s(0x%X)", func, err);
#else
	inline void SetError(T err){
#endif
		if(m_LastError == default_error_code)
			m_LastError	= err;
	}

private:
	T		m_LastError;
};

#endif //__ERROR_HANDLER_H__
