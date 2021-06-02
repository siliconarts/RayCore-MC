//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MDINAPI.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MDINAPI_H__
#define		__MDINAPI_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#ifndef		__MDINTYPE_H__
#include	"mdintype.h"
#endif

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

#define SYSTEM_USE_MDIN240

// typedef	enum {
// 	MDIN_NO_ERROR = 0,		// no error
// 	MDIN_I2C_ERROR,			// i2c error (i2c_busy, i2c_no_ack)
// 	MDIN_INVALID_FORMAT,	// invalid src/out video format
// 	MDIN_INVALID_PARAM		// invalid parameters
// 
// }	MDIN_ERROR_t;

typedef	enum {
	MDIN_IN_DATA_MAP0 = 0,	// input data map mode 0
	MDIN_IN_DATA_MAP2 = 2,	// input data map mode 2
	MDIN_IN_DATA_MAP3 = 3	// input data map mode 3

}	MDIN_IN_DATA_MAP_t;

typedef	enum {
	MDIN_IRQ_VSYNC_PULSE = 0,	// enable/disable IRQ(Vertical sync pulse)
	MDIN_IRQ_TIMER,				// enable/disable IRQ(Timer)
	MDIN_IRQ_MFC_OVERRUN,		// enable/disable IRQ(MFC FIFO overrun)
	MDIN_IRQ_IN_OVERRUN = 4,	// enable/disable IRQ(Input FIFO overrun)
	MDIN_IRQ_IN_SYNC_DET = 6,	// enable/disable IRQ(Input port sync detect)
#ifdef SYSTEM_USE_MDIN240
	MDIN_IRQ_HDMI_TX,			// enable/disable IRQ(HDMI transmitter)
#if SYSTEM_USE_HDMICEC==1
	MDIN_IRQ_HDMI_CEC = 9,		// enable/disable IRQ(HDMI CEC)
#endif
#endif
	MDIN_IRQ_IN_SYNC_CHG = 8,	// enable/disable IRQ(Input port sync change)
	MDIN_IRQ_IN_SYNC_LOST,		// enable/disable IRQ(Input port sync lost)
	MDIN_IRQ_FRAME_WR_SKIP,		// enable/disable IRQ(Frame write skip)
	MDIN_IRQ_FRAME_RD_RPT,		// enable/disable IRQ(Frame read repeat)
	MDIN_IRQ_OSD_UNDERFLOW = 15	// enable/disable IRQ(OSD FIFO underflow)

}	MDIN_IRQ_t;

#define		MDIN_IRQ_LEVEL_SENSE		1		// level sense
#define		MDIN_IRQ_EDGE_SENSE			0		// edge sense

#define		MDIN_INT_OUT_ACT_HIGH		1		// active high output
#define		MDIN_INT_OUT_ACT_LOW		0		// active low output

typedef	enum {
	MDIN_PIN_DATA_OUT = 1,	// enable/disable DATA_OUT(23:0) port
	MDIN_PIN_VCLK_OUT,		// enable/disable VCLK_OUT & VCLK_OUT_B port
	MDIN_PIN_SYNC_OUT,		// enable/disable HSYNC_OUT & VSYNC_OUT port
	MDIN_PIN_DE_OUT,		// enable/disable DE_OUT port
	MDIN_PIN_ALL_OUT		// enable/disable all above port

}	MDIN_PIN_OUT_t;

typedef	struct
{
	BYTE	endian;			// Big-Endian or Little_Endian
	BYTE	drive;			// level1 or level2 drivability of signals
	BYTE	delay;			// delay of SDRAM_OUT_CLK (0=default, 1, 2, 3)

}	MDIN_MEMORY_CONFIG, *PMDIN_MEMORY_CONFIG;

#define		MDIN_USE_BIG_ENDIAN			0		// the access of SDRAM memory
#define		MDIN_USE_LITTLE_ENDIAN		1

#define		CPU_ALIGN_BIG_ENDIAN		0

