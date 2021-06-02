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
#include <stdio.h>
#include <math.h>
#include "RayRendering.h"   
#include "operate.h"
#include "texture.h"

void RayRendering::shading(   
	_option opt, 
	RayFloat CamDistance,
	_texture_index texture_index,
	_texture_index normalmap_index){
	_material mat;
	REG_MATERIAL*	pMat;
   
	RayFloat tex_coord[2];
	RayFloat colorA[3];
	RayFloat colorD[3];
	RayFloat colorS[3];
	RayFloat tex_color[4];
	static int Lod;
	int ShadingMode;

	colorD[0] = 0;
	colorD[1] = 0;
	colorD[2] = 0;
	colorS[0] = 0;
	colorS[1] = 0;
	colorS[2] = 0;

	if((RayKind == RAY_TYPE_PRIMARY)&&RayHit.hit_result==0)	
		ShadingMode = 0;	//Primary ray miss
	else if(RayKind == RAY_TYPE_PRIMARY)
		ShadingMode = 1;	//Primary ray hit
	else if((RayKind == RAY_TYPE_SHADOW)&&hit.hit_result)
		ShadingMode = 2;	//Texture shadow
	else if(RayKind == RAY_TYPE_SHADOW)
		ShadingMode = 3;	//Shadow
	else if((RayKind == RAY_TYPE_TEXTURE_SHADOW)&&hit.hit_result)
		ShadingMode = 4;	//Texture shadow again
	else if(RayKind == RAY_TYPE_TEXTURE_SHADOW)
		ShadingMode = 5;	//Shadow
	else if(RayKind == RAY_TYPE_SKYBOX)
		ShadingMode = 8;	//Sky box
	else if(RayKind == RAY_TYPE_NORMAL)
		ShadingMode = 9;	//Normal mapping
	else if(RayHit.hit_result)
		ShadingMode = 6;	//Main ray hit
	else
		ShadingMode = 7;	//Main ray miss

	switch(ShadingMode){
		case 0 : //PrimaryRay Miss
			Color[0]=g_reg.control.background.r;
			Color[1]=g_reg.control.background.g;
			Color[2]=g_reg.control.background.b;
			Weight[0]=ray.weight[0];
			Weight[1]=ray.weight[1];
			Weight[2]=ray.weight[2];
			reflection_color[0] =
			reflection_color[1] =
			reflection_color[2] = 0;
			refraction_color[0] =
			refraction_color[1] =
			refraction_color[2] = 0;
			refraction_index	= 0;
			break;
		case 1 : //PrimaryRay Hit
			Color[0]=0;
			Color[1]=0;
			Color[2]=0;
			Weight[0]=ray.weight[0];
			Weight[1]=ray.weight[1];
			Weight[2]=ray.weight[2];      
			pMat	= &g_reg.material[g_mem.pPrimitiveInfo[RayHit.prim_num].material_index];
			mat = material[g_mem.pPrimitiveInfo[RayHit.prim_num].material_index];
			
			if(pMat->texture.enable){
				TexCoordCalculator(RayHit.prim_num, tex_coord);  
				Lod = TexLODCalculator(g_mem.pPrimitiveInfo[RayHit.prim_num].base_lod, CamDistance);
				//Lod = 0;
				TexColorCalculator(g_mem.pPrimitiveInfo[RayHit.prim_num].material_index, mat, texture_index, Lod, tex_coord, bilinear_tex_color);
			}

			NextRayInfoParameter(g_mem.pPrimitiveInfo[RayHit.prim_num].material_index, depth, g_reg.option.set.ray_bounce, mat);
			
			if(g_reg.option.set.normal_map == 0 || !pMat->normal.enable)
				NormalCalculator(RayHit.prim_num);
			CameraAttenuation();
			break;
		case 2 : //////TextureShadow
			TransmitColor[0]=0x3f0000;
			TransmitColor[1]=0x3f0000;
			TransmitColor[2]=0x3f0000;
			pMat	= &g_reg.material[g_mem.pPrimitiveInfo[hit.prim_num].material_index];
			mat = material[g_mem.pPrimitiveInfo[hit.prim_num].material_index];
			if(pMat->texture.enable){
				TexCoordCalculator(hit.prim_num, tex_coord);
				TexColorCalculator(g_mem.pPrimitiveInfo[hit.prim_num].material_index, mat, texture_index, 0, tex_coord, tex_color);
				if(tex_color[3]==0){
					TransmitColor[0] = fmul(TransmitColor[0],ray.dielectric[0]);
					TransmitColor[1] = fmul(TransmitColor[1],ray.dielectric[1]);
					TransmitColor[2] = fmul(TransmitColor[2],ray.dielectric[2]);
				}
				else{
					unsigned int bias = 0x3f0000;
					TransmitColor[0] = fmul(ray.dielectric[0],fmul(TransmitColor[0],fexp(f32f24((float)(1.0f - tex_color[3]/256.)), f32f24((float)(5.0f-4*tex_color[0]/256.)))));
					TransmitColor[1] = fmul(ray.dielectric[1],fmul(TransmitColor[1],fexp(f32f24((float)(1.0f - tex_color[3]/256.)), f32f24((float)(5.0f-4*tex_color[1]/256.)))));
					TransmitColor[2] = fmul(ray.dielectric[2],fmul(TransmitColor[2],fexp(f32f24((float)(1.0f - tex_color[3]/256.)), f32f24((float)(5.0f-4*tex_color[2]/256.)))));
				}
				TransmitColor[0] = TransmitColor[0]&0xfffff0;
				TransmitColor[1] = TransmitColor[1]&0xfffff0;
				TransmitColor[2] = TransmitColor[2]&0xfffff0;
			}
			break;
		case 3 : //Shadow
			TransmitColor[0] = ray.dielectric[0];
			TransmitColor[1] = ray.dielectric[1];
			TransmitColor[2] = ray.dielectric[2];
			pMat	= &g_reg.material[g_mem.pPrimitiveInfo[RayHit.prim_num].material_index];
			mat = material[g_mem.pPrimitiveInfo[RayHit.prim_num].material_index];

			LightAttenuation();//dk 130529 light attenuation modification
			MatColorCalculator(g_mem.pPrimitiveInfo[RayHit.prim_num].material_index, mat, colorA, colorD, colorS);
			ColorAccumulator(colorA, colorD, colorS);
			break;
		case 4 : //////TextureShadow
			pMat	= &g_reg.material[g_mem.pPrimitiveInfo[hit.prim_num].material_index];
			mat = material[g_mem.pPrimitiveInfo[hit.prim_num].material_index];
			if(pMat->texture.enable){
				TexCoordCalculator(hit.prim_num, tex_coord);              
				TexColorCalculator(g_mem.pPrimitiveInfo[hit.prim_num].material_index, mat, texture_index, 0, tex_coord, tex_color);

				if(tex_color[3]==0){
					TransmitColor[0] = fmul(TransmitColor[0],ray.dielectric[0]);
					TransmitColor[1] = fmul(TransmitColor[1],ray.dielectric[1]);
					TransmitColor[2] = fmul(TransmitColor[2],ray.dielectric[2]);
				}
				else{
					unsigned int bias = 0x3f0000;
					TransmitColor[0] = fmul(ray.dielectric[0],fmul(TransmitColor[0],fexp(f32f24((float)(1.0f - tex_color[3]/256.)), f32f24((float)(5.0f-4*tex_color[0]/256.)))));
					TransmitColor[1] = fmul(ray.dielectric[1],fmul(TransmitColor[1],fexp(f32f24((float)(1.0f - tex_color[3]/256.)), f32f24((float)(5.0f-4*tex_color[1]/256.)))));
					TransmitColor[2] = fmul(ray.dielectric[2],fmul(TransmitColor[2],fexp(f32f24((float)(1.0f - tex_color[3]/256.)), f32f24((float)(5.0f-4*tex_color[2]/256.)))));
				}
				TransmitColor[0] = TransmitColor[0]&0xfffff0;
				TransmitColor[1] = TransmitColor[1]&0xfffff0;
				TransmitColor[2] = TransmitColor[2]&0xfffff0;
			}
			break;
		case 5 : //Shadow
			TransmitColor[0] = fmul(TransmitColor[0],ray.dielectric[0]);
			TransmitColor[1] = fmul(TransmitColor[1],ray.dielectric[1]);
			TransmitColor[2] = fmul(TransmitColor[2],ray.dielectric[2]);
			pMat	= &g_reg.material[g_mem.pPrimitiveInfo[RayHit.prim_num].material_index];
			mat = material[g_mem.pPrimitiveInfo[RayHit.prim_num].material_index];

			LightAttenuation();//dk 130529 light attenuation modification
			MatColorCalculator(g_mem.pPrimitiveInfo[RayHit.prim_num].material_index, mat, colorA, colorD, colorS);
			ColorAccumulator(colorA, colorD, colorS);
			break;
		case 6 : ///MainRay Hit
			Weight[0]=ray.weight[0];
			Weight[1]=ray.weight[1];
			Weight[2]=ray.weight[2];
			pMat	= &g_reg.material[g_mem.pPrimitiveInfo[RayHit.prim_num].material_index];
			mat = material[g_mem.pPrimitiveInfo[RayHit.prim_num].material_index];
			if(pMat->texture.enable){
				TexCoordCalculator(RayHit.prim_num, tex_coord); 
				Lod = TexLODCalculator(g_mem.pPrimitiveInfo[RayHit.prim_num].base_lod, CamDistance);
				//Lod = 0;
				TexColorCalculator(g_mem.pPrimitiveInfo[RayHit.prim_num].material_index, mat, texture_index, Lod, tex_coord, bilinear_tex_color);
			}

			NextRayInfoParameter(g_mem.pPrimitiveInfo[RayHit.prim_num].material_index, depth, g_reg.option.set.ray_bounce, mat);
			
			if(g_reg.option.set.normal_map == 0 || !pMat->normal.enable)
				NormalCalculator(RayHit.prim_num);
			break;
		case 7 : //MainRay Miss
			colorA[0] = 0;
			colorA[1] = 0;
			colorA[2] = 0;
			colorD[0] = g_reg.control.background.r;
			colorD[1] = g_reg.control.background.g;
			colorD[2] = g_reg.control.background.b;
			colorS[0] = 0;
			colorS[1] = 0;
			colorS[2] = 0;
			Weight[0]=ray.weight[0];
			Weight[1]=ray.weight[1];
			Weight[2]=ray.weight[2];
			reflection_color[0] = 0;
			reflection_color[1] = 0;
			reflection_color[2] = 0;
			refraction_color[0] = 0;
			refraction_color[1] = 0;
			refraction_color[2] = 0;
			refraction_index=0;  
			ColorAccumulator(colorA, colorD, colorS);
			break;
		case 8 : //dk background(self illumination = 1)
			pMat	= &g_reg.material[g_mem.pPrimitiveInfo[RayHit.prim_num].material_index];
			mat = material[g_mem.pPrimitiveInfo[RayHit.prim_num].material_index];
			if(!pMat->texture.enable){//dk no texture
				Color[0] = fadder(Color[0], fmul(pMat->diffuse.r, Weight[0]), 0);
				Color[1] = fadder(Color[1], fmul(pMat->diffuse.g, Weight[1]), 0);
				Color[2] = fadder(Color[2], fmul(pMat->diffuse.b, Weight[2]), 0);
			}
			else{//dk texture
				Color[0] = fadder(Color[0], fmul(f32f24(bilinear_tex_color[0]/256.f), Weight[0]), 0);
				Color[1] = fadder(Color[1], fmul(f32f24(bilinear_tex_color[1]/256.f), Weight[1]), 0);
				Color[2] = fadder(Color[2], fmul(f32f24(bilinear_tex_color[2]/256.f), Weight[2]), 0);
			}
			reflection_color[0] = 0;
			reflection_color[1] = 0;
			reflection_color[2] = 0;
			refraction_color[0] = 0;
			refraction_color[1] = 0;
			refraction_color[2] = 0;
			refraction_index = 0;
			if(compare(Color[0],f32f24(1.f))==1)
				Color[0] =f32f24(1.f);
			if(compare(Color[1],f32f24(1.f))==1)
				Color[1] =f32f24(1.f);
			if(compare(Color[2],f32f24(1.f))==1)
				Color[2] =f32f24(1.f);
			break;
		case 9 :
			pMat	= &g_reg.material[g_mem.pPrimitiveInfo[RayHit.prim_num].material_index];
			mat = material[g_mem.pPrimitiveInfo[RayHit.prim_num].material_index];
			TexCoordCalculator(RayHit.prim_num, tex_coord);
			Lod = TexLODCalculator(g_mem.pPrimitiveInfo[RayHit.prim_num].base_lod, CamDistance);//dk 130529 added
			calculateNormalFromTexture(g_mem.pPrimitiveInfo[RayHit.prim_num].material_index, mat, normalmap_index, Lod, tex_coord, RayHit.prim_num);//dk 130529 Lod added
	}

	if((RayKind == RAY_TYPE_PRIMARY)&&(RayHit.hit_result == 0)) // primary ray miss
		RayKind = 0;
	else if(RayKind == RAY_TYPE_PRIMARY){ // primary hit
		RayKind = 0;
		if(mat.self_illumination == 0x3f0000)
			RayKind = RAY_TYPE_SKYBOX;
		else if(g_reg.option.set.normal_map && pMat->normal.enable)
			RayKind = RAY_TYPE_NORMAL;
	}
	else if((RayKind == RAY_TYPE_SHADOW) && hit.hit_result) // texture shadow
		RayKind = RAY_TYPE_TEXTURE_SHADOW;
	else if(RayKind == RAY_TYPE_SHADOW){ // shadow
		RayKind = 0;
		if(RayHit.hit_result && (depth<g_reg.option.set.ray_bounce)){
			if(reflection_color[0] != 0 ||reflection_color[1] != 0 || reflection_color[2] != 0)
				RayKind = RAY_TYPE_REFLECTION;
			if(refraction_color[0] != 0 ||refraction_color[1] != 0 || refraction_color[2] != 0)
				RayKind |= RAY_TYPE_REFRACTION;
		}
	}
	else if((RayKind == RAY_TYPE_TEXTURE_SHADOW) && hit.hit_result) // texture shadow
		RayKind = RAY_TYPE_TEXTURE_SHADOW;
	else if(RayKind == RAY_TYPE_TEXTURE_SHADOW){ // shadow
		RayKind = 0;
		if(RayHit.hit_result && (depth<g_reg.option.set.ray_bounce)){
			if(reflection_color[0] != 0 ||reflection_color[1] != 0 || reflection_color[2] != 0)
				RayKind = RAY_TYPE_REFLECTION;
			if(refraction_color[0] != 0 ||refraction_color[1] != 0 || refraction_color[2] != 0)
				RayKind |= RAY_TYPE_REFRACTION;
		}
	}
	else if(RayKind == RAY_TYPE_SKYBOX){
		RayKind		= 0;
		light_num	= g_reg.option.set.light_number;
	}
	else if(RayKind == RAY_TYPE_NORMAL){
		RayKind		= 0;
		light_num	= 0;
	}
	else if(RayHit.hit_result){ // main ray hit
		RayKind = 0;
		if(mat.self_illumination == 0x3f0000)
			RayKind	= RAY_TYPE_SKYBOX;
		else if(g_reg.option.set.normal_map && pMat->normal.enable)
			RayKind	= RAY_TYPE_NORMAL;
	}
	else // main ray miss
		RayKind		= 0;
}

