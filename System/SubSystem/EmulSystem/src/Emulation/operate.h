#ifndef	OPERATE_H
#define OPERATE_H

#define low0 0
#define low1 1
#define low2 2
#define low3 3
#define low4 4
#define low5 5
#define low6 6
#define low7 7
#define low8 8
#define low9 9
#define low10 10
#define low11 11
#define low12 12
#define low13 13
#define low14 14
#define low15 15
#define low16 16
#define low17 17
#define low18 18
#define low19 19
#define low20 20
#define low21 21
#define low22 22
#define low23 23
#define low24 24
#define low25 25
#define low26 26
#define low27 27
#define low28 28
#define low29 29
#define low30 30
#define low31 31

#define Mask1  0x00000001
#define Mask2  0x00000003
#define Mask3  0x00000007
#define Mask4  0x0000000F
#define Mask5  0x0000001F
#define Mask6  0x0000003F
#define Mask7  0x0000007F
#define Mask8  0x000000FF
#define Mask9  0x000001FF
#define Mask10 0x000003FF
#define Mask11 0x000007FF
#define Mask12 0x00000FFF
#define Mask13 0x00001FFF
#define Mask14 0x00003FFF
#define Mask15 0x00007FFF
#define Mask16 0x0000FFFF
#define Mask17 0x0001FFFF
#define Mask18 0x0003FFFF
#define Mask19 0x0007FFFF
#define Mask20 0x000FFFFF
#define Mask21 0x001FFFFF
#define Mask22 0x003FFFFF
#define Mask23 0x007FFFFF
#define Mask24 0x00FFFFFF
#define Mask25 0x01FFFFFF
#define Mask26 0x03FFFFFF
#define Mask27 0x07FFFFFF
#define Mask28 0x0FFFFFFF
#define Mask29 0x1FFFFFFF
#define Mask30 0x3FFFFFFF
#define Mask31 0x7FFFFFFF

#define OneMask0  0x00000001
#define OneMask1  0x00000002
#define OneMask2  0x00000004
#define OneMask3  0x00000008
#define OneMask4  0x00000010
#define OneMask5  0x00000020
#define OneMask6  0x00000040
#define OneMask7  0x00000080
#define OneMask8  0x00000100
#define OneMask9  0x00000200
#define OneMask10 0x00000400
#define OneMask11 0x00000800
#define OneMask12 0x00001000
#define OneMask13 0x00002000
#define OneMask14 0x00004000
#define OneMask15 0x00008000

typedef struct _DataMargin2{
	int In0;
	int In1;
	int Out;
}_DataMargin2;

typedef struct _DataMargin3{
	int In0;
	int In1;
	int In2;
	int Out;
}_DataMargin3;

unsigned int fadder(unsigned int x, unsigned int y, unsigned int mode);
unsigned int TFAdder(int x, int y, int z);

unsigned int MUL_20x20 (unsigned int cand, unsigned int ier);
unsigned int MUL_17x13 (unsigned int cand, unsigned int ier);
unsigned int MUL_17x10 (unsigned int cand, unsigned int ier);
unsigned int BIT_INV_20B (unsigned int ain);
unsigned int DLUT(unsigned int addr);
unsigned int MAN_UNIT(unsigned int x, unsigned int y);
unsigned int fdivider(unsigned int x, unsigned int y);

unsigned int fmul(unsigned int x, unsigned int y);

unsigned int mul_add (unsigned int in1, unsigned int in2);
unsigned int MUX (unsigned int in1, unsigned int flag, unsigned f_x);
unsigned int squaring (unsigned int in);
unsigned int mul_comple_for_D (unsigned int e, unsigned int in1, unsigned int in2);
unsigned int mul_for_R (unsigned int in1, unsigned int in2);
unsigned int SQRLUT (unsigned int e, unsigned int index);
unsigned int exp_calc (unsigned int e, unsigned int f, unsigned int* a);
unsigned int invsqrt(unsigned int x);

int SQRT(int x);

unsigned int compare(unsigned int x, unsigned int y);

unsigned int float2fixed(int INFP);
int mul_fixed(int a, int b);
int MUX_log_table (int select);
int MUX_exp_table (int select);
int fexp(int a, int b);
int nmod (int INFP);

int CalculateLod(int x, int y);
unsigned int ACOS(unsigned int i);

/////Margin
unsigned int TFAdderMargin(int x, int y, int z, _DataMargin3 *DataMargin);
unsigned int fadderMargin(unsigned int x, unsigned int y, unsigned int mode, _DataMargin2 *DataMargin);
unsigned int fmulMargin(unsigned int x, unsigned int y, _DataMargin2 *DataMargin);

#endif //OPERATE_H