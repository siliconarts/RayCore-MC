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
#include <math.h>
#include "operate.h"
#include "rc_util.h"

extern FILE *adderfi;

//fadder
unsigned int fadder(unsigned int x, unsigned int y, unsigned int mode){
	int det_add_sub;
	int great_value, sign_l, exp_l, exp_s;
	int frac_l, frac_s, exp_diff, align_s, xor_l, adder0;
	int pre_frac, final_sign, shift_exp, frac_normal, exp_compute, frac_compute, out;
	int xfraction, yfraction;
	int xexp, yexp;

	det_add_sub = (x>>low23)^(y>>low23);
	det_add_sub = (det_add_sub&Mask1);
	det_add_sub = det_add_sub^mode;

	great_value = ((x&Mask23)<(y&Mask23))? 1 : 0;

	xexp = (x>>low16)&Mask7;
	yexp = (y>>low16)&Mask7;

	if(xexp==0) xfraction=0;
	else xfraction = (x&Mask16)|0x10000;
	if(yexp==0) yfraction=0;
	else yfraction = (y&Mask16)|0x10000;

	sign_l = (great_value) ? ((y>>low23)&Mask1)^mode : ((x>>low23)&Mask1);

	if(great_value){
		exp_l=yexp;
		exp_s=xexp;
		frac_l = yfraction;
		frac_s = xfraction;
	}
	else {
		exp_l=xexp;
		exp_s=yexp;
		frac_l = xfraction;
		frac_s = yfraction;
	}

	exp_diff = (exp_l-exp_s)&Mask7;

	//wire [17:0] align_s = frac_s >> exp_diff;
	if((exp_diff<20)&&(exp_diff>0))
		align_s = ((frac_s>>(exp_diff-1))&Mask18);
	else if(exp_diff==0)
		align_s = frac_s<<1;
	else align_s = 0;

	if(det_add_sub)
		xor_l=(~(frac_l<<1))&Mask19;
	else xor_l = (frac_l<<1)&Mask19;

	adder0 = (align_s+xor_l)&Mask19;

	if(det_add_sub)
		pre_frac = (~adder0)&Mask19;
	else pre_frac=adder0;

	final_sign = sign_l;

	if((pre_frac&(Mask19&(~Mask18))))
		shift_exp = 0;
	else if((pre_frac&(Mask18&(~Mask17))))
		shift_exp = 1;
	else if((pre_frac&(Mask17&(~Mask16))))
		shift_exp = 2;
	else if((pre_frac&(Mask16&(~Mask15))))
		shift_exp = 3;
	else if((pre_frac&(Mask15&(~Mask14))))
		shift_exp = 4;
	else if((pre_frac&(Mask14&(~Mask13))))
		shift_exp = 5;
	else if((pre_frac&(Mask13&(~Mask12))))
		shift_exp = 6;
	else if((pre_frac&(Mask12&(~Mask11))))
		shift_exp = 7;
	else if((pre_frac&(Mask11&(~Mask10))))
		shift_exp = 8;
	else if((pre_frac&(Mask10&(~Mask9))))
		shift_exp = 9;
	else if((pre_frac&(Mask9&(~Mask8))))
		shift_exp = 10;
	else if((pre_frac&(Mask8&(~Mask7))))
		shift_exp = 11;
	else if((pre_frac&(Mask7&(~Mask6))))
		shift_exp = 12;
	else if((pre_frac&(Mask6&(~Mask5))))
		shift_exp = 13;
	else if((pre_frac&(Mask5&(~Mask4))))
		shift_exp = 14;
	else if((pre_frac&(Mask4&(~Mask3))))
		shift_exp = 15;
	else if((pre_frac&(Mask3&(~Mask2))))
		shift_exp = 16;
	else if((pre_frac&(Mask2&(~Mask1))))
		shift_exp = 17;
	else shift_exp = 18;

	frac_normal = (pre_frac<<shift_exp)&Mask19;

	exp_compute = (exp_l - shift_exp + 1)&Mask7;

	//wire  [15:0]    Frac_Compute = frac_normal[16:1];
	frac_compute = (frac_normal>>low2)&Mask16;

	if(pre_frac==0)
		out=0;
	else out=(final_sign<<low23)|(exp_compute<<low16)|frac_compute;

	//fprintf(adderfi,"x = 24'h%06x;\n",x);
	//fprintf(adderfi,"y = 24'h%06x;\n",y);
	//fprintf(adderfi,"mode = 1'b%x;\n",mode);
	//fprintf(adderfi,"out = 24'h%06x;\n",out);
	//fprintf(adderfi,"#20\n");
	return out;
}

unsigned int MUL_20x20 (unsigned int cand, unsigned int ier){
    //input [19:0] cand, ier;
    //output [17:0] a;
    //wire [39:0] product;
	int ierH, ierL, a, productH, productL;

	ierL = ier&Mask10;
	ierH = (ier>>10)&Mask10;

    //assign #1 product = cand * ier;
	productL = (cand * ierL)>>low10;
	productH = (cand * ierH) + productL;

	//assign a = product[38:21];
	//a = (productH>>low11)&Mask18;
	a = (productH>>low10)&Mask19;

	return a;
}

unsigned int MUL_17x13 (unsigned int cand, unsigned int ier){
    //input [16:0] cand;
    //input [12:0] ier;
    //output [19:0] a;

    //wire [33:0] product;
    //wire [16:0] ext;
	int a, product;

    //assign ext = {ier,4'b0000};
    //assign #1 product = cand * ext;
	product = cand* ier;
    //assign a = product[32:13]; //[28:9];
	a = (product>>low9)&Mask20;

	return a;
}

unsigned int MUL_17x10 (unsigned int cand, unsigned int ier){	
	int a, yh, yh_yl, ext, yl, product;

	//assign yh = {cand[16:11], 11'b000_0000_0000};
	//assign yl = cand[10:0];
	//yh = ((cand>>low11)&Mask6)<<11;
	//yl = cand&Mask11;
	yh = ((cand>>low11)&Mask6)<<11;
	//yh = yh|0x400;
	//if(cand&0x400){
		yl = (~cand&Mask11)&Mask11;
		yh_yl = (yh+yl)&Mask17;
	//}
	/*else{
		yl = ((~cand)&Mask10);
		yh_yl = (yh+yl)&Mask17;
	}*/

	//assign yh_yl = yh - yl;
	//yh_yl = (yh-yl)&Mask17;

	//assign ext = {ier, 7'b000_0000};
	ext = ier; 

	//product = yh_yl * ext;
	product = yh_yl * ext; //[26:0]
	//assign a = product[32:20]; //[25: 13]
	a= (product>>low13)&Mask13;

	return a;
}

unsigned int BIT_INV_20B (unsigned int ain){
	int aout;

	//assign aout = 2 - ain;   
	//aout = (-ain)&Mask20;
	aout = (1-ain)&Mask20;

	return aout;
}

unsigned int DLUT(unsigned int addr){
	/*int LUTMEM[32]={0x200, 0x1e1, 0x1c5, 0x1ab,
		            0x194, 0x17e, 0x16b, 0x158,
					0x147, 0x137, 0x129, 0x11b,
					0x10e, 0x102, 0x0f7, 0x0ed,
					0x0e3, 0x0da, 0x0d1, 0x0c9,
					0x0c1, 0x0ba, 0x0b3, 0x0ad,
					0x0a7, 0x0a1, 0x09b, 0x096,
					0x091, 0x08c, 0x088, 0x084};*/
	/*int LUTMEM[32]={0x200, 0x1e1, 0x1c6, 0x1ac,
   					0x195, 0x17f, 0x16b, 0x159,
					0x148, 0x138, 0x129, 0x11c,
					0x10f, 0x103, 0x0f8, 0x0ed,
					0x0e4, 0x0da, 0x0d2, 0x0ca,
					0x0c2, 0x0bb, 0x0b4, 0x0ad,
					0x0a7, 0x0a1, 0x09c, 0x097,
					0x092, 0x08d, 0x088, 0x084};*/
	int LUTMEM[32]={0x1f0,0x1d3,0x1b8,0x1a0,0x18a,0x175,0x162,0x150,0x140,0x130,0x122,0x115,0x109,0xfd,0xf2,0xe8,0xdf,0xd6,0xce,0xc6,0xbe,0xb7,0xb1,0xaa,0xa4,0x9f,0x99,0x94,0x8f,0x8b,0x86,0x82};

	return LUTMEM[addr];				
}

unsigned int MAN_UNIT(unsigned int x, unsigned int y){
	int q, one_over_yh2,a,ax,ay,inv_ay, q_temp,q_temp1;
	int exp1, exp4, overflow, underflow;

    //LUT U3 (y[15:11], one_over_yh2);	
	one_over_yh2 = DLUT((y>>low11)&Mask5);

    //MUL_17x10 U4 ({1'b1,y[15:0]}, one_over_yh2, a);	
	a = MUL_17x10(((y&Mask16)|0x10000), one_over_yh2);

    //assign exp1 = {2'b0, x[22:16]} + 9'b0_0011_1111 - {2'b0, y[22:16]};
	exp1 = (((x>>low16)&Mask7)+63-((y>>low16)&Mask7))&Mask9;

    //assign overflow = (exp1[7])? 1 : 0;
	if(((exp1>>low7)&Mask1)) overflow = 1;
	else overflow = 0;

    //assign underflow = (exp1[8])? 1 : 0; 
	if(((exp1>>low8)&Mask1)) underflow = 1;
	else underflow = 0;

    //MUL_17x13 U5 ({1'b1,x[15:0]}, a, ax);	
	ax = MUL_17x13(((x&Mask16)|0x10000), a);
    //MUL_17x13 U6 ({1'b1,y[15:0]}, a, ay);
	ay = MUL_17x13(((y&Mask16)|0x10000), a);

    //BIT_INV_20B U7 (ay, inv_ay);
	inv_ay = BIT_INV_20B(ay);

    //MUL_20x20 U8 (ax, inv_ay, q_temp);	
	q_temp = MUL_20x20(ax, inv_ay);

	//assign q_temp1 = (q_temp[17])? q_temp[17:1] : q_temp[16:0];
	//assign exp4 = (q_temp[17])? exp1 : (exp1 - 1'b1);
	/*if(((q_temp>>low17)&Mask1)==1){
		q_temp1 = (q_temp>>1)&Mask17;
		exp4 = exp1;
	}
	else {
		q_temp1 = q_temp&Mask17;
		exp4 = (exp1-1)&Mask9;
	}*/

	/*if(((q_temp>>low18)&Mask1)==1){
		q_temp1 = ((q_temp>>2)&Mask17)+((q_temp>>1)&1);
	}
	else {
		q_temp1 = ((q_temp>>1)&Mask17)+(q_temp&1);
	}
	if(((q_temp1>>low17)&Mask1)==1){
		exp4 = exp1;
	}
	else {
		exp4 = (exp1-1)&Mask9;
	}*/

	if(((q_temp>>low18)&Mask1)==1){
		q_temp1 = ((q_temp>>2)&Mask17);//+((q_temp>>1)&1);
		exp4 = exp1;
	}
	else {
		q_temp1 = ((q_temp>>1)&Mask17);//+(q_temp&1);
		exp4 = (exp1-1)&Mask9;
	}

	//assign q = (|x == 0)? 23'b0 : ((underflow)? 23'b0 : ((overflow)? 23'b111_1111_1111_1111_1111_1111 : {exp4[6:0],q_temp1[15:0]}));

	if(x==0)
		q=0;
	else if(underflow)
		q=0;
	else if(overflow)
		q=0x7fffff;
	else
		q=((exp4&Mask7)<<low16)|(q_temp1&Mask16);

	return q;
}