#define		MDIN_LVL2_CTRL_SDRAM		0x04	// drivability of SDRAM ctrl line
#define		MDIN_LVL1_CTRL_SDRAM		0x00
#define		MDIN_LVL2_DATA_SDRAM		0x02	// drivability of SDRAM data line
#define		MDIN_LVL1_DATA_SDRAM		0x00
#define		MDIN_LVL2_CLK_SDRAM			0x01	// drivability of SDRAM clk line
#define		MDIN_LVL1_CLK_SDRAM			0x00

typedef	enum {
	MDIN_PLL_SOURCE_CLKB = 0,	// CLKB is internal PLL source for video clock
	MDIN_PLL_SOURCE_XTAL,		// XTAL is internal PLL source for video clock
	MDIN_PLL_SOURCE_CLKA,		// CLKA is internal PLL source for video clock
	MDIN_PLL_SOURCE_HACT		// HACT is internal PLL source for video clock

}	MDIN_PLL_SOURCE_t;

typedef	enum {
	MDIN_CLK_DRV_VCLK_2X = 0,	// enable/disable vclk_2x(double rate video clock)
	MDIN_CLK_DRV_VCLK_OUT,		// enable/disable vclk_out(video output clock)
	MDIN_CLK_DRV_VCLK,			// enable/disable vclk(video clock)
	MDIN_CLK_DRV_DAC,			// enable/disable dac_clk(DAC clock)
	MDIN_CLK_DRV_MCLK,			// enable/disable mclk(memory clock)
	MDIN_CLK_DRV_IN_CLK,		// enable/disable clk_in & clk_m(input clock)
#ifdef SYSTEM_USE_MDIN240
	MDIN_CLK_DRV_HDMI_ICLK,		// enable/disable hdmi_iclk(video data clock)
	MDIN_CLK_DRV_HDMI_VCLK,		// enable/disable hdmi_vclk(TMDS clock)
	MDIN_CLK_DRV_HDMI_OCLK,		// enable/disable hdmi_oclk(DDC master clock)
	MDIN_CLK_DRV_HDMI_ASCLK,	// enable/disable hdmi_asclk(S/PDIF capture clock)
	MDIN_CLK_DRV_HDMI_SCK,		// enable/disable hdmi_sck(audio serial clock)
	MDIN_CLK_DRV_HDMI_MCLK,		// enable/disable hdmi_mclk(audio master clock)
#endif
	MDIN_CLK_DRV_SDRAM = 14,	// enable/disable clock of the internal SDRAM
	MDIN_CLK_DRV_ALL			// enable/disable all above clock

}	MDIN_CLK_DRV_t;

typedef	struct
{
	BYTE	invert;		// TRUE inverts clock, FALSE does not invert clock
	BYTE	delay;		// amount of clock delay (0=default, 1, 2, 3)

}	MDIN_VIDEO_CLOCK, *PMDIN_VIDEO_CLOCK;

typedef	struct
{
	WORD	sizeH;		// Horizontal size of video area
	WORD	sizeV;		// Vertical size of video area
	WORD	posiH;		// Horizontal position of the top-left corner of video area
	WORD	posiV;		// Vertical position of the top-left corner of video area

}	MDIN_VIDEO_WINDOW, *PMDIN_VIDEO_WINDOW;

typedef	struct
{
	WORD	coef[9];		// CSC coefficient 0~8
	WORD	inBias[3];		// added to G/Y,B/Cb,R/Cr channel before CSC
	WORD	outBias[3];		// added to G/Y,B/Cb,R/Cr channel after CSC

}	MDIN_CSCCTRL_DATA, *PMDIN_CSCCTRL_DATA;

typedef	struct
{
	WORD	loPhase[64];	// 2-tab lower 32-phase
	WORD	upPhase[64];	// 2-tab upper 32-phase

}	MDIN_MFCFILT_COEF, *PMDIN_MFCFILT_COEF;

typedef	struct
{
	BYTE	attb;		// CbCr swap, FieldID input, FieldID polarity, Composite Hsync

	BYTE	delayYC;	// adjust the timing difference between Y and C
	PMDIN_VIDEO_CLOCK	pCLK;		// invert & delay of CLK_A or CLK_B input clock
	PMDIN_CSCCTRL_DATA	pCSC;		// input CSC control

}	MDIN_SRCVIDEO_AUX, *PMDIN_SRCVIDEO_AUX;

