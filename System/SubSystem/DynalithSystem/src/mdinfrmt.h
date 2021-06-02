//----------------------------------------------------------------------------------------------------------------------
// (C) Copyright 2008  Macro Image Technology Co., LTd. , All rights reserved
// 
// This source code is the property of Macro Image Technology and is provided
// pursuant to a Software License Agreement. This code's reuse and distribution
// without Macro Image Technology's permission is strictly limited by the confidential
// information provisions of the Software License Agreement.
//-----------------------------------------------------------------------------------------------------------------------
//
// File Name   		:  MDINFRMT.H
// Description 		:  This file contains typedefine for the driver files	
// Ref. Docment		: 
// Revision History 	:

#ifndef		__MDINFRMT_H__
#define		__MDINFRMT_H__

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Struct/Union Types and define
// -----------------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
// default value for inVideo format
static  MDIN_OUTSYNC_INFO defMDINOutSync[]	= {
/*
	{	// 1440x480i60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		 276,		// vdpulse_pos_bot
		1716,		// htotal_size
		 239,		// hactive_start
		1679,		// hactive_end
		 125,		// hsync_start
		   1,		// hsync_end
		 525,		// vtotal_size
		  22,		// vactive_start
		 262,		// vactive_end
		   7,		// vsync_start
		   4,		// vsync_end
		 285,		// vactive_start_bot
		   0,		// vactive_end_bot
		 269,		// vsync_start_bot
		 266,		// vsync_end_bot
		 859,		// vsync_bot_fld_pos
	
		   4,		// pre_div_vclk (P)
		  16,		// post_div_vclk (M)
		   2		// post_scale_vclk (S)
	},
	{	// 1440x576i50
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		 323,		// vdpulse_pos_bot
		1728,		// htotal_size
		 265,		// hactive_start
		1705,		// hactive_end
		 127,		// hsync_start
		   1,		// hsync_end
		 625,		// vtotal_size
		  23,		// vactive_start
		 311,		// vactive_end
		   4,		// vsync_start
		   1,		// vsync_end
		 336,		// vactive_start_bot
		 624,		// vactive_end_bot
		 316,		// vsync_start_bot
		 313,		// vsync_end_bot
		 865,		// vsync_bot_fld_pos
	
		   4,		// pre_div_vclk (P)
		  16,		// post_div_vclk (M)
		   2		// post_scale_vclk (S)
	},
*/
	{	// 720x480i60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		 276,		// vdpulse_pos_bot
		 858,		// htotal_size
		 120,		// hactive_start
		 840,		// hactive_end
		  63,		// hsync_start
		   1,		// hsync_end
		 525,		// vtotal_size
		  22,		// vactive_start
		 262,		// vactive_end
		   7,		// vsync_start
		   4,		// vsync_end
		 285,		// vactive_start_bot
		   0,		// vactive_end_bot
		 269,		// vsync_start_bot
		 266,		// vsync_end_bot
		 430,		// vsync_bot_fld_pos
	
		   4,		// pre_div_vclk (P)
		  16,		// post_div_vclk (M)
		   3		// post_scale_vclk (S)
	},
	{	// 720x576i50
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		 323,		// vdpulse_pos_bot
		 864,		// htotal_size
		 133,		// hactive_start
		 853,		// hactive_end
		  64,		// hsync_start
		   1,		// hsync_end
		 625,		// vtotal_size
		  23,		// vactive_start
		 311,		// vactive_end
		   4,		// vsync_start
		   1,		// vsync_end
		 336,		// vactive_start_bot
		 624,		// vactive_end_bot
		 316,		// vsync_start_bot
		 313,		// vsync_end_bot
		 433,		// vsync_bot_fld_pos
	
		   4,		// pre_div_vclk (P)
		  16,		// post_div_vclk (M)
		   3		// post_scale_vclk (S)
	},
	{	// 720x480p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		 858,		// htotal_size
		 123,		// hactive_start
		 843,		// hactive_end
		  63,		// hsync_start
		   1,		// hsync_end
		 525,		// vtotal_size
		  43,		// vactive_start
		 523,		// vactive_end
		  13,		// vsync_start
		   7,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   4,		// pre_div_vclk (P)
		  16,		// post_div_vclk (M)
		   2		// post_scale_vclk (S)
	},
	{	// 720x576p50
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		 864,		// htotal_size
		 133,		// hactive_start
		 853,		// hactive_end
		  65,		// hsync_start
		   1,		// hsync_end
		 625,		// vtotal_size
		  45,		// vactive_start
		 621,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   4,		// pre_div_vclk (P)
		  16,		// post_div_vclk (M)
		   2		// post_scale_vclk (S)
	},
	{	// 1280x720p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1650,		// htotal_size
		 261,		// hactive_start
		1541,		// hactive_end
		  41,		// hsync_start
		   1,		// hsync_end
		 750,		// vtotal_size
		  26,		// vactive_start
		 746,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   4,		// pre_div_vclk (P)
		  44,		// post_div_vclk (M)
		   2		// post_scale_vclk (S)
	},
	{	// 1280x720p50
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1980,		// htotal_size
		 261,		// hactive_start
		1541,		// hactive_end
		  41,		// hsync_start
		   1,		// hsync_end
		 750,		// vtotal_size
		  26,		// vactive_start
		 746,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   4,		// pre_div_vclk (P)
		  44,		// post_div_vclk (M)
		   2		// post_scale_vclk (S)
	},
	{	// 1920x1080i60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		 573,		// vdpulse_pos_bot
		2200,		// htotal_size
		 193,		// hactive_start
		2113,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		1125,		// vtotal_size
		  21,		// vactive_start
		 561,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		 584,		// vactive_start_bot
		1124,		// vactive_end_bot
		 568,		// vsync_start_bot
		 563,		// vsync_end_bot
		1101,		// vsync_bot_fld_pos
	
		   4,		// pre_div_vclk (P)
		  44,		// post_div_vclk (M)
		   2		// post_scale_vclk (S)
	},
	{	// 1920x1080i50
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		 573,		// vdpulse_pos_bot
		2640,		// htotal_size
		 193,		// hactive_start
		2113,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		1125,		// vtotal_size
		  21,		// vactive_start
		 561,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		 584,		// vactive_start_bot
		1124,		// vactive_end_bot
		 568,		// vsync_start_bot
		 563,		// vsync_end_bot
		1321,		// vsync_bot_fld_pos
	
		   4,		// pre_div_vclk (P)
		  44,		// post_div_vclk (M)
		   2		// post_scale_vclk (S)
	},
	{	// 1920x1080p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2200,		// htotal_size
		 193,		// hactive_start
		2113,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		1125,		// vtotal_size
		  42,		// vactive_start
		1122,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   4,		// pre_div_vclk (P)
		  22,		// post_div_vclk (M)
		   0		// post_scale_vclk (S)
	},
	{	// 1920x1080p50
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2640,		// htotal_size
		 193,		// hactive_start
		2113,		// hactive_end
		  45,		// hsync_start
		   1,		// hsync_end
		1125,		// vtotal_size
		  42,		// vactive_start
		1122,		// vactive_end
		   6,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   4,		// pre_div_vclk (P)
		  22,		// post_div_vclk (M)
		   0		// post_scale_vclk (S)
	},
	{	// 640x480p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		 800,		// htotal_size
		 145,		// hactive_start
		 785,		// hactive_end
		  97,		// hsync_start
		   1,		// hsync_end
		 525,		// vtotal_size
		  36,		// vactive_start
		 516,		// vactive_end
		   3,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		  26,		// pre_div_vclk (P)
		  97,		// post_div_vclk (M)
		   2		// post_scale_vclk (S)
	},
	{	// 640x480p75
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		 840,		// htotal_size
		 185,		// hactive_start
		 825,		// hactive_end
		  65,		// hsync_start
		   1,		// hsync_end
		 500,		// vtotal_size
		  19,		// vactive_start
		 499,		// vactive_end
		   3,		// vsync_start
		   0,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		  27,		// pre_div_vclk (P)
		  63,		// post_div_vclk (M)
		   1		// post_scale_vclk (S)
	},
	{	// 800x480p60
			20,			// hdpulse_pos
			10,			// vdpulse_pos
			0,			// vdpulse_pos_bot
			992,		// htotal_size
			169,		// hactive_start
			969,		// hactive_end
			73,			// hsync_start
			1,			// hsync_end
			500,		// vtotal_size
			17,		// vactive_start
			497,	// vactive_end
			7,		// vsync_start
			0,		// vsync_end
			0,		// vactive_start_bot
			0,		// vactive_end_bot
			0,		// vsync_start_bot
			0,		// vsync_end_bot
			0,		// vsync_bot_fld_pos

			27,		// pre_div_vclk (P)
			118,	// post_div_vclk (M)
			2		// post_scale_vclk (S)
	},
	{	// 800x600p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1056,		// htotal_size
		 217,		// hactive_start
		1017,		// hactive_end
		 129,		// hsync_start
		   1,		// hsync_end
		 628,		// vtotal_size
		  27,		// vactive_start
		 627,		// vactive_end
		   4,		// vsync_start
		   0,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		  27,		// pre_div_vclk (P)
		 160,		// post_div_vclk (M)
		   2		// post_scale_vclk (S)
	},
	{	// 800x600p75
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1056,		// htotal_size
		 241,		// hactive_start
		1041,		// hactive_end
		  81,		// hsync_start
		   1,		// hsync_end
		 625,		// vtotal_size
		  24,		// vactive_start
		 624,		// vactive_end
		   3,		// vsync_start
		   0,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		  24,		// pre_div_vclk (P)
		 176,		// post_div_vclk (M)
		   2		// post_scale_vclk (S)
	},
	{	// 1024x768p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1344,		// htotal_size
		 297,		// hactive_start
		1321,		// hactive_end
		 137,		// hsync_start
		   1,		// hsync_end
		 806,		// vtotal_size
		  36,		// vactive_start
		 804,		// vactive_end
		   7,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		  27,		// pre_div_vclk (P)
		 130,		// post_div_vclk (M)
		   1		// post_scale_vclk (S)
	},
	{	// 1024x768p75
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1312,		// htotal_size
		 273,		// hactive_start
		1297,		// hactive_end
		  97,		// hsync_start
		   1,		// hsync_end
		 800,		// vtotal_size
		  31,		// vactive_start
		 799,		// vactive_end
		   3,		// vsync_start
		   0,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		  24,		// pre_div_vclk (P)
		 140,		// post_div_vclk (M)
		   1		// post_scale_vclk (S)
	},
	{	// 1280x1024p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1688,		// htotal_size
		 361,		// hactive_start
		1641,		// hactive_end
		 113,		// hsync_start
		   1,		// hsync_end
		1066,		// vtotal_size
		  41,		// vactive_start
		1065,		// vactive_end
		   3,		// vsync_start
		   0,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   4,		// pre_div_vclk (P)
		  16,		// post_div_vclk (M)
		   0		// post_scale_vclk (S)
	},
	{	// 1280x1024p75
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1688,		// htotal_size
		 393,		// hactive_start
		1673,		// hactive_end
		 145,		// hsync_start
		   1,		// hsync_end
		1066,		// vtotal_size
		  41,		// vactive_start
		1065,		// vactive_end
		   3,		// vsync_start
		   0,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   4,		// pre_div_vclk (P)
		  20,		// post_div_vclk (M)
		   0		// post_scale_vclk (S)
	},

	{	// 1360x768p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1792,		// htotal_size
		 369,		// hactive_start
		1729,		// hactive_end
		 113,		// hsync_start
		   1,		// hsync_end
		 795,		// vtotal_size
		  25,		// vactive_start
		 793,		// vactive_end
		   7,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
				
		  27,		// pre_div_vclk (P)
		 171,		// post_div_vclk (M)
		   1		// post_scale_vclk (S)
	},
	{	// 1440x900p60
		  20,		// hdpulse_pos	
		  10,		// vdpulse_pos	
		   0,		// vdpulse_pos_bot	
		1904,		// htotal_size	
		 385,		// hactive_start	
		1825,		// hactive_end	
		 153,		// hsync_start	
		   1,		// hsync_end	
		 934,		// vtotal_size	
		  32,		// vactive_start	
		 932,		// vactive_end	
		   7,		// vsync_start	
		   1,		// vsync_end	
		   0,		// vactive_start_bot	
		   0,		// vactive_end_bot	
		   0,		// vsync_start_bot	
		   0,		// vsync_end_bot	
		   0,		// vsync_bot_fld_pos	
					
		  27,		// pre_div_vclk (P)	
		 213,		// post_div_vclk (M)	
		   1		// post_scale_vclk (S)	
	},
	{	// 1440x900p75
		  20,		// hdpulse_pos	
		  10,		// vdpulse_pos	
		   0,		// vdpulse_pos_bot	
		1936,		// htotal_size	
		 401,		// hactive_start	
		1841,		// hactive_end	
		 153,		// hsync_start	
		   1,		// hsync_end	
		 942,		// vtotal_size	
		  40,		// vactive_start	
		 940,		// vactive_end	
		   7,		// vsync_start	
		   1,		// vsync_end	
		   0,		// vactive_start_bot	
		   0,		// vactive_end_bot	
		   0,		// vsync_start_bot	
		   0,		// vsync_end_bot	
		   0,		// vsync_bot_fld_pos	
					
		  24,		// pre_div_vclk (P)	
		 243,		// post_div_vclk (M)	
		   1		// post_scale_vclk (S)	
	},
	{	// 1680x1050p60
		  20,		// hdpulse_pos	
		  10,		// vdpulse_pos	
		   0,		// vdpulse_pos_bot	
		2240,		// htotal_size	
		 457,		// hactive_start	
		2137,		// hactive_end	
		 177,		// hsync_start	
		   1,		// hsync_end	
		1089,		// vtotal_size	
		  37,		// vactive_start	
		1087,		// vactive_end	
		   7,		// vsync_start	
		   1,		// vsync_end	
		   0,		// vactive_start_bot	
		   0,		// vactive_end_bot	
		   0,		// vsync_start_bot	
		   0,		// vsync_end_bot	
		   0,		// vsync_bot_fld_pos	
					
		  12,		// pre_div_vclk (P)	
		  65,		// post_div_vclk (M)	
		   0		// post_scale_vclk (S)	
	},
	{	// 1600x1200p60
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2160,		// htotal_size
		 497,		// hactive_start
		2097,		// hactive_end
		 193,		// hsync_start
		   1,		// hsync_end
		1250,		// vtotal_size
		  49,		// vactive_start
		1249,		// vactive_end
		   3,		// vsync_start
		   0,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
	
		   4,		// pre_div_vclk (P)
		  24,		// post_div_vclk (M)
		   0		// post_scale_vclk (S)
	},

	{
		// 1680x1050p60 Reduced Blanking
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		1840,		// htotal_size
		 113,		// hactive_start
		1793,		// hactive_end
		  33,		// hsync_start
		   1,		// hsync_end
		1080,		// vtotal_size
		  28,		// vactive_start
		1078,		// vactive_end
		   7,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
		 
		  27,		// pre_div_vclk (P)
		 119,		// post_div_vclk (M)
		   0		// post_scale_vclk (S)
	},
	{
		// 1920x1080p60 Reduced Blanking
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2080,		// htotal_size
		 113,		// hactive_start
		2033,		// hactive_end
		  33,		// hsync_start
		   1,		// hsync_end
		1111,		// vtotal_size
		  30,		// vactive_start
		1110,		// vactive_end
		   7,		// vsync_start
		   2,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
		 
		  27,		// pre_div_vclk (P)
		 277,		// post_div_vclk (M)
		   1		// post_scale_vclk (S)
	},
	{
		// 1920x1200p60 Reduced Blanking
		  20,		// hdpulse_pos
		  10,		// vdpulse_pos
		   0,		// vdpulse_pos_bot
		2080,		// htotal_size
		 113,		// hactive_start
		2033,		// hactive_end
		  33,		// hsync_start
		   1,		// hsync_end
		1235,		// vtotal_size
		  33,		// vactive_start
		1233,		// vactive_end
		   7,		// vsync_start
		   1,		// vsync_end
		   0,		// vactive_start_bot
		   0,		// vactive_end_bot
		   0,		// vsync_start_bot
		   0,		// vsync_end_bot
		   0,		// vsync_bot_fld_pos
		 
		  27,		// pre_div_vclk (P)
		 154,		// post_div_vclk (M)
		   0		// post_scale_vclk (S)
	}
};

