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
// DATE    : 8/27/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#include <stdio.h>
#include "RayRendering.h"
#include "operate.h"
#include "EmulSystem.h"

extern _option	*RtOpt;

void ray_generation(_RayGenInput *InData, _RayGenOutput *OutData){
	_ray_in_buffer Refl_ray_InStack, Refr_ray_InStack;
	RayFloat inv_length;
	RayFloat tempRayDirection[3]; //dk 130529 direction light modification
#ifdef USE_REDUCED_ERROR_CORRECTION
	RayFloat significantFigureOne;//dk significant figure * 1.0
#endif
	*OutData->valid_traversal=1;
	//if ray generation is failed, depth of output ray is -1
	if (!*InData->valid_ray_generation){
		OutData->ray->depth = (-1);
		printf("RayGen :  Error\n");
		return;
	}

	//generate primary ray
	if (*InData->RayKind==RAY_TYPE_PRIMARY){
		RayFloat dx, dy, dz;
		float EyeWidth;

		//EyeWidth = f24f32(RtCamera->distance)*0.0325;
		EyeWidth = 0;
		if((InData->ScreenY&1)==0){
			OutData->ray->o[0] = f32f24(f24f32(g_reg.option.camera.eye.x)-EyeWidth*f24f32(g_reg.option.camera.basis[0].x));
			OutData->ray->o[1] = f32f24(f24f32(g_reg.option.camera.eye.y)-EyeWidth*f24f32(g_reg.option.camera.basis[0].y));
			OutData->ray->o[2] = f32f24(f24f32(g_reg.option.camera.eye.z)-EyeWidth*f24f32(g_reg.option.camera.basis[0].z));
			dx = fadder(fadder(g_reg.control.start.x,f32f24(EyeWidth*1.1f),0),fmul(f32f24((float)InData->ScreenX),g_reg.control.pixel_size.x),0);
		}
		else {
			OutData->ray->o[0] = f32f24(f24f32(g_reg.option.camera.eye.x)+EyeWidth*f24f32(g_reg.option.camera.basis[0].x));
			OutData->ray->o[1] = f32f24(f24f32(g_reg.option.camera.eye.y)+EyeWidth*f24f32(g_reg.option.camera.basis[0].y));
			OutData->ray->o[2] = f32f24(f24f32(g_reg.option.camera.eye.z)+EyeWidth*f24f32(g_reg.option.camera.basis[0].z));
			dx = fadder(fadder(g_reg.control.start.x,f32f24(EyeWidth*1.1f),1),fmul(f32f24((float)InData->ScreenX),g_reg.control.pixel_size.x),0);
		}

		dy = fadder(g_reg.control.start.y,fmul(f32f24((float)(InData->ScreenY)),g_reg.control.pixel_size.y),0);
		dz = fadder(0,g_reg.option.camera.distance,1);

		OutData->ray->d[0] = TFAdder(fmul(g_reg.option.camera.basis[0].x, dx), fmul(g_reg.option.camera.basis[1].x,dy), fmul(g_reg.option.camera.basis[2].x, dz)); //direction of primary ray 
		OutData->ray->d[1] = TFAdder(fmul(g_reg.option.camera.basis[0].y, dx), fmul(g_reg.option.camera.basis[1].y,dy), fmul(g_reg.option.camera.basis[2].y, dz)); //direction of primary ray 
		OutData->ray->d[2] = TFAdder(fmul(g_reg.option.camera.basis[0].z, dx), fmul(g_reg.option.camera.basis[1].z,dy), fmul(g_reg.option.camera.basis[2].z, dz)); //direction of primary ray 
		OutData->ray->weight[0]	= 0x3f0000;
		OutData->ray->weight[1]	= 0x3f0000;
		OutData->ray->weight[2]	= 0x3f0000;
		OutData->ray->TotLength	= 0;
		OutData->ray->depth		= 0;
		*OutData->StackCnt		= 0;
	}
	//shadow ray, 섀도우 옵션이 켜져 있지 않을 때에도 광원과 교차점 사이의 벡터가 셰이딩에 쓰임
	else if (*InData->RayKind==RAY_TYPE_SHADOW){
		RayFloat n_dot_l;
		int comp;

		if(g_reg.light[*InData->light_num].atten.type==1){// direction light

			OutData->ray->d[0] = fadder( 0, g_reg.light[*InData->light_num].direction.x, 1);
			OutData->ray->d[1] = fadder( 0, g_reg.light[*InData->light_num].direction.y, 1);
			OutData->ray->d[2] = fadder( 0, g_reg.light[*InData->light_num].direction.z, 1);
			tempRayDirection[0] = fadder( g_reg.light[*InData->light_num].position.x, InData->Position[0], 1);
			tempRayDirection[1] = fadder( g_reg.light[*InData->light_num].position.y, InData->Position[1], 1);
			tempRayDirection[2] = fadder( g_reg.light[*InData->light_num].position.z, InData->Position[2], 1);
		}
		else {
			OutData->ray->d[0] = fadder( g_reg.light[*InData->light_num].position.x, InData->Position[0], 1);
			OutData->ray->d[1] = fadder( g_reg.light[*InData->light_num].position.y, InData->Position[1], 1);
			OutData->ray->d[2] = fadder( g_reg.light[*InData->light_num].position.z, InData->Position[2], 1);
		}
		n_dot_l = TFAdder(fmul(InData->Normal[0], OutData->ray->d[0]), fmul(InData->Normal[1], OutData->ray->d[1]), fmul(InData->Normal[2], OutData->ray->d[2]));
		comp=compare(n_dot_l,0);
		if(comp!=1)
			*OutData->valid_traversal = 0;
#ifdef USE_REDUCED_ERROR_CORRECTION
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////dk Rendering Error Correction : Use Significant figure
		significantFigureOne = (InData->positionSignificantFigure[0] + 0x40)<<16;//dk significant digit * 1.0
		/*if(InData->Direction[0] != 0){
			if((InData->Direction[0] >> 23) ^ (InData->Position[0] >> 23)) //dk comparison between sign bit of Direction and sign bit of Position
				OutData->ray->o[0] = fadder(InData->Position[0], significantFigureOne, 0);//dk each sign bit are not equal
			else
				OutData->ray->o[0] = fadder(InData->Position[0], significantFigureOne, 1);//dk each sign bit are equal
		}
		else{//dk ray.d == 0
			OutData->ray->o[0] = InData->Position[0];
		}*/
		if(f24f32(InData->Direction[0]) > 0)
			OutData->ray->o[0] = fadder(InData->Position[0], SignConv(significantFigureOne), 0);
		else if(f24f32(InData->Direction[0]) < 0)
			OutData->ray->o[0] = fadder(InData->Position[0], significantFigureOne, 0);
		else
			OutData->ray->o[0] = InData->Position[0];
		
		significantFigureOne = (InData->positionSignificantFigure[1] + 0x40)<<16;//dk significant digit * 1.0
		/*if(InData->Direction[1] != 0){
			if((InData->Direction[1] >> 23) ^ (InData->Position[1] >> 23)) //dk comparison between sign bit of Direction and sign bit of Position
				OutData->ray->o[1] = fadder(InData->Position[1], significantFigureOne, 0);//dk each sign bit are not equal
			else
				OutData->ray->o[1] = fadder(InData->Position[1], significantFigureOne, 1);//dk each sign bit are equal
		}
		else{//dk ray.d == 0
			OutData->ray->o[1] = InData->Position[1];
		}*/
		if(f24f32(InData->Direction[1]) > 0)
			OutData->ray->o[1] = fadder(InData->Position[1], SignConv(significantFigureOne), 0);
		else if(f24f32(InData->Direction[1]) < 0)
			OutData->ray->o[1] = fadder(InData->Position[1], significantFigureOne, 0);
		else
			OutData->ray->o[1] = InData->Position[1];
		
		significantFigureOne = (InData->positionSignificantFigure[2] + 0x40)<<16;//dk significant digit * 1.0
		/*if(InData->Direction[2] != 0){
			if((InData->Direction[2] >> 23) ^ (InData->Position[2] >> 23)) //dk comparison between sign bit of Direction and sign bit of Position
				OutData->ray->o[2] = fadder(InData->Position[2], significantFigureOne, 0);//dk each sign bit are not equal
			else
				OutData->ray->o[2] = fadder(InData->Position[2], significantFigureOne, 1);//dk each sign bit are equal
		}
		else{//dk ray.d == 0
			OutData->ray->o[2] = InData->Position[2];
		}*/
		if(f24f32(InData->Direction[2]) > 0)
			OutData->ray->o[2] = fadder(InData->Position[2], SignConv(significantFigureOne), 0);
		else if(f24f32(InData->Direction[2]) < 0)
			OutData->ray->o[2] = fadder(InData->Position[2], significantFigureOne, 0);
		else
			OutData->ray->o[2] = InData->Position[2];
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else
		OutData->ray->o[0] = InData->Position[0];
		OutData->ray->o[1] = InData->Position[1];
		OutData->ray->o[2] = InData->Position[2];
#endif
		OutData->ray->depth = *InData->depth;
	}
	else if (*InData->RayKind==RAY_TYPE_TEXTURE_SHADOW){
		return;
	}
	else if (*InData->RayKind==RAY_TYPE_SKYBOX){
		*OutData->valid_traversal = 0;
		*OutData->SIT_primNum = InData->hit->prim_num;//dk SIT
		return;
	}
	else if (*InData->RayKind==RAY_TYPE_NORMAL){
		*OutData->valid_traversal = 0;
		*OutData->SIT_primNum = InData->hit->prim_num;//dk SIT
		return;
	}
	else if(*InData->RayKind!=0){  //reflection or refraction
		RayFloat r;
		RayFloat dn;
		int comp;
		r = f32f24(1.0f);
		dn= TFAdder(fmul(InData->Direction[0], InData->Normal[0]), fmul(InData->Direction[1], InData->Normal[1]), fmul(InData->Direction[2], InData->Normal[2]));
		if (*InData->RayKind & RAY_TYPE_REFLECTION){ //reflection
			RayFloat ndn[3];
			ndn[0] = fmul(InData->Normal[0], dn);
			ndn[1] = fmul(InData->Normal[1], dn);
			ndn[2] = fmul(InData->Normal[2], dn);

			Refl_ray_InStack.d[0] = fadder(InData->Direction[0], fmul(0x400000,ndn[0]),1);
			Refl_ray_InStack.d[1] = fadder(InData->Direction[1], fmul(0x400000,ndn[1]),1);
			Refl_ray_InStack.d[2] = fadder(InData->Direction[2], fmul(0x400000,ndn[2]),1);
			Refl_ray_InStack.weight[0] = fmul(InData->Weight[0],InData->reflection_color[0]);
			Refl_ray_InStack.weight[1] = fmul(InData->Weight[1],InData->reflection_color[1]);
			Refl_ray_InStack.weight[2] = fmul(InData->Weight[2],InData->reflection_color[2]);
#ifdef USE_REDUCED_ERROR_CORRECTION
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////dk Rendering Error Correction : Use Significant figure
			significantFigureOne = (InData->positionSignificantFigure[0] + 0x40)<<16;//dk significant digit * 1.0
			/*if(InData->Direction[0] != 0){
				if((InData->Direction[0] >> 23) ^ (InData->Position[0] >> 23)) //dk comparison between sign bit of Direction and sign bit of Position
					Refl_ray_InStack.o[0] = fadder(InData->Position[0], significantFigureOne, 0);//dk each sign bit are not equal
				else
					Refl_ray_InStack.o[0] = fadder(InData->Position[0], significantFigureOne, 1);//dk each sign bit are equal
			 }
			else{//dk ray.d == 0
				Refl_ray_InStack.o[0] = InData->Position[0];
			 }*/
			if(f24f32(InData->Direction[0]) > 0)
				Refl_ray_InStack.o[0] = fadder(InData->Position[0], SignConv(significantFigureOne), 0);
			else if(f24f32(InData->Direction[0]) < 0)
				Refl_ray_InStack.o[0] = fadder(InData->Position[0], significantFigureOne, 0);
			else
				Refl_ray_InStack.o[0] = InData->Position[0];

			significantFigureOne = (InData->positionSignificantFigure[1] + 0x40)<<16;//dk significant digit * 1.0
			/*if(InData->Direction[1] != 0){
				if((InData->Direction[1] >> 23) ^ (InData->Position[1] >> 23)) //dk comparison between sign bit of Direction and sign bit of Position
					Refl_ray_InStack.o[1] = fadder(InData->Position[1], significantFigureOne, 0);//dk each sign bit are not equal
				else
					Refl_ray_InStack.o[1] = fadder(InData->Position[1], significantFigureOne, 1);//dk each sign bit are equal
			}
			else{//dk ray.d == 0
				Refl_ray_InStack.o[1] = InData->Position[1];
			}*/
			if(f24f32(InData->Direction[1]) > 0)
				Refl_ray_InStack.o[1] = fadder(InData->Position[1], SignConv(significantFigureOne), 0);
			else if(f24f32(InData->Direction[1]) < 0)
				Refl_ray_InStack.o[1] = fadder(InData->Position[1], significantFigureOne, 0);
			else
				Refl_ray_InStack.o[1] = InData->Position[1];

			significantFigureOne = (InData->positionSignificantFigure[2] + 0x40)<<16;//dk significant digit * 1.0
			/*if(InData->Direction[2] != 0){
				if((InData->Direction[2] >> 23) ^ (InData->Position[2] >> 23)) //dk comparison between sign bit of Direction and sign bit of Position
					Refl_ray_InStack.o[2] = fadder(InData->Position[2], significantFigureOne, 0);//dk each sign bit are not equal
				else
					Refl_ray_InStack.o[2] = fadder(InData->Position[2], significantFigureOne, 1);//dk each sign bit are equal
			}
			else{//dk ray.d == 0
				Refl_ray_InStack.o[2] = InData->Position[2];
			}*/
			if(f24f32(InData->Direction[2]) > 0)
				Refl_ray_InStack.o[2] = fadder(InData->Position[2], SignConv(significantFigureOne), 0);
			else if(f24f32(InData->Direction[2]) < 0)
				Refl_ray_InStack.o[2] = fadder(InData->Position[2], significantFigureOne, 0);
			else
				Refl_ray_InStack.o[2] = InData->Position[2];
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else
			Refl_ray_InStack.o[0] = InData->Position[0];
			Refl_ray_InStack.o[1] = InData->Position[1];
			Refl_ray_InStack.o[2] = InData->Position[2];
#endif
			Refl_ray_InStack.TotLength	= OutData->ray->TotLength;
			Refl_ray_InStack.depth		= *InData->depth+1;
		}

		if (*InData->RayKind & RAY_TYPE_REFRACTION){//refraction
			RayFloat a,c;
			a = *InData->refraction_index;
			c = fadder(0, dn, 1);
			r = fadder(0x3f0000, fmul(fmul(a, a), fmul(fadder(0x3f0000, c,1),fadder(0x3f0000, c, 0))),1);

			comp= compare(r,0);

			if(comp==1){
				RayFloat acr = fadder(fmul(a, c), SQRT(r),1);
				Refr_ray_InStack.d[0] = fadder(fmul(InData->Direction[0], a), fmul(InData->Normal[0], acr),0);
				Refr_ray_InStack.d[1] = fadder(fmul(InData->Direction[1], a), fmul(InData->Normal[1], acr),0);
				Refr_ray_InStack.d[2] = fadder(fmul(InData->Direction[2], a), fmul(InData->Normal[2], acr),0);
#ifdef USE_REDUCED_ERROR_CORRECTION
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////dk Rendering Error Correction : Use Significant figure
				significantFigureOne = (InData->positionSignificantFigure[0] + 0x40)<<16;//dk significant digit * 1.0
				/*if(InData->Direction[0] != 0){
					if((InData->Direction[0] >> 23) ^ (InData->Position[0] >> 23)) //dk comparison between sign bit of Direction and sign bit of Position
						Refr_ray_InStack.o[0] = fadder(InData->Position[0], significantFigureOne, 1);//dk each sign bit are not equal
					else
						Refr_ray_InStack.o[0] = fadder(InData->Position[0], significantFigureOne, 0);//dk each sign bit are equal
				}
				else{//dk ray.d == 0
					Refr_ray_InStack.o[0] = InData->Position[0];
				}*/
				if(f24f32(InData->Direction[0]) > 0)
					Refr_ray_InStack.o[0] = fadder(InData->Position[0], significantFigureOne, 0);
				else if(f24f32(InData->Direction[0]) < 0)
					Refr_ray_InStack.o[0] = fadder(InData->Position[0], SignConv(significantFigureOne), 0);
				else
					Refr_ray_InStack.o[0] = InData->Position[0];

				significantFigureOne = (InData->positionSignificantFigure[1] + 0x40)<<16;//dk significant digit * 1.0
				/*if(InData->Direction[1] != 0){
					if((InData->Direction[1] >> 23) ^ (InData->Position[1] >> 23)) //dk comparison between sign bit of Direction and sign bit of Position
						Refr_ray_InStack.o[1] = fadder(InData->Position[1], significantFigureOne, 1);//dk each sign bit are not equal
					else
						Refr_ray_InStack.o[1] = fadder(InData->Position[1], significantFigureOne, 0);//dk each sign bit are equal
				}
				else{//dk ray.d == 0
					Refr_ray_InStack.o[1] = InData->Position[1];
				}*/
				if(f24f32(InData->Direction[1]) > 0)
					Refr_ray_InStack.o[1] = fadder(InData->Position[1], significantFigureOne, 0);
				else if(f24f32(InData->Direction[1]) < 0)
					Refr_ray_InStack.o[1] = fadder(InData->Position[1], SignConv(significantFigureOne), 0);
				else
					Refr_ray_InStack.o[1] = InData->Position[1];

				significantFigureOne = (InData->positionSignificantFigure[2] + 0x40)<<16;//dk significant digit * 1.0
				/*if(InData->Direction[2] != 0){
					if((InData->Direction[2] >> 23) ^ (InData->Position[2] >> 23)) //dk comparison between sign bit of Direction and sign bit of Position
						Refr_ray_InStack.o[2] = fadder(InData->Position[2], significantFigureOne, 1);//dk each sign bit are not equal
					else
						Refr_ray_InStack.o[2] = fadder(InData->Position[2], significantFigureOne, 0);//dk each sign bit are equal
				}
				else{//dk ray.d == 0
					Refr_ray_InStack.o[2] = InData->Position[2];
				}*/
				if(f24f32(InData->Direction[2]) > 0)
					Refr_ray_InStack.o[2] = fadder(InData->Position[2], significantFigureOne, 0);
				else if(f24f32(InData->Direction[2]) < 0)
					Refr_ray_InStack.o[2] = fadder(InData->Position[2], SignConv(significantFigureOne), 0);
				else
					Refr_ray_InStack.o[2] = InData->Position[2];
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else
				Refr_ray_InStack.o[0] = InData->Position[0];
				Refr_ray_InStack.o[1] = InData->Position[1];
				Refr_ray_InStack.o[2] = InData->Position[2];
#endif
				Refr_ray_InStack.TotLength=OutData->ray->TotLength;
				Refr_ray_InStack.depth = *InData->depth+1;
				Refr_ray_InStack.weight[0] = fmul(InData->Weight[0],InData->refraction_color[0]);
				Refr_ray_InStack.weight[1] = fmul(InData->Weight[1],InData->refraction_color[1]);
				Refr_ray_InStack.weight[2] = fmul(InData->Weight[2],InData->refraction_color[2]);
			} 
			else{//total reflection
				RayFloat ndn[3];
				ndn[0] = fmul(InData->Normal[0], dn);
				ndn[1] = fmul(InData->Normal[1], dn);
				ndn[2] = fmul(InData->Normal[2], dn);

				Refl_ray_InStack.d[0] = fadder(InData->Direction[0], fmul(0x400000,ndn[0]),1);
				Refl_ray_InStack.d[1] = fadder(InData->Direction[1], fmul(0x400000,ndn[1]),1);
				Refl_ray_InStack.d[2] = fadder(InData->Direction[2], fmul(0x400000,ndn[2]),1);

				Refl_ray_InStack.weight[0] = fmul(InData->Weight[0],fadder(InData->refraction_color[0],InData->reflection_color[0],0));
				Refl_ray_InStack.weight[1] = fmul(InData->Weight[1],fadder(InData->refraction_color[1],InData->reflection_color[1],0));
				Refl_ray_InStack.weight[2] = fmul(InData->Weight[2],fadder(InData->refraction_color[2],InData->reflection_color[2],0));
#ifdef USE_REDUCED_ERROR_CORRECTION
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////dk Rendering Error Correction : Use Significant figure
				significantFigureOne = (InData->positionSignificantFigure[0] + 0x40)<<16;//dk significant digit * 1.0
				/*if(InData->Direction[0] != 0){
					if((InData->Direction[0] >> 23) ^ (InData->Position[0] >> 23)) //dk comparison between sign bit of Direction and sign bit of Position
						Refl_ray_InStack.o[0] = fadder(InData->Position[0], significantFigureOne, 0);//dk each sign bit are not equal
					else
						Refl_ray_InStack.o[0] = fadder(InData->Position[0], significantFigureOne, 1);//dk each sign bit are equal
				}
				else{//dk ray.d == 0
					Refl_ray_InStack.o[0] = InData->Position[0];
				}*/
				if(f24f32(InData->Direction[0]) > 0)
					Refl_ray_InStack.o[0] = fadder(InData->Position[0], SignConv(significantFigureOne), 0);
				else if(f24f32(InData->Direction[0]) < 0)
					Refl_ray_InStack.o[0] = fadder(InData->Position[0], significantFigureOne, 0);
				else
					Refl_ray_InStack.o[0] = InData->Position[0];

				significantFigureOne = (InData->positionSignificantFigure[1] + 0x40)<<16;//dk significant digit * 1.0
				/*if(InData->Direction[1] != 0){
					if((InData->Direction[1] >> 23) ^ (InData->Position[1] >> 23)) //dk comparison between sign bit of Direction and sign bit of Position
						Refl_ray_InStack.o[1] = fadder(InData->Position[1], significantFigureOne, 0);//dk each sign bit are not equal
					else
						Refl_ray_InStack.o[1] = fadder(InData->Position[1], significantFigureOne, 1);//dk each sign bit are equal
				}
				else{//dk ray.d == 0
					Refl_ray_InStack.o[1] = InData->Position[1];
				}*/
				if(f24f32(InData->Direction[1]) > 0)
					Refl_ray_InStack.o[1] = fadder(InData->Position[1], SignConv(significantFigureOne), 0);
				else if(f24f32(InData->Direction[1]) < 0)
					Refl_ray_InStack.o[1] = fadder(InData->Position[1], significantFigureOne, 0);
				else
					Refl_ray_InStack.o[1] = InData->Position[1];

				significantFigureOne = (InData->positionSignificantFigure[2] + 0x40)<<16;//dk significant digit * 1.0
				/*if(InData->Direction[2] != 0){
					if((InData->Direction[2] >> 23) ^ (InData->Position[2] >> 23)) //dk comparison between sign bit of Direction and sign bit of Position
						Refl_ray_InStack.o[2] = fadder(InData->Position[2], significantFigureOne, 0);//dk each sign bit are not equal
					else
						Refl_ray_InStack.o[2] = fadder(InData->Position[2], significantFigureOne, 1);//dk each sign bit are equal
				}
				else{//dk ray.d == 0
					Refl_ray_InStack.o[2] = InData->Position[2];
				}*/
				if(f24f32(InData->Direction[2]) > 0)
					Refl_ray_InStack.o[2] = fadder(InData->Position[2], SignConv(significantFigureOne), 0);
				else if(f24f32(InData->Direction[2]) < 0)
					Refl_ray_InStack.o[2] = fadder(InData->Position[2], significantFigureOne, 0);
				else
					Refl_ray_InStack.o[2] = InData->Position[2];
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else
				Refl_ray_InStack.o[0] = InData->Position[0];
				Refl_ray_InStack.o[1] = InData->Position[1];
				Refl_ray_InStack.o[2] = InData->Position[2];
#endif
				Refl_ray_InStack.TotLength=OutData->ray->TotLength;
				Refl_ray_InStack.depth = *InData->depth+1;
			}
		}

		if ((*InData->RayKind&RAY_TYPE_REFLECTION)&&(*InData->RayKind&RAY_TYPE_REFRACTION)){
			comp = compare(r,0);
			if(comp!=1){//total reflection
				OutData->ray->o[0] = Refl_ray_InStack.o[0];
				OutData->ray->o[1] = Refl_ray_InStack.o[1];
				OutData->ray->o[2] = Refl_ray_InStack.o[2];
				OutData->ray->d[0] = Refl_ray_InStack.d[0];
				OutData->ray->d[1] = Refl_ray_InStack.d[1];
				OutData->ray->d[2] = Refl_ray_InStack.d[2];
				OutData->ray->depth = Refl_ray_InStack.depth;
				OutData->ray->TotLength = Refl_ray_InStack.TotLength;
				OutData->ray->weight[0] = Refl_ray_InStack.weight[0];
				OutData->ray->weight[1] = Refl_ray_InStack.weight[1];
				OutData->ray->weight[2] = Refl_ray_InStack.weight[2];
				*InData->RayKind&=(~RAY_TYPE_REFRACTION);
			}
			else{
				RayFloat W1, W2;
				if(compare(Refr_ray_InStack.weight[0], Refr_ray_InStack.weight[1])==1)
					W1=Refr_ray_InStack.weight[0];
				else
					W1=Refr_ray_InStack.weight[1];
				if(compare(W1, Refr_ray_InStack.weight[2])!=1)
					W1=Refr_ray_InStack.weight[2];

				if(compare(Refl_ray_InStack.weight[0], Refl_ray_InStack.weight[1])==1)
					W2=Refl_ray_InStack.weight[0];
				else
					W2=Refl_ray_InStack.weight[1];
				if(compare(W2, Refl_ray_InStack.weight[2])!=1)
					W2=Refl_ray_InStack.weight[2];

				if(compare(W1,W2)==1){
					if (*OutData->StackCnt < StackSize) {
						InData->StackData[*OutData->StackCnt].o[0] = Refr_ray_InStack.o[0];
						InData->StackData[*OutData->StackCnt].o[1] = Refr_ray_InStack.o[1];
						InData->StackData[*OutData->StackCnt].o[2] = Refr_ray_InStack.o[2];
						InData->StackData[*OutData->StackCnt].d[0] = Refr_ray_InStack.d[0];
						InData->StackData[*OutData->StackCnt].d[1] = Refr_ray_InStack.d[1];
						InData->StackData[*OutData->StackCnt].d[2] = Refr_ray_InStack.d[2];
						InData->StackData[*OutData->StackCnt].TotLength = Refr_ray_InStack.TotLength;
						InData->StackData[*OutData->StackCnt].depth = Refr_ray_InStack.depth;
      
						InData->StackData[*OutData->StackCnt].weight[0] = Refr_ray_InStack.weight[0];
						InData->StackData[*OutData->StackCnt].weight[1] = Refr_ray_InStack.weight[1];
						InData->StackData[*OutData->StackCnt].weight[2] = Refr_ray_InStack.weight[2];
						InData->StackData[*OutData->StackCnt].SIT_primNum = InData->hit->prim_num;//dk SIT
						(*OutData->StackCnt)++;
					}
					else{
						*InData->RayKind&=(~RAY_TYPE_REFRACTION);
					}
					OutData->ray->o[0] = Refl_ray_InStack.o[0];
					OutData->ray->o[1] = Refl_ray_InStack.o[1];
					OutData->ray->o[2] = Refl_ray_InStack.o[2];
					OutData->ray->d[0] = Refl_ray_InStack.d[0];
					OutData->ray->d[1] = Refl_ray_InStack.d[1];
					OutData->ray->d[2] = Refl_ray_InStack.d[2];
					OutData->ray->depth = Refl_ray_InStack.depth;
					OutData->ray->TotLength = Refl_ray_InStack.TotLength;
					OutData->ray->weight[0] = Refl_ray_InStack.weight[0];
					OutData->ray->weight[1] = Refl_ray_InStack.weight[1];
					OutData->ray->weight[2] = Refl_ray_InStack.weight[2];
				}
				else{
					if (*OutData->StackCnt < StackSize) {
						InData->StackData[*OutData->StackCnt].o[0] = Refl_ray_InStack.o[0];
						InData->StackData[*OutData->StackCnt].o[1] = Refl_ray_InStack.o[1];
						InData->StackData[*OutData->StackCnt].o[2] = Refl_ray_InStack.o[2];
						InData->StackData[*OutData->StackCnt].d[0] = Refl_ray_InStack.d[0];
						InData->StackData[*OutData->StackCnt].d[1] = Refl_ray_InStack.d[1];
						InData->StackData[*OutData->StackCnt].d[2] = Refl_ray_InStack.d[2];
						InData->StackData[*OutData->StackCnt].depth = Refl_ray_InStack.depth;
						InData->StackData[*OutData->StackCnt].TotLength = Refl_ray_InStack.TotLength;
						InData->StackData[*OutData->StackCnt].weight[0] = Refl_ray_InStack.weight[0];
						InData->StackData[*OutData->StackCnt].weight[1] = Refl_ray_InStack.weight[1];
						InData->StackData[*OutData->StackCnt].weight[2] = Refl_ray_InStack.weight[2];
						InData->StackData[*OutData->StackCnt].SIT_primNum = InData->hit->prim_num;//dk SIT
						(*OutData->StackCnt)++;
					}
					else{
						*InData->RayKind&=(~RAY_TYPE_REFRACTION);
					}
					OutData->ray->o[0] = Refr_ray_InStack.o[0];
					OutData->ray->o[1] = Refr_ray_InStack.o[1];
					OutData->ray->o[2] = Refr_ray_InStack.o[2];
					OutData->ray->d[0] = Refr_ray_InStack.d[0];
					OutData->ray->d[1] = Refr_ray_InStack.d[1];
					OutData->ray->d[2] = Refr_ray_InStack.d[2];
					OutData->ray->depth = Refr_ray_InStack.depth;
					OutData->ray->TotLength = Refr_ray_InStack.TotLength;
					OutData->ray->weight[0] = Refr_ray_InStack.weight[0];
					OutData->ray->weight[1] = Refr_ray_InStack.weight[1];
					OutData->ray->weight[2] = Refr_ray_InStack.weight[2];
				}
			}
		}
		else if(*InData->RayKind&RAY_TYPE_REFLECTION){
			OutData->ray->o[0] = Refl_ray_InStack.o[0];
			OutData->ray->o[1] = Refl_ray_InStack.o[1];
			OutData->ray->o[2] = Refl_ray_InStack.o[2];
			OutData->ray->d[0] = Refl_ray_InStack.d[0];
			OutData->ray->d[1] = Refl_ray_InStack.d[1];
			OutData->ray->d[2] = Refl_ray_InStack.d[2];
			OutData->ray->depth = Refl_ray_InStack.depth;
			OutData->ray->TotLength = Refl_ray_InStack.TotLength;
			OutData->ray->weight[0] = Refl_ray_InStack.weight[0];
			OutData->ray->weight[1] = Refl_ray_InStack.weight[1];
			OutData->ray->weight[2] = Refl_ray_InStack.weight[2];
		}
		else if(*InData->RayKind & RAY_TYPE_REFRACTION){
			comp = compare(r,0);
			if(comp!=1){//total reflection
				OutData->ray->o[0] = Refl_ray_InStack.o[0];
				OutData->ray->o[1] = Refl_ray_InStack.o[1];
				OutData->ray->o[2] = Refl_ray_InStack.o[2];
				OutData->ray->d[0] = Refl_ray_InStack.d[0];
				OutData->ray->d[1] = Refl_ray_InStack.d[1];
				OutData->ray->d[2] = Refl_ray_InStack.d[2];
				OutData->ray->depth = Refl_ray_InStack.depth;
				OutData->ray->TotLength = Refl_ray_InStack.TotLength;
				OutData->ray->weight[0] = Refl_ray_InStack.weight[0];
				OutData->ray->weight[1] = Refl_ray_InStack.weight[1];
				OutData->ray->weight[2] = Refl_ray_InStack.weight[2];
			}
			else {
				OutData->ray->o[0] = Refr_ray_InStack.o[0];
				OutData->ray->o[1] = Refr_ray_InStack.o[1];
				OutData->ray->o[2] = Refr_ray_InStack.o[2];
				OutData->ray->d[0] = Refr_ray_InStack.d[0];
				OutData->ray->d[1] = Refr_ray_InStack.d[1];
				OutData->ray->d[2] = Refr_ray_InStack.d[2];
				OutData->ray->depth = Refr_ray_InStack.depth;
				OutData->ray->TotLength = Refr_ray_InStack.TotLength;
				OutData->ray->weight[0] = Refr_ray_InStack.weight[0];
				OutData->ray->weight[1] = Refr_ray_InStack.weight[1];
				OutData->ray->weight[2] = Refr_ray_InStack.weight[2];
			}
		}
	}
	*OutData->SIT_primNum = InData->hit->prim_num;//dk SIT
	if (*InData->RayKind==0){
		if (*OutData->StackCnt==0){   
			OutData->ray->depth = -1;
			return;
		}
		else{
			//생성할 광선이 있으면 디큐
			(*OutData->StackCnt)--;
			OutData->ray->o[0] = InData->StackData[*OutData->StackCnt].o[0];
			OutData->ray->o[1] = InData->StackData[*OutData->StackCnt].o[1];
			OutData->ray->o[2] = InData->StackData[*OutData->StackCnt].o[2];
			OutData->ray->d[0] = InData->StackData[*OutData->StackCnt].d[0];
			OutData->ray->d[1] = InData->StackData[*OutData->StackCnt].d[1];
			OutData->ray->d[2] = InData->StackData[*OutData->StackCnt].d[2];
			OutData->ray->depth = InData->StackData[*OutData->StackCnt].depth;
			OutData->ray->TotLength = InData->StackData[*OutData->StackCnt].TotLength;
			OutData->ray->weight[0] = InData->StackData[*OutData->StackCnt].weight[0];
			OutData->ray->weight[1] = InData->StackData[*OutData->StackCnt].weight[1];
			OutData->ray->weight[2] = InData->StackData[*OutData->StackCnt].weight[2];
			*OutData->SIT_primNum = InData->StackData[*OutData->StackCnt].SIT_primNum;//dk SIT
		}      
	}

	//normalization
	OutData->ray->length = SQRT( TFAdder(fmul(OutData->ray->d[0], OutData->ray->d[0]), fmul(OutData->ray->d[1], OutData->ray->d[1]), fmul(OutData->ray->d[2], OutData->ray->d[2])));
	inv_length = fdivider(0x3f0000, OutData->ray->length);
	OutData->ray->inv_d[0] = fmul(OutData->ray->length,fdivider(0x3f0000, OutData->ray->d[0]));
	OutData->ray->inv_d[1] = fmul(OutData->ray->length,fdivider(0x3f0000, OutData->ray->d[1]));
	OutData->ray->inv_d[2] = fmul(OutData->ray->length,fdivider(0x3f0000, OutData->ray->d[2]));

	*OutData->SpotAtten=0;
	if (*InData->RayKind==RAY_TYPE_SHADOW&&*OutData->valid_traversal){
		if((g_reg.light[*InData->light_num].atten.type)==2){
			RayFloat LdotD, C0Length, C1Length, SpotDenom;
			int comp1, comp2;
			LdotD = TFAdder(fmul(g_reg.light[*InData->light_num].direction.x,OutData->ray->d[0]), fmul(g_reg.light[*InData->light_num].direction.y,OutData->ray->d[1]), fmul(g_reg.light[*InData->light_num].direction.z,OutData->ray->d[2]));
			LdotD = fadder(0,LdotD,1);
			C0Length = fmul(g_reg.light[*InData->light_num].spot.cos_in, OutData->ray->length);
			C1Length = fmul(g_reg.light[*InData->light_num].spot.cos_out, OutData->ray->length);
			SpotDenom = fdivider(0x3f0000, fadder(g_reg.light[*InData->light_num].spot.cos_in, g_reg.light[*InData->light_num].spot.cos_out, 1));
			comp1 = compare(LdotD,C1Length);
			comp2 = compare(LdotD,C0Length);
			if(comp1==2){
				*OutData->SpotAtten = 0;
				*OutData->valid_traversal = 0;            
			}
			else if(comp2==1)
				*OutData->SpotAtten = 0x3f0000;
			else{
				*OutData->SpotAtten = fmul(fmul(fadder(LdotD, C1Length, 1),SpotDenom),inv_length);
				*OutData->SpotAtten = fexp(*OutData->SpotAtten, g_reg.light[*InData->light_num].spot.falloff);
			}
		}
	}

	if((*InData->RayKind==RAY_TYPE_SHADOW)&&((g_reg.light[*InData->light_num].atten.type)==1)){//dk directional light
		//dk original ver.
		//OutData->ray->length=(float)(3.6893207e+018);
		//OutData->ray->length=0x7fffff;

		//dk 130529 directional light (3DS max ver.)
		OutData->ray->length = TFAdder(fmul(g_reg.light[*InData->light_num].direction.x,tempRayDirection[0]), fmul(g_reg.light[*InData->light_num].direction.y,tempRayDirection[1]), fmul(g_reg.light[*InData->light_num].direction.z,tempRayDirection[2]));;
		OutData->ray->length = SignConv(OutData->ray->length);
		if(OutData->ray->length & 0x800000) //dk check sign bit
			*OutData->valid_traversal = 0; //dk -
		else
			*OutData->valid_traversal = 1; //dk +
	}
	OutData->ray->d[0] = fmul(OutData->ray->d[0], inv_length);
	OutData->ray->d[1] = fmul(OutData->ray->d[1], inv_length);
	OutData->ray->d[2] = fmul(OutData->ray->d[2], inv_length);
}