void RayRendering::NormalCalculator(int prim_num){
	RayFloat NB_NA[3], NC_NA[3];
	RayFloat normal_temp;
	RayFloat NDotD;
	int comp;
	
	DDK_PRIMITIVE_INFO* pPrimInfo	= &g_mem.pPrimitiveInfo[prim_num];

	if(RayHit.gamma&0x800000)	RayHit.gamma	= 0;
	if(RayHit.beta&0x800000)	RayHit.beta		= 0;

	if(pPrimInfo->fGroup==0){
		RCVECTOR3u	norm_0	= g_mem.PrimitiveInfoNormal(prim_num, 0);
		RCVECTOR3u	norm_1	= g_mem.PrimitiveInfoNormal(prim_num, 1);
		RCVECTOR3u	norm_2	= g_mem.PrimitiveInfoNormal(prim_num, 2);
		NB_NA[0] = fmul(fadder(norm_1.x, norm_0.x, 1), RayHit.beta);
		NB_NA[1] = fmul(fadder(norm_1.y, norm_0.y, 1), RayHit.beta);
		NB_NA[2] = fmul(fadder(norm_1.z, norm_0.z, 1), RayHit.beta);
		NC_NA[0] = fmul(TFAdder(norm_2.x, SignConv(norm_0.x), 0), RayHit.gamma);
		NC_NA[1] = fmul(TFAdder(norm_2.y, SignConv(norm_0.y), 0), RayHit.gamma);
		NC_NA[2] = fmul(TFAdder(norm_2.z, SignConv(norm_0.z), 0), RayHit.gamma);
		Normal[0] = TFAdder(norm_0.x, NB_NA[0], NC_NA[0]);
		Normal[1] = TFAdder(norm_0.y, NB_NA[1], NC_NA[1]);
		Normal[2] = TFAdder(norm_0.z, NB_NA[2], NC_NA[2]);
	}
	else if(pPrimInfo->fGroup==2){
		RCVECTOR3u	norm_0	= g_mem.PrimitiveInfoNormal(prim_num, 0);
		Normal[0] = norm_0.x;
		Normal[1] = norm_0.y;
		Normal[2] = norm_0.z;
	}
	else if(pPrimInfo->fGroup==3){
		RCVECTOR3u CenterPos	= g_mem.PrimitiveCenterPos(RayHit.prim_num);
		Normal[0] = fadder(Position[0],CenterPos.x, 1);	// CenterX
		Normal[1] = fadder(Position[1],CenterPos.y, 1);	// CenterY
		Normal[2] = fadder(Position[2],CenterPos.z, 1);	// CenterZ
	}

	normal_temp=TFAdder(fmul(Normal[0],Normal[0]),fmul(Normal[1],Normal[1]),fmul(Normal[2],Normal[2]));
	NDotD = TFAdder(fmul(Normal[0], Direction[0]), fmul(Normal[1], Direction[1]), fmul(Normal[2], Direction[2]));
	normal_temp=SQRT(normal_temp);
	Normal[0]=fmul(Normal[0],fdivider(f32f24(1.0f),normal_temp));
	Normal[1]=fmul(Normal[1],fdivider(f32f24(1.0f),normal_temp));
	Normal[2]=fmul(Normal[2],fdivider(f32f24(1.0f),normal_temp));

	comp=compare(NDotD,0);
	if(comp==1){
		Normal[0] = SignConv(Normal[0]);
		Normal[1] = SignConv(Normal[1]);
		Normal[2] = SignConv(Normal[2]);
		if(refraction_index!=0)
			refraction_index = fdivider(f32f24(1.0f),refraction_index);            
	}
}