static  MDIN_DACSYNC_DATA defMDINDACData[]	= {
/*
	{	// 1440x480i60
		 76,			// 0x00 : dac_sync_level_a
		153,			// 0x01 : dac_sync_level_b
		229,			// 0x02 : dac_sync_level_c
		383,			// 0x03 : dac_sync_level_d
		460,			// 0x04 : dac_sync_level_e
		536,			// 0x05 : dac_sync_level_f
		614,			// 0x06 : dac_sync_level_g
		307,			// 0x07 : dac_blank_level_g
		307,			// 0x08 : dac_blank_level_br
		126,			// 0x09 : dac_heq_pulse_size
		252,			// 0x0A : dac_hsync_size
		1716,			// 0x0B : dac_htotal_half_size
		 74,			// 0x0C : dac_hfp_interval
		326,			// 0x0D : dac_hbp_interval
		10<<12 |   1,	// 0x0E : dac_line_format1(15:12), dac_vstart_pos1(10:0)
		 5<<12 |   4,	// 0x0F : dac_line_format2(15:12), dac_vstart_pos2(10:0)
		10<<12 |   7,	// 0x10 : dac_line_format3(15:12), dac_vstart_pos3(10:0)
		 0<<12 |  10,	// 0x11 : dac_line_format4(15:12), dac_vstart_pos4(10:0)
		 0<<12 |  22,	// 0x12 : dac_line_format5(15:12), dac_vstart_pos5(10:0)
		 2<<12 | 262,	// 0x13 : dac_line_format6(15:12), dac_vstart_pos6(10:0)
		10<<12 | 264,	// 0x14 : dac_line_format7(15:12), dac_vstart_pos7(10:0)
		 9<<12 | 266,	// 0x15 : dac_line_format8(15:12), dac_vstart_pos8(10:0)
		 5<<12 | 267,	// 0x16 : dac_line_format9(15:12), dac_vstart_pos9(10:0)
		 6<<12 | 269,	// 0x17 : dac_line_format10(15:12), dac_vstart_pos10(10:0)
		10<<12 | 270,	// 0x18 : dac_line_format11(15:12), dac_vstart_pos11(10:0)
		 8<<12 | 272,	// 0x19 : dac_line_format12(15:12), dac_vstart_pos12(10:0)
		 0<<12 | 273,	// 0x1A : dac_line_format13(15:12), dac_vstart_pos13(10:0)
		 0<<12 | 285,	// 0x1B : dac_line_format14(15:12), dac_vstart_pos14(10:0)
		10<<12 |   0,	// 0x1C : dac_line_format15(15:12), dac_vstart_pos15(10:0)
		10<<12 |   0,	// 0x1D : dac_line_format16(15:12), dac_vstart_pos16(10:0)
		 1<<15 | 717,	// 0x1E : dac_scale_g_en(15), dac_scale_g(9:0)
		 1<<15 | 717,	// 0x1F : dac_scale_b_en(15), dac_scale_b(9:0)
		 1<<15 | 717,	// 0x20 : dac_scale_r_en(15), dac_scale_r(9:0)
		 1<<15 | 307,	// 0x21 : dac_clip_l_g_en(15), dac_clip_l_g(9:0)
		 1<<15 | 1023,	// 0x22 : dac_clip_u_g_en(15), dac_clip_u_g(9:0)
		 1<<15 | 153,	// 0x23 : dac_clip_l_br_en(15), dac_clip_l_br(9:0)
		 1<<15 | 871	// 0x24 : dac_clip_u_br_en(15), dac_clip_u_br(9:0)
	},
	{	// 1440x576i50
		 76,			// 0x00 : dac_sync_level_a                                
		153,			// 0x01 : dac_sync_level_b                                
		229,			// 0x02 : dac_sync_level_c                                
		383,			// 0x03 : dac_sync_level_d                                
		460,			// 0x04 : dac_sync_level_e                                
		536,			// 0x05 : dac_sync_level_f                                
		614,			// 0x06 : dac_sync_level_g                                
		307,			// 0x07 : dac_blank_level_g                               
		307,			// 0x08 : dac_blank_level_br                              
		128,			// 0x09 : dac_heq_pulse_size                              
		256,			// 0x0A : dac_hsync_size                                  
		1728,			// 0x0B : dac_htotal_half_size                            
		 46,			// 0x0C : dac_hfp_interval                                
		302,			// 0x0D : dac_hbp_interval                                
		 5<<12 |   1,	// 0x0E : dac_line_format1(15:12), dac_vstart_pos1(10:0)  
		 6<<12 |   3,	// 0x0F : dac_line_format2(15:12), dac_vstart_pos2(10:0)  
		10<<12 |   4,	// 0x10 : dac_line_format3(15:12), dac_vstart_pos3(10:0)  
		 0<<12 |   6,	// 0x11 : dac_line_format4(15:12), dac_vstart_pos4(10:0)  
		 0<<12 |  23,	// 0x12 : dac_line_format5(15:12), dac_vstart_pos5(10:0)  
		10<<12 | 311,	// 0x13 : dac_line_format6(15:12), dac_vstart_pos6(10:0)  
		 9<<12 | 313,	// 0x14 : dac_line_format7(15:12), dac_vstart_pos7(10:0)  
		 5<<12 | 314,	// 0x15 : dac_line_format8(15:12), dac_vstart_pos8(10:0)  
		10<<12 | 316,	// 0x16 : dac_line_format9(15:12), dac_vstart_pos9(10:0)  
		 8<<12 | 318,	// 0x17 : dac_line_format10(15:12), dac_vstart_pos10(10:0)
		 0<<12 | 319,	// 0x18 : dac_line_format11(15:12), dac_vstart_pos11(10:0)
		 0<<12 | 336,	// 0x19 : dac_line_format12(15:12), dac_vstart_pos12(10:0)
		10<<12 | 624,	// 0x1A : dac_line_format13(15:12), dac_vstart_pos13(10:0)
		10<<12 |   0,	// 0x1B : dac_line_format14(15:12), dac_vstart_pos14(10:0)
		10<<12 |   0,	// 0x1C : dac_line_format15(15:12), dac_vstart_pos15(10:0)
		10<<12 |   0,	// 0x1D : dac_line_format16(15:12), dac_vstart_pos16(10:0)
		 1<<15 | 717,	// 0x1E : dac_scale_g_en(15), dac_scale_g(9:0)            
		 1<<15 | 717,	// 0x1F : dac_scale_b_en(15), dac_scale_b(9:0)            
		 1<<15 | 717,	// 0x20 : dac_scale_r_en(15), dac_scale_r(9:0)            
		 1<<15 | 307,	// 0x21 : dac_clip_l_g_en(15), dac_clip_l_g(9:0)          
		 1<<15 | 1023,	// 0x22 : dac_clip_u_g_en(15), dac_clip_u_g(9:0)          
		 1<<15 | 153,	// 0x23 : dac_clip_l_br_en(15), dac_clip_l_br(9:0)        
		 1<<15 | 871	// 0x24 : dac_clip_u_br_en(15), dac_clip_u_br(9:0)        
	},
*/
	{	// 720x480i60
		 76,			// 0x00 : dac_sync_level_a                                
		153,			// 0x01 : dac_sync_level_b                                
		229,			// 0x02 : dac_sync_level_c                                
		383,			// 0x03 : dac_sync_level_d                                
		460,			// 0x04 : dac_sync_level_e                                
		536,			// 0x05 : dac_sync_level_f                                
		614,			// 0x06 : dac_sync_level_g                                
		307,			// 0x07 : dac_blank_level_g                               
		307,			// 0x08 : dac_blank_level_br                              
		 32,			// 0x09 : dac_heq_pulse_size                              
		 64,			// 0x0A : dac_hsync_size                                  
		429,			// 0x0B : dac_htotal_half_size                            
		 48,			// 0x0C : dac_hfp_interval                                
		 90,			// 0x0D : dac_hbp_interval                                
		10<<12 |   1,	// 0x0E : dac_line_format1(15:12), dac_vstart_pos1(10:0)  
		 5<<12 |   4,	// 0x0F : dac_line_format2(15:12), dac_vstart_pos2(10:0)  
		10<<12 |   7,	// 0x10 : dac_line_format3(15:12), dac_vstart_pos3(10:0)  
		 0<<12 |  10,	// 0x11 : dac_line_format4(15:12), dac_vstart_pos4(10:0)  
		 0<<12 |  22,	// 0x12 : dac_line_format5(15:12), dac_vstart_pos5(10:0)  
		 2<<12 | 262,	// 0x13 : dac_line_format6(15:12), dac_vstart_pos6(10:0)  
		10<<12 | 264,	// 0x14 : dac_line_format7(15:12), dac_vstart_pos7(10:0)  
		 9<<12 | 266,	// 0x15 : dac_line_format8(15:12), dac_vstart_pos8(10:0)  
		 5<<12 | 267,	// 0x16 : dac_line_format9(15:12), dac_vstart_pos9(10:0)  
		 6<<12 | 269,	// 0x17 : dac_line_format10(15:12), dac_vstart_pos10(10:0)
		10<<12 | 270,	// 0x18 : dac_line_format11(15:12), dac_vstart_pos11(10:0)
		 8<<12 | 272,	// 0x19 : dac_line_format12(15:12), dac_vstart_pos12(10:0)
		 0<<12 | 273,	// 0x1A : dac_line_format13(15:12), dac_vstart_pos13(10:0)
		 0<<12 | 285,	// 0x1B : dac_line_format14(15:12), dac_vstart_pos14(10:0)
		10<<12 |   0,	// 0x1C : dac_line_format15(15:12), dac_vstart_pos15(10:0)
		10<<12 |   0,	// 0x1D : dac_line_format16(15:12), dac_vstart_pos16(10:0)
		 1<<15 | 717,	// 0x1E : dac_scale_g_en(15), dac_scale_g(9:0)            
		 1<<15 | 717,	// 0x1F : dac_scale_b_en(15), dac_scale_b(9:0)            
		 1<<15 | 717,	// 0x20 : dac_scale_r_en(15), dac_scale_r(9:0)            
		 1<<15 | 307,	// 0x21 : dac_clip_l_g_en(15), dac_clip_l_g(9:0)          
		 1<<15 | 1023,	// 0x22 : dac_clip_u_g_en(15), dac_clip_u_g(9:0)          
		 1<<15 | 153,	// 0x23 : dac_clip_l_br_en(15), dac_clip_l_br(9:0)        
		 1<<15 | 871	// 0x24 : dac_clip_u_br_en(15), dac_clip_u_br(9:0)        
	},
	{	// 720x576i50
		 76,			// 0x00 : dac_sync_level_a                                
		153,			// 0x01 : dac_sync_level_b                                
		229,			// 0x02 : dac_sync_level_c                                
		383,			// 0x03 : dac_sync_level_d                                
		460,			// 0x04 : dac_sync_level_e                                
		536,			// 0x05 : dac_sync_level_f                                
		614,			// 0x06 : dac_sync_level_g                                
		307,			// 0x07 : dac_blank_level_g                               
		307,			// 0x08 : dac_blank_level_br                              
		 32,			// 0x09 : dac_heq_pulse_size                              
		 64,			// 0x0A : dac_hsync_size                                  
		432,			// 0x0B : dac_htotal_half_size                            
		 44,			// 0x0C : dac_hfp_interval                                
		100,			// 0x0D : dac_hbp_interval                                
		 5<<12 |   1,	// 0x0E : dac_line_format1(15:12), dac_vstart_pos1(10:0)  
		 5<<12 |   3,	// 0x0F : dac_line_format2(15:12), dac_vstart_pos2(10:0)  
		10<<12 |   4,	// 0x10 : dac_line_format3(15:12), dac_vstart_pos3(10:0)  
		 0<<12 |   6,	// 0x11 : dac_line_format4(15:12), dac_vstart_pos4(10:0)  
		 0<<12 |  23,	// 0x12 : dac_line_format5(15:12), dac_vstart_pos5(10:0)  
		10<<12 | 311,	// 0x13 : dac_line_format6(15:12), dac_vstart_pos6(10:0)  
		 9<<12 | 313,	// 0x14 : dac_line_format7(15:12), dac_vstart_pos7(10:0)  
		 5<<12 | 314,	// 0x15 : dac_line_format8(15:12), dac_vstart_pos8(10:0)  
		10<<12 | 316,	// 0x16 : dac_line_format9(15:12), dac_vstart_pos9(10:0)  
		 8<<12 | 318,	// 0x17 : dac_line_format10(15:12), dac_vstart_pos10(10:0)
		 0<<12 | 319,	// 0x18 : dac_line_format11(15:12), dac_vstart_pos11(10:0)
		 0<<12 | 336,	// 0x19 : dac_line_format12(15:12), dac_vstart_pos12(10:0)
		10<<12 | 624,	// 0x1A : dac_line_format13(15:12), dac_vstart_pos13(10:0)
		10<<12 |   0,	// 0x1B : dac_line_format14(15:12), dac_vstart_pos14(10:0)
		10<<12 |   0,	// 0x1C : dac_line_format15(15:12), dac_vstart_pos15(10:0)
		10<<12 |   0,	// 0x1D : dac_line_format16(15:12), dac_vstart_pos16(10:0)
		 1<<15 | 717,	// 0x1E : dac_scale_g_en(15), dac_scale_g(9:0)            
		 1<<15 | 717,	// 0x1F : dac_scale_b_en(15), dac_scale_b(9:0)            
		 1<<15 | 717,	// 0x20 : dac_scale_r_en(15), dac_scale_r(9:0)            
		 1<<15 | 307,	// 0x21 : dac_clip_l_g_en(15), dac_clip_l_g(9:0)          
		 1<<15 | 1023,	// 0x22 : dac_clip_u_g_en(15), dac_clip_u_g(9:0)          
		 1<<15 | 153,	// 0x23 : dac_clip_l_br_en(15), dac_clip_l_br(9:0)        
		 1<<15 | 871	// 0x24 : dac_clip_u_br_en(15), dac_clip_u_br(9:0)        
	},
	{	// 720x480p60
		 76,			// 0x00 : dac_sync_level_a                                
		153,			// 0x01 : dac_sync_level_b                                
		229,			// 0x02 : dac_sync_level_c                                
		383,			// 0x03 : dac_sync_level_d                                
		460,			// 0x04 : dac_sync_level_e                                
		536,			// 0x05 : dac_sync_level_f                                
		614,			// 0x06 : dac_sync_level_g                                
		307,			// 0x07 : dac_blank_level_g                               
		307,			// 0x08 : dac_blank_level_br                              
		 32,			// 0x09 : dac_heq_pulse_size                              
		 64,			// 0x0A : dac_hsync_size                                  
		429,			// 0x0B : dac_htotal_half_size                            
		 48,			// 0x0C : dac_hfp_interval                                
		 90,			// 0x0D : dac_hbp_interval                                
		 0<<12 |   1,	// 0x0E : dac_line_format1(15:12), dac_vstart_pos1(10:0)  
		15<<12 |   7,	// 0x0F : dac_line_format2(15:12), dac_vstart_pos2(10:0)  
		 0<<12 |  13,	// 0x10 : dac_line_format3(15:12), dac_vstart_pos3(10:0)  
		 0<<12 |  43,	// 0x11 : dac_line_format4(15:12), dac_vstart_pos4(10:0)  
		 0<<12 | 523,	// 0x12 : dac_line_format5(15:12), dac_vstart_pos5(10:0)  
		 0<<12 |   0,	// 0x13 : dac_line_format6(15:12), dac_vstart_pos6(10:0)  
		 0<<12 |   0,	// 0x14 : dac_line_format7(15:12), dac_vstart_pos7(10:0)  
		 0<<12 |   0,	// 0x15 : dac_line_format8(15:12), dac_vstart_pos8(10:0)  
		 0<<12 |   0,	// 0x16 : dac_line_format9(15:12), dac_vstart_pos9(10:0)  
		 0<<12 |   0,	// 0x17 : dac_line_format10(15:12), dac_vstart_pos10(10:0)
		 0<<12 |   0,	// 0x18 : dac_line_format11(15:12), dac_vstart_pos11(10:0)
		 0<<12 |   0,	// 0x19 : dac_line_format12(15:12), dac_vstart_pos12(10:0)
		 0<<12 |   0,	// 0x1A : dac_line_format13(15:12), dac_vstart_pos13(10:0)
		 0<<12 |   0,	// 0x1B : dac_line_format14(15:12), dac_vstart_pos14(10:0)
		 0<<12 |   0,	// 0x1C : dac_line_format15(15:12), dac_vstart_pos15(10:0)
		 0<<12 |   0,	// 0x1D : dac_line_format16(15:12), dac_vstart_pos16(10:0)
		 1<<15 | 717,	// 0x1E : dac_scale_g_en(15), dac_scale_g(9:0)            
		 1<<15 | 717,	// 0x1F : dac_scale_b_en(15), dac_scale_b(9:0)            
		 1<<15 | 717,	// 0x20 : dac_scale_r_en(15), dac_scale_r(9:0)            
		 1<<15 | 307,	// 0x21 : dac_clip_l_g_en(15), dac_clip_l_g(9:0)          
		 1<<15 | 1023,	// 0x22 : dac_clip_u_g_en(15), dac_clip_u_g(9:0)          
		 1<<15 | 153,	// 0x23 : dac_clip_l_br_en(15), dac_clip_l_br(9:0)        
		 1<<15 | 871	// 0x24 : dac_clip_u_br_en(15), dac_clip_u_br(9:0)        
	},
	{	// 720x576p50
		 76,			// 0x00 : dac_sync_level_a                                
		153,			// 0x01 : dac_sync_level_b                                
		229,			// 0x02 : dac_sync_level_c                                
		383,			// 0x03 : dac_sync_level_d                                
		460,			// 0x04 : dac_sync_level_e                                
		536,			// 0x05 : dac_sync_level_f                                
		614,			// 0x06 : dac_sync_level_g                                
		307,			// 0x07 : dac_blank_level_g                               
		307,			// 0x08 : dac_blank_level_br                              
		 32,			// 0x09 : dac_heq_pulse_size                              
		 64,			// 0x0A : dac_hsync_size                                  
		432,			// 0x0B : dac_htotal_half_size                            
		 44,			// 0x0C : dac_hfp_interval                                
		100,			// 0x0D : dac_hbp_interval                                
		15<<12 |   1,	// 0x0E : dac_line_format1(15:12), dac_vstart_pos1(10:0)  
		 0<<12 |   6,	// 0x0F : dac_line_format2(15:12), dac_vstart_pos2(10:0)  
		 0<<12 |  45,	// 0x10 : dac_line_format3(15:12), dac_vstart_pos3(10:0)  
		 0<<12 | 621,	// 0x11 : dac_line_format4(15:12), dac_vstart_pos4(10:0)  
		 0<<12 |   0,	// 0x12 : dac_line_format5(15:12), dac_vstart_pos5(10:0)  
		 0<<12 |   0,	// 0x13 : dac_line_format6(15:12), dac_vstart_pos6(10:0)  
		 0<<12 |   0,	// 0x14 : dac_line_format7(15:12), dac_vstart_pos7(10:0)  
		 0<<12 |   0,	// 0x15 : dac_line_format8(15:12), dac_vstart_pos8(10:0)  
		 0<<12 |   0,	// 0x16 : dac_line_format9(15:12), dac_vstart_pos9(10:0)  
		 0<<12 |   0,	// 0x17 : dac_line_format10(15:12), dac_vstart_pos10(10:0)
		 0<<12 |   0,	// 0x18 : dac_line_format11(15:12), dac_vstart_pos11(10:0)
		 0<<12 |   0,	// 0x19 : dac_line_format12(15:12), dac_vstart_pos12(10:0)
		 0<<12 |   0,	// 0x1A : dac_line_format13(15:12), dac_vstart_pos13(10:0)
		 0<<12 |   0,	// 0x1B : dac_line_format14(15:12), dac_vstart_pos14(10:0)
		 0<<12 |   0,	// 0x1C : dac_line_format15(15:12), dac_vstart_pos15(10:0)
		 0<<12 |   0,	// 0x1D : dac_line_format16(15:12), dac_vstart_pos16(10:0)
		 1<<15 | 717,	// 0x1E : dac_scale_g_en(15), dac_scale_g(9:0)            
		 1<<15 | 717,	// 0x1F : dac_scale_b_en(15), dac_scale_b(9:0)            
		 1<<15 | 717,	// 0x20 : dac_scale_r_en(15), dac_scale_r(9:0)            
		 1<<15 | 307,	// 0x21 : dac_clip_l_g_en(15), dac_clip_l_g(9:0)          
		 1<<15 | 1023,	// 0x22 : dac_clip_u_g_en(15), dac_clip_u_g(9:0)          
		 1<<15 | 153,	// 0x23 : dac_clip_l_br_en(15), dac_clip_l_br(9:0)        
		 1<<15 | 871	// 0x24 : dac_clip_u_br_en(15), dac_clip_u_br(9:0)        
	},
	{	// 1280x720p60
		 76,			// 0x00 : dac_sync_level_a                                
		153,			// 0x01 : dac_sync_level_b                                
		229,			// 0x02 : dac_sync_level_c                                
		383,			// 0x03 : dac_sync_level_d                                
		460,			// 0x04 : dac_sync_level_e                                
		536,			// 0x05 : dac_sync_level_f                                
		614,			// 0x06 : dac_sync_level_g                                
		307,			// 0x07 : dac_blank_level_g                               
		307,			// 0x08 : dac_blank_level_br                              
		 20,			// 0x09 : dac_heq_pulse_size                              
		 41,			// 0x0A : dac_hsync_size                                  
		825,			// 0x0B : dac_htotal_half_size                            
		110,			// 0x0C : dac_hfp_interval                                
		260,			// 0x0D : dac_hbp_interval                                
		15<<12 |   1,	// 0x0E : dac_line_format1(15:12), dac_vstart_pos1(10:0)  
		 0<<12 |   6,	// 0x0F : dac_line_format2(15:12), dac_vstart_pos2(10:0)  
		 0<<12 |  26,	// 0x10 : dac_line_format3(15:12), dac_vstart_pos3(10:0)  
		 0<<12 | 746,	// 0x11 : dac_line_format4(15:12), dac_vstart_pos4(10:0)  
		 0<<12 |   0,	// 0x12 : dac_line_format5(15:12), dac_vstart_pos5(10:0)  
		 0<<12 |   0,	// 0x13 : dac_line_format6(15:12), dac_vstart_pos6(10:0)  
		 0<<12 |   0,	// 0x14 : dac_line_format7(15:12), dac_vstart_pos7(10:0)  
		 0<<12 |   0,	// 0x15 : dac_line_format8(15:12), dac_vstart_pos8(10:0)  
		 0<<12 |   0,	// 0x16 : dac_line_format9(15:12), dac_vstart_pos9(10:0)  
		 0<<12 |   0,	// 0x17 : dac_line_format10(15:12), dac_vstart_pos10(10:0)
		 0<<12 |   0,	// 0x18 : dac_line_format11(15:12), dac_vstart_pos11(10:0)
		 0<<12 |   0,	// 0x19 : dac_line_format12(15:12), dac_vstart_pos12(10:0)
		 0<<12 |   0,	// 0x1A : dac_line_format13(15:12), dac_vstart_pos13(10:0)
		 0<<12 |   0,	// 0x1B : dac_line_format14(15:12), dac_vstart_pos14(10:0)
		 0<<12 |   0,	// 0x1C : dac_line_format15(15:12), dac_vstart_pos15(10:0)
		 0<<12 |   0,	// 0x1D : dac_line_format16(15:12), dac_vstart_pos16(10:0)
		 1<<15 | 717,	// 0x1E : dac_scale_g_en(15), dac_scale_g(9:0)            
		 1<<15 | 717,	// 0x1F : dac_scale_b_en(15), dac_scale_b(9:0)            
		 1<<15 | 717,	// 0x20 : dac_scale_r_en(15), dac_scale_r(9:0)            
		 1<<15 | 307,	// 0x21 : dac_clip_l_g_en(15), dac_clip_l_g(9:0)          
		 1<<15 | 1023,	// 0x22 : dac_clip_u_g_en(15), dac_clip_u_g(9:0)          
		 1<<15 | 153,	// 0x23 : dac_clip_l_br_en(15), dac_clip_l_br(9:0)        
		 1<<15 | 871	// 0x24 : dac_clip_u_br_en(15), dac_clip_u_br(9:0)        
	},
	{	// 1280x720p50
		 76,			// 0x00 : dac_sync_level_a                                
		153,			// 0x01 : dac_sync_level_b                                
		229,			// 0x02 : dac_sync_level_c                                
		383,			// 0x03 : dac_sync_level_d                                
		460,			// 0x04 : dac_sync_level_e                                
		536,			// 0x05 : dac_sync_level_f                                
		614,			// 0x06 : dac_sync_level_g                                
		307,			// 0x07 : dac_blank_level_g                               
		307,			// 0x08 : dac_blank_level_br                              
		 20,			// 0x09 : dac_heq_pulse_size                              
		 41,			// 0x0A : dac_hsync_size                                  
		990,			// 0x0B : dac_htotal_half_size                            
		440,			// 0x0C : dac_hfp_interval                                
		260,			// 0x0D : dac_hbp_interval                                
		15<<12 |   1,	// 0x0E : dac_line_format1(15:12), dac_vstart_pos1(10:0)  
		 0<<12 |   6,	// 0x0F : dac_line_format2(15:12), dac_vstart_pos2(10:0)  
		 0<<12 |  26,	// 0x10 : dac_line_format3(15:12), dac_vstart_pos3(10:0)  
		 0<<12 | 746,	// 0x11 : dac_line_format4(15:12), dac_vstart_pos4(10:0)  
		 0<<12 |   0,	// 0x12 : dac_line_format5(15:12), dac_vstart_pos5(10:0)  
		 0<<12 |   0,	// 0x13 : dac_line_format6(15:12), dac_vstart_pos6(10:0)  
		 0<<12 |   0,	// 0x14 : dac_line_format7(15:12), dac_vstart_pos7(10:0)  
		 0<<12 |   0,	// 0x15 : dac_line_format8(15:12), dac_vstart_pos8(10:0)  
		 0<<12 |   0,	// 0x16 : dac_line_format9(15:12), dac_vstart_pos9(10:0)  
		 0<<12 |   0,	// 0x17 : dac_line_format10(15:12), dac_vstart_pos10(10:0)
		 0<<12 |   0,	// 0x18 : dac_line_format11(15:12), dac_vstart_pos11(10:0)
		 0<<12 |   0,	// 0x19 : dac_line_format12(15:12), dac_vstart_pos12(10:0)
		 0<<12 |   0,	// 0x1A : dac_line_format13(15:12), dac_vstart_pos13(10:0)
		 0<<12 |   0,	// 0x1B : dac_line_format14(15:12), dac_vstart_pos14(10:0)
		 0<<12 |   0,	// 0x1C : dac_line_format15(15:12), dac_vstart_pos15(10:0)
		 0<<12 |   0,	// 0x1D : dac_line_format16(15:12), dac_vstart_pos16(10:0)
		 1<<15 | 717,	// 0x1E : dac_scale_g_en(15), dac_scale_g(9:0)            
		 1<<15 | 717,	// 0x1F : dac_scale_b_en(15), dac_scale_b(9:0)            
		 1<<15 | 717,	// 0x20 : dac_scale_r_en(15), dac_scale_r(9:0)            
		 1<<15 | 307,	// 0x21 : dac_clip_l_g_en(15), dac_clip_l_g(9:0)          
		 1<<15 | 1023,	// 0x22 : dac_clip_u_g_en(15), dac_clip_u_g(9:0)          
		 1<<15 | 153,	// 0x23 : dac_clip_l_br_en(15), dac_clip_l_br(9:0)        
		 1<<15 | 871	// 0x24 : dac_clip_u_br_en(15), dac_clip_u_br(9:0)        
	},
	{	// 1920x1080i60
		 76,			// 0x00 : dac_sync_level_a                                
		153,			// 0x01 : dac_sync_level_b                                
		229,			// 0x02 : dac_sync_level_c                                
		383,			// 0x03 : dac_sync_level_d                                
		460,			// 0x04 : dac_sync_level_e                                
		536,			// 0x05 : dac_sync_level_f                                
		614,			// 0x06 : dac_sync_level_g                                
		307,			// 0x07 : dac_blank_level_g                               
		307,			// 0x08 : dac_blank_level_br                              
		 22,			// 0x09 : dac_heq_pulse_size                              
		 45,			// 0x0A : dac_hsync_size                                  
		1100,			// 0x0B : dac_htotal_half_size                            
		 88,			// 0x0C : dac_hfp_interval                                
		133,			// 0x0D : dac_hbp_interval                                
		 5<<12 |   1,	// 0x0E : dac_line_format1(15:12), dac_vstart_pos1(10:0)  
		10<<12 |   6,	// 0x0F : dac_line_format2(15:12), dac_vstart_pos2(10:0)  
		 0<<12 |   7,	// 0x10 : dac_line_format3(15:12), dac_vstart_pos3(10:0)  
		 0<<12 |  21,	// 0x11 : dac_line_format4(15:12), dac_vstart_pos4(10:0)  
		 0<<12 | 561,	// 0x12 : dac_line_format5(15:12), dac_vstart_pos5(10:0)  
		 9<<12 | 563,	// 0x13 : dac_line_format6(15:12), dac_vstart_pos6(10:0)  
		 5<<12 | 564,	// 0x14 : dac_line_format7(15:12), dac_vstart_pos7(10:0)  
		 6<<12 | 568,	// 0x15 : dac_line_format8(15:12), dac_vstart_pos8(10:0)  
		 0<<12 | 569,	// 0x16 : dac_line_format9(15:12), dac_vstart_pos9(10:0)  
		 0<<12 | 584,	// 0x17 : dac_line_format10(15:12), dac_vstart_pos10(10:0)
		 0<<12 | 1124,	// 0x18 : dac_line_format11(15:12), dac_vstart_pos11(10:0)
		 0<<12 |   0,	// 0x19 : dac_line_format12(15:12), dac_vstart_pos12(10:0)
		 0<<12 |   0,	// 0x1A : dac_line_format13(15:12), dac_vstart_pos13(10:0)
		 0<<12 |   0,	// 0x1B : dac_line_format14(15:12), dac_vstart_pos14(10:0)
		 0<<12 |   0,	// 0x1C : dac_line_format15(15:12), dac_vstart_pos15(10:0)
		 0<<12 |   0,	// 0x1D : dac_line_format16(15:12), dac_vstart_pos16(10:0)
		 1<<15 | 717,	// 0x1E : dac_scale_g_en(15), dac_scale_g(9:0)            
		 1<<15 | 717,	// 0x1F : dac_scale_b_en(15), dac_scale_b(9:0)            
		 1<<15 | 717,	// 0x20 : dac_scale_r_en(15), dac_scale_r(9:0)            
		 1<<15 | 307,	// 0x21 : dac_clip_l_g_en(15), dac_clip_l_g(9:0)          
		 1<<15 | 1023,	// 0x22 : dac_clip_u_g_en(15), dac_clip_u_g(9:0)          
		 1<<15 | 153,	// 0x23 : dac_clip_l_br_en(15), dac_clip_l_br(9:0)        
		 1<<15 | 871	// 0x24 : dac_clip_u_br_en(15), dac_clip_u_br(9:0)        
	},
	{	// 1920x1080i50
		 76,			// 0x00 : dac_sync_level_a                                
		153,			// 0x01 : dac_sync_level_b                                
		229,			// 0x02 : dac_sync_level_c                                
		383,			// 0x03 : dac_sync_level_d                                
		460,			// 0x04 : dac_sync_level_e                                
		536,			// 0x05 : dac_sync_level_f                                
		614,			// 0x06 : dac_sync_level_g                                
		307,			// 0x07 : dac_blank_level_g                               
		307,			// 0x08 : dac_blank_level_br                              
		 22,			// 0x09 : dac_heq_pulse_size                              
		 45,			// 0x0A : dac_hsync_size                                  
		1320,			// 0x0B : dac_htotal_half_size                            
		313,			// 0x0C : dac_hfp_interval                                
		133,			// 0x0D : dac_hbp_interval                                
		 5<<12 |   1,	// 0x0E : dac_line_format1(15:12), dac_vstart_pos1(10:0)  
		10<<12 |   6,	// 0x0F : dac_line_format2(15:12), dac_vstart_pos2(10:0)  
		 0<<12 |   7,	// 0x10 : dac_line_format3(15:12), dac_vstart_pos3(10:0)  
		 0<<12 |  21,	// 0x11 : dac_line_format4(15:12), dac_vstart_pos4(10:0)  
		 0<<12 | 561,	// 0x12 : dac_line_format5(15:12), dac_vstart_pos5(10:0)  
		 9<<12 | 563,	// 0x13 : dac_line_format6(15:12), dac_vstart_pos6(10:0)  
		 5<<12 | 564,	// 0x14 : dac_line_format7(15:12), dac_vstart_pos7(10:0)  
		 6<<12 | 568,	// 0x15 : dac_line_format8(15:12), dac_vstart_pos8(10:0)  
		 0<<12 | 569,	// 0x16 : dac_line_format9(15:12), dac_vstart_pos9(10:0)  
		 0<<12 | 584,	// 0x17 : dac_line_format10(15:12), dac_vstart_pos10(10:0)
		 0<<12 | 1124,	// 0x18 : dac_line_format11(15:12), dac_vstart_pos11(10:0)
		 0<<12 |   0,	// 0x19 : dac_line_format12(15:12), dac_vstart_pos12(10:0)
		 0<<12 |   0,	// 0x1A : dac_line_format13(15:12), dac_vstart_pos13(10:0)
		 0<<12 |   0,	// 0x1B : dac_line_format14(15:12), dac_vstart_pos14(10:0)
		 0<<12 |   0,	// 0x1C : dac_line_format15(15:12), dac_vstart_pos15(10:0)
		 0<<12 |   0,	// 0x1D : dac_line_format16(15:12), dac_vstart_pos16(10:0)
		 1<<15 | 717,	// 0x1E : dac_scale_g_en(15), dac_scale_g(9:0)            
		 1<<15 | 717,	// 0x1F : dac_scale_b_en(15), dac_scale_b(9:0)            
		 1<<15 | 717,	// 0x20 : dac_scale_r_en(15), dac_scale_r(9:0)            
		 1<<15 | 307,	// 0x21 : dac_clip_l_g_en(15), dac_clip_l_g(9:0)          
		 1<<15 | 1023,	// 0x22 : dac_clip_u_g_en(15), dac_clip_u_g(9:0)          
		 1<<15 | 153,	// 0x23 : dac_clip_l_br_en(15), dac_clip_l_br(9:0)        
		 1<<15 | 871	// 0x24 : dac_clip_u_br_en(15), dac_clip_u_br(9:0)        
	},
	{	// 1920x1080p60
		 76,			// 0x00 : dac_sync_level_a                                
		153,			// 0x01 : dac_sync_level_b                                
		229,			// 0x02 : dac_sync_level_c                                
		383,			// 0x03 : dac_sync_level_d                                
		460,			// 0x04 : dac_sync_level_e                                
		536,			// 0x05 : dac_sync_level_f                                
		614,			// 0x06 : dac_sync_level_g                                
		307,			// 0x07 : dac_blank_level_g                               
		307,			// 0x08 : dac_blank_level_br                              
		 22,			// 0x09 : dac_heq_pulse_size                              
		 45,			// 0x0A : dac_hsync_size                                  
		1100,			// 0x0B : dac_htotal_half_size                            
		 88,			// 0x0C : dac_hfp_interval                                
		133,			// 0x0D : dac_hbp_interval                                
		15<<12 |   1,	// 0x0E : dac_line_format1(15:12), dac_vstart_pos1(10:0)  
		 0<<12 |   6,	// 0x0F : dac_line_format2(15:12), dac_vstart_pos2(10:0)  
		 0<<12 |  42,	// 0x10 : dac_line_format3(15:12), dac_vstart_pos3(10:0)  
		 0<<12 | 1122,	// 0x11 : dac_line_format4(15:12), dac_vstart_pos4(10:0)  
		 0<<12 |   0,	// 0x12 : dac_line_format5(15:12), dac_vstart_pos5(10:0)  
		 0<<12 |   0,	// 0x13 : dac_line_format6(15:12), dac_vstart_pos6(10:0)  
		 0<<12 |   0,	// 0x14 : dac_line_format7(15:12), dac_vstart_pos7(10:0)  
		 0<<12 |   0,	// 0x15 : dac_line_format8(15:12), dac_vstart_pos8(10:0)  
		 0<<12 |   0,	// 0x16 : dac_line_format9(15:12), dac_vstart_pos9(10:0)  
		 0<<12 |   0,	// 0x17 : dac_line_format10(15:12), dac_vstart_pos10(10:0)
		 0<<12 |   0,	// 0x18 : dac_line_format11(15:12), dac_vstart_pos11(10:0)
		 0<<12 |   0,	// 0x19 : dac_line_format12(15:12), dac_vstart_pos12(10:0)
		 0<<12 |   0,	// 0x1A : dac_line_format13(15:12), dac_vstart_pos13(10:0)
		 0<<12 |   0,	// 0x1B : dac_line_format14(15:12), dac_vstart_pos14(10:0)
		 0<<12 |   0,	// 0x1C : dac_line_format15(15:12), dac_vstart_pos15(10:0)
		 0<<12 |   0,	// 0x1D : dac_line_format16(15:12), dac_vstart_pos16(10:0)
		 1<<15 | 717,	// 0x1E : dac_scale_g_en(15), dac_scale_g(9:0)            
		 1<<15 | 717,	// 0x1F : dac_scale_b_en(15), dac_scale_b(9:0)            
		 1<<15 | 717,	// 0x20 : dac_scale_r_en(15), dac_scale_r(9:0)            
		 1<<15 | 307,	// 0x21 : dac_clip_l_g_en(15), dac_clip_l_g(9:0)          
		 1<<15 | 1023,	// 0x22 : dac_clip_u_g_en(15), dac_clip_u_g(9:0)          
		 1<<15 | 153,	// 0x23 : dac_clip_l_br_en(15), dac_clip_l_br(9:0)        
		 1<<15 | 871	// 0x24 : dac_clip_u_br_en(15), dac_clip_u_br(9:0)        
	},
	{	// 1920x1080p50
		 76,			// 0x00 : dac_sync_level_a                                
		153,			// 0x01 : dac_sync_level_b                                
		229,			// 0x02 : dac_sync_level_c                                
		383,			// 0x03 : dac_sync_level_d                                
		460,			// 0x04 : dac_sync_level_e                                
		536,			// 0x05 : dac_sync_level_f                                
		614,			// 0x06 : dac_sync_level_g                                
		307,			// 0x07 : dac_blank_level_g                               
		307,			// 0x08 : dac_blank_level_br                              
		 22,			// 0x09 : dac_heq_pulse_size                              
		 45,			// 0x0A : dac_hsync_size                                  
		1320,			// 0x0B : dac_htotal_half_size                            
		528,			// 0x0C : dac_hfp_interval                                
		133,			// 0x0D : dac_hbp_interval                                
		15<<12 |   1,	// 0x0E : dac_line_format1(15:12), dac_vstart_pos1(10:0)  
		 0<<12 |   6,	// 0x0F : dac_line_format2(15:12), dac_vstart_pos2(10:0)  
		 0<<12 |  42,	// 0x10 : dac_line_format3(15:12), dac_vstart_pos3(10:0)  
		 0<<12 | 1122,	// 0x11 : dac_line_format4(15:12), dac_vstart_pos4(10:0)  
		 0<<12 |   0,	// 0x12 : dac_line_format5(15:12), dac_vstart_pos5(10:0)  
		 0<<12 |   0,	// 0x13 : dac_line_format6(15:12), dac_vstart_pos6(10:0)  
		 0<<12 |   0,	// 0x14 : dac_line_format7(15:12), dac_vstart_pos7(10:0)  
		 0<<12 |   0,	// 0x15 : dac_line_format8(15:12), dac_vstart_pos8(10:0)  
		 0<<12 |   0,	// 0x16 : dac_line_format9(15:12), dac_vstart_pos9(10:0)  
		 0<<12 |   0,	// 0x17 : dac_line_format10(15:12), dac_vstart_pos10(10:0)
		 0<<12 |   0,	// 0x18 : dac_line_format11(15:12), dac_vstart_pos11(10:0)
		 0<<12 |   0,	// 0x19 : dac_line_format12(15:12), dac_vstart_pos12(10:0)
		 0<<12 |   0,	// 0x1A : dac_line_format13(15:12), dac_vstart_pos13(10:0)
		 0<<12 |   0,	// 0x1B : dac_line_format14(15:12), dac_vstart_pos14(10:0)
		 0<<12 |   0,	// 0x1C : dac_line_format15(15:12), dac_vstart_pos15(10:0)
		 0<<12 |   0,	// 0x1D : dac_line_format16(15:12), dac_vstart_pos16(10:0)
		 1<<15 | 717,	// 0x1E : dac_scale_g_en(15), dac_scale_g(9:0)            
		 1<<15 | 717,	// 0x1F : dac_scale_b_en(15), dac_scale_b(9:0)            
		 1<<15 | 717,	// 0x20 : dac_scale_r_en(15), dac_scale_r(9:0)            
		 1<<15 | 307,	// 0x21 : dac_clip_l_g_en(15), dac_clip_l_g(9:0)          
		 1<<15 | 1023,	// 0x22 : dac_clip_u_g_en(15), dac_clip_u_g(9:0)          
		 1<<15 | 153,	// 0x23 : dac_clip_l_br_en(15), dac_clip_l_br(9:0)        
		 1<<15 | 871	// 0x24 : dac_clip_u_br_en(15), dac_clip_u_br(9:0)        
	}
};
	