unsigned int fdivider(unsigned int x, unsigned int y){
	int s,q1;

    //SIGN_UNIT U1 (x[23], y[23], clk, s);
	s= (x>>low23)^(y>>low23);

	//MAN_UNIT U3 (x[23:0], y[23:0], q1, clk);
	q1=MAN_UNIT((x&Mask23),(y&Mask23));

    //assign nan0 = (|y == 0)? 1'b1 : 1'b0;

	// q <= {s,q1};
	if(q1==0)
		s=0;
	if(y==0)
		q1=0x7fffff;

	return (s<<low23)|q1;
}
/*
unsigned int MUL_20x20 (unsigned int cand, unsigned int ier){
    //input [19:0] cand, ier;
    //output [17:0] a;

    //wire [39:0] product;
	int ierH, ierL, a, productH, productL;

	ierL = ier&Mask10;
	ierH = (ier>>10)&Mask10;

    //assign #1 product = cand * ier;
	productL = (cand * ierL)>>low10;
	productH = (cand * ierH) + productL;

	//assign a = product[38:21];
	a = (productH>>low11)&Mask18;

	return a;
}

unsigned int MUL_17x13 (unsigned int cand, unsigned int ier){
    //input [16:0] cand;
    //input [12:0] ier;
    //output [19:0] a;

    //wire [33:0] product;
    //wire [16:0] ext;
	int a, product;

    //assign ext = {ier,4'b0000};
    //assign #1 product = cand * ext;
	product = cand* ier;
    //assign a = product[32:13]; //[28:9];
	a = (product>>low9)&Mask20;

	return a;
}

unsigned int MUL_17x10 (unsigned int cand, unsigned int ier){	
    int a, yh, yh_yl, ext, yl, product;

    //assign yh = {cand[16:11], 11'b000_0000_0000};
	//assign yl = cand[10:0];
	yh = ((cand>>low11)&Mask6)<<11;
	yl = cand&Mask11;

	//assign yh_yl = yh - yl;
    yh_yl = (yh-yl)&Mask17;

    //assign ext = {ier, 7'b000_0000};
	ext = ier; 

    //product = yh_yl * ext;
	product = yh_yl * ext; //[26:0]
    //assign a = product[32:20]; //[25: 13]
	a= (product>>low13)&Mask13;

	return a;
}

unsigned int BIT_INV_20B (unsigned int ain){
	int aout;

	//assign aout = 2 - ain;   
	aout = (2-ain)&Mask20;

	return aout;
}

unsigned int DLUT(unsigned int addr){
	int LUTMEM[32]={0x200, 0x1e1, 0x1c5, 0x1ab,
		            0x194, 0x17e, 0x16b, 0x158,
					0x147, 0x137, 0x129, 0x11b,
					0x10e, 0x102, 0x0f7, 0x0ed,
					0x0e3, 0x0da, 0x0d1, 0x0c9,
					0x0c1, 0x0ba, 0x0b3, 0x0ad,
					0x0a7, 0x0a1, 0x09b, 0x096,
					0x091, 0x08c, 0x088, 0x084};

	return LUTMEM[addr];				
}

unsigned int MAN_UNIT(unsigned int x, unsigned int y){
	int q, one_over_yh2,a,ax,ay,inv_ay, q_temp,q_temp1;
	int exp1, exp4, overflow, underflow;

    //LUT U3 (y[15:11], one_over_yh2);	
	one_over_yh2 = DLUT((y>>low11)&Mask5);

    //MUL_17x10 U4 ({1'b1,y[15:0]}, one_over_yh2, a);	
	a = MUL_17x10(((y&Mask16)|0x10000), one_over_yh2);

    //assign exp1 = {2'b0, x[22:16]} + 9'b0_0011_1111 - {2'b0, y[22:16]};
	exp1 = (((x>>low16)&Mask7)+63-((y>>low16)&Mask7))&Mask9;

    //assign overflow = (exp1[7])? 1 : 0;
	if(((exp1>>low7)&Mask1)) overflow = 1;
	else overflow = 0;

    //assign underflow = (exp1[8])? 1 : 0; 
	if(((exp1>>low8)&Mask1)) underflow = 1;
	else underflow = 0;

    //MUL_17x13 U5 ({1'b1,x[15:0]}, a, ax);	
	ax = MUL_17x13(((x&Mask16)|0x10000), a);
    //MUL_17x13 U6 ({1'b1,y[15:0]}, a, ay);
	ay = MUL_17x13(((y&Mask16)|0x10000), a);

    //BIT_INV_20B U7 (ay, inv_ay);
	inv_ay = BIT_INV_20B(ay);

    //MUL_20x20 U8 (ax, inv_ay, q_temp);	
	q_temp = MUL_20x20(ax, inv_ay);

	//assign q_temp1 = (q_temp[17])? q_temp[17:1] : q_temp[16:0];
	//assign exp4 = (q_temp[17])? exp1 : (exp1 - 1'b1);
	if(((q_temp>>low17)&Mask1)==1){
		q_temp1 = (q_temp>>1)&Mask17;
		exp4 = exp1;
	}
	else {
		q_temp1 = q_temp&Mask17;
		exp4 = (exp1-1)&Mask9;
	}

	//assign q = (|x == 0)? 23'b0 : ((underflow)? 23'b0 : ((overflow)? 23'b111_1111_1111_1111_1111_1111 : {exp4[6:0],q_temp1[15:0]}));

	if(x==0)
		q=0;
	else if(underflow)
		q=0;
	else if(overflow)
		q=0x7fffff;
	else
		q=((exp4&Mask7)<<low16)|(q_temp1&Mask16);

	return q;
}

unsigned int fdivider(unsigned int x, unsigned int y){
	int s,q1;

    //SIGN_UNIT U1 (x[23], y[23], clk, s);
	s= (x>>low23)^(y>>low23);

	//MAN_UNIT U3 (x[23:0], y[23:0], q1, clk);
	q1=MAN_UNIT((x&Mask23),(y&Mask23));

    //assign nan0 = (|y == 0)? 1'b1 : 1'b0;

	// q <= {s,q1};

	return (s<<low23)|q1;
}
*/

//fmul
unsigned int fmul(unsigned int x, unsigned int y){
	int sign_x, sign_y, exp_x, exp_y, frac_x, frac_yH, frac_yL, sign_compute;
	int frac_multiplyL, frac_multiplyH, frac_compute;
	int exp_c, underflow, exp_compute, overflow, out,one_zero;

	//wire	sign_x = x[23];
	//wire  sign_y = y[23];
	//wire	[6:0]		exp_x = x[22:16];
	//wire	[6:0]		exp_y = y[22:16];
	//wire	[15:0]		frac_x = x[15:0];
	//wire	[15:0]		frac_y = y[15:0];
	sign_x = (x>>low23)&Mask1;
	exp_x  = (x>>low16)&Mask7;
	frac_x = x&Mask16;
	sign_y = (y>>low23)&Mask1;
	exp_y  = (y>>low16)&Mask7;
	frac_yH = (y>>low8)&Mask8;
	frac_yL = y&Mask8;
	if(exp_x==0||exp_y==0)one_zero=1;
	else one_zero=0;

	//wire	sign_compute = sign_x ^ sign_y;
	sign_compute =sign_x^sign_y;

	//wire	[33:0]	frac_multiply = {1'b1, frac_x} * {1'b1, frac_y};
	frac_multiplyL=((frac_x|0x10000)*frac_yL)>>low8;
	frac_multiplyH=(frac_x|0x10000)*(0x100|frac_yH)+frac_multiplyL;

	//wire	[15:0]		frac_compute = (frac_multiply[33])? frac_multiply[32:17] : frac_multiply[31:16];
	if(((frac_multiplyH>>low25)&Mask1))
		frac_compute = (frac_multiplyH>>low9)&Mask16;
	else frac_compute = (frac_multiplyH>>low8)&Mask16;

	//wire [7:0] exp_c = exp_x + exp_y;
	exp_c = ((exp_x+exp_y)&Mask8) + ((frac_multiplyH>>low25)&Mask1);

	//wire underflow = (exp_c[7] || exp_c[6])? 0 : 1;
	if((exp_c&(Mask8&(~Mask6))))
		underflow = 0;
	else underflow = 1;

	//wire	[7:0]		exp_compute =  exp_c - 8'b00111111 + frac_multiply[33];
	//exp_compute = exp_c-63 + ((frac_multiplyH>>low25)&Mask1);
   exp_compute = exp_c-63;

	//wire overflow = (exp_compute[7] == 1'b1)? 1 : 0;
	if(((exp_compute>>low7)&Mask1))
		overflow = 1;
	else overflow = 0;

	//assign out = (underflow)? 24'b0 : (overflow)? {sign_compute,23'b11111_11111_11111_11111_111} :{sign_compute, exp_compute[6:0], frac_compute[15:0]};
	if(one_zero)
		out=0;
	else if(underflow)
		out=0;
	else if(overflow)
		out=(sign_compute<<low23)|Mask23;
	else
		out=(sign_compute<<low23)|(exp_compute<<low16)|frac_compute;

   return out;
}

//inverse square root
unsigned int mul_add (unsigned int in1, unsigned int in2){ //ok
//input [13:0] in1;
//input [15:0] in2;
//output [15:0] out;
	unsigned int RD, RD_te, RD_te2;
	unsigned int output;
	//wire [29:0] RD = in1 * in2;
	RD= in1*in2;
	//wire [29:0] RD_te = RD >> 1;
	RD_te = RD>>1;
	//wire [15:0] RD_te2 = RD_te[29:14];
	RD_te2=(RD_te>>low14)&Mask16;
	//assign out = RD_te2 + in2;
	output = (RD_te2+in2)&Mask16;

	return output;
}

unsigned int MUX (unsigned int in1, unsigned int flag, unsigned f_x){ //ok
	//input [15:0] in1;
	//output [15:0] out;
	int temp;
	//wire [15:0] temp = (in1) - 16'h8000;
	temp = (in1 - 32768)&Mask16;

	//assign out = (flag==1)? f_x : temp<<1;
	if (flag) return f_x &Mask16;

	return (temp << 1)&Mask16;
}

unsigned int squaring (unsigned int in){ //ok
    //input [15:0] in;
	//output [24:0] out;
	unsigned int temp;
	//wire [31:0] temp = in * in;
	temp = in*in;
    //assign out = temp[31:7];

	return (temp>>low7)&Mask25;
}

unsigned int mul_comple_for_D (unsigned int e, unsigned int in1, unsigned int in2){
	//input [24:0] in1; 
	//input [15:0] in2;
	int temp = 0x10000 | in2;
	int temp2 = (e==0)?  (temp<<1)&Mask18 : temp;
	__int64 temp3 = (__int64)in1 * temp2;
	int temp4 = (int)(temp3>>27);
	if (temp4>=0x3FFF) temp4=0x03FFE;

	return (~temp4)&Mask14;
}

unsigned int mul_for_R (unsigned int in1, unsigned int in2){
//input [15:0] in1;
//input [13:0] in2;
//output [15:0] out;
	int mod_in2, temp;  

	//wire [15:0] mod_in2 = {1'b1, in2[13:9], ~in2[8], in2[8], ~in2[7], in2[7], ~in2[6], in2[6], ~in2[5], in2[5], ~in2[4], in2[4]}; 
	
	mod_in2 = 0x8000; //1'b1 [15]	

	mod_in2 |= (in2&0x3E00)<<1;                //in2[13:9] [14:10]
	//mod_in2 |= ((in2)&0x3E00 << 1)&0x7C00 ; //in2[13:9] [14:10]
	mod_in2 |= ((in2)&0x100)?0:0x200;         //~in2[8]   [9]
	mod_in2 |= ((in2)&0x100)?0x100:0;         //in2[8]    [8]
	mod_in2 |= ((in2)&0x80)?0:0x80;          //~in2[7]   [7]
	mod_in2 |= ((in2)&0x80)?0x40:0;          //in2[7]    [6]
	mod_in2 |= ((in2)&0x40)?0:0x20;          //~in2[6]   [5]
	mod_in2 |= ((in2)&0x40)?0x10:0;          //in2[6]    [4]
	mod_in2 |= ((in2)&0x20)?0:0x8;           //~in2[5]   [3]
	mod_in2 |= ((in2)&0x20)?0x4:0;           //in2[5]    [2]
	mod_in2 |= ((in2)&0x10)?0:0x2;           //~in2[4]   [1]
	mod_in2 |= ((in2)&0x10)?0x1:0;           //in2[4]    [0]
	
	//wire [31:0] temp = (in1 * mod_in2) << 1;
	temp= ((in1*mod_in2) <<1 );

	//assign out = temp[31:16];

	return (temp>>16)&Mask16;
}

