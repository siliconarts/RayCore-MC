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
// Project : DynalithSystem
// DATE    : 4/23/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#include "I2CInterface.h"
#include "iProveInterface.h"

//#define	MDIN_I2C_ADDR	(1<<15)
#define	MDIN_I2C_ADDR	(1<<14)
I2CInterface::I2CInterface(void) :
	m_SCL_T(TRUE),
	m_SCL_O(TRUE),
	m_SCL_I(TRUE),
	m_SDA_T(TRUE),
	m_SDA_O(TRUE),
	m_SDA_I(TRUE)
{
}

I2CInterface::~I2CInterface(void)
{
}

static void _I2C_Wait(double delay_time = 0.000005){	// 평균100kHz
	static BOOL		bInitialize = FALSE;
	static LONGLONG llQPFTicksPerSec  = 0;
	static LONGLONG	PrevTime;
	LONGLONG		CurTime;
	if(!bInitialize){
		QueryPerformanceFrequency((LARGE_INTEGER*)&llQPFTicksPerSec);
		bInitialize	= TRUE;
		QueryPerformanceCounter((LARGE_INTEGER*)&PrevTime);
	}
	do{
		QueryPerformanceCounter((LARGE_INTEGER*)&CurTime);
	}while(((double)(CurTime - PrevTime) / llQPFTicksPerSec) < delay_time);
	PrevTime	= CurTime;
}

void I2CInterface::Set(DWORD scl, DWORD sda){
	g_iProve.WriteRegister(MDIN_I2C_ADDR, (sda<<1)|scl);

	_I2C_Wait();	// usb 전송이 너무 느려서 필요치 않다.
	//Sleep(1);
}
void I2CInterface::Get(void){
	DWORD dwData = g_iProve.ReadRegister(MDIN_I2C_ADDR);
	m_SCL_I	= (dwData&0x2) != 0;
	m_SDA_I	= (dwData&0x8) != 0;
}

void I2CInterface::Start(void){
	// start
	Set(1,1);
	Set(1,0);
	Set(0,0);
}

void I2CInterface::Stop(void){
	// stop
	Set(0,0);
	Set(1,0);
	Set(1,1);
}

void I2CInterface::RepeatedStart(void){
	// repeated start
	Set(0,0);
	Set(0,1);
	Set(1,1);
	Set(1,0);
	Set(0,0);
}

BOOL I2CInterface::SendByte(BYTE data){
	BOOL bRet = FALSE;
	for(int t=0;t<8;t++){
		int bit = (data&0x80) ? 1:0;
		Set(0,bit);
		Set(1,bit);
		Set(0,bit);
		data<<=1;
	}
	// ack
	Set(0,1);
	Set(1,1);
	Get();
	bRet	= (m_SDA_I == FALSE);	// ack(0), nack(1)
	Set(0,1);

	return bRet;
}

BOOL I2CInterface::GetByte(BYTE& data, BOOL bLast){
	for(int t=0;t<8;t++){
		data<<=1;
		Set(0,1);
		Set(1,1);
		Get();
		data |= m_SDA_I;
		Set(0,1);
	}
	// ack
	Set(0,bLast);
	Set(1,bLast);
	Set(0,bLast);

	return TRUE;
}

BOOL I2CInterface::Write16(BYTE DevAddr, WORD InterAddr, WORD* pData, DWORD dwCount){
	// start
	Start();

	{	// send device
		//           dev_id       write
		if(!SendByte((DevAddr&0xFE)|(0))) goto ERROR_OCCURED;
	}
	{	// send address (16 bit(2byte))
		if(!SendByte((BYTE)(InterAddr>>8))) goto ERROR_OCCURED;
		if(!SendByte((BYTE)InterAddr)) goto ERROR_OCCURED;
	}
	
	// repeated start
	RepeatedStart();

	{	// send device
		//           dev_id       write
		if(!SendByte((DevAddr&0xFE)|(0))) goto ERROR_OCCURED;
	}

	{	// send data (16 bit pair)
		for(DWORD i=0;i<dwCount;i++){
			if(!SendByte((BYTE)(pData[i]>>8))) goto ERROR_OCCURED;
			if(!SendByte((BYTE)pData[i])) goto ERROR_OCCURED;
		}
	}

	// stop
	Stop();
	return TRUE;
ERROR_OCCURED:
	Stop();
	return FALSE;
}

BOOL I2CInterface::Read16(BYTE DevAddr, WORD InterAddr, WORD* pData, DWORD dwCount){
	// start
	Start();

	{	// send device
		//           dev_id       write
		if(!SendByte((DevAddr&0xFE)|(0))) goto ERROR_OCCURED;
	}
	{	// send address (16 bit(2byte))
		if(!SendByte((BYTE)(InterAddr>>8))) goto ERROR_OCCURED;
		if(!SendByte((BYTE)InterAddr)) goto ERROR_OCCURED;
	}
	
	// repeated start
	RepeatedStart();

	{	// send device
		//           dev_id       read
		if(!SendByte((DevAddr)|(1))) goto ERROR_OCCURED;
	}

	{	// read data (16 bit pair)
		for(DWORD i=0;i<dwCount;i++){
			BYTE byte_data;
			if(!GetByte(byte_data)) goto ERROR_OCCURED;
			pData[i] = ((WORD)byte_data)<<8;
			if(!GetByte(byte_data, i==(dwCount-1))) goto ERROR_OCCURED;
			pData[i] |= byte_data;
		}
	}

	// stop
	Stop();
	return TRUE;
ERROR_OCCURED:
	Stop();
	return FALSE;
}

BOOL I2CInterface::Write8(BYTE DevAddr, BYTE InterAddr, BYTE* pData, DWORD dwCount){
	// start
	Start();

	{	// send device
		//           dev_id       write
		if(!SendByte((DevAddr&0xFE)|(0))) goto ERROR_OCCURED;
	}
	{	// send address (8 bit(1byte))
		if(!SendByte(InterAddr)) goto ERROR_OCCURED;
	}
	
	{	// send data (8 bit)
		for(DWORD i=0;i<dwCount;i++){
			if(!SendByte((BYTE)pData[i])) goto ERROR_OCCURED;
		}
	}

	// stop
	Stop();
	return TRUE;
ERROR_OCCURED:
	Stop();
	return FALSE;
}

BOOL I2CInterface::Read8(BYTE DevAddr, BYTE InterAddr, BYTE* pData, DWORD dwCount){
	// start
	Start();

	{	// send device
		//           dev_id       write
		if(!SendByte((DevAddr&0xFE)|(0))) goto ERROR_OCCURED;
	}
	{	// send address (8 bit(1byte))
		if(!SendByte(InterAddr)) goto ERROR_OCCURED;
	}

	// repeated start
	RepeatedStart();

	{	// send device
		//           dev_id       read
		if(!SendByte((DevAddr&0xFE)|(1))) goto ERROR_OCCURED;
	}

	{	// read data (8 bit)
		for(DWORD i=0;i<dwCount;i++){
			BYTE byte_data;
			if(!GetByte(byte_data, i==(dwCount-1))) goto ERROR_OCCURED;
			pData[i] = byte_data;
		}
	}

	// stop
	Stop();
	return TRUE;
ERROR_OCCURED:
	Stop();
	return FALSE;
}