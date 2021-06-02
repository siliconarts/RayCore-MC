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
// Project : Dynalith system
// DATE    : 7/12/2013 Fri
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#include "IProveInterface.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include "ConfigINI.h"
#include "HDMITransmitter.h"

#pragma comment(lib, "iprove.lib")

iProveInterface		g_iProve;
#define WithCheck(x) if((iRet=(x))>0) { \
	fprintf(stderr, "Error: %s\n", iPROVE_ERROR_MESSAGE[iRet]); goto ERROR_OCCURED;}
char EIF[200];

#define SBMSIZE 1024
#define AXI
#ifdef AXI
#define EI ((0&0x1)<<31)
#define WR ((1&0x1)<<30)
#define LK ((0&0x1)<<29)
#define EX ((0&0x1)<<28)
#define SZ ((2&0x7)<<25)
#define BT ((1&0x3)<<23)
#define PR ((0&0x7)<<20)
//#define CA ((2&0xF)<<16)
#define CA ((0&0xF)<<16)
#define ID ((0&0xF)<<12)
#define READ_CMD (EI|LK|EX|SZ|BT|PR|CA|ID)
#define WRITE_CMD (EI|WR|LK|EX|SZ|BT|PR|CA|ID)
#endif

iProveInterface::iProveInterface(void) :
	m_bInitialized(FALSE)
{
	m_dwCardID	= 0;
}

iProveInterface::~iProveInterface(void){
	Release();
}

BOOL iProveInterface::IsInitialized(void){
	return m_bInitialized;
}

BOOL iProveInterface::Initialize(DWORD dwCardID){
	if(m_bInitialized && m_dwCardID!=dwCardID) Release();

	if(!m_bInitialized){
		m_dwCardID			= dwCardID;

		int	iRet;
		unsigned int freq_value;
		printf ("Configurating with EIF file...\n");
		WithCheck (iProveSetApiAttribute(SKIP_CONF_WHEN_DONE, 1));
#ifdef TRSIM
		WithCheck (iProveSetDeviceType(0));	// 1 for USB, 0 for PCI
#endif
		WithCheck (iProveLoadEmulationInfoFile(g_ConfigINI.GetString("iProve", "eif")));
		{

			iProveGetICSFreq(m_dwCardID, INEXT_V6_INTERNAL, INEXT_V6_ICS8430, &freq_value);	
			printf("ICS Freq_value = %dMhz \n", freq_value);
			Sleep(5);
		}

		if(freq_value!=400){
			printf("ICS8430 for iNEXT-V6, FPGA I0/I1 : 32 ~ 400Mh\n");
			iProveSetICSFreq(m_dwCardID, INEXT_V6_INTERNAL, INEXT_V6_ICS8430, 400);	
			Sleep(5);
			printf("Check 400Mh\n"); 
			{
				//// 설정 후 값 출력 
				iProveGetICSFreq(m_dwCardID, INEXT_V6_INTERNAL, INEXT_V6_ICS8430, &freq_value);	
				Sleep(5);
				printf("ICS Freq_value = %dMhz, Press Enter\n", freq_value); 
				getchar();
			}
		}


		{
			iProveGetICSFreq(m_dwCardID, INEXT_V6_EXTERNAL, INEXT_V6_ICS8430, &freq_value);	
			Sleep(5);
			printf("ICS Freq_value = %dMhz \n", freq_value);
		}

		if(freq_value!=400){
			printf("ICS8430 for iNEXT-V6, FPGA e0/e1 : 32 ~ 400Mh\n");
			iProveSetICSFreq(m_dwCardID, INEXT_V6_EXTERNAL, INEXT_V6_ICS8430, 400);	
			Sleep(5);
			printf("Check 400Mh\n"); 
			{
				//// 설정 후 값 출력 
				iProveGetICSFreq(m_dwCardID, INEXT_V6_EXTERNAL, INEXT_V6_ICS8430, &freq_value);	
				Sleep(5);
				printf("ICS Freq_value = %dMhz, Press Enter\n", freq_value); 
				getchar();
			}
		}

#ifdef BILA
		WithCheck (iProveAdvBILASetMemUsage(100));	// memry usage in percent
		WithCheck (iProveAdvBILAInit(1));	// 1 hw trigger enable, 0 disable
#endif

		WithCheck (iProveGetModuleHandle("AhbBfm", &m_iProveHandle));
		WithCheck (iProveAllocReadBuffer(m_iProveHandle, SBMSIZE));
		WithCheck (iProveAllocWriteBuffer(m_iProveHandle, SBMSIZE));

		WithCheck (iProveStart(m_dwCardID));
		if(!DDRInitialize()) goto ERROR_OCCURED;
		
		if(g_ConfigINI.GetInt("HDMI", "ENABLE", 0)){	// HDMI 초기화
			HDMITransmitter	hdmi;
			if(!hdmi.SetOutputScreen(g_ConfigINI.GetInt("HDMI", "WIDTH"), g_ConfigINI.GetInt("HDMI", "HEIGHT")))
				return 1;	// unsupported screen size
			hdmi.Initialize();
		}

		goto SUCCESS_DONE;

ERROR_OCCURED:
		return m_bInitialized;
SUCCESS_DONE:
		m_bInitialized		= TRUE;
	}
	return m_bInitialized;
}