void RayRendering::TexCoordCalculator(int prim_num, RayFloat *tex_coord){
	RayFloat TB_TA[3], TC_TA[3];
	RayFloat Vn[3], Vp[3], Ve[3], VnCrossVe[3];
	RayFloat VnDotVp, VpDotVe, VpLength, VnCrossVeDotVp;
	RayFloat Phi, theta;

	DDK_PRIMITIVE_INFO* pPrimInfo	= &g_mem.pPrimitiveInfo[prim_num];

	if(pPrimInfo->fGroup==0){
		if(hit.gamma&0x800000)	hit.gamma=0;
		if(hit.beta&0x800000)	hit.beta=0;

		RCVECTOR2u	tex_coord_0	= g_mem.PrimitiveInfoTexCoord(prim_num, 0);
		RCVECTOR2u	tex_coord_1	= g_mem.PrimitiveInfoTexCoord(prim_num, 1);
		RCVECTOR2u	tex_coord_2	= g_mem.PrimitiveInfoTexCoord(prim_num, 2);

		TB_TA[0] = fmul(fadder(tex_coord_1.u, tex_coord_0.u, 1), hit.beta);
		TB_TA[1] = fmul(fadder(tex_coord_1.v, tex_coord_0.v, 1), hit.beta);
		TC_TA[0] = fmul(TFAdder(tex_coord_2.u, SignConv(tex_coord_0.u), 0), hit.gamma);
		TC_TA[1] = fmul(TFAdder(tex_coord_2.v, SignConv(tex_coord_0.v), 0), hit.gamma);
		tex_coord[0] = TFAdder(tex_coord_0.u, TB_TA[0], TC_TA[0]);
		tex_coord[1] = TFAdder(tex_coord_0.v, TB_TA[1], TC_TA[1]);
	}
	else if(pPrimInfo->fGroup==2){
		RCVECTOR3u	norm_0	= g_mem.PrimitiveInfoNormal(prim_num, 0);
		Vn[0]= norm_0.x;
		Vn[1]= norm_0.y;
		Vn[2]= norm_0.z;
		Ve[0]= 0xbf0000;
		Ve[1]= 0;
		Ve[2]= 0;
		{
			RCVECTOR3u CenterPos	= g_mem.PrimitiveCenterPos(RayHit.prim_num);
			Vp[0] = fadder(Position[0],CenterPos.x, 1);
			Vp[1] = fadder(Position[1],CenterPos.y, 1);
			Vp[2] = fadder(Position[2],CenterPos.z, 1);
		}
		VpLength = TFAdder(fmul(Vp[0],Vp[0]),fmul(Vp[1],Vp[1]),fmul(Vp[2],Vp[2]));
		VpLength = SQRT(VpLength);
		Vp[0]=fmul(Vp[0],fdivider(f32f24(1.0f),VpLength));
		Vp[1]=fmul(Vp[1],fdivider(f32f24(1.0f),VpLength));
		Vp[2]=fmul(Vp[2],fdivider(f32f24(1.0f),VpLength));
		tex_coord[0] = VpLength;
		VpDotVe = TFAdder(fmul(Ve[0],Vp[0]),fmul(Ve[1],Vp[1]),fmul(Ve[2],Vp[2]));
		theta = fmul(ACOS(VpDotVe),0x3e0000);
		VnCrossVe[0]=fadder(fmul(Vn[1],Ve[2]),fmul(Vn[2],Ve[1]),1);
		VnCrossVe[1]=fadder(fmul(Vn[2],Ve[0]),fmul(Vn[0],Ve[2]),1);
		VnCrossVe[2]=fadder(fmul(Vn[0],Ve[1]),fmul(Vn[1],Ve[0]),1);
		VnCrossVeDotVp =TFAdder(fmul(VnCrossVe[0],Vp[0]),fmul(VnCrossVe[1],Vp[1]),fmul(VnCrossVe[2],Vp[2]));
		
		if(VnCrossVeDotVp&0x800000)
			tex_coord[1] = fadder(0x3f0000,theta,1);
		else
			tex_coord[1] = theta;
	}
	else if(pPrimInfo->fGroup==3){
		unsigned int SignPhi;

		Vn[0]= 0;
		Vn[1]= 0x3f0000;
		Vn[2]= 0;
		Ve[0]= 0xbf0000;
		Ve[1]= 0;
		Ve[2]= 0;
		{
			RCVECTOR3u CenterPos	= g_mem.PrimitiveCenterPos(RayHit.prim_num);
			Vp[0] = fadder(Position[0],CenterPos.x, 1);
			Vp[1] = fadder(Position[1],CenterPos.y, 1);
			Vp[2] = fadder(Position[2],CenterPos.z, 1);
		}
		VpLength = TFAdder(fmul(Vp[0],Vp[0]),fmul(Vp[1],Vp[1]),fmul(Vp[2],Vp[2]));
		VpLength = SQRT(VpLength);
		Vp[0]=fmul(Vp[0],fdivider(f32f24(1.0f),VpLength));
		Vp[1]=fmul(Vp[1],fdivider(f32f24(1.0f),VpLength));
		Vp[2]=fmul(Vp[2],fdivider(f32f24(1.0f),VpLength));      
		VnDotVp = TFAdder(fmul(Vn[0],Vp[0]),fmul(Vn[1],Vp[1]),fmul(Vn[2],Vp[2]));

		if(f24f32(VnDotVp)>1.)
			Phi=f32f24(acos(-1.f));
		else if(f24f32(VnDotVp)<-1.)
			Phi=f32f24(acos(1.f));
		else
			Phi=ACOS(SignConv(VnDotVp));

		tex_coord[1]=Phi;
		VpDotVe = TFAdder(fmul(Ve[0],Vp[0]),fmul(Ve[1],Vp[1]),fmul(Ve[2],Vp[2]));
		SignPhi=SQRT(fadder(0x3f0000,fmul(VnDotVp,VnDotVp),1));

		if((f24f32(VpDotVe)/f24f32(SignPhi))>1.)
			theta=0;
		else if((f24f32(VpDotVe)/f24f32(SignPhi))<-1.)
			theta=0x3e0000;
		else
			theta = fmul(ACOS(fdivider(VpDotVe,SignPhi)),0x3e0000);

		VnCrossVe[0]=fadder(fmul(Vn[1],Ve[2]),fmul(Vn[2],Ve[1]),1);
		VnCrossVe[1]=fadder(fmul(Vn[2],Ve[0]),fmul(Vn[0],Ve[2]),1);
		VnCrossVe[2]=fadder(fmul(Vn[0],Ve[1]),fmul(Vn[1],Ve[0]),1);
		VnCrossVeDotVp =TFAdder(fmul(VnCrossVe[0],Vp[0]),fmul(VnCrossVe[1],Vp[1]),fmul(VnCrossVe[2],Vp[2]));

		if(VnCrossVeDotVp&0x800000)
			tex_coord[0] = fadder(0x3f0000,theta,1);
		else
			tex_coord[0] = theta;
	}
}

