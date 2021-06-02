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
#ifndef __RC_STRUCTURES_H__
#define __RC_STRUCTURES_H__
#include "STDInterface.h"
#include "ddk/rcDDK_Defines.h"

// base structures
typedef union{
	float	m[2];
	struct{ float	x, y;};
	struct{ float	u, v;};
}RCVECTOR2f;

typedef union{
	int		m[2];
	struct{ int		x, y;};
	struct{ int		u, v;};
}RCVECTOR2i;

typedef union{
	DWORD	m[2];
	struct{ DWORD	x, y;};
	struct{ DWORD	u, v;};
}RCVECTOR2u;

typedef union{
	float	m[3];
	struct{	float	x, y, z;};
	struct{	float	r, g, b;};
	struct{ float	u, v, s;};
}RCVECTOR3f;

typedef union{
	int		m[3];
	struct{	int		x, y, z;};
	struct{	int		r, g, b;};
	struct{ int		u, v, s;};
}RCVECTOR3i;

typedef union{
	DWORD	m[3];
	struct{	DWORD	x, y, z;};
	struct{	DWORD	r, g, b;};
	struct{ DWORD	u, v, s;};
}RCVECTOR3u;

typedef union{
	float	m[4];
	struct{	float	x, y, z, w;};
	struct{	float	r, g, b, a;};
	struct{ float	u, v, s, t;};
}RCVECTOR4f;

typedef union{
	int		m[4];
	struct{	int		x, y, z, w;};
	struct{	int		r, g, b, a;};
	struct{ int		u, v, s, t;};
}RCVECTOR4i;

typedef union{
	DWORD	m[4];
	struct{	DWORD	x, y, z, w;};
	struct{	DWORD	r, g, b, a;};
	struct{ DWORD	u, v, s, t;};
}RCVECTOR4u;

typedef union{
	float	m[4][4];
	struct{
		float	m_00, m_01, m_02, m_03;
		float	m_10, m_11, m_12, m_13;
		float	m_20, m_21, m_22, m_23;
		float	m_30, m_31, m_32, m_33;
	};
}RCMATRIX16f;
//----------------------------------------------------------------------------------------------------------------
// config
typedef struct {
	struct{
		PVOID	node;
		PVOID	primitive;
		PVOID	list;
		PVOID	transmit;
		PVOID	primitive_info;
		PVOID	texture;
		PVOID	framebuffer_0;
		PVOID	framebuffer_1;
	}base;
} RC_CONFIG;

//----------------------------------------------------------------------------------------------------------------
// control
typedef struct {
	struct{
		DWORD		width, height;
	}resolution;
	RCVECTOR2f		pixel_size;
	RCVECTOR2f		start;
	RCVECTOR3f		background;
	DWORD			fb_stride;
} RC_CONTROL;

//----------------------------------------------------------------------------------------------------------------
// option
typedef struct{
	union{
		unsigned int		all[2];
		struct{
			unsigned int	shadow			: 1;	// shadow on/off
			unsigned int	color_shadow	: 1;	// transmit shadow on/off
			unsigned int	lod				: 1;	// lod on/off
			unsigned int	super_sampling	: 1;	// super-sampling on/off
			unsigned int	adaptive_render	: 1;	// enable adaptive rendering
			unsigned int	normal_map		: 1;	// normal map
			unsigned int	/*dummy*/		: 10;
			unsigned int	rgba_mode		: 2;	// reserved
			unsigned int	/*dummy*/		: 0;

			//}hjkwon - 20190408
			//For 1k lights support
			//Option Parameter modification
			//}
			//unsigned int	light_number	: 4;	// number of light
			//unsigned int	/*dummy*/		: 4;
			//unsigned int	adaptive_size	: 6;	// adaptive rendering quality
			//unsigned int	/*dummy*/		: 2;
			//unsigned int	camera_atten	: 1;	// camera attenuation enable
			//unsigned int	/*dummy*/		: 7;
			//unsigned int	ray_bounce		: 4;	// maximum ray bounce : 실제 5비트이지만 5번째 비트확장되어 음수가 적용되고 있다. --;
			//unsigned int	/*dummy*/		: 0;

			unsigned int	light_number : 11;	// number of light (Max:1024)
			unsigned int	/*dummy*/ : 5;
			unsigned int	adaptive_size : 6;	// adaptive rendering quality
			unsigned int	/*dummy*/ : 2;
			unsigned int	ray_bounce : 5;	// maximum ray bounce : 실제 5비트이지만 5번째 비트확장되어 음수가 적용되고 있다. --;
			unsigned int	/*dummy*/ : 2;
			unsigned int	camera_atten : 1;	// camera attenuation enable
			unsigned int	/*dummy*/ : 0;
		};
	}set;

	float		threshold_weight;

	struct{
		RCVECTOR3f	eye;			// camera position XYZ
		RCVECTOR3f	basis[3];		// camera basis 0/1/2 XYZ
		float	distance;			// distance from screen to camera
		struct{
			float	start;			// attenuation start
			float	end;			// attenuation end
			float	weight[3];		// attenuation coefficient 0/1/2
		}atten;
	}camera;

// 	int			NodeCount;
// 	int			TriangleCount;
// 	int			ListCount;
// 
// 	int			DefPrimNum;
// 	int			DefListAddr;
}RC_OPTION;

