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
//=======================================================================

#ifndef __OPENRC_H__
#define __OPENRC_H__

#ifdef WIN32
#include <windows.h>
#endif

#ifdef WIN32
	#define RC_API			__declspec(dllexport)
	#define RC_APIENTRY		__stdcall
#else //ANDROID_NDK
	#ifdef __cplusplus
		#define RC_API			extern "C"
	#else
		#define RC_API			extern
	#endif
	#define RC_APIENTRY
#endif // _LIBRARY


#ifdef __cplusplus
extern "C" {
#endif

/*
 * This document is licensed under the SGI Free Software B License Version
 * 2.0. For details, see http://oss.sgi.com/projects/FreeB/ .
 */

typedef int              RCfixed;
typedef int              RCclampx;

typedef signed long int  RCintptr;
typedef signed long int  RCsizeiptr;


typedef void                RCvoid;
typedef char                RCchar;
typedef unsigned int        RCenum;
typedef unsigned char       RCboolean;
typedef unsigned int        RCbitfield;
typedef unsigned char       RCbyte;
typedef short               RCshort;
typedef int                 RCint;
typedef int                 RCsizei;
typedef unsigned char       RCubyte;
typedef unsigned short      RCushort;
typedef unsigned int        RCuint;
typedef float               RCfloat;
typedef float               RCclampf;

typedef double              RCdouble;


/*************************************************************/

/* OpenRC ES core versions */
#define RC_VERSION_ES_CM_1_0          1
#define RC_VERSION_ES_CL_1_0          1
#define RC_VERSION_ES_CM_1_1          1
#define RC_VERSION_ES_CL_1_1          1

/* ClearBufferMask */
#define RC_DEPTH_BUFFER_BIT               0x00000100
#define RC_STENCIL_BUFFER_BIT             0x00000400
#define RC_COLOR_BUFFER_BIT               0x00004000

/* Boolean */
#define RC_FALSE                          0
#define RC_TRUE                           1

/* BeginMode */
#define RC_POINTS                         0x0000
#define RC_LINES                          0x0001
#define RC_LINE_LOOP                      0x0002
#define RC_LINE_STRIP                     0x0003
#define RC_TRIANGLES                      0x0004
#define RC_TRIANGLE_STRIP                 0x0005
#define RC_TRIANGLE_FAN                   0x0006
#define RC_QUADS                          0x0007

/* AlphaFunction */
#define RC_NEVER                          0x0200
#define RC_LESS                           0x0201
#define RC_EQUAL                          0x0202
#define RC_LEQUAL                         0x0203
#define RC_GREATER                        0x0204
#define RC_NOTEQUAL                       0x0205
#define RC_GEQUAL                         0x0206
#define RC_ALWAYS                         0x0207

/* BlendingFactorDest */
#define RC_ZERO                           0
#define RC_ONE                            1
#define RC_SRC_COLOR                      0x0300
#define RC_ONE_MINUS_SRC_COLOR            0x0301
#define RC_SRC_ALPHA                      0x0302
#define RC_ONE_MINUS_SRC_ALPHA            0x0303
#define RC_DST_ALPHA                      0x0304
#define RC_ONE_MINUS_DST_ALPHA            0x0305

/* BlendingFactorSrc */
/*      RC_ZERO */
/*      RC_ONE */
#define RC_DST_COLOR                      0x0306
#define RC_ONE_MINUS_DST_COLOR            0x0307
#define RC_SRC_ALPHA_SATURATE             0x0308
/*      RC_SRC_ALPHA */
/*      RC_ONE_MINUS_SRC_ALPHA */
/*      RC_DST_ALPHA */
/*      RC_ONE_MINUS_DST_ALPHA */

/* ClipPlaneName */
#define RC_CLIP_PLANE0                    0x3000
#define RC_CLIP_PLANE1                    0x3001
#define RC_CLIP_PLANE2                    0x3002
#define RC_CLIP_PLANE3                    0x3003
#define RC_CLIP_PLANE4                    0x3004
#define RC_CLIP_PLANE5                    0x3005

/* ColorMaterialFace */
/*      RC_FRONT_AND_BACK */

/* ColorMaterialParameter */
/*      RC_AMBIENT_AND_DIFFUSE */

/* ColorPointerType */
/*      RC_UNSIGNED_BYTE */
/*      RC_FLOAT */
/*      RC_FIXED */

/* CullFaceMode */
#define RC_FRONT                          0x0404
#define RC_BACK                           0x0405
#define RC_FRONT_AND_BACK                 0x0408

/* DepthFunction */
/*      RC_NEVER */
/*      RC_LESS */
/*      RC_EQUAL */
/*      RC_LEQUAL */
/*      RC_GREATER */
/*      RC_NOTEQUAL */
/*      RC_GEQUAL */
/*      RC_ALWAYS */

/* EnableCap */
#define RC_FOG                            0x0B60
#define RC_LIGHTING                       0x0B50
#define RC_TEXTURE_2D                     0x0DE1
#define RC_TEXTURE_2D_NORMAL              0x0DE2
#define RC_CULL_FACE                      0x0B44
#define RC_ALPHA_TEST                     0x0BC0
#define RC_BLEND                          0x0BE2
#define RC_COLOR_LOGIC_OP                 0x0BF2
#define RC_DITHER                         0x0BD0
#define RC_STENCIL_TEST                   0x0B90
#define RC_DEPTH_TEST                     0x0B71
/*      RC_LIGHT0 */
/*      RC_LIGHT1 */
/*      RC_LIGHT2 */
/*      RC_LIGHT3 */
/*      RC_LIGHT4 */
/*      RC_LIGHT5 */
/*      RC_LIGHT6 */
/*      RC_LIGHT7 */
#define RC_POINT_SMOOTH                   0x0B10
#define RC_LINE_SMOOTH                    0x0B20
#define RC_SCISSOR_TEST                   0x0C11
#define RC_COLOR_MATERIAL                 0x0B57
#define RC_NORMALIZE                      0x0BA1
#define RC_RESCALE_NORMAL                 0x803A
#define RC_POLYGON_OFFSET_FILL            0x8037
#define RC_VERTEX_ARRAY                   0x8074
#define RC_NORMAL_ARRAY                   0x8075
#define RC_COLOR_ARRAY                    0x8076
#define RC_TEXTURE_COORD_ARRAY            0x8078
#define RC_MULTISAMPLE                    0x809D
#define RC_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define RC_SAMPLE_ALPHA_TO_ONE            0x809F
#define RC_SAMPLE_COVERAGE                0x80A0
#define RC_USE_SHADOW					  0x80A1
#define RC_USE_COLOR_SHADOW               0x80A2
#define RC_USE_TEXTURE_ALPHA_SHADOW		  0x80A3
#define RC_USE_TEXTURE_ONLY				  0x80A4
#define RC_USE_TRANSMITTANCE_SHADOW		  0x80A5

/* ErrorCode */
#define RC_NO_ERROR                       0
#define RC_INVALID_ENUM                   0x0500
#define RC_INVALID_VALUE                  0x0501
#define RC_INVALID_OPERATION              0x0502
#define RC_STACK_OVERFLOW                 0x0503
#define RC_STACK_UNDERFLOW                0x0504
#define RC_OUT_OF_MEMORY                  0x0505

/* FogMode */
/*      RC_LINEAR */
#define RC_EXP                            0x0800
#define RC_EXP2                           0x0801

/* FogParameter */
#define RC_FOG_DENSITY                    0x0B62
#define RC_FOG_START                      0x0B63
#define RC_FOG_END                        0x0B64
#define RC_FOG_MODE                       0x0B65
#define RC_FOG_COLOR                      0x0B66

/* FrontFaceDirection */
#define RC_CW                             0x0900
#define RC_CCW                            0x0901

/* GetPName */
#define RC_CURRENT_AMBIENT                0x1B00
#define RC_CURRENT_DIFFUSE                0x1B01
#define RC_CURRENT_SPECULAR               0x1B02

#define RC_CURRENT_COLOR                  0x0B00
#define RC_CURRENT_NORMAL                 0x0B02
#define RC_CURRENT_TEXTURE_COORDS         0x0B03
#define RC_POINT_SIZE                     0x0B11
#define RC_POINT_SIZE_MIN                 0x8126
#define RC_POINT_SIZE_MAX                 0x8127
#define RC_POINT_FADE_THRESHOLD_SIZE      0x8128
#define RC_POINT_DISTANCE_ATTENUATION     0x8129
#define RC_SMOOTH_POINT_SIZE_RANGE        0x0B12
#define RC_LINE_WIDTH                     0x0B21
#define RC_SMOOTH_LINE_WIDTH_RANGE        0x0B22
#define RC_ALIASED_POINT_SIZE_RANGE       0x846D
#define RC_ALIASED_LINE_WIDTH_RANGE       0x846E
#define RC_CULL_FACE_MODE                 0x0B45
#define RC_FRONT_FACE                     0x0B46
#define RC_SHADE_MODEL                    0x0B54
#define RC_DEPTH_RANGE                    0x0B70
#define RC_DEPTH_WRITEMASK                0x0B72
#define RC_DEPTH_CLEAR_VALUE              0x0B73
#define RC_DEPTH_FUNC                     0x0B74
#define RC_STENCIL_CLEAR_VALUE            0x0B91
#define RC_STENCIL_FUNC                   0x0B92
#define RC_STENCIL_VALUE_MASK             0x0B93
#define RC_STENCIL_FAIL                   0x0B94
#define RC_STENCIL_PASS_DEPTH_FAIL        0x0B95
#define RC_STENCIL_PASS_DEPTH_PASS        0x0B96
#define RC_STENCIL_REF                    0x0B97
#define RC_STENCIL_WRITEMASK              0x0B98
#define RC_MATRIX_MODE                    0x0BA0
#define RC_VIEWPORT                       0x0BA2
#define RC_MODELVIEW_STACK_DEPTH          0x0BA3
#define RC_PROJECTION_STACK_DEPTH         0x0BA4
#define RC_TEXTURE_STACK_DEPTH            0x0BA5
#define RC_MODELVIEW_MATRIX               0x0BA6
#define RC_PROJECTION_MATRIX              0x0BA7
#define RC_TEXTURE_MATRIX                 0x0BA8
#define RC_ALPHA_TEST_FUNC                0x0BC1
#define RC_ALPHA_TEST_REF                 0x0BC2
#define RC_BLEND_DST                      0x0BE0
#define RC_BLEND_SRC                      0x0BE1
#define RC_LOGIC_OP_MODE                  0x0BF0
#define RC_SCISSOR_BOX                    0x0C10
#define RC_SCISSOR_TEST                   0x0C11
#define RC_COLOR_CLEAR_VALUE              0x0C22
#define RC_COLOR_WRITEMASK                0x0C23
#define RC_UNPACK_ALIGNMENT               0x0CF5
#define RC_PACK_ALIGNMENT                 0x0D05
#define RC_MAX_LIGHTS                     0x0D31
#define RC_MAX_CLIP_PLANES                0x0D32
#define RC_MAX_TEXTURE_SIZE               0x0D33
#define RC_MAX_MODELVIEW_STACK_DEPTH      0x0D36
#define RC_MAX_PROJECTION_STACK_DEPTH     0x0D38
#define RC_MAX_TEXTURE_STACK_DEPTH        0x0D39
#define RC_MAX_VIEWPORT_DIMS              0x0D3A
#define RC_MAX_THRESHOLD_LEVELS           0x0D3B
#define RC_MAX_TEXTURE_UNITS              0x84E2
#define RC_SUBPIXEL_BITS                  0x0D50
#define RC_RED_BITS                       0x0D52
#define RC_GREEN_BITS                     0x0D53
#define RC_BLUE_BITS                      0x0D54
#define RC_ALPHA_BITS                     0x0D55
#define RC_DEPTH_BITS                     0x0D56
#define RC_STENCIL_BITS                   0x0D57
#define RC_POLYGON_OFFSET_UNITS           0x2A00
#define RC_POLYGON_OFFSET_FILL            0x8037
#define RC_POLYGON_OFFSET_FACTOR          0x8038
#define RC_MATERIAL_BINDING               0x8068
#define RC_TEXTURE_BINDING_2D             0x8069
#define RC_VERTEX_ARRAY_SIZE              0x807A
#define RC_VERTEX_ARRAY_TYPE              0x807B
#define RC_VERTEX_ARRAY_STRIDE            0x807C
#define RC_NORMAL_ARRAY_TYPE              0x807E
#define RC_NORMAL_ARRAY_STRIDE            0x807F
#define RC_COLOR_ARRAY_SIZE               0x8081
#define RC_COLOR_ARRAY_TYPE               0x8082
#define RC_COLOR_ARRAY_STRIDE             0x8083
#define RC_TEXTURE_COORD_ARRAY_SIZE       0x8088
#define RC_TEXTURE_COORD_ARRAY_TYPE       0x8089
#define RC_TEXTURE_COORD_ARRAY_STRIDE     0x808A
#define RC_VERTEX_ARRAY_POINTER           0x808E
#define RC_NORMAL_ARRAY_POINTER           0x808F
#define RC_COLOR_ARRAY_POINTER            0x8090
#define RC_TEXTURE_COORD_ARRAY_POINTER    0x8092
#define RC_SAMPLE_BUFFERS                 0x80A8
#define RC_SAMPLES                        0x80A9
#define RC_SAMPLE_COVERAGE_VALUE          0x80AA
#define RC_SAMPLE_COVERAGE_INVERT         0x80AB

/* GetTextureParameter */
/*      RC_TEXTURE_MAG_FILTER */
/*      RC_TEXTURE_MIN_FILTER */
/*      RC_TEXTURE_WRAP_S */
/*      RC_TEXTURE_WRAP_T */

#define RC_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define RC_COMPRESSED_TEXTURE_FORMATS     0x86A3

/* HintMode */
#define RC_DONT_CARE                      0x1100
#define RC_FASTEST                        0x1101
#define RC_NICEST                         0x1102
#define RC_FASTEST_AND_NICEST             0x1103
#define RC_THRESHOLD_LEVEL0               0x1118
#define RC_THRESHOLD_LEVEL1               0x1119
#define RC_THRESHOLD_LEVEL2               0x111A
#define RC_THRESHOLD_LEVEL3               0x111B
#define RC_THRESHOLD_LEVEL4               0x111C
#define RC_THRESHOLD_LEVEL5               0x111D
#define RC_THRESHOLD_LEVEL6               0x111E
#define RC_THRESHOLD_LEVEL7               0x111F
#define RC_THRESHOLD_LEVEL8               0x1120
#define RC_THRESHOLD_LEVEL9               0x1121

/* HintTarget */
#define RC_PERSPECTIVE_CORRECTION_HINT    0x0C50
#define RC_POINT_SMOOTH_HINT              0x0C51
#define RC_LINE_SMOOTH_HINT               0x0C52
#define RC_FOG_HINT                       0x0C54
#define RC_MIPMAP_HINT                    0x0C55
#define RC_RENDERING_HINT                 0x0C56
#define RC_RAYBOUNCE_THRESHOLD_HINT       0x0C58
#define RC_GENERATE_MIPMAP_HINT           0x8192

/* LightModelParameter */
#define RC_LIGHT_MODEL_AMBIENT            0x0B53
#define RC_LIGHT_MODEL_TWO_SIDE           0x0B52

/* LightParameter */
#define RC_AMBIENT                        0x1200
#define RC_DIFFUSE                        0x1201
#define RC_SPECULAR                       0x1202
#define RC_POSITION                       0x1203
#define RC_SPOT_DIRECTION                 0x1204
#define RC_SPOT_EXPONENT                  0x1205
#define RC_SPOT_INNER_CONE                0x1206
#define RC_SPOT_OUTER_CONE                0x1207
#define RC_SPOT_CUTOFF                    0x1208
#define RC_ATTENUATION_RANGE              0x1209
#define RC_START_ATTENUATION              0x1210
#define RC_END_ATTENUATION                0x1211
#define RC_CONSTANT_ATTENUATION           0x1212
#define RC_LINEAR_ATTENUATION             0x1213
#define RC_QUADRATIC_ATTENUATION          0x1214

/* DataType */
#define RC_BYTE                           0x1400
#define RC_UNSIGNED_BYTE                  0x1401
#define RC_SHORT                          0x1402
#define RC_UNSIGNED_SHORT                 0x1403
#define RC_FLOAT                          0x1406
#define RC_FIXED                          0x140C

/* LogicOp */
#define RC_CLEAR                          0x1500
#define RC_AND                            0x1501
#define RC_AND_REVERSE                    0x1502
#define RC_COPY                           0x1503
#define RC_AND_INVERTED                   0x1504
#define RC_NOOP                           0x1505
#define RC_XOR                            0x1506
#define RC_OR                             0x1507
#define RC_NOR                            0x1508
#define RC_EQUIV                          0x1509
#define RC_INVERT                         0x150A
#define RC_OR_REVERSE                     0x150B
#define RC_COPY_INVERTED                  0x150C
#define RC_OR_INVERTED                    0x150D
#define RC_NAND                           0x150E
#define RC_SET                            0x150F

/* MaterialFace */
/*      RC_FRONT_AND_BACK */

/* MaterialParameter */
#define RC_EMISSION                       0x1600
#define RC_SHININESS                      0x1601
#define RC_AMBIENT_AND_DIFFUSE            0x1602
#define RC_REFLECTION                     0x1603
#define RC_TRANSMITTANCE                  0x1604
#define RC_REFRACTION_INDEX               0x1605
#define RC_TEXTURE_ALPHA                  0x1606
#define RC_SPECULAR_LEVEL                 0x1607

// roughness
#define RC_ROUGHNESS					  0x1608
// roughness

/*      RC_AMBIENT */
/*      RC_DIFFUSE */
/*      RC_SPECULAR */

/* MatrixMode */
#define RC_MODELVIEW                      0x1700
#define RC_PROJECTION                     0x1701
#define RC_TEXTURE                        0x1702

/* NormalPointerType */
/*      RC_BYTE */
/*      RC_SHORT */
/*      RC_FLOAT */
/*      RC_FIXED */

/* PixelFormat */
#define RC_ALPHA                          0x1906
#define RC_RGB                            0x1907
#define RC_RGBA                           0x1908
#define RC_LUMINANCE                      0x1909
#define RC_LUMINANCE_ALPHA                0x190A

/* PixelStoreParameter */
#define RC_UNPACK_ALIGNMENT               0x0CF5
#define RC_PACK_ALIGNMENT                 0x0D05

/* PixelType */
/*      RC_UNSIGNED_BYTE */
#define RC_UNSIGNED_SHORT_4_4_4_4         0x8033
#define RC_UNSIGNED_SHORT_5_5_5_1         0x8034
#define RC_UNSIGNED_SHORT_5_6_5           0x8363

/* ShadingModel */
#define RC_FLAT                           0x1D00
#define RC_SMOOTH                         0x1D01

/* StencilFunction */
/*      RC_NEVER */
/*      RC_LESS */
/*      RC_EQUAL */
/*      RC_LEQUAL */
/*      RC_GREATER */
/*      RC_NOTEQUAL */
/*      RC_GEQUAL */
/*      RC_ALWAYS */

/* StencilOp */
/*      RC_ZERO */
#define RC_KEEP                           0x1E00
#define RC_REPLACE                        0x1E01
#define RC_INCR                           0x1E02
#define RC_DECR                           0x1E03
/*      RC_INVERT */

/* StringName */
#define RC_VENDOR                         0x1F00
#define RC_RENDERER                       0x1F01
#define RC_VERSION                        0x1F02
#define RC_EXTENSIONS                     0x1F03

/* TexCoordPointerType */
/*      RC_SHORT */
/*      RC_FLOAT */
/*      RC_FIXED */
/*      RC_BYTE */

/* TextureEnvMode */
#define RC_MODULATE                       0x2100
#define RC_DECAL                          0x2101
/*      RC_BLEND */
#define RC_ADD                            0x0104
/*      RC_REPLACE */

/* TextureEnvParameter */
#define RC_TEXTURE_ENV_MODE               0x2200
#define RC_TEXTURE_ENV_COLOR              0x2201

/* TextureEnvTarget */
#define RC_TEXTURE_ENV                    0x2300

/* TextureMagFilter */
#define RC_NEAREST                        0x2600
#define RC_LINEAR                         0x2601

/* TextureMinFilter */
/*      RC_NEAREST */
/*      RC_LINEAR */
#define RC_NEAREST_MIPMAP_NEAREST         0x2700
#define RC_LINEAR_MIPMAP_NEAREST          0x2701
#define RC_NEAREST_MIPMAP_LINEAR          0x2702
#define RC_LINEAR_MIPMAP_LINEAR           0x2703

/* TextureParameterName */
#define RC_TEXTURE_MAG_FILTER             0x2800
#define RC_TEXTURE_MIN_FILTER             0x2801
#define RC_TEXTURE_WRAP_S                 0x2802
#define RC_TEXTURE_WRAP_T                 0x2803
#define RC_GENERATE_MIPMAP                0x8191

/* TextureTarget */
/*      RC_TEXTURE_2D */

/* TextureUnit */
#define RC_TEXTURE0                       0x84C0
#define RC_TEXTURE1                       0x84C1
#define RC_TEXTURE2                       0x84C2
#define RC_TEXTURE3                       0x84C3
#define RC_TEXTURE4                       0x84C4
#define RC_TEXTURE5                       0x84C5
#define RC_TEXTURE6                       0x84C6
#define RC_TEXTURE7                       0x84C7
#define RC_TEXTURE8                       0x84C8
#define RC_TEXTURE9                       0x84C9
#define RC_TEXTURE10                      0x84CA
#define RC_TEXTURE11                      0x84CB
#define RC_TEXTURE12                      0x84CC
#define RC_TEXTURE13                      0x84CD
#define RC_TEXTURE14                      0x84CE
#define RC_TEXTURE15                      0x84CF
#define RC_TEXTURE16                      0x84D0
#define RC_TEXTURE17                      0x84D1
#define RC_TEXTURE18                      0x84D2
#define RC_TEXTURE19                      0x84D3
#define RC_TEXTURE20                      0x84D4
#define RC_TEXTURE21                      0x84D5
#define RC_TEXTURE22                      0x84D6
#define RC_TEXTURE23                      0x84D7
#define RC_TEXTURE24                      0x84D8
#define RC_TEXTURE25                      0x84D9
#define RC_TEXTURE26                      0x84DA
#define RC_TEXTURE27                      0x84DB
#define RC_TEXTURE28                      0x84DC
#define RC_TEXTURE29                      0x84DD
#define RC_TEXTURE30                      0x84DE
#define RC_TEXTURE31                      0x84DF
#define RC_ACTIVE_TEXTURE                 0x84E0
#define RC_CLIENT_ACTIVE_TEXTURE          0x84E1

/* TextureWrapMode */
#define RC_REPEAT                         0x2901
#define RC_MIRRORED_REPEAT                0x8370
#define RC_CLAMP_TO_EDGE                  0x812F

/* VertexPointerType */
/*      RC_SHORT */
/*      RC_FLOAT */
/*      RC_FIXED */
/*      RC_BYTE */

/* LightName */
#define	RC_LIGHT0	0x4000
#define	RC_LIGHT1	0x4001
#define	RC_LIGHT2	0x4002
#define	RC_LIGHT3	0x4003
#define	RC_LIGHT4	0x4004
#define	RC_LIGHT5	0x4005
#define	RC_LIGHT6	0x4006
#define	RC_LIGHT7	0x4007
#define	RC_LIGHT8	0x4008
#define	RC_LIGHT9	0x4009
#define	RC_LIGHT10	0x400A
#define	RC_LIGHT11	0x400B
#define	RC_LIGHT12	0x400C
#define	RC_LIGHT13	0x400D
#define	RC_LIGHT14	0x400E
#define	RC_LIGHT15	0x400F
#define	RC_LIGHT16	0x4010
#define	RC_LIGHT17	0x4011
#define	RC_LIGHT18	0x4012
#define	RC_LIGHT19	0x4013
#define	RC_LIGHT20	0x4014
#define	RC_LIGHT21	0x4015
#define	RC_LIGHT22	0x4016
#define	RC_LIGHT23	0x4017
#define	RC_LIGHT24	0x4018
#define	RC_LIGHT25	0x4019
#define	RC_LIGHT26	0x401A
#define	RC_LIGHT27	0x401B
#define	RC_LIGHT28	0x401C
#define	RC_LIGHT29	0x401D
#define	RC_LIGHT30	0x401E
#define	RC_LIGHT31	0x401F
#define	RC_LIGHT32	0x4020
#define	RC_LIGHT33	0x4021
#define	RC_LIGHT34	0x4022
#define	RC_LIGHT35	0x4023
#define	RC_LIGHT36	0x4024
#define	RC_LIGHT37	0x4025
#define	RC_LIGHT38	0x4026
#define	RC_LIGHT39	0x4027
#define	RC_LIGHT40	0x4028
#define	RC_LIGHT41	0x4029
#define	RC_LIGHT42	0x402A
#define	RC_LIGHT43	0x402B
#define	RC_LIGHT44	0x402C
#define	RC_LIGHT45	0x402D
#define	RC_LIGHT46	0x402E
#define	RC_LIGHT47	0x402F
#define	RC_LIGHT48	0x4030
#define	RC_LIGHT49	0x4031
#define	RC_LIGHT50	0x4032
#define	RC_LIGHT51	0x4033
#define	RC_LIGHT52	0x4034
#define	RC_LIGHT53	0x4035
#define	RC_LIGHT54	0x4036
#define	RC_LIGHT55	0x4037
#define	RC_LIGHT56	0x4038
#define	RC_LIGHT57	0x4039
#define	RC_LIGHT58	0x403A
#define	RC_LIGHT59	0x403B
#define	RC_LIGHT60	0x403C
#define	RC_LIGHT61	0x403D
#define	RC_LIGHT62	0x403E
#define	RC_LIGHT63	0x403F
#define	RC_LIGHT64	0x4040
#define	RC_LIGHT65	0x4041
#define	RC_LIGHT66	0x4042
#define	RC_LIGHT67	0x4043
#define	RC_LIGHT68	0x4044
#define	RC_LIGHT69	0x4045
#define	RC_LIGHT70	0x4046
#define	RC_LIGHT71	0x4047
#define	RC_LIGHT72	0x4048
#define	RC_LIGHT73	0x4049
#define	RC_LIGHT74	0x404A
#define	RC_LIGHT75	0x404B
#define	RC_LIGHT76	0x404C
#define	RC_LIGHT77	0x404D
#define	RC_LIGHT78	0x404E
#define	RC_LIGHT79	0x404F
#define	RC_LIGHT80	0x4050
#define	RC_LIGHT81	0x4051
#define	RC_LIGHT82	0x4052
#define	RC_LIGHT83	0x4053
#define	RC_LIGHT84	0x4054
#define	RC_LIGHT85	0x4055
#define	RC_LIGHT86	0x4056
#define	RC_LIGHT87	0x4057
#define	RC_LIGHT88	0x4058
#define	RC_LIGHT89	0x4059
#define	RC_LIGHT90	0x405A
#define	RC_LIGHT91	0x405B
#define	RC_LIGHT92	0x405C
#define	RC_LIGHT93	0x405D
#define	RC_LIGHT94	0x405E
#define	RC_LIGHT95	0x405F
#define	RC_LIGHT96	0x4060
#define	RC_LIGHT97	0x4061
#define	RC_LIGHT98	0x4062
#define	RC_LIGHT99	0x4063
#define	RC_LIGHT100	0x4064
#define	RC_LIGHT101	0x4065
#define	RC_LIGHT102	0x4066
#define	RC_LIGHT103	0x4067
#define	RC_LIGHT104	0x4068
#define	RC_LIGHT105	0x4069
#define	RC_LIGHT106	0x406A
#define	RC_LIGHT107	0x406B
#define	RC_LIGHT108	0x406C
#define	RC_LIGHT109	0x406D
#define	RC_LIGHT110	0x406E
#define	RC_LIGHT111	0x406F
#define	RC_LIGHT112	0x4070
#define	RC_LIGHT113	0x4071
#define	RC_LIGHT114	0x4072
#define	RC_LIGHT115	0x4073
#define	RC_LIGHT116	0x4074
#define	RC_LIGHT117	0x4075
#define	RC_LIGHT118	0x4076
#define	RC_LIGHT119	0x4077
#define	RC_LIGHT120	0x4078
#define	RC_LIGHT121	0x4079
#define	RC_LIGHT122	0x407A
#define	RC_LIGHT123	0x407B
#define	RC_LIGHT124	0x407C
#define	RC_LIGHT125	0x407D
#define	RC_LIGHT126	0x407E
#define	RC_LIGHT127	0x407F
#define	RC_LIGHT128	0x4080
#define	RC_LIGHT129	0x4081
#define	RC_LIGHT130	0x4082
#define	RC_LIGHT131	0x4083
#define	RC_LIGHT132	0x4084
#define	RC_LIGHT133	0x4085
#define	RC_LIGHT134	0x4086
#define	RC_LIGHT135	0x4087
#define	RC_LIGHT136	0x4088
#define	RC_LIGHT137	0x4089
#define	RC_LIGHT138	0x408A
#define	RC_LIGHT139	0x408B
#define	RC_LIGHT140	0x408C
#define	RC_LIGHT141	0x408D
#define	RC_LIGHT142	0x408E
#define	RC_LIGHT143	0x408F
#define	RC_LIGHT144	0x4090
#define	RC_LIGHT145	0x4091
#define	RC_LIGHT146	0x4092
#define	RC_LIGHT147	0x4093
#define	RC_LIGHT148	0x4094
#define	RC_LIGHT149	0x4095
#define	RC_LIGHT150	0x4096
#define	RC_LIGHT151	0x4097
#define	RC_LIGHT152	0x4098
#define	RC_LIGHT153	0x4099
#define	RC_LIGHT154	0x409A
#define	RC_LIGHT155	0x409B
#define	RC_LIGHT156	0x409C
#define	RC_LIGHT157	0x409D
#define	RC_LIGHT158	0x409E
#define	RC_LIGHT159	0x409F
#define	RC_LIGHT160	0x40A0
#define	RC_LIGHT161	0x40A1
#define	RC_LIGHT162	0x40A2
#define	RC_LIGHT163	0x40A3
#define	RC_LIGHT164	0x40A4
#define	RC_LIGHT165	0x40A5
#define	RC_LIGHT166	0x40A6
#define	RC_LIGHT167	0x40A7
#define	RC_LIGHT168	0x40A8
#define	RC_LIGHT169	0x40A9
#define	RC_LIGHT170	0x40AA
#define	RC_LIGHT171	0x40AB
#define	RC_LIGHT172	0x40AC
#define	RC_LIGHT173	0x40AD
#define	RC_LIGHT174	0x40AE
#define	RC_LIGHT175	0x40AF
#define	RC_LIGHT176	0x40B0
#define	RC_LIGHT177	0x40B1
#define	RC_LIGHT178	0x40B2
#define	RC_LIGHT179	0x40B3
#define	RC_LIGHT180	0x40B4
#define	RC_LIGHT181	0x40B5
#define	RC_LIGHT182	0x40B6
#define	RC_LIGHT183	0x40B7
#define	RC_LIGHT184	0x40B8
#define	RC_LIGHT185	0x40B9
#define	RC_LIGHT186	0x40BA
#define	RC_LIGHT187	0x40BB
#define	RC_LIGHT188	0x40BC
#define	RC_LIGHT189	0x40BD
#define	RC_LIGHT190	0x40BE
#define	RC_LIGHT191	0x40BF
#define	RC_LIGHT192	0x40C0
#define	RC_LIGHT193	0x40C1
#define	RC_LIGHT194	0x40C2
#define	RC_LIGHT195	0x40C3
#define	RC_LIGHT196	0x40C4
#define	RC_LIGHT197	0x40C5
#define	RC_LIGHT198	0x40C6
#define	RC_LIGHT199	0x40C7
#define	RC_LIGHT200	0x40C8
#define	RC_LIGHT201	0x40C9
#define	RC_LIGHT202	0x40CA
#define	RC_LIGHT203	0x40CB
#define	RC_LIGHT204	0x40CC
#define	RC_LIGHT205	0x40CD
#define	RC_LIGHT206	0x40CE
#define	RC_LIGHT207	0x40CF
#define	RC_LIGHT208	0x40D0
#define	RC_LIGHT209	0x40D1
#define	RC_LIGHT210	0x40D2
#define	RC_LIGHT211	0x40D3
#define	RC_LIGHT212	0x40D4
#define	RC_LIGHT213	0x40D5
#define	RC_LIGHT214	0x40D6
#define	RC_LIGHT215	0x40D7
#define	RC_LIGHT216	0x40D8
#define	RC_LIGHT217	0x40D9
#define	RC_LIGHT218	0x40DA
#define	RC_LIGHT219	0x40DB
#define	RC_LIGHT220	0x40DC
#define	RC_LIGHT221	0x40DD
#define	RC_LIGHT222	0x40DE
#define	RC_LIGHT223	0x40DF
#define	RC_LIGHT224	0x40E0
#define	RC_LIGHT225	0x40E1
#define	RC_LIGHT226	0x40E2
#define	RC_LIGHT227	0x40E3
#define	RC_LIGHT228	0x40E4
#define	RC_LIGHT229	0x40E5
#define	RC_LIGHT230	0x40E6
#define	RC_LIGHT231	0x40E7
#define	RC_LIGHT232	0x40E8
#define	RC_LIGHT233	0x40E9
#define	RC_LIGHT234	0x40EA
#define	RC_LIGHT235	0x40EB
#define	RC_LIGHT236	0x40EC
#define	RC_LIGHT237	0x40ED
#define	RC_LIGHT238	0x40EE
#define	RC_LIGHT239	0x40EF
#define	RC_LIGHT240	0x40F0
#define	RC_LIGHT241	0x40F1
#define	RC_LIGHT242	0x40F2
#define	RC_LIGHT243	0x40F3
#define	RC_LIGHT244	0x40F4
#define	RC_LIGHT245	0x40F5
#define	RC_LIGHT246	0x40F6
#define	RC_LIGHT247	0x40F7
#define	RC_LIGHT248	0x40F8
#define	RC_LIGHT249	0x40F9
#define	RC_LIGHT250	0x40FA
#define	RC_LIGHT251	0x40FB
#define	RC_LIGHT252	0x40FC
#define	RC_LIGHT253	0x40FD
#define	RC_LIGHT254	0x40FE
#define	RC_LIGHT255	0x40FF
#define	RC_LIGHT256	0x4100
#define	RC_LIGHT257	0x4101
#define	RC_LIGHT258	0x4102
#define	RC_LIGHT259	0x4103
#define	RC_LIGHT260	0x4104
#define	RC_LIGHT261	0x4105
#define	RC_LIGHT262	0x4106
#define	RC_LIGHT263	0x4107
#define	RC_LIGHT264	0x4108
#define	RC_LIGHT265	0x4109
#define	RC_LIGHT266	0x410A
#define	RC_LIGHT267	0x410B
#define	RC_LIGHT268	0x410C
#define	RC_LIGHT269	0x410D
#define	RC_LIGHT270	0x410E
#define	RC_LIGHT271	0x410F
#define	RC_LIGHT272	0x4110
#define	RC_LIGHT273	0x4111
#define	RC_LIGHT274	0x4112
#define	RC_LIGHT275	0x4113
#define	RC_LIGHT276	0x4114
#define	RC_LIGHT277	0x4115
#define	RC_LIGHT278	0x4116
#define	RC_LIGHT279	0x4117
#define	RC_LIGHT280	0x4118
#define	RC_LIGHT281	0x4119
#define	RC_LIGHT282	0x411A
#define	RC_LIGHT283	0x411B
#define	RC_LIGHT284	0x411C
#define	RC_LIGHT285	0x411D
#define	RC_LIGHT286	0x411E
#define	RC_LIGHT287	0x411F
#define	RC_LIGHT288	0x4120
#define	RC_LIGHT289	0x4121
#define	RC_LIGHT290	0x4122
#define	RC_LIGHT291	0x4123
#define	RC_LIGHT292	0x4124
#define	RC_LIGHT293	0x4125
#define	RC_LIGHT294	0x4126
#define	RC_LIGHT295	0x4127
#define	RC_LIGHT296	0x4128
#define	RC_LIGHT297	0x4129
#define	RC_LIGHT298	0x412A
#define	RC_LIGHT299	0x412B
#define	RC_LIGHT300	0x412C
#define	RC_LIGHT301	0x412D
#define	RC_LIGHT302	0x412E
#define	RC_LIGHT303	0x412F
#define	RC_LIGHT304	0x4130
#define	RC_LIGHT305	0x4131
#define	RC_LIGHT306	0x4132
#define	RC_LIGHT307	0x4133
#define	RC_LIGHT308	0x4134
#define	RC_LIGHT309	0x4135
#define	RC_LIGHT310	0x4136
#define	RC_LIGHT311	0x4137
#define	RC_LIGHT312	0x4138
#define	RC_LIGHT313	0x4139
#define	RC_LIGHT314	0x413A
#define	RC_LIGHT315	0x413B
#define	RC_LIGHT316	0x413C
#define	RC_LIGHT317	0x413D
#define	RC_LIGHT318	0x413E
#define	RC_LIGHT319	0x413F
#define	RC_LIGHT320	0x4140
#define	RC_LIGHT321	0x4141
#define	RC_LIGHT322	0x4142
#define	RC_LIGHT323	0x4143
#define	RC_LIGHT324	0x4144
#define	RC_LIGHT325	0x4145
#define	RC_LIGHT326	0x4146
#define	RC_LIGHT327	0x4147
#define	RC_LIGHT328	0x4148
#define	RC_LIGHT329	0x4149
#define	RC_LIGHT330	0x414A
#define	RC_LIGHT331	0x414B
#define	RC_LIGHT332	0x414C
#define	RC_LIGHT333	0x414D
#define	RC_LIGHT334	0x414E
#define	RC_LIGHT335	0x414F
#define	RC_LIGHT336	0x4150
#define	RC_LIGHT337	0x4151
#define	RC_LIGHT338	0x4152
#define	RC_LIGHT339	0x4153
#define	RC_LIGHT340	0x4154
#define	RC_LIGHT341	0x4155
#define	RC_LIGHT342	0x4156
#define	RC_LIGHT343	0x4157
#define	RC_LIGHT344	0x4158
#define	RC_LIGHT345	0x4159
#define	RC_LIGHT346	0x415A
#define	RC_LIGHT347	0x415B
#define	RC_LIGHT348	0x415C
#define	RC_LIGHT349	0x415D
#define	RC_LIGHT350	0x415E
#define	RC_LIGHT351	0x415F
#define	RC_LIGHT352	0x4160
#define	RC_LIGHT353	0x4161
#define	RC_LIGHT354	0x4162
#define	RC_LIGHT355	0x4163
#define	RC_LIGHT356	0x4164
#define	RC_LIGHT357	0x4165
#define	RC_LIGHT358	0x4166
#define	RC_LIGHT359	0x4167
#define	RC_LIGHT360	0x4168
#define	RC_LIGHT361	0x4169
#define	RC_LIGHT362	0x416A
#define	RC_LIGHT363	0x416B
#define	RC_LIGHT364	0x416C
#define	RC_LIGHT365	0x416D
#define	RC_LIGHT366	0x416E
#define	RC_LIGHT367	0x416F
#define	RC_LIGHT368	0x4170
#define	RC_LIGHT369	0x4171
#define	RC_LIGHT370	0x4172
#define	RC_LIGHT371	0x4173
#define	RC_LIGHT372	0x4174
#define	RC_LIGHT373	0x4175
#define	RC_LIGHT374	0x4176
#define	RC_LIGHT375	0x4177
#define	RC_LIGHT376	0x4178
#define	RC_LIGHT377	0x4179
#define	RC_LIGHT378	0x417A
#define	RC_LIGHT379	0x417B
#define	RC_LIGHT380	0x417C
#define	RC_LIGHT381	0x417D
#define	RC_LIGHT382	0x417E
#define	RC_LIGHT383	0x417F
#define	RC_LIGHT384	0x4180
#define	RC_LIGHT385	0x4181
#define	RC_LIGHT386	0x4182
#define	RC_LIGHT387	0x4183
#define	RC_LIGHT388	0x4184
#define	RC_LIGHT389	0x4185
#define	RC_LIGHT390	0x4186
#define	RC_LIGHT391	0x4187
#define	RC_LIGHT392	0x4188
#define	RC_LIGHT393	0x4189
#define	RC_LIGHT394	0x418A
#define	RC_LIGHT395	0x418B
#define	RC_LIGHT396	0x418C
#define	RC_LIGHT397	0x418D
#define	RC_LIGHT398	0x418E
#define	RC_LIGHT399	0x418F
#define	RC_LIGHT400	0x4190
#define	RC_LIGHT401	0x4191
#define	RC_LIGHT402	0x4192
#define	RC_LIGHT403	0x4193
#define	RC_LIGHT404	0x4194
#define	RC_LIGHT405	0x4195
#define	RC_LIGHT406	0x4196
#define	RC_LIGHT407	0x4197
#define	RC_LIGHT408	0x4198
#define	RC_LIGHT409	0x4199
#define	RC_LIGHT410	0x419A
#define	RC_LIGHT411	0x419B
#define	RC_LIGHT412	0x419C
#define	RC_LIGHT413	0x419D
#define	RC_LIGHT414	0x419E
#define	RC_LIGHT415	0x419F
#define	RC_LIGHT416	0x41A0
#define	RC_LIGHT417	0x41A1
#define	RC_LIGHT418	0x41A2
#define	RC_LIGHT419	0x41A3
#define	RC_LIGHT420	0x41A4
#define	RC_LIGHT421	0x41A5
#define	RC_LIGHT422	0x41A6
#define	RC_LIGHT423	0x41A7
#define	RC_LIGHT424	0x41A8
#define	RC_LIGHT425	0x41A9
#define	RC_LIGHT426	0x41AA
#define	RC_LIGHT427	0x41AB
#define	RC_LIGHT428	0x41AC
#define	RC_LIGHT429	0x41AD
#define	RC_LIGHT430	0x41AE
#define	RC_LIGHT431	0x41AF
#define	RC_LIGHT432	0x41B0
#define	RC_LIGHT433	0x41B1
#define	RC_LIGHT434	0x41B2
#define	RC_LIGHT435	0x41B3
#define	RC_LIGHT436	0x41B4
#define	RC_LIGHT437	0x41B5
#define	RC_LIGHT438	0x41B6
#define	RC_LIGHT439	0x41B7
#define	RC_LIGHT440	0x41B8
#define	RC_LIGHT441	0x41B9
#define	RC_LIGHT442	0x41BA
#define	RC_LIGHT443	0x41BB
#define	RC_LIGHT444	0x41BC
#define	RC_LIGHT445	0x41BD
#define	RC_LIGHT446	0x41BE
#define	RC_LIGHT447	0x41BF
#define	RC_LIGHT448	0x41C0
#define	RC_LIGHT449	0x41C1
#define	RC_LIGHT450	0x41C2
#define	RC_LIGHT451	0x41C3
#define	RC_LIGHT452	0x41C4
#define	RC_LIGHT453	0x41C5
#define	RC_LIGHT454	0x41C6
#define	RC_LIGHT455	0x41C7
#define	RC_LIGHT456	0x41C8
#define	RC_LIGHT457	0x41C9
#define	RC_LIGHT458	0x41CA
#define	RC_LIGHT459	0x41CB
#define	RC_LIGHT460	0x41CC
#define	RC_LIGHT461	0x41CD
#define	RC_LIGHT462	0x41CE
#define	RC_LIGHT463	0x41CF
#define	RC_LIGHT464	0x41D0
#define	RC_LIGHT465	0x41D1
#define	RC_LIGHT466	0x41D2
#define	RC_LIGHT467	0x41D3
#define	RC_LIGHT468	0x41D4
#define	RC_LIGHT469	0x41D5
#define	RC_LIGHT470	0x41D6
#define	RC_LIGHT471	0x41D7
#define	RC_LIGHT472	0x41D8
#define	RC_LIGHT473	0x41D9
#define	RC_LIGHT474	0x41DA
#define	RC_LIGHT475	0x41DB
#define	RC_LIGHT476	0x41DC
#define	RC_LIGHT477	0x41DD
#define	RC_LIGHT478	0x41DE
#define	RC_LIGHT479	0x41DF
#define	RC_LIGHT480	0x41E0
#define	RC_LIGHT481	0x41E1
#define	RC_LIGHT482	0x41E2
#define	RC_LIGHT483	0x41E3
#define	RC_LIGHT484	0x41E4
#define	RC_LIGHT485	0x41E5
#define	RC_LIGHT486	0x41E6
#define	RC_LIGHT487	0x41E7
#define	RC_LIGHT488	0x41E8
#define	RC_LIGHT489	0x41E9
#define	RC_LIGHT490	0x41EA
#define	RC_LIGHT491	0x41EB
#define	RC_LIGHT492	0x41EC
#define	RC_LIGHT493	0x41ED
#define	RC_LIGHT494	0x41EE
#define	RC_LIGHT495	0x41EF
#define	RC_LIGHT496	0x41F0
#define	RC_LIGHT497	0x41F1
#define	RC_LIGHT498	0x41F2
#define	RC_LIGHT499	0x41F3
#define	RC_LIGHT500	0x41F4
#define	RC_LIGHT501	0x41F5
#define	RC_LIGHT502	0x41F6
#define	RC_LIGHT503	0x41F7
#define	RC_LIGHT504	0x41F8
#define	RC_LIGHT505	0x41F9
#define	RC_LIGHT506	0x41FA
#define	RC_LIGHT507	0x41FB
#define	RC_LIGHT508	0x41FC
#define	RC_LIGHT509	0x41FD
#define	RC_LIGHT510	0x41FE
#define	RC_LIGHT511	0x41FF
#define	RC_LIGHT512	0x4200
#define	RC_LIGHT513	0x4201
#define	RC_LIGHT514	0x4202
#define	RC_LIGHT515	0x4203
#define	RC_LIGHT516	0x4204
#define	RC_LIGHT517	0x4205
#define	RC_LIGHT518	0x4206
#define	RC_LIGHT519	0x4207
#define	RC_LIGHT520	0x4208
#define	RC_LIGHT521	0x4209
#define	RC_LIGHT522	0x420A
#define	RC_LIGHT523	0x420B
#define	RC_LIGHT524	0x420C
#define	RC_LIGHT525	0x420D
#define	RC_LIGHT526	0x420E
#define	RC_LIGHT527	0x420F
#define	RC_LIGHT528	0x4210
#define	RC_LIGHT529	0x4211
#define	RC_LIGHT530	0x4212
#define	RC_LIGHT531	0x4213
#define	RC_LIGHT532	0x4214
#define	RC_LIGHT533	0x4215
#define	RC_LIGHT534	0x4216
#define	RC_LIGHT535	0x4217
#define	RC_LIGHT536	0x4218
#define	RC_LIGHT537	0x4219
#define	RC_LIGHT538	0x421A
#define	RC_LIGHT539	0x421B
#define	RC_LIGHT540	0x421C
#define	RC_LIGHT541	0x421D
#define	RC_LIGHT542	0x421E
#define	RC_LIGHT543	0x421F
#define	RC_LIGHT544	0x4220
#define	RC_LIGHT545	0x4221
#define	RC_LIGHT546	0x4222
#define	RC_LIGHT547	0x4223
#define	RC_LIGHT548	0x4224
#define	RC_LIGHT549	0x4225
#define	RC_LIGHT550	0x4226
#define	RC_LIGHT551	0x4227
#define	RC_LIGHT552	0x4228
#define	RC_LIGHT553	0x4229
#define	RC_LIGHT554	0x422A
#define	RC_LIGHT555	0x422B
#define	RC_LIGHT556	0x422C
#define	RC_LIGHT557	0x422D
#define	RC_LIGHT558	0x422E
#define	RC_LIGHT559	0x422F
#define	RC_LIGHT560	0x4230
#define	RC_LIGHT561	0x4231
#define	RC_LIGHT562	0x4232
#define	RC_LIGHT563	0x4233
#define	RC_LIGHT564	0x4234
#define	RC_LIGHT565	0x4235
#define	RC_LIGHT566	0x4236
#define	RC_LIGHT567	0x4237
#define	RC_LIGHT568	0x4238
#define	RC_LIGHT569	0x4239
#define	RC_LIGHT570	0x423A
#define	RC_LIGHT571	0x423B
#define	RC_LIGHT572	0x423C
#define	RC_LIGHT573	0x423D
#define	RC_LIGHT574	0x423E
#define	RC_LIGHT575	0x423F
#define	RC_LIGHT576	0x4240
#define	RC_LIGHT577	0x4241
#define	RC_LIGHT578	0x4242
#define	RC_LIGHT579	0x4243
#define	RC_LIGHT580	0x4244
#define	RC_LIGHT581	0x4245
#define	RC_LIGHT582	0x4246
#define	RC_LIGHT583	0x4247
#define	RC_LIGHT584	0x4248
#define	RC_LIGHT585	0x4249
#define	RC_LIGHT586	0x424A
#define	RC_LIGHT587	0x424B
#define	RC_LIGHT588	0x424C
#define	RC_LIGHT589	0x424D
#define	RC_LIGHT590	0x424E
#define	RC_LIGHT591	0x424F
#define	RC_LIGHT592	0x4250
#define	RC_LIGHT593	0x4251
#define	RC_LIGHT594	0x4252
#define	RC_LIGHT595	0x4253
#define	RC_LIGHT596	0x4254
#define	RC_LIGHT597	0x4255
#define	RC_LIGHT598	0x4256
#define	RC_LIGHT599	0x4257
#define	RC_LIGHT600	0x4258
#define	RC_LIGHT601	0x4259
#define	RC_LIGHT602	0x425A
#define	RC_LIGHT603	0x425B
#define	RC_LIGHT604	0x425C
#define	RC_LIGHT605	0x425D
#define	RC_LIGHT606	0x425E
#define	RC_LIGHT607	0x425F
#define	RC_LIGHT608	0x4260
#define	RC_LIGHT609	0x4261
#define	RC_LIGHT610	0x4262
#define	RC_LIGHT611	0x4263
#define	RC_LIGHT612	0x4264
#define	RC_LIGHT613	0x4265
#define	RC_LIGHT614	0x4266
#define	RC_LIGHT615	0x4267
#define	RC_LIGHT616	0x4268
#define	RC_LIGHT617	0x4269
#define	RC_LIGHT618	0x426A
#define	RC_LIGHT619	0x426B
#define	RC_LIGHT620	0x426C
#define	RC_LIGHT621	0x426D
#define	RC_LIGHT622	0x426E
#define	RC_LIGHT623	0x426F
#define	RC_LIGHT624	0x4270
#define	RC_LIGHT625	0x4271
#define	RC_LIGHT626	0x4272
#define	RC_LIGHT627	0x4273
#define	RC_LIGHT628	0x4274
#define	RC_LIGHT629	0x4275
#define	RC_LIGHT630	0x4276
#define	RC_LIGHT631	0x4277
#define	RC_LIGHT632	0x4278
#define	RC_LIGHT633	0x4279
#define	RC_LIGHT634	0x427A
#define	RC_LIGHT635	0x427B
#define	RC_LIGHT636	0x427C
#define	RC_LIGHT637	0x427D
#define	RC_LIGHT638	0x427E
#define	RC_LIGHT639	0x427F
#define	RC_LIGHT640	0x4280
#define	RC_LIGHT641	0x4281
#define	RC_LIGHT642	0x4282
#define	RC_LIGHT643	0x4283
#define	RC_LIGHT644	0x4284
#define	RC_LIGHT645	0x4285
#define	RC_LIGHT646	0x4286
#define	RC_LIGHT647	0x4287
#define	RC_LIGHT648	0x4288
#define	RC_LIGHT649	0x4289
#define	RC_LIGHT650	0x428A
#define	RC_LIGHT651	0x428B
#define	RC_LIGHT652	0x428C
#define	RC_LIGHT653	0x428D
#define	RC_LIGHT654	0x428E
#define	RC_LIGHT655	0x428F
#define	RC_LIGHT656	0x4290
#define	RC_LIGHT657	0x4291
#define	RC_LIGHT658	0x4292
#define	RC_LIGHT659	0x4293
#define	RC_LIGHT660	0x4294
#define	RC_LIGHT661	0x4295
#define	RC_LIGHT662	0x4296
#define	RC_LIGHT663	0x4297
#define	RC_LIGHT664	0x4298
#define	RC_LIGHT665	0x4299
#define	RC_LIGHT666	0x429A
#define	RC_LIGHT667	0x429B
#define	RC_LIGHT668	0x429C
#define	RC_LIGHT669	0x429D
#define	RC_LIGHT670	0x429E
#define	RC_LIGHT671	0x429F
#define	RC_LIGHT672	0x42A0
#define	RC_LIGHT673	0x42A1
#define	RC_LIGHT674	0x42A2
#define	RC_LIGHT675	0x42A3
#define	RC_LIGHT676	0x42A4
#define	RC_LIGHT677	0x42A5
#define	RC_LIGHT678	0x42A6
#define	RC_LIGHT679	0x42A7
#define	RC_LIGHT680	0x42A8
#define	RC_LIGHT681	0x42A9
#define	RC_LIGHT682	0x42AA
#define	RC_LIGHT683	0x42AB
#define	RC_LIGHT684	0x42AC
#define	RC_LIGHT685	0x42AD
#define	RC_LIGHT686	0x42AE
#define	RC_LIGHT687	0x42AF
#define	RC_LIGHT688	0x42B0
#define	RC_LIGHT689	0x42B1
#define	RC_LIGHT690	0x42B2
#define	RC_LIGHT691	0x42B3
#define	RC_LIGHT692	0x42B4
#define	RC_LIGHT693	0x42B5
#define	RC_LIGHT694	0x42B6
#define	RC_LIGHT695	0x42B7
#define	RC_LIGHT696	0x42B8
#define	RC_LIGHT697	0x42B9
#define	RC_LIGHT698	0x42BA
#define	RC_LIGHT699	0x42BB
#define	RC_LIGHT700	0x42BC
#define	RC_LIGHT701	0x42BD
#define	RC_LIGHT702	0x42BE
#define	RC_LIGHT703	0x42BF
#define	RC_LIGHT704	0x42C0
#define	RC_LIGHT705	0x42C1
#define	RC_LIGHT706	0x42C2
#define	RC_LIGHT707	0x42C3
#define	RC_LIGHT708	0x42C4
#define	RC_LIGHT709	0x42C5
#define	RC_LIGHT710	0x42C6
#define	RC_LIGHT711	0x42C7
#define	RC_LIGHT712	0x42C8
#define	RC_LIGHT713	0x42C9
#define	RC_LIGHT714	0x42CA
#define	RC_LIGHT715	0x42CB
#define	RC_LIGHT716	0x42CC
#define	RC_LIGHT717	0x42CD
#define	RC_LIGHT718	0x42CE
#define	RC_LIGHT719	0x42CF
#define	RC_LIGHT720	0x42D0
#define	RC_LIGHT721	0x42D1
#define	RC_LIGHT722	0x42D2
#define	RC_LIGHT723	0x42D3
#define	RC_LIGHT724	0x42D4
#define	RC_LIGHT725	0x42D5
#define	RC_LIGHT726	0x42D6
#define	RC_LIGHT727	0x42D7
#define	RC_LIGHT728	0x42D8
#define	RC_LIGHT729	0x42D9
#define	RC_LIGHT730	0x42DA
#define	RC_LIGHT731	0x42DB
#define	RC_LIGHT732	0x42DC
#define	RC_LIGHT733	0x42DD
#define	RC_LIGHT734	0x42DE
#define	RC_LIGHT735	0x42DF
#define	RC_LIGHT736	0x42E0
#define	RC_LIGHT737	0x42E1
#define	RC_LIGHT738	0x42E2
#define	RC_LIGHT739	0x42E3
#define	RC_LIGHT740	0x42E4
#define	RC_LIGHT741	0x42E5
#define	RC_LIGHT742	0x42E6
#define	RC_LIGHT743	0x42E7
#define	RC_LIGHT744	0x42E8
#define	RC_LIGHT745	0x42E9
#define	RC_LIGHT746	0x42EA
#define	RC_LIGHT747	0x42EB
#define	RC_LIGHT748	0x42EC
#define	RC_LIGHT749	0x42ED
#define	RC_LIGHT750	0x42EE
#define	RC_LIGHT751	0x42EF
#define	RC_LIGHT752	0x42F0
#define	RC_LIGHT753	0x42F1
#define	RC_LIGHT754	0x42F2
#define	RC_LIGHT755	0x42F3
#define	RC_LIGHT756	0x42F4
#define	RC_LIGHT757	0x42F5
#define	RC_LIGHT758	0x42F6
#define	RC_LIGHT759	0x42F7
#define	RC_LIGHT760	0x42F8
#define	RC_LIGHT761	0x42F9
#define	RC_LIGHT762	0x42FA
#define	RC_LIGHT763	0x42FB
#define	RC_LIGHT764	0x42FC
#define	RC_LIGHT765	0x42FD
#define	RC_LIGHT766	0x42FE
#define	RC_LIGHT767	0x42FF
#define	RC_LIGHT768	0x4300
#define	RC_LIGHT769	0x4301
#define	RC_LIGHT770	0x4302
#define	RC_LIGHT771	0x4303
#define	RC_LIGHT772	0x4304
#define	RC_LIGHT773	0x4305
#define	RC_LIGHT774	0x4306
#define	RC_LIGHT775	0x4307
#define	RC_LIGHT776	0x4308
#define	RC_LIGHT777	0x4309
#define	RC_LIGHT778	0x430A
#define	RC_LIGHT779	0x430B
#define	RC_LIGHT780	0x430C
#define	RC_LIGHT781	0x430D
#define	RC_LIGHT782	0x430E
#define	RC_LIGHT783	0x430F
#define	RC_LIGHT784	0x4310
#define	RC_LIGHT785	0x4311
#define	RC_LIGHT786	0x4312
#define	RC_LIGHT787	0x4313
#define	RC_LIGHT788	0x4314
#define	RC_LIGHT789	0x4315
#define	RC_LIGHT790	0x4316
#define	RC_LIGHT791	0x4317
#define	RC_LIGHT792	0x4318
#define	RC_LIGHT793	0x4319
#define	RC_LIGHT794	0x431A
#define	RC_LIGHT795	0x431B
#define	RC_LIGHT796	0x431C
#define	RC_LIGHT797	0x431D
#define	RC_LIGHT798	0x431E
#define	RC_LIGHT799	0x431F
#define	RC_LIGHT800	0x4320
#define	RC_LIGHT801	0x4321
#define	RC_LIGHT802	0x4322
#define	RC_LIGHT803	0x4323
#define	RC_LIGHT804	0x4324
#define	RC_LIGHT805	0x4325
#define	RC_LIGHT806	0x4326
#define	RC_LIGHT807	0x4327
#define	RC_LIGHT808	0x4328
#define	RC_LIGHT809	0x4329
#define	RC_LIGHT810	0x432A
#define	RC_LIGHT811	0x432B
#define	RC_LIGHT812	0x432C
#define	RC_LIGHT813	0x432D
#define	RC_LIGHT814	0x432E
#define	RC_LIGHT815	0x432F
#define	RC_LIGHT816	0x4330
#define	RC_LIGHT817	0x4331
#define	RC_LIGHT818	0x4332
#define	RC_LIGHT819	0x4333
#define	RC_LIGHT820	0x4334
#define	RC_LIGHT821	0x4335
#define	RC_LIGHT822	0x4336
#define	RC_LIGHT823	0x4337
#define	RC_LIGHT824	0x4338
#define	RC_LIGHT825	0x4339
#define	RC_LIGHT826	0x433A
#define	RC_LIGHT827	0x433B
#define	RC_LIGHT828	0x433C
#define	RC_LIGHT829	0x433D
#define	RC_LIGHT830	0x433E
#define	RC_LIGHT831	0x433F
#define	RC_LIGHT832	0x4340
#define	RC_LIGHT833	0x4341
#define	RC_LIGHT834	0x4342
#define	RC_LIGHT835	0x4343
#define	RC_LIGHT836	0x4344
#define	RC_LIGHT837	0x4345
#define	RC_LIGHT838	0x4346
#define	RC_LIGHT839	0x4347
#define	RC_LIGHT840	0x4348
#define	RC_LIGHT841	0x4349
#define	RC_LIGHT842	0x434A
#define	RC_LIGHT843	0x434B
#define	RC_LIGHT844	0x434C
#define	RC_LIGHT845	0x434D
#define	RC_LIGHT846	0x434E
#define	RC_LIGHT847	0x434F
#define	RC_LIGHT848	0x4350
#define	RC_LIGHT849	0x4351
#define	RC_LIGHT850	0x4352
#define	RC_LIGHT851	0x4353
#define	RC_LIGHT852	0x4354
#define	RC_LIGHT853	0x4355
#define	RC_LIGHT854	0x4356
#define	RC_LIGHT855	0x4357
#define	RC_LIGHT856	0x4358
#define	RC_LIGHT857	0x4359
#define	RC_LIGHT858	0x435A
#define	RC_LIGHT859	0x435B
#define	RC_LIGHT860	0x435C
#define	RC_LIGHT861	0x435D
#define	RC_LIGHT862	0x435E
#define	RC_LIGHT863	0x435F
#define	RC_LIGHT864	0x4360
#define	RC_LIGHT865	0x4361
#define	RC_LIGHT866	0x4362
#define	RC_LIGHT867	0x4363
#define	RC_LIGHT868	0x4364
#define	RC_LIGHT869	0x4365
#define	RC_LIGHT870	0x4366
#define	RC_LIGHT871	0x4367
#define	RC_LIGHT872	0x4368
#define	RC_LIGHT873	0x4369
#define	RC_LIGHT874	0x436A
#define	RC_LIGHT875	0x436B
#define	RC_LIGHT876	0x436C
#define	RC_LIGHT877	0x436D
#define	RC_LIGHT878	0x436E
#define	RC_LIGHT879	0x436F
#define	RC_LIGHT880	0x4370
#define	RC_LIGHT881	0x4371
#define	RC_LIGHT882	0x4372
#define	RC_LIGHT883	0x4373
#define	RC_LIGHT884	0x4374
#define	RC_LIGHT885	0x4375
#define	RC_LIGHT886	0x4376
#define	RC_LIGHT887	0x4377
#define	RC_LIGHT888	0x4378
#define	RC_LIGHT889	0x4379
#define	RC_LIGHT890	0x437A
#define	RC_LIGHT891	0x437B
#define	RC_LIGHT892	0x437C
#define	RC_LIGHT893	0x437D
#define	RC_LIGHT894	0x437E
#define	RC_LIGHT895	0x437F
#define	RC_LIGHT896	0x4380
#define	RC_LIGHT897	0x4381
#define	RC_LIGHT898	0x4382
#define	RC_LIGHT899	0x4383
#define	RC_LIGHT900	0x4384
#define	RC_LIGHT901	0x4385
#define	RC_LIGHT902	0x4386
#define	RC_LIGHT903	0x4387
#define	RC_LIGHT904	0x4388
#define	RC_LIGHT905	0x4389
#define	RC_LIGHT906	0x438A
#define	RC_LIGHT907	0x438B
#define	RC_LIGHT908	0x438C
#define	RC_LIGHT909	0x438D
#define	RC_LIGHT910	0x438E
#define	RC_LIGHT911	0x438F
#define	RC_LIGHT912	0x4390
#define	RC_LIGHT913	0x4391
#define	RC_LIGHT914	0x4392
#define	RC_LIGHT915	0x4393
#define	RC_LIGHT916	0x4394
#define	RC_LIGHT917	0x4395
#define	RC_LIGHT918	0x4396
#define	RC_LIGHT919	0x4397
#define	RC_LIGHT920	0x4398
#define	RC_LIGHT921	0x4399
#define	RC_LIGHT922	0x439A
#define	RC_LIGHT923	0x439B
#define	RC_LIGHT924	0x439C
#define	RC_LIGHT925	0x439D
#define	RC_LIGHT926	0x439E
#define	RC_LIGHT927	0x439F
#define	RC_LIGHT928	0x43A0
#define	RC_LIGHT929	0x43A1
#define	RC_LIGHT930	0x43A2
#define	RC_LIGHT931	0x43A3
#define	RC_LIGHT932	0x43A4
#define	RC_LIGHT933	0x43A5
#define	RC_LIGHT934	0x43A6
#define	RC_LIGHT935	0x43A7
#define	RC_LIGHT936	0x43A8
#define	RC_LIGHT937	0x43A9
#define	RC_LIGHT938	0x43AA
#define	RC_LIGHT939	0x43AB
#define	RC_LIGHT940	0x43AC
#define	RC_LIGHT941	0x43AD
#define	RC_LIGHT942	0x43AE
#define	RC_LIGHT943	0x43AF
#define	RC_LIGHT944	0x43B0
#define	RC_LIGHT945	0x43B1
#define	RC_LIGHT946	0x43B2
#define	RC_LIGHT947	0x43B3
#define	RC_LIGHT948	0x43B4
#define	RC_LIGHT949	0x43B5
#define	RC_LIGHT950	0x43B6
#define	RC_LIGHT951	0x43B7
#define	RC_LIGHT952	0x43B8
#define	RC_LIGHT953	0x43B9
#define	RC_LIGHT954	0x43BA
#define	RC_LIGHT955	0x43BB
#define	RC_LIGHT956	0x43BC
#define	RC_LIGHT957	0x43BD
#define	RC_LIGHT958	0x43BE
#define	RC_LIGHT959	0x43BF
#define	RC_LIGHT960	0x43C0
#define	RC_LIGHT961	0x43C1
#define	RC_LIGHT962	0x43C2
#define	RC_LIGHT963	0x43C3
#define	RC_LIGHT964	0x43C4
#define	RC_LIGHT965	0x43C5
#define	RC_LIGHT966	0x43C6
#define	RC_LIGHT967	0x43C7
#define	RC_LIGHT968	0x43C8
#define	RC_LIGHT969	0x43C9
#define	RC_LIGHT970	0x43CA
#define	RC_LIGHT971	0x43CB
#define	RC_LIGHT972	0x43CC
#define	RC_LIGHT973	0x43CD
#define	RC_LIGHT974	0x43CE
#define	RC_LIGHT975	0x43CF
#define	RC_LIGHT976	0x43D0
#define	RC_LIGHT977	0x43D1
#define	RC_LIGHT978	0x43D2
#define	RC_LIGHT979	0x43D3
#define	RC_LIGHT980	0x43D4
#define	RC_LIGHT981	0x43D5
#define	RC_LIGHT982	0x43D6
#define	RC_LIGHT983	0x43D7
#define	RC_LIGHT984	0x43D8
#define	RC_LIGHT985	0x43D9
#define	RC_LIGHT986	0x43DA
#define	RC_LIGHT987	0x43DB
#define	RC_LIGHT988	0x43DC
#define	RC_LIGHT989	0x43DD
#define	RC_LIGHT990	0x43DE
#define	RC_LIGHT991	0x43DF
#define	RC_LIGHT992	0x43E0
#define	RC_LIGHT993	0x43E1
#define	RC_LIGHT994	0x43E2
#define	RC_LIGHT995	0x43E3
#define	RC_LIGHT996	0x43E4
#define	RC_LIGHT997	0x43E5
#define	RC_LIGHT998	0x43E6
#define	RC_LIGHT999	0x43E7
#define	RC_LIGHT1000	0x43E8
#define	RC_LIGHT1001	0x43E9
#define	RC_LIGHT1002	0x43EA
#define	RC_LIGHT1003	0x43EB
#define	RC_LIGHT1004	0x43EC
#define	RC_LIGHT1005	0x43ED
#define	RC_LIGHT1006	0x43EE
#define	RC_LIGHT1007	0x43EF
#define	RC_LIGHT1008	0x43F0
#define	RC_LIGHT1009	0x43F1
#define	RC_LIGHT1010	0x43F2
#define	RC_LIGHT1011	0x43F3
#define	RC_LIGHT1012	0x43F4
#define	RC_LIGHT1013	0x43F5
#define	RC_LIGHT1014	0x43F6
#define	RC_LIGHT1015	0x43F7
#define	RC_LIGHT1016	0x43F8
#define	RC_LIGHT1017	0x43F9
#define	RC_LIGHT1018	0x43FA
#define	RC_LIGHT1019	0x43FB
#define	RC_LIGHT1020	0x43FC
#define	RC_LIGHT1021	0x43FD
#define	RC_LIGHT1022	0x43FE
#define	RC_LIGHT1023	0x43FF

/* Buffer Objects */
#define RC_ARRAY_BUFFER                   0x8892
#define RC_ELEMENT_ARRAY_BUFFER           0x8893

#define RC_ARRAY_BUFFER_BINDING               0x8894
#define RC_ELEMENT_ARRAY_BUFFER_BINDING       0x8895
#define RC_VERTEX_ARRAY_BUFFER_BINDING        0x8896
#define RC_NORMAL_ARRAY_BUFFER_BINDING        0x8897
#define RC_COLOR_ARRAY_BUFFER_BINDING         0x8898
#define RC_TEXTURE_COORD_ARRAY_BUFFER_BINDING 0x889A

#define RC_STATIC_DRAW                    0x88E4
#define RC_DYNAMIC_DRAW                   0x88E8

#define RC_BUFFER_SIZE                    0x8764
#define RC_BUFFER_USAGE                   0x8765

/* Texture combine + dot3 */
#define RC_SUBTRACT                       0x84E7
#define RC_COMBINE                        0x8570
#define RC_COMBINE_RGB                    0x8571
#define RC_COMBINE_ALPHA                  0x8572
#define RC_RGB_SCALE                      0x8573
#define RC_ADD_SIGNED                     0x8574
#define RC_INTERPOLATE                    0x8575
#define RC_CONSTANT                       0x8576
#define RC_PRIMARY_COLOR                  0x8577
#define RC_PREVIOUS                       0x8578
#define RC_OPERAND0_RGB                   0x8590
#define RC_OPERAND1_RGB                   0x8591
#define RC_OPERAND2_RGB                   0x8592
#define RC_OPERAND0_ALPHA                 0x8598
#define RC_OPERAND1_ALPHA                 0x8599
#define RC_OPERAND2_ALPHA                 0x859A

#define RC_ALPHA_SCALE                    0x0D1C

#define RC_SRC0_RGB                       0x8580
#define RC_SRC1_RGB                       0x8581
#define RC_SRC2_RGB                       0x8582
#define RC_SRC0_ALPHA                     0x8588
#define RC_SRC1_ALPHA                     0x8589
#define RC_SRC2_ALPHA                     0x858A

#define RC_DOT3_RGB                       0x86AE
#define RC_DOT3_RGBA                      0x86AF


#define RC_BYTE                           0x1400
#define RC_UNSIGNED_BYTE                  0x1401
#define RC_SHORT                          0x1402
#define RC_UNSIGNED_SHORT                 0x1403
#define RC_INT                            0x1404
#define RC_UNSIGNED_INT                   0x1405
#define RC_FLOAT                          0x1406
#define RC_2_BYTES                        0x1407
#define RC_3_BYTES                        0x1408
#define RC_4_BYTES                        0x1409
#define RC_DOUBLE                         0x140A

/*------------------------------------------------------------------------*
 * required OES extension tokens
 *------------------------------------------------------------------------*/

/* RC_OES_matrix_palette */
#ifndef RC_OES_matrix_palette
#define RC_MATRIX_PALETTE_OES									0x8840
#define RC_MAX_PALETTE_MATRICES_OES                             0x8842
#define RC_MAX_VERTEX_UNITS_OES                                 0x86A4
#define RC_MATRIX_INDEX_ARRAY_OES                               0x8844
#define RC_WEIGHT_ARRAY_OES                                     0x86AD
#define RC_CURRENT_PALETTE_MATRIX_OES                           0x8843
#define RC_MATRIX_INDEX_ARRAY_SIZE_OES                          0x8846
#define RC_MATRIX_INDEX_ARRAY_TYPE_OES                          0x8847
#define RC_MATRIX_INDEX_ARRAY_STRIDE_OES                        0x8848
#define RC_MATRIX_INDEX_ARRAY_POINTER_OES                       0x8849
#define RC_MATRIX_INDEX_ARRAY_BUFFER_BINDING_OES                0x8B9E
#define RC_WEIGHT_ARRAY_SIZE_OES                                0x86AB
#define RC_WEIGHT_ARRAY_TYPE_OES                                0x86A9
#define RC_WEIGHT_ARRAY_STRIDE_OES                              0x86AA
#define RC_WEIGHT_ARRAY_POINTER_OES                             0x86AC
#define RC_WEIGHT_ARRAY_BUFFER_BINDING_OES                      0x889E
#endif

/* OES_read_format */
#ifndef RC_OES_read_format
#define RC_IMPLEMENTATION_COLOR_READ_TYPE_OES                   0x8B9A
#define RC_IMPLEMENTATION_COLOR_READ_FORMAT_OES                 0x8B9B
#endif

/* RC_OES_compressed_paletted_texture */
#ifndef RC_OES_compressed_paletted_texture
#define RC_PALETTE4_RGB8_OES                                    0x8B90
#define RC_PALETTE4_RGBA8_OES                                   0x8B91
#define RC_PALETTE4_R5_G6_B5_OES                                0x8B92
#define RC_PALETTE4_RGBA4_OES                                   0x8B93
#define RC_PALETTE4_RGB5_A1_OES                                 0x8B94
#define RC_PALETTE8_RGB8_OES                                    0x8B95
#define RC_PALETTE8_RGBA8_OES                                   0x8B96
#define RC_PALETTE8_R5_G6_B5_OES                                0x8B97
#define RC_PALETTE8_RGBA4_OES                                   0x8B98
#define RC_PALETTE8_RGB5_A1_OES                                 0x8B99
#endif

/* OES_point_size_array */
#ifndef RC_OES_point_size_array
#define RC_POINT_SIZE_ARRAY_OES                                 0x8B9C
#define RC_POINT_SIZE_ARRAY_TYPE_OES                            0x898A
#define RC_POINT_SIZE_ARRAY_STRIDE_OES                          0x898B
#define RC_POINT_SIZE_ARRAY_POINTER_OES                         0x898C
#define RC_POINT_SIZE_ARRAY_BUFFER_BINDING_OES                  0x8B9F
#endif

/* RC_OES_point_sprite */
#ifndef RC_OES_point_sprite
#define RC_POINT_SPRITE_OES                                     0x8861
#define RC_COORD_REPLACE_OES                                    0x8862
#endif

/*************************************************************/

/* Unavailable only in Common profile */
RC_API void RC_APIENTRY rcAlphaFunc (RCenum func, RCclampf ref);
RC_API void RC_APIENTRY rcBlendFunc (RCenum sfactor, RCenum dfactor);
RC_API void RC_APIENTRY rcClipPlanef (RCenum plane, const RCfloat *equation);
RC_API void RC_APIENTRY rcColorMask (RCboolean red, RCboolean green, RCboolean blue, RCboolean alpha);
RC_API void RC_APIENTRY rcCullFace (RCenum mode);
RC_API void RC_APIENTRY rcDepthRangef (RCclampf zNear, RCclampf zFar);
RC_API void RC_APIENTRY rcDepthMask (RCboolean flag);
RC_API void RC_APIENTRY rcDepthFunc (RCenum func);
RC_API void RC_APIENTRY rcClearDepthf (RCclampf depth);
RC_API void RC_APIENTRY rcFrontFace (RCenum mode);
RC_API void RC_APIENTRY rcScissor (RCint x, RCint y, RCsizei width, RCsizei height);
RC_API void RC_APIENTRY rcPixelStorei (RCenum pname, RCint param);
RC_API void RC_APIENTRY rcLineWidth (RCfloat width);
RC_API void RC_APIENTRY rcLogicOp (RCenum opcode);
RC_API void RC_APIENTRY rcPointSize (RCfloat size);
RC_API void RC_APIENTRY rcTexEnvi (RCenum target, RCenum pname, RCint param);
RC_API void RC_APIENTRY rcTexEnviv (RCenum target, RCenum pname, const RCint *params);
RC_API void RC_APIENTRY rcPolygonOffset (RCfloat factor, RCfloat units);
RC_API void RC_APIENTRY rcStencilMask (RCuint mask);
RC_API void RC_APIENTRY rcStencilOp (RCenum fail, RCenum zfail, RCenum zpass);
RC_API void RC_APIENTRY rcStencilFunc (RCenum func, RCint ref, RCuint mask);
RC_API void RC_APIENTRY rcTexEnvfv (RCenum target, RCenum pname, const RCfloat *params);
RC_API void RC_APIENTRY rcTexEnvf (RCenum target, RCenum pname, RCfloat param);
RC_API void RC_APIENTRY rcCopyTexImage2D (RCenum target, RCint level, RCenum internalformat, RCint x, RCint y, RCsizei width, RCsizei height, RCint border);
RC_API void RC_APIENTRY rcCopyTexSubImage2D (RCenum target, RCint level, RCint xoffset, RCint yoffset, RCint x, RCint y, RCsizei width, RCsizei height);
RC_API void RC_APIENTRY rcClearStencil (RCint s);
RC_API void RC_APIENTRY rcOrthof (RCfloat left, RCfloat right, RCfloat bottom, RCfloat top, RCfloat zNear, RCfloat zFar);


/* Available only in Common profile */
RC_API void RC_APIENTRY rcClearColor (RCclampf red, RCclampf green, RCclampf blue, RCclampf alpha);
RC_API void RC_APIENTRY rcColor4f (RCfloat red, RCfloat green, RCfloat blue, RCfloat alpha);
RC_API void RC_APIENTRY rcFrustum (RCdouble left, RCdouble right, RCdouble bottom, RCdouble top, RCdouble zNear, RCdouble zFar);
RC_API void RC_APIENTRY rcFrustumf (RCfloat left, RCfloat right, RCfloat bottom, RCfloat top, RCfloat zNear, RCfloat zFar);
RC_API void RC_APIENTRY rcGetFloatv (RCenum pname, RCfloat *params);
RC_API void RC_APIENTRY rcGetLightfv (RCenum light, RCenum pname, RCfloat *params);
RC_API void RC_APIENTRY rcGetMaterialfv (RCenum face, RCenum pname, RCfloat *params);
RC_API void RC_APIENTRY rcGetTexParameterfv (RCenum target, RCenum pname, RCfloat *params);
RC_API void RC_APIENTRY rcLightf (RCenum light, RCenum pname, RCfloat param);
RC_API void RC_APIENTRY rcLightfv (RCenum light, RCenum pname, const RCfloat *params);
RC_API void RC_APIENTRY rcLoadMatrixf (const RCfloat *m);
RC_API void RC_APIENTRY rcMaterialf (RCenum face, RCenum pname, RCfloat param);
RC_API void RC_APIENTRY rcMaterialfv (RCenum face, RCenum pname, const RCfloat *params);
RC_API void RC_APIENTRY rcMultMatrixf (const RCfloat *m);
RC_API void RC_APIENTRY rcRotatef (RCfloat angle, RCfloat x, RCfloat y, RCfloat z);
RC_API void RC_APIENTRY rcScalef (RCfloat x, RCfloat y, RCfloat z);
RC_API void RC_APIENTRY rcTexParameterf (RCenum target, RCenum pname, RCfloat param);
RC_API void RC_APIENTRY rcTexParameterfv (RCenum target, RCenum pname, const RCfloat *params);
RC_API void RC_APIENTRY rcTranslatef (RCfloat x, RCfloat y, RCfloat z);

/* Available in both Common and Common-Lite profiles */
RC_API void RC_APIENTRY rcBindBuffer (RCenum target, RCuint buffer);
RC_API void RC_APIENTRY rcBindMaterial (RCuint material);
RC_API void RC_APIENTRY rcBindTexture (RCenum target, RCuint texture);
RC_API void RC_APIENTRY rcBufferData (RCenum target, RCsizeiptr size, const RCvoid *data, RCenum usage);
RC_API void RC_APIENTRY rcBufferSubData (RCenum target, RCintptr offset, RCsizeiptr size, const RCvoid *data);
RC_API void RC_APIENTRY rcClear (RCbitfield mask);
RC_API void RC_APIENTRY rcClearColorx (RCclampx red, RCclampx green, RCclampx blue, RCclampx alpha);
RC_API void RC_APIENTRY rcColor4ub (RCubyte red, RCubyte green, RCubyte blue, RCubyte alpha);
RC_API void RC_APIENTRY rcColor4x (RCfixed red, RCfixed green, RCfixed blue, RCfixed alpha);
RC_API void RC_APIENTRY rcColorPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer);
RC_API void RC_APIENTRY rcDeleteBuffers (RCsizei n, const RCuint *buffers);
RC_API void RC_APIENTRY rcDeleteMaterials (RCsizei n, RCuint *materials);
RC_API void RC_APIENTRY rcDeleteTextures (RCsizei n, const RCuint *textures);
RC_API void RC_APIENTRY rcDisable (RCenum cap);
RC_API void RC_APIENTRY rcDisableClientState (RCenum array);
RC_API void RC_APIENTRY rcDrawArrays (RCenum mode, RCint first, RCsizei count);
RC_API void RC_APIENTRY rcDrawElements (RCenum mode, RCsizei count, RCenum type, const RCvoid *indices);
RC_API void RC_APIENTRY rcEnable (RCenum cap);
RC_API void RC_APIENTRY rcEnableClientState (RCenum array);
RC_API void RC_APIENTRY rcFinish (void);
RC_API void RC_APIENTRY rcFlush (void);
RC_API void RC_APIENTRY rcFrustumx (RCfixed left, RCfixed right, RCfixed bottom, RCfixed top, RCfixed zNear, RCfixed zFar);
RC_API void RC_APIENTRY rcGetBooleanv (RCenum pname, RCboolean *params);
RC_API void RC_APIENTRY rcGetBufferParameteriv (RCenum target, RCenum pname, RCint *params);
RC_API void RC_APIENTRY rcGenBuffers (RCsizei n, RCuint *buffers);
RC_API void RC_APIENTRY rcGenMaterials (RCsizei n, RCuint *materials);
RC_API void RC_APIENTRY rcGenTextures (RCsizei n, RCuint *textures);
RC_API RCenum RC_APIENTRY rcGetError (void);
RC_API void RC_APIENTRY rcGetFixedv (RCenum pname, RCfixed *params);
RC_API void RC_APIENTRY rcGetIntegerv (RCenum pname, RCint *params);
RC_API void RC_APIENTRY rcGetLightxv (RCenum light, RCenum pname, RCfixed *params);
RC_API void RC_APIENTRY rcGetMaterialxv (RCenum face, RCenum pname, RCfixed *params);
RC_API void RC_APIENTRY rcGetPointerv (RCenum pname, RCvoid **params);
RC_API const RCubyte * RC_APIENTRY rcGetString (RCenum name);
RC_API void RC_APIENTRY rcGetTexParameteriv (RCenum target, RCenum pname, RCint *params);
RC_API void RC_APIENTRY rcGetTexParameterxv (RCenum target, RCenum pname, RCfixed *params);
RC_API void RC_APIENTRY rcHint (RCenum target, RCenum mode);
RC_API RCboolean RC_APIENTRY rcIsBuffer (RCuint buffer);
RC_API RCboolean RC_APIENTRY rcIsEnabled (RCenum cap);
RC_API RCboolean RC_APIENTRY rcIsMaterial (RCuint material);
RC_API RCboolean RC_APIENTRY rcIsTexture (RCuint texture);
RC_API void RC_APIENTRY rcLightx (RCenum light, RCenum pname, RCfixed param);
RC_API void RC_APIENTRY rcLightxv (RCenum light, RCenum pname, const RCfixed *params);
RC_API void RC_APIENTRY rcLoadIdentity (void);
RC_API void RC_APIENTRY rcLoadMatrixx (const RCfixed *m);
RC_API void RC_APIENTRY rcMaterialx (RCenum face, RCenum pname, RCfixed param);
RC_API void RC_APIENTRY rcMaterialxv (RCenum face, RCenum pname, const RCfixed *params);
RC_API void RC_APIENTRY rcMatrixMode (RCenum mode);
RC_API void RC_APIENTRY rcMultMatrixx (const RCfixed *m);
RC_API void RC_APIENTRY rcNormalPointer (RCenum type, RCsizei stride, const RCvoid *pointer);
RC_API void RC_APIENTRY rcPopMatrix (void);
RC_API void RC_APIENTRY rcPushMatrix (void);
RC_API void RC_APIENTRY rcReadPixels (RCint x, RCint y, RCsizei width, RCsizei height, RCenum format, RCenum type, RCvoid *pixels);
RC_API void RC_APIENTRY rcRotatex (RCfixed angle, RCfixed x, RCfixed y, RCfixed z);
RC_API void RC_APIENTRY rcScalex (RCfixed x, RCfixed y, RCfixed z);
RC_API void RC_APIENTRY rcTexCoordPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer);
RC_API void RC_APIENTRY rcTexImage2D (RCenum target, RCint level, RCint internalformat, RCsizei width, RCsizei height, RCint border, RCenum format, RCenum type, const RCvoid *pixels);
RC_API void RC_APIENTRY rcTexParameteri (RCenum target, RCenum pname, RCint param);
RC_API void RC_APIENTRY rcTexParameterx (RCenum target, RCenum pname, RCfixed param);
RC_API void RC_APIENTRY rcTexParameteriv (RCenum target, RCenum pname, const RCint *params);
RC_API void RC_APIENTRY rcTexParameterxv (RCenum target, RCenum pname, const RCfixed *params);
RC_API void RC_APIENTRY rcTexSubImage2D (RCenum target, RCint level, RCint xoffset, RCint yoffset, RCsizei width, RCsizei height, RCenum format, RCenum type, const RCvoid *pixels);
RC_API void RC_APIENTRY rcTranslatex (RCfixed x, RCfixed y, RCfixed z);
RC_API void RC_APIENTRY rcVertexPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer);
RC_API void RC_APIENTRY rcViewport (RCint x, RCint y, RCsizei width, RCsizei height);

/*------------------------------------------------------------------------*
 * Required OES extension functions
 *------------------------------------------------------------------------*/

#ifndef RC_OES_matrix_palette
#define RC_OES_matrix_palette 1
RC_API void RC_APIENTRY rcCurrentPaletteMatrixOES (RCuint matrixpaletteindex);
RC_API void RC_APIENTRY rcLoadPaletteFromModelViewMatrixOES (void);
RC_API void RC_APIENTRY rcMatrixIndexPointerOES (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer);
RC_API void RC_APIENTRY rcWeightPointerOES (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer);
#endif


/*------------------------------------------------------------------------*
 * Extended Functions
 *------------------------------------------------------------------------*/
RC_API void RC_APIENTRY rcuPerspective ( RCfloat fovy, RCfloat aspect, RCfloat zNear, RCfloat zFar);
RC_API void RC_APIENTRY rcuLookAt (RCfloat eyex, RCfloat eyey, RCfloat eyez,
                                   RCfloat centerx, RCfloat centery, RCfloat centerz,
                                   RCfloat upx, RCfloat upy, RCfloat upz);
RC_API void RC_APIENTRY rcSceneAllInit(void);
RC_API void RC_APIENTRY rcStaticSceneBegin(void);
RC_API void RC_APIENTRY rcStaticSceneEnd(void);

RC_API void RC_APIENTRY rcTextureAlpha(RCbyte value);
RC_API void RC_APIENTRY rcDepthBounce (RCuint value);

// light radius
RC_API void RC_APIENTRY rcSetLightRadius(RCfloat radius);
// light radius

// diffuse rate
RC_API void RC_APIENTRY rcSetDiffuseRate(RCuint rate);
// diffuse rate

// spp
RC_API void RC_APIENTRY rcSetSPPCount(RCuint count);
// spp

#ifdef __cplusplus
}
#endif

#endif /* __OPENRC_H__ */