#define		MDIN_SRC_CbCrSWAP_ON		0x08	// swap the ordering of chominance data
#define		MDIN_SRC_CbCrSWAP_OFF		0x00	// maintain the ordering of chominance data

#define		MDIN_FIELDID_INPUT			0x04	// FIELDID input is used as a field-id signal
#define		MDIN_FIELDID_BYPASS			0x00	// FIELDID is generated from H/V active signal

#define		MDIN_HIGH_IS_TOPFLD			0x02	// high level of field-id represents top field
#define		MDIN_LOW_IS_TOPFLD			0x00	// low level of field-id represents top field

#define		MDIN_SRC_HACT_CSYNC			0x01	// HACTIVE_A or B input is composite sync
#define		MDIN_SRC_HACT_HSYNC			0x00	// HACTIVE_A or B input is Hsync

typedef	enum {
	VIDSRC_720x480i60 = 0,		// 720x480i 60Hz
	VIDSRC_720x576i50,			// 720x576i 50Hz
	VIDSRC_720x480p60,			// 720x480p 60Hz
	VIDSRC_720x576p50,			// 720x576p 50Hz
	VIDSRC_1280x720p60,			// 1280x720p 60Hz
	VIDSRC_1280x720p50,			// 1280x720p 50Hz
	VIDSRC_1920x1080p30,		// 1920x1080p 30Hz
	VIDSRC_1920x1080p25,		// 1920x1080p 25Hz
	VIDSRC_1920x1080i60,		// 1920x1080i 60Hz
	VIDSRC_1920x1080i50,		// 1920x1080i 50Hz
	VIDSRC_640x480pANY,			// 640x480p anyHz
	VIDSRC_800x480pANY,			// 800x480p anyHz
	VIDSRC_800x600pANY,			// 800x600p anyHz
	VIDSRC_1024x768pANY,		// 1024x768p anyHz
	VIDSRC_1280x1024pANY,		// 1280x1024p anyHz
	VIDSRC_FORMAT_END			// source format end

}	MDIN_SRCVIDEO_FORMAT_t;

typedef	enum {
	MDIN_SRC_RGB444_A8 = 0,		// RGB 4:4:4 8-bit with separate sync on portA
	MDIN_SRC_YUV444_A8,			// YCbCr 4:4:4 8-bit with separate sync on portA
	MDIN_SRC_EMB422_A8,			// YCbCr 4:2:2 8-bit with embedded sync on portA
	MDIN_SRC_SEP422_A8,			// YCbCr 4:2:2 8-bit with separate sync on portA
	MDIN_SRC_EMB422_A10,		// YCbCr 4:2:2 10-bit with embedded sync on portA
	MDIN_SRC_SEP422_A10,		// YCbCr 4:2:2 10-bit with separate sync on portA

	MDIN_SRC_MUX656_A8,			// Y/C-mux 4:2:2 8-bit with embedded sync on portA
	MDIN_SRC_MUX656_A10,		// Y/C-mux 4:2:2 10-bit with embedded sync on portA
	MDIN_SRC_MUX656_B8,			// Y/C-mux 4:2:2 8-bit with embedded sync on portB
	MDIN_SRC_MUX656_B10			// Y/C-mux 4:2:2 10-bit with embedded sync on portB

}	MDIN_SRCPORT_MODE_t;

typedef	enum {
	NTSC_SYSTEM = 0,	// NTSC Mode
	PAL_SYSTEM
};
// -----------------------------------------------------------------------------

typedef	struct
{
	BYTE	frmt;		// video format of input port (H/V Polarity & ScanType)
	BYTE	mode;		// video mode of input port (one of MDIN_INPORT_MODE)
	MDIN_SRCVIDEO_AUX	stAUX;		// input auxiliary
	MDIN_VIDEO_WINDOW	stCLIP;		// input clip window

	WORD	total;		// Horizontal total size of input port
	WORD	attb;		// H/V sync polarity, scan type
	MDIN_VIDEO_WINDOW	stSRC;		// input video window

}	MDIN_SRCVIDEO_INFO, *PMDIN_SRCVIDEO_INFO;