//----------------------------------------------------------------------------------------------------------------
// AST
typedef struct {
	DWORD	def_list_num;
	DWORD	def_list_addr;
	struct{
		DWORD		index;			// node tree address for root bounding box
		RCVECTOR3f	min;			// minimum XYZ
		RCVECTOR3f	max;			// maximum XYZ
	}bound_box;
	struct{
		float	x,y,z;				// not used yet
	}translate;
	float	AODist;					// reserved
	float	MinDist;				//
}RC_AST;

//----------------------------------------------------------------------------------------------------------------
// light
typedef struct {
	RCVECTOR3f		position;		// position
	float			positionWeight;	// w component of the position
	RCVECTOR3f		direction;		// direction
	float			spot[3];		// Spot light in-cone, out-cone, fall-off value
	RCVECTOR3f		ambient;	    // light ambient color
	RCVECTOR3f		diffuse;	    // light diffuse color
	RCVECTOR3f		specular;		// light specular color
	float			attenStart;		// start
	float			attenEnd;		// end
	float			attenWeight[3];	// weight 0/1/2
	RCVECTOR3f		staticAmbient;	//dk
	union{
		unsigned int	all;
		struct{
			unsigned int	type			: 2;	// light type(0: point, 1: directional, 2: spot)
			unsigned int	enable			: 1;
			unsigned int	/*dummy*/		: 0;
		};
		// light type 0 : point light + no attenuation
		//            1 : directional light + no attenuation
		//            2 : spot light + no attenuation
		//            4 : point light + attenuation
		//            5 : directional light + attenuation
		//            6 : spot light + attenuation
	}atten;
} RC_LIGHT;

//----------------------------------------------------------------------------------------------------------------
// material

typedef struct{
	BOOL			enable;
	PVOID			pData;
	DWORD			width, height;
	BOOL			mipmap;
	struct{
		BOOL		u,v;
	}repeat;
	struct{
		BOOL		u,v;
	}mirror;
}RC_MATERIAL_TEXTURE;

typedef struct{
	RCVECTOR3f		ambient;
	RCVECTOR3f		diffuse;
	RCVECTOR3f		specular;
	float			specular_level;
	float			specular_exponent;

	float			reflectance;
	float			transmittance;
	float			refraction_index;

	struct{
		RC_MATERIAL_TEXTURE	diffuse, normal;
	}tex;
}RC_MATERIAL;

//----------------------------------------------------------------------------------------------------------------
// primitive
// vertex structure
typedef struct{
	RCVECTOR3f		position;
	RCVECTOR3f		normal;
	RCVECTOR2f		texcoord;
}RC_VERTEX;

// accelerated primitive structure
typedef struct{
	int				type;				// 0 : triangle, 1 : box, 2 : disk, 3 : sphere
	int				axis;				// projection dimension k

	RC_VERTEX		vertex[3];			// vertex of triangle [A, B, C]
	float			baseLOD;			// base value for calculating LOD
	int				materialIndex;		// material index
	int				Flag;

	float       nu;                     // normal.u / normal.k
	float       nv;                     // normal.v / normal.k
	float       nd;                     // constant of plane equation
	union{
		struct{
			float	bnu, bnv, bd, cnu;
		}triangle;
		struct{
			float	centerX, centerY, centerZ, radius;
		}disk;
	};
	float       cnv;
	float       cd;
} RC_PRIMITIVE;

//----------------------------------------------------------------------------------------------------------------
// list

//----------------------------------------------------------------------------------------------------------------
// node
typedef struct{
	DWORD	number;
	DWORD	index;
	DWORD	axis;
}RC_NODE;

//----------------------------------------------------------------------------------------------------------------
// texture
typedef struct{
	unsigned int	index;
	int				textureSize;
	union{
		int			m[2];
		struct{
			int		width, height;
		};
	}resolution;					// texture resolution [x, y]
	int				colorBit;       // texture color bit ( 24bit : r, g, b, 32bit : r, g, b, a )
	unsigned int	*colorData;     // 32bit unsigned integer texture color
	int				LODIndex[2];    // level of detail index [x, y]
	unsigned int	address;        // texture address
} RC_TEXTURE_DATA;

//----------------------------------------------------------------------------------------------------------------
// transmittance data
typedef union{
	WORD		m[4];
	struct{
		WORD		r,g,b;
		WORD		a;			// not used at this time.
	};
} RC_TRANSMITTANCE_DATA;

#endif//__RC_STRUCTURES_H__