void RayRendering::UpdateFrameBuffer(void){
	unsigned char r,g,b;

	//[0,1] clamp
#ifdef RT32
	if (Color[0] < 0) Color[0] = 0;
	if (Color[1] < 0) Color[1] = 0;
	if (Color[2] < 0) Color[2] = 0;

	if (Color[0] > 1) Color[0] = 1;
	if (Color[1] > 1) Color[1] = 1;
	if (Color[2] > 1) Color[2] = 1;

	r = ( unsigned char )( Color[0] * 255 );
	g = ( unsigned char )( Color[1] * 255 );
	b = ( unsigned char )( Color[2] * 255 );
#else
	if (Color[0]&0x800000) Color[0] = 0;
	if (Color[1]&0x800000) Color[1] = 0;
	if (Color[2]&0x800000) Color[2] = 0;

	if(((Color[0]>>16)&0x7f)>0x3e)Color[0]=0x3effff;//f32f24(1.f);
	if(((Color[1]>>16)&0x7f)>0x3e)Color[1]=0x3effff;
	if(((Color[2]>>16)&0x7f)>0x3e)Color[2]=0x3effff;

	r = (unsigned char)f24f32(fmul(Color[0],f32f24(256.f)));
	g = (unsigned char)f24f32(fmul(Color[1],f32f24(256.f)));
	b = (unsigned char)f24f32(fmul(Color[2],f32f24(256.f)));
#endif
	//write color value at frame buffer
	g_mem.pFramebuffer[m_iX + m_iY*g_reg.control.fb_stride] = (b<<16)+(g<<8)+(r);
	g_pEmulSystem->UpdateBackFramebuffer();
}
