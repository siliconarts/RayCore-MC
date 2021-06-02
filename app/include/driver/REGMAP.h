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
// DATE    : 8/19/2013 Mon
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#ifndef __REGISTER_MAP_H__
#define __REGISTER_MAP_H__

#include "driver/RCStructures.h"

typedef enum{
	REG_DISPLAY_WIDTH		= 0x800,		// r/w	: 디스플레이의 너비를 지정
	REG_DISPLAY_HEIGHT,						// r/w	: 디스플레이의 높이를 지정
	REG_DISPLAY_FORMAT,						// r/w	: 디스플레이의 색상 포멧
	REG_DISPLAY_BYTE_STRIDE,				// r/w	: 디스플레이의 너비 메모리 폭을 지정
	REG_DISPLAY_ADDRESS_FRONT,				// r/w	: 디스플레이 Front buffer 물리 주소를 지정한다.
	REG_DISPLAY_ADDRESS_BACK,				// r/w	: 디스플레이 Back buffer 물리 주소를 지정한다.
	REG_DISPLAY_UPDATE_FRONT,				// w	: temporary, Front buffer 업데이트 (에뮬에서만 존재)
	REG_DISPLAY_UPDATE_BACK,				// w	: temporary, Back buffer 업데이트 (에뮬에서만 존재)
};

//----------------------------------------------------------------------------------------------------------------
// Information
#define REG_INFO_SIZE		4

typedef union{
	unsigned int		reg[REG_INFO_SIZE];
	struct{
		unsigned int		magic_code;		// "SARC" = 0x43524153

		unsigned int		series_id		: 16;
		unsigned int		/*dummy*/		: 0;

		unsigned int		raycore_id		: 16;
		unsigned int		ddr_size		: 4;
		unsigned int		/*dummy*/		: 0;

		unsigned int		build_time;
	};
}REG_INFO;

//----------------------------------------------------------------------------------------------------------------
// Configuration
#define REG_CONFIG_SIZE		8

typedef union{
	unsigned int		reg[REG_CONFIG_SIZE];
	struct{
		struct{
			unsigned int		node;
			unsigned int		primitive;
			unsigned int		list;
			unsigned int		transmit;
			unsigned int		primitive_info;
			unsigned int		texture;
			unsigned int		framebuffer_0;
			unsigned int		framebuffer_1;
		}base;
	};
}REG_CONFIG;

//----------------------------------------------------------------------------------------------------------------
// Control
#define REG_CONTROL_SIZE		16

typedef union{
	unsigned int	reg[REG_CONTROL_SIZE];
	struct{
		union{
			unsigned int			all;
			struct{
				unsigned short		width;				// screen width (12bits)
				unsigned short		height;				// screen height (12bits)
			};
		}resolution;

		RCVECTOR2u				pixel_size;
		RCVECTOR2u				start;
		RCVECTOR3u				background;				// background red 24bit float [0.0~1.0]

		struct{
			unsigned int		core_thread_end	: 24;
			unsigned int		chip_mask		: 4;	// FPGA only
			unsigned int		/*dummy*/		: 2;
			unsigned int		ddr_init_done	: 1;	// FPGA only
			unsigned int		interrupt_enable	: 1;	// interrupt enable
		}status;

		unsigned int		tnl_timeout_cnt		: 16;
		unsigned int		/*dummy*/			: 16;
		unsigned int		interrupt_mask;

		struct{
			unsigned int		tnl_timeout		: 30;
			unsigned int		thread_end		: 1;
			unsigned int		fb_write_done	: 1;	// framebuffer write done
		}interrupt_status;

		unsigned int		fb_stride;					// framebuffer stride	: * 4byte

		unsigned int		core_mask;					// add for core run 
		unsigned int		reserved;					// hjkwon 20.01.20 radius setup

		union{
			unsigned int			all;
			struct{
				unsigned int		sw_reset			: 1;	// software reset(active low)
				unsigned int		start				: 1;	// rendering start
				unsigned int		end					: 1;	// rendering end
				unsigned int		cache_clear			: 1;	// cache clear
				unsigned int		/*dummy*/			: 0;
			};
		}control;	
	};
}REG_CONTROL;

//----------------------------------------------------------------------------------------------------------------
// Option
#define REG_OPTION_SIZE		22

typedef struct{
	RCVECTOR3u		eye;				// camera position XYZ
	RCVECTOR3u		basis[3];			// camera basis 0/1/2 XYZ
	unsigned int	distance;			// distance from screen to camera
	struct{
		unsigned int	start;			// attenuation start
		unsigned int	end;			// attenuation end
		unsigned int	weight[3];		// attenuation coefficient 0/1/2
	}atten;
}REG_CAMERA;

typedef union{
	unsigned int	reg[REG_OPTION_SIZE];
	struct{

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

				unsigned int	light_number	: 11;	// number of light (Max:1024)
				unsigned int	/*dummy*/		: 5;
				unsigned int	adaptive_size	: 6;	// adaptive rendering quality
				unsigned int	/*dummy*/		: 2;
				unsigned int	ray_bounce		: 5;	// maximum ray bounce :  
				unsigned int	/*dummy*/		: 2;
				unsigned int	camera_atten	: 1;	// camera attenuation enable
				unsigned int	/*dummy*/		: 0;

				
			};
		}set;

		unsigned int	reserved;
		
		unsigned int	threshold_weight;		// threshold weight

		REG_CAMERA		camera;
	};
}REG_OPTION;

//----------------------------------------------------------------------------------------------------------------
// Acceleration structure information
#define REG_AST_SIZE		14

typedef union{
	unsigned int		reg[REG_AST_SIZE];
	struct{
		unsigned int	def_list_num;
		unsigned int	def_list_addr;
		struct{
			unsigned int	index;				// node tree address for root bounding box
			RCVECTOR3u		min;				// minimum XYZ
			RCVECTOR3u		max;				// maximum XYZ
		}bound_box;
		RCVECTOR3u			translate;			// not used yet

 		unsigned int	AODist;					// reserved
		unsigned int	MinDist;				// 24 bit float exponent [22:16]
	};
}REG_AST;

//----------------------------------------------------------------------------------------------------------------
// Report
#define REG_REPORT_SIZE		14

typedef union{
	unsigned int		reg[REG_REPORT_SIZE];
	struct{
		unsigned int	tick_count;				// tick clock count
		unsigned int	frame_write_address;	// current frame write address
		unsigned int	bus_read_req_count;		// bus read request count
		unsigned int	reserved[5];
		unsigned int	ray_count[6];
	};
}REG_REPORT;

//----------------------------------------------------------------------------------------------------------------
// Light
#define REG_LIGHT_SIZE		24

typedef union{
	unsigned int		reg[REG_LIGHT_SIZE];
	struct{
		RCVECTOR3u		position;		// light position XYZ
		RCVECTOR3u		direction;		// light direction XYZ
		struct{
			unsigned int	cos_in, cos_out, falloff;	// spot light cos(theta) of in-cone, cos(theta) of out-cone, fall-off value
		}spot;
		RCVECTOR3u		ambient;		// light ambient RGB
		RCVECTOR3u		diffuse;		// light diffuse RGB
		RCVECTOR3u		specular;		// light specular RGB
		struct{
			unsigned int	start,end;	// start/end attenuation
			unsigned int	weight[3];	// coefficient 0/1/2
			union{
				unsigned int	mode;
				struct{
					unsigned int	type		: 2;	// light type(0: point, 1: directional, 2: spot)
					unsigned int	enable		: 1;	// attenuation enable
					unsigned int	/*dummy*/	: 0;
				};
			};
		}atten;
	};
}REG_LIGHT;

//----------------------------------------------------------------------------------------------------------------
// Material
#define REG_MATERIAL_SIZE	16

typedef union{
	unsigned int	all;
	struct{
		unsigned int	address				: 18;
		unsigned int	log_width			: 4;
		unsigned int	log_height			: 4;
		unsigned int	repeat_u			: 1;
		unsigned int	repeat_v			: 1;
		unsigned int	mirror_u			: 1;	// reserved
		unsigned int	mirror_v			: 1;	// reserved
		unsigned int	mipmap				: 1;	// mipmap enable
		unsigned int	enable				: 1;	// texture enable
	};
}MATERIAL_TEXTURE;

typedef union{
	unsigned int	reg[REG_MATERIAL_SIZE];
	struct{
		RCVECTOR3u			ambient;		// [0~1]
		RCVECTOR3u			diffuse;		// [0~1]
		struct{
			unsigned int		r,g,b;		// [0~1] //specular
			unsigned int		level;		// [0~255)
			unsigned int		exponent;
		}specular;

		unsigned int		reflectance;	// [0~1]
		unsigned int		transmittance;	// [0~1] //dielectric
		unsigned int		refraction_index;

		MATERIAL_TEXTURE	texture, normal;
	};
}REG_MATERIAL;

//----------------------------------------------------------------------------------------------------------------
// Local SRam Memory Map
#define RC_REG_OFFSET_INFO			0x00000000
#define RC_REG_OFFSET_CONFIG		0x00000020
#define RC_REG_OFFSET_CONTROL		0x00000080
#define RC_REG_OFFSET_OPTION		0x00000100
#define RC_REG_OFFSET_REPORT		0x00001000
#define RC_REG_OFFSET_AST			0x00003000

//}hjkwon - 20190408
//For 1k lights support
//Internal Reg Map Change
//}
//#define RC_REG_OFFSET_LIGHT			0x00010000
//#define RC_REG_OFFSET_MATERIAL		0x00020000
#define RC_REG_OFFSET_MATERIAL		0x00010000
#define RC_REG_OFFSET_LIGHT  		0x00020000


#endif//__REGISTER_MAP_H__
