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
// Project : DDK
// DATE    : 7/26/2013 Fri
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#include "rcDDK_Context.h"
#include "ddk/rcDDK_Dump.h"

#ifdef WIN32
	#include <io.h>
#else
	#include <unistd.h>
#endif
#include "zlib/zlib.h"

typedef struct{
	FILE*			fp;
	DUMP_HEADER		header;
	PFUNC_DUMPSYSTEM dump_func;
}DUMP_PRIVATE;

static void __enum_memory(IMemory* pMem, void* pPrivate){
	DUMP_PRIVATE*		pDump	= (DUMP_PRIVATE*)pPrivate;
	DUMP_DESCRIPTION	desc;
	memset(&desc, 0, sizeof(DUMP_DESCRIPTION));
	desc.bMemory		= TRUE;
	desc.dwAddress		= pMem->Physical();
	desc.dwSize			= pMem->ByteSize();

	if(pDump->dump_func)
		pDump->dump_func(desc.bMemory, desc.dwAddress, desc.dwSize, pMem->Virtual());

	{	// zlib 압축
		uLongf compSize	= (int)(desc.dwSize + (desc.dwSize + 12)*0.1f) + 32;
		BYTE* pCompBuff	= new BYTE[compSize];
		compress2(pCompBuff, &compSize, (const Bytef*)pMem->Virtual(), desc.dwSize, 5);
		desc.dwCompressedSize	= compSize;
		fwrite(&desc, sizeof(DUMP_DESCRIPTION), 1, pDump->fp);
		fwrite(pCompBuff, compSize, 1, pDump->fp);
		delete[] pCompBuff;
	}
// 	fwrite(&desc, sizeof(DUMP_DESCRIPTION), 1, pDump->fp);
// 	fwrite(pMem->Virtual(), pMem->ByteSize(), 1, pDump->fp);

	pDump->header.item_count++;
}

static void __enum_register(void* pReg, DWORD dwOffset, DWORD dwSize, void* pPrivate){
	DUMP_PRIVATE*		pDump	= (DUMP_PRIVATE*)pPrivate;
	DUMP_DESCRIPTION	desc;
	memset(&desc, 0, sizeof(DUMP_DESCRIPTION));
	desc.bMemory		= FALSE;
	desc.dwAddress		= dwOffset;
	desc.dwSize			= dwSize;

	if(pDump->dump_func)
		pDump->dump_func(desc.bMemory, desc.dwAddress, desc.dwSize, pReg);

	fwrite(&desc, sizeof(DUMP_DESCRIPTION), 1, pDump->fp);
	fwrite(pReg, dwSize, 1, pDump->fp);

	pDump->header.item_count++;
}

#define enum_register(reg, private_data)	__enum_register(reg.GetReg(), reg.GetOffset(), reg.GetSize(), &private_data)

BOOL rcDDKContext::DumpSystem(const char* file_name, PFUNC_DUMPSYSTEM dump_func){
	printf("DDK: System dumping to ");
	{
		DUMP_PRIVATE	dump;
		memset(&dump, 0, sizeof(DUMP_PRIVATE));
		
		dump.dump_func	= dump_func;
		{	// dump file 이름 정하기
			char dump_filename[256];
			if(file_name){
				strcpy(dump_filename, file_name);
			}else{
				for(int i=0;1;i++){
					sprintf(dump_filename, "scene_%03d.dump", i);
#ifdef WIN32
					if(_access(dump_filename, 04))
#else
					if(access(dump_filename, R_OK))
#endif
						break;
				}
			}
			printf("\"%s\" file in progress...", dump_filename);
#ifdef WIN32
			_flushall();
#endif
			dump.fp	= fopen(dump_filename, "wb");
		}

		if(dump.fp){	// dump 시도
			dump.header.magic_code	= 0xDEADDEEF;
			dump.header.version		= 0;
			{	// 프레임 버퍼 주소 해더에 저장 (편의를 위해)
				REG_CONFIG* pCfg	= m_Reg.Config.GetReg();
				REG_CONTROL* pCtrl	= m_Reg.Control.GetReg();
				dump.header.framebuffer.width		= pCtrl->resolution.width;
				dump.header.framebuffer.height		= pCtrl->resolution.height;
				dump.header.framebuffer.dwAddress	= pCfg->base.framebuffer_0;
			}

			fwrite(&dump.header, sizeof(DUMP_HEADER), 1, dump.fp);

			// memory dump
			MemoryEnumerate(__enum_memory, &dump);

			// register dump
			{
				enum_register(m_Reg.Config, dump);
				enum_register(m_Reg.Option, dump);
				enum_register(m_Reg.AST, dump);
				for(int i=0;i<RCDDK_MAX_LIGHT_COUNT;i++)
					enum_register(m_Reg.Light[i], dump);
				for(int i=0;i<RCDDK_MAX_MATERIAL_COUNT;i++)
					enum_register(m_Reg.Material[i], dump);
				enum_register(m_Reg.Control, dump);
			}

			// 해더 다시 기록
			fseek(dump.fp, 0, SEEK_SET);
			fwrite(&dump.header, sizeof(DUMP_HEADER), 1, dump.fp);

			fclose(dump.fp);

			printf("Done!\n");
#ifdef WIN32
			_flushall();
#endif
			return TRUE;
		}
	}
	printf("Failed!\n");
#ifdef WIN32
			_flushall();
#endif
	return FALSE;
}
RCDDK_API BOOL RCDDK_ENTRY LoadDumpScene(const char* file_name){
	rcDDKContext* pContext	= new rcDDKContext;
	
	printf("Loading dump scene file : %s\n", file_name);

	if(!pContext || !pContext->IsInitialized()){
		printf("*E: Can't load DDK context.\n");
	}else{
		if(!pContext->LoadDumpSystem(file_name)){
			printf("*E: Loading dump scene is failed!\n");
		}else{
			printf("\nLoading dump scene is successful.\n");
		}
	}
	SAFE_RELEASE(pContext);
	return TRUE;
}