void iProveInterface::Release(void){
	if(!m_bInitialized) return;
	
	int	iRet;
#ifdef BILA
# ifndef TRSIM
	printf( "Press 'Enter' to upload BILA data.\n" );
	getchar();
# endif
	WithCheck (iProveAdvBILAUpload());
#endif
	WithCheck (iProveStop(m_dwCardID));
	WithCheck (iProveCloseCard(m_dwCardID));

ERROR_OCCURED:
	m_bInitialized	= FALSE;
}

BOOL iProveInterface::DDRInitialize(void){
	/*DWORD buffer[16];
	DWORD temp;
	int count=0;

	g_iProve.ReadRegister(0x0, buffer, 4);
	temp=buffer[3];
	while((temp&0x2)==0){
		Sleep(1);
		count ++;
		printf("DDR Initialize Wait %d\n", count);
		g_iProve.ReadRegister(0x0, buffer, 4);
		temp=buffer[3];
		if(count==100){
			printf("DDR Initialize Error\n");
			return FALSE;
		}
	}*/
	Sleep(10);

	return TRUE;
}

void iProveInterface::WriteMemory(DWORD dwAddress, DWORD* pData, DWORD dwSize){
#ifdef AXI
	DWORD iCount	= dwSize>>2;
	{
		unsigned int cmd[2];
		cmd[1]	= (dwAddress<<2) + 0x80000000;	// memory

		while(iCount){
			DWORD	iTransCount	= (iCount >= 16) ? 16 : iCount;
			DWORD	iRetCount;
			cmd[0]	= WRITE_CMD | iTransCount;	// write
			iProveCmdWrite(m_iProveHandle, cmd, 2);
			do{
				iProveDataWrite(m_iProveHandle, pData, iTransCount, (unsigned int*)&iRetCount);
				pData		+= iRetCount;
				iTransCount	-= iRetCount;
				iCount		-= iRetCount;
			}while(iTransCount);
			cmd[1]	+= (16<<2);
		}
	}
#else
	DWORD iCount	= dwSize>>2;
	dwAddress		>>= 2;
	iCount			+= (dwAddress & 0xF);	// adjust byte alignment
	pData			-= (dwAddress & 0xF);	// adjust byte alignment
	iCount			= (iCount+0xF) >> 4;
	{
		DWORD cmd		= (1<<30)|(16<<23)|(dwAddress>>4);	// write memory command
		for(DWORD i=0;i<iCount;i++){
			DWORD tCount	= 16;
			unsigned int wcount;
			iProveCmdWrite(m_iProveHandle,&cmd,1);
			do{
				iProveDataWrite(m_iProveHandle, (void*)pData, tCount, &wcount);
				pData	+= wcount;
				tCount	-= wcount;
			}while(tCount);
			cmd++;
		}
	}
#endif
}

