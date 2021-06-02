#pragma once
#include <windows.h>
#include "I2CInterface.h"
#include "mdintype.h"
#include "mdinapi.h"

class HDMITransmitter
{
public:
	HDMITransmitter(void);
	~HDMITransmitter(void);

	BOOL SetOutputScreen(int iWidth, int iHeight);
	BOOL Initialize(void);
	
	WORD MDINAPI_GetDeviceID(void);
	BOOL MDINAPI_SetMemory(BYTE endian, BYTE drive, BYTE delay = 0);
	BOOL MDINAPI_DisplayOutWindow(BOOL OnOff);
	BOOL MDINAPI_SoftReset(void);
	BOOL MDINAPI_SetVCLKPLLSource(MDIN_PLL_SOURCE_t src);
	BOOL MDINAPI_EnableClockDrive(MDIN_CLK_DRV_t clock, BOOL OnOff);
	BOOL MDINAPI_SetInDataMapMode(MDIN_IN_DATA_MAP_t mode);
	BOOL MDINAPI_LoadDeintData(void);
	BOOL MDINAPI_SetDeintYMode(MDIN_DEINT_MODE_t mode);
	BOOL MDINAPI_SetDeintCMode(MDIN_DEINT_MODE_t mode);
	BOOL MDINAPI_EnableDeintHDMode(BOOL bOn);
	BOOL MDINAPI_EnableDeintV2TAB(BOOL OnOff);
	BOOL MDINAPI_EnableDeintHD3DAdp(BOOL OnOff);
	BOOL MDINAPI_SetDeintFilmMode(MDIN_DEINT_FILM_t mode);
	BOOL MDINAPI_SetDeintCurMotionNRSel(BYTE mode);
	BOOL MDINAPI_SetDeintCurMotionThres(BYTE level);
	BOOL MDINAPI_SetDeintPDCurMotionThres(BYTE level);
	BOOL MDINAPI_EnableDeintCurMotionNRUse(BOOL bOn);
	BOOL MDINAPI_EnableNextMotionNRUse(BOOL bOn);
	BOOL MDINAPI_EnableDeintFastMotion(BOOL OnOff);
	BOOL MDINAPI_EnableDeintFastHistoryUse(BOOL bOn);
	BOOL MDINAPI_EnableDeintBorder(BOOL OnOff);
	BOOL MDINAPI_EnableDeintBorderNRUse(BOOL bOn);
	BOOL MDINAPI_EnableDeintSlowMotionNRUse(BOOL bOn);
	BOOL MDINAPI_EnableDeintStillMotionNRUse(BOOL bOn);
	BOOL MDINAPI_EnableDeintNR(BOOL bOn);
	BOOL MDINAPI_EnableDeintCCS(BOOL bOn);
	BOOL MDINAPI_SetSurfaceFilterCoef(PWORD pCoef);
	BOOL MDINAPI_SetPeakingFilterCoef(PWORD pCoef);
	BOOL MDINAPI_EnablePeakingFilter(BOOL OnOff);
	BOOL MDINAPI_SetPeakingFilterGain(BYTE val);
	BOOL MDINAPI_SetPeakingCoringValue(BYTE val);
	BOOL MDINAPI_EnablePeakingCoring(BOOL OnOff);
	BOOL MDINAPI_SetPeakingSaturationValue(BYTE val);
	BOOL MDINAPI_EnablePeakingSaturation(BOOL OnOff);
	BOOL MDINAPI_SetColorEnhFilterCoef(PWORD pCoef);
	BOOL MDINAPI_SetFrontNRYFilterCoef(PWORD pCoef);
	BOOL MDINAPI_EnableFrontNRYFilter(BOOL OnOff);
	BOOL MDINAPI_SetFrontNRCFilterCoef(PWORD pCoef);
	BOOL MDINAPI_EnableFrontNRCFilter(BOOL OnOff);
	BOOL MDINAPI_EnableMedianFilter(BOOL OnOff);
	BOOL MDINAPI_SetBWExtensionPoint(PWORD pInPoint, PWORD pOutPoint);
	BOOL MDINAPI_EnableBWExtension(BOOL OnOff);
	BOOL MDINAPI_EnableLTI(BOOL OnOff);
	BOOL MDINAPI_EnableCTICb(BOOL OnOff);
	BOOL MDINAPI_EnableCTICr(BOOL OnOff);
	