#define		MDIN_USE_INPORT_A			0x8000	// input port A is selected for input video
#define		MDIN_USE_INPORT_B			0x0000	// input port B is selected for input video

#define		MDIN_QUALITY_HD				0x4000	// HD quality
#define		MDIN_QUALITY_SD				0x0000	// SD quality

#define		MDIN_POSITIVE_HSYNC			0x0020	// low level of Hsync contains active data region
#define		MDIN_NEGATIVE_HSYNC			0x0000	// high level of Hsync contains active data region

#define		MDIN_POSITIVE_VSYNC			0x0010	// low level of Vsync contains active data region
#define		MDIN_NEGATIVE_VSYNC			0x0000	// high level of Vsync contains active data region

#define		MDIN_SCANTYPE_PROG			0x0008	// progressive scan
#define		MDIN_SCANTYPE_INTR			0x0000	// interlaced scan

#define		MDIN_PRECISION_8			0x0004	// 8-bit precision per color component
#define		MDIN_PRECISION_10			0x0000	// 10-bit precision per color component

#define		MDIN_PIXELSIZE_422			0x0002	// pixel-size is 4:2:2 format
#define		MDIN_PIXELSIZE_444			0x0000	// pixel-size is 4:4:4 format

#define		MDIN_COLORSPACE_YUV			0x0001	// color space is YCbCr domain
#define		MDIN_COLORSPACE_RGB			0x0000	// color space is RGB domain

typedef	struct
{
	WORD	posH;		// the position of horizontal reference pulse
	WORD	posV;		// the position of vertical reference pulse
	WORD	posVB;		// the position of vertical reference pulse for bottom field

	WORD	totHS;		// the horizontal total size of output video
	WORD	bgnHA;		// the start position of horizontal active output
	WORD	endHA;		// the end position of horizontal active output
	WORD	bgnHS;		// the start position of horizontal sync output
	WORD	endHS;		// the end position of horizontal sync output

	WORD	totVS;		// the vertical total size of output video
	WORD	bgnVA;		// the start position of vertical active output
	WORD	endVA;		// the end position of vertical active output
	WORD	bgnVS;		// the start position of vertical sync output
	WORD	endVS;		// the end position of vertical sync output

	WORD	bgnVAB;		// the start position of Vact for the bottom field
	WORD	endVAB;		// the end position of Vact for the bottom field
	WORD	bgnVSB;		// the start position of Vsync for the bottom field
	WORD	endVSB;		// the end position of Vsync for the bottom field
	WORD	posFLD;		// the transition position of Vsync for the bottom field

	WORD	vclkP;		// Pre-divider value of the internal PLL for video clock
	WORD	vclkM;		// Post-divider value of the internal PLL for video clock
	WORD	vclkS;		// Post-scaler value of the internal PLL for video clock

}	MDIN_OUTSYNC_INFO, *PMDIN_OUTSYNC_INFO;

typedef	struct
{
	BYTE	iod;		// TMDS output current control
	BYTE	iorv;		// TMDS output de-emphasis control

}	MDIN_OUTTMDS_CTRL, *PMDIN_OUTTMDS_CTRL;

typedef	struct
{
	WORD	ctrl[37];	// a special register set to control the operation of the internal DAC

}	MDIN_DACSYNC_DATA, *PMDIN_DACSYNC_DATA;

typedef	struct
{
	WORD	attb;		// DIG/DAC CbCr Swap, DE output, H/V active polarity, H/V sync outmode

	PMDIN_VIDEO_CLOCK	pOutVCLK;		// invert & delay of output clock (VCLK_OUT)
	PMDIN_VIDEO_CLOCK	pOutVCLKB;		// invert & delay of output clock (VCLK_OUT_B)
	PMDIN_VIDEO_CLOCK	pIntVCLK;		// invert & delay of internal clock (vclk)
	PMDIN_VIDEO_CLOCK	pIntVCLK2X;		// invert & delay of internal clock (vclk_2x)
	PMDIN_VIDEO_CLOCK	pIntDACCLK;		// invert & delay of internal clock (dac_clk)
	PMDIN_CSCCTRL_DATA	pCSC;			// output CSC control
	PMDIN_MFCFILT_COEF	pMFCHY;			// MFCHY filter coefficient
	PMDIN_MFCFILT_COEF	pMFCHC;			// MFCHC filter coefficient
	PMDIN_MFCFILT_COEF	pMFCVY;			// MFCVY filter coefficient
	PMDIN_MFCFILT_COEF	pMFCVC;			// MFCVC filter coefficient

}	MDIN_OUTVIDEO_AUX, *PMDIN_OUTVIDEO_AUX;

