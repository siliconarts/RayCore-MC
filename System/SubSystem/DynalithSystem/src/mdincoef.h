//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MDINCOEF.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MDINCOEF_H__
#define		__MDINCOEF_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
// RGB to YCbCr for input CSC
static  MDIN_CSCCTRL_DATA MDIN_CscRGBtoYUV_SD_FullRange		= {
	 258,	// coef0
	  50,	// coef1
	 132,	// coef2
	-149,	// coef3
	 225,	// coef4
	 -76,	// coef5
	-188,	// coef6
	 -36,	// coef7
	 225,	// coef8
	   0,	// in_bias0
	   0,	// in_bias1
	   0,	// in_bias2
	  64,	// out_bias0
	 512,	// out_bias1
	 512	// out_bias2
};

static  MDIN_CSCCTRL_DATA MDIN_CscRGBtoYUV_SD_StdRange		= {
	 301,	// coef0    
	  58,	// coef1    
	 153,	// coef2    
	-174,	// coef3    
	 262,	// coef4    
	 -88,	// coef5    
	-219,	// coef6    
	 -42,	// coef7    
	 262,	// coef8    
	   0,	// in_bias0 
	   0,	// in_bias1 
	   0,	// in_bias2 
	   0,	// out_bias0
	 512,	// out_bias1
	 512	// out_bias2
};

static  MDIN_CSCCTRL_DATA MDIN_CscRGBtoYUV_HD_FullRange		= {
	 314,	// coef0    
	  32,	// coef1    
	  94,	// coef2    
	-173,	// coef3    
	 225,	// coef4    
	 -52,	// coef5    
	-204,	// coef6    
	 -20,	// coef7    
	 225,	// coef8    
	   0,	// in_bias0 
	   0,	// in_bias1 
	   0,	// in_bias2 
	  64,	// out_bias0
	 512,	// out_bias1
	 512	// out_bias2
};

static  MDIN_CSCCTRL_DATA MDIN_CscRGBtoYUV_HD_StdRange		= {
	 366,	// coef0    
	  37,	// coef1    
	 109,	// coef2    
	-202,	// coef3    
	 262,	// coef4    
	 -60,	// coef5    
	-238,	// coef6    
	 -24,	// coef7    
	 262,	// coef8    
	   0,	// in_bias0 
	   0,	// in_bias1 
	   0,	// in_bias2 
	   0,	// out_bias0
	 512,	// out_bias1
	 512	// out_bias2
};

// YCbCr to RGB for output CSC
static  MDIN_CSCCTRL_DATA MDIN_CscYUVtoRGB_SD_FullRange		= {
	 596,	// coef0    
	-200,	// coef1    
	-416,	// coef2    
	 596,	// coef3    
	1033,	// coef4    
	   0,	// coef5    
	 596,	// coef6    
	   0,	// coef7    
	 817,	// coef8    
	 -64,	// in_bias0 
	-512,	// in_bias1 
	-512,	// in_bias2 
	   0,	// out_bias0
	   0,	// out_bias1
	   0	// out_bias2
};
/*
static  MDIN_CSCCTRL_DATA MDIN_CscYUVtoRGB_SD_StdRange		= {
	 512,	// coef0    
	-172,	// coef1    
	-357,	// coef2    
	 512,	// coef3    
	 887,	// coef4    
	   0,	// coef5    
	 512,	// coef6    
	   0,	// coef7    
	 702,	// coef8    
	 -64,	// in_bias0 
	-512,	// in_bias1 
	-512,	// in_bias2 
	   0,	// out_bias0
	   0,	// out_bias1
	   0	// out_bias2
};
*/
static  MDIN_CSCCTRL_DATA MDIN_CscYUVtoRGB_HD_FullRange		= {
	 596,	// coef0    
	-109,	// coef1    
	-273,	// coef2    
	 596,	// coef3    
	1083,	// coef4    
	   0,	// coef5    
	 596,	// coef6    
	   0,	// coef7    
	 918,	// coef8    
	 -64,	// in_bias0 
	-512,	// in_bias1 
	-512,	// in_bias2 
	   0,	// out_bias0
	   0,	// out_bias1
	   0	// out_bias2
};
/*
static  MDIN_CSCCTRL_DATA MDIN_CscYUVtoRGB_HD_StdRange		= {
	 512,	// coef0    
	 -96,	// coef1    
	-239,	// coef2    
	 512,	// coef3    
	 950,	// coef4    
	   0,	// coef5    
	 512,	// coef6    
	   0,	// coef7    
	 804,	// coef8    
	   0,	// in_bias0 
	-512,	// in_bias1 
	-512,	// in_bias2 
	   0,	// out_bias0
	   0,	// out_bias1
	   0	// out_bias2
};

static  MDIN_CSCCTRL_DATA MDIN_CscBypass_FullRange		= {
	 596,	// coef0    
	   0,	// coef1    
	   0,	// coef2    
	   0,	// coef3    
	 596,	// coef4    
	   0,	// coef5    
	   0,	// coef6    
	   0,	// coef7    
	 596,	// coef8    
	 -64,	// in_bias0 
	-512,	// in_bias1 
	-512,	// in_bias2 
	   0,	// out_bias0
	 512,	// out_bias1
	 512	// out_bias2
};

static  MDIN_CSCCTRL_DATA MDIN_CscBypass_RemoveBlack_StdRange		= {
	 512,	// coef0    
	   0,	// coef1    
	   0,	// coef2    
	   0,	// coef3    
	 512,	// coef4    
	   0,	// coef5    
	   0,	// coef6    
	   0,	// coef7    
	 512,	// coef8    
	 -64,	// in_bias0 
	-512,	// in_bias1 
	-512,	// in_bias2 
	   0,	// out_bias0 (remove black level)
	 512,	// out_bias1
	 512	// out_bias2
};
*/
static  MDIN_CSCCTRL_DATA MDIN_CscBypass_StdRange		= {
	 512,	// coef0    
	   0,	// coef1    
	   0,	// coef2    
	   0,	// coef3    
	 512,	// coef4    
	   0,	// coef5    
	   0,	// coef6    
	   0,	// coef7    
	 512,	// coef8    
	 -64,	// in_bias0 
	-512,	// in_bias1 
	-512,	// in_bias2 
	  64,	// out_bias0
	 512,	// out_bias1
	 512	// out_bias2
};

static  MDIN_CSCCTRL_DATA MDIN_CscSDtoHD_StdRange		= {
	 512,	// coef0    
	   0,	// coef1    
	   0,	// coef2    
	   0,	// coef3    
	 522,	// coef4    
	  59,	// coef5    
	   0,	// coef6    
	  38,	// coef7    
	 525,	// coef8    
	 -64,	// in_bias0 
	-512,	// in_bias1 
	-512,	// in_bias2 
	  64,	// out_bias0
	 512,	// out_bias1
	 512	// out_bias2
};

static  MDIN_CSCCTRL_DATA MDIN_CscHDtoSD_StdRange		= {
	 512,	// coef0    
	  51,	// coef1    
	  98,	// coef2    
	   0,	// coef3    
	 507,	// coef4    
	 -56,	// coef5    4040
	   0,	// coef6    
	 -37,	// coef7   4059 
	 504,	// coef8    
	 -64,	// in_bias0 
	-512,	// in_bias1 
	-512,	// in_bias2 
	  64,	// out_bias0
	 512,	// out_bias1
	 512	// out_bias2
};

//--------------------------------------------------------------------------------------------------------------------------
// hue range -60~60, cos(hue) table for picture HUE control
static  WORD MDIN_CscCosHue[]		= {
	0x0200, 0x0207, 0x020e, 0x0216, 0x021d, 0x0224, 0x022b, 0x0232,
	0x0239, 0x0240, 0x0247, 0x024e, 0x0254, 0x025b, 0x0262, 0x0269,
	0x026f, 0x0276, 0x027d, 0x0283, 0x028a, 0x0290, 0x0297, 0x029d,
	0x02a3, 0x02a9, 0x02b0, 0x02b6, 0x02bc, 0x02c2, 0x02c8, 0x02ce,
	0x02d4, 0x02da, 0x02e0, 0x02e6, 0x02eb, 0x02f1, 0x02f7, 0x02fc,
	0x0302, 0x0307, 0x030d, 0x0312, 0x0318, 0x031d, 0x0322, 0x0327,
	0x032c, 0x0331, 0x0336, 0x033b, 0x0340, 0x0345, 0x034a, 0x034f,
	0x0353, 0x0358, 0x035d, 0x0361, 0x0366, 0x036a, 0x036e, 0x0373,
	0x0377, 0x037b, 0x037f, 0x0383, 0x0387, 0x038b, 0x038f, 0x0393,
	0x0396, 0x039a, 0x039e, 0x03a1, 0x03a5, 0x03a8, 0x03ac, 0x03af,
	0x03b2, 0x03b5, 0x03b8, 0x03bb, 0x03be, 0x03c1, 0x03c4, 0x03c7,
	0x03ca, 0x03cc, 0x03cf, 0x03d1, 0x03d4, 0x03d6, 0x03d9, 0x03db,
	0x03dd, 0x03df, 0x03e1, 0x03e3, 0x03e5, 0x03e7, 0x03e9, 0x03eb,
	0x03ec, 0x03ee, 0x03ef, 0x03f1, 0x03f2, 0x03f4, 0x03f5, 0x03f6,
	0x03f7, 0x03f8, 0x03f9, 0x03fa, 0x03fb, 0x03fc, 0x03fd, 0x03fd,
	0x03fe, 0x03fe, 0x03ff, 0x03ff, 0x03ff, 0x0400, 0x0400, 0x0400,
	0x0400, 0x0400, 0x0400, 0x0400, 0x03ff, 0x03ff, 0x03ff, 0x03fe,
	0x03fe, 0x03fd, 0x03fd, 0x03fc, 0x03fb, 0x03fa, 0x03f9, 0x03f8,
	0x03f7, 0x03f6, 0x03f5, 0x03f4, 0x03f2, 0x03f1, 0x03ef, 0x03ee,
	0x03ec, 0x03eb, 0x03e9, 0x03e7, 0x03e5, 0x03e3, 0x03e1, 0x03df,
	0x03dd, 0x03db, 0x03d9, 0x03d6, 0x03d4, 0x03d1, 0x03cf, 0x03cc,
	0x03ca, 0x03c7, 0x03c4, 0x03c1, 0x03be, 0x03bb, 0x03b8, 0x03b5,
	0x03b2, 0x03af, 0x03ac, 0x03a8, 0x03a5, 0x03a1, 0x039e, 0x039a,
	0x0396, 0x0393, 0x038f, 0x038b, 0x0387, 0x0383, 0x037f, 0x037b,
	0x0377, 0x0373, 0x036e, 0x036a, 0x0366, 0x0361, 0x035d, 0x0358,
	0x0353, 0x034f, 0x034a, 0x0345, 0x0340, 0x033b, 0x0336, 0x0331,
	0x032c, 0x0327, 0x0322, 0x031d, 0x0318, 0x0312, 0x030d, 0x0307,
	0x0302, 0x02fc, 0x02f7, 0x02f1, 0x02eb, 0x02e6, 0x02e0, 0x02da,
	0x02d4, 0x02ce, 0x02c8, 0x02c2, 0x02bc, 0x02b6, 0x02b0, 0x02a9,
	0x02a3, 0x029d, 0x0297, 0x0290, 0x028a, 0x0283, 0x027d, 0x0276,
	0x026f, 0x0269, 0x0262, 0x025b, 0x0254, 0x024e, 0x0247, 0x0240,
	0x0239, 0x0232, 0x022b, 0x0224, 0x021d, 0x0216, 0x020e, 0x0207
};

// hue range -60~60, sin(hue) table for picture HUE control
static  WORD MDIN_CscSinHue[]		= {
	0xfc89, 0xfc8d, 0xfc92, 0xfc96, 0xfc9a, 0xfc9f, 0xfca3, 0xfca8,
	0xfcad, 0xfcb1, 0xfcb6, 0xfcbb, 0xfcc0, 0xfcc5, 0xfcca, 0xfccf,
	0xfcd4, 0xfcd9, 0xfcde, 0xfce3, 0xfce8, 0xfcee, 0xfcf3, 0xfcf9,
	0xfcfe, 0xfd04, 0xfd09, 0xfd0f, 0xfd15, 0xfd1a, 0xfd20, 0xfd26,
	0xfd2c, 0xfd32, 0xfd38, 0xfd3e, 0xfd44, 0xfd4a, 0xfd50, 0xfd57,
	0xfd5d, 0xfd63, 0xfd69, 0xfd70, 0xfd76, 0xfd7d, 0xfd83, 0xfd8a,
	0xfd91, 0xfd97, 0xfd9e, 0xfda5, 0xfdac, 0xfdb2, 0xfdb9, 0xfdc0,
	0xfdc7, 0xfdce, 0xfdd5, 0xfddc, 0xfde3, 0xfdea, 0xfdf2, 0xfdf9,
	0xfe00, 0xfe07, 0xfe0f, 0xfe16, 0xfe1d, 0xfe25, 0xfe2c, 0xfe34,
	0xfe3b, 0xfe43, 0xfe4a, 0xfe52, 0xfe59, 0xfe61, 0xfe69, 0xfe70,
	0xfe78, 0xfe80, 0xfe88, 0xfe8f, 0xfe97, 0xfe9f, 0xfea7, 0xfeaf,
	0xfeb7, 0xfebf, 0xfec7, 0xfecf, 0xfed7, 0xfedf, 0xfee7, 0xfeef,
	0xfef7, 0xfeff, 0xff07, 0xff0f, 0xff17, 0xff20, 0xff28, 0xff30,
	0xff38, 0xff40, 0xff49, 0xff51, 0xff59, 0xff61, 0xff6a, 0xff72,
	0xff7a, 0xff83, 0xff8b, 0xff93, 0xff9c, 0xffa4, 0xffac, 0xffb5,
	0xffbd, 0xffc5, 0xffce, 0xffd6, 0xffdf, 0xffe7, 0xffef, 0xfff8,
	0x0000, 0x0008, 0x0011, 0x0019, 0x0021, 0x002a, 0x0032, 0x003b,
	0x0043, 0x004b, 0x0054, 0x005c, 0x0064, 0x006d, 0x0075, 0x007d,
	0x0086, 0x008e, 0x0096, 0x009f, 0x00a7, 0x00af, 0x00b7, 0x00c0,
	0x00c8, 0x00d0, 0x00d8, 0x00e0, 0x00e9, 0x00f1, 0x00f9, 0x0101,
	0x0109, 0x0111, 0x0119, 0x0121, 0x0129, 0x0131, 0x0139, 0x0141,
	0x0149, 0x0151, 0x0159, 0x0161, 0x0169, 0x0171, 0x0178, 0x0180,
	0x0188, 0x0190, 0x0197, 0x019f, 0x01a7, 0x01ae, 0x01b6, 0x01bd,
	0x01c5, 0x01cc, 0x01d4, 0x01db, 0x01e3, 0x01ea, 0x01f1, 0x01f9,
	0x0200, 0x0207, 0x020e, 0x0216, 0x021d, 0x0224, 0x022b, 0x0232,
	0x0239, 0x0240, 0x0247, 0x024e, 0x0254, 0x025b, 0x0262, 0x0269,
	0x026f, 0x0276, 0x027d, 0x0283, 0x028a, 0x0290, 0x0297, 0x029d,
	0x02a3, 0x02a9, 0x02b0, 0x02b6, 0x02bc, 0x02c2, 0x02c8, 0x02ce,
	0x02d4, 0x02da, 0x02e0, 0x02e6, 0x02eb, 0x02f1, 0x02f7, 0x02fc,
	0x0302, 0x0307, 0x030d, 0x0312, 0x0318, 0x031d, 0x0322, 0x0327,
	0x032c, 0x0331, 0x0336, 0x033b, 0x0340, 0x0345, 0x034a, 0x034f,
	0x0353, 0x0358, 0x035d, 0x0361, 0x0366, 0x036a, 0x036e, 0x0373
};

//--------------------------------------------------------------------------------------------------------------------------
/*
// Lanczos, 8-tap, 64-phase for interpolation filter coefficients
static  MDIN_MFCFILT_COEF MDIN_MFCFilter_Lanczos_8		= {
	  2048,     0,     0,     0,	// phase 00
	  2047,   -28,    10,    -3,	// phase 01
	  2044,   -56,    20,    -6,	// phase 02
	  2040,   -81,    29,    -9,	// phase 03
	  2034,  -106,    38,   -12,	// phase 04
	  2026,  -130,    47,   -14,	// phase 05
	  2016,  -153,    55,   -17,	// phase 06
	  2005,  -174,    63,   -19,	// phase 07
	  1992,  -194,    70,   -21,	// phase 08
	  1977,  -213,    77,   -22,	// phase 09
	  1961,  -231,    83,   -24,	// phase 10
	  1943,  -247,    90,   -26,	// phase 11
	  1923,  -262,    95,   -27,	// phase 12
	  1902,  -277,   100,   -28,	// phase 13
	  1879,  -290,   105,   -29,	// phase 14
	  1855,  -301,   110,   -30,	// phase 15
	  1830,  -312,   114,   -31,	// phase 16
	  1803,  -321,   117,   -32,	// phase 17
	  1775,  -330,   120,   -32,	// phase 18
	  1745,  -337,   123,   -32,	// phase 19
	  1714,  -343,   125,   -32,	// phase 20
	  1682,  -348,   127,   -32,	// phase 21
	  1649,  -352,   128,   -32,	// phase 22
	  1615,  -355,   129,   -32,	// phase 23
	  1580,  -357,   130,   -32,	// phase 24
	  1543,  -358,   130,   -31,	// phase 25
	  1506,  -358,   130,   -31,	// phase 26
	  1468,  -357,   129,   -30,	// phase 27
	  1429,  -355,   129,   -29,	// phase 28
	  1390,  -353,   128,   -29,	// phase 29
	  1350,  -349,   126,   -28,	// phase 30
	  1309,  -345,   124,   -27,	// phase 31
	  1268,  -340,   122,   -26,	// phase 32
	  1226,  -334,   120,   -25,	// phase 33
	  1183,  -328,   118,   -24,	// phase 34
	  1141,  -321,   115,   -23,	// phase 35
	  1097,  -313,   112,   -22,	// phase 36
	  1054,  -305,   109,   -20,	// phase 37
	  1011,  -296,   105,   -19,	// phase 38
	   967,  -287,   102,   -18,	// phase 39
	   923,  -277,    98,   -17,	// phase 40
	   880,  -267,    94,   -16,	// phase 41
	   836,  -257,    90,   -14,	// phase 42
	   793,  -246,    86,   -14,	// phase 43
	   749,  -235,    82,   -12,	// phase 44
	   706,  -223,    77,   -11,	// phase 45
	   664,  -212,    73,   -10,	// phase 46
	   621,  -200,    69,    -9,	// phase 47
	   579,  -188,    64,    -8,	// phase 48
	   537,  -176,    60,    -7,	// phase 49
	   496,  -163,    56,    -6,	// phase 50
	   456,  -151,    51,    -5,	// phase 51
	   416,  -139,    47,    -5,	// phase 52
	   376,  -126,    42,    -4,	// phase 53
	   338,  -114,    38,    -3,	// phase 54
	   300,  -102,    34,    -3,	// phase 55
	   263,   -90,    30,    -2,	// phase 56
	   227,   -78,    26,    -2,	// phase 57
	   192,   -66,    22,    -1,	// phase 58
	   157,   -55,    18,    -1,	// phase 59
	   124,   -43,    14,    -1,	// phase 60
	    91,   -32,    10,     0,	// phase 61
	    60,   -21,     7,     0,	// phase 62
	    29,   -10,     3,     0		// phase 63
};

// Lanczos, 4-tap, 64-phase for interpolation filter coefficients
static  MDIN_MFCFILT_COEF MDIN_MFCFilter_Lanczos_4		= {
	  2048,     0,		// phase 00
	  2047,   -20,		// phase 01
	  2043,   -38,		// phase 02
	  2038,   -56,		// phase 03
	  2030,   -71,		// phase 04
	  2020,   -86,		// phase 05
	  2007,   -99,		// phase 06
	  1993,  -111,		// phase 07
	  1977,  -123,		// phase 08
	  1958,  -133,		// phase 09
	  1938,  -141,		// phase 10
	  1916,  -149,		// phase 11
	  1892,  -156,		// phase 12
	  1866,  -161,		// phase 13
	  1838,  -165,		// phase 14
	  1809,  -169,		// phase 15
	  1779,  -172,		// phase 16
	  1747,  -174,		// phase 17
	  1714,  -175,		// phase 18
	  1679,  -175,		// phase 19
	  1643,  -174,		// phase 20
	  1607,  -173,		// phase 21
	  1569,  -171,		// phase 22
	  1530,  -169,		// phase 23
	  1490,  -166,		// phase 24
	  1450,  -163,		// phase 25
	  1409,  -158,		// phase 26
	  1367,  -154,		// phase 27
	  1325,  -150,		// phase 28
	  1282,  -144,		// phase 29
	  1239,  -139,		// phase 30
	  1196,  -134,		// phase 31
	  1152,  -128,		// phase 32
	  1108,  -122,		// phase 33
	  1064,  -116,		// phase 34
	  1020,  -110,		// phase 35
	   977,  -104,		// phase 36
	   933,   -98,		// phase 37
	   889,   -92,		// phase 38
	   846,   -85,		// phase 39
	   803,   -79,		// phase 40
	   760,   -73,		// phase 41
	   718,   -68,		// phase 42
	   676,   -62,		// phase 43
	   635,   -56,		// phase 44
	   595,   -51,		// phase 45
	   555,   -46,		// phase 46
	   516,   -41,		// phase 47
	   477,   -36,		// phase 48
	   440,   -32,		// phase 49
	   403,   -28,		// phase 50
	   367,   -24,		// phase 51
	   332,   -20,		// phase 52
	   298,   -17,		// phase 53
	   265,   -14,		// phase 54
	   234,   -11,		// phase 55
	   203,    -9,		// phase 56
	   173,    -7,		// phase 57
	   145,    -5,		// phase 58
	   117,    -3,		// phase 59
	    91,    -2,		// phase 60
	    67,    -1,		// phase 61
	    43,     0,		// phase 62
	    21,     0		// phase 63
};
*/
// Spline, B=0.00, C=1.00, 4-tap, 64-phase for interpolation filter coefficients
static  MDIN_MFCFILT_COEF MDIN_MFCFilter_Spline_1_00		= {
// Spline, B=0.00, C=1.00, 2-tap, lower 32-phase
	  2048,     0,		// phase 00
	  2044,   -60,		// phase 02
	  2033,  -113,		// phase 04
	  2014,  -158,		// phase 06
	  1988,  -196,		// phase 08
	  1956,  -228,		// phase 10
	  1918,  -254,		// phase 12
	  1873,  -273,		// phase 14
	  1824,  -288,		// phase 16
	  1770,  -298,		// phase 18
	  1711,  -303,		// phase 20
	  1647,  -303,		// phase 22
	  1580,  -300,		// phase 24
	  1509,  -293,		// phase 26
	  1436,  -284,		// phase 28
	  1359,  -271,		// phase 30
	  1280,  -256,		// phase 32
	  1199,  -239,		// phase 34
	  1117,  -221,		// phase 36
	  1033,  -201,		// phase 38
	   948,  -180,		// phase 40
	   863,  -159,		// phase 42
	   778,  -138,		// phase 44
	   692,  -116,		// phase 46
	   608,   -96,		// phase 48
	   525,   -77,		// phase 50
	   443,   -59,		// phase 52
	   362,   -42,		// phase 54
	   284,   -28,		// phase 56
	   208,   -16,		// phase 58
	   136,    -8,		// phase 60
	    66,    -2,		// phase 62

// Spline, B=1.00, C=0.00, 2-tap, upper 32-phase
	  1365,   341,		// phase 00
	  1364,   310,		// phase 02
	  1358,   281,		// phase 04
	  1348,   254,		// phase 06
	  1335,   229,		// phase 08
	  1319,   205,		// phase 10
	  1300,   183,		// phase 12
	  1278,   163,		// phase 14
	  1253,   144,		// phase 16
	  1226,   127,		// phase 18
	  1197,   111,		// phase 20
	  1165,    96,		// phase 22
	  1131,    83,		// phase 24
	  1096,    71,		// phase 26
	  1059,    61,		// phase 28
	  1021,    51,		// phase 30
	   981,    43,		// phase 32
	   941,    35,		// phase 34
	   900,    29,		// phase 36
	   858,    23,		// phase 38
	   815,    18,		// phase 40
	   773,    14,		// phase 42
	   730,    10,		// phase 44
	   687,     8,		// phase 46
	   645,     5,		// phase 48
	   604,     3,		// phase 50
	   563,     2,		// phase 52
	   523,     1,		// phase 54
	   483,     1,		// phase 56
	   446,     0,		// phase 58
	   409,     0,		// phase 60
	   374,     0 		// phase 62
};
/*
// Spline, B=0.00, C=0.75, 4-tap, 64-phase for interpolation filter coefficients
static  MDIN_MFCFILT_COEF MDIN_MFCFilter_Spline_0_75		= {
	  2048,     0,		// phase 00
	  2047,   -23,		// phase 01
	  2044,   -45,		// phase 02
	  2038,   -65,		// phase 03
	  2031,   -84,		// phase 04
	  2021,  -102,		// phase 05
	  2009,  -118,		// phase 06
	  1996,  -133,		// phase 07
	  1981,  -147,		// phase 08
	  1964,  -160,		// phase 09
	  1945,  -171,		// phase 10
	  1925,  -181,		// phase 11
	  1903,  -190,		// phase 12
	  1879,  -198,		// phase 13
	  1854,  -205,		// phase 14
	  1828,  -211,		// phase 15
	  1800,  -216,		// phase 16
	  1771,  -220,		// phase 17
	  1740,  -223,		// phase 18
	  1709,  -226,		// phase 19
	  1676,  -227,		// phase 20
	  1642,  -227,		// phase 21
	  1607,  -227,		// phase 22
	  1572,  -227,		// phase 23
	  1535,  -225,		// phase 24
	  1498,  -223,		// phase 25
	  1459,  -220,		// phase 26
	  1420,  -217,		// phase 27
	  1380,  -213,		// phase 28
	  1340,  -208,		// phase 29
	  1299,  -203,		// phase 30
	  1258,  -198,		// phase 31
	  1216,  -192,		// phase 32
	  1174,  -186,		// phase 33
	  1131,  -179,		// phase 34
	  1089,  -173,		// phase 35
	  1046,  -165,		// phase 36
	  1003,  -158,		// phase 37
	   959,  -150,		// phase 38
	   916,  -143,		// phase 39
	   873,  -135,		// phase 40
	   830,  -127,		// phase 41
	   787,  -119,		// phase 42
	   744,  -111,		// phase 43
	   702,  -103,		// phase 44
	   660,   -95,		// phase 45
	   618,   -87,		// phase 46
	   577,   -80,		// phase 47
	   536,   -72,		// phase 48
	   496,   -65,		// phase 49
	   456,   -57,		// phase 50
	   417,   -50,		// phase 51
	   379,   -44,		// phase 52
	   342,   -38,		// phase 53
	   305,   -31,		// phase 54
	   270,   -26,		// phase 55
	   235,   -21,		// phase 56
	   201,   -16,		// phase 57
	   169,   -12,		// phase 58
	   138,    -9,		// phase 59
	   107,    -6,		// phase 60
	    78,    -3,		// phase 61
	    51,    -2,		// phase 62
	    25,    -1		// phase 63
};

// Spline, B=0.00, C=0.50, 4-tap, 64-phase for interpolation filter coefficients
static  MDIN_MFCFILT_COEF MDIN_MFCFilter_Spline_0_50		= {
	  2048,     0,		// phase 00
	  2047,   -16,		// phase 01
	  2043,   -30,		// phase 02
	  2037,   -44,		// phase 03
	  2029,   -56,		// phase 04
	  2018,   -68,		// phase 05
	  2006,   -79,		// phase 06
	  1991,   -89,		// phase 07
	  1974,   -98,		// phase 08
	  1955,  -106,		// phase 09
	  1935,  -114,		// phase 10
	  1912,  -121,		// phase 11
	  1888,  -127,		// phase 12
	  1863,  -132,		// phase 13
	  1835,  -137,		// phase 14
	  1806,  -141,		// phase 15
	  1776,  -144,		// phase 16
	  1744,  -147,		// phase 17
	  1711,  -149,		// phase 18
	  1677,  -150,		// phase 19
	  1642,  -151,		// phase 20
	  1605,  -152,		// phase 21
	  1568,  -152,		// phase 22
	  1529,  -151,		// phase 23
	  1490,  -150,		// phase 24
	  1450,  -149,		// phase 25
	  1409,  -147,		// phase 26
	  1367,  -144,		// phase 27
	  1325,  -142,		// phase 28
	  1283,  -139,		// phase 29
	  1239,  -135,		// phase 30
	  1196,  -132,		// phase 31
	  1152,  -128,		// phase 32
	  1108,  -124,		// phase 33
	  1064,  -120,		// phase 34
	  1019,  -115,		// phase 35
	   975,  -110,		// phase 36
	   930,  -105,		// phase 37
	   886,  -100,		// phase 38
	   842,   -95,		// phase 39
	   798,   -90,		// phase 40
	   755,   -85,		// phase 41
	   711,   -79,		// phase 42
	   669,   -74,		// phase 43
	   626,   -69,		// phase 44
	   585,   -64,		// phase 45
	   544,   -58,		// phase 46
	   504,   -53,		// phase 47
	   464,   -48,		// phase 48
	   426,   -43,		// phase 49
	   388,   -38,		// phase 50
	   351,   -34,		// phase 51
	   316,   -29,		// phase 52
	   282,   -25,		// phase 53
	   248,   -21,		// phase 54
	   216,   -17,		// phase 55
	   186,   -14,		// phase 56
	   157,   -11,		// phase 57
	   129,    -8,		// phase 58
	   104,    -6,		// phase 59
	    79,    -4,		// phase 60
	    57,    -2,		// phase 61
	    36,    -1,		// phase 62
	    17,     0		// phase 63
};
*/

