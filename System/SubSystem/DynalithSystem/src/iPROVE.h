
/*********************************************************
 * iprove.h
 * Interface of the iPROVE from Dynalith Systems Co., Ltd.
 * Version 1.2
 * August 2002.
 *
 * Copyright (c) 2002 by Dynalith Systems Co., Ltd.
 * All right reserved.
 *
 * http://www.dynalith.com
 * info@dynalith.com
 *
 * Dynalith Systems
 * 373-1 Kusong-dong, Yusong-gu
 * 3rd floor of CHIPS, KAIST
 * Taejon 305-701, Korea
 *********************************************************/

#ifndef __IPROVE_API__
#define __IPROVE_API__

#if defined(OS_LINUX) || defined(OS_SOLARIS)

#  define IPROVEAPI_API 

#else

#  ifdef IPROVE_EXPORTS
#  define IPROVEAPI_API __declspec(dllexport)
#  else
#  ifdef TRSIM
#  define IPROVEAPI_API
#  else
#  define IPROVEAPI_API __declspec(dllimport)
#  endif
#  endif

#  ifdef IPROVE_STATIC
#  undef IPROVEAPI_API
#  define IPROVEAPI_API
#  endif

#endif


#ifdef __cplusplus
extern "C" {
#endif

/**** Type Define ****/
#ifndef __IPROVE_INT__
typedef struct _iPROVE_Module *iPROVE_Module;
typedef struct _iPROVE_Signal *iPROVE_Signal;
#endif

#define APIATTR_BASE 100
#define SLEEP_AFTER_ERASE_CONF		(APIATTR_BASE + 0)
#define SLEEP_AFTER_SEND_CONF		(APIATTR_BASE + 1)
#define ITER_FOR_CHECK_CONF_DONE	(APIATTR_BASE + 2)
#define CONF_BURST_SIZE			(APIATTR_BASE + 3)
#define SLEEP_BEFORE_ERASE_CONF		(APIATTR_BASE + 4)
#define SKIP_CONF_WHEN_DONE		(APIATTR_BASE + 5)
#define SKIP_USBHIU_HEADER		(APIATTR_BASE + 6)
#define FREQ_ID					(APIATTR_BASE + 7)
#define FREQ_VALUE				(APIATTR_BASE + 8)
#define FID_I0	(1<<1)
#define FID_I1	(1<<2)
#define FID_I2	(1<<3)
#define FID_I3	(1<<4)
#define FID_E0	(1<<5)
#define FID_E1	(1<<6)
#define FID_E2	(1<<7)
#define FID_E3	(1<<8)

// AMBA Transfer Type
#ifndef HBURST_SINGLE
#define HBURST_SINGLE   0  
#endif
#ifndef HBURST_INCR
#define HBURST_INCR 	1
#endif
#ifndef HBURST_WRAP4
#define HBURST_WRAP4    2
#endif
#ifndef HBURST_INCR4
#define HBURST_INCR4    3  
#endif
#ifndef HBURST_WRAP8
#define HBURST_WRAP8    4
#endif
#ifndef HBURST_INCR8
#define HBURST_INCR8    5
#endif
#ifndef HBURST_WRAP16
#define HBURST_WRAP16   6
#endif
#ifndef HBURST_INCR16
#define HBURST_INCR16   7
#endif
// AMBA Transfer size
#ifndef HSIZE_BYTE
#define HSIZE_BYTE	0
#endif
#ifndef HSIZE_HWORD
#define HSIZE_HWORD	1
#endif
#ifndef HSIZE_WORD
#define HSIZE_WORD	2
#endif

// Only for iNEXT-V6
#define INEXT_V6_ICS8430		0
#define INEXT_V6_ICS84312		1

#define INEXT_V6_EXTERNAL		0
#define INEXT_V6_INTERNAL		1

typedef enum{
	iproveClockToggleMode,
	iproveClockUpDownMode,
	iproveClockDownUpMode
} iproveClockOperationModeT;

/**** Common Control Functions ****/

// Integrated Initialization
// iProveOpenCard + iProveInitCard + iProveLoadModuleInfoFile + iProveLoadSignalInfoFile
IPROVEAPI_API int iProveLoadEmulationInfoFile(char* sEmulationInfoFileName);

// Call for each card
IPROVEAPI_API int iProveOpenCard(unsigned int uCardId);
IPROVEAPI_API int iProveInitCard(unsigned int uCardId, char *sFpgaFileName);
IPROVEAPI_API int iProveLoadModuleInfoFile(unsigned int uCardId, char *sInstanceTableFileName);
IPROVEAPI_API int iProveCloseCard(unsigned int uCardId);

// Reset card for iNEXT-PCIE
IPROVEAPI_API int iProveReset(unsigned int uCardId);

IPROVEAPI_API int iProveSetICSFreq(unsigned uCid, unsigned bIsInternal, unsigned uFreqID, unsigned uFreqValue);
IPROVEAPI_API int iProveGetICSFreq(unsigned uCid, unsigned bIsInternal, unsigned uFreqID, unsigned *uFreqValue);

IPROVEAPI_API int iProveSetDmaSize(int uCid, unsigned long size);
IPROVEAPI_API int iProveSetDmaStartAddress(int uCid, unsigned long address);

// Call for each module
IPROVEAPI_API int iProveGetModuleHandle(char *sModuleInstanceName, iPROVE_Module *pModule);

/**** Primary Functions ****/		
// Control function
IPROVEAPI_API int iProveAllocReadBuffer(iPROVE_Module hModule, int nLen);
IPROVEAPI_API int iProveAllocWriteBuffer(iPROVE_Module hModule, int nLen);
IPROVEAPI_API int iProveStart(unsigned int uCardId);
IPROVEAPI_API int iProveStop(unsigned int uCardId);

// Transfer function
IPROVEAPI_API int iProveDataRead(iPROVE_Module hModule, void* pBuffer, unsigned int nNumberOfItemsToRead, unsigned int* pNumberOfItemsRead);
IPROVEAPI_API int iProveDataWrite(iPROVE_Module hModule, void* pBuffer, unsigned int nNumberOfItemsToWrite, unsigned int* pNumberOfItemsWritten);
IPROVEAPI_API int iProveCmdWrite(iPROVE_Module hModule, void* pBuffer, unsigned int nNumberOfCmdsToWrite);

// Direct access to control area 
IPROVEAPI_API int iProveRegRead(iPROVE_Module hModule, int nAddr, int* pValue);
IPROVEAPI_API int iProveRegWrite(iPROVE_Module hModule, int nAddr, int* pValue);

/**** Abstract-Bus Transfer Functions ****/ 
IPROVEAPI_API int iProveAbstBusRead(iPROVE_Module hModule, unsigned int nAddr, int* pValue);
IPROVEAPI_API int iProveAbstBusWrite(iPROVE_Module hModule, unsigned int nAddr, int* pValue);

/**** Cycle-level Transfer Functions ****/						//Cycle-level only
// Control Function
IPROVEAPI_API int iProveCycLoadSignalInfoFile(iPROVE_Module hModule, char *sSignalInfoFileName);
IPROVEAPI_API int iProveCycGetSignalHandle(iPROVE_Module hModule, char* sSigName, iPROVE_Signal* pSignal);
IPROVEAPI_API int iProveCycGetClockDomain(iPROVE_Module hModule, char* sSigName, unsigned int* pDomainId);
IPROVEAPI_API int iProveCycQueryClockDomain(iPROVE_Module hModule, unsigned int uDomainId, unsigned int* pStatus);
IPROVEAPI_API int iProveCycSetClockDuty(char *sModuleInstanceName, char* sSigName, unsigned int uDuty);
//IPROVEAPI_API int iProveCycGetClockOperationMode(iPROVE_Module hModule, iproveClockOperationModeT *tClockOperationMode); //obsolete EIF v1.00
IPROVEAPI_API int iProveCycGetClockOperationMode(unsigned int uCardId, unsigned int uDomainId, iproveClockOperationModeT *tClockOperationMode);
//IPROVEAPI_API int iProveSetSDRAMClockFreq(unsigned int uCardId, unsigned int uFreqCode);

// Transfer function
IPROVEAPI_API int iProveCycSignalWrite(iPROVE_Module hModule, iPROVE_Signal hSignal, unsigned int* pValue);
IPROVEAPI_API int iProveCycSignalRead(iPROVE_Module hModule, iPROVE_Signal hSignal, unsigned int* pValue);
IPROVEAPI_API int iProveCycClockAdvance(unsigned int uCardId, unsigned int uDomainId);
IPROVEAPI_API int iProveCycClockUp(unsigned int uCardId, unsigned int uDomainId);
IPROVEAPI_API int iProveCycClockDown(unsigned int uCardId, unsigned int uDomainId);
IPROVEAPI_API int iProveCycClockUpDown(unsigned int uCardId, unsigned int uDomainId);
IPROVEAPI_API int iProveCycClockDownUp(unsigned int uCardId, unsigned int uDomainId);

// Transfer function(genClk version 2 only)
IPROVEAPI_API int iProveCycInitClock(unsigned int uCardId, unsigned int uDomainId, unsigned int uInitialValue, iproveClockOperationModeT uClockMode);
IPROVEAPI_API int iProveCycMultiClockAdvance(unsigned int uCardId, unsigned int uEncodedDomainId);

// Fine-control function
IPROVEAPI_API int iProveCycWritePort(unsigned int uCardId);
IPROVEAPI_API int iProveCycReadPort(unsigned int uCardId);
IPROVEAPI_API int iProveCycSetGenClkReg1(unsigned int cid, unsigned int uAllField);
IPROVEAPI_API int iProveCycClockHigh(unsigned int uCardId, unsigned int uDomainId);
IPROVEAPI_API int iProveCycClockLow(unsigned int uCardId, unsigned int uDomainId);
IPROVEAPI_API int iProveCycClockHighLow(unsigned int uCardId, unsigned int uDomainId);
IPROVEAPI_API int iProveCycClockLowHigh(unsigned int uCardId, unsigned int uDomainId);

// User friendly transfer function
IPROVEAPI_API int iProveCycSignalWriteByName(iPROVE_Module hModule, char* sSigName, unsigned int* pValue);
IPROVEAPI_API int iProveCycSignalReadByName(iPROVE_Module hModule, char* sSigName, unsigned int* pValue);

// Error Codes 
extern IPROVEAPI_API char * iPROVE_ERROR_MESSAGE[];

// Handle allocation and free
IPROVEAPI_API void* iProveAllocSignalHandle(void);
IPROVEAPI_API void* iProveAllocModuleHandle(void);
IPROVEAPI_API void iProveFreeHandle(void *hModuleSignal);
IPROVEAPI_API void iProveFree(void *hObject);

// For internal variable access
IPROVEAPI_API int iProveGetCardIdByModule(iPROVE_Module hModule, unsigned int* pCardId);
IPROVEAPI_API int iProveGetNumOfModule(unsigned int uCardId, int* pNumOfModule);
IPROVEAPI_API int iProveCycGetNumOfModuleWithCD(unsigned int uCardId, unsigned int uDomainId, unsigned int *pNumOfModule);
IPROVEAPI_API int iProveGetCardId(unsigned int* pEncodeduCardId);
IPROVEAPI_API int iProveGetDeviceType(unsigned int cid);
IPROVEAPI_API int iProveGetBoardVersion(unsigned int cid, unsigned int external, char *version);
IPROVEAPI_API int iProveGetAppCardInfo(unsigned int cid, unsigned uFpgaId, int *DevExist, int* DevFamiliy, int* DevCapacity, char* TargetMachineType, int* DevSpeed);

// For VCD dump
IPROVEAPI_API int iProveCycVcdDumpFile(char *sVcdFileName);
IPROVEAPI_API int iProveCycVcdDumpVars(unsigned int uNumOfModuleToDump, ...);
IPROVEAPI_API int iProveCycVcdDumpOn(void);
IPROVEAPI_API int iProveCycVcdDumpOff(void);

// iGnite iScanner
IPROVEAPI_API int iProveScanInit(char *input, char *output);
IPROVEAPI_API int iProveScanDump();
IPROVEAPI_API int iProveScanEnd();
IPROVEAPI_API int iProveScanOn();
IPROVEAPI_API int iProveScanOff();

IPROVEAPI_API int iProveScanInitMC(char *input, char *output, char* sTimeUnit);
IPROVEAPI_API int iProveScanClockNameInitMC(unsigned int uCid, unsigned int uDid, char *cClockName); 
IPROVEAPI_API int iProveScanClockValue(unsigned int uCid, unsigned int uDid, unsigned int uClockValuei, unsigned int uSimulationTime);
//IPROVEAPI_API int iProveScanClockModeInitMC (unsigned int uCid, unsigned int uDid, unsigned int uClockValue, iproveClockOperationModeT tClockOperationMode, unsigned int uSimulationTime); 
IPROVEAPI_API int iProveScanDumpMC(unsigned int uCid, unsigned int uEncodedDid, unsigned int simulationTime);

//Device driver share control
//IPROVEAPI_API int iProveSetShareMode(unsigned int uCardId, unsigned int uShareMode);
//IPROVEAPI_API int iProveGetShareMode(unsigned int uCardId, unsigned int* uShareMode);
IPROVEAPI_API int iProveUserLock(unsigned int uCardId, unsigned int uLockFree, unsigned int* uResult);

/**** API ATTRIBUTE DEFINE ****/
// Card Attribute for iProveSetCardAttribute
#define SHARE_KEY 1	//When modify this, you have to modify iPROVE_INT.h too
#define ATF_ENABLE 2
#define CONF_TARGET 3	//for only USB based system

IPROVEAPI_API int iProveSetCardAttribute(unsigned int uCardId, unsigned int uAttributeType, unsigned int uValue);
IPROVEAPI_API int iProveSetApiAttribute(unsigned int uAttributeType, unsigned int uValue);

IPROVEAPI_API int iProveSetErrPrintCb(void (*fp)(char *msg));
IPROVEAPI_API int iProveSetInfoPrintCb(void (*fp)(char *msg));
IPROVEAPI_API int iProveSetConfStatusCb(void (*fp)(float progress));

IPROVEAPI_API int iProveBlockedDataRead(iPROVE_Module hModule, unsigned int* pBuffer, unsigned int NumberOfItemsToRead);
IPROVEAPI_API int iProveBlockedDataWrite(iPROVE_Module hModule, unsigned int* pBuffer, unsigned int NumberOfItemsToWrite);

/**** API for iCON/iVORY/iNTUITION (USB) ****/
IPROVEAPI_API int iProveIntGenericI2CRead(unsigned int uCardId, unsigned char i2c_addr, unsigned int sizeInByte, unsigned char *buffer);
IPROVEAPI_API int iProveIntGenericI2CWrite(unsigned int uCardId, unsigned char i2c_addr, unsigned int sizeInByte, unsigned char *buffer);
IPROVEAPI_API int iConIntReadOEC(unsigned int uCardId, unsigned char *value);
IPROVEAPI_API int iConIntWriteOEC(unsigned int uCardId, unsigned char value);
IPROVEAPI_API int iConIntReadIOC(unsigned int uCardId, unsigned char *value);
IPROVEAPI_API int iConIntWriteIOC(unsigned int uCardId, unsigned char value);
IPROVEAPI_API int iNtuitionCoreTileDetect(unsigned int uCid, unsigned int* uDetected);

IPROVEAPI_API int iProveAdvBILASWTrigger();
IPROVEAPI_API int iProveAdvBILAUpload();
IPROVEAPI_API int iProveAdvBILAGetModuleHandle(void);
IPROVEAPI_API int iProveAdvBILAInit(unsigned int hwtrigena);
IPROVEAPI_API int iProveAdvBILASetInfoFile(char *);
IPROVEAPI_API int iProveAdvBILASetTrgFile(char *);
IPROVEAPI_API int iProveAdvBILASetVCDFile(char *);
IPROVEAPI_API int iProveAdvBILASetMemUsage(int percent);

IPROVEAPI_API unsigned iProveAhbBfmWrite(
	iPROVE_Module mh_AhbBfm,
	unsigned int uAddress, unsigned int* pBuffer, 
	unsigned int uNumberOfItemsToWrite, unsigned int uBurstMode, 
	unsigned int uTransferSize, unsigned int uProt, unsigned int uLock );

IPROVEAPI_API unsigned iProveAhbBfmRead(
	iPROVE_Module mh_AhbBfm,
	unsigned int uAddress, unsigned int* pBuffer, 
	unsigned int uNumberOfItemsToRead, unsigned int uBurstMode, 
	unsigned int uTransferSize, unsigned int uProt, unsigned int uLock);

IPROVEAPI_API unsigned iProveAhbBfmWriteSingle(
	iPROVE_Module mh_AhbBfm,
	unsigned int uAddress, unsigned int wdata );

IPROVEAPI_API unsigned iProveAhbBfmReadSingle(
	iPROVE_Module mh_AhbBfm,
	unsigned int uAddress );

/***** iNTUITION ****/
//IPROVEAPI_API int iNtuitionIntSetConfMode(unsigned int uCardId);

/***** transim ****/
IPROVEAPI_API int iProveSetDeviceType(int deviceType);

/***** EoIP ****/
IPROVEAPI_API int iProveSetServerAddress(char *address);
#ifdef __cplusplus
}
#endif

#endif //__IPROVE_API__