#define		MDIN_DIG_CbCrSWAP_ON		0x8000	// CbCr are swapped on digital output
#define		MDIN_DIG_CbCrSWAP_OFF		0x0000	// CbCr are not swapped on digital output

#define		MDIN_DAC_PbPrSWAP_ON		0x0200	// PbPr are swapped on DAC output
#define		MDIN_DAC_PbPrSWAP_OFF		0x0000	// PbPr are not swapped on DAC output

#define		MDIN_DE_OUT_IS_HACT			0x0020	// DE_OUT pin is Hactive signal
#define		MDIN_DE_OUT_IS_COMP			0x0010	// DE_OUT pin is composite sync
#define		MDIN_DE_OUT_IS_DE			0x0000	// DE_OUT pin is data enable signal
#define		MDIN_NEGATIVE_HACT			0x0008	// Hactive data region is low level
#define		MDIN_POSITIVE_HACT			0x0000	// Hactive data region is high level
#define		MDIN_NEGATIVE_VACT			0x0004	// Vactive data region is low level
#define		MDIN_POSITIVE_VACT			0x0000	// Vactive data region is high level
#define		MDIN_HSYNC_OUT_HACT			0x0002	// HSYNC_OUT pin is Hactive signal
#define		MDIN_HSYNC_OUT_HSYNC		0x0000	// HSYNC_OUT pin is Hsync signal
#define		MDIN_VSYNC_OUT_VACT			0x0001	// VSYNC_OUT pin is Vactive signal
#define		MDIN_VSYNC_OUT_VSYNC		0x0000	// VSYNC_OUT pin is Vsync signal

typedef	enum {
//	VIDOUT_1440x480i60 = 0,		// 1440x480i 60Hz
//	VIDOUT_1440x576i50,			// 1440x576i 50Hz
	VIDOUT_720x480i60 = 0,		// 720x480i 60Hz
	VIDOUT_720x576i50,			// 720x576i 50Hz
	VIDOUT_720x480p60,			// 720x480p 60Hz
	VIDOUT_720x576p50,			// 720x576p 50Hz
	VIDOUT_1280x720p60,			// 1280x720p 60Hz
	VIDOUT_1280x720p50,			// 1280x720p 50Hz
	VIDOUT_1920x1080i60,		// 1920x1080i 60Hz
	VIDOUT_1920x1080i50,		// 1920x1080i 50Hz
	VIDOUT_1920x1080p60,		// 1920x1080p 60Hz
	VIDOUT_1920x1080p50,		// 1920x1080p 50Hz
	VIDOUT_640x480p60,			// 640x480p 60Hz
	VIDOUT_640x480p75,			// 640x480p 75Hz
	VIDOUT_800x480p60,			// 800x480p 60Hz
	VIDOUT_800x600p60,			// 800x600p 60Hz
	VIDOUT_800x600p75,			// 800x600p 75Hz
	VIDOUT_1024x768p60,			// 1024x768p 60Hz
	VIDOUT_1024x768p75,			// 1024x768p 75Hz
	VIDOUT_1280x1024p60,		// 1280x1024p 60Hz
	VIDOUT_1280x1024p75,		// 1280x1024p 75Hz

	VIDOUT_1360x768p60,			// 1360x768p 60Hz
	VIDOUT_1440x900p60,			// 1440x900p 60Hz
	VIDOUT_1440x900p75,			// 1440x900p 75Hz
	VIDOUT_1680x1050p60,		// 1680x1050p 60Hz
	VIDOUT_1600x1200p60,		// 1600x1200p 60Hz

	VIDOUT_1680x1050pRB,		// 1680x1050pRB 60Hz
	VIDOUT_1920x1080pRB,		// 1920x1080pRB 60Hz
	VIDOUT_1920x1200pRB,		// 1920x1200pRB 60Hz
	VIDOUT_FORMAT_END			// output format end

}	MDIN_OUTVIDEO_FORMAT_t;

