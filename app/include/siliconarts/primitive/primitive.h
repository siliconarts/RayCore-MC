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
// Project : RayCore
// DATE    : 8/28/2013 Wed
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#pragma once
#include "driver/RCStructures.h"

/************************************************************************/
/* external 24bit include data structure */
/************************************************************************/

// vertex structure
typedef struct VERTEX24{
	RCVECTOR3i		normal;						// normal [x, y, z]
    union{
		RCVECTOR2i		textureCoordinate;		// texture coordinate [u, v]
        DWORD			radius[2];				// radius [value, null]
    };
}VERTEX24;

// accelerated primitive structure
typedef struct PRIMITIVE24 {

	int				type;						// 0 : triangle, 1 : box, 2 : disk, 3 : sphere
	int				axis;						// projection dimension k

	VERTEX24		vertex[3];					// vertex of triangle [A, B, C]
	DWORD			baseLOD;					// base value for calculating LOD
	int				materialIndex;				// material index

	DWORD			nu;							// normal.u / normal.k
	DWORD			nv;							// normal.v / normal.k
	DWORD			nd;							// constant of plane equation
	union{
		DWORD			bnu;					// for triangle
		DWORD			centerX;				// for disk
	};
	union{
		DWORD			bnv;					// for triangle
		DWORD			centerY;				// for disk
	};
	union{
		DWORD			bd;						// for triangle
		DWORD			centerZ;				// for disk
	};
	union{
		DWORD			cnu;					// for triangle
		DWORD			radius;					// for disk
	};
	DWORD			cnv;
	DWORD			cd;
} PRIMITIVE24;


typedef union{
	DWORD	data[8];
	struct{
		DWORD		bd				: 24;
		DWORD		cd_l			: 8;
		DWORD		cd_h			: 16;
		DWORD		bnu_l			: 16;
		DWORD		bnu_h			: 8;
		DWORD		bnv				: 24;
		DWORD		cnu				: 24;
		DWORD		cnv_l			: 8;
		DWORD		cnv_h			: 16;
		DWORD		nd_l			: 16;
		DWORD		nd_h			: 8;
		DWORD		nu				: 24;
		DWORD		nv				: 24;
		DWORD		/*dummy*/		: 0;
		DWORD		mat_index		: 12;
		DWORD		axis			: 2;
		//DWORD		flag			: 3;
		DWORD		transmittance_shadow	: 1;
		DWORD		texture_only			: 1;
		DWORD		texture_alpha_shadow	: 1;
		DWORD		/*dummy*/		: 0;
	}triangle;
}DDK_PRIMITIVE;

typedef union{
	DWORD	data[16];
	struct{
		DWORD		v0_norm_x		: 24;
		DWORD		v0_norm_y_l		: 8;
		DWORD		v0_norm_y_h		: 16;
		DWORD		v0_norm_z_l		: 16;
		DWORD		v0_norm_z_h		: 8;
		DWORD		v1_norm_x		: 24;
		DWORD		v1_norm_y		: 24;
		DWORD		v1_norm_z_l		: 8;
		DWORD		v1_norm_z_h		: 16;
		DWORD		v2_norm_x_l		: 16;
		DWORD		v2_norm_x_h		: 8;
		DWORD		v2_norm_y		: 24;
		DWORD		v2_norm_z		: 24;
		DWORD		v0_tex_u_l		: 8;
		DWORD		v0_tex_u_h		: 16;
		DWORD		v0_tex_v_l		: 16;
		DWORD		v0_tex_v_h		: 8;
		DWORD		v1_tex_u		: 24;
		DWORD		v1_tex_v		: 24;
		DWORD		v2_tex_u_l		: 8;
		DWORD		v2_tex_u_h		: 16;
		DWORD		v2_tex_v_l		: 16;
		DWORD		v2_tex_v_h		: 8;
		DWORD		base_lod		: 24;
		DWORD		material_index	: 12;
		DWORD		/*dummy*/		: 2;
		//DWORD		flag			: 3;
		DWORD		transmittance_shadow	: 1;
		DWORD		texture_only			: 1;
		DWORD		texture_alpha_shadow	: 1;
		DWORD		/*dummy*/		: 0;
		DWORD		/*dummy*/		: 32;
		DWORD		/*dummy*/		: 32;
		DWORD		/*dummy*/		: 32;
	};
}DDK_PRIMITIVE_INFO;

#define PACKING_PRIMITIVE_POSITION(in_pname, in_materal_array24, out_pname)		\
            out_pname->data[0] = PACKING(in_pname.cd,  in_pname.bd,  24);		\
            out_pname->data[1] = PACKING(in_pname.bnu, in_pname.cd,  16);		\
            out_pname->data[2] = PACKING(in_pname.bnv, in_pname.bnu,  8);		\
            out_pname->data[3] = PACKING(in_pname.cnv, in_pname.cnu, 24);		\
            out_pname->data[4] = PACKING(in_pname.nd,  in_pname.cnv, 16);		\
            out_pname->data[5] = PACKING(in_pname.nu,  in_pname.nd,   8);		\
            out_pname->data[6] = PACKING(0x0, in_pname.nv, 24);				\
            out_pname->data[7] = ((in_materal_array24 & 0x7) << 14) | ((in_pname.axis & 0x3)<<12) | (in_pname.materialIndex & 0xFFFF)


#define PACKING_PRIMITIVE_INFORMATION(in_pname, in_materal_array24, out_acname)          \
            out_acname->data[0]  = PACKING(in_pname.vertex[0].normal.y, in_pname.vertex[0].normal.x, 24);    \
            out_acname->data[1]  = PACKING(in_pname.vertex[0].normal.z, in_pname.vertex[0].normal.y, 16);    \
            out_acname->data[2]  = PACKING(in_pname.vertex[1].normal.x, in_pname.vertex[0].normal.z,  8);    \
            out_acname->data[3]  = PACKING(in_pname.vertex[1].normal.z, in_pname.vertex[1].normal.y, 24);    \
            out_acname->data[4]  = PACKING(in_pname.vertex[2].normal.x, in_pname.vertex[1].normal.z, 16);    \
            out_acname->data[5]  = PACKING(in_pname.vertex[2].normal.y, in_pname.vertex[2].normal.x,  8);    \
            out_acname->data[6]  = PACKING(in_pname.vertex[0].textureCoordinate.u, in_pname.vertex[2].normal.z, 24);    \
            out_acname->data[7]  = PACKING(in_pname.vertex[0].textureCoordinate.v, in_pname.vertex[0].textureCoordinate.u, 16);    \
            out_acname->data[8]  = PACKING(in_pname.vertex[1].textureCoordinate.u, in_pname.vertex[0].textureCoordinate.v,  8);    \
            out_acname->data[9]  = PACKING(in_pname.vertex[2].textureCoordinate.u, in_pname.vertex[1].textureCoordinate.v, 24);    \
            out_acname->data[10] = PACKING(in_pname.vertex[2].textureCoordinate.v, in_pname.vertex[2].textureCoordinate.u, 16);    \
            out_acname->data[11] = PACKING(in_pname.baseLOD, in_pname.vertex[2].textureCoordinate.v,  8);      \
            out_acname->data[12] = ((in_materal_array24 & 0x7) << 14) | (in_pname.materialIndex & 0xFFFF);     \
            out_acname->data[13] = 0;    \
            out_acname->data[14] = 0;    \
            out_acname->data[15] = 0
