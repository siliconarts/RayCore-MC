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
// Project : Emulation
// DATE    : 8/29/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#pragma once
#include "Emulation_Common.h"
#include "ray.h"
#include "hit.h"
#include "camera.h"
#include "kdtree.h"
#include "material.h"
#include "option.h"
#include "texture.h"

typedef struct{
	int		ScreenX, ScreenY;
	int		*valid_ray_generation;
	int		*RayKind;
	int		*light_num;
	int		*positionSignificantFigure;//dk significant figure of position
	RayFloat	*Position, *Normal, *Direction, *Weight;
	RayFloat	*reflection_color,*refraction_color, *refraction_index;
	unsigned int		*depth;
	_ray_in_buffer *StackData;
	_hit	*hit;//dk SIT
}_RayGenInput;

typedef struct{
	int		*valid_traversal;
	_ray	*ray;
	int		*StackCnt;
	RayFloat	*SpotAtten;
	int		*SIT_primNum;//dk SIT
}_RayGenOutput;

typedef struct{
	int		*valid_traversal;
	int		*RayKind;
	_ray	*ray;
	_hit	*hit;
	int		*SIT_primNum;//dk SIT
}_TnIInput;

typedef struct{
	_hit	*hit;
	RayFloat	*ShadowRayMinDist;
}_TnIOutput;

void ray_generation(_RayGenInput *InData, _RayGenOutput *OutData);
void traversal(_TnIInput *InData, _TnIOutput *OutData);
int box_intersection(RayFloat *minimum, RayFloat *maximum, _ray *ray, RayFloat old_tmax, RayFloat *new_tmin, RayFloat *new_tmax);
void tri_intersectionOpt(int PrimNum, _ray *ray, _hit* Hit);
int NodeTraversal(RayFloat ro, RayFloat rd, DDK_NODE node, RayFloat *tmin, RayFloat *tmax, RayFloat PrimDist, int *NodeAddr, struct KdStack *Stack);

class RayRendering{
public:
	RayRendering(void);
	~RayRendering(void);

	void Initialize(int x, int y);

	int				m_iX, m_iY;
	int				Valid;		// 없어져야 함.

	int				RayKind;
	unsigned int	depth;
	int				mode, light_num, shadow_mode, ShadingMode;
	int				valid_ray_generation, valid_traversal, valid_shading; 
	int				rgb;
	int				positionSignificantFigure[3];//dk significant figure of position
	RayFloat		Position[3], Normal[3], Direction[3], Weight[3];
	RayFloat		reflection_color[3],refraction_color[3], refraction_index;
	RayFloat		Color[3];
	RayFloat		accColorS[3], accColorAD[3];
	RayFloat		bilinear_tex_color[4];
	RayFloat		ShadowRayMinDist;
	RayFloat		TransmitColor[3];

	int				ListCount;
	int				NodeCount;
	int				TriCount;

	_ray			ray;
	_hit			RayHit, hit;
	REG_LIGHT*		light;
	_material*		material;
	_texture_id*	texture_id;
	_texture*		texture;
	_texture*		normalmap;
	_option*		opt;
	int				SIT_primNum;//dk SIT

	_ray_in_buffer		StackData[StackSize];
	_SIT_primNumBuf		SITbuf;
	int					StackCnt;

	RayFloat			SpotAtten, CamAtten, LightAtten, Atten;
	RayFloat			RayLength;

	void UpdateFrameBuffer(void);

	void shading(_option opt, RayFloat CamDistance, _texture_index texture_index, _texture_index normalmap_index);

	void TexCoordCalculator(int prim_num, RayFloat *tex_coord);
	void NextRayInfoParameter(int mat_index, int Depth, int MaxDepth, _material mat);
	void NormalCalculator(int prim_num);

	void MatColorCalculator(int mat_index, _material mat, RayFloat *MatColorA, RayFloat *MatColorD, RayFloat *MatColorS);   
	void LightAttenuation(void);//dk 130529 light attenuation modification
	void CameraAttenuation(void);
	void ColorAccumulator(RayFloat *CAcolorA, RayFloat *CAcolorD, RayFloat *CAcolorS);

	int TexLODCalculator(RayFloat BaseLod, RayFloat CamDistance);
	void TexColorCalculator(int mat_index, _material mat, _texture_index texture_index, int Lod, RayFloat *tex_coord, RayFloat *tex_color);
	void calculateNormalFromTexture(int mat_index, _material mat, _texture_index normalmap_index, int Lod, RayFloat *tex_coord, int hit_prim_num);//dk normal mapping, 130529 Lod added

	_RayGenInput	RayGenInData;
	_RayGenOutput	RayGenOutData;
	_TnIInput		TnIInData;
	_TnIOutput		TnIOutData;
};