typedef	enum {
	MDIN_OUT_RGB444_8 = 0,		// RGB 4:4:4 8-bit with separate sync on digital out port
	MDIN_OUT_YUV444_8,			// YCbCr 4:4:4 8-bit with separate sync on digital out port
	MDIN_OUT_EMB422_8,			// YCbCr 4:2:2 8-bit with embedded sync on digital out port
	MDIN_OUT_SEP422_8,			// YCbCr 4:2:2 8-bit with separate sync on digital out port
	MDIN_OUT_EMB422_10,			// YCbCr 4:2:2 10-bit with embedded sync on digital out port
	MDIN_OUT_SEP422_10,			// YCbCr 4:2:2 10-bit with separate sync on digital out port
	MDIN_OUT_MUX656_8,			// Y/C-mux 4:2:2 8-bit with embedded sync on digital out port
	MDIN_OUT_MUX656_10			// Y/C-mux 4:2:2 10-bit with embedded sync on digital out port

}	MDIN_OUTPORT_MODE_t;

typedef	struct
{
	BYTE	frmt;		// video format of output port (H/V Polarity & ScanType & FreeRun)
	BYTE	mode;		// video mode of output port (one of MDIN_OUTPORT_MODE)
	MDIN_OUTVIDEO_AUX	stAUX;		// output auxiliary
	MDIN_VIDEO_WINDOW	stZOOM;		// output zoom window
	MDIN_VIDEO_WINDOW	stVIEW;		// output view window

	BYTE	brightness;		// default = 128, range: from 0 to 255
	BYTE	contrast;		// default = 128, range: from 0 to 255
	BYTE	saturation;		// default = 128, range: from 0 to 255
	BYTE	hue;			// default = 128, range: from 0 to 255

	BYTE	offsetG;		// default = 128, range: from 0 to 255
	BYTE	offsetB;		// default = 128, range: from 0 to 255
	BYTE	offsetR;		// default = 128, range: from 0 to 255
	BYTE	dummy;			// dummy byte for 16-bit alignment

	WORD	attb;		// free/lock, sync polarity, scan type
	MDIN_VIDEO_WINDOW	stOUT;		// output video window
	MDIN_OUTSYNC_INFO	stSYNC;		// output sync info
	MDIN_DACSYNC_DATA	stDAC;		// output DAC data
	MDIN_OUTTMDS_CTRL	stTMDS;		// output TMDS ctrl

}	MDIN_OUTVIDEO_INFO, *PMDIN_OUTVIDEO_INFO;

#define		MDIN_SYNC_FREERUN			0x0100	// the out-sync runs freely regardless of in-sync
#define		MDIN_SYNC_FRMLOCK			0x0000	// the out-sync is frame-locked to the in-sync

typedef enum {
	MDIN_DE_OUT_HACTIVE = 0,		// H-active signal is output on DE_OUT pin
	MDIN_DE_OUT_COMPOSITE,			// composite sync signal is output on DE_OUT pin
	MDIN_DE_OUT_DATAENABLE			// 2-D active signal is output on DE_OUT pin

}	MDIN_DE_OUT_t;

typedef	struct
{
	BYTE	attb;		// DN/HD/NR
	BYTE	useDN;		// DN attribute
	WORD	useHD;		// HD attribute
	BYTE	useNR;		// NR attribute

}	MDIN_DEINT_CTRL, *PMDIN_DEINT_CTRL;

#define		MDIN_DEINT_FULL_HD			0x10	// enable INTRA
#define		MDIN_DEINT_DECI_HD			0x08	// enable HDECI
#define		MDIN_DEINT_INTR_DN			0x04	// enable CCS
#define		MDIN_DEINT_MODE_HD			0x02	// enable HD
#define		MDIN_DEINT_3DNR_ON			0x01	// enable 3D-NR