	BOOL MDINAPI_SetHDecimateVideo(PMDIN_SRCVIDEO_INFO pSRC);
	BOOL MDINAPI_SetPLLVideoClock(WORD P, WORD M, WORD S);
	BOOL MDINAPI_SetPLLMemoryClock(WORD P, WORD M);
	BOOL MDINAPI_SetSrcHDMapBug(PMDIN_SRCVIDEO_INFO pINFO);
	BOOL MDINAPI_SetSrcVideoAUX(PMDIN_SRCVIDEO_INFO pINFO);
	BOOL MDINAPI_SetSrcPortMode(PMDIN_SRCVIDEO_INFO pINFO);
	BOOL MDINAPI_SetDACSyncData(PMDIN_DACSYNC_DATA pDAC);
	BOOL MDINAPI_SetOutVideoAUX(PMDIN_OUTVIDEO_INFO pINFO);
	BOOL MDINAPI_SetOutPortMode(PMDIN_OUTVIDEO_INFO pINFO);
	BOOL MDINAPI_SetCSCSrcVideo(PMDIN_SRCVIDEO_INFO pSRC, PMDIN_OUTVIDEO_INFO pOUT);
	BOOL MDINAPI_SetCSCOutVideo(PMDIN_SRCVIDEO_INFO pSRC, PMDIN_OUTVIDEO_INFO pOUT);
	BOOL MDINAPI_SetWindowSrcVideo(PMDIN_SRCVIDEO_INFO pSRC);
	BOOL MDINAPI_SetWindowOutVideo(PMDIN_OUTVIDEO_INFO pOUT);
	BOOL MDINAPI_SetFrontNRFilterInHD(PMDIN_SRCVIDEO_INFO pSRC);
	BOOL MDINAPI_SetMFCHYFilterCoef(PMDIN_OUTVIDEO_INFO pOUT, PMDIN_MFCFILT_COEF pCoef);
	BOOL MDINAPI_SetMFCHCFilterCoef(PMDIN_OUTVIDEO_INFO pOUT, PMDIN_MFCFILT_COEF pCoef);
	BOOL MDINAPI_SetMFCVYFilterCoef(PMDIN_OUTVIDEO_INFO pOUT, PMDIN_MFCFILT_COEF pCoef);
	BOOL MDINAPI_SetMFCVCFilterCoef(PMDIN_OUTVIDEO_INFO pOUT, PMDIN_MFCFILT_COEF pCoef);
	BOOL MDINAPI_SetMFCFilterCoef(PMDIN_OUTVIDEO_INFO pOUT);
	BOOL MDINAPI_SetMemPrecision(PMDIN_SRCVIDEO_INFO pSRC, PMDIN_OUTVIDEO_INFO pOUT);
	BOOL MDINAPI_SetDeintCaptionVposiThres(WORD level);
	BOOL MDINAPI_SetSurfaceFilterThres(BYTE level);
	BOOL MDINAPI_SetDeinterMode(PMDIN_SRCVIDEO_INFO pINFO);
	BOOL MDINAPI_ResetOutSync(WORD delay);
	BOOL MDINAPI_VideoProcess(PMDIN_SRCVIDEO_INFO pSRC, PMDIN_OUTVIDEO_INFO pOUT);
	BOOL MDINAPI_EnableOutPortPin(MDIN_PIN_OUT_t port, BOOL OnOff);
	BOOL MDINAPI_EnableDACPortOut(BOOL OnOff);
	
protected:
	I2CInterface		m_I2C;

	BOOL MDIN_MultiWrite(WORD rAddr, PWORD pBuff, WORD bytes);
	BOOL MDIN_MultiRead(WORD rAddr, PWORD pBuff, WORD bytes);
	BOOL MDIN_RegWrite(WORD rAddr, WORD wData);
	BOOL MDIN_RegRead(WORD rAddr, PWORD rData);
	BOOL MDIN_RegField(WORD rAddr, WORD bPos, WORD bCnt, WORD bData);
	BOOL MDVI1ST_MultiWrite(BYTE rAddr, PBYTE pBuff, WORD bytes);
	BOOL MDVI1ST_MultiRead(BYTE rAddr, PBYTE pBuff, WORD bytes);
	BOOL MDVI1ST_RegWrite(BYTE rAddr, BYTE wData);
	BOOL MDVI1ST_RegRead(BYTE rAddr, PBYTE rData);
	BOOL MDVI1ST_RegField(BYTE rAddr, WORD bPos, WORD bCnt, BYTE bData);
	BOOL MDVI2ND_MultiWrite(BYTE rAddr, PBYTE pBuff, WORD bytes);
	BOOL MDVI2ND_MultiRead(BYTE rAddr, PBYTE pBuff, WORD bytes);
	BOOL MDVI2ND_RegWrite(BYTE rAddr, BYTE wData);
	BOOL MDVI2ND_RegRead(BYTE rAddr, PBYTE rData);
	BOOL MDVI2ND_RegField(BYTE rAddr, WORD bPos, WORD bCnt, BYTE bData);

	WORD				MDINParseIRQ, MDINPLLClock;
	MDIN_DEINT_CTRL		stMDINDeint;
	MDIN_VIDEO_WINDOW	stMDINSrcWIN, stMDINDstWIN;
	MDIN_VIDEO_WINDOW	stMDINZoomWIN, stMDINViewWIN;

	MDIN_SRCVIDEO_INFO	stSrcVideoInfo;
	MDIN_OUTVIDEO_INFO	stOutVideoInfo;
	MDIN_VIDEO_CLOCK	stSrcVideoClock;
	MDIN_VIDEO_CLOCK	OutVideoClockInv;	// 2009,Oct, 15 for output clock delay/invert control
};

