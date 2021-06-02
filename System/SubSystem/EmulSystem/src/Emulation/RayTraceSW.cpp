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
// DATE    : 8/28/2013 Wed
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#include "Emulation_Common.h"
#include "RayTraceSW.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "operate.h"
#include "EmulSystem.h"

RayTraceSW::RayTraceSW(void){
	g_reg.control.pixel_size.x		= f32f24(1.0f / 512);
	g_reg.control.pixel_size.y		= f32f24(1.0f / 512);
	g_reg.control.start.x			= f32f24(-400.f/512);
	g_reg.control.start.y			= f32f24(-240.f/512);
	g_reg.option.threshold_weight	= f32f24(0.00f);
	g_reg.option.set.ray_bounce		= 10;
	
	g_reg.option.set.shadow			= 1;
	g_reg.option.set.super_sampling	= 0;
	g_reg.option.set.normal_map		= 1;

	memset(material, 0, sizeof(_material)*MAX_MATERIAL);
	memset(texture_id, -1, sizeof(_texture_id)*MAX_MATERIAL);
}

RayTraceSW::~RayTraceSW(void){
}

int RayTraceSW::load(char folderName[256]){
	FILE *fp;
	int index=0;
	unsigned int i;
	char path_tok[256];
	char *tok;
	int TextureFlag[1024]={0};
	int tex_base_addr = 0;

	strcpy(opt.path, folderName);
	strcpy(path_tok, opt.path);
	tok = strtok(path_tok, "\\");
	while(strcmp(tok, "output") != 0){
		strcpy(opt.sceneName, tok);
		tok = strtok(NULL, "\\");
	}

	{	// 해상도 설정
		int width	= g_pEmulSystem->Read(REG_DISPLAY_WIDTH);
		int height	= g_pEmulSystem->Read(REG_DISPLAY_HEIGHT);

		
		{	// framebuffer 출력
			for(int y=0;y<height;y++)
			for(int x=0;x<width;x++)
				g_mem.pFramebuffer[x+y*width]	= ((x^y)&0x10) ? 0xFFFFFFFF : 0x7F7F7F7F;
			g_pEmulSystem->UpdateBackFramebuffer();
		}

		if(g_reg.option.set.super_sampling){
			width	*= 2;
			height	*= 2;
		}
		g_reg.control.resolution.width	= width;
		g_reg.control.resolution.height	= height;
		
	}

// 	strcpy(file,opt.path);
// 	strcat(file,"textureRemap.txt");
// 	fp=fopen(file,"rb");
// 	if(fp){
// 		int num_read=0;
// 		int TextureTotalSize, n, m, ReadSize;
// 
// 		texture_index.total_texture_num = 0;
// 		opt.TextureTotalSize = 0;
// 
// 		for (index=0; !feof(fp); index++){
// 			ReadSize=0;
// 			fread(&texture[index].width,sizeof(unsigned int),1,fp);
// 			if (feof(fp)) break;
// 			fread(&texture[index].height,sizeof(unsigned int),1,fp);
// 			fread(&texture[index].colorbit,sizeof(unsigned int),1,fp);
// 
// 			texture[index].colordata32 = (unsigned int*) malloc(2*texture[index].width * texture[index].height * sizeof(unsigned int));
// 			if(texture[index].height>texture[index].width){
// 				m=(int)(log((double)texture[index].width)/log(2.));
// 				n=(int)(log((double)texture[index].height)/log(2.));
// 			}
// 			else {
// 				n=(int)(log((double)texture[index].width)/log(2.));
// 				m=(int)(log((double)texture[index].height)/log(2.));
// 			}
// 			TextureTotalSize = (int)(pow(2.,n-m)*(pow(4.,m+1)-1)/3.);
// 			opt.TextureTotalSize += TextureTotalSize;
// 			//printf("%x\n",TextureTotalSize);//dk comment
// 
// 			texture_index.sizeX[index] = 0;
// 			texture_index.sizeY[index] = 0;
// 			if (texture[index].width == 32) texture_index.sizeX[index] = 5;
// 			if (texture[index].width == 64) texture_index.sizeX[index] = 6;
// 			if (texture[index].width == 128) texture_index.sizeX[index] = 7;
// 			if (texture[index].width == 256) texture_index.sizeX[index] = 8;
// 			if (texture[index].width == 512) texture_index.sizeX[index] = 9;
// 			if (texture[index].width == 1024) texture_index.sizeX[index] = 10;
// 
// 			if (texture[index].height == 32) texture_index.sizeY[index] = 5;
// 			if (texture[index].height == 64) texture_index.sizeY[index] = 6;
// 			if (texture[index].height == 128) texture_index.sizeY[index] = 7;
// 			if (texture[index].height == 256) texture_index.sizeY[index] = 8;
// 			if (texture[index].height == 512) texture_index.sizeY[index] = 9;
// 			if (texture[index].height == 1024) texture_index.sizeY[index] = 10;
// 
// 			texture_index.address[index] = (tex_base_addr) >> 13;      
// 
// 			do{
// 				if (texture[index].colorbit==32){
// 					if((TextureTotalSize-ReadSize)>=128)
// 						num_read = fread(&texture[index].colordata32[ReadSize],sizeof(unsigned int),128,fp);
// 					else
// 						num_read = fread(&texture[index].colordata32[ReadSize],sizeof(unsigned int),TextureTotalSize-ReadSize,fp);
// 
// 					if(num_read!=0){
// 						tex_base_addr += num_read*4;
// 						ReadSize+=num_read;
// 					}
// 					else 
// 						break;
// 				}
// 			}while(ReadSize<TextureTotalSize);
// 
// 			if((tex_base_addr&0x1fff)!=0)
// 				tex_base_addr=(tex_base_addr+0x2000)&0xffffe000;
// 		}
// 		texture_index.total_texture_num = index;
// 		fclose(fp);
// 	}
// 	else{
// 		printf("%s file is not exist\n",file);
// 	}

	//머티리얼 정보
// 	strcpy(file,opt.path);
// 	strcat(file,"f_material.txt");
// 	fp=fopen(file,"r");
// 	{
// 		int num_material;
// 		for(index=0;index<num_material;index++){
// 			fscanf(fp,"%f", &temp);
// 			material[index].dielectric				= f32f16(temp);
// 			fscanf(fp,"%f", &temp);	// refraction_index
// 			fscanf(fp,"%f", &temp);
// 			material[index].self_illumination		= f32f24(temp);
// 			if(material[index].dielectric)
// 				MaterialFlag[index]=1;
// 			else
// 				MaterialFlag[index]=0;
// 			if(material[index].self_illumination==0x3f0000)
// 				MaterialFlag[index]|=2;
// 			if(tex_index!=-1)
// 				MaterialFlag[index]|=(TextureFlag[tex_index]<<2);
// 		}
// 	}
// 	fclose(fp);

	//primitive information of kD tree
// 	strcpy(file,opt.path);
// 	strcat(file,"f_KD_prim_pool.txt");
// 	fp=fopen(file,"r");
	for (index=0; !feof(fp); index++){
		//position, normal, texture coordinate of each vertex
		//float PositionBuf[3][3];
		for (i=0;i<3;i++){
// 			fscanf(fp,"%f", &temp);
// 			
// 			kd_triangle[index].vertex[i].position[0]=f32f24(temp);
// 			kd_triangle[index].vertex[i].fposition[0]=temp;//dk
// 			fscanf(fp,"%f", &temp);
// 			kd_triangle[index].vertex[i].position[1]=f32f24(temp);
// 			kd_triangle[index].vertex[i].fposition[1]=temp;//dk
// 			fscanf(fp,"%f", &temp);
// 			kd_triangle[index].vertex[i].position[2]=f32f24(temp);
// 			kd_triangle[index].vertex[i].fposition[2]=temp;//dk
// 			fscanf(fp,"%f", &temp);
// 			kd_triangle[index].vertex[i].normal[0]=f32f24(temp);
// 			fscanf(fp,"%f", &temp);
// 			kd_triangle[index].vertex[i].normal[1]=f32f24(temp);
// 			fscanf(fp,"%f", &temp);
// 			kd_triangle[index].vertex[i].normal[2]=f32f24(temp);
// 			fscanf(fp,"%f", &temp);
// 			kd_triangle[index].vertex[i].tex_coord[0]=f32f24(temp);
// 			fscanf(fp,"%f", &temp);
// 			kd_triangle[index].vertex[i].tex_coord[1]=f32f24(temp);
		}
		//보간시 필요한 1/area 값과 셰이딩시 필요한 머티리얼 인덱스
		//다음 방문할 노드를 가리키는 escape 포인터- 교차 여부와 상관 없음)
// 		fscanf(fp,"%f", &temp);
// 		fscanf(fp,"%d", &kd_triangle[index].material_index);
// 		kd_triangle[index].fGroup=kd_triangle[index].material_index>>23;

		/*if(index == 249)
			printf("test\n");*/
		// primitive preprocessing
// 		if(PrePrimitive[index].Type==0){
// 			double Normal[3];
// 			double Vb[3], Vc[3];
// 			double nu,nv;
// 			int k,u,v;
// 
// 			for(i=0;i<3;i++){
// //  				Vb[i]= f24f32(kd_triangle[index].vertex[2].position[i])-f24f32(kd_triangle[index].vertex[0].position[i]);
// //  				Vc[i]= f24f32(kd_triangle[index].vertex[1].position[i])-f24f32(kd_triangle[index].vertex[0].position[i]);
// 				Vb[i]= kd_triangle[index].vertex[2].fposition[i]-kd_triangle[index].vertex[0].fposition[i];//dk
// 				Vc[i]= kd_triangle[index].vertex[1].fposition[i]-kd_triangle[index].vertex[0].fposition[i];
// 			}
// 			for(k=0;k<3;k++){
// 				u=(k+1)%3;
// 				v=(k+2)%3;
// 				Normal[k]= Vb[u]*Vc[v]- Vb[v]*Vc[u];
// 			}
// 			if((fabs(Normal[0])>fabs(Normal[1]))&&(fabs(Normal[0])>fabs(Normal[2]))){
// 				k=0;
// 				u=1;
// 				v=2;
// 			}
// 			else if(fabs(Normal[1])>fabs(Normal[2])){
// 				k=1;
// 				u=2;
// 				v=0;
// 			}
// 			else{
// 				k=2;
// 				u=0;
// 				v=1;
// 			}
// 			
// 			//if(fabs(Normal[k]) > 0){
// 			if(fabs(Normal[k]) > 2e-30f){
// 				nu=Normal[u]/Normal[k];
// 				nv=Normal[v]/Normal[k];
// 
// 				PrePrimitive[index].Axis = k;
// 				PrePrimitive[index].nu = f32f24((float)nu);
// 				PrePrimitive[index].nv = f32f24((float)nv);
// 				//PrePrimitive[index].nd = f32f24(f24f32(kd_triangle[index].vertex[0].position[u])*nu + f24f32(kd_triangle[index].vertex[0].position[v])*nv + f24f32(kd_triangle[index].vertex[0].position[k]));
// 				PrePrimitive[index].nd = f32f24(kd_triangle[index].vertex[0].fposition[u]*(float)nu + kd_triangle[index].vertex[0].fposition[v]*(float)nv + kd_triangle[index].vertex[0].fposition[k]);//dk
// 
// 				PrePrimitive[index].bnu = f32f24((float)(-Vb[v]/Normal[k]));
// 				PrePrimitive[index].bnv = f32f24((float)(Vb[u]/Normal[k]));
// 				//PrePrimitive[index].bd  = f32f24((f24f32(kd_triangle[index].vertex[0].position[u])*Vb[v] - f24f32(kd_triangle[index].vertex[0].position[v])*Vb[u])/Normal[k]);
// 				PrePrimitive[index].bd = f32f24((kd_triangle[index].vertex[0].fposition[u]*(float)Vb[v] - kd_triangle[index].vertex[0].fposition[v]*(float)Vb[u])/(float)Normal[k]);//dk
// 
// 				PrePrimitive[index].cnu = f32f24((float)(Vc[v]/Normal[k]));
// 				PrePrimitive[index].cnv = f32f24((float)(-Vc[u]/Normal[k]));
// 				//PrePrimitive[index].cd  = f32f24((f24f32(kd_triangle[index].vertex[0].position[v])*Vc[u] - f24f32(kd_triangle[index].vertex[0].position[u])*Vc[v])/Normal[k]);
// 				PrePrimitive[index].cd = f32f24((kd_triangle[index].vertex[0].fposition[v]*(float)Vc[u] - kd_triangle[index].vertex[0].fposition[u]*(float)Vc[v])/(float)Normal[k]);//dk
// 			}
// 			else{
// 				PrePrimitive[index].Axis = 0;
// 				PrePrimitive[index].nu = f32f24(0.0f);
// 				PrePrimitive[index].nv = f32f24(0.0f);
// 				PrePrimitive[index].nd = f32f24(10000.0f);
// 
// 				PrePrimitive[index].bnu = f32f24(0.0f);
// 				PrePrimitive[index].bnv = f32f24(0.0f);
// 				PrePrimitive[index].bd  = f32f24(10000.0f);
// 
// 				PrePrimitive[index].cnu = f32f24(0.0f);
// 				PrePrimitive[index].cnv = f32f24(0.0f);
// 				PrePrimitive[index].cd  = f32f24(10000.0f);
// 			}
// 		}
// 		else if(PrePrimitive[index].Type==2){
// 			double Normal[3];
// 			double nu,nv;
// 			int k,u,v;
// 
// 			for(k=0;k<3;k++){
// 				Normal[k]=f24f32(kd_triangle[index].vertex[0].normal[k]);
// 			}
// 
// 			if((fabs(Normal[0])>fabs(Normal[1]))&&(fabs(Normal[0])>fabs(Normal[2]))){
// 				k=0;
// 				u=1;
// 				v=2;
// 			}
// 			else if(fabs(Normal[1])>fabs(Normal[2])){
// 				k=1;
// 				u=2;
// 				v=0;
// 			}
// 			else{
// 				k=2;
// 				u=0;
// 				v=1;
// 			}
// 
// 			nu=Normal[u]/Normal[k];
// 			nv=Normal[v]/Normal[k];
// 
// 			PrePrimitive[index].Axis = k;
// 			PrePrimitive[index].CenterX = kd_triangle[index].vertex[0].Center[0];
// 			PrePrimitive[index].CenterY = kd_triangle[index].vertex[0].Center[1];
// 			PrePrimitive[index].CenterZ = kd_triangle[index].vertex[0].Center[2];
// 			PrePrimitive[index].nu = f32f24((float)nu);
// 			PrePrimitive[index].nv = f32f24((float)nv);
// 			PrePrimitive[index].nd = f32f24(f24f32(kd_triangle[index].vertex[0].position[u])*(float)nu + f24f32(kd_triangle[index].vertex[0].position[v])*(float)nv + f24f32(kd_triangle[index].vertex[0].position[k]));
// 			PrePrimitive[index].Radius = kd_triangle[index].vertex[0].Radius[0];
// 		}
// 		else if(PrePrimitive[index].Type==3){
// 			PrePrimitive[index].CenterX = kd_triangle[index].vertex[0].Center[0];
// 			PrePrimitive[index].CenterY = kd_triangle[index].vertex[0].Center[1];
// 			PrePrimitive[index].CenterZ = kd_triangle[index].vertex[0].Center[2];
// 			PrePrimitive[index].Radius = kd_triangle[index].vertex[0].Radius[0];
// 		}
// 		else {
// 			PrePrimitive[index].Axis = 0;
// 
// 			PrePrimitive[index].nu = kd_triangle[index].vertex[0].position[0];
// 			PrePrimitive[index].nv = kd_triangle[index].vertex[1].position[0];
// 
// 			PrePrimitive[index].nd = kd_triangle[index].vertex[0].position[1];
// 			PrePrimitive[index].cd = kd_triangle[index].vertex[1].position[1];
// 
// 			PrePrimitive[index].bnu = kd_triangle[index].vertex[0].position[2];
// 			PrePrimitive[index].bd  = kd_triangle[index].vertex[1].position[2];
// 
// 			PrePrimitive[index].bnv = 0;
// 			PrePrimitive[index].cnu = 0;
// 			PrePrimitive[index].cnv = 0;
// 		}
// 
// 		// LoD preprocessing
// 		if(kd_triangle[index].fGroup==0){
// 			if(g_reg.material[kd_triangle[index].material_index].texture.enable){
// 				double TriNormal[3];
// 				double Vb[3], Vc[3];
// 				double Tn, Pta,Pa,Ta, Res,Tb;
// 				int k,u,v;
// 
// 				for(i=0;i<3;i++){
// 					Vb[i]= f24f32(kd_triangle[index].vertex[2].position[i])-f24f32(kd_triangle[index].vertex[0].position[i]);
// 					Vc[i]= f24f32(kd_triangle[index].vertex[1].position[i])-f24f32(kd_triangle[index].vertex[0].position[i]);
// 				}
// 				for(k=0;k<3;k++){
// 					u=(k+1)%3;
// 					v=(k+2)%3;
// 					TriNormal[k]= Vb[u]*Vc[v]- Vb[v]*Vc[u];
// 				}
// 
// 				Pa  = sqrt(TriNormal[0]*TriNormal[0]+TriNormal[1]*TriNormal[1]+TriNormal[2]*TriNormal[2]);
// 				Pta = (( f24f32(kd_triangle[index].vertex[0].tex_coord[0]) * f24f32(kd_triangle[index].vertex[1].tex_coord[1]) + 
// 							f24f32(kd_triangle[index].vertex[1].tex_coord[0]) * f24f32(kd_triangle[index].vertex[2].tex_coord[1]) +
// 							f24f32(kd_triangle[index].vertex[2].tex_coord[0]) * f24f32(kd_triangle[index].vertex[0].tex_coord[1])) -
// 						(  f24f32(kd_triangle[index].vertex[1].tex_coord[0]) * f24f32(kd_triangle[index].vertex[0].tex_coord[1]) + 
// 							f24f32(kd_triangle[index].vertex[2].tex_coord[0]) * f24f32(kd_triangle[index].vertex[1].tex_coord[1]) +
// 							f24f32(kd_triangle[index].vertex[0].tex_coord[0]) * f24f32(kd_triangle[index].vertex[2].tex_coord[1]))); //// bound include.
// 				if(Pta<0)
// 					Pta=-Pta;
// 				Tn = Pta*texture[texture_id[kd_triangle[index].material_index].texture_num].width*texture[texture_id[kd_triangle[index].material_index].texture_num].height;
// 
// 				Ta = Pa/Tn;
// 				Res = f24f32(g_reg.control.pixel_size.x)*f24f32(g_reg.control.pixel_size.y);
// 				Tb=sqrt(Ta/Res);
//             
// 				kd_triangle[index].BaseLod=f32f24((float)(1.0/Tb));
// 				//kd_triangle[index].BaseLod = fmul(kd_triangle[index].BaseLod, f32f24(1.8661f));//dk baseLod round down 2^0.9
// 				if(Pta==0){
// 					kd_triangle[index].BaseLod=0x3f0000;
// 				}
// 
// 				if(g_reg.option.set.normal_map && g_reg.material[kd_triangle[index].material_index].normal.enable){// preprocessing - TBN matrix
// 					RayFloat Uvector[2];
// 					RayFloat Vvector[2];
// 					RayFloat Pvector[2][3];
// 					RayFloat tbn_invdeno;
// 					RayFloat t_normal[3], t_tangent[3], t_binormal[3];
// 					RayFloat test_normal;
// 					RayFloat normal_temp;
// 					RayFloat newNormal[3];
// 					RayFloat newTriNormal[3];
// 					//RayFloat normalLength;
// 
// 					Uvector[0] = fadder(kd_triangle[index].vertex[1].tex_coord[0],	kd_triangle[index].vertex[0].tex_coord[0], 1);
// 					Uvector[1] = fadder(kd_triangle[index].vertex[2].tex_coord[0],	kd_triangle[index].vertex[0].tex_coord[0], 1);
// 					Vvector[0] = fadder(kd_triangle[index].vertex[1].tex_coord[1],	kd_triangle[index].vertex[0].tex_coord[1], 1);
// 					Vvector[1] = fadder(kd_triangle[index].vertex[2].tex_coord[1],	kd_triangle[index].vertex[0].tex_coord[1], 1);
// 					for (i = 0; i <3 ; i++){
// 						Pvector[0][i] = fadder(kd_triangle[index].vertex[1].position[i], kd_triangle[index].vertex[0].position[i], 1);
// 						Pvector[1][i] = fadder(kd_triangle[index].vertex[2].position[i], kd_triangle[index].vertex[0].position[i], 1);
// 					}
// 					tbn_invdeno = fdivider(f32f24(1.0f), (fadder(fmul(Uvector[0], Vvector[1]), fmul(Vvector[0], Uvector[1]), 1)));
// 					for(i = 0 ; i <3 ; i++) {
// 						t_tangent[i] = fmul(tbn_invdeno, (fadder(fmul(Vvector[1], Pvector[0][i]), fmul(Vvector[0], Pvector[1][i]), 1)));
// 						t_binormal[i] = fmul(tbn_invdeno, (fadder(fmul(Uvector[0], Pvector[1][i]), fmul(Uvector[1], Pvector[0][i]), 1)));
// 					}
// 					normal_temp=TFAdder(fmul(t_tangent[0],t_tangent[0]),fmul(t_tangent[1],t_tangent[1]),fmul(t_tangent[2],t_tangent[2]));
// 					normal_temp=SQRT(normal_temp);
// 					t_tangent[0]=fmul(t_tangent[0],fdivider(f32f24(1.0f),normal_temp));
// 					t_tangent[1]=fmul(t_tangent[1],fdivider(f32f24(1.0f),normal_temp));
// 					t_tangent[2]=fmul(t_tangent[2],fdivider(f32f24(1.0f),normal_temp));//dk normalize(tangent of TBN)
// 					normal_temp=TFAdder(fmul(t_binormal[0],t_binormal[0]),fmul(t_binormal[1],t_binormal[1]),fmul(t_binormal[2],t_binormal[2]));
// 					normal_temp=SQRT(normal_temp);
// 					t_binormal[0]=fmul(t_binormal[0],fdivider(f32f24(1.0f),normal_temp));
// 					t_binormal[1]=fmul(t_binormal[1],fdivider(f32f24(1.0f),normal_temp));
// 					t_binormal[2]=fmul(t_binormal[2],fdivider(f32f24(1.0f),normal_temp));//dk normalize(binormal of TBN)
// 					t_normal[0] = fadder(fmul(t_tangent[1], t_binormal[2]), fmul(t_tangent[2], t_binormal[1]), 1);
// 					t_normal[1] = fadder(fmul(t_tangent[2], t_binormal[0]), fmul(t_tangent[0], t_binormal[2]), 1);
// 					t_normal[2] = fadder(fmul(t_tangent[0], t_binormal[1]), fmul(t_tangent[1], t_binormal[0]), 1);
// 
// 					/*normalLength = sqrt(TFAdder(fmul(t_normal[0], t_normal[0]), fmul(t_normal[1], t_normal[1]), fmul(t_normal[2], t_normal[2])));
// 					normalLength = sqrt(normalLength);
// 					t_normal[0] /= normalLength;
// 					t_normal[1] /= normalLength;
// 					t_normal[2] /= normalLength;*/
// 
// 					normal_temp=f32f24((float)((TriNormal[0]*TriNormal[0])+(TriNormal[1]*TriNormal[1])+(TriNormal[2]*TriNormal[2])));
// 					normal_temp=SQRT(normal_temp);
// 					newTriNormal[0]=fmul(f32f24((float)TriNormal[0]),fdivider(f32f24(1.0f),normal_temp));
// 					newTriNormal[1]=fmul(f32f24((float)TriNormal[1]),fdivider(f32f24(1.0f),normal_temp));
// 					newTriNormal[2]=fmul(f32f24((float)TriNormal[2]),fdivider(f32f24(1.0f),normal_temp));
// 					normal_temp=TFAdder(fmul(t_normal[0],t_normal[0]),fmul(t_normal[1],t_normal[1]),fmul(t_normal[2],t_normal[2]));
// 					normal_temp=SQRT(normal_temp);
// 					newNormal[0]=fmul(t_normal[0],fdivider(f32f24(1.0f),normal_temp));
// 					newNormal[1]=fmul(t_normal[1],fdivider(f32f24(1.0f),normal_temp));
// 					newNormal[2]=fmul(t_normal[2],fdivider(f32f24(1.0f),normal_temp));//dk normalize(normal of TBN)
// 					test_normal=TFAdder(fmul(newNormal[0],newTriNormal[0]),fmul(newNormal[1],newTriNormal[1]),fmul(newNormal[2],newTriNormal[2]));
// 					test_normal=SignConv(test_normal);
// 
// 					i = compare(test_normal, 0);
// 					if(i == 1) {
// 						t_normal[0] = SignConv(t_normal[0]);
// 						t_normal[1] = SignConv(t_normal[1]);
// 						t_normal[2] = SignConv(t_normal[2]);
// 						t_tangent[0] = SignConv(t_tangent[0]);
// 						t_tangent[1] = SignConv(t_tangent[1]);
// 						t_tangent[2] = SignConv(t_tangent[2]);
// 					}
// 					kd_triangle[index].vertex[0].normal[0] = t_tangent[0];
// 					kd_triangle[index].vertex[0].normal[1] = t_tangent[1];
// 					kd_triangle[index].vertex[0].normal[2] = t_tangent[2];
// 					kd_triangle[index].vertex[1].normal[0] = t_binormal[0];
// 					kd_triangle[index].vertex[1].normal[1] = t_binormal[1];
// 					kd_triangle[index].vertex[1].normal[2] = t_binormal[2];
// 					kd_triangle[index].vertex[2].normal[0] = t_normal[0];
// 					kd_triangle[index].vertex[2].normal[1] = t_normal[1];
// 					kd_triangle[index].vertex[2].normal[2] = t_normal[2];
// 				}
// 			}
// 			else
// 				kd_triangle[index].BaseLod=0x3f0000;
// 		}
 	}

	fclose(fp);

	return 1;
}