unsigned int SQRLUT (unsigned int e, unsigned int index){
	//input e;
	//input [4:0] index;
	//output [15:0] out;
	int addr;
	int SQRLUTMEM[64] = {0xb0de, 0xa901, 0xa1b6, 0x9aed, 0x949a, 0x8eb2, 0x892c, 0x83ff, 
		                 0x7f23, 0x7a92, 0x7645, 0x7237, 0x6e63, 0x6ac4, 0x6757, 0x6418, 
						 0x6104, 0x5e17, 0x5b4f, 0x58aa, 0x5624, 0x53bd, 0x5171, 0x4f40, 
						 0x4d28, 0x4b27, 0x493c, 0x4765, 0x45a2, 0x43f1, 0x4251, 0x40c2, 
						 0xfa20, 0xef03, 0xe4b2, 0xdb19, 0xd228, 0xc9ce, 0xc1fe, 0xbaac, 
						 0xb3cd, 0xad57, 0xa742, 0xa186, 0x9c1c, 0x96fe, 0x9226, 0x8d8e, 
						 0x8933, 0x8510, 0x8122, 0x7d63, 0x79d3, 0x766c, 0x732e, 0x7014, 
						 0x6d1e, 0x6a48, 0x6792, 0x64f8, 0x627a, 0x6015, 0x5dc9, 0x5b94};
	addr = (e<<low5)+index;

	return SQRLUTMEM[addr];
}

unsigned int exp_calc (unsigned int e, unsigned int f, unsigned int* a){ //ok
	//input [6:0] e;
	//input [15:0] f;
	//output [6:0] exp_f2;
	int exp_f2, exp_temp, f_zero;

	//wire f_zero = (f ==16'b0)? 1 : 0;
	if(f==0) f_zero = 1;
	else f_zero = 0;

	//assign a = (e[0] && f_zero)? 1 : 0;
	*a = ((e&1) && f_zero)? 1: 0;

	//assign exp_temp = ([0]==1'b0)? ((9'h17D - {1'b0,e+64})>>1) : (f_zero)? ((9'h17E - {1'b0,e+64})>>1) : ((9'h17C - {1'b0,e+64}) >> 1);
	if((e&1)==0)
		exp_temp = ((0x17D-(e+64))>>1)&Mask9;
	else if(f_zero)
		exp_temp = ((0x17E-(e+64))>>1)&Mask9;
	else exp_temp = ((0x17C-(e+64))>>1)&Mask9;

	//assign exp_f1 = exp_temp-64;
	//assign exp_f2 = exp_f1[6:0];
	exp_f2 = (exp_temp-64)&Mask7;

	return exp_f2;
}

unsigned int invsqrt(unsigned int x){
	int y, sign, exp, fraction, out_table, e_lut, f_R, R;
	int out_sqr, out_D, out_fi, frac_final, exp_f,f_lut;
	unsigned int a;

	//wire sign = x[23];
	sign = (x>>low23)&Mask1;
	//wire [6:0] exp = x[22:16];
	exp = ((x&0x7F0000)>>16)&Mask7;
	//wire [15:0] fraction = x[15:0];
	fraction = x&Mask16;

	//wire [15:0] out_table;
	//wire e_lut = x[16];
	e_lut = (x>>low16)&Mask1;
	//wire [4:0] f_lut = x[15:11];
	f_lut = (x>>low11)&Mask5;
	//wire [13:0] f_R = x[15:2];
	f_R = (x>>low2)&Mask14;
	//wire [15:0] R;
	//wire [24:0] out_sqr;
	//wire [13:0] out_D;
	//wire [15:0] out_fi;
	//wire [15:0] frac_final;
	//wire [6:0] exp_f;

	//exp_calc U1 (exp, fraction, exp_f, a);
	exp_f = exp_calc(exp, fraction, &a);

	//SQRLUT U2 (e_lut, f_lut, out_table);
	out_table = SQRLUT(e_lut, f_lut);

	//mul_for_R U3 (out_table, f_R, R);
	R = mul_for_R (out_table, f_R);

	//squaring U4 (R, out_sqr);
	out_sqr = squaring (R);

	//mul_comple_for_D U5 (e_lut, out_sqr, fraction, out_D);
	out_D = mul_comple_for_D(e_lut, out_sqr, fraction);

	//mul_add U6 (out_D, R, out_fi);
	out_fi = mul_add(out_D, R);

	//MUX U7 (out_fi, a, fraction, frac_final);
	frac_final = MUX (out_fi, a, fraction);

	//assign y = {sign, exp_f, frac_final};
	y= (sign<<low23)|(exp_f<<low16)|frac_final;

	return y;
}

//square root
int SALUT[256] = {
				//n=2m+1
            0x6900, 0x6800, 0x6700, 0x6500, 0x6400, 0x6200, 0x6100, 0x6000, 0x5f00, 0x5d00, 0x5c00, 0x5b00, 0x5a00, 0x5800, 0x5700, 0x5600,
            0x5500, 0x5300, 0x5300, 0x5100, 0x5000, 0x4f00, 0x4e00, 0x4d00, 0x4c00, 0x4b00, 0x4a00, 0x4800, 0x4700, 0x4700, 0x4500, 0x4400,
            0x4300, 0x4200, 0x4100, 0x4100, 0x3f00, 0x3e00, 0x3d00, 0x3c00, 0x3c00, 0x3b00, 0x3900, 0x3900, 0x3800, 0x3700, 0x3600, 0x3500,
            0x3400, 0x3400, 0x3300, 0x3200, 0x3100, 0x3000, 0x2f00, 0x2f00, 0x2d00, 0x2d00, 0x2c00, 0x2b00, 0x2a00, 0x2a00, 0x2900, 0x2800,
            0x2700, 0x2600, 0x2600, 0x2500, 0x2400, 0x2400, 0x2300, 0x2200, 0x2200, 0x2000, 0x2000, 0x1f00, 0x1f00, 0x1d00, 0x1d00, 0x1d00,
            0x1c00, 0x1b00, 0x1b00, 0x1900, 0x1900, 0x1900, 0x1700, 0x1700, 0x1700, 0x1600, 0x1600, 0x1400, 0x1400, 0x1300, 0x1300, 0x1200,
            0x1100, 0x1100, 0x1000, 0x1000, 0x0f00, 0x0f00, 0x0e00, 0x0d00, 0x0d00, 0x0d00, 0x0b00, 0x0b00, 0x0a00, 0x0a00, 0x0900, 0x0900,
            0x0800, 0x0800, 0x0700, 0x0600, 0x0600, 0x0500, 0x0500, 0x0500, 0x0400, 0x0300, 0x0300, 0x0200, 0x0200, 0x0100, 0x0100, 0x0000, 
				//n=2m
				0xff00, 0xfd00, 0xfb00, 0xf900, 0xf700, 0xf500, 0xf400, 0xf200, 0xf000, 0xee00, 0xec00, 0xea00, 0xe900, 0xe700, 0xe500, 0xe300,
            0xe300, 0xe100, 0xe000, 0xdd00, 0xdb00, 0xda00, 0xd800, 0xd700, 0xd500, 0xd400, 0xd200, 0xd000, 0xcf00, 0xce00, 0xcc00, 0xcb00,
            0xca00, 0xc800, 0xc600, 0xc500, 0xc400, 0xc200, 0xc100, 0xc000, 0xbe00, 0xbd00, 0xbb00, 0xbb00, 0xba00, 0xb800, 0xb700, 0xb500,
            0xb400, 0xb300, 0xb200, 0xb000, 0xaf00, 0xae00, 0xad00, 0xac00, 0xaa00, 0xa900, 0xa800, 0xa700, 0xa600, 0xa500, 0xa400, 0xa200,
            0xa200, 0xa000, 0x9f00, 0x9e00, 0x9d00, 0x9c00, 0x9b00, 0x9a00, 0x9900, 0x9800, 0x9700, 0x9600, 0x9500, 0x9400, 0x9300, 0x9200,
            0x9100, 0x9000, 0x8f00, 0x8e00, 0x8d00, 0x8d00, 0x8b00, 0x8b00, 0x8a00, 0x8800, 0x8800, 0x8700, 0x8700, 0x8600, 0x8400, 0x8300,
            0x8300, 0x8200, 0x8100, 0x8000, 0x7f00, 0x7e00, 0x7e00, 0x7d00, 0x7b00, 0x7b00, 0x7a00, 0x7900, 0x7900, 0x7800, 0x7700, 0x7600,
            0x7600, 0x7500, 0x7400, 0x7300, 0x7200, 0x7200, 0x7100, 0x7000, 0x6f00, 0x6f00, 0x6e00, 0x6d00, 0x6d00, 0x6c00, 0x6b00, 0x6a00, 

};
int SYLUT[256] = {
				//n=2m+1
            0x6a0a, 0x6b73, 0x6cdb, 0x6e42, 0x6fa7, 0x710b, 0x726d, 0x73ce, 0x752e, 0x768d, 0x77ea, 0x7946, 0x7aa1, 0x7bfb, 0x7d53, 0x7eaa,
            0x8000, 0x8155, 0x82a8, 0x83fb, 0x854c, 0x869c, 0x87eb, 0x8939, 0x8a86, 0x8bd1, 0x8d1c, 0x8e66, 0x8fae, 0x90f5, 0x923c, 0x9381,
            0x94c6, 0x9609, 0x974b, 0x988c, 0x99cd, 0x9b0c, 0x9c4b, 0x9d88, 0x9ec4, 0xa000, 0xa13b, 0xa274, 0xa3ad, 0xa4e5, 0xa61c, 0xa752,
            0xa887, 0xa9bb, 0xaaef, 0xac21, 0xad53, 0xae84, 0xafb4, 0xb0e3, 0xb212, 0xb33f, 0xb46c, 0xb598, 0xb6c3, 0xb7ed, 0xb917, 0xba40,
            0xbb68, 0xbc8f, 0xbdb5, 0xbedb, 0xc000, 0xc124, 0xc248, 0xc36a, 0xc48c, 0xc5ae, 0xc6ce, 0xc7ee, 0xc90d, 0xca2c, 0xcb49, 0xcc66,
            0xcd83, 0xce9e, 0xcfb9, 0xd0d4, 0xd1ed, 0xd306, 0xd41f, 0xd536, 0xd64d, 0xd764, 0xd879, 0xd98f, 0xdaa3, 0xdbb7, 0xdcca, 0xdddd,
            0xdeef, 0xe000, 0xe111, 0xe221, 0xe330, 0xe43f, 0xe54e, 0xe65c, 0xe769, 0xe875, 0xe982, 0xea8d, 0xeb98, 0xeca2, 0xedac, 0xeeb5,
            0xefbe, 0xf0c6, 0xf1ce, 0xf2d5, 0xf3db, 0xf4e1, 0xf5e7, 0xf6eb, 0xf7f0, 0xf8f4, 0xf9f7, 0xfafa, 0xfbfc, 0xfcfe, 0xfdff, 0xff00,
            
            //n=2m
            0x0000, 0x0100, 0x01fe, 0x02fc, 0x03f8, 0x04f4, 0x05ee, 0x06e8, 0x07e1, 0x08d9, 0x09d0, 0x0ac6, 0x0bbb, 0x0cb0, 0x0da3, 0x0e96,
            0x0f87, 0x1078, 0x1168, 0x1258, 0x1346, 0x1434, 0x1521, 0x160d, 0x16f8, 0x17e3, 0x18cd, 0x19b6, 0x1a9e, 0x1b85, 0x1c6c, 0x1d52,
            0x1e37, 0x1f1c, 0x2000, 0x20e3, 0x21c6, 0x22a8, 0x2389, 0x2469, 0x2549, 0x2628, 0x2707, 0x27e4, 0x28c1, 0x299e, 0x2a7a, 0x2b55,
            0x2c30, 0x2d0a, 0x2de3, 0x2ebc, 0x2f94, 0x306c, 0x3143, 0x3219, 0x32ef, 0x33c4, 0x3499, 0x356d, 0x3640, 0x3713, 0x37e6, 0x38b8,
            0x3989, 0x3a5a, 0x3b2a, 0x3bfa, 0x3cc9, 0x3d97, 0x3e65, 0x3f33, 0x4000, 0x40cd, 0x4199, 0x4264, 0x432f, 0x43fa, 0x44c4, 0x458d,
            0x4656, 0x471f, 0x47e7, 0x48af, 0x4976, 0x4a3c, 0x4b03, 0x4bc8, 0x4c8e, 0x4d53, 0x4e17, 0x4edb, 0x4f9e, 0x5061, 0x5124, 0x51e6,
            0x52a8, 0x5369, 0x542a, 0x54eb, 0x55ab, 0x566a, 0x5729, 0x57e8, 0x58a7, 0x5965, 0x5a22, 0x5adf, 0x5b9c, 0x5c58, 0x5d14, 0x5dd0,
            0x5e8b, 0x5f46, 0x6000, 0x60ba, 0x6174, 0x622d, 0x62e6, 0x639e, 0x6456, 0x650e, 0x65c5, 0x667c, 0x6733, 0x67e9, 0x689f, 0x6955 
				
}; 