void iProveInterface::ReadMemory(DWORD dwAddress, DWORD *pData, DWORD dwSize){
#ifdef AXI
	DWORD iCount	= dwSize>>2;
	{
		unsigned int cmd[2];
		cmd[1]	= (dwAddress<<2) + 0x80000000;	// memory

		while(iCount){
			DWORD	iTransCount	= (iCount >= 16) ? 16 : iCount;
			DWORD	iRetCount;
			cmd[0]	= READ_CMD | iTransCount;	// read
			iProveCmdWrite(m_iProveHandle, cmd, 2);
			do{
				iProveDataWrite(m_iProveHandle, pData, iTransCount, (unsigned int*)&iRetCount);
				pData		+= iRetCount;
				iTransCount	-= iRetCount;
				iCount		-= iRetCount;
			}while(iTransCount);
			cmd[1]	+= (16<<2);
		}
	}
#else
	DWORD iCount	= dwSize>>2;
	dwAddress		>>= 2;
	iCount			+= (dwAddress & 0xF);	// adjust byte alignment
	pData			-= (dwAddress & 0xF);	// adjust byte alignment
	iCount			= (iCount+0xF) >> 4;

	{
		DWORD cmd		= (0<<30)|(16<<23)|(dwAddress>>4);	// write memory command
		for(DWORD i=0;i<iCount;i++){
			DWORD tCount	= 16;
			unsigned int wcount;
			iProveCmdWrite(m_iProveHandle,&cmd,1);
			do{
				iProveDataRead(m_iProveHandle, (void*)pData, tCount, &wcount);
				pData	+= wcount;
				tCount	-= wcount;
			}while(tCount);
			cmd++;
		}
	}
#endif
}

void iProveInterface::WriteRegister(DWORD Offset32, const DWORD* pData, int iCount){
#ifdef AXI
	{
		unsigned int cmd[2];
		cmd[1]	= (Offset32<<2);				// register

		while(iCount){
			DWORD	iTransCount	= (iCount >= 16) ? 16 : iCount;
			DWORD	iRetCount;
			cmd[0]	= WRITE_CMD | iTransCount;	// write
			iProveCmdWrite(m_iProveHandle, cmd, 2);
			do{
				iProveDataWrite(m_iProveHandle, (void*)pData, iTransCount, (unsigned int*)&iRetCount);
				pData		+= iRetCount;
				iTransCount	-= iRetCount;
				iCount		-= iRetCount;
			}while(iTransCount);
			cmd[1]	+= (16<<2);
		}
	}
#else
	{	// write command
		DWORD	cmd		= (1<<31)|(1<<30)|(iCount<<23)|Offset32;
		iProveCmdWrite(m_iProveHandle,&cmd,1);
	}
	{	// write data
		unsigned int wcount;
		do{
			iProveDataWrite(m_iProveHandle, (void*)pData, iCount, &wcount);
			pData	+= wcount;
			iCount	-= wcount;
		}while(iCount);
	}
#endif
}

void iProveInterface::ReadRegister(DWORD Offset32, DWORD* pData, int iCount){
#ifdef AXI
	{
		unsigned int cmd[2];
		cmd[1]	= (Offset32<<2);				// register

		while(iCount){
			DWORD	iTransCount	= (iCount >= 16) ? 16 : iCount;
			DWORD	iRetCount;
			cmd[0]	= READ_CMD | iTransCount;	// read
			iProveCmdWrite(m_iProveHandle, cmd, 2);
			do{
				iProveDataWrite(m_iProveHandle, (void*)pData, iTransCount, (unsigned int*)&iRetCount);
				pData		+= iRetCount;
				iTransCount	-= iRetCount;
				iCount		-= iRetCount;
			}while(iTransCount);
			cmd[1]	+= (16<<2);
		}
	}
#else
	{	// read command
		DWORD	cmd		= (1<<31)|(0<<30)|(iCount<<23)|Offset32;
		iProveCmdWrite(m_iProveHandle,&cmd,1);
	}
	{	// write data
		unsigned int wcount;
		do{
			iProveDataRead(m_iProveHandle, (void*)pData, iCount, &wcount);
			pData	+= wcount;
			iCount	-= wcount;
		}while(iCount);
	}
#endif
}

void iProveInterface::WriteRegister(DWORD Offset32, DWORD dwData){
	DWORD regs[2]	= {dwData, dwData};
	WriteRegister(Offset32, regs, 2);
}

DWORD iProveInterface::ReadRegister(DWORD Offset32){
	DWORD regs[2];
	ReadRegister(Offset32, regs, 2);
	return regs[0];
}