static  MDIN_OUTTMDS_CTRL defMDINOutTMDS[]	= {
//	 iod     iorv
//	{  4, (1<<3)|0},	// 1440x480i60 
//	{  4, (1<<3)|0},	// 1440x576i50 
	{  4, (1<<3)|0},	// 720x480i60  
	{  4, (1<<3)|0},	// 720x576i50  
	{  4, (1<<3)|0},	// 720x480p60  
	{  4, (1<<3)|0},	// 720x576p50  
	{  7, (0<<3)|5},	// 1280x720p60 
	{  7, (0<<3)|5},	// 1280x720p50 
	{  7, (0<<3)|5},	// 1920x1080i60
	{  7, (0<<3)|5},	// 1920x1080i50
	{  7, (0<<3)|5},	// 1920x1080p60
	{  7, (0<<3)|5},	// 1920x1080p50
	{  4, (1<<3)|0},	// 640x480p60  
	{  4, (1<<3)|0},	// 640x480p75  
	{  4, (1<<3)|0},	// 800x480p60	// @todo : 셋팅해야 한다.
	{  4, (1<<3)|0},	// 800x600p60  
	{  4, (1<<3)|0},	// 800x600p75  
	{  7, (0<<3)|5},	// 1024x768p60 
	{  7, (0<<3)|5},	// 1024x768p75 
	{  7, (0<<3)|5},	// 1280x1024p60
	{  7, (0<<3)|5},	// 1280x1024p75

	{  7, (0<<3)|5},	// 1360x768p60 
	{  7, (0<<3)|5},	// 1440x900p60
	{  7, (0<<3)|5},	// 1440x900p75
	{  7, (0<<3)|5},	// 1680x1050p60
	{  7, (0<<3)|5},	// 1600x1200p60

	{  7, (0<<3)|5},	// 1680x1050pRB
	{  7, (0<<3)|5},	// 1920x1080pRB
	{  7, (0<<3)|5}		// 1920x1200pRB
};

// default value for srcVideo format
static  WORD defMDINSrcVideo[][4]	= {
//	 Htot					H/V Polarity & ScanType                    Hact  Vact
	{ 858, MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_INTR,  720,  480},	// 720x480i60
	{ 864, MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_INTR,  720,  576},	// 720x576i50
	{ 858, MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_PROG,  720,  480},	// 720x480p60
	{ 864, MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_PROG,  720,  576},	// 720x576p50
	{1650, MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_PROG, 1280,  720},	// 1280x720p60
	{1980, MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_PROG, 1280,  720},	// 1280x720p50
	{2200, MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_PROG, 1920, 1080},	// 1920x1080p30
	{2640, MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_PROG, 1920, 1080},	// 1920x1080p25
	{2200, MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_INTR, 1920, 1080},	// 1920x1080i60
	{2640, MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_INTR, 1920, 1080},	// 1920x1080i50
	{ 800, MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_PROG,  640,  480},	// 640x480pANY
	{ 992, MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_PROG,  800,  480},	// 800x480pANY
	{1056, MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_PROG,  800,  600},	// 800x600pANY
	{1344, MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_PROG, 1024,  768},	// 1024x768pANY
	{1688, MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_PROG, 1280, 1024}	// 1280x1024pANY
};

// default value for outVideo format
static  WORD defMDINOutVideo[][3]	= {
//	 						H/V Polarity & ScanType & FreeRun					   Hact  Vact 
//	{MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_INTR|MDIN_SYNC_FREERUN, 1440,  480},	// 1440x480i60
//	{MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_INTR|MDIN_SYNC_FREERUN, 1440,  576},	// 1440x576i50
	{MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_INTR|MDIN_SYNC_FREERUN,  720,  480},	// 720x480i60
	{MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_INTR|MDIN_SYNC_FREERUN,  720,  576},	// 720x576i50
	{MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN,  720,  480},	// 720x480p60
	{MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN,  720,  576},	// 720x576p50
	{MDIN_POSITIVE_HSYNC|MDIN_POSITIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN, 1280,  720},	// 1280x720p60
	{MDIN_POSITIVE_HSYNC|MDIN_POSITIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN, 1280,  720},	// 1280x720p50
	{MDIN_POSITIVE_HSYNC|MDIN_POSITIVE_VSYNC|MDIN_SCANTYPE_INTR|MDIN_SYNC_FREERUN, 1920, 1080},	// 1920x1080i60
	{MDIN_POSITIVE_HSYNC|MDIN_POSITIVE_VSYNC|MDIN_SCANTYPE_INTR|MDIN_SYNC_FREERUN, 1920, 1080},	// 1920x1080i50
	{MDIN_POSITIVE_HSYNC|MDIN_POSITIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN, 1920, 1080},	// 1920x1080p60
	{MDIN_POSITIVE_HSYNC|MDIN_POSITIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN, 1920, 1080},	// 1920x1080p50
	{MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN,  640,  480},	// 640x480p60
	{MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN,  640,  480},	// 640x480p75
	{MDIN_POSITIVE_HSYNC|MDIN_POSITIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN,  800,  480},	// 800x480p60
	{MDIN_POSITIVE_HSYNC|MDIN_POSITIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN,  800,  600},	// 800x600p60
	{MDIN_POSITIVE_HSYNC|MDIN_POSITIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN,  800,  600},	// 800x600p75
	{MDIN_NEGATIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN, 1024,  768},	// 1024x768p60
	{MDIN_POSITIVE_HSYNC|MDIN_POSITIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN, 1024,  768},	// 1024x768p75
	{MDIN_POSITIVE_HSYNC|MDIN_POSITIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN, 1280, 1024},	// 1280x1024p60
	{MDIN_POSITIVE_HSYNC|MDIN_POSITIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN, 1280, 1024},	// 1280x1024p75

	{MDIN_POSITIVE_HSYNC|MDIN_POSITIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN, 1360,  768},	// 1360x768p60
	{MDIN_NEGATIVE_HSYNC|MDIN_POSITIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN, 1440,  900},	// 1440x900p60
	{MDIN_NEGATIVE_HSYNC|MDIN_POSITIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN, 1440,  900},	// 1440x900p75
	{MDIN_NEGATIVE_HSYNC|MDIN_POSITIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN, 1680, 1050},	// 1680x1050p60	
	{MDIN_POSITIVE_HSYNC|MDIN_POSITIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN, 1600, 1200},	// 1600x1200p60

	{MDIN_POSITIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN, 1680, 1050},	// 1680x1050pRB	
	{MDIN_POSITIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN, 1920, 1080},	// 1920x1080pRB	
	{MDIN_POSITIVE_HSYNC|MDIN_NEGATIVE_VSYNC|MDIN_SCANTYPE_PROG|MDIN_SYNC_FREERUN, 1920, 1200}	// 1920x1200pRB	
};

// -----------------------------------------------------------------------------
// Exported function Prototype
// -----------------------------------------------------------------------------


#endif	/* __MDINFRMT_H__ */