typedef	enum {
	MDIN_DEINT_MODE_NONSTILL = 0,	// adaptive mode : combination of intra and inter
	MDIN_DEINT_MODE_ADAPTIVE,		// adaptive mode : non-motion detect
	MDIN_DEINT_MODE_INTRA_ONLY,		// only intra-field interpolation mode
	MDIN_DEINT_MODE_INTER_ONLY		// only inter-field interpolation mode

}	MDIN_DEINT_MODE_t;

typedef enum {
	MDIN_DEINT_FILM_DISABLE = 0,	// disables film mode
	MDIN_DEINT_FILM_32,				// enables 3:2 film mode only
	MDIN_DEINT_FILM_22,				// enables 2:2 film mode only
	MDIN_DEINT_FILM_ALL				// enables both 3:2 and 2:2 film modes

}	MDIN_DEINT_FILM_t;

typedef enum {
	MDIN_DEINT_FAST_DISABLE = 0,	// disables fsat mode
	MDIN_DEINT_FAST_NORMAL,			// enables normal fast mode
	MDIN_DEINT_FAST_VERY			// enables very fast mode

}	MDIN_DEINT_FAST_t;

typedef enum {
	MDIN_DEINT_CAPTION_DISABLE = 0,		// disables caption mode
	MDIN_DEINT_CAPTION_INTRA,			// intra mode only on caption area
	MDIN_DEINT_CAPTION_AUTO_INTRA,		// intra mode with auto detect
	MDIN_DEINT_CAPTION_AUTO_ADAPTIVE	// adaptive mode with auto detect

}	MDIN_DEINT_CAPTION_t;

typedef enum {
	MDIN_DEINT_EDGE_DISABLE = 0,	// vertical (90-deg) interpolation only
	MDIN_DEINT_EDGE_45,				// 90-deg and 45-deg interpolation
	MDIN_DEINT_EDGE_26,				// up to 26-deg interpolation
	MDIN_DEINT_EDGE_ALL = 7			// all angle interpolation

}	MDIN_DEINT_EDGE_t;

typedef enum {
	MDIN_DEINT_NR_DISABLE = 0,		// disables 3D NR mode
	MDIN_DEINT_NR_MIN,				// minimum level of 3D NR mode
	MDIN_DEINT_NR_TYP,				// typical level of 3D NR mode
	MDIN_DEINT_NR_MAX				// maximum level of 3D NR mode

}	MDIN_DEINT_NR_t;

typedef enum {
	MDIN_IN_TEST_DISABLE = 0,		// disable input test pattern
	MDIN_IN_TEST_WINDOW,			// window test pattern
	MDIN_IN_TEST_H_RAMP,			// horizontal ramp pattern
	MDIN_IN_TEST_V_RAMP,			// vertical ramp pattern
	MDIN_IN_TEST_H_GRAY,			// horizontal gray scale pattern
	MDIN_IN_TEST_V_GRAY,			// vertical gray scale pattern
	MDIN_IN_TEST_H_COLOR,			// horizontal color bar pattern
	MDIN_IN_TEST_V_COLOR			// vertical color bar pattern

}	MDIN_IN_TEST_t;

typedef enum {
	MDIN_OUT_TEST_DISABLE = 0,		// disable output test pattern
	MDIN_OUT_TEST_WHITE,			// white test pattern
	MDIN_OUT_TEST_CROSS,			// cross test pattern
	MDIN_OUT_TEST_HATCH,			// cross hatch pattern
	MDIN_OUT_TEST_COLOR,			// color bar pattern
	MDIN_OUT_TEST_GRAY,				// gray scale pattern
	MDIN_OUT_TEST_WINDOW,			// white window pattern
	MDIN_OUT_TEST_H_RAMP,			// horizontal ramp pattern
	MDIN_OUT_TEST_WH_RAMP,			// wide horizontal ramp pattern
	MDIN_OUT_TEST_WV_RAMP,			// wide vertical ramp pattern
	MDIN_OUT_TEST_DIAGONAL			// diagonal lines pattern

}	MDIN_OUT_TEST_t;

#endif	/* __MDINAPI_H__ */