// Bi-linear (2-tap), 32-phase for interpolation filter coefficients
static  MDIN_MFCFILT_COEF MDIN_MFCFilter_Bilinear		= {
	  2048,     0,		// phase 00
	  1984,     0,		// phase 02
	  1920,     0,		// phase 04
	  1856,     0,		// phase 06
	  1792,     0,		// phase 08
	  1728,     0,		// phase 10
	  1664,     0,		// phase 12
	  1600,     0,		// phase 14
	  1536,     0,		// phase 16
	  1472,     0,		// phase 18
	  1408,     0,		// phase 20
	  1344,     0,		// phase 22
	  1280,     0,		// phase 24
	  1216,     0,		// phase 26
	  1152,     0,		// phase 28
	  1088,     0,		// phase 30
	  1024,     0,		// phase 32
	   960,     0,		// phase 34
	   896,     0,		// phase 36
	   832,     0,		// phase 38
	   768,     0,		// phase 40
	   704,     0,		// phase 42
	   640,     0,		// phase 44
	   576,     0,		// phase 46
	   512,     0,		// phase 48
	   448,     0,		// phase 50
	   384,     0,		// phase 52
	   320,     0,		// phase 54
	   256,     0,		// phase 56
	   192,     0,		// phase 58
	   128,     0,		// phase 60
	    64,     0,		// phase 62

// Spline, B=1.00, C=0.00, 2-tap, upper 32-phase
	  1365,   341,		// phase 00
	  1364,   310,		// phase 02
	  1358,   281,		// phase 04
	  1348,   254,		// phase 06
	  1335,   229,		// phase 08
	  1319,   205,		// phase 10
	  1300,   183,		// phase 12
	  1278,   163,		// phase 14
	  1253,   144,		// phase 16
	  1226,   127,		// phase 18
	  1197,   111,		// phase 20
	  1165,    96,		// phase 22
	  1131,    83,		// phase 24
	  1096,    71,		// phase 26
	  1059,    61,		// phase 28
	  1021,    51,		// phase 30
	   981,    43,		// phase 32
	   941,    35,		// phase 34
	   900,    29,		// phase 36
	   858,    23,		// phase 38
	   815,    18,		// phase 40
	   773,    14,		// phase 42
	   730,    10,		// phase 44
	   687,     8,		// phase 46
	   645,     5,		// phase 48
	   604,     3,		// phase 50
	   563,     2,		// phase 52
	   523,     1,		// phase 54
	   483,     1,		// phase 56
	   446,     0,		// phase 58
	   409,     0,		// phase 60
	   374,     0 		// phase 62
};

//--------------------------------------------------------------------------------------------------------------------------
// Lanczos, 8-tap, 2-phase for double rate converter filter
/*
static  WORD MDIN_DRCFilter_Lanczos_8[]		= {
	  2048,     0,     0,     0,	// phase 0
	  1268,  -340,   122,   -26		// phase 1
};

// Lanczos, 4-tap, 2-phase for double rate converter filter
static  WORD MDIN_DRCFilter_Lanczos_4[]		= {
	  2048,     0,	// phase 0
	  1152,  -128	// phase 1
};

// Spline, B=0.00, C=1.00, 4-tap, 2-phase for double rate converter filter
static  WORD MDIN_DRCFilter_Spline_1_00[]	= {
	  2048,     0,	// phase 0
	  1280,  -256	// phase 1
};

// Spline, B=0.00, C=0.75, 4-tap, 2-phase for double rate converter filter
static  WORD MDIN_DRCFilter_Spline_0_75[]	= {
	  2048,     0,	// phase 0
	  1216,  -192	// phase 1
};

// Spline, B=0.00, C=0.50, 4-tap, 2-phase for double rate converter filter
static  WORD MDIN_DRCFilter_Spline_0_50[]	= {
	  2048,     0,	// phase 0
	  1152,  -128	// phase 1
};
*/
//--------------------------------------------------------------------------------------------------------------------------
// Lanczos for color enhance filter
static  WORD MDIN_CEFilter_Lanczos[]			= {
	   256,   144,     0,   -16
};

// Spline, B 0.00, C 1.00 for color enhance filter
static  WORD MDIN_CEFilter_Spline_1_00[]		= {
	   256,   160,     0,   -32
};
/*
// Spline, B 0.00, C 0.75 for color enhance filter
static  WORD MDIN_CEFilter_Spline_0_75[]		= {
	   256,   152,     0,   -24
};

// Spline, B 0.00, C 0.50 for color enhance filter
static  WORD MDIN_CEFilter_Spline_0_50[]		= {
	   256,   144,     0,   -16
};
*/
//--------------------------------------------------------------------------------------------------------------------------
/* Filter Coefficients for Peaking Filter */
static  WORD MDIN_PeakingFilter[]		= {
	512, 0, -256, 0, 0, 0, 0, 0		/* 1st coefficient is center-tap */
};

/* Filter Coefficients for Surface Filter */
static  WORD MDIN_SurfaceFilter[]		= {
	256, 128, 128, 128				/* 1st coefficient is center-tap */
};

/* Initialization Data for LTI/CTI */
static  WORD MDIN_LTICTIFactor[]			= {
	0x02F9, 0x5018
};

//--------------------------------------------------------------------------------------------------------------------------
// Coefficients for FrontNR filter
static  WORD MDIN_FNRYFilter[]		= {
	0x0400, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};

static  WORD MDIN_FNRYHDFilt[][8]	= {
	0x023C, 0x0152, 0x0FDD, 0x0F97, 0x0021, 0x0036, 0x0FE3, 0x0FE2,		// 1920->1024
	0x02CA, 0x00FB, 0x0F67, 0x002A, 0x0023, 0x0FC7, 0x0022, 0x0003,		// 1440->1024
	0x0307, 0x00E9, 0x0F5E, 0x004A, 0x0001, 0x0FD5, 0x0030, 0x0FE5,		// 1368->1024
	0x0351, 0x00C6, 0x0F62, 0x0068, 0x0FD1, 0x0000, 0x001D, 0x0FDA,		// 1280->1024
	0x0170, 0x011F, 0x006D, 0x0FDA, 0x0FB7, 0x0FEA, 0x0020, 0x0022		// 1920->720
};

static  WORD MDIN_FNRCFilter[]		= {
	0x0400, 0x0000, 0x0000, 0x0000
};

static  WORD MDIN_FNRCHDFilt[][4	]	= {
	0x024E, 0x0157, 0x0FDE, 0x0FA4,		// H: 1920->1024
	0x02E0, 0x00FE, 0x0F6D, 0x0025,		// H: 1440->1024
	0x02EF, 0x00DE, 0x0F6D, 0x003D,		// H: 1368->1024
	0x0309, 0x00B2, 0x0F79, 0x0051,		// H: 1280->1024
	0x0160, 0x010E, 0x0061, 0x0FE1,		// H: 1920->720
};

//--------------------------------------------------------------------------------------------------------------------------
/* default value for Deinterlacer */
static  WORD MDIN_DeintFactor[]		= {
	0x6C0E, 0xBB12, 0x9C04, 0x8020, 0xF7FF, 0x2000, 0x4F00, 0x0A38, /* 500 */	// Edge_2nd_en disable
//	0x0048, 0x100A, 0x80D0, 0x81E8, 0x1330, 0xC620, 0x08FF, 0x1E08, /* 508 */	// 0x508 changed for border
	0x0098, 0x100A, 0x80D0, 0x81E8, 0x1330, 0xC620, 0x08FF, 0x1E08, /* 508 */	// 0x508 changed for border, Sep04 2009
	0x0000, 0x0000, 0x0028, 0xFFDF, 0x00FF, 0x0000, 0x01B3, 0x1105, /* 510 */
	0x00B4, 0x0000, 0x0000, 0xC4C4, 0x2710, 0x2710, 0x0000, 0x3C14, /* 518 */
	0x00FF, 0x1E01, 0xFFFF, 0x0102, 0xF888, 0x18FF, 0xC800, 0xF590, /* 520 */	// 0x520 Changed for border
	0x0A43, 0xC7E8, 0x10DC, 0x0000, 0x004C,	0x0000, 0x0000, 0x0000, /* 528 */
	0x0000, 0x3040, 0x8A04, 0x0000, 0x0000,	0xFFDC, 0x6028, 0x0501, /* 530 */
	0x08FF, 0x1886, 0xFF86, 0x8832, 0x0A3F,	0x2F6B, 0x0414, 0x0600  /* 538 */
};

//--------------------------------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------


#endif	/* __MDINCOEF_H__ */