BOOL rcDDKContext::LoadDumpSystem(const char* file_name){
	if(!file_name) return FALSE;
	DUMP_HEADER			header;
	DUMP_DESCRIPTION	desc;
	BOOL				bRet	= TRUE;
	{
		FILE*				fp = fopen(file_name, "rb");
		if(!fp) return FALSE;

		{	// 해더 읽기
			fread(&header, sizeof(DUMP_HEADER), 1, fp);
			if(header.magic_code != 0xDEADDEEF){
				printf("*E: Not a dump file.\n");
				bRet	= FALSE;
				goto END_LOAD;
			}
			// 해더 정보 출력
			printf("    version    = %d\n", header.version);
			printf("    Item count = %d\n", header.item_count);

			printf("framebuffer = %dx%d, addr:0x%08X\n", header.framebuffer.width, header.framebuffer.height, header.framebuffer.dwAddress);
			DisplaySetup(header.framebuffer.width, header.framebuffer.height);
		}

		// 각 아이템 읽어 시스템에 반영하기
		for(DWORD i=0;i<header.item_count;i++){
			BYTE* pData	= NULL;
			fread(&desc, sizeof(DUMP_DESCRIPTION), 1, fp);

			// 아이템 정보
			printf("Item (%d/%d) : %s, address = 0x%08X, size = %d bytes\n",
				i+1, header.item_count, 
				desc.bMemory ? "Memory" : "Register",
				desc.dwAddress, desc.dwSize);

			if(!desc.dwSize){
				printf("*E: Zero sized data!\n");
				bRet	= FALSE;
				goto END_LOAD;
			}
			
			{	// 덤프 파일로 부터 메모리/레지스터 읽기
				DWORD dwSize	= (desc.dwSize + ((1<<6)-1))&(~((1<<6)-1));
				pData = new BYTE[dwSize];
				if(desc.dwCompressedSize){
					// 압축된 데이터
					BYTE* pCompData		= new BYTE[desc.dwCompressedSize];
					uLongf decomp_size	= desc.dwSize;
					fread(pCompData, desc.dwCompressedSize, 1, fp);
					uncompress(pData, &decomp_size, pCompData, desc.dwCompressedSize);
					delete [] pCompData;
				}else{
					// 비압축 데이터
					fread(pData, desc.dwSize, 1, fp);
				}
			}

			{	// upload to system
				if(desc.bMemory){
					// 메모리인 경우
					if(!m_pSystem->WriteMemory(desc.dwAddress, pData, desc.dwSize)){
						printf("*E: Invalid memory write operation...\n");
						SAFE_DELETE_ARRAY(pData);
						bRet	= FALSE;
						goto END_LOAD;
					}
				}else{
					// 레지스터인 경우
					m_pSystem->Writes(desc.dwAddress>>2, (DWORD*)pData, desc.dwSize>>2);
				}
			}
			SAFE_DELETE_ARRAY(pData);
		}

END_LOAD:
		fclose(fp);
	}
#ifdef WIN32
	_flushall();
#endif
	if(bRet){	// 시스템 정상 완료
		{// 렌더링이 끝날 때까지 대기.
			DWORD buff[2];
			for(;;){
				m_pSystem->Reads((RC_REG_OFFSET_CONTROL+0x38)>>2, (DWORD*)buff, 2);
				if(buff[1]&0x4) break;
			}
		}
		
#ifdef USE_TESTDRIVE
		if(m_pTDMemory){	// TestDrive 에 쓰기
			DisplayConfig* pConfig				= (DisplayConfig*)m_pTDMemory->GetConfig();
			ITestDriverMemory* pSystemMemory	= TestDriver_GetMemory("RayCore");
			if(pSystemMemory){
				m_pSystem->ReadMemory(header.framebuffer.dwAddress, pSystemMemory->GetPointer(), sizeof(DWORD)*header.framebuffer.width*header.framebuffer.height);
				pConfig->Front.dwAddress	= 0;
				pConfig->Front.bUpdate		= TRUE;
			}
		}
#endif
	}

	return bRet;
}
