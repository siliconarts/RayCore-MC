#include "HDMITransmitter.h"
#include <stdio.h>
#include "mdincoef.h"
#include "mdinfrmt.h"

HDMITransmitter::HDMITransmitter(void){
	memset((PBYTE)&stSrcVideoInfo, 0, sizeof(stSrcVideoInfo));
	memset((PBYTE)&stOutVideoInfo, 0, sizeof(stOutVideoInfo));
	SetOutputScreen(800, 480);
}


HDMITransmitter::~HDMITransmitter(void){
}

WORD HDMITransmitter::MDINAPI_GetDeviceID(void){
	WORD did;
	MDIN_RegRead(0x004, &did);
	return did;
}

BOOL HDMITransmitter::MDINAPI_SetMemory(BYTE endian, BYTE drive, BYTE delay){
	if (!MDIN_RegField(0x012, 0, 2, (WORD)((endian)? 3:0))) return FALSE;
	if (!MDIN_RegField(0x022, 4, 2, (WORD)((drive&MDIN_LVL2_CTRL_SDRAM)? 0:2))) return FALSE;
	if (!MDIN_RegField(0x022, 2, 2, (WORD)((drive&MDIN_LVL2_DATA_SDRAM)? 0:2))) return FALSE;
	if (!MDIN_RegField(0x022, 0, 2, delay)) return FALSE;
	if (!MDIN_RegField(0x03C, 12, 2, (WORD)((drive&MDIN_LVL2_CLK_SDRAM)? 0:2))) return FALSE;
	if (!MDIN_RegField(0x454, 3, 1, 1)) return FALSE;	// set fcmc starvation
	if (!MDIN_RegField(0x455, 0, 2, 0)) return FALSE;	// fix 32-bit 8MByte
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_DisplayOutWindow(BOOL OnOff){
	if (!MDIN_RegField(0x4B0, 5, 1, (WORD)(OnOff ? 1:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SoftReset(void){
	if (!MDIN_RegWrite(0x070, 0)) return FALSE;
	if (!MDIN_RegWrite(0x070, 1)) return FALSE;
	return TRUE;
}

BOOL HDMITransmitter::MDINAPI_SetVCLKPLLSource(MDIN_PLL_SOURCE_t src){
	if (!MDIN_RegWrite(0x03A, (WORD)((src==MDIN_PLL_SOURCE_HACT)? 1:0))) return FALSE;
	if (!MDIN_RegWrite(0x02A, (WORD)((src==MDIN_PLL_SOURCE_HACT)? 1:src))) return FALSE;
	if (!MDIN_RegField(0x018, 2, 1, 1)) return FALSE;	// intVCLK inverse for MFC coef
	MDINPLLClock = (WORD)src;	// restore PLL source clock for MFC coef
	return MDINAPI_SoftReset();
}

BOOL HDMITransmitter::MDINAPI_EnableClockDrive(MDIN_CLK_DRV_t clock, BOOL OnOff){
	switch (clock) {
	case MDIN_CLK_DRV_ALL:
		if (!MDIN_RegField(0x03C, 0, 12, (WORD)((OnOff)? 0:0xfff))) return FALSE;
		return MDIN_RegField(0x03C, 14, 1, (WORD)((OnOff)? 0:1));

	default:
		return MDIN_RegField(0x03C, (WORD)(clock), 1, (WORD)((OnOff)? 0:1));
	}
}

BOOL HDMITransmitter::MDINAPI_SetInDataMapMode(MDIN_IN_DATA_MAP_t mode){
	if (!MDIN_RegField(0x400, 14, 2, (BYTE)mode)) return FALSE;	// in data map mode
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_LoadDeintData(void){
	PWORD pBuff = (PWORD)MDIN_DeintFactor;
	if(!MDIN_MultiWrite(0x500, pBuff, 128)) return FALSE;

	// INTR_DN Off, HD_MODE Off, 3DNR_ON On
	stMDINDeint.attb  = 0x01;	stMDINDeint.useDN = 0x00;
	stMDINDeint.useNR = 0xBF;	stMDINDeint.useHD = (14<<8)|0x7F;

	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetDeintYMode(MDIN_DEINT_MODE_t mode){ // added by dsyang 20081202 default = "01"
	if (!MDIN_RegField(0x500, 14, 2, (WORD)(mode))) return FALSE;
	if (!MDIN_RegField(0x402, 15, 1, (WORD)((mode==2)? ON:OFF))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetDeintCMode(MDIN_DEINT_MODE_t mode){
	if ((stMDINDeint.attb&MDIN_DEINT_MODE_HD)==0)	// default = 0
		stMDINDeint.useHD = stMDINDeint.useHD&(~(3<<6))|(mode<<6);
	else mode = MDIN_DEINT_MODE_ADAPTIVE;

	if (!MDIN_RegField(0x528,  6, 2, (WORD)(mode))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnableDeintHDMode(BOOL bOn){
	if (bOn)	stMDINDeint.attb |=  MDIN_DEINT_MODE_HD;	// default = OFF
	else		stMDINDeint.attb &= ~MDIN_DEINT_MODE_HD;

	if (!MDIN_RegField(0x529, 11, 1, (WORD)((bOn)? 1:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnableDeintV2TAB(BOOL OnOff){
	if ((stMDINDeint.attb&MDIN_DEINT_INTR_DN)==0)	// default = OFF
		 stMDINDeint.useDN = stMDINDeint.useDN&(~(1<<0))|((BYTE)OnOff<<0);
	else OnOff = ON;

	if (!MDIN_RegField(0x53D, 7, 1, (WORD)((OnOff)? 1:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnableDeintHD3DAdp(BOOL OnOff){
	if (!MDIN_RegField(0x53E, 14, 1, (WORD)((OnOff)? 1:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetDeintFilmMode(MDIN_DEINT_FILM_t mode){
	if ((stMDINDeint.attb&MDIN_DEINT_MODE_HD)==0)	// default = 3
		stMDINDeint.useHD = stMDINDeint.useHD&(~(3<<4))|(mode<<4);
	else mode = MDIN_DEINT_FILM_DISABLE;

	WORD film_mode = (mode==MDIN_DEINT_FILM_32)? 5:(mode==MDIN_DEINT_FILM_22)? 6: (mode==MDIN_DEINT_FILM_ALL)? 4:0;
	if (!MDIN_RegField(0x50B, 12, 3, film_mode)) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetDeintCurMotionNRSel(BYTE mode){
	if (stMDINDeint.attb&MDIN_DEINT_3DNR_ON)	// default = 2
		stMDINDeint.useNR = stMDINDeint.useNR&(~(3<<6))|(mode<<6);
	else mode = 3;

	if (!MDIN_RegField(0x500, 12, 2, mode)) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetDeintCurMotionThres(BYTE level){
	if ((stMDINDeint.attb&MDIN_DEINT_MODE_HD)==0)	// default = 20
		 stMDINDeint.useHD = (stMDINDeint.useHD&0xff)|((WORD)level<<8);
	else level = 255;

	if (!MDIN_RegField(0x500, 0, 8, level)) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetDeintPDCurMotionThres(BYTE level){
	if (!MDIN_RegField(0x527, 0, 7, level)) return FALSE;
	if (!MDIN_RegField(0x527, 7, 1, (WORD)((level==40)? 0:1))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnableDeintCurMotionNRUse(BOOL bOn){
	if (stMDINDeint.attb&MDIN_DEINT_3DNR_ON)	// default = ON
		stMDINDeint.useNR = stMDINDeint.useNR&(~(1<<5))|((BYTE)bOn<<5);
	else bOn = FALSE;

	if (!MDIN_RegField(0x524, 15, 1, (WORD)((bOn)? 1:0))) return FALSE;
	if (!MDIN_RegField(0x513, 4, 1, (WORD)((bOn)? 1:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnableNextMotionNRUse(BOOL bOn){
	if (stMDINDeint.attb&MDIN_DEINT_3DNR_ON)	// default = ON
		stMDINDeint.useNR = stMDINDeint.useNR&(~(1<<4))|((BYTE)bOn<<4);
	else bOn = FALSE;

	if (!MDIN_RegField(0x529, 7, 1, (WORD)((bOn)? 1:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnableDeintFastMotion(BOOL OnOff){
	if ((stMDINDeint.attb&MDIN_DEINT_MODE_HD)==0)	// default = ON
		 stMDINDeint.useHD = stMDINDeint.useHD&(~(1<<3))|((BYTE)OnOff<<3);
	else OnOff = OFF;

	if (!MDIN_RegField(0x503, 15, 1, (WORD)((OnOff)? 1:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnableDeintFastHistoryUse(BOOL bOn){
	if (stMDINDeint.attb&MDIN_DEINT_3DNR_ON)	// default = ON
		 stMDINDeint.useNR = stMDINDeint.useNR&(~(1<<3))|((BYTE)bOn<<3);
	else bOn = FALSE;

	if (!MDIN_RegField(0x53D, 1, 1, (WORD)((bOn)? 1:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnableDeintBorder(BOOL OnOff){
	if ((stMDINDeint.attb&MDIN_DEINT_MODE_HD)==0)	// default = ON
		 stMDINDeint.useHD = stMDINDeint.useHD&(~(1<<2))|((BYTE)OnOff<<2);
	else OnOff = OFF;

	if (!MDIN_RegField(0x508, 3, 1, (WORD)((OnOff)? 1:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnableDeintBorderNRUse(BOOL bOn){
	if (stMDINDeint.attb&MDIN_DEINT_3DNR_ON)	// default = ON
		 stMDINDeint.useNR = stMDINDeint.useNR&(~(1<<2))|((BYTE)bOn<<2);
	else bOn = FALSE;

	if (!MDIN_RegField(0x53D, 3, 1, (WORD)((bOn)? 1:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnableDeintSlowMotionNRUse(BOOL bOn){
	if (stMDINDeint.attb&MDIN_DEINT_3DNR_ON)	// default = ON
		 stMDINDeint.useNR = stMDINDeint.useNR&(~(1<<1))|((BYTE)bOn<<1);
	else bOn = FALSE;

	if (!MDIN_RegField(0x524, 14, 1, (WORD)((bOn)? 1:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnableDeintStillMotionNRUse(BOOL bOn){
	if (stMDINDeint.attb&MDIN_DEINT_3DNR_ON)	// default = ON
		 stMDINDeint.useNR = stMDINDeint.useNR&(~(1<<0))|((BYTE)bOn<<0);
	else bOn = FALSE;

	if (!MDIN_RegField(0x524, 13, 1, (WORD)((bOn)? 1:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnableDeintNR(BOOL bOn){
	if ((stMDINDeint.attb&MDIN_DEINT_MODE_HD)==0) {	// default = ON
		if (stMDINDeint.attb&MDIN_DEINT_DECI_HD) bOn = FALSE;
		else stMDINDeint.useHD = stMDINDeint.useHD&(~(1<<0))|((BYTE)bOn<<0);
	}
	else bOn = FALSE;

	if (bOn)	stMDINDeint.attb |=  MDIN_DEINT_3DNR_ON;	// default = ON
	else		stMDINDeint.attb &= ~MDIN_DEINT_3DNR_ON;

	if (!MDIN_RegField(0x52A, 7, 1, (WORD)((bOn)? 1:0))) return FALSE;
	if (!MDINAPI_SetDeintCurMotionNRSel((BYTE)(stMDINDeint.useNR>>6))) return FALSE;
	if (!MDINAPI_EnableDeintCurMotionNRUse((BYTE)((stMDINDeint.useNR&0x20) != 0))) return FALSE;
	if (!MDINAPI_EnableNextMotionNRUse((BYTE)((stMDINDeint.useNR&0x10) != 0))) return FALSE;
	if (!MDINAPI_EnableDeintFastHistoryUse((BYTE)((stMDINDeint.useNR&0x08) != 0))) return FALSE;
	if (!MDINAPI_EnableDeintBorderNRUse((BYTE)((stMDINDeint.useNR&0x04) != 0))) return FALSE;
	if (!MDINAPI_EnableDeintSlowMotionNRUse((BYTE)((stMDINDeint.useNR&0x02) != 0))) return FALSE;
	if (!MDINAPI_EnableDeintStillMotionNRUse((BYTE)((stMDINDeint.useNR&0x01) != 0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnableDeintCCS(BOOL bOn){
	if ((stMDINDeint.attb&MDIN_DEINT_MODE_HD)==0) {	// default = ON
		if (stMDINDeint.attb&MDIN_DEINT_DECI_HD) bOn = OFF;
		else stMDINDeint.useHD = stMDINDeint.useHD&(~(1<<1))|((BYTE)bOn<<1);
	}
	else bOn = FALSE;

	if (!MDIN_RegField(0x52A, 6, 1, (WORD)((bOn)? 1:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetSurfaceFilterCoef(PWORD pCoef){
	if (pCoef==NULL) pCoef = MDIN_SurfaceFilter;
	if (!MDIN_MultiWrite(0x46D, (PWORD)pCoef, 8)) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetPeakingFilterCoef(PWORD pCoef){
	if (pCoef==NULL) pCoef = MDIN_PeakingFilter;
	if (!MDIN_MultiWrite(0x458, (PWORD)pCoef, 16)) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnablePeakingFilter(BOOL OnOff)
{
	if (!MDIN_RegField(0x460, 0, 1, (WORD)((OnOff)? 1:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetPeakingFilterGain(BYTE val){
	if (!MDIN_RegField(0x461, 8, 8, val)) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetPeakingCoringValue(BYTE val){
	if (!MDIN_RegField(0x460, 12, 4, val)) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnablePeakingCoring(BOOL OnOff){
	if (!MDIN_RegField(0x460, 2, 1, (WORD)((OnOff)? 1:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetPeakingSaturationValue(BYTE val){
	if (!MDIN_RegField(0x460, 8, 4, val)) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnablePeakingSaturation(BOOL OnOff){
	if (!MDIN_RegField(0x460, 1, 1, (WORD)((OnOff)? 1:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetColorEnhFilterCoef(PWORD pCoef){
	if (pCoef==NULL) pCoef = MDIN_CEFilter_Spline_1_00;
	if (!MDIN_MultiWrite(0x468, (PWORD)pCoef, 8)) return FALSE;
	if (!MDIN_RegWrite(0x46C, ON)) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetFrontNRYFilterCoef(PWORD pCoef){
	if (pCoef==NULL) pCoef = MDIN_FNRYFilter;
	MDIN_MultiWrite(0x422, (PWORD)pCoef, 16);
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnableFrontNRYFilter(BOOL OnOff){
	if (!MDIN_RegField(0x42A, 0, 1, (WORD)((OnOff)? 1:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);;	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetFrontNRCFilterCoef(PWORD pCoef){
	if (pCoef==NULL) pCoef = MDIN_FNRCFilter;
	MDIN_MultiWrite(0x42C, (PWORD)pCoef, 8);
	return MDIN_RegWrite(0x4C4, 1);;	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnableFrontNRCFilter(BOOL OnOff){
	if (!MDIN_RegField(0x42A, 3, 1, (WORD)((OnOff)? 1:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);;	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnableMedianFilter(BOOL OnOff){
	if (!MDIN_RegField(0x42A, 1, 1, (WORD)((OnOff)? 1:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}


BOOL HDMITransmitter::MDINAPI_SetBWExtensionPoint(PWORD pInPoint, PWORD pOutPoint){
	if (pInPoint==NULL) return FALSE;
	if (pOutPoint==NULL) return FALSE;
//	if (pInPoint==NULL) pInPoint = MDIN_BWExt_InPoint;
//	if (pOutPoint==NULL) pOutPoint = MDIN_BWExt_OutPoint;

	if (!MDIN_MultiWrite(0x4A6, (PWORD)pInPoint, 8)) return FALSE;
	if (!MDIN_RegField(0x4AA, 0, 10, *pOutPoint++)) return FALSE;
	if (!MDIN_RegField(0x4AB, 0, 10, *pOutPoint++)) return FALSE;
	if (!MDIN_RegField(0x4AC, 0, 10, *pOutPoint++)) return FALSE;
	if (!MDIN_RegField(0x4AD, 0, 10, *pOutPoint++)) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnableBWExtension(BOOL OnOff){
	if (!MDIN_RegField(0x4AF, 0, 1, (WORD)((OnOff)? 1:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnableLTI(BOOL OnOff){
	PWORD pCoef = MDIN_LTICTIFactor;
//	if (!MDIN_RegWrite(0x462, (OnOff)? pCoef[0]:0)) return FALSE;	// v1.90Q sunny
	if (!MDIN_RegWrite(0x463, (WORD)((OnOff)? pCoef[0]:0))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnableCTICb(BOOL OnOff){
	PWORD pCoef = MDIN_LTICTIFactor;
	if (!MDIN_RegWrite(0x464, (WORD)((OnOff)? pCoef[0]:0))) return FALSE;
//	if (!MDIN_RegWrite(0x465, (OnOff)? pCoef[0]:0)) return FALSE;	// v1.90Q sunny
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_EnableCTICr(BOOL OnOff){
	PWORD pCoef = MDIN_LTICTIFactor;
	if (!MDIN_RegWrite(0x465, (WORD)((OnOff)? pCoef[0]:0))) return FALSE;
//	if (!MDIN_RegWrite(0x466, (OnOff)? pCoef[0]:0)) return FALSE;	// v1.90Q sunny
//	if (!MDIN_RegWrite(0x467, (OnOff)? pCoef[0]:0)) return FALSE;	// v1.90Q sunny
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetSrcHDMapBug(PMDIN_SRCVIDEO_INFO pINFO){
	// interlace HD memory map bug
	if (pINFO->frmt>=VIDSRC_FORMAT_END) return FALSE;

	if (!MDIN_RegWrite(0x43C, 800)) return FALSE;
	if (!MDIN_RegWrite(0x43D, 480)) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetSrcVideoAUX(PMDIN_SRCVIDEO_INFO pINFO){
	WORD mode;	BYTE port = (pINFO->attb&MDIN_USE_INPORT_A)? 1:0;

	mode = (pINFO->stAUX.attb&MDIN_SRC_HACT_CSYNC)? ((port)? 1:0):0;
	if (!MDIN_RegField(0x402, (WORD)(14-port*8), 1, mode)) return FALSE;

	// in fieldid ctrl
	mode = (pINFO->stAUX.attb&MDIN_HIGH_IS_TOPFLD)? 1:0;
	if (!MDIN_RegField(0x401, (WORD)(0+port), 1, mode)) return FALSE;
	mode = (pINFO->stAUX.attb&MDIN_FIELDID_INPUT)? 1:((port)? 0:1);
	if (!MDIN_RegField(0x401, (WORD)(2+port), 1, mode)) return FALSE;

	// in format ctrl
	if (!MDIN_RegField(0x400, (WORD)(8+port*3), 3, pINFO->stAUX.delayYC)) return FALSE;
	mode = (pINFO->stAUX.attb&MDIN_SRC_CbCrSWAP_ON)? 1:0;
	if (!MDIN_RegField(0x400, (WORD)(2+port), 1, mode)) return FALSE;
	if (!MDIN_RegWrite(0x4C4, 1)) return FALSE;	// update local register

	// clock delay
	if (pINFO->stAUX.pCLK==NULL) return TRUE;
	mode = (pINFO->stAUX.pCLK->invert<<2)|pINFO->stAUX.pCLK->delay;
	if (!MDIN_RegWrite((WORD)(0x046-port*2), mode)) return FALSE;
	return TRUE;
}

BOOL HDMITransmitter::MDINAPI_SetSrcPortMode(PMDIN_SRCVIDEO_INFO pINFO){
	WORD mode;	BYTE port;
	if (pINFO->frmt>=VIDSRC_FORMAT_END) return FALSE;

	// set Htotal, attb[H/V-Polarity,ScanType], H/V size
	memcpy(&pINFO->total, (PBYTE)defMDINSrcVideo[pINFO->frmt], sizeof(defMDINSrcVideo[0]));

	// set InPort, Quality, Precision, PixelSize, ColorSpace
	pINFO->attb &= ~(MDIN_USE_INPORT_A|MDIN_QUALITY_HD|MDIN_PRECISION_8|MDIN_PIXELSIZE_422|MDIN_COLORSPACE_YUV);
	switch (pINFO->mode) {
		case MDIN_SRC_RGB444_A8:	mode = (0<<12)|(0<<4);
			pINFO->attb |= (MDIN_USE_INPORT_A|MDIN_PRECISION_8|MDIN_PIXELSIZE_444|MDIN_COLORSPACE_RGB); break;
		case MDIN_SRC_YUV444_A8:	mode = (0<<12)|(0<<4);
			pINFO->attb |= (MDIN_USE_INPORT_A|MDIN_PRECISION_8|MDIN_PIXELSIZE_444|MDIN_COLORSPACE_YUV); break;
		case MDIN_SRC_EMB422_A8:	mode = (1<<12)|(2<<4);
			pINFO->attb |= (MDIN_USE_INPORT_A|MDIN_PRECISION_8|MDIN_PIXELSIZE_422|MDIN_COLORSPACE_YUV); break;
		case MDIN_SRC_SEP422_A8:	mode = (1<<12)|(0<<4);
			pINFO->attb |= (MDIN_USE_INPORT_A|MDIN_PRECISION_8|MDIN_PIXELSIZE_422|MDIN_COLORSPACE_YUV); break;
		case MDIN_SRC_EMB422_A10:	mode = (1<<12)|(2<<4);
			pINFO->attb |= (MDIN_USE_INPORT_A|MDIN_PRECISION_10|MDIN_PIXELSIZE_422|MDIN_COLORSPACE_YUV); break;
		case MDIN_SRC_SEP422_A10:	mode = (1<<12)|(0<<4);
			pINFO->attb |= (MDIN_USE_INPORT_A|MDIN_PRECISION_10|MDIN_PIXELSIZE_422|MDIN_COLORSPACE_YUV); break;
		case MDIN_SRC_MUX656_A8:	mode = (2<<12)|(1<<4);
			pINFO->attb |= (MDIN_USE_INPORT_A|MDIN_PRECISION_8|MDIN_PIXELSIZE_422|MDIN_COLORSPACE_YUV); break;
		case MDIN_SRC_MUX656_A10:	mode = (2<<12)|(1<<4);
			pINFO->attb |= (MDIN_USE_INPORT_A|MDIN_PRECISION_10|MDIN_PIXELSIZE_422|MDIN_COLORSPACE_YUV); break;
		case MDIN_SRC_MUX656_B8:	mode = (2<<12)|(1<<4);
			pINFO->attb |= (MDIN_USE_INPORT_B|MDIN_PRECISION_8|MDIN_PIXELSIZE_422|MDIN_COLORSPACE_YUV); break;
		case MDIN_SRC_MUX656_B10:	mode = (2<<12)|(1<<4);
			pINFO->attb |= (MDIN_USE_INPORT_B|MDIN_PRECISION_10|MDIN_PIXELSIZE_422|MDIN_COLORSPACE_YUV); break;
	}

	if (pINFO->frmt==VIDSRC_1280x720p60||pINFO->frmt==VIDSRC_1920x1080i60||
		pINFO->frmt==VIDSRC_1280x720p50||pINFO->frmt==VIDSRC_1920x1080i50||
		pINFO->frmt==VIDSRC_1920x1080p30||pINFO->frmt==VIDSRC_1920x1080p25)
		 pINFO->attb |= MDIN_QUALITY_HD;
	else pINFO->attb |= MDIN_QUALITY_SD;

	// input sync ctrl
	if (!MDIN_RegField(0x402,  4, 2, (WORD)((mode>> 4)&3))) return FALSE;
	if (!MDIN_RegField(0x402, 12, 2, (WORD)((mode>>12)&3))) return FALSE;

	port = (pINFO->attb&MDIN_USE_INPORT_A)? 1:0;
	mode = (pINFO->attb&MDIN_SCANTYPE_PROG)? 1:0;
	if (pINFO->attb&MDIN_POSITIVE_HSYNC) mode |= ((port)? 4:0);
	if (pINFO->attb&MDIN_POSITIVE_VSYNC) mode |= ((port)? 2:0);
	if (!MDIN_RegField(0x402, (WORD)(8-port*8), 3, mode)) return FALSE;

	// fid pol thres
	if (!MDIN_RegField(0x401, 4, 12, pINFO->total)) return FALSE;

	// select input port
	if (!MDIN_RegField(0x4B0, 3, 1, port)) return FALSE;
//	if (!MDIN_RegWrite(0x451, 0x0842)) return FALSE;	// fix arbiter priority

	// H/V offset
	if (!MDIN_RegWrite((WORD)(0x405-port), (WORD)((pINFO->stCLIP.posiV<<9)|pINFO->stCLIP.posiH))) return FALSE;
	return MDINAPI_SetSrcVideoAUX(pINFO);	// set source video auxiliary
}

BOOL HDMITransmitter::MDINAPI_SetDACSyncData(PMDIN_DACSYNC_DATA pDAC){
	BYTE i;
	for (i=0; i<sizeof(MDIN_DACSYNC_DATA)/2; i++) {
		if (!MDIN_RegWrite(0x588, (WORD)((pDAC==NULL)? 0 : pDAC->ctrl[i]))) return FALSE;
		if (!MDIN_RegWrite(0x589, (WORD)(0x080|i))) return FALSE;
	}
	return TRUE;
}

BOOL HDMITransmitter::MDINAPI_SetPLLVideoClock(WORD P, WORD M, WORD S){
//	Vclk[MHz] = (Fin[MHz]*M)/(P*S), (Fin/P)>15KHz
	if (!MDIN_RegField(0x020, 0, 1, 1)) return FALSE;	// disable PLL
//	if (!MDIN_RegWrite(0x02A, 1)) return FALSE;			// used XTAL_IN
	if (!MDIN_RegWrite(0x02C, P)) return FALSE;			// pre-divider
	if (!MDIN_RegWrite(0x02E, M)) return FALSE;			// post-divider
	if (!MDIN_RegWrite(0x030, S)) return FALSE;			// post-scaler
	if (!MDIN_RegField(0x020, 0, 1, 0)) return FALSE;	// enable PLL
	return TRUE;
}

BOOL HDMITransmitter::MDINAPI_SetPLLMemoryClock(WORD P, WORD M){
//	Mclk[MHz] = (Fin[MHz]*M)/P, (Fin/P)>9MHz
	if (!MDIN_RegField(0x020, 1, 1, 1)) return FALSE;	// disable PLL
	if (!MDIN_RegWrite(0x032, P)) return FALSE;			// pre-divider
	if (!MDIN_RegWrite(0x034, M)) return FALSE;			// post-divider
	if (!MDIN_RegField(0x020, 1, 1, 0)) return FALSE;	// enable PLL
	return TRUE;
}

BOOL HDMITransmitter::MDINAPI_SetOutVideoAUX(PMDIN_OUTVIDEO_INFO pINFO){
	WORD mode;

	if (!MDIN_RegField(0x4A2, 4, 1, (WORD)((pINFO->stAUX.attb&MDIN_DIG_CbCrSWAP_ON)? 1:0))) return FALSE;
	if (!MDIN_RegField(0x4A3, 4, 1, (WORD)((pINFO->stAUX.attb&MDIN_DAC_PbPrSWAP_ON)? 1:0))) return FALSE;

	mode = (pINFO->stAUX.attb&MDIN_DE_OUT_IS_COMP)? 1:
		   (pINFO->stAUX.attb&MDIN_DE_OUT_IS_HACT)? 0:2;
	if (!MDIN_RegField(0x4B4, 8, 2, mode)) return FALSE;	// DE output mode

	// invert hact, inver vact
	if (!MDIN_RegField(0x4B4, 7, 1, (WORD)((pINFO->stAUX.attb&MDIN_NEGATIVE_HACT)? 1:0))) return FALSE;
	if (!MDIN_RegField(0x4B4, 6, 1, (WORD)((pINFO->stAUX.attb&MDIN_NEGATIVE_VACT)? 1:0))) return FALSE;

	// hsync out mode, vsync out mode
	if (!MDIN_RegField(0x4B4, 3, 1, (WORD)((pINFO->stAUX.attb&MDIN_HSYNC_OUT_HACT)? 1:0))) return FALSE;
	if (!MDIN_RegField(0x4B4, 2, 1, (WORD)((pINFO->stAUX.attb&MDIN_VSYNC_OUT_VACT)? 1:0))) return FALSE;
	if (!MDIN_RegWrite(0x4C4, 1)) return FALSE;		// update local register

	if (pINFO->stAUX.pOutVCLK!=NULL) {		// VCLK_OUT delay
		if (!MDIN_RegField(0x024, 0, 3,
		   (WORD)((pINFO->stAUX.pOutVCLK->invert<<2)|pINFO->stAUX.pOutVCLK->delay))) return FALSE;
	}

	if (pINFO->stAUX.pOutVCLKB!=NULL) {		// VCLK_OUT_B delay
		if (!MDIN_RegField(0x024, 4, 3,
		   (WORD)((pINFO->stAUX.pOutVCLKB->invert<<2)|pINFO->stAUX.pOutVCLKB->delay))) return FALSE;
	}

	if (pINFO->stAUX.pIntVCLK!=NULL) {		// internal vclk delay
		if (!MDIN_RegWrite(0x018,
		   (WORD)((pINFO->stAUX.pIntVCLK->invert<<2)|pINFO->stAUX.pIntVCLK->delay))) return FALSE;
	}

	if (pINFO->stAUX.pIntVCLK2X!=NULL) {	// internal vclk2x delay
		if (!MDIN_RegWrite(0x026,
		   (WORD)((pINFO->stAUX.pIntVCLK2X->invert<<2)|pINFO->stAUX.pIntVCLK2X->delay))) return FALSE;
	}

	if (pINFO->stAUX.pIntDACCLK!=NULL) {	// internal dacclk delay
		if (!MDIN_RegWrite(0x028,
		   (WORD)((pINFO->stAUX.pIntDACCLK->invert<<2)|pINFO->stAUX.pIntDACCLK->delay))) return FALSE;
	}
	return TRUE;
}

BOOL HDMITransmitter::MDINAPI_SetOutPortMode(PMDIN_OUTVIDEO_INFO pINFO){
	WORD mode, *pPLL = (PWORD)&pINFO->stSYNC.vclkP;

	if (pINFO->frmt>=VIDOUT_FORMAT_END) return FALSE;

	// set attb[H/V-Polarity,ScanType,FreeRun], SyncInfo, DACdata
	memcpy((PBYTE)&pINFO->attb, (PBYTE)defMDINOutVideo[pINFO->frmt], sizeof(defMDINOutVideo[0]));
	memcpy((PBYTE)&pINFO->stSYNC, (PBYTE)&defMDINOutSync[pINFO->frmt], sizeof(MDIN_OUTSYNC_INFO));
//	memcpy((PBYTE)&pINFO->stDAC, (PBYTE)&defMDINDACData[pINFO->frmt], sizeof(MDIN_DACSYNC_DATA));
	memcpy((PBYTE)&pINFO->stTMDS, (PBYTE)&defMDINOutTMDS[pINFO->frmt], sizeof(MDIN_OUTTMDS_CTRL));

	// set Quality, Precision, PixelSize, ColorSpace
	pINFO->attb &= ~(MDIN_QUALITY_HD|MDIN_PRECISION_8|MDIN_PIXELSIZE_422|MDIN_COLORSPACE_YUV);
	switch (pINFO->mode) {
		case MDIN_OUT_RGB444_8:		mode = (1<<8)|(0<<5)|(1<<2)|0;
			pINFO->attb |= (MDIN_PRECISION_8|MDIN_PIXELSIZE_444|MDIN_COLORSPACE_RGB); break;
		case MDIN_OUT_YUV444_8:		mode = (0<<8)|(2<<5)|(1<<2)|0;
			pINFO->attb |= (MDIN_PRECISION_8|MDIN_PIXELSIZE_444|MDIN_COLORSPACE_YUV); break;
		case MDIN_OUT_EMB422_8:		mode = (0<<8)|(3<<5)|(3<<2)|1;
			pINFO->attb |= (MDIN_PRECISION_8|MDIN_PIXELSIZE_422|MDIN_COLORSPACE_YUV); break;
		case MDIN_OUT_SEP422_8:		mode = (0<<8)|(2<<5)|(1<<2)|1;
			pINFO->attb |= (MDIN_PRECISION_8|MDIN_PIXELSIZE_422|MDIN_COLORSPACE_YUV); break;
		case MDIN_OUT_EMB422_10:	mode = (0<<8)|(3<<5)|(2<<2)|1;
			pINFO->attb |= (MDIN_PRECISION_10|MDIN_PIXELSIZE_422|MDIN_COLORSPACE_YUV); break;
		case MDIN_OUT_SEP422_10:	mode = (0<<8)|(2<<5)|(0<<2)|1;
			pINFO->attb |= (MDIN_PRECISION_10|MDIN_PIXELSIZE_422|MDIN_COLORSPACE_YUV); break;
		case MDIN_OUT_MUX656_8:		mode = (0<<8)|(3<<5)|(3<<2)|2;
			pINFO->attb |= (MDIN_PRECISION_8|MDIN_PIXELSIZE_422|MDIN_COLORSPACE_YUV); break;
		case MDIN_OUT_MUX656_10:	mode = (0<<8)|(3<<5)|(2<<2)|2;
			pINFO->attb |= (MDIN_PRECISION_10|MDIN_PIXELSIZE_422|MDIN_COLORSPACE_YUV); break;
	}

	if (pINFO->frmt==VIDOUT_1280x720p60||pINFO->frmt==VIDOUT_1280x720p50||pINFO->frmt==VIDOUT_1920x1080i60
	||pINFO->frmt==VIDOUT_1920x1080i50||pINFO->frmt==VIDOUT_1920x1080p60||pINFO->frmt==VIDOUT_1920x1080p50)
		 pINFO->attb |= MDIN_QUALITY_HD;
	else pINFO->attb |= MDIN_QUALITY_SD;

	if (pINFO->frmt==VIDOUT_720x480i60||pINFO->frmt==VIDOUT_720x480p60||pINFO->frmt==VIDOUT_720x576i50||
		pINFO->frmt==VIDOUT_720x576p50||(pINFO->attb&MDIN_QUALITY_HD))
		 memcpy((PBYTE)&pINFO->stDAC, (PBYTE)&defMDINDACData[pINFO->frmt], sizeof(MDIN_DACSYNC_DATA));
	else memset((PBYTE)&pINFO->stDAC, 0, sizeof(MDIN_DACSYNC_DATA));

	// BT656 format, sizeH<1024
	if (pINFO->stOUT.sizeH<1024&&(pINFO->mode==MDIN_OUT_MUX656_8||pINFO->mode==MDIN_OUT_MUX656_10)) {
		pINFO->stSYNC.totHS = 2*pINFO->stSYNC.totHS;	pINFO->stSYNC.bgnHA = 2*pINFO->stSYNC.bgnHA-1;
		pINFO->stSYNC.endHA = 2*pINFO->stSYNC.endHA-1;	pINFO->stSYNC.bgnHS = 2*pINFO->stSYNC.bgnHS-1;
		pINFO->stSYNC.vclkS =   pINFO->stSYNC.vclkS-1;	pINFO->stOUT.sizeH  = 2*pINFO->stOUT.sizeH;
	}

	if (!MDIN_RegField(0x4A2, 0, 4, (WORD)(mode&15))) return FALSE;	// out control
	if (!MDIN_RegField(0x4A2, 5, 5, (WORD)(mode>>5))) return FALSE;

	mode  = ((pINFO->attb&MDIN_COLORSPACE_YUV)? 5:0);
	mode |= ((pINFO->attb&MDIN_QUALITY_HD)? 2:0);
	if (!MDIN_RegField(0x4A3, 0, 4, mode)) return FALSE;	// dac control

// #if SUPPROT_HDMI_SDINTR==1
// 	mode = (pINFO->frmt==VIDOUT_720x480i60||pINFO->frmt==VIDOUT_720x576i50)? 2:0;
// 	if (!MDIN_RegField(0x042, 2, 2, mode)) return FALSE;	// set vclk-src for HDMI(480i/576i)
// 	if (mode==2) pINFO->stSYNC.vclkS = pINFO->stSYNC.vclkS-1;
// #endif

	if (!MDINAPI_SetDACSyncData((pINFO->attb&MDIN_COLORSPACE_YUV)? &pINFO->stDAC:NULL)) return FALSE;
	if (!MDIN_MultiWrite(0x484, (PWORD)&pINFO->stSYNC.posH, 6)) return FALSE;	// set sync registers
	if (!MDIN_MultiWrite(0x488, (PWORD)&pINFO->stSYNC.totHS, 30)) return FALSE;

	if (!MDINAPI_SetPLLVideoClock(*pPLL, *(pPLL+1), *(pPLL+2))) return FALSE;	// set video clock

	mode = (pINFO->frmt==VIDOUT_1920x1200pRB)? 11 : 10;		// set memory clock to 148.5/135.0MHz
	if (!MDINAPI_SetPLLMemoryClock(2, mode)) return FALSE;

	if (!MDIN_RegWrite(0x01C, 0)) return FALSE;			// clock is not double
	if (!MDIN_RegField(0x445, 0, 2, 2)) return FALSE;	// disable DRC filter
	if (!MDIN_RegField(0x445, 8, 8, 166)) return FALSE;	// VY 32-phase dual-filter
//	if (!MDIN_RegField(0x445, 8, 2, 3)) return FALSE;	// VY 32-phase dual-filter	// SS Demo

	if (!MDIN_RegField(0x040, 4, 3, pINFO->stTMDS.iod)) return FALSE;	// set TMDS current
	if (!MDIN_RegField(0x040, 0, 4, pINFO->stTMDS.iorv)) return FALSE;	// set TMDS de-emphasis

	if (!MDIN_RegWrite(0x497, (WORD)((pINFO->attb&MDIN_SCANTYPE_PROG)? 0:1))) return FALSE;	// scan_type
	if (!MDIN_RegWrite(0x498, (WORD)((pINFO->attb&MDIN_SYNC_FREERUN)? 0x4d:0x4c))) return FALSE;// free-run
	if (!MDIN_RegField(0x4B4, 5, 1, (WORD)((pINFO->attb&MDIN_POSITIVE_HSYNC)? 1:0))) return FALSE;	// Hsync
	if (!MDIN_RegField(0x4B4, 4, 1, (WORD)((pINFO->attb&MDIN_POSITIVE_VSYNC)? 1:0))) return FALSE;	// Vsync
	return MDINAPI_SetOutVideoAUX(pINFO);	// set output video auxiliary
}

BOOL HDMITransmitter::MDINAPI_SetCSCSrcVideo(PMDIN_SRCVIDEO_INFO pSRC, PMDIN_OUTVIDEO_INFO pOUT){
	PMDIN_CSCCTRL_DATA pCSC;

	if (pSRC->attb&MDIN_COLORSPACE_YUV) {
		if (pOUT->attb&MDIN_COLORSPACE_YUV)
			 pCSC = &MDIN_CscBypass_StdRange;	// YUV(HD or SD) to YUV(HD or SD)
		else pCSC = &MDIN_CscBypass_StdRange;	// YUV(HD or SD) to RGB(HD or SD)
	}		
	else if (pSRC->attb&MDIN_QUALITY_HD) {
		if (pOUT->attb&MDIN_COLORSPACE_YUV)
			 pCSC = &MDIN_CscRGBtoYUV_HD_StdRange;	// RGB(HD) to YUV(HD or SD)
		else pCSC = &MDIN_CscRGBtoYUV_HD_FullRange;	// RGB(HD) to RGB(HD or SD)
	}
	else {
		if (pOUT->attb&MDIN_COLORSPACE_YUV)
			 pCSC = &MDIN_CscRGBtoYUV_SD_StdRange;	// RGB(SD) to YUV(HD or SD)
		else pCSC = &MDIN_CscRGBtoYUV_SD_FullRange;	// RGB(SD) to RGB(HD or SD)
	}

	if (pSRC->stAUX.pCSC!=NULL) pCSC = pSRC->stAUX.pCSC;
	if (!MDIN_MultiWrite(0x40A, (PWORD)pCSC, sizeof(MDIN_CSCCTRL_DATA))) return FALSE;
	if (!MDIN_RegWrite(0x419, (WORD)((pSRC->attb&MDIN_COLORSPACE_YUV)? 0x1fc:0x1f8))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetCSCOutVideo(PMDIN_SRCVIDEO_INFO pSRC, PMDIN_OUTVIDEO_INFO pOUT){
	MDIN_CSCCTRL_DATA stMDINOutCSC, *pCSC;
	LONG contrast, saturation, brightness, coshue, sinhue;
	SHORT offsetG, offsetR, offsetB;
//	FLOAT contrast, hue, saturation, brightness;

	if (pSRC->attb&MDIN_QUALITY_HD) {
		if (pOUT->attb&MDIN_COLORSPACE_YUV) {
			if (pOUT->attb&MDIN_QUALITY_HD)
				 pCSC = &MDIN_CscBypass_StdRange;	// RGB or YUV(HD) to YUV(HD)
			else pCSC = &MDIN_CscHDtoSD_StdRange;	// RGB or YUV(HD) to YUV(SD)
		}
		else {
			if (pOUT->attb&MDIN_QUALITY_HD)
				 pCSC = &MDIN_CscYUVtoRGB_HD_FullRange;	// RGB or YUV(HD) to RGB(HD)
			else pCSC = &MDIN_CscYUVtoRGB_HD_FullRange;	// RGB or YUV(HD) to RGB(SD)
		}
	}
	else {
		if (pOUT->attb&MDIN_COLORSPACE_YUV) {
			if (pOUT->attb&MDIN_QUALITY_HD)
				 pCSC = &MDIN_CscSDtoHD_StdRange;	// RGB or YUV(SD) to YUV(HD)
			else pCSC = &MDIN_CscBypass_StdRange;	// RGB or YUV(SD) to YUV(SD)
		}
		else {
			if (pOUT->attb&MDIN_QUALITY_HD)
				 pCSC = &MDIN_CscYUVtoRGB_SD_FullRange;	// RGB or YUV(SD) to RGB(HD)
			else pCSC = &MDIN_CscYUVtoRGB_SD_FullRange;	// RGB or YUV(SD) to RGB(SD)
		}
	}

	if (pOUT->stAUX.pCSC!=NULL) pCSC = pOUT->stAUX.pCSC;
	memcpy(&stMDINOutCSC, (PBYTE)pCSC, sizeof(MDIN_CSCCTRL_DATA));
/*
	contrast = (FLOAT)pINFO->contrast/128.0;
	hue = 3.141592*((FLOAT)pINFO->hue-128)/384.0;	// +-180:128, +-90:256, +-60:384
	saturation = (FLOAT)pINFO->saturation/128.0;
	brightness = ((FLOAT)pINFO->brightness-128)*2.0;
	
	stMDINOutCSC.coef[0] =
		CLIP12(((SHORT)pCSC->coef[0]*contrast))&0xfff;
	stMDINOutCSC.coef[1] =
		CLIP12(((SHORT)pCSC->coef[1]*cos(hue)+(SHORT)pCSC->coef[2]*sin(hue))*saturation)&0xfff;
	stMDINOutCSC.coef[2] =
		CLIP12(((SHORT)pCSC->coef[2]*cos(hue)-(SHORT)pCSC->coef[1]*sin(hue))*saturation)&0xfff;
	stMDINOutCSC.coef[3] =
		CLIP12(((SHORT)pCSC->coef[3]*contrast))&0xfff;
	stMDINOutCSC.coef[4] =
		CLIP12(((SHORT)pCSC->coef[4]*cos(hue)+(SHORT)pCSC->coef[5]*sin(hue))*saturation)&0xfff;
	stMDINOutCSC.coef[5] =
		CLIP12(((SHORT)pCSC->coef[5]*cos(hue)-(SHORT)pCSC->coef[4]*sin(hue))*saturation)&0xfff;
	stMDINOutCSC.coef[6] =
		CLIP12(((SHORT)pCSC->coef[6]*contrast))&0xfff;
	stMDINOutCSC.coef[7] =
		CLIP12(((SHORT)pCSC->coef[7]*cos(hue)+(SHORT)pCSC->coef[8]*sin(hue))*saturation)&0xfff;
	stMDINOutCSC.coef[8] =
		CLIP12(((SHORT)pCSC->coef[8]*cos(hue)-(SHORT)pCSC->coef[7]*sin(hue))*saturation)&0xfff;
	stMDINOutCSC.inBias[0] =
		CLIP12(((SHORT)pCSC->inBias[0]+brightness))&0xfff;
*/
	saturation = (LONG)(WORD)pOUT->saturation;
	contrast = (LONG)(WORD)pOUT->contrast;
	brightness = (LONG)(WORD)pOUT->brightness;
	coshue = (LONG)(SHORT)MDIN_CscCosHue[pOUT->hue];
	sinhue = (LONG)(SHORT)MDIN_CscSinHue[pOUT->hue];

	stMDINOutCSC.coef[0] =
		CLIP12((((SHORT)pCSC->coef[0]*contrast)>>7))&0xfff;
	stMDINOutCSC.coef[1] =
		CLIP12(((((SHORT)pCSC->coef[1]*coshue+(SHORT)pCSC->coef[2]*sinhue)*saturation)>>17))&0xfff;
	stMDINOutCSC.coef[2] =
		CLIP12(((((SHORT)pCSC->coef[2]*coshue-(SHORT)pCSC->coef[1]*sinhue)*saturation)>>17))&0xfff;
	stMDINOutCSC.coef[3] =
		CLIP12((((SHORT)pCSC->coef[3]*contrast)>>7))&0xfff;
	stMDINOutCSC.coef[4] =
		CLIP12(((((SHORT)pCSC->coef[4]*coshue+(SHORT)pCSC->coef[5]*sinhue)*saturation)>>17))&0xfff;
	stMDINOutCSC.coef[5] =
		CLIP12(((((SHORT)pCSC->coef[5]*coshue-(SHORT)pCSC->coef[4]*sinhue)*saturation)>>17))&0xfff;
	stMDINOutCSC.coef[6] =
		CLIP12((((SHORT)pCSC->coef[6]*contrast)>>7))&0xfff;
	stMDINOutCSC.coef[7] =
		CLIP12(((((SHORT)pCSC->coef[7]*coshue+(SHORT)pCSC->coef[8]*sinhue)*saturation)>>17))&0xfff;
	stMDINOutCSC.coef[8] =
		CLIP12(((((SHORT)pCSC->coef[8]*coshue-(SHORT)pCSC->coef[7]*sinhue)*saturation)>>17))&0xfff;
	stMDINOutCSC.inBias[0] =
		CLIP12(((SHORT)pCSC->inBias[0]+(brightness-128)*2))&0xfff;

	if ((pOUT->attb&MDIN_COLORSPACE_YUV)==0) {	// only apply for RGB output
		offsetG = (SHORT)pOUT->offsetG;	stMDINOutCSC.outBias[0] = CLIP12((offsetG-128)*2)&0xfff;
		offsetB = (SHORT)pOUT->offsetB;	stMDINOutCSC.outBias[1] = CLIP12((offsetB-128)*2)&0xfff;
		offsetR = (SHORT)pOUT->offsetR;	stMDINOutCSC.outBias[2] = CLIP12((offsetR-128)*2)&0xfff;
	}
	
	if (!MDIN_MultiWrite(0x472, (PWORD)&stMDINOutCSC, sizeof(MDIN_CSCCTRL_DATA))) return FALSE;
	if (!MDIN_RegWrite(0x481, 0x1fc)) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetWindowSrcVideo(PMDIN_SRCVIDEO_INFO pSRC){
	BYTE port = (pSRC->attb&MDIN_USE_INPORT_A)? 1:0;
	BYTE mode = (pSRC->attb&MDIN_PRECISION_8)? 8:24;

//	normalize precision
	pSRC->stSRC.sizeH = (pSRC->stSRC.sizeH/mode)*mode;
	pSRC->stSRC.sizeV = (pSRC->stSRC.sizeV/8)*8;

//	replace clip window
	if (pSRC->stCLIP.sizeH>0&&pSRC->stCLIP.sizeV>0)
		 stMDINSrcWIN = pSRC->stCLIP;
	else stMDINSrcWIN = pSRC->stSRC;

//	in_offset_H, in_offset_V
	if (!MDIN_RegField((WORD)(0x405-port), 0, 9, pSRC->stCLIP.posiH)) return FALSE;
	if (!MDIN_RegField((WORD)(0x405-port), 9, 7, pSRC->stCLIP.posiV)) return FALSE;

//	in_size_H, in_size_V
	if (!MDIN_MultiWrite((WORD)(0x408-2*port), (PWORD)&stMDINSrcWIN, 4)) return FALSE;
	memset(&stMDINSrcWIN.posiH, 0, 4);	// clear position

//	src_size_H2, src_size_V2
	if (!MDIN_MultiWrite(0x43A, (PWORD)&stMDINSrcWIN, 4)) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetHDecimateVideo(PMDIN_SRCVIDEO_INFO pSRC){
//	H-decimation Mode ON/OFF
	if ((pSRC->attb&MDIN_SCANTYPE_PROG)==0&&(stMDINSrcWIN.sizeH>1024))
		 stMDINDeint.attb |=  MDIN_DEINT_DECI_HD;	// interlace & over 1024
	else if (stMDINSrcWIN.sizeH>1280)
		 stMDINDeint.attb |=  MDIN_DEINT_DECI_HD;	// over 1280
	else stMDINDeint.attb &= ~MDIN_DEINT_DECI_HD;

//	if (stMDINDeint.attb&MDIN_DEINT_FULL_HD) return TRUE;
	if (stMDINDeint.attb&MDIN_DEINT_DECI_HD) {
		if ((pSRC->attb&MDIN_SCANTYPE_PROG)==0)
			stMDINSrcWIN.sizeH = 1024;		// H-decimation (interlace input)
		else 
			stMDINSrcWIN.sizeH = 1280;		// H-decimation (progressive input)
	}
	return TRUE;
}

BOOL HDMITransmitter::MDINAPI_SetWindowOutVideo(PMDIN_OUTVIDEO_INFO pOUT){
	stMDINDstWIN = stMDINSrcWIN;	// copy source window

	//	replace view window
	if (pOUT->stVIEW.sizeH>0&&pOUT->stVIEW.sizeV>0){
		 stMDINViewWIN = pOUT->stVIEW;
		 // 화면 가운데로 보정
		 if(pOUT->stVIEW.sizeH < pOUT->stOUT.sizeH)
			 stMDINViewWIN.posiH = (pOUT->stOUT.sizeH - pOUT->stVIEW.sizeH) / 2;
		 if(pOUT->stVIEW.sizeV < pOUT->stOUT.sizeV)
			 stMDINViewWIN.posiV = (pOUT->stOUT.sizeV - pOUT->stVIEW.sizeV) / 2;
	}else stMDINViewWIN = pOUT->stOUT;


	if (stMDINDstWIN.sizeH>stMDINViewWIN.sizeH)		// H-decimation
		 stMDINDstWIN.sizeH = stMDINViewWIN.sizeH;

//	dst_size_H2, dst_size_V2
	if (!MDIN_MultiWrite(0x43C, (PWORD)&stMDINDstWIN, 4)) return FALSE;

//	replace zoom window
	if (pOUT->stZOOM.sizeH>0&&pOUT->stZOOM.sizeV>0)
		 stMDINZoomWIN = pOUT->stZOOM;
	else stMDINZoomWIN = stMDINDstWIN;

//	src_size_H, src_size_V, src_posi_H, src_posi_V
	if (!MDIN_MultiWrite(0x432, (PWORD)&stMDINZoomWIN, 8)) return FALSE;

//	dst_size_H, dst_size_V, dst_posi_H, dst_posi_V
	if (!MDIN_MultiWrite(0x436, (PWORD)&stMDINViewWIN, 8)) return FALSE;

	if ((pOUT->attb&MDIN_SCANTYPE_PROG)==0) stMDINViewWIN.sizeV>>=1;	// 576i to 480i
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetFrontNRFilterInHD(PMDIN_SRCVIDEO_INFO pSRC){
	PWORD pBuff; BYTE mode;

	mode = (stMDINDeint.attb&MDIN_DEINT_DECI_HD)? ON:OFF;
	if (stMDINDeint.attb&MDIN_DEINT_FULL_HD) mode = OFF;	// H-double proc
	if (!MDINAPI_EnableFrontNRYFilter((BYTE)((mode)? ON:OFF))) return FALSE;
	if (!MDINAPI_EnableFrontNRCFilter((BYTE)((mode)? ON:OFF))) return FALSE;

	if (mode==OFF) return TRUE;

//	Select type of FrontNRY/FrontNRC filter
//	Current version of API support for 1920x1080i60/50 & 1920x1080p30/25 input
	if ((pSRC->attb&MDIN_SCANTYPE_PROG)==0)	// interlace input
		mode = (stMDINViewWIN.sizeH>=1024)? 0:4;	// h size : 1920->1024, 1920->720
	else 									// progressive input
		mode = (stMDINViewWIN.sizeH>=1280)? 1:4;	// h size : 1920->1280, 1920->720

 	pBuff = (PWORD)MDIN_FNRCHDFilt[mode];
	if (!MDINAPI_SetFrontNRCFilterCoef(pBuff)) return FALSE;

	pBuff = (PWORD)MDIN_FNRYHDFilt[mode];
	if (!MDINAPI_SetFrontNRYFilterCoef(pBuff)) return FALSE;
	return TRUE;
}

BOOL HDMITransmitter::MDINAPI_SetMFCHYFilterCoef(PMDIN_OUTVIDEO_INFO pOUT, PMDIN_MFCFILT_COEF pCoef){
	BYTE i;	WORD pBuff[4] = {0,0,0,0};

	pOUT->stAUX.pMFCHY = (PMDIN_MFCFILT_COEF)pCoef;
	if (pCoef==NULL) pCoef = &MDIN_MFCFilter_Spline_1_00;

	for (i=0; i<64; i++) {
//		memcpy(pBuff, ((PWORD)pCoef)+i*4, 8);	// case of Lanczos_8
		memcpy(pBuff, ((PWORD)pCoef)+i*2, 4);
		if (!MDIN_MultiWrite(0x590, pBuff, 8)) return FALSE;
		if (!MDIN_RegWrite(0x594, (WORD)(0x080|i))) return FALSE;
	}
	return TRUE;
}


BOOL HDMITransmitter::MDINAPI_SetMFCHCFilterCoef(PMDIN_OUTVIDEO_INFO pOUT, PMDIN_MFCFILT_COEF pCoef){
	BYTE i;	WORD pBuff[4] = {0,0,0,0};

	pOUT->stAUX.pMFCHC = (PMDIN_MFCFILT_COEF)pCoef;
	if (pCoef==NULL) pCoef = &MDIN_MFCFilter_Spline_1_00;

	for (i=0; i<64; i++) {
		memcpy(pBuff, ((PWORD)pCoef)+i*2, 4);
		if (!MDIN_MultiWrite(0x590, pBuff, 8)) return FALSE;
		if (!MDIN_RegWrite(0x594, (WORD)(0x180|i))) return FALSE;
	}
	return TRUE;
}

BOOL HDMITransmitter::MDINAPI_SetMFCVYFilterCoef(PMDIN_OUTVIDEO_INFO pOUT, PMDIN_MFCFILT_COEF pCoef){
	BYTE i;	WORD pBuff[4] = {0,0,0,0};

	pOUT->stAUX.pMFCVY = (PMDIN_MFCFILT_COEF)pCoef;
	if (pCoef==NULL) pCoef = &MDIN_MFCFilter_Spline_1_00;

	if (stMDINViewWIN.sizeV<stMDINDstWIN.sizeV/2)	// over half scale-down
		pCoef = &MDIN_MFCFilter_Bilinear;

	for (i=0; i<64; i++) {
		memcpy(pBuff, ((PWORD)pCoef)+i*2, 4);
		if (!MDIN_MultiWrite(0x590, pBuff, 8)) return FALSE;
		if (!MDIN_RegWrite(0x594, (WORD)(0x280|i))) return FALSE;
	}
	return TRUE;
}

BOOL HDMITransmitter::MDINAPI_SetMFCVCFilterCoef(PMDIN_OUTVIDEO_INFO pOUT, PMDIN_MFCFILT_COEF pCoef){
	BYTE i;	WORD pBuff[4] = {0,0,0,0};

	pOUT->stAUX.pMFCVC = (PMDIN_MFCFILT_COEF)pCoef;
	if (pCoef==NULL) pCoef = &MDIN_MFCFilter_Spline_1_00;

	if (stMDINViewWIN.sizeV<stMDINDstWIN.sizeV/2)	// over half scale-down
		pCoef = &MDIN_MFCFilter_Bilinear;

	for (i=0; i<64; i++) {
		memcpy(pBuff, ((PWORD)pCoef)+i*2, 4);
		if (!MDIN_MultiWrite(0x590, pBuff, 8)) return FALSE;
		if (!MDIN_RegWrite(0x594, (WORD)(0x380|i))) return FALSE;
	}
	return TRUE;
}

BOOL HDMITransmitter::MDINAPI_SetMFCFilterCoef(PMDIN_OUTVIDEO_INFO pOUT){
	static BYTE MDINMFCCoef = 0; BYTE mfc;
	MDIN_PLL_SOURCE_t src = (MDIN_PLL_SOURCE_t)MDINPLLClock;

	mfc = (stMDINViewWIN.sizeV<stMDINDstWIN.sizeV/2)? 2:1;
	if (MDINMFCCoef==mfc) return TRUE;

	if (!MDIN_RegField(0x020, 0, 1, 1)) return FALSE;	// disable PLL
	if (!MDINAPI_SetVCLKPLLSource(MDIN_PLL_SOURCE_XTAL)) return FALSE;

	if (!MDINAPI_SetMFCHYFilterCoef(pOUT, pOUT->stAUX.pMFCHY)) return FALSE;
	if (!MDINAPI_SetMFCHCFilterCoef(pOUT, pOUT->stAUX.pMFCHC)) return FALSE;
	if (!MDINAPI_SetMFCVYFilterCoef(pOUT, pOUT->stAUX.pMFCVY)) return FALSE;
	if (!MDINAPI_SetMFCVCFilterCoef(pOUT, pOUT->stAUX.pMFCVC)) return FALSE;

	if (!MDINAPI_SetVCLKPLLSource(src)) return FALSE;	// restore PLL source
	if (!MDIN_RegField(0x020, 0, 1, 0)) return FALSE;	// enable PLL

	MDINMFCCoef = mfc;
	return TRUE;
}

BOOL HDMITransmitter::MDINAPI_SetMemPrecision(PMDIN_SRCVIDEO_INFO pSRC, PMDIN_OUTVIDEO_INFO pOUT){
	if (!MDIN_RegField(0x455, 2, 1, (WORD)((pSRC->attb&MDIN_PRECISION_8)? 0:1))) return FALSE;

	if (stMDINDstWIN.sizeH>768) pSRC->attb |= MDIN_PRECISION_8;
	if (pOUT->frmt==VIDOUT_1920x1080p60) pSRC->attb |= MDIN_PRECISION_8;
	if (pOUT->frmt==VIDOUT_1920x1080p50) pSRC->attb |= MDIN_PRECISION_8;
	if (pOUT->frmt==VIDOUT_1024x768p75)  pSRC->attb |= MDIN_PRECISION_8;
	if (pOUT->frmt==VIDOUT_1280x1024p60) pSRC->attb |= MDIN_PRECISION_8;
	if (pOUT->frmt==VIDOUT_1280x1024p75) pSRC->attb |= MDIN_PRECISION_8;
	if (pOUT->frmt==VIDOUT_1440x900p60)  pSRC->attb |= MDIN_PRECISION_8;
	if (pOUT->frmt==VIDOUT_1680x1050p60) pSRC->attb |= MDIN_PRECISION_8;
	if (pOUT->frmt==VIDOUT_1600x1200p60) pSRC->attb |= MDIN_PRECISION_8;		// v1.90Q sunny
	if (!MDIN_RegField(0x455, 2, 1, (WORD)((pSRC->attb&MDIN_PRECISION_8)? 0:1))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetDeintCaptionVposiThres(WORD level){	// added by dsyang 20081203 default = 180
	if (!MDIN_RegField(0x518, 0, 10, level)) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetSurfaceFilterThres(BYTE level){	// added by dsyang 20081203 default = 3
	if (!MDIN_RegField(0x53A, 14, 2, 0)) return FALSE;
	if (!MDIN_RegField(0x53A, 8, 6, level)) return FALSE;
	if (!MDIN_RegField(0x53A, 0, 6, level)) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

BOOL HDMITransmitter::MDINAPI_SetDeinterMode(PMDIN_SRCVIDEO_INFO pINFO){
	// When tenbit mode is OFF, source_h_size > 1024, When tenbit mode is on, source_h_size > 768.  
	if (((pINFO->attb&MDIN_PRECISION_8)&&stMDINDstWIN.sizeH>1024)||
	   (!(pINFO->attb&MDIN_PRECISION_8)&&stMDINDstWIN.sizeH>768))
		 stMDINDeint.attb |=  MDIN_DEINT_MODE_HD;
	else stMDINDeint.attb &= ~MDIN_DEINT_MODE_HD;

	if (!MDINAPI_EnableDeintHDMode((BYTE)((stMDINDeint.attb&MDIN_DEINT_MODE_HD)? ON:OFF))) return FALSE;
	if (!MDINAPI_EnableDeintHD3DAdp((BYTE)((stMDINDeint.attb&MDIN_DEINT_MODE_HD)? ON:OFF))) return FALSE;
	if (!MDINAPI_SetDeintCMode((MDIN_DEINT_MODE_t)((stMDINDeint.useHD>>6)&3))) return FALSE;
	if (!MDINAPI_SetDeintCurMotionThres((BYTE)(stMDINDeint.useHD>>8))) return FALSE;
	if (!MDINAPI_EnableDeintFastMotion((BYTE)((stMDINDeint.useHD&0x08)? ON:OFF))) return FALSE;
	if (!MDINAPI_EnableDeintBorder((BYTE)((stMDINDeint.useHD&0x04)? ON:OFF))) return FALSE;
	if (!MDINAPI_SetDeintFilmMode((MDIN_DEINT_FILM_t)((stMDINDeint.useHD>>4)&3))) return FALSE;
	if (!MDINAPI_EnableDeintCCS((BYTE)((stMDINDeint.useHD&0x02)? ON:OFF))) return FALSE;
	if (!MDINAPI_EnableDeintNR((BYTE)((stMDINDeint.useHD&0x01)? ON:OFF))) return FALSE;

	// input is interlace and dest_v_size < source_v_size/2
	if ((pINFO->attb&MDIN_SCANTYPE_PROG)==0&&(stMDINViewWIN.sizeV<stMDINDstWIN.sizeV/2))
		 stMDINDeint.attb |=  MDIN_DEINT_INTR_DN;
	else stMDINDeint.attb &= ~MDIN_DEINT_INTR_DN;

	if (!MDINAPI_EnableDeintV2TAB((stMDINDeint.useDN&0x01))? ON:OFF) return FALSE;

	return MDINAPI_SetDeintCaptionVposiThres((WORD)(pINFO->stSRC.sizeV*3/4));
}

BOOL HDMITransmitter::MDINAPI_ResetOutSync(WORD delay){
	if (!MDIN_RegWrite(0x499, 0)) return FALSE;
	if (!MDIN_RegWrite(0x499, (WORD)(0x1000|delay))) return FALSE;
	return MDIN_RegWrite(0x4C4, 1);	// update local register
}

// Drive Function for Video Process Block (Input/Output/Scaler/Deinterlace/CSC/PLL)
BOOL HDMITransmitter::MDINAPI_VideoProcess(PMDIN_SRCVIDEO_INFO pSRC, PMDIN_OUTVIDEO_INFO pOUT){
	if (!MDINAPI_SetSrcHDMapBug(pSRC)) return FALSE;		// correct HD-MAP bug
	if (!MDINAPI_SetSrcPortMode(pSRC)) return FALSE;		// set source video port
	if (!MDINAPI_SetOutPortMode(pOUT)) return FALSE;		// set output video port
	if (!MDINAPI_SetCSCSrcVideo(pSRC, pOUT)) return FALSE;	// set source CSC
	if (!MDINAPI_SetCSCOutVideo(pSRC, pOUT)) return FALSE;	// set output CSC

	if (!MDINAPI_SetWindowSrcVideo(pSRC)) return FALSE;		// set in-out scale
	if (!MDINAPI_SetHDecimateVideo(pSRC)) return FALSE;		// set H-decimation
//	if (!MDINAPI_SetHDecimateVideo(pSRC, pOUT)) return FALSE;	// set H-decimation
	if (!MDINAPI_SetWindowOutVideo(pOUT)) return FALSE;
//	if (!MDINAPI_SetFrontNRFilterInHD())	 return FALSE;		// set FNR filter in HD
	if (!MDINAPI_SetFrontNRFilterInHD(pSRC))	 return FALSE;		// set FNR filter in HD
	if (!MDINAPI_SetMFCFilterCoef(pOUT))  return FALSE;		// set MFC filter

	if (!MDINAPI_SetMemPrecision(pSRC, pOUT)) return FALSE;	// set memory precison
	if (!MDINAPI_SetDeinterMode(pSRC)) return FALSE;		// set deinterlacer regs

	return MDINAPI_ResetOutSync(100);
}

BOOL HDMITransmitter::MDINAPI_EnableOutPortPin(MDIN_PIN_OUT_t port, BOOL OnOff){
	switch (port) {
		case MDIN_PIN_ALL_OUT:
		return MDIN_RegField(0x01E, 1, 4, (WORD)((OnOff)? 0:0x0f));

		default:
		return MDIN_RegField(0x01E, (WORD)(port), 1, (WORD)((OnOff)? 0:1));
	}
	return TRUE;
}

BOOL HDMITransmitter::MDINAPI_EnableDACPortOut(BOOL OnOff){
	return MDIN_RegField(0x01E, 0, 1, (WORD)((OnOff)? 0:1));
}

BOOL HDMITransmitter::SetOutputScreen(int iWidth, int iHeight){
	if(iWidth == 800 && iHeight == 480){
		stOutVideoInfo.frmt = VIDOUT_800x480p60;
	}else
	if(iWidth == 1920 && iHeight == 1080){
		//stOutVideoInfo.frmt = VIDOUT_1920x1080p60;
		stOutVideoInfo.frmt = VIDOUT_1920x1080pRB;	// reduced blank
	}
	return TRUE;
}

BOOL HDMITransmitter::Initialize(void){
	printf("Please wait a few seconds for MDIN-240 init.\n");
	printf("MDIN-240 device ID: 0x%x (normal value:0x83)\n", MDINAPI_GetDeviceID());

	printf("MDIN : setup Memory...\n");
	MDINAPI_SetMemory(MDIN_USE_BIG_ENDIAN, MDIN_LVL2_CLK_SDRAM, 0);
	MDINAPI_DisplayOutWindow(OFF);

	printf("MDIN : setup PLL...\n");
	MDINAPI_SetVCLKPLLSource(MDIN_PLL_SOURCE_XTAL);	// set PLL source
	MDINAPI_EnableClockDrive(MDIN_CLK_DRV_ALL, ON);
	MDINAPI_SetInDataMapMode(MDIN_IN_DATA_MAP3);

	// setup deinterlacer
	printf("MDIN : setup deinterlacer...\n");
	MDINAPI_LoadDeintData();				// load default deinterlacer data
	MDINAPI_SetDeintYMode(MDIN_DEINT_MODE_ADAPTIVE);
	MDINAPI_SetDeintFilmMode(MDIN_DEINT_FILM_ALL);
	MDINAPI_EnableDeintNR(TRUE);
	MDINAPI_EnableDeintCCS(TRUE);
	MDINAPI_SetSurfaceFilterCoef(NULL);		// set default surface filter coef

	// setup enhancement
	printf("MDIN : setup enhancement...\n");
	MDINAPI_SetPeakingFilterCoef(NULL);		// set default peaking filter coef
	MDINAPI_SetColorEnhFilterCoef(NULL);	// set default color enhancer coef
	MDINAPI_SetFrontNRYFilterCoef(NULL);	// set default frontNR Y filter coef
	MDINAPI_SetFrontNRCFilterCoef(NULL);	// set default frontNR C filter coef
	MDINAPI_SetBWExtensionPoint(NULL, NULL); // set default B/W Extension point

	MDINAPI_EnableLTI(OFF);					// set LTI off
	MDINAPI_EnableCTICb(OFF);				// set CTI(Cb) off
	MDINAPI_EnableCTICr(OFF);				// set CTI(Cr) off
	MDINAPI_SetPeakingFilterGain(12);		// set peaking gain
	MDINAPI_EnablePeakingFilter(ON);		// set peaking on
	MDINAPI_EnableFrontNRYFilter(OFF);		// set frontNR Y off
	MDINAPI_EnableFrontNRCFilter(OFF);		// set frontNR C off
	MDINAPI_EnableMedianFilter(OFF);		// set median off
	MDINAPI_EnableBWExtension(OFF);			// set B/W extension off

	// video processing
	printf("MDIN : setup video processing...\n");
	stSrcVideoClock.delay = 0;
	stSrcVideoClock.invert = TRUE;

	// video source setting
	stSrcVideoInfo.frmt = VIDSRC_800x480pANY;	// @todo 이곳에서 소스 해상도를 결정한다.
	stSrcVideoInfo.mode = MDIN_SRC_RGB444_A8;
	stSrcVideoInfo.stAUX.attb = MDIN_FIELDID_INPUT|MDIN_HIGH_IS_TOPFLD;
	stSrcVideoInfo.stAUX.pCLK = &stSrcVideoClock;

	stSrcVideoInfo.stCLIP.posiH = 0;		// set H/V-offset
	stSrcVideoInfo.stCLIP.posiV = 0;

	// video output setting 
	//stOutVideoInfo.frmt = VIDOUT_800x480p60;	// @todo 이곳에서 출력 해상도를 결정한다.
	//stOutVideoInfo.frmt = VIDOUT_1920x1080pRB;	// @todo 이곳에서 출력 해상도를 결정한다.
	stOutVideoInfo.mode = MDIN_OUT_RGB444_8;

	// 800 x 480 output size setting 
	{
		MDIN_VIDEO_WINDOW temp;
		temp.sizeH = 800;
		temp.sizeV = 480;
		temp.posiH = 0;
		temp.posiV = 0;

		stOutVideoInfo.stVIEW = temp;
		stOutVideoInfo.stOUT  = temp;
	}

	stOutVideoInfo.brightness = 128;		// set MDIN picture factor
	stOutVideoInfo.contrast = 128;
	stOutVideoInfo.saturation = 128;
	stOutVideoInfo.hue = 128;

	stOutVideoInfo.offsetG = 128;			// set MDIN offset factor
	stOutVideoInfo.offsetB = 128;
	stOutVideoInfo.offsetR = 128;

	OutVideoClockInv.invert= OFF;
	OutVideoClockInv.delay= 0;	// 0:no delay, 1:1ns, 2:2ns and 3:3ns
	stOutVideoInfo.stAUX.pOutVCLK = &OutVideoClockInv;// 2009,Oct, 15 for output clock delay/invert control

	MDINAPI_VideoProcess(&stSrcVideoInfo, &stOutVideoInfo);
	MDINAPI_EnableOutPortPin(MDIN_PIN_ALL_OUT, ON);
	MDINAPI_EnableDACPortOut(ON); 			// enable DAC operation

	//--------------------------------------------------
	// HDMI setting
	printf("MDIN : setup HDMI...\n");
	MDVI1ST_RegWrite(0x05, 0x00);			// for DVI output
	MDVI1ST_RegWrite(0x08, 0x37);
	MDVI1ST_RegWrite(0x0f, 0x00);
	MDVI1ST_RegWrite(0x33, 0x00);
	MDVI1ST_RegWrite(0x48, 0x00);
	MDVI1ST_RegWrite(0x4a, 0x00);
	MDVI1ST_RegWrite(0x82, 0x20);
	MDVI2ND_RegWrite(0x14, 0x00);
	MDVI2ND_RegWrite(0x2f, 0x00);

	//--------------------------------------------------
    // Display On setting
	MDINAPI_DisplayOutWindow(ON);			// display ON

	printf("MDIN-240 initialization finished.\n");

	return TRUE;
}

BOOL HDMITransmitter::MDIN_MultiWrite(WORD rAddr, PWORD pBuff, WORD bytes){
	return m_I2C.Write16(I2C_MDINAPI_ADDR, rAddr, pBuff, bytes>>1);
}

BOOL HDMITransmitter::MDIN_MultiRead(WORD rAddr, PWORD pBuff, WORD bytes){
	return m_I2C.Read16(I2C_MDINAPI_ADDR, rAddr, pBuff, bytes>>1);
}

BOOL HDMITransmitter::MDIN_RegWrite(WORD rAddr, WORD wData){
	return MDIN_MultiWrite(rAddr, &wData, 2);
}

BOOL HDMITransmitter::MDIN_RegRead(WORD rAddr, PWORD rData){
	return MDIN_MultiRead(rAddr, rData, 2);
}

BOOL HDMITransmitter::MDIN_RegField(WORD rAddr, WORD bPos, WORD bCnt, WORD bData){
	WORD temp;
	if (bPos>15||bCnt==0||bCnt>16||(bPos+bCnt)>16) return FALSE;
	if (!MDIN_RegRead(rAddr, &temp)) return FALSE;
	bCnt = ~(0xffff<<bCnt);
	temp &= ~(bCnt<<bPos);
	temp |= ((bData&bCnt)<<bPos);
	return MDIN_RegWrite(rAddr, temp);
}

BOOL HDMITransmitter::MDVI1ST_MultiWrite(BYTE rAddr, PBYTE pBuff, WORD bytes){
	return m_I2C.Write8(I2C_MDVI1ST_ADDR, rAddr, pBuff, bytes);
}

BOOL HDMITransmitter::MDVI1ST_MultiRead(BYTE rAddr, PBYTE pBuff, WORD bytes){
	return m_I2C.Read8(I2C_MDVI1ST_ADDR, rAddr, pBuff, bytes);
}

BOOL HDMITransmitter::MDVI1ST_RegWrite(BYTE rAddr, BYTE wData){
	return MDVI1ST_MultiWrite(rAddr, &wData, 1);
}

BOOL HDMITransmitter::MDVI1ST_RegRead(BYTE rAddr, PBYTE rData){
	return MDVI1ST_MultiRead(rAddr, rData, 1);
}

BOOL HDMITransmitter::MDVI1ST_RegField(BYTE rAddr, WORD bPos, WORD bCnt, BYTE bData){
	BYTE temp;
	if (bPos>7||bCnt==0||bCnt>8||(bPos+bCnt)>8) return FALSE;
	if (MDVI1ST_RegRead(rAddr, &temp)) return FALSE;
	bCnt = ~(0xff<<bCnt);
	temp &= ~(bCnt<<bPos);
	temp |= ((bData&bCnt)<<bPos);
	return MDVI1ST_RegWrite(rAddr, temp);
}


BOOL HDMITransmitter::MDVI2ND_MultiWrite(BYTE rAddr, PBYTE pBuff, WORD bytes){
	return m_I2C.Write8(I2C_MDVI2ND_ADDR, rAddr, pBuff, bytes);
}

BOOL HDMITransmitter::MDVI2ND_MultiRead(BYTE rAddr, PBYTE pBuff, WORD bytes){
	return m_I2C.Read8(I2C_MDVI2ND_ADDR, rAddr, pBuff, bytes);
}

BOOL HDMITransmitter::MDVI2ND_RegWrite(BYTE rAddr, BYTE wData){
	return MDVI2ND_MultiWrite(rAddr, &wData, 1);
}

BOOL HDMITransmitter::MDVI2ND_RegRead(BYTE rAddr, PBYTE rData){
	return MDVI2ND_MultiRead(rAddr, rData, 1);
}

BOOL HDMITransmitter::MDVI2ND_RegField(BYTE rAddr, WORD bPos, WORD bCnt, BYTE bData){
	BYTE temp;
	if (bPos>7||bCnt==0||bCnt>8||(bPos+bCnt)>8) return FALSE;
	if (MDVI2ND_RegRead(rAddr, &temp)) return FALSE;
	bCnt = ~(0xff<<bCnt);
	temp &= ~(bCnt<<bPos);
	temp |= ((bData&bCnt)<<bPos);
	return MDVI2ND_RegWrite(rAddr, temp);
}