void RayRendering::MatColorCalculator(int mat_index, _material mat, RayFloat *MatColorA, RayFloat *MatColorD, RayFloat *MatColorS){
	RayFloat n_dot_l;
	RayFloat colortemp;
	int comp1;
	RCVECTOR3u	ambient, diffuse;
	REG_MATERIAL*	pMat	= &g_reg.material[mat_index];

	n_dot_l = TFAdder(fmul(Normal[0], ray.d[0]), fmul(Normal[1], ray.d[1]), fmul(Normal[2], ray.d[2]));
	if(pMat->texture.enable){
		ambient.r	=
		diffuse.r = f32f24((float)(bilinear_tex_color[0]/256.));
		ambient.g	=
		diffuse.g = f32f24((float)(bilinear_tex_color[1]/256.));
		ambient.b	=
		diffuse.b = f32f24((float)(bilinear_tex_color[2]/256.));
	}else{
		diffuse.r	= pMat->diffuse.r;
		diffuse.g	= pMat->diffuse.g;
		diffuse.b	= pMat->diffuse.b;
		ambient.r	= pMat->ambient.r;
		ambient.g	= pMat->ambient.g;
		ambient.b	= pMat->ambient.b;
	}

	colortemp = fmul(ambient.r, light->ambient.r);
	comp1= compare(colortemp,0);
	if(comp1==2)
		colortemp=0;
	MatColorA[0] = colortemp;

	colortemp = fmul(ambient.g, light->ambient.g);
	comp1= compare(colortemp,0);
	if(comp1==2)
		colortemp=0;
	MatColorA[1] = colortemp;

	colortemp = fmul(ambient.b, light->ambient.b);
	comp1= compare(colortemp,0);
	if(comp1==2)
		colortemp=0;
	MatColorA[2] = colortemp;

	// Diffuse
	comp1= compare(n_dot_l,0);
	if(comp1==1){            
		colortemp = fmul(fmul(fmul(fmul(diffuse.r,light->diffuse.r),TransmitColor[0]),n_dot_l),LightAtten);//dk 130529 light attenuation added
		comp1= compare(colortemp,0);
		if(comp1==2)
			colortemp=0;
		MatColorD[0] = colortemp;

		colortemp = fmul(fmul(fmul(fmul(diffuse.g,light->diffuse.g),TransmitColor[1]),n_dot_l),LightAtten);//dk 130529 light attenuation added
		comp1= compare(colortemp,0);
		if(comp1==2)
			colortemp=0;
		MatColorD[1] = colortemp;

		colortemp = fmul(fmul(fmul(fmul(diffuse.b,light->diffuse.b),TransmitColor[2]),n_dot_l),LightAtten);//dk 130529 light attenuation added
		comp1= compare(colortemp,0);
		if(comp1==2)
			colortemp=0;
		MatColorD[2] = colortemp;
	}
	// Specular
	comp1= compare(n_dot_l,0);
	if(comp1==1){
		RayFloat v[3], r[3], r_dot_v, r_pow_s, n_dot_l1;
		v[0] = SignConv(Direction[0]);
		v[1] = SignConv(Direction[1]);
		v[2] = SignConv(Direction[2]);
		n_dot_l1 = fadder(n_dot_l, n_dot_l, 0);
		r[0] = fadder(fmul(Normal[0], n_dot_l1), ray.d[0], 1);
		r[1] = fadder(fmul(Normal[1], n_dot_l1), ray.d[1], 1);
		r[2] = fadder(fmul(Normal[2], n_dot_l1), ray.d[2], 1);
		r_dot_v = TFAdder(fmul(r[0], v[0]), fmul(r[1], v[1]), fmul(r[2], v[2]));

		comp1= compare(r_dot_v,0);
		if(comp1==1){
			r_pow_s = fexp(r_dot_v, pMat->specular.exponent);

			colortemp = fmul(fmul(fmul(fmul(fmul(fmul(n_dot_l,pMat->specular.level),LightAtten),pMat->specular.r),light->specular.r),TransmitColor[0]),r_pow_s);//dk 130529 light attenuation added
			comp1= compare(colortemp,0);
			if(comp1==2)
				colortemp=0;
			MatColorS[0] = colortemp;

			colortemp = fmul(fmul(fmul(fmul(fmul(fmul(n_dot_l,pMat->specular.level),LightAtten),pMat->specular.g),light->specular.g),TransmitColor[1]),r_pow_s);//dk 130529 light attenuation added
			comp1= compare(colortemp,0);
			if(comp1==2)
				colortemp=0;
			MatColorS[1] = colortemp;

			colortemp = fmul(fmul(fmul(fmul(fmul(fmul(n_dot_l,pMat->specular.level),LightAtten),pMat->specular.b),light->specular.b),TransmitColor[2]),r_pow_s);//dk 130529 light attenuation added
			comp1= compare(colortemp,0);
			if(comp1==2)
				colortemp=0;
			MatColorS[2] = colortemp;
		}
	}
}

int RayRendering::TexLODCalculator(RayFloat BaseLod, RayFloat CamDistance){
	RayFloat LodF;
	int Lod;
	LodF = fmul(fmul(ray.TotLength, BaseLod),fdivider(f32f24(1.0f),CamDistance));
#ifdef RT32
	Lod = log(LodF)/log(2.);
#else
	Lod = (LodF>>16)-63;
	if((LodF&0xffff)!=0)//dk 130529 added
		Lod+=1;
#endif

	return Lod;
}

void RayRendering::TexColorCalculator(int mat_index, _material mat, _texture_index texture_index, int Lod, RayFloat *tex_coord, RayFloat *tex_color){
	int i,j;
	RayFloat s, t;
	RayFloat u, v;
	int c[4][4] = {0};
	int MaxLod, TexStartAddr;
	int i_tex_color;
	int addr;
	int width, height;
	int delta, x, y;
	_texture tex = texture[texture_id[mat_index].texture_num];   

#ifdef RT32
	s = (float)fmod(tex_coord[0], 1.0f) ;
	if (s<0) s+= 1.0f;
	t = (float)fmod(tex_coord[1], 1.0f) ;
	if (t<0) t+= 1.0f;
#else
	s = nmod(tex_coord[0]);
	t = nmod(tex_coord[1]);
	s=s<<3;  //10bit
	t=t<<3;
#endif
	if(tex.height>tex.width)
		MaxLod = (int)(log((double)tex.width)/log(2.));
	else 
		MaxLod = (int)(log((double)tex.height)/log(2.));
	MaxLod = MaxLod-3;

   delta = 0;//dk lod zoom in off
	if(Lod<0) 
		Lod = 0;
	if(Lod>MaxLod) 
		Lod = MaxLod;

	TexStartAddr = g_mem.LodTable[Lod]<<(texture_index.sizeX[texture_id[mat_index].texture_num] + texture_index.sizeY[texture_id[mat_index].texture_num]+2-2*Lod);
	width = tex.width>>Lod;
	height = tex.height>>Lod;

#ifdef RT32
	x = s*width;
	y = t*height;
	u = s*width-x;
	v = t*height-y;
#else
	s=s>>(11-texture_index.sizeX[texture_id[mat_index].texture_num]+Lod);
	t=t>>(11-texture_index.sizeY[texture_id[mat_index].texture_num]+Lod);
	x=s>>8;
	y=t>>8;
	u=s&0xff;
	v=t&0xff;
#endif

	for(j=0;j<2;j++){
		for(i=0;i<2;i++){
			int x1, y1;
#ifdef RT32
			x1 = x + u + i*pow(0.5,delta);
			y1 = y + v + j*pow(0.5,delta);
#else
			x1= x+ ((u+i*(0x100>>delta))>>8);
			y1= y+ ((v+j*(0x100>>delta))>>8);
#endif
			if (x1>=width) x1 = 0;
			if (y1>=height) y1 = 0;

			addr = TexStartAddr+(y1&0xfff8)*width+((x1&0xfff8)<<3)+ ((y1&0x4)<<3)+((x1&0x4)<<2)+((y1&0x3)<<2)|(x1&0x3);

			if(tex.colorbit==32){
				i_tex_color = tex.colordata32[addr];
				c[j*2+i][0] = ((i_tex_color>>24) & 0xFF);  //0.xx
				c[j*2+i][1] = ((i_tex_color>>16) & 0xFF);
				c[j*2+i][2] = ((i_tex_color>>8) & 0xFF);
				c[j*2+i][3] = (i_tex_color & 0xFF);

				if(c[j*2+i][0]==0xff)
					c[j*2+i][0]=0x100;
				if(c[j*2+i][1]==0xff)
					c[j*2+i][1]=0x100;
				if(c[j*2+i][2]==0xff)
					c[j*2+i][2]=0x100;
				if(c[j*2+i][3]==0xff)
					c[j*2+i][3]=0x100;
			}
		}
	}
#ifdef RT32
	float ASize, BSize, CSize, DSize;
	ASize = (1.f-u)*(1.f-v) * pow(4.f,delta);
	BSize = (u+pow(0.5f,delta)-1.f)*(1.f-v) * pow(4.f,delta);
	CSize = (1.f-u)*(v+pow(0.5f,delta)-1.f) * pow(4.f,delta);
	DSize = (u+pow(0.5f,delta)-1.f)*(v+pow(0.5f,delta)-1.f) * pow(4.f,delta);
#else       
	int ASize, BSize, CSize, DSize;
	int uSel, vSel;
	uSel = (u+(0x100>>delta))>>8;
	vSel = (v+(0x100>>delta))>>8;
	ASize = (0x100 - u             ) * (0x100 - v             );
	BSize = (u+(0x100>>delta)-0x100) * (0x100 - v             );
	CSize = (0x100 - u             ) * (v+(0x100>>delta)-0x100);
	DSize = (u+(0x100>>delta)-0x100) * (v+(0x100>>delta)-0x100);

	if(uSel == 0 && vSel==0){
		ASize=0x100;
		BSize=0;
		CSize=0;
		DSize=0;
	}
	else if(uSel == 1 && vSel==0){
		ASize=((0x100-u)<<delta);
		BSize = 0x100-ASize;
		CSize=0;
		DSize=0;
	}
	else if(uSel == 0 && vSel==1){
		ASize=((0x100-v)<<delta);
		BSize=0;
		CSize=0x100-ASize;
		DSize=0;
	}
	else{
		BSize = (BSize<<(2*delta))>>8;
		CSize = (CSize<<(2*delta))>>8;
		DSize = (DSize<<(2*delta))>>8;
		ASize = 0x100- BSize-CSize-DSize;
	}
#endif

	for(i=0;i<4;i++){
#ifdef RT32
		tex_color[i] = (float)(c[0][i]/256. * ASize +
		c[1][i]/256. * BSize +
		c[2][i]/256. * CSize +
		c[3][i]/256. * DSize);
#else
		tex_color[i] = ((c[0][i] * ASize)>>6) +
			((c[1][i] * BSize)>>6) +
			((c[2][i] * CSize)>>6) +
			((c[3][i] * DSize)>>6);

		tex_color[i] = tex_color[i]>>2;
		if(tex_color[i]==0xff)
			tex_color[i]=0x100;
#endif
	}
}

void RayRendering::calculateNormalFromTexture(int mat_index, _material mat, _texture_index normalmap_index, int Lod, RayFloat *tex_coord, int hit_prim_num){
	int i;
	RayFloat s, t;
	RayFloat u, v;
	int normalMapData[3] = {0};
	int TexStartAddr;
	int i_tex_color;
	int addr;
	int width, height;
	int x, y;
	int MaxLod, delta;
	_texture nm = normalmap[texture_id[mat_index].normalmap_num];

	RCVECTOR3u	t_normal, t_tangent, t_binormal;
	RayFloat normal_temp;
	RayFloat newNormal[3];
	RayFloat NDotD;
	int comp;

	s = nmod(tex_coord[0]);
	t = nmod(tex_coord[1]);
	s=s<<3;  //10bit
	t=t<<3;

	//Lod = 0; //dk 130529 deleted
	delta = 0;

	if(nm.height>nm.width)
		MaxLod = (int)(log((double)nm.width)/log(2.));
	else 
		MaxLod = (int)(log((double)nm.height)/log(2.));
	MaxLod = MaxLod-3;
	if(Lod<0)
		delta=(-Lod);
	else
		delta = 0;
	if(delta>5)
		delta=5;

	if(Lod<0) 
		Lod = 0;
	if(Lod>MaxLod) 
		Lod = MaxLod;

	TexStartAddr = g_mem.LodTable[Lod]<<(normalmap_index.sizeX[texture_id[mat_index].normalmap_num] + normalmap_index.sizeY[texture_id[mat_index].normalmap_num]+2-2*Lod);
	width = nm.width>>Lod;
	height = nm.height>>Lod;

	s=s>>(11-normalmap_index.sizeX[texture_id[mat_index].normalmap_num]+Lod);
	t=t>>(11-normalmap_index.sizeY[texture_id[mat_index].normalmap_num]+Lod);
	x=s>>8;
	y=t>>8;
	u=s&0xff;
	v=t&0xff;

	addr = TexStartAddr+(y&0xfff8)*width+((x&0xfff8)<<3)+ ((y&0x4)<<3)+((x&0x4)<<2)+((y&0x3)<<2)|(x&0x3);
	if(nm.colorbit==32){
		i_tex_color = nm.colordata32[addr];
		normalMapData[0] = ((i_tex_color>>24) & 0xFF);  //0.xx
		normalMapData[1] = ((i_tex_color>>16) & 0xFF);
		normalMapData[2] = ((i_tex_color>>8) & 0xFF);
	}
	for(i=0;i<3;i++){
		normalMapData[i] = (normalMapData[i] - 128);
		if(normalMapData[i] < 0)
			Normal[i] = SignConv(f32f24((float)((~normalMapData[i]) / 128.)));
		else
			Normal[i] = f32f24((float)(normalMapData[i] / 128.));
	}

	
	t_tangent	= g_mem.PrimitiveInfoNormal(hit_prim_num, 0);
	t_binormal	= g_mem.PrimitiveInfoNormal(hit_prim_num, 1);
	t_normal	= g_mem.PrimitiveInfoNormal(hit_prim_num, 2);
	newNormal[0] = TFAdder(fmul(Normal[0], t_tangent.x), fmul(Normal[1], t_binormal.x), fmul(Normal[2], t_normal.x));
	newNormal[1] = TFAdder(fmul(Normal[0], t_tangent.y), fmul(Normal[1], t_binormal.y), fmul(Normal[2], t_normal.y));
	newNormal[2] = TFAdder(fmul(Normal[0], t_tangent.z), fmul(Normal[1], t_binormal.z), fmul(Normal[2], t_normal.z));

	Normal[0]=newNormal[0];
	Normal[1]=newNormal[1];
	Normal[2]=newNormal[2];

	NDotD = TFAdder(fmul(t_normal.x, Direction[0]), fmul(t_normal.y, Direction[1]), fmul(t_normal.z, Direction[2]));//dk 130610 modified
	comp=compare(NDotD,0);

	normal_temp=TFAdder(fmul(Normal[0],Normal[0]),fmul(Normal[1],Normal[1]),fmul(Normal[2],Normal[2]));
	normal_temp=SQRT(normal_temp);
	Normal[0]=fmul(Normal[0],fdivider(f32f24(1.0f),normal_temp));
	Normal[1]=fmul(Normal[1],fdivider(f32f24(1.0f),normal_temp));
	Normal[2]=fmul(Normal[2],fdivider(f32f24(1.0f),normal_temp));

	if(comp==1){
		Normal[0] = SignConv(Normal[0]);
		Normal[1] = SignConv(Normal[1]);
		Normal[2] = SignConv(Normal[2]);
		if(refraction_index!=0)
			refraction_index = fdivider(f32f24(1.0f),refraction_index);    
	}  
}

void RayRendering::NextRayInfoParameter(int mat_index, int Depth, int MaxDepth, _material mat){
	if(Depth<MaxDepth){
		REG_MATERIAL*	pMat	= &g_reg.material[mat_index];
		if (pMat->reflectance !=0){
			reflection_color[0] = fmul(pMat->reflectance, pMat->specular.r);
			reflection_color[1] = fmul(pMat->reflectance, pMat->specular.g);
			reflection_color[2] = fmul(pMat->reflectance, pMat->specular.b);
		}
		else{
			reflection_color[0] = 0;
			reflection_color[1] = 0;
			reflection_color[2] = 0;
		}

		if (pMat->transmittance !=0){
			refraction_color[0] = pMat->transmittance;
			refraction_color[1] = pMat->transmittance;
			refraction_color[2] = pMat->transmittance;
			refraction_index	= pMat->refraction_index;
		}
		else{
			refraction_color[0] = 0;
			refraction_color[1] = 0;
			refraction_color[2] = 0;
			refraction_index = 0;
		}
		if(pMat->texture.enable){
			if(bilinear_tex_color[3]!=0){
				reflection_color[0] = fmul(reflection_color[0], fadder(f32f24(1.0f), f32f24((float)(bilinear_tex_color[3]/256.)), 1));
				reflection_color[1] = fmul(reflection_color[1], fadder(f32f24(1.0f), f32f24((float)(bilinear_tex_color[3]/256.)), 1));
				reflection_color[2] = fmul(reflection_color[2], fadder(f32f24(1.0f), f32f24((float)(bilinear_tex_color[3]/256.)), 1));
				refraction_color[0] = fmul(refraction_color[0], fadder(f32f24(1.0f), f32f24((float)(bilinear_tex_color[3]/256.)), 1));
				refraction_color[1] = fmul(refraction_color[1], fadder(f32f24(1.0f), f32f24((float)(bilinear_tex_color[3]/256.)), 1));
				refraction_color[2] = fmul(refraction_color[2], fadder(f32f24(1.0f), f32f24((float)(bilinear_tex_color[3]/256.)), 1));
			}
			else{
				reflection_color[0] = 0;
				reflection_color[1] = 0;
				reflection_color[2] = 0;
				refraction_color[0] = f32f24(1.0f);
				refraction_color[1] = f32f24(1.0f);
				refraction_color[2] = f32f24(1.0f);
				refraction_index = f32f24(1.0f);
			}
		}
	}
	else{
		reflection_color[0] = 0;
		reflection_color[1] = 0;
		reflection_color[2] = 0;
		refraction_color[0] = 0;
		refraction_color[1] = 0;
		refraction_color[2] = 0;
		refraction_index = 0;
	}
}

//void RayRendering::LightAttenuation(RayFloat *LAcolorA, RayFloat *LAcolorD, RayFloat *LAcolorS){ //dk 130529 light attenuation modification
void RayRendering::LightAttenuation(void){
	int comp1, comp2;

	if(light->atten.enable){
		comp1 = compare(light->atten.start, ray.length);
		comp2 = compare(light->atten.end, ray.length);
		if(comp1!=2)
			LightAtten=f32f24(1.f);
		else if(comp2==2)
			LightAtten = 0;
		else{
			LightAtten = TFAdder(light->atten.weight[0],fmul(light->atten.weight[1],ray.length),fmul(light->atten.weight[2],fmul(ray.length,ray.length)));
			LightAtten = fdivider(f32f24(1.f),LightAtten);
		}
	}
	else
		LightAtten = f32f24(1.f);
	if(light->atten.type==2)
		LightAtten = fmul(LightAtten, SpotAtten);
}

void RayRendering::CameraAttenuation(void){
	int comp1, comp2;
	
	if(g_reg.option.set.camera_atten){
		comp1 = compare(g_reg.option.camera.atten.start, ray.TotLength);
		comp2 = compare(g_reg.option.camera.atten.end, ray.TotLength);
		if(comp1!=2)
			CamAtten=f32f24(1.f);
		else if(comp2==2)
			CamAtten = 0;
		else{
			CamAtten = TFAdder(g_reg.option.camera.atten.weight[0],fmul(g_reg.option.camera.atten.weight[1],hit.t),fmul(g_reg.option.camera.atten.weight[2],fmul(hit.t,hit.t)));
			CamAtten = fdivider(f32f24(1.f),CamAtten);
		}
		Color[0] = fadder(Color[0],fmul(fmul(Weight[0],fadder(f32f24(1.f),SignConv(CamAtten),0)),g_reg.control.background.r),0);
		Color[1] = fadder(Color[1],fmul(fmul(Weight[1],fadder(f32f24(1.f),SignConv(CamAtten),0)),g_reg.control.background.g),0);
		Color[2] = fadder(Color[2],fmul(fmul(Weight[2],fadder(f32f24(1.f),SignConv(CamAtten),0)),g_reg.control.background.b),0);            

		Weight[0] = fmul(Weight[0],CamAtten);
		Weight[1] = fmul(Weight[1],CamAtten);
		Weight[2] = fmul(Weight[2],CamAtten);
	}
	else 
		CamAtten = f32f24(1.f);
}

void RayRendering::ColorAccumulator(RayFloat *CAcolorA, RayFloat *CAcolorD, RayFloat *CAcolorS){
	int i;
	int comp1;
	RayFloat maxSpecular;

	if(light_num == 1){
		for(i=0;i<3;i++){
			accColorAD[i] = 0;
			accColorS[i] = 0;
		}
	}

	accColorAD[0] = TFAdder(accColorAD[0], TFAdder(CAcolorA[0],CAcolorD[0],0), 0);
	accColorAD[1] = fadder (accColorAD[1], fadder (CAcolorA[1],CAcolorD[1],0), 0);
	accColorAD[2] = fadder (accColorAD[2], fadder (CAcolorA[2],CAcolorD[2],0), 0);

	accColorS[0] = TFAdder(accColorS[0], CAcolorS[0], 0);
	accColorS[1] = fadder (accColorS[1], CAcolorS[1], 0);
	accColorS[2] = fadder (accColorS[2], CAcolorS[2], 0);


	if(light_num == g_reg.option.set.light_number){//dk last light
		if(accColorS[0] > accColorS[1])//dk
			maxSpecular = accColorS[0];
		else
			maxSpecular = accColorS[1];
		if(maxSpecular < accColorS[2])
			maxSpecular = accColorS[2];

		accColorS[0] = fmul(accColorS[0],Weight[0]);
		accColorS[1] = fmul(accColorS[1],Weight[1]);
		accColorS[2] = fmul(accColorS[2],Weight[2]);

		accColorAD[0] = fmul(accColorAD[0],fmul(fadder(0x3f0000, SignConv(refraction_color[0]), 0), Weight[0]));
		accColorAD[1] = fmul(accColorAD[1],fmul(fadder(0x3f0000, SignConv(refraction_color[1]), 0), Weight[1]));
		accColorAD[2] = fmul(accColorAD[2],fmul(fadder(0x3f0000, SignConv(refraction_color[2]), 0), Weight[2]));

		Color[0]=fadder(accColorAD[0], fadder(Color[0],accColorS[0],0), 0);
		Color[1]=fadder(accColorAD[1], fadder(Color[1],accColorS[1],0), 0);
		Color[2]=fadder(accColorAD[2], fadder(Color[2],accColorS[2],0), 0);

		refraction_color[0] = fmul(refraction_color[0], fadder(0x3f0000, SignConv(maxSpecular), 0));
		refraction_color[1] = fmul(refraction_color[1], fadder(0x3f0000, SignConv(maxSpecular), 0));
		refraction_color[2] = fmul(refraction_color[2], fadder(0x3f0000, SignConv(maxSpecular), 0));
	}

	comp1= compare(Color[0],f32f24(1.f));
	if(comp1==1)
		Color[0] =f32f24(1.f);
	comp1= compare(Color[1],f32f24(1.f));
	if(comp1==1)
		Color[1] =f32f24(1.f);
	comp1= compare(Color[2],f32f24(1.f));
	if(comp1==1)
		Color[2] =f32f24(1.f);

	ray.weight[0]=Weight[0];
	ray.weight[1]=Weight[1];
	ray.weight[2]=Weight[2];
}