int SQRT(int x){
	int exponent,xfrH, xfrL;//,fraction;
	//float temp,temp1,temp2;
	//float atemp, ytemp;
	int y1,a;
	int aaddr;
	int flag;
	int temp3,temp4,temp5;
	int exp1, out;
   //int car;

	exponent = ((x&0x7f0000)>>16);
	exp1 = ((x&0x7f0000)>>16);
	xfrH = x&0xfe00;
	xfrL = x&0x1ff;
	aaddr=((x&0x1fe00)>>9);
	exponent = exponent-63;

	a=0x10000|SALUT[aaddr];  //1.16
	y1=0x10000|SYLUT[aaddr]; //1.16
	flag=exponent&1;
   temp5=(a*xfrL);
	if((exponent&1)==0){
		temp3=temp5>>18;
      //a/=4.;
		//y1=sqrt(ytemp);
	}
	else{
		temp3=temp5>>17;
	//	//a=1./(sqrt(2.*ytemp)); 
	//	a/=2.;
	//	//y1=sqrt(2.*ytemp);
	}

	temp4=y1+temp3;
	exponent = exponent>>1;
	exponent = exponent+63+(temp4>>17);
	if(exp1==0)
		out=0;
	else out = (exponent<<16)|(temp4&0xffff);

	return out;
}

//compare
unsigned int compare(unsigned int x, unsigned int y){
	float fx,fy;

	fx=f24f32(x);
	fy=f24f32(y);

	if(fx>fy)
		return 1;
	else if(fy>fx)
		return 2;
	else
		return 0;
}

//fexp
unsigned int float2fixed(int INFP){
	int OUTFX;
	int temp_fx;
	int temp;
	int L_shift;
	int R_shift;

	L_shift=((INFP&Mask22)>>low16)+1;
	R_shift=(~((INFP&Mask22)>>low16))&Mask5;
	if(((INFP&Mask23)>>low16)==0)//dk Mask22->Mask23, low17->low16
		temp=0;
	else temp=0x10000|(INFP&Mask16);
	if(INFP&0x400000)
		temp_fx=temp<<L_shift;
	else temp_fx = temp>>R_shift;
	OUTFX =((temp_fx&Mask24)>>low8);

	return OUTFX;
}
/*unsigned int float2fixed(int INFP){
	int OUTFX;
	int temp_fx;
	int temp;
	int L_shift;
	int R_shift;

	L_shift=((INFP&Mask22)>>low16)+1;
	R_shift=(~((INFP&Mask22)>>low16))&Mask5;
	if(((INFP&Mask22)>>low17)==0)
		temp=0;
	else temp=0x10000|(INFP&Mask16);
	if(INFP&0x400000)
		temp_fx=temp<<L_shift;
	else temp_fx = temp>>R_shift;
	OUTFX =((temp_fx&Mask24)>>low8);

	return OUTFX;
}*/

int mul_fixed(int a, int b){
	int out;
	out=a*b;

	return out;
}

int MUX_log_table (int select){
	int log_table[512] = {
		0x000,0x3fd,0x3fa,0x3f7,0x3f4,0x3f1,0x3ee,0x3eb,0x3e9,0x3e6,0x3e3,0x3e0,0x3dd,0x3da,0x3d8,0x3d5,
		0x3d2,0x3cf,0x3cc,0x3ca,0x3c7,0x3c4,0x3c1,0x3bf,0x3bc,0x3b9,0x3b6,0x3b4,0x3b1,0x3ae,0x3ab,0x3a9,
		0x3a6,0x3a3,0x3a1,0x39e,0x39b,0x398,0x396,0x393,0x390,0x38e,0x38b,0x388,0x386,0x383,0x380,0x37e,
		0x37b,0x378,0x376,0x373,0x371,0x36e,0x36b,0x369,0x366,0x364,0x361,0x35e,0x35c,0x359,0x357,0x354,
		0x351,0x34f,0x34c,0x34a,0x347,0x345,0x342,0x340,0x33d,0x33b,0x338,0x336,0x333,0x331,0x32e,0x32c,
		0x329,0x327,0x324,0x322,0x31f,0x31d,0x31a,0x318,0x315,0x313,0x310,0x30e,0x30b,0x309,0x306,0x304,
		0x302,0x2ff,0x2fd,0x2fa,0x2f8,0x2f6,0x2f3,0x2f1,0x2ee,0x2ec,0x2ea,0x2e7,0x2e5,0x2e2,0x2e0,0x2de,
		0x2db,0x2d9,0x2d7,0x2d4,0x2d2,0x2cf,0x2cd,0x2cb,0x2c8,0x2c6,0x2c4,0x2c1,0x2bf,0x2bd,0x2ba,0x2b8,
		0x2b6,0x2b4,0x2b1,0x2af,0x2ad,0x2aa,0x2a8,0x2a6,0x2a3,0x2a1,0x29f,0x29d,0x29a,0x298,0x296,0x294,
		0x291,0x28f,0x28d,0x28b,0x288,0x286,0x284,0x282,0x27f,0x27d,0x27b,0x279,0x277,0x274,0x272,0x270,
		0x26e,0x26c,0x269,0x267,0x265,0x263,0x261,0x25e,0x25c,0x25a,0x258,0x256,0x254,0x251,0x24f,0x24d,
		0x24b,0x249,0x247,0x245,0x242,0x240,0x23e,0x23c,0x23a,0x238,0x236,0x234,0x231,0x22f,0x22d,0x22b,
		0x229,0x227,0x225,0x223,0x221,0x21f,0x21d,0x21a,0x218,0x216,0x214,0x212,0x210,0x20e,0x20c,0x20a,
		0x208,0x206,0x204,0x202,0x200,0x1fe,0x1fc,0x1fa,0x1f8,0x1f5,0x1f3,0x1f1,0x1ef,0x1ed,0x1eb,0x1e9,
		0x1e7,0x1e5,0x1e3,0x1e1,0x1df,0x1dd,0x1db,0x1d9,0x1d7,0x1d5,0x1d3,0x1d1,0x1cf,0x1ce,0x1cc,0x1ca,
		0x1c8,0x1c6,0x1c4,0x1c2,0x1c0,0x1be,0x1bc,0x1ba,0x1b8,0x1b6,0x1b4,0x1b2,0x1b0,0x1ae,0x1ac,0x1aa,
		0x1a8,0x1a7,0x1a5,0x1a3,0x1a1,0x19f,0x19d,0x19b,0x199,0x197,0x195,0x193,0x192,0x190,0x18e,0x18c,
		0x18a,0x188,0x186,0x184,0x183,0x181,0x17f,0x17d,0x17b,0x179,0x177,0x175,0x174,0x172,0x170,0x16e,
		0x16c,0x16a,0x169,0x167,0x165,0x163,0x161,0x15f,0x15d,0x15c,0x15a,0x158,0x156,0x154,0x153,0x151,
		0x14f,0x14d,0x14b,0x14a,0x148,0x146,0x144,0x142,0x141,0x13f,0x13d,0x13b,0x139,0x138,0x136,0x134,
		0x132,0x130,0x12f,0x12d,0x12b,0x129,0x128,0x126,0x124,0x122,0x121,0x11f,0x11d,0x11b,0x11a,0x118,
		0x116,0x114,0x113,0x111,0x10f,0x10d,0x10c,0x10a,0x108,0x107,0x105,0x103,0x101,0x100,0x0fe,0x0fc,
		0x0fa,0x0f9,0x0f7,0x0f5,0x0f4,0x0f2,0x0f0,0x0ef,0x0ed,0x0eb,0x0e9,0x0e8,0x0e6,0x0e4,0x0e3,0x0e1,
		0x0df,0x0de,0x0dc,0x0da,0x0d9,0x0d7,0x0d5,0x0d4,0x0d2,0x0d0,0x0cf,0x0cd,0x0cb,0x0ca,0x0c8,0x0c6,
		0x0c5,0x0c3,0x0c1,0x0c0,0x0be,0x0bd,0x0bb,0x0b9,0x0b8,0x0b6,0x0b4,0x0b3,0x0b1,0x0af,0x0ae,0x0ac,
		0x0ab,0x0a9,0x0a7,0x0a6,0x0a4,0x0a3,0x0a1,0x09f,0x09e,0x09c,0x09b,0x099,0x097,0x096,0x094,0x093,
		0x091,0x08f,0x08e,0x08c,0x08b,0x089,0x087,0x086,0x084,0x083,0x081,0x080,0x07e,0x07c,0x07b,0x079,
		0x078,0x076,0x075,0x073,0x071,0x070,0x06e,0x06d,0x06b,0x06a,0x068,0x067,0x065,0x063,0x062,0x060,
		0x05f,0x05d,0x05c,0x05a,0x059,0x057,0x056,0x054,0x053,0x051,0x050,0x04e,0x04c,0x04b,0x049,0x048,
		0x046,0x045,0x043,0x042,0x040,0x03f,0x03d,0x03c,0x03a,0x039,0x037,0x036,0x034,0x033,0x031,0x030,
		0x02e,0x02d,0x02b,0x02a,0x028,0x027,0x025,0x024,0x023,0x021,0x020,0x01e,0x01d,0x01b,0x01a,0x018,
		0x017,0x015,0x014,0x012,0x011,0x00f,0x00e,0x00d,0x00b,0x00a,0x008,0x007,0x005,0x004,0x002,0x001
	};
// 	int log_table[256] = {	0x00,0xfe,0xfd,0xfb,0xfa,0xf8,0xf7,0xf6,0xf4,0xf3,0xf1,0xf0,0xef,0xed,0xec,0xea,
// 							0xe9,0xe8,0xe6,0xe5,0xe4,0xe2,0xe1,0xe0,0xde,0xdd,0xdc,0xda,0xd9,0xd8,0xd7,0xd5,
// 							0xd4,0xd3,0xd1,0xd0,0xcf,0xce,0xcc,0xcb,0xca,0xc9,0xc7,0xc6,0xc5,0xc4,0xc2,0xc1,
// 							0xc0,0xbf,0xbe,0xbc,0xbb,0xba,0xb9,0xb8,0xb6,0xb5,0xb4,0xb3,0xb2,0xb1,0xaf,0xae,
// 							0xad,0xac,0xab,0xaa,0xa8,0xa7,0xa6,0xa5,0xa4,0xa3,0xa2,0xa1,0x9f,0x9e,0x9d,0x9c,
// 							0x9b,0x9a,0x99,0x98,0x97,0x96,0x95,0x93,0x92,0x91,0x90,0x8f,0x8e,0x8d,0x8c,0x8b,
// 							0x8a,0x89,0x88,0x87,0x86,0x85,0x84,0x83,0x82,0x81,0x80,0x7f,0x7e,0x7c,0x7b,0x7a,
// 							0x79,0x78,0x77,0x76,0x75,0x74,0x73,0x73,0x72,0x71,0x70,0x6f,0x6e,0x6d,0x6c,0x6b,
// 							0x6a,0x69,0x68,0x67,0x66,0x65,0x64,0x63,0x62,0x61,0x60,0x5f,0x5e,0x5d,0x5d,0x5c,
// 							0x5b,0x5a,0x59,0x58,0x57,0x56,0x55,0x54,0x53,0x52,0x52,0x51,0x50,0x4f,0x4e,0x4d,
// 							0x4c,0x4b,0x4a,0x4a,0x49,0x48,0x47,0x46,0x45,0x44,0x43,0x43,0x42,0x41,0x40,0x3f,
// 							0x3e,0x3d,0x3d,0x3c,0x3b,0x3a,0x39,0x38,0x37,0x37,0x36,0x35,0x34,0x33,0x32,0x32,
// 							0x31,0x30,0x2f,0x2e,0x2e,0x2d,0x2c,0x2b,0x2a,0x29,0x29,0x28,0x27,0x26,0x25,0x25,
// 							0x24,0x23,0x22,0x21,0x21,0x20,0x1f,0x1e,0x1e,0x1d,0x1c,0x1b,0x1a,0x1a,0x19,0x18,
// 							0x17,0x17,0x16,0x15,0x14,0x14,0x13,0x12,0x11,0x10,0x10,0x0f,0x0e,0x0d,0x0d,0x0c,
// 							0x0b,0x0a,0x0a,0x09,0x08,0x08,0x07,0x06,0x05,0x05,0x04,0x03,0x02,0x02,0x01,0x00};
// 	float temp;
// 	temp=1.+(float)select/512.;
// 	return (int)((1.-log((float)temp)/log(2.))*1024);
	/*float temp;
	FILE *tempfile;
	int log_table[512] = {};
	int i;
	tempfile =fopen("temp.txt","w");
	for(i=0;i<512;i++){
		temp=1.+(float)i/512.;
		log_table[i]=(int)((1.-log((float)temp)/log(2.))*1024);
	}
	for(i=0;i<512;i++)//dk hw
		fprintf(tempfile,"   9'h%03x : out = 10'h%03x;\n",i,log_table[i]&0x3ff);
	for(i=0;i<512;i++){//dk sw
		fprintf(tempfile,"0x%03x,",log_table[i]&0x3ff);
		if(((i+1)&0xf)==0)
			fprintf(tempfile,"\n");
	}
	fclose(tempfile);*/ //dk
	/*float temp;
	FILE *tempfile;
	tempfile =fopen("temp.txt","w");
	for(i=0;i<256;i++){
		temp=1.+(float)i/256.;
		log_table[i]=(int)((1.-log((float)temp)/log(2.))*256);
	}
	for(i=0;i<256;i++)//dk hw
		fprintf(tempfile,"   8'h%02x : out = 8'h%02x;\n",i,log_table[i]&0xff);
	for(i=0;i<256;i++){//dk sw
		fprintf(tempfile,"0x%02x,",log_table[i]&0xff);
		if(((i+1)&0xf)==0)
			fprintf(tempfile,"\n");
	}
	fclose(tempfile);*/

	return log_table[select];
}

int MUX_exp_table (int select){
	int exp_table[256] = {	0x0000,0xfe9d,0xfd3c,0xfbdb,0xfa7c,0xf91d,0xf7bf,0xf663,0xf507,0xf3ac,0xf252,0xf0f9,0xefa1,0xee4a,0xecf4,0xeb9f,
							0xea4a,0xe8f7,0xe7a5,0xe653,0xe502,0xe3b3,0xe264,0xe116,0xdfc9,0xde7d,0xdd32,0xdbe7,0xda9e,0xd955,0xd80e,0xd6c7,
							0xd581,0xd43c,0xd2f8,0xd1b5,0xd072,0xcf31,0xcdf0,0xccb0,0xcb72,0xca34,0xc8f6,0xc7ba,0xc67f,0xc544,0xc40a,0xc2d1,
							0xc199,0xc062,0xbf2c,0xbdf6,0xbcc1,0xbb8e,0xba5b,0xb928,0xb7f7,0xb6c6,0xb597,0xb468,0xb33a,0xb20c,0xb0e0,0xafb4,
							0xae89,0xad5f,0xac36,0xab0e,0xa9e6,0xa8bf,0xa799,0xa674,0xa550,0xa42c,0xa309,0xa1e7,0xa0c6,0x9fa5,0x9e86,0x9d67,
							0x9c49,0x9b2b,0x9a0f,0x98f3,0x97d8,0x96bd,0x95a4,0x948b,0x9373,0x925c,0x9145,0x902f,0x8f1a,0x8e06,0x8cf3,0x8be0,
							0x8ace,0x89bd,0x88ac,0x879c,0x868d,0x857f,0x8471,0x8364,0x8258,0x814d,0x8042,0x7f38,0x7e2f,0x7d26,0x7c1e,0x7b17,
							0x7a11,0x790b,0x7806,0x7702,0x75fe,0x74fb,0x73f9,0x72f8,0x71f7,0x70f7,0x6ff7,0x6ef9,0x6dfb,0x6cfd,0x6c01,0x6b05,
							0x6a09,0x690f,0x6815,0x671c,0x6623,0x652b,0x6434,0x633d,0x6247,0x6152,0x605e,0x5f6a,0x5e76,0x5d84,0x5c92,0x5ba1,
							0x5ab0,0x59c0,0x58d1,0x57e2,0x56f4,0x5607,0x551a,0x542e,0x5342,0x5257,0x516d,0x5084,0x4f9b,0x4eb2,0x4dcb,0x4ce4,
							0x4bfd,0x4b17,0x4a32,0x494e,0x486a,0x4786,0x46a4,0x45c2,0x44e0,0x43ff,0x431f,0x423f,0x4160,0x4082,0x3fa4,0x3ec7,
							0x3dea,0x3d0e,0x3c32,0x3b57,0x3a7d,0x39a4,0x38ca,0x37f2,0x371a,0x3643,0x356c,0x3496,0x33c0,0x32eb,0x3217,0x3143,
							0x306f,0x2f9d,0x2eca,0x2df9,0x2d28,0x2c57,0x2b87,0x2ab8,0x29e9,0x291b,0x284d,0x2780,0x26b4,0x25e8,0x251c,0x2451,
							0x2387,0x22bd,0x21f4,0x212b,0x2063,0x1f9c,0x1ed5,0x1e0e,0x1d48,0x1c82,0x1bbe,0x1af9,0x1a35,0x1972,0x18af,0x17ed,
							0x172b,0x166a,0x15a9,0x14e9,0x1429,0x136a,0x12ab,0x11ed,0x1130,0x1073,0x0fb6,0x0efa,0x0e3e,0x0d83,0x0cc9,0x0c0f,
							0x0b55,0x0a9c,0x09e3,0x092b,0x0874,0x07bd,0x0706,0x0650,0x059b,0x04e5,0x0431,0x037d,0x02c9,0x0216,0x0163,0x00b1};
// 	int exp_table[512] = {};
// 	return (int)(pow(2.,1.-select/1024.)*256.*256.);
	/*FILE *tempfile;
	tempfile =fopen("temp.txt","w");
	for(i=0;i<256;i++){
		exp_table[(i&0xff)]=(int)(pow(2.,1.-i/256.)*256.*256.);
	}
	for(i=0;i<256;i++)
		fprintf(tempfile,"   8'h%02x : out = 16'h%04x;\n",i,exp_table[i]&0xffff);
	for(i=0;i<256;i++){
		fprintf(tempfile,"0x%04x,",exp_table[i]&0xffff);
		if(((i+1)&0xf)==0)
			fprintf(tempfile,"\n");
	}
	
	fclose(tempfile);*/
	
	return exp_table[select];
}
/*
int fexp(int x, int y){
	int xexp, xexp1, xfr,y1;
	int l,l1;
	int m,exp,fr;
	int out;

	xexp=(x>>16)&0x3f;
	//xfr=(x>>8)&0xff;
	xfr=(x>>7)&0x1ff;
	if(xfr==0)
		xexp1=(0x3f-xexp)&0x3f;
	else xexp1=(0x3e-xexp)&0x3f; //6.0
	y1 = float2fixed(y);  //8.8
	l  = MUX_log_table(xfr); //(1-log(1.xfr)), 0.8
	//l1=(xexp1<<8)|(l&0xff); //6.8
	l1=(xexp1<<9)|(l&0x1ff); //6.8
	m  = mul_fixed(y1,l1); //14.16; -n.a = -n-1+(1-0.a)

	fr = MUX_exp_table((m>>8)&0xff);//(2^(1-m))
	//fr = MUX_exp_table((m>>6)&0x3ff);

	if(((m>>8)&0xff)==0)
		exp = 63-(m>>16);
	else exp = 62-(m>>16); //-n-1
	if(exp<0)
		out=0;
	else if(xexp==0)
		out=0;
	else out=(exp<<16)|(fr&0xffff);

	return out;
}*///dk 20121212 specular exponent test

int fexp(int x, int y){
	int xexp, xexp1, xfr,y1;
	int l,l1;
	int m,exp,fr;
	int out;

	xexp=(x>>16)&0x3f;
	xfr=(x>>7)&0x1ff;
	if(xfr==0)//dk 0x000, 0x001 둘 다 0이기때문에 구별하기 위한 조건문
		xexp1=(0x3f-xexp)&0x3f;
	else xexp1=(0x3e-xexp)&0x3f; //6.0
	y1 = float2fixed(y);  //8.8
	l  = MUX_log_table(xfr); //(1-log(1.xfr)), 0.10
	l1=(xexp1<<10)|(l&0x3ff); //6.10
	m  = mul_fixed(y1,l1); //14.18; -n.a = -n-1+(1-0.a)

	fr = MUX_exp_table((m>>10)&0xff);//(2^(1-m))

	if(((m>>10)&0xff)==0)
		exp = 63-(m>>18);
	else exp = 62-(m>>18); //-n-1
	if(exp<0)
		out=0;
	else if(xexp==0)
		out=0;
	else out=(exp<<16)|(fr&0xffff);

	return out;
}

unsigned int TFAdder(int x, int y, int z){
   //int fX,fY,fZ;
   int sfX,sfY,sfZ;
   int xydiff,xzdiff,yzdiff;
   int f_l,f_s1,f_s2,exp_l,exp_s1,exp_s2;
   int exp_diff1, exp_diff2;
   int fr_n1, fr_n2;
   int Sign_s1, Sign_s2;
   int addition, final_sign;
   int pre_frac, shift_exp;
   int frac_normal, exp_compute, frac_compute, out;

   if((x&0x7F0000)==0)
      sfX = 0;
   else if((x&0x800000)==0)
      sfX = ((x&0xFFFF)|0x10000)*4;
   else {
      sfX = ((x&0xFFFF)|0x10000)*4;
      sfX=-sfX;
   }

   if((y&0x7F0000)==0)
      sfY = 0;
   else if((y&0x800000)==0)
      sfY = ((y&0xFFFF)|0x10000)*4;
   else {
      sfY = ((y&0xFFFF)|0x10000)*4;
      sfY=-sfY;
   }

   if((z&0x7F0000)==0)
      sfZ = 0;
   else if((z&0x800000)==0)
      sfZ = ((z&0xFFFF)|0x10000)*4;
   else {
      sfZ = ((z&0xFFFF)|0x10000)*4;
      sfZ=-sfZ;
   }

	xydiff = (x&0x7f0000)-(y&0x7f0000);
	xzdiff = (x&0x7f0000)-(z&0x7f0000);
	yzdiff = (y&0x7f0000)-(z&0x7f0000);

	if(xydiff>=0){
	   if(xzdiff>=0){
		  f_l=sfX;
		  exp_l=((x&0x7f0000)>>16);
		  f_s1=sfY;
		  exp_s1=((y&0x7f0000)>>16);
		  f_s2=sfZ;
		  exp_s2=((z&0x7f0000)>>16);
		  Sign_s1 = ((y>>23)&1);
		  Sign_s2 = ((z>>23)&1);
	   }
	   else {
		  f_l=sfZ;
		  exp_l=((z&0x7f0000)>>16);
		  f_s1=sfX;
		  exp_s1=((x&0x7f0000)>>16);
		  f_s2=sfY;
		  exp_s2=((y&0x7f0000)>>16);
		  Sign_s1 = ((x>>23)&1);
		  Sign_s2 = ((y>>23)&1);
	   }
	}
	else if(yzdiff>=0){
		  f_l=sfY;
		  exp_l=((y&0x7f0000)>>16);
		  f_s1=sfZ;
		  exp_s1=((z&0x7f0000)>>16);
		  f_s2=sfX;
		  exp_s2=((x&0x7f0000)>>16);
		  Sign_s1 = ((z>>23)&1);
		  Sign_s2 = ((x>>23)&1);
	}
	else {
		  f_l=sfZ;
		  exp_l=((z&0x7f0000)>>16);
		  f_s1=sfY;
		  exp_s1=((y&0x7f0000)>>16);
		  f_s2=sfX;
		  exp_s2=((x&0x7f0000)>>16);
		  Sign_s1 = ((y>>23)&1);
		  Sign_s2 = ((x>>23)&1);
	}

	exp_diff1 = exp_l-exp_s1;
	exp_diff2 = exp_l-exp_s2;

	if(exp_diff1<=19){
	   if(Sign_s1)
		  fr_n1 = -((-f_s1)>>exp_diff1);
	   else
		  fr_n1 = f_s1>>exp_diff1;
	}
	else 
		fr_n1 = 0;

	if(exp_diff2<=19)
	   if(Sign_s2)
		  fr_n2 = -((-f_s2)>>exp_diff2);
	   else
		  fr_n2 = f_s2>>exp_diff2;
	else 
		fr_n2 = 0;

	/*if(xydiff>=0){
	   if(xzdiff>=0){
		  f_l=sfX;
		  exp_l=((x&0x7f0000)>>16);
		  f_s1=sfY;
		  exp_s1=((y&0x7f0000)>>16);
		  f_s2=sfZ;
		  exp_s2=((z&0x7f0000)>>16);
	   }
	   else {
		  f_l=sfZ;
		  exp_l=((z&0x7f0000)>>16);
		  f_s1=sfX;
		  exp_s1=((x&0x7f0000)>>16);
		  f_s2=sfY;
		  exp_s2=((y&0x7f0000)>>16);
	   }
	}
	else if(yzdiff>=0){
		  f_l=sfY;
		  exp_l=((y&0x7f0000)>>16);
		  f_s1=sfZ;
		  exp_s1=((z&0x7f0000)>>16);
		  f_s2=sfX;
		  exp_s2=((x&0x7f0000)>>16);
	}
	else {
		  f_l=sfZ;
		  exp_l=((z&0x7f0000)>>16);
		  f_s1=sfY;
		  exp_s1=((y&0x7f0000)>>16);
		  f_s2=sfX;
		  exp_s2=((x&0x7f0000)>>16);
	}

	exp_diff1 = exp_l-exp_s1;
	exp_diff2 = exp_l-exp_s2;

	if(exp_diff1<=19)
	   fr_n1 = f_s1>>exp_diff1;
	else fr_n1 = 0;
	if(exp_diff2<=19)
	   fr_n2 = f_s2>>exp_diff2;
	else fr_n2 = 0;
	*/

	addition = f_l+fr_n1+fr_n2;
	if(addition<0)
	   final_sign = 1;
	else final_sign =0;

	if(addition<0)
	   pre_frac=(-addition);
	else pre_frac =addition;

	if((pre_frac&(Mask21&(~Mask20))))
		shift_exp = 0;
	else if((pre_frac&(Mask20&(~Mask19))))
		shift_exp = 1;
	else if((pre_frac&(Mask19&(~Mask18))))
		shift_exp = 2;
	else if((pre_frac&(Mask18&(~Mask17))))
		shift_exp = 3;
	else if((pre_frac&(Mask17&(~Mask16))))
		shift_exp = 4;
	else if((pre_frac&(Mask16&(~Mask15))))
		shift_exp = 5;
	else if((pre_frac&(Mask15&(~Mask14))))
		shift_exp = 6;
	else if((pre_frac&(Mask14&(~Mask13))))
		shift_exp = 7;
	else if((pre_frac&(Mask13&(~Mask12))))
		shift_exp = 8;
	else if((pre_frac&(Mask12&(~Mask11))))
		shift_exp = 9;
	else if((pre_frac&(Mask11&(~Mask10))))
		shift_exp = 10;
	else if((pre_frac&(Mask10&(~Mask9))))
		shift_exp = 11;
	else if((pre_frac&(Mask9&(~Mask8))))
		shift_exp = 12;
	else if((pre_frac&(Mask8&(~Mask7))))
		shift_exp = 13;
	else if((pre_frac&(Mask7&(~Mask6))))
		shift_exp = 14;
	else if((pre_frac&(Mask6&(~Mask5))))
		shift_exp = 15;
	else if((pre_frac&(Mask5&(~Mask4))))
		shift_exp = 16;
	else if((pre_frac&(Mask4&(~Mask3))))
		shift_exp = 17;
	else if((pre_frac&(Mask3&(~Mask2))))
		shift_exp = 18;
	else if((pre_frac&(Mask2&(~Mask1))))
		shift_exp = 19;
	else shift_exp = 20;

	frac_normal = (pre_frac<<shift_exp)&Mask20;

	exp_compute = (exp_l - shift_exp + 2)&Mask7;

	frac_compute = (frac_normal>>low4)&Mask16;

	if(pre_frac==0)
		out=0;
	else out=(final_sign<<low23)|(exp_compute<<low16)|frac_compute;

	/*{
		float fx, fy, fz, fout;
		int outtemp;
		fx= f24f32(x);
		fy= f24f32(y);
		fz= f24f32(z);
		fout = fx+fy;
		fout = fout+fz;
		outtemp=(int)f32f24(fout)-out;
		if(outtemp>30||outtemp<-30)
		printf("TfAdder error\n");
	}*/

	return out;
}

int nmod (int INFP){
	int OUTFX;
	int temp_fx;
	int temp;
	int L_shift;
	int R_shift;

	L_shift=((INFP&Mask22)>>low16)+1;
	R_shift=(~((INFP&Mask22)>>low16))&Mask5;
	if(((INFP&Mask23)>>low16)==0)
		temp=0;
	else temp=0x10000|(INFP&Mask16);

	if(INFP&0x400000)
		temp_fx=temp<<L_shift;
	else temp_fx = temp>>R_shift;

	if(INFP&0x800000)
      OUTFX = (-temp_fx)&0xffff;
   else
      OUTFX =temp_fx&0xffff;

	return OUTFX;
}

int CalculateLod(int x, int y){
	int ExpX, ExpY, FracX, FracYL, FracYH, FracMulYL, FracMulYH, Lod;

	ExpX = (x>>low16)&Mask7;
	ExpY = (y>>low16)&Mask7;
   
	FracX = (ExpX) ? (x&Mask16)+0x10000 : 0;
	FracYH = (ExpY) ? (y>>low8)&Mask8 + 0x100 : 0;
	FracYL = y&Mask8;
	
	FracMulYL = (FracX*FracYL)>>low8;
	FracMulYH = (FracX * FracYH)+FracMulYL;

	Lod = 0x7E-(ExpX+ExpY) + ((FracMulYH>>low25)&Mask1);

	return Lod;
}

int AcosATable[512]={0x045f6,0x0460b,0x04634,0x04671,0x046c3,0x0472a,0x047a6,0x04837,0x048de,0x0499b,0x04a6e,0x04b59,0x04c5b,0x04d75,0x04ea8,0x04ff4,
                     0x0515c,0x052de,0x0547d,0x0563a,0x05815,0x05a11,0x05c2e,0x05e6f,0x060d5,0x06362,0x06618,0x068fb,0x06c0b,0x06f4e,0x072c5,0x07674,
                     0x0795d,0x07b63,0x07d79,0x07fa1,0x081da,0x08427,0x08686,0x088fa,0x08b83,0x08e22,0x090d8,0x093a5,0x0968d,0x0998e,0x09cab,0x09fe5,
                     0x0a33e,0x0a6b6,0x0aa51,0x0ae0f,0x0b1f3,0x0b5ff,0x0ba35,0x0be98,0x0c32b,0x0c7f0,0x0cceb,0x0d220,0x0d793,0x0dd47,0x0e343,0x0e98a,
                     0x0ee73,0x0f1d6,0x0f54f,0x0f8e0,0x0fc8a,0x10026,0x10215,0x10412,0x1061e,0x10839,0x10a63,0x10c9f,0x10eec,0x1114b,0x113be,0x11645,
                     0x118e1,0x11b94,0x11e5f,0x12143,0x12441,0x1275c,0x12a94,0x12dec,0x13165,0x13503,0x138c6,0x13cb2,0x140ca,0x14510,0x14989,0x14e38,
                     0x151df,0x15463,0x156f7,0x1599c,0x15c52,0x15f1a,0x161f5,0x164e4,0x167e8,0x16b02,0x16e32,0x1717a,0x174db,0x17856,0x17bec,0x17f9f,
                     0x18371,0x18762,0x18b75,0x18fac,0x19409,0x1988e,0x19d3c,0x1a218,0x1a723,0x1ac61,0x1b1d6,0x1b784,0x1bd70,0x1c39e,0x1ca14,0x1d0d7,
                     0x1d61c,0x1d9bc,0x1dd73,0x1e142,0x1e52a,0x1e92c,0x1ed48,0x1f181,0x1f5d7,0x1fa4b,0x1fee0,0x201cb,0x20438,0x206b8,0x2094b,0x20bf2,
                     0x20eaf,0x21183,0x2146e,0x21773,0x21a93,0x21dce,0x22128,0x224a2,0x2283d,0x22bfd,0x22fe3,0x233f1,0x2382c,0x23c96,0x24132,0x24605,
                     0x249c7,0x24c5e,0x24f04,0x251bb,0x25484,0x2575e,0x25a4c,0x25d4e,0x26065,0x26392,0x266d5,0x26a30,0x26da4,0x27133,0x274dd,0x278a4,
                     0x27c89,0x2808f,0x284b7,0x28902,0x28d73,0x2920c,0x296d0,0x29bc1,0x2a0e1,0x2a635,0x2abbf,0x2b183,0x2b785,0x2bdca,0x2c456,0x2cb30,
                     0x2d086,0x2d432,0x2d7f5,0x2dbd0,0x2dfc3,0x2e3d1,0x2e7f9,0x2ec3e,0x2f0a0,0x2f521,0x2f9c2,0x2fe85,0x301b6,0x3043b,0x306d5,0x30982,
                     0x30c46,0x30f20,0x31212,0x3151d,0x31843,0x31b86,0x31ee6,0x32267,0x32609,0x329cf,0x32dbc,0x331d2,0x33614,0x33a84,0x33f28,0x34402,
                     0x347ca,0x34a63,0x34d0d,0x34fc8,0x35294,0x35573,0x35865,0x35b6b,0x35e85,0x361b5,0x364fc,0x3685b,0x36bd3,0x36f66,0x37313,0x376de,
                     0x37ac8,0x37ed1,0x382fc,0x3874c,0x38bc1,0x3905e,0x39525,0x39a1a,0x39f3e,0x3a496,0x3aa23,0x3afeb,0x3b5f1,0x3bc39,0x3c2c9,0x3c9a6,
                     0x3ceff,0x3d2ad,0x3d672,0x3da4e,0x3de43,0x3e252,0x3e67c,0x3eac3,0x3ef26,0x3f3a9,0x3f84b,0x3fd0f,0x400fc,0x40382,0x4061c,0x408ca,
                     0x40b8e,0x40e69,0x4115b,0x41467,0x4178d,0x41ad0,0x41e31,0x421b1,0x42554,0x4291a,0x42d07,0x4311c,0x4355e,0x439ce,0x43e71,0x4434a,
                     0x44712,0x449ab,0x44c54,0x44f0e,0x451da,0x454b8,0x457a9,0x45aae,0x45dc7,0x460f6,0x4643c,0x4679a,0x46b11,0x46ea1,0x4724e,0x47617,
                     0x479fe,0x47e06,0x4822f,0x4867c,0x48aee,0x48f89,0x4944d,0x4993f,0x49e60,0x4a3b3,0x4a93d,0x4af00,0x4b501,0x4bb45,0x4c1cf,0x4c8a5,
                     0x4cfcf,0x4d751,0x4df33,0x4e77f,0x4f03d,0x4f978,0x5019e,0x506cb,0x50c4c,0x51229,0x5186d,0x51f25,0x5265e,0x52e2b,0x5369f,0x53fd3,
                     0x549e7,0x554fe,0x5614a,0x56f04,0x57e7b,0x59015,0x5a45d,0x5bc15,0x5d855,0x5fac6,0x61308,0x62f63,0x656dd,0x69368,0x70171,0x7b29a};

int AcosYTable[512]={0x00000,0x145f6,0x24601,0x2e91a,0x34629,0x397da,0x3e9a5,0x41dc7,0x446ce,0x46fea,0x4991d,0x4c26b,0x4ebd6,0x50ab1,0x51f88,0x53472,
                     0x54972,0x55e87,0x573b5,0x588fd,0x59e61,0x5b3e2,0x5c983,0x5df46,0x5f52d,0x6059d,0x610b8,0x61be9,0x62731,0x63291,0x63e0c,0x649a2,
                     0x65555,0x65b3b,0x66128,0x6671e,0x66d1d,0x67324,0x67935,0x67f4f,0x68573,0x68ba1,0x691d9,0x6981d,0x69e6b,0x6a4c6,0x6ab2c,0x6b19e,
                     0x6b81e,0x6beab,0x6c546,0x6cbef,0x6d2a7,0x6d96f,0x6e047,0x6e730,0x6ee2a,0x6f537,0x6fc57,0x701c5,0x7056a,0x70919,0x70cd3,0x7109a,
                     0x7146d,0x7165b,0x7184d,0x71a42,0x71c3b,0x71e38,0x72038,0x7223c,0x72445,0x72651,0x72861,0x72a76,0x72c8f,0x72ead,0x730d0,0x732f7,
                     0x73524,0x73755,0x7398d,0x73bc9,0x73e0c,0x74054,0x742a3,0x744f8,0x74754,0x749b7,0x74c21,0x74e93,0x7510c,0x7538e,0x75618,0x758ab,
                     0x75b47,0x75c99,0x75ded,0x75f44,0x7609e,0x761fa,0x76359,0x764bb,0x76620,0x76788,0x768f3,0x76a61,0x76bd3,0x76d48,0x76ec0,0x7703c,
                     0x771bc,0x7733f,0x774c7,0x77652,0x777e2,0x77976,0x77b0e,0x77cac,0x77e4e,0x77ff5,0x781a1,0x78353,0x7850b,0x786c8,0x7888c,0x78a56,
                     0x78c27,0x78d12,0x78dff,0x78eed,0x78fde,0x790d0,0x791c5,0x792bc,0x793b4,0x794af,0x795ad,0x796ac,0x797ae,0x798b2,0x799b9,0x79ac2,
                     0x79bce,0x79cdd,0x79dee,0x79f03,0x7a01a,0x7a135,0x7a253,0x7a374,0x7a498,0x7a5c1,0x7a6ed,0x7a81d,0x7a950,0x7aa89,0x7abc5,0x7ad06,
                     0x7ae4d,0x7aef1,0x7af98,0x7b03f,0x7b0e8,0x7b192,0x7b23e,0x7b2eb,0x7b39a,0x7b44a,0x7b4fc,0x7b5af,0x7b664,0x7b71b,0x7b7d4,0x7b88e,
                     0x7b94b,0x7ba09,0x7bac9,0x7bb8c,0x7bc50,0x7bd17,0x7bde0,0x7beab,0x7bf79,0x7c04a,0x7c11d,0x7c1f3,0x7c2cb,0x7c3a7,0x7c486,0x7c568,
                     0x7c64e,0x7c6c2,0x7c737,0x7c7ad,0x7c824,0x7c89c,0x7c915,0x7c98f,0x7ca0a,0x7ca86,0x7cb04,0x7cb82,0x7cc02,0x7cc83,0x7cd05,0x7cd88,
                     0x7ce0d,0x7ce93,0x7cf1b,0x7cfa4,0x7d02e,0x7d0ba,0x7d148,0x7d1d8,0x7d269,0x7d2fc,0x7d391,0x7d428,0x7d4c1,0x7d55c,0x7d5f9,0x7d699,
                     0x7d73b,0x7d78d,0x7d7df,0x7d833,0x7d887,0x7d8db,0x7d931,0x7d987,0x7d9de,0x7da35,0x7da8e,0x7dae7,0x7db41,0x7db9c,0x7dbf8,0x7dc55,
                     0x7dcb3,0x7dd11,0x7dd71,0x7ddd2,0x7de34,0x7de97,0x7defb,0x7df60,0x7dfc7,0x7e02f,0x7e098,0x7e102,0x7e16e,0x7e1dc,0x7e24b,0x7e2bc,
                     0x7e32e,0x7e368,0x7e3a2,0x7e3dd,0x7e419,0x7e454,0x7e491,0x7e4ce,0x7e50b,0x7e549,0x7e587,0x7e5c6,0x7e606,0x7e646,0x7e687,0x7e6c9,
                     0x7e70b,0x7e74e,0x7e792,0x7e7d6,0x7e81b,0x7e861,0x7e8a8,0x7e8f0,0x7e938,0x7e981,0x7e9cc,0x7ea17,0x7ea63,0x7eab1,0x7eaff,0x7eb4f,
                     0x7eba0,0x7ebc9,0x7ebf2,0x7ec1c,0x7ec46,0x7ec70,0x7ec9b,0x7ecc6,0x7ecf1,0x7ed1d,0x7ed49,0x7ed75,0x7eda2,0x7edd0,0x7edfe,0x7ee2c,
                     0x7ee5b,0x7ee8a,0x7eeba,0x7eeea,0x7ef1b,0x7ef4d,0x7ef7f,0x7efb1,0x7efe5,0x7f018,0x7f04d,0x7f082,0x7f0b8,0x7f0ef,0x7f126,0x7f15f,
                     0x7f198,0x7f1d2,0x7f20d,0x7f249,0x7f286,0x7f2c4,0x7f304,0x7f344,0x7f386,0x7f3c9,0x7f40e,0x7f454,0x7f49c,0x7f4e6,0x7f532,0x7f580,
                     0x7f5d0,0x7f623,0x7f679,0x7f6d1,0x7f72e,0x7f78e,0x7f7f2,0x7f85c,0x7f8cc,0x7f943,0x7f9c3,0x7fa4e,0x7fae8,0x7fb97,0x7fc66,0x7fd74};

unsigned int ACOS(unsigned int i){
	unsigned int exp, fraction, sign;
	int diff_exp;
	int AddrH, AddrL, Addr;
	int FixData;
	int overflow;
	int Y0, Y0Exp, Y0Fraction;
	int A, AExp, AFraction;
	unsigned int X_X0Exp, X_X0Fraction;
	unsigned int Mul_Exp, Mul_Fraction;
	__int64 Mul;

	exp = (i>>16)&0x3f;
	sign = (i>>23)&1;
	fraction = (exp==0) ? 0 : 0x10000|(i&0xffff);
	diff_exp = 0x3f-exp;
	if(diff_exp<=0)
		overflow = 1;
	else 
		overflow = 0;
	FixData = fraction>>diff_exp;////////.xxxx_xxxx_xxxx_xxxx
	if(exp<0x3e){
		AddrH = 0;
		//AddrL = FixData>>10;
	}
	else if(exp>0x3e){
		AddrH = 16;
		//AddrL = 0x1f;
	}
	else{
		if((~fraction)&0x8000)
			AddrH = 1;
		else if((~fraction)&0x4000)
			AddrH = 2;
		else if((~fraction)&0x2000)
			AddrH = 3;
		else if((~fraction)&0x1000)
			AddrH = 4;
		else if((~fraction)&0x800)
			AddrH = 5;
		else if((~fraction)&0x400)
			AddrH = 6;
		else if((~fraction)&0x200)
			AddrH = 7;
		else if((~fraction)&0x100)
			AddrH = 8;
		else if((~fraction)&0x80)
			AddrH = 9;
		else //if((~fraction)&0x40)
			AddrH = 10;
		//else //if((~fraction)&0x20)
		//   AddrH = 11;
	}
	if(AddrH==10)
		AddrL = (FixData>>1)&0x1f;
	else
		AddrL = (FixData>>(10-AddrH))&0x1f;
	Addr = (AddrH<<5)|AddrL;
	Y0=AcosYTable[Addr];
	A=AcosATable[Addr];

	Y0Exp=(Y0>>16) ? (Y0>>16)+0x36 : 0;
	Y0Fraction = (Y0>>16) ? (Y0&0xffff)+0x10000 : 0;
	AExp = (A>>16);
	AFraction = (A&0xffff)+0x10000;

	//x>=x0
	if(Addr==0){
		X_X0Exp=exp;
		X_X0Fraction = fraction;
	}
	else if(AddrH==0){
		X_X0Exp=0x39;
		X_X0Fraction = (fraction<<(exp-0x39))&0xffff;
	}
	else if(AddrH==10){
		X_X0Exp=0x3e-14;
		X_X0Fraction = (fraction<<14)&0xffff;
	}
	else{
		X_X0Exp=0x3e-(AddrH+5);
		X_X0Fraction = (fraction<<(AddrH+5))&0xffff;
	}
	Mul_Exp = AExp+X_X0Exp-2;
	Mul = (__int64)X_X0Fraction * AFraction;
	Mul_Fraction = (unsigned int)(Mul>>16);

	if(Y0Exp!=0){
		diff_exp = Y0Exp-Mul_Exp;
		Mul_Exp= Y0Exp;
		Mul_Fraction = (Mul_Fraction>>diff_exp);
		Mul_Fraction = Mul_Fraction+Y0Fraction;
	}

	diff_exp = 0x3e-Mul_Exp;

	int align_s, xor_l, addr0, pre_frac;
	int Shift_exp, frac_normal, exp_compute;
	align_s = Mul_Fraction>>diff_exp;

	if(sign)
		xor_l = 0x10000;
	else
		xor_l = 0xffff;

	addr0 = xor_l+align_s;

	if(sign)
		pre_frac = addr0;
	else
		pre_frac = ~addr0;

	if(pre_frac&0x10000)
		Shift_exp=0;
	else if(pre_frac&0x8000)
		Shift_exp=1;
	else if(pre_frac&0x4000)
		Shift_exp=2;
	else if(pre_frac&0x2000)
		Shift_exp=3;
	else if(pre_frac&0x1000)
		Shift_exp=4;
	else if(pre_frac&0x800)
		Shift_exp=5;
	else if(pre_frac&0x400)
		Shift_exp=6;
	else if(pre_frac&0x200)
		Shift_exp=7;
	else if(pre_frac&0x100)
		Shift_exp=8;
	else if(pre_frac&0x80)
		Shift_exp=9;
	else if(pre_frac&0x40)
		Shift_exp=10;
	else if(pre_frac&0x20)
		Shift_exp=11;
	else if(pre_frac&0x10)
		Shift_exp=12;
	else if(pre_frac&0x8)
		Shift_exp=13;
	else if(pre_frac&0x4)
		Shift_exp=14;
	else if(pre_frac&0x2)
		Shift_exp=15;
	else
		Shift_exp=16;

	frac_normal = (pre_frac<<Shift_exp);

	exp_compute = (0x3e - Shift_exp);

	if(overflow)
		return (sign) ? 0x3f0000 : 0;
	else
		return (exp_compute<<16)|(frac_normal&0xffff);
}

//margin
unsigned int TFAdderMargin(int x, int y, int z, _DataMargin3 *DataMargin){
	//int fX,fY,fZ;
	int sfX,sfY,sfZ;
	int xydiff,xzdiff,yzdiff;
	int f_l,f_s1,f_s2,exp_l,exp_s1,exp_s2;
	int exp_diff1, exp_diff2;
	int fr_n1, fr_n2;
	int Sign_s1, Sign_s2;
	int addition, final_sign;
	int pre_frac, shift_exp;
	int frac_normal, exp_compute, frac_compute, out;

	if((x&0x7F0000)==0)
		sfX = 0;
	else if((x&0x800000)==0)
		sfX = ((x&0xFFFF)|0x10000)*4;
	else {
		sfX = ((x&0xFFFF)|0x10000)*4;
		sfX=-sfX;
	}

	if((y&0x7F0000)==0)
		sfY = 0;
	else if((y&0x800000)==0)
		sfY = ((y&0xFFFF)|0x10000)*4;
	else {
		sfY = ((y&0xFFFF)|0x10000)*4;
		sfY=-sfY;
	}

	if((z&0x7F0000)==0)
		sfZ = 0;
	else if((z&0x800000)==0)
		sfZ = ((z&0xFFFF)|0x10000)*4;
	else {
		sfZ = ((z&0xFFFF)|0x10000)*4;
		sfZ=-sfZ;
	}

	xydiff = (x&0x7f0000)-(y&0x7f0000);
	xzdiff = (x&0x7f0000)-(z&0x7f0000);
	yzdiff = (y&0x7f0000)-(z&0x7f0000);

	if(xydiff>=0){
		if(xzdiff>=0){
			f_l=sfX;
			exp_l=((x&0x7f0000)>>16);
			f_s1=sfY;
			exp_s1=((y&0x7f0000)>>16);
			f_s2=sfZ;
			exp_s2=((z&0x7f0000)>>16);
			Sign_s1 = ((y>>23)&1);
			Sign_s2 = ((z>>23)&1);
		}
		else {
			f_l=sfZ;
			exp_l=((z&0x7f0000)>>16);
			f_s1=sfX;
			exp_s1=((x&0x7f0000)>>16);
			f_s2=sfY;
			exp_s2=((y&0x7f0000)>>16);
			Sign_s1 = ((x>>23)&1);
			Sign_s2 = ((y>>23)&1);
		}
	}
	else if(yzdiff>=0){
		f_l=sfY;
		exp_l=((y&0x7f0000)>>16);
		f_s1=sfZ;
		exp_s1=((z&0x7f0000)>>16);
		f_s2=sfX;
		exp_s2=((x&0x7f0000)>>16);
		Sign_s1 = ((z>>23)&1);
		Sign_s2 = ((x>>23)&1);
	}
	else {
		f_l=sfZ;
		exp_l=((z&0x7f0000)>>16);
		f_s1=sfY;
		exp_s1=((y&0x7f0000)>>16);
		f_s2=sfX;
		exp_s2=((x&0x7f0000)>>16);
		Sign_s1 = ((y>>23)&1);
		Sign_s2 = ((x>>23)&1);
	}

	exp_diff1 = exp_l-exp_s1;
	exp_diff2 = exp_l-exp_s2;

	if(exp_diff1<=19){
		if(Sign_s1)
			fr_n1 = -((-f_s1)>>exp_diff1);
		else
			fr_n1 = f_s1>>exp_diff1;
	}
	else fr_n1 = 0;

	if(exp_diff2<=19)
		if(Sign_s2)
			fr_n2 = -((-f_s2)>>exp_diff2);
		else
			fr_n2 = f_s2>>exp_diff2;
	else fr_n2 = 0;

	addition = f_l+fr_n1+fr_n2;
	if(addition<0)
		final_sign = 1;
	else final_sign =0;

	if(addition<0)
		pre_frac=(-addition);
	else pre_frac =addition;

	if((pre_frac&(Mask21&(~Mask20))))
		shift_exp = 0;
	else if((pre_frac&(Mask20&(~Mask19))))
		shift_exp = 1;
	else if((pre_frac&(Mask19&(~Mask18))))
		shift_exp = 2;
	else if((pre_frac&(Mask18&(~Mask17))))
		shift_exp = 3;
	else if((pre_frac&(Mask17&(~Mask16))))
		shift_exp = 4;
	else if((pre_frac&(Mask16&(~Mask15))))
		shift_exp = 5;
	else if((pre_frac&(Mask15&(~Mask14))))
		shift_exp = 6;
	else if((pre_frac&(Mask14&(~Mask13))))
		shift_exp = 7;
	else if((pre_frac&(Mask13&(~Mask12))))
		shift_exp = 8;
	else if((pre_frac&(Mask12&(~Mask11))))
		shift_exp = 9;
	else if((pre_frac&(Mask11&(~Mask10))))
		shift_exp = 10;
	else if((pre_frac&(Mask10&(~Mask9))))
		shift_exp = 11;
	else if((pre_frac&(Mask9&(~Mask8))))
		shift_exp = 12;
	else if((pre_frac&(Mask8&(~Mask7))))
		shift_exp = 13;
	else if((pre_frac&(Mask7&(~Mask6))))
		shift_exp = 14;
	else if((pre_frac&(Mask6&(~Mask5))))
		shift_exp = 15;
	else if((pre_frac&(Mask5&(~Mask4))))
		shift_exp = 16;
	else if((pre_frac&(Mask4&(~Mask3))))
		shift_exp = 17;
	else if((pre_frac&(Mask3&(~Mask2))))
		shift_exp = 18;
	else if((pre_frac&(Mask2&(~Mask1))))
		shift_exp = 19;
	else shift_exp = 20;

	frac_normal = (pre_frac<<shift_exp)&Mask20;

	exp_compute = (exp_l - shift_exp + 2)&Mask7;

	frac_compute = (frac_normal>>low4)&Mask16;

	if(pre_frac==0)
		out=0;
	else 
		out=(final_sign<<low23)|(exp_compute<<low16)|frac_compute;
	{
		if(pre_frac==0)
			DataMargin->Out = -0x3f;
		else{
			if((DataMargin->In0 > DataMargin->In1)&&(DataMargin->In0 > DataMargin->In2))
				DataMargin->Out = DataMargin->In0;
			else if(DataMargin->In1 > DataMargin->In2)
				DataMargin->Out = DataMargin->In1;
			else
				DataMargin->Out = DataMargin->In2;
			if(shift_exp==0)
				DataMargin->Out+=2;
			else if(shift_exp==1)
				DataMargin->Out+=1;
		}
	}
//		{
//			int Mask,temp;
//			if(pre_frac==0)
//				DataMargin->Out = -0x3f;
//			else{
//				DataMargin->Out = exp_compute - 0x4f;
// 
//			if(DataMargin->Out<DataMargin->In0)
//				DataMargin->Out = DataMargin->In0;
//			if(DataMargin->Out<DataMargin->In1)
//				DataMargin->Out = DataMargin->In1;
//			if(DataMargin->Out<DataMargin->In2)
//				DataMargin->Out = DataMargin->In2;
// 
//			temp= DataMargin->Out+0x3f+16-exp_compute;  //after
//			Mask= ~((int)0x80000000>>(31-temp));
//			if(Mask){
//				if((frac_compute&Mask)==0)
//					DataMargin->Out++;            
//			}  
//		}
//	}

	return out;
}

unsigned int fadderMargin(unsigned int x, unsigned int y, unsigned int mode, _DataMargin2 *DataMargin){
	int det_add_sub;
	int great_value, sign_l, exp_l, exp_s;
	int frac_l, frac_s, exp_diff, align_s, xor_l, adder0;
	int pre_frac, final_sign, shift_exp, frac_normal, exp_compute, frac_compute, out;
	int xfraction, yfraction;
	int xexp, yexp;

	det_add_sub = (x>>low23)^(y>>low23);
	det_add_sub = (det_add_sub&Mask1);
	det_add_sub = det_add_sub^mode;

	great_value = ((x&Mask23)<(y&Mask23))? 1 : 0;

	xexp = (x>>low16)&Mask7;
	yexp = (y>>low16)&Mask7;

	if(xexp==0) xfraction=0;
	else xfraction = (x&Mask16)|0x10000;
	if(yexp==0) yfraction=0;
	else yfraction = (y&Mask16)|0x10000;

	sign_l = (great_value) ? ((y>>low23)&Mask1)^mode : ((x>>low23)&Mask1);

	if(great_value){
		exp_l=yexp;
		exp_s=xexp;
		frac_l = yfraction;
		frac_s = xfraction;
	}
	else {
		exp_l=xexp;
		exp_s=yexp;
		frac_l = xfraction;
		frac_s = yfraction;
	}

	exp_diff = (exp_l-exp_s)&Mask7;

	if((exp_diff<20)&&(exp_diff>0))
		align_s = ((frac_s>>(exp_diff-1))&Mask18);
	else if(exp_diff==0)
		align_s = frac_s<<1;
	else align_s = 0;

	if(det_add_sub)
		xor_l=(~(frac_l<<1))&Mask19;
	else xor_l = (frac_l<<1)&Mask19;

	adder0 = (align_s+xor_l)&Mask19;

	if(det_add_sub)
		pre_frac = (~adder0)&Mask19;
	else pre_frac=adder0;

	final_sign = sign_l;

	if((pre_frac&(Mask19&(~Mask18))))
		shift_exp = 0;
	else if((pre_frac&(Mask18&(~Mask17))))
		shift_exp = 1;
	else if((pre_frac&(Mask17&(~Mask16))))
		shift_exp = 2;
	else if((pre_frac&(Mask16&(~Mask15))))
		shift_exp = 3;
	else if((pre_frac&(Mask15&(~Mask14))))
		shift_exp = 4;
	else if((pre_frac&(Mask14&(~Mask13))))
		shift_exp = 5;
	else if((pre_frac&(Mask13&(~Mask12))))
		shift_exp = 6;
	else if((pre_frac&(Mask12&(~Mask11))))
		shift_exp = 7;
	else if((pre_frac&(Mask11&(~Mask10))))
		shift_exp = 8;
	else if((pre_frac&(Mask10&(~Mask9))))
		shift_exp = 9;
	else if((pre_frac&(Mask9&(~Mask8))))
		shift_exp = 10;
	else if((pre_frac&(Mask8&(~Mask7))))
		shift_exp = 11;
	else if((pre_frac&(Mask7&(~Mask6))))
		shift_exp = 12;
	else if((pre_frac&(Mask6&(~Mask5))))
		shift_exp = 13;
	else if((pre_frac&(Mask5&(~Mask4))))
		shift_exp = 14;
	else if((pre_frac&(Mask4&(~Mask3))))
		shift_exp = 15;
	else if((pre_frac&(Mask3&(~Mask2))))
		shift_exp = 16;
	else if((pre_frac&(Mask2&(~Mask1))))
		shift_exp = 17;
	else shift_exp = 18;

	frac_normal = (pre_frac<<shift_exp)&Mask19;

	exp_compute = (exp_l - shift_exp + 1)&Mask7;

	frac_compute = (frac_normal>>low2)&Mask16;

	if(pre_frac==0)
		out=0;
	else 
		out=(final_sign<<low23)|(exp_compute<<low16)|frac_compute;
	{
		if(pre_frac==0)
			DataMargin->Out = -0x3f;
		else{
			if(DataMargin->In0 > DataMargin->In1)
				DataMargin->Out = DataMargin->In0;
			else
				DataMargin->Out = DataMargin->In1;
			if(shift_exp==0)
				DataMargin->Out+=1;
		}
	}

//	{
//		int Mask,temp;
//		if(pre_frac==0)
//			DataMargin->Out = -0x3f;
//		else{
//			DataMargin->Out = exp_compute - 0x4f;
// 
//			if(DataMargin->Out<DataMargin->In0)
//				DataMargin->Out = DataMargin->In0;
//			if(DataMargin->Out<DataMargin->In1)
//				DataMargin->Out = DataMargin->In1;
//
//			temp= DataMargin->Out+0x4f-exp_compute;  //after
//			Mask= ~((int)0x80000000>>(31-temp));
//			if(Mask){
//				if((frac_compute&Mask)==0)
//					DataMargin->Out++;            
//			}  
//		}
//	}

	return out;
}

unsigned int fmulMargin(unsigned int x, unsigned int y, _DataMargin2 *DataMargin){
	int sign_x, sign_y, exp_x, exp_y, frac_x, frac_yH, frac_yL, sign_compute;
	int frac_multiplyL, frac_multiplyH, frac_compute;
	int exp_c, underflow, exp_compute, overflow, out,one_zero;

	sign_x = (x>>low23)&Mask1;
	exp_x  = (x>>low16)&Mask7;
	frac_x = x&Mask16;
	sign_y = (y>>low23)&Mask1;
	exp_y  = (y>>low16)&Mask7;
	frac_yH = (y>>low8)&Mask8;
	frac_yL = y&Mask8;
	if(exp_x==0||exp_y==0)one_zero=1;
	else one_zero=0;

	sign_compute =sign_x^sign_y;

	frac_multiplyL=((frac_x|0x10000)*frac_yL)>>low8;
	frac_multiplyH=(frac_x|0x10000)*(0x100|frac_yH)+frac_multiplyL;

	if(((frac_multiplyH>>low25)&Mask1))
		frac_compute = (frac_multiplyH>>low9)&Mask16;
	else frac_compute = (frac_multiplyH>>low8)&Mask16;

	exp_c = ((exp_x+exp_y)&Mask8) + ((frac_multiplyH>>low25)&Mask1);

	if((exp_c&(Mask8&(~Mask6))))
		underflow = 0;
	else underflow = 1;

	exp_compute = exp_c-63;

	if(((exp_compute>>low7)&Mask1))
		overflow = 1;
	else overflow = 0;

	if(one_zero)
		out=0;
	else if(underflow)
		out=0;
	else if(overflow)
		out=(sign_compute<<low23)|Mask23;
	else
		out=(sign_compute<<low23)|(exp_compute<<low16)|frac_compute;
// 	{
// 		if(pre_frac==0)
// 			DataMargin->Out = -0x3f;
// 		else{
// 			if(DataMargin->In0 > DataMargin->In1)
// 				DataMargin->Out = DataMargin->In0;
// 			else
// 				DataMargin->Out = DataMargin->In1;
// 			if(shift_exp==0)
// 				DataMargin->Out+=1;
// 		}
// 	}
	{
		int min;
		int xCount = (exp_x-0x3f-DataMargin->In0);
		int yCount = (exp_y-0x3f-DataMargin->In1);
		if(xCount>yCount)
			min = yCount;
		else 
			min = xCount;
		if(one_zero)
			DataMargin->Out = -0x3f - min;
		else{
			DataMargin->Out = exp_compute - 0x3f - min+((frac_multiplyH>>low25)&Mask1);//dktest
		}
	}
	// 	{
	// 		int min;
	// 		int xCount = (exp_x-0x3f-DataMargin->In0);
	// 		int yCount = (exp_y-0x3f-DataMargin->In1);
	// 		if(xCount>yCount)
	// 			min = yCount;
	// 		else min = xCount;
	//
	// 		if(one_zero)
	// 			DataMargin->Out = -0x3f - min;
	// 		else
	// 			DataMargin->Out = exp_compute - 0x3f - min;
	// 	}

	return out;
}