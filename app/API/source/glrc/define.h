// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore API v 1.0
// Date : 

#pragma once

#include <rc_define.h>

#define LOOP(m) for(i = 0; i < m; i++)

//#define   	FLAG_V								0x2
//#define   	FLAG_N								0x4
//#define   	FLAG_T								0x8

//#define	    FLAG_TEXTURE						0x10
//#define   	FLAG_LOD							0x20
//#define   	FLAG_MIPMAP         				0x40
//#define   	FLAG_VERTEXLIST						0x80

//#define   	FLAG_BACKGROUND_SET                 0x100
//#define   	FLAG_BACKGROUND_SETTING             0x200

//#define   	FLAG_TEXTURE_LOADED					0x1000
//#define   	FLAG_MATRIX_MODELVIEW_INVERSE		0x2000
//#define   	FLAG_ERROR							0x4000
//#define		FLAG_SHADOW 				        0x8000

//#define		FLAG_COLOR_SHADOW					0x10000
//#define		FLAG_SUPERSAMPLE    				0x20000
//#define   	FLAG_MATRIX_PALETTE  				0x40000

#define 	SIZEOFIDENTITYMATRIX                64

//32 bit float
//#define FLOAT_MAX          3.40282346e+38F        /* max value */
//#define FLOAT_MIN         -1.17549434e+38F        /* min value */

//24 bit float
#define FLOAT_MAX          1.8446e+19F			/* max value */
#define FLOAT_MIN         -9.2233e+18F        /* min value */

// light radius
#define MaxLightRadius	8191
#define MinLightRadius	1			
// light radius

// diffuse generation rate
#define MaxGenerationRate	10
#define MinGenerationRate	0			
// diffuse generation rate

// spp
#define MaxSppPower 5
#define MinSppPower 0

#define MaxSppCount 32
#define MinSppCount 1
// spp
