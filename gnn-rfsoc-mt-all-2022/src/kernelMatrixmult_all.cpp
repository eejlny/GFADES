/*===============================================================================
* This file is part of the gFADES GNN accelerator
* has been written at Linkoping University
* for the WASP project
* Author : Jose Nunez-Yanez
*
==============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <string>
#include <fstream>
#include <sstream> // ////std::stringstream

#include <hls_stream.h>
#include "ap_int.h"

#include "matrix_mult.h"

#include "hls_streamofblocks.h"

//typedef ITYPE buf[B_HEIGHT/4][C_WIDTH_BLOCK];

typedef ITYPE buf[B_HEIGHT/FEA_THREADS][C_WIDTH_BLOCK];

// note that  BLOCK shoudl be less than B_WIDTH_gmmBLOCK

//#if (B_WIDTH_BLOCK == 1)
//	const int BLOCK=2;
//#else
	//const int BLOCK=B_WIDTH_BLOCK;   //BLOCK should be less than B_WIDTH_BLOCK
//#endif

const int BLOCK=B_WIDTH_BLOCK;   //BLOCK should be less than B_WIDTH_BLOCK
const int SBLOCK=SPMM_BLOCK;   //BLOCK should be less than B_WIDTH_BLOCK

const int PARALLEL_ROW = B_BLOCK_PARALLEL;
const int A_WIDTH_FIFO =  A_WIDTH;
//const int UNROLL_ADJ = PES_ADJ;
//const int UNROLL_FEA = PES_FEA;
const int FIFO_DEPTH = MAX_FIFO;

const int FADD_LATENCY_ADJ = FTYPE_LATENCY_ADJ;
const int FADD_LATENCY_FEA = FTYPE_LATENCY_FEA;

static ap_int<64> fifo_full_0;
static ap_int<64> fifo_full_1;
static ap_int<64> fifo_full_2;
static ap_int<64> fifo_empty_0;
static ap_int<64> fifo_empty_1;
static ap_int<64> fifo_empty_2;
static ap_int<64> fifo_read_0;
static ap_int<64> fifo_read_1;
static ap_int<64> fifo_read_2;
static ap_int<64> fifo_write_0;
static ap_int<64> fifo_write_1;
static ap_int<64> fifo_write_2;
static ap_int<64> fifo_cycle_0;
static ap_int<64> fifo_cycle_1;
static ap_int<64> fifo_cycle_2;


#ifdef simulation
extern float max_adj;
extern float min_adj;
extern float max_fea;
extern float min_fea;
extern float acc2_fea_min;
extern float acc2_fea_max;
extern float acc2_adj_min;
extern float acc2_adj_max;
#endif



//extern "C" {
//     void *__dso_handle = NULL;
//}

void dsp_kernel_float_adj_1(ATYPE a_value,BTYPE b_block[B_HEIGHT][B_WIDTH_BLOCK],ap_int<32> b_row,ap_int<8> zero_point_lhs,ap_int<8> zero_point_rhs,ITYPE acc[B_WIDTH_BLOCK])
{
	//#pragma HLS ALLOCATION instances=mul limit=64 operation
	#pragma HLS INLINE

	//ITYPE acc[B_WIDTH_BLOCK];
	//#pragma HLS ARRAY_PARTITION variable=acc complete


	for (int j = 0; j < B_WIDTH_BLOCK; j++) {
			//#pragma HLS UNROLL factor=UNROLL_ADJ
			BTYPE b_val = b_block[b_row][j];
 	  		ATYPE a_val = a_value;
			//acc[j] += A_val*(b_value-zero_point_rhs);
 			//acc[j] = (a_val_float)*(b_val_float-rhs_float);
			acc[j] = (ITYPE)a_val*(ITYPE)b_val;
			//if (b_val > 1)
			//{
			//	//std::cout << " b_val " << b_val << std::endl;
			//	exit(0);
			//}
			////std::cout << "a_val " << a_val << " b_val " << b_val << std::endl;
			////std::cout << "acc " << acc[j] << std::endl;

	} // j loop


}
void dsp_kernel_float_adj_2(int block_size,ATYPE a_value,BTYPE b_block1[B_HEIGHT][B_WIDTH_BLOCK],BTYPE b_block2[B_HEIGHT][B_WIDTH_BLOCK],ap_int<32> b_row,ap_int<8> zero_point_lhs,ap_int<8> zero_point_rhs,ITYPE acc[B_WIDTH_BLOCK])
{
	//#pragma HLS ALLOCATION instances=mul limit=64 operation
	#pragma HLS INLINE

	//ITYPE acc[B_WIDTH_BLOCK];
	//#pragma HLS ARRAY_PARTITION variable=acc complete


	for (int j = 0; j < B_WIDTH_BLOCK; j++) {

	  		ATYPE a_val = a_value;
	  		BTYPE b_val;


	  		int sel_block; // = (b_row>>(log2N-2))&0x3;
	  		int b_row_block;

	  		//std::cout << "b_row " << b_row << std::endl;

	  		if (b_row < block_size)
	  		{
	  			b_row_block = b_row;
	  			sel_block = 0;
	  		}
	  		if (b_row > (block_size-1))
	  		{
	  			b_row_block = b_row-block_size;
	  			sel_block = 1;
	  		}

  		//if (b_row > (2*block_size-1) && b_row < 3*block_size)
	  		//{
  		//	b_row_block = b_row-2*block_size;
	  		//	sel_block = 2;
	  		//}
  		//if (b_row > 3*block_size-1)
	  		//{
  		//	b_row_block = b_row-3*block_size;
	  		//	sel_block = 3;
	  		//}
  		//std::cout << "sel_block "  << sel_block << "b_row_block " << b_row_block << std::endl;

	  		//b_row = b_row&mask;



	  		BTYPE b_val1 = b_block1[b_row_block][j];
			BTYPE b_val2 = b_block2[b_row_block][j];
			//BTYPE b_val3 = b_block3[b_row_block][j];
			//BTYPE b_val4 = b_block4[b_row_block][j];


	  		switch(sel_block)
	  		{
	  			case 0:
	  				b_val = b_val1; //only one value of B in each row. This is the result of the first matrix mult.
	  				break;
	  			case 1:
	  				b_val = b_val2;
  				break;
	  			//case 2:
	  			//	b_val = b_val3;
  			//	break;
	  			//case 3:
	  			//	b_val = b_val4;
  			//	break;
	  		}
			//#pragma HLS UNROLL factor=UNROLL_ADJ
			//acc[j] += A_val*(b_value-zero_point_rhs);
 			//acc[j] = (a_val_float)*(b_val_float-rhs_float);
			acc[j] = (ITYPE)a_val*(ITYPE)b_val;
			//if (b_val > 1)
			//{
			//	//std::cout << " b_val " << b_val << std::endl;
			//	exit(0);
			//}
			////std::cout << "a_val " << a_val << " b_val " << b_val << std::endl;
			////std::cout << "acc " << acc[j] << std::endl;

	} // j loop


}


void dsp_kernel_float_adj_4(int block_size,ATYPE a_value,BTYPE b_block1[B_HEIGHT][B_WIDTH_BLOCK],BTYPE b_block2[B_HEIGHT][B_WIDTH_BLOCK],BTYPE b_block3[B_HEIGHT][B_WIDTH_BLOCK],BTYPE b_block4[B_HEIGHT][B_WIDTH_BLOCK],ap_int<32> b_row,ap_int<8> zero_point_lhs,ap_int<8> zero_point_rhs,ITYPE acc[B_WIDTH_BLOCK])
{
	//#pragma HLS ALLOCATION instances=mul limit=64 operation
	#pragma HLS INLINE

	//ITYPE acc[B_WIDTH_BLOCK];
	//#pragma HLS ARRAY_PARTITION variable=acc complete


	for (int j = 0; j < B_WIDTH_BLOCK; j++) {

	  		ATYPE a_val = a_value;
	  		BTYPE b_val;


	  		int sel_block; // = (b_row>>(log2N-2))&0x3;
	  		int b_row_block;

	  		//std::cout << "b_row " << b_row << std::endl;

	  		if (b_row < block_size)
	  		{
	  			b_row_block = b_row;
	  			sel_block = 0;
	  		}
	  		if (b_row > (block_size-1))
	  		{
	  			b_row_block = b_row-block_size;
	  			sel_block = 1;
	  		}

  		    if (b_row > (2*block_size-1) && b_row < 3*block_size)
	  		{
  		  	    b_row_block = b_row-2*block_size;
	  			sel_block = 2;
	  		}
  		    if (b_row > 3*block_size-1)
	  		{
  			    b_row_block = b_row-3*block_size;
	  			sel_block = 3;
	  		}
  		//std::cout << "sel_block "  << sel_block << "b_row_block " << b_row_block << std::endl;

	  		//b_row = b_row&mask;



	  		BTYPE b_val1 = b_block1[b_row_block][j];
			BTYPE b_val2 = b_block2[b_row_block][j];
			BTYPE b_val3 = b_block3[b_row_block][j];
			BTYPE b_val4 = b_block4[b_row_block][j];


	  		switch(sel_block)
	  		{
	  			case 0:
	  				b_val = b_val1; //only one value of B in each row. This is the result of the first matrix mult.
	  				break;
	  			case 1:
	  				b_val = b_val2;
  				break;
	  			case 2:
	  				b_val = b_val3;
  				break;
	  			case 3:
	  				b_val = b_val4;
  				break;
	  		}
			//#pragma HLS UNROLL factor=UNROLL_ADJ
			//acc[j] += A_val*(b_value-zero_point_rhs);
 			//acc[j] = (a_val_float)*(b_val_float-rhs_float);
			acc[j] = (ITYPE)a_val*(ITYPE)b_val;
			//if (b_val > 1)
			//{
			//	//std::cout << " b_val " << b_val << std::endl;
			//	exit(0);
			//}
			////std::cout << "a_val " << a_val << " b_val " << b_val << std::endl;
			////std::cout << "acc " << acc[j] << std::endl;

	} // j loop


}



void dsp_kernel_float_fea(ATYPE a_value,BTYPE b_block[B_HEIGHT][B_WIDTH_BLOCK],ap_int<32> b_row,ap_int<8> zero_point_lhs,ap_int<8> zero_point_rhs,ITYPE acc[B_WIDTH_BLOCK])
{
	//#pragma HLS ALLOCATION instances=mul limit=64 operation
	#pragma HLS INLINE

    //ITYPE acc_int[B_WIDTH_BLOCK];
	//#pragma HLS ARRAY_PARTITION variable=acc complete


	for (int j = 0; j < B_WIDTH_BLOCK; j++) {
			//#pragma HLS UNROLL factor=UNROLL_ADJ
			BTYPE b_val = b_block[b_row][j];
 	  		ATYPE a_val = a_value;
			//acc[j] += A_val*(b_value-zero_point_rhs);
 			//acc[j] = (a_val_float)*(b_val_float-rhs_float);
 	  		acc[j] = (ITYPE)a_val*(ITYPE)b_val;
			//if(b_val > 1)
			//{
			//	//std::cout << " b_val " << b_val << std::endl;
			//	exit(0);
			//}
			////std::cout << "a_val " << a_val << " b_val " << b_val << std::endl;
			////std::cout << "acc " << acc[j] << std::endl;

	} // j loop


}

void dsp_kernel_int_adj_1(int block_size,ATYPE a_value,ITYPE b_block1[B_HEIGHT][B_WIDTH_BLOCK],
		//ITYPE b_block2[B_HEIGHT/2][B_WIDTH_BLOCK],
		//ITYPE b_block3[B_HEIGHT/4][B_WIDTH_BLOCK],
		//ITYPE b_block4[B_HEIGHT/4][B_WIDTH_BLOCK],
		ap_int<32> b_row,ap_int<8> zero_point_lhs,ap_int<8> zero_point_rhs,ITYPE acc[B_WIDTH_BLOCK])
{
	//#pragma HLS ALLOCATION instances=mul limit=64 operation
	//#pragma HLS INLINE

	//ITYPE acc[B_WIDTH_BLOCK];
	//#pragma HLS ARRAY_PARTITION variable=acc complete

	for (int j = 0; j < B_WIDTH_BLOCK; j++) {

			#pragma HLS UNROLL

				acc[j] = 0;
        }

	//int mask = (1 << (log2N-2)) - 1;


	//////////std::cout << "a_value " << a_value << ////std::endl;
	for (int j = 0; j < B_WIDTH_BLOCK; j++) {
		//#pragma HLS UNROLL factor=UNROLL_ADJ
	    //#pragma HLS PIPELINE
 	  		ATYPE a_val = a_value;
 	  		BTYPE b_val;


 	  		int sel_block; // = (b_row>>(log2N-2))&0x3;
 	  		int b_row_block;

 	  		//std::cout << "b_row " << b_row << std::endl;

 	  		if (b_row < block_size)
 	  		{
 	  			b_row_block = b_row;
 	  			sel_block = 0;
 	  		}
 	  		//if (b_row > (block_size-1))
 	  		//{
 	  		//	b_row_block = b_row-block_size;
 	  		//	sel_block = 1;
 	  		//}

	  		//if (b_row > (2*block_size-1) && b_row < 3*block_size)
 	  		//{
	  		//	b_row_block = b_row-2*block_size;
 	  		//	sel_block = 2;
 	  		//}
	  		//if (b_row > 3*block_size-1)
 	  		//{
	  		//	b_row_block = b_row-3*block_size;
 	  		//	sel_block = 3;
 	  		//}
	  		//std::cout << "sel_block "  << sel_block << "b_row_block " << b_row_block << std::endl;

 	  		//b_row = b_row&mask;



 	  		BTYPE b_val1 = b_block1[b_row_block][j];
 			//BTYPE b_val2 = b_block2[b_row_block][j];
 			//BTYPE b_val3 = b_block3[b_row_block][j];
 			//BTYPE b_val4 = b_block4[b_row_block][j];


 	  		switch(sel_block)
 	  		{
 	  			case 0:
 	  				b_val = b_val1; //only one value of B in each row. This is the result of the first matrix mult.
 	  				break;
 	  			//case 1:
 	  			//	b_val = b_val2;
	  			//	break;
 	  			//case 2:
 	  			//	b_val = b_val3;
	  			//	break;
 	  			//case 3:
 	  			//	b_val = b_val4;
	  			//	break;
 	  		}

 	  		////std::cout << "b_val "  << b_val << std::endl;

 	  		//acc[j] += (A_val-zero_point_lhs)*(b_value-zero_point_rhs);
			//acc[j] += A_val*(b_value-zero_point_rhs);
			ITYPE a_val_i = (ITYPE)a_val;
			ITYPE b_val_i = (ITYPE)b_val;

			ITYPE acc_i = a_val_i*b_val_i;
			acc[j] += acc_i;
			////std::cout << "A val " << a_val_i << " " << a_val << " B val " << b_val_i << //std::endl;
			////std::cout << "dsp kernel for j " << j << " is " << acc_i << //std::endl;
	} // j loop




}

void dsp_kernel_int_adj_2(int block_size,ATYPE a_value,ITYPE b_block1[B_HEIGHT/2][B_WIDTH_BLOCK],
		ITYPE b_block2[B_HEIGHT/2][B_WIDTH_BLOCK],
		//ITYPE b_block3[B_HEIGHT/4][B_WIDTH_BLOCK],
		//ITYPE b_block4[B_HEIGHT/4][B_WIDTH_BLOCK],
		ap_int<32> b_row,ap_int<8> zero_point_lhs,ap_int<8> zero_point_rhs,ITYPE acc[B_WIDTH_BLOCK])
{
	//#pragma HLS ALLOCATION instances=mul limit=64 operation
	//#pragma HLS INLINE

	//ITYPE acc[B_WIDTH_BLOCK];
	//#pragma HLS ARRAY_PARTITION variable=acc complete

	for (int j = 0; j < B_WIDTH_BLOCK; j++) {

			#pragma HLS UNROLL

				acc[j] = 0;
        }

	//int mask = (1 << (log2N-2)) - 1;


	//////////std::cout << "a_value " << a_value << ////std::endl;
	for (int j = 0; j < B_WIDTH_BLOCK; j++) {
		//#pragma HLS UNROLL factor=UNROLL_ADJ
	    //#pragma HLS PIPELINE
 	  		ATYPE a_val = a_value;
 	  		BTYPE b_val;


 	  		int sel_block; // = (b_row>>(log2N-2))&0x3;
 	  		int b_row_block;

 	  		//std::cout << "b_row " << b_row << std::endl;

 	  		if (b_row < block_size)
 	  		{
 	  			b_row_block = b_row;
 	  			sel_block = 0;
 	  		}
 	  		if (b_row > (block_size-1))
 	  		{
 	  			b_row_block = b_row-block_size;
 	  			sel_block = 1;
 	  		}

	  		//if (b_row > (2*block_size-1) && b_row < 3*block_size)
 	  		//{
	  		//	b_row_block = b_row-2*block_size;
 	  		//	sel_block = 2;
 	  		//}
	  		//if (b_row > 3*block_size-1)
 	  		//{
	  		//	b_row_block = b_row-3*block_size;
 	  		//	sel_block = 3;
 	  		//}
	  		//std::cout << "sel_block "  << sel_block << "b_row_block " << b_row_block << std::endl;

 	  		//b_row = b_row&mask;



 	  		BTYPE b_val1 = b_block1[b_row_block][j];
 			BTYPE b_val2 = b_block2[b_row_block][j];
 			//BTYPE b_val3 = b_block3[b_row_block][j];
 			//BTYPE b_val4 = b_block4[b_row_block][j];


 	  		switch(sel_block)
 	  		{
 	  			case 0:
 	  				b_val = b_val1; //only one value of B in each row. This is the result of the first matrix mult.
 	  				break;
 	  			case 1:
 	  				b_val = b_val2;
	  				break;
 	  			//case 2:
 	  			//	b_val = b_val3;
	  			//	break;
 	  			//case 3:
 	  			//	b_val = b_val4;
	  			//	break;
 	  		}

 	  		////std::cout << "b_val "  << b_val << std::endl;

 	  		//acc[j] += (A_val-zero_point_lhs)*(b_value-zero_point_rhs);
			//acc[j] += A_val*(b_value-zero_point_rhs);
			ITYPE a_val_i = (ITYPE)a_val;
			ITYPE b_val_i = (ITYPE)b_val;

			ITYPE acc_i = a_val_i*b_val_i;
			acc[j] += acc_i;
			////std::cout << "A val " << a_val_i << " " << a_val << " B val " << b_val_i << //std::endl;
			////std::cout << "dsp kernel for j " << j << " is " << acc_i << //std::endl;
	} // j loop




}

void dsp_kernel_int_adj_4(int block_size,ATYPE a_value,ITYPE b_block1[B_HEIGHT/4][B_WIDTH_BLOCK],
		ITYPE b_block2[B_HEIGHT/4][B_WIDTH_BLOCK],ITYPE b_block3[B_HEIGHT/4][B_WIDTH_BLOCK],
		ITYPE b_block4[B_HEIGHT/4][B_WIDTH_BLOCK],ap_int<32> b_row,ap_int<8> zero_point_lhs,ap_int<8> zero_point_rhs,ITYPE acc[B_WIDTH_BLOCK])
{
	//#pragma HLS ALLOCATION instances=mul limit=64 operation
	//#pragma HLS INLINE

	//ITYPE acc[B_WIDTH_BLOCK];
	//#pragma HLS ARRAY_PARTITION variable=acc complete

	for (int j = 0; j < B_WIDTH_BLOCK; j++) {

			#pragma HLS UNROLL

				acc[j] = 0;
        }

	//int mask = (1 << (log2N-2)) - 1;


	//////////std::cout << "a_value " << a_value << ////std::endl;
	for (int j = 0; j < B_WIDTH_BLOCK; j++) {
		//#pragma HLS UNROLL factor=UNROLL_ADJ
	    //#pragma HLS PIPELINE
 	  		ATYPE a_val = a_value;
 	  		BTYPE b_val;


 	  		int sel_block; // = (b_row>>(log2N-2))&0x3;
 	  		int b_row_block;

 	  		//std::cout << "b_row " << b_row << std::endl;

 	  		if (b_row < block_size)
 	  		{
 	  			b_row_block = b_row;
 	  			sel_block = 0;
 	  		}
 	  		if (b_row > (block_size-1) && b_row < 2*block_size)
 	  		{
 	  			b_row_block = b_row-block_size;
 	  			sel_block = 1;
 	  		}
	  		if (b_row > (2*block_size-1) && b_row < 3*block_size)
 	  		{
	  			b_row_block = b_row-2*block_size;
 	  			sel_block = 2;
 	  		}
	  		if (b_row > 3*block_size-1)
 	  		{
	  			b_row_block = b_row-3*block_size;
 	  			sel_block = 3;
 	  		}
	  		//std::cout << "sel_block "  << sel_block << "b_row_block " << b_row_block << std::endl;

 	  		//b_row = b_row&mask;



 	  		BTYPE b_val1 = b_block1[b_row_block][j];
 			BTYPE b_val2 = b_block2[b_row_block][j];
 			BTYPE b_val3 = b_block3[b_row_block][j];
 			BTYPE b_val4 = b_block4[b_row_block][j];


 	  		switch(sel_block)
 	  		{
 	  			case 0:
 	  				b_val = b_val1; //only one value of B in each row. This is the result of the first matrix mult.
 	  				break;
 	  			case 1:
 	  				b_val = b_val2;
	  				break;
 	  			case 2:
 	  				b_val = b_val3;
	  				break;
 	  			case 3:
 	  				b_val = b_val4;
	  				break;
 	  		}

 	  		////std::cout << "b_val "  << b_val << std::endl;

 	  		//acc[j] += (A_val-zero_point_lhs)*(b_value-zero_point_rhs);
			//acc[j] += A_val*(b_value-zero_point_rhs);
			ITYPE a_val_i = (ITYPE)a_val;
			ITYPE b_val_i = (ITYPE)b_val;

			ITYPE acc_i = a_val_i*b_val_i;
			acc[j] += acc_i;
			////std::cout << "A val " << a_val_i << " " << a_val << " B val " << b_val_i << //std::endl;
			////std::cout << "dsp kernel for j " << j << " is " << acc_i << //std::endl;
	} // j loop




}


void dsp_kernel_int_fea(FTYPE a_value,BTYPE b_block[B_HEIGHT/4][B_WIDTH_BLOCK],ap_int<32> b_row,ap_int<8> zero_point_lhs,ap_int<8> zero_point_rhs,ITYPE acc[B_WIDTH_BLOCK])
{
	//#pragma HLS ALLOCATION instances=mul limit=64 operation
	//#pragma HLS INLINE

	//ITYPE acc[B_WIDTH_BLOCK];
	//#pragma HLS ARRAY_PARTITION variable=acc complete

	for (int j = 0; j < B_WIDTH_BLOCK; j++) {

			#pragma HLS UNROLL

				acc[j] = 0;
        }

	//////////std::cout << "a_value " << a_value << ////std::endl;
	for (int j = 0; j < B_WIDTH_BLOCK; j++) {
		//#pragma HLS UNROLL factor=UNROLL_FEA
	    //#pragma HLS PIPELINE
 	  		FTYPE a_val = a_value;
			BTYPE b_val = b_block[b_row][j]; //only one value of B in each row. This is the result of the first matrix mult.
			//acc[j] += (A_val-zero_point_lhs)*(b_value-zero_point_rhs);
			//acc[j] += A_val*(b_value-zero_point_rhs);
			ITYPE a_val_i = (ITYPE)a_val;
			ITYPE b_val_i = (ITYPE)b_val;

			ITYPE acc_i = a_val_i*b_val_i;
			acc[j] += acc_i;
			////std::cout << "a val " << a_val_i << " b val " << b_val_i << //std::endl;
			////std::cout << "dsp kernel fea for j " << j << " is " << acc_i << //std::endl;
	} // j loop




}

/*void writec(int first_row, int row_count,int P, hls::stream<ITYPE> write_fifo[C_WIDTH_BLOCK][SPMM_BLOCK], DTYPE* C,int B_index,int B_index_loop,int tail)
{
		int B_WIDTH_INT;

		int WL;

		#if defined FLOAT
			WL = row_count;
			//array_c_adjust = array_c_adjust;
		#endif

		#if defined HALF
			WL = row_count;
			//array_c_adjust = array_c_adjust;
		#endif

		#ifdef EIGHTBIT
			//WL = (N>>2);
			//array_c_adjust = (array_c_adjust>>2);
			WL = row_count;
			//array_c_adjust = array_c_adjust;

		#endif



		if (B_index < (B_index_loop-1))
			B_WIDTH_INT = B_WIDTH_BLOCK;
		else
			B_WIDTH_INT = tail;


		////std::cout << " WL " << WL << " B_WIDTH_INT " << B_WIDTH_INT << std::endl;
		LOOP_WRITE1:    for (int i = 0; i < WL; i+=SPMM_BLOCK) {
			DTYPE C_out;
	  		LOOP_WRITE2: for (int j = 0; j <  B_WIDTH_INT; j++) {
		  		LOOP_WRITE3: for (int z = 0; z <  SPMM_BLOCK; z++) {
					#pragma HLS PIPELINE
	 				if ((z+i) < WL)
					{
						C_out =  write_fifo[j][z].read();
						//////std::cout << "B_index " << B_index << ////std::endl;
						////std::cout << " cout in position " << (i+first_row) << "," << j << " is " << C_out << std::endl;
						#ifdef ENABLE_TRANSPOSE
							//C[i+(j+B_index*B_WIDTH_BLOCK)*(array_c_adjust)] = C_out;
						#else
							C[(i+z)*P+j+B_index*B_WIDTH_BLOCK] = C_out;
							//C[i*B_WIDTH_INT+j] = C_out;
							//C[j] = C_out;
						#endif
					}
				}
			}

		}
}*/



void writec(bool relu,int first_row, int row_count,int P, hls::stream<ITYPE> write_fifo[C_WIDTH_BLOCK][SPMM_BLOCK], DTYPE* C,int B_index,int B_index_loop,int tail)
{
		int B_WIDTH_INT;

		int WL;

		#if defined FLOAT
			WL = row_count;
			//array_c_adjust = array_c_adjust;
		#endif

		#if defined HALF
			WL = row_count;
			//array_c_adjust = array_c_adjust;
		#endif

		#ifdef EIGHTBIT
			//WL = (N>>2);
			//array_c_adjust = (array_c_adjust>>2);
			WL = row_count;
			//array_c_adjust = array_c_adjust;

		#endif



		if (B_index < (B_index_loop-1))
			B_WIDTH_INT = B_WIDTH_BLOCK;
		else
			B_WIDTH_INT = tail;


		//std::cout << " WL " << WL << " B_WIDTH_INT " << B_WIDTH_INT << std::endl;
		//LOOP_WRITE1:    for (int i = 0; i < WL; i++) {

		#if (USE_SBLOCKS == 1)
		  LOOP_WRITE1:    for (int i = 0; i < WL; i+=SPMM_BLOCK) {
			DTYPE C_out;
	  		LOOP_WRITE3: for (int z = 0; z <  SPMM_BLOCK; z++) {
 				if ((z+i) < WL)
				{
                #if (USE_TAIL == 1)
 					LOOP_WRITE2: for (int j = 0; j <  B_WIDTH_INT; j++) {
				#else
 					LOOP_WRITE2: for (int j = 0; j <  B_WIDTH_BLOCK; j++) {
                #endif
					#pragma HLS PIPELINE
						C_out =  write_fifo[j][z].read();
						//C_out =  write_fifo[j][0].read();
						//////std::cout << "B_index " << B_index << ////std::endl;
						////std::cout << " cout in position " << (i+first_row) << "," << j << " is " << C_out << std::endl;
						#ifdef ENABLE_TRANSPOSE
							//C[i+(j+B_index*B_WIDTH_BLOCK)*(array_c_adjust)] = C_out;
						#else
							#if (USE_RELU == 1)
						    if(j<B_WIDTH_INT)
			    			{
						    	//if (C_out > 0 || relu == 0)
						    		C[(i+z)*P+j+B_index*B_WIDTH_BLOCK] = C_out;
						    	//else
						    	//	C[(i+z)*P+j+B_index*B_WIDTH_BLOCK] = 0.0;
						    }
							#else
					    		 C[(i+z)*P+j+B_index*B_WIDTH_BLOCK] = C_out;
                            #endif
							//C[i*P+j+B_index*B_WIDTH_BLOCK] = C_out;
							//C[i*B_WIDTH_INT+j] = C_out;
							//C[j] = C_out;
						#endif
					}
				}
			}
		  }
		#endif

        #if (USE_SBLOCKS == 0)
		LOOP_WRITE4:    for (int i = 0; i < WL; i++) {
			DTYPE C_out;
				 #if (USE_TAIL == 1)
			            LOOP_WRITE5: for (int j = 0; j <  B_WIDTH_INT; j++) { //this reduces performance
                 #else
			        	LOOP_WRITE5: for (int j = 0; j <  B_WIDTH_BLOCK; j++) {
                 #endif
					#pragma HLS PIPELINE
			        	C_out =  write_fifo[j][0].read();
						#if (USE_RELU == 1)
						if(j<B_WIDTH_INT)
	    				{
			        		if (C_out > 0 || relu == 0)
								C[i*P+j+B_index*B_WIDTH_BLOCK] = C_out;
			        		else
			        			C[i*P+j+B_index*B_WIDTH_BLOCK] = 0.0;
			        	}
						#else
			        		C[i*P+j+B_index*B_WIDTH_BLOCK] = C_out;
						#endif
					}
		}
		#endif
}


void readptr_fea(bool gemm_mode,int N,int M,int *rowPtr,hls::stream<int> rnnz_fifo[SPMM_BLOCK])
{

    #pragma HLS inline off
	int rnnz,current_index,next_index;


	if (gemm_mode==0)
	{
		current_index= rowPtr[0];
		//printf("N rows are %d\n", N);
		LOOP_A_INDEX_SPMM1 : for (int A_index = 0; A_index < N; A_index+=SPMM_BLOCK) {
			int brnnz = 0;
			LOOP_B_INDEX_SPMM1 : for (int B_index = 0; B_index < SPMM_BLOCK; B_index++) {
			#pragma HLS PIPELINE
				if((A_index+B_index) < N)
				{
					next_index=rowPtr[A_index+B_index+1];
					rnnz = next_index-current_index;
					brnnz+=rnnz;
					current_index = next_index;
					rnnz_fifo[B_index] << brnnz;
					//printf ("BRNNZ %d\n",brnnz);
				}
				else
				{
					rnnz_fifo[B_index] << brnnz; //juse use the last value of rnnz in block
					//printf ("BRNNZ %d\n",brnnz);
				}
			}
        }
	}
	else
	{
		LOOP_A_INDEX_SPMM2 : for (int A_index = 0; A_index < N; A_index+=SPMM_BLOCK) {
			int brnnz = 0;
			LOOP_B_INDEX_SPMM2 : for (int B_index = 0; B_index < SPMM_BLOCK; B_index++) {
			#pragma HLS PIPELINE
			if((A_index+B_index) < N)
			{
				rnnz = M;
				brnnz+=rnnz;
				rnnz_fifo[B_index] << brnnz;
			}
			else
			{
				rnnz_fifo[B_index] << brnnz; //juse use the last value of rnnz in block
			}
		 }
       }
	} //end else

}

void readptr_adj(int N,int *rowPtr,hls::stream<int> rnnz_fifo[SPMM_BLOCK])
{
	#pragma HLS inline off
	int rnnz,current_index,next_index;

	current_index= rowPtr[0];



		LOOP_A_INDEX_SPMM1 : for (int A_index = 0; A_index < N; A_index+=SPMM_BLOCK) {
			int brnnz=0;
			LOOP_B_INDEX_SPMM1 : for (int B_index = 0; B_index < SPMM_BLOCK; B_index++) {
			#pragma HLS PIPELINE
			if((A_index+B_index) < N)
			{
				next_index=rowPtr[A_index+B_index+1];
				rnnz = next_index-current_index;
				brnnz+=rnnz;
				current_index = next_index;
				//printf ("RNNZ %d\n",rnnz);
				rnnz_fifo[B_index] << brnnz;
			}
			else
				rnnz_fifo[B_index] << brnnz; //juse use the last value of rnnz in block
        }
	}
}

void readval_adj(int last_index,hls::stream<ATYPE> &A_fifo,hls::stream<int> &col_indices_fifo,ATYPE *values,int *columnIndex)
{

		#pragma HLS inline off
		LOOP_J_SPMM : for (int j = 0; j < last_index; j++) {
			#pragma HLS PIPELINE
			A_fifo <<  values[j];
			col_indices_fifo << columnIndex[j];
			////std::cout << "col indices " << columnIndex[j] << std::endl;
		}

}

/*void readval_fea(int last_index,hls::stream<bool> &exit_loop,hls::stream<FTYPE> &A_fifo,hls::stream<int> &col_indices_fifo,FTYPE *values,int *columnIndex)
{

		#pragma HLS inline off
	    exit_loop << 0;
		LOOP_J_SPMM : for (int j = 0; j < last_index; j++) {
			#pragma HLS PIPELINE

			A_fifo <<  values[j];
			////std::cout << "A_fifo " << values[j] << std::endl;
			col_indices_fifo << columnIndex[j];
			////std::cout << "col index fea " << columnIndex[j] << std::endl;
		}
		exit_loop << 1;

}
*/
/*
void readval_fea(int last_index,hls::stream<FTYPE> &A_fifo,hls::stream<int> &col_indices_fifo,FTYPE *values,int *columnIndex)
{

		#pragma HLS inline off
	    int j=0;
		LOOP_J_SPMM : while(j < last_index) {
			#pragma HLS PIPELINE
			if(!A_fifo.full() && !col_indices_fifo.full())
			{
				A_fifo << values[j];
				////std::cout << "A_fifo " << values[j] << std::endl;
			    col_indices_fifo << columnIndex[j];
				j++;
			}
			else
			{
				fifo_full_0;
			}

			////std::cout << "col index fea " << columnIndex[j] << std::endl;
		}

}
*/
void readval_fea(bool gemm_mode,int ccount,int last_index,hls::stream<FTYPE> &A_fifo,hls::stream<int> &col_indices_fifo,FTYPE *values,int *columnIndex)
{

	#pragma HLS inline off
	if (gemm_mode==0)
	{
		LOOP_J_SPMM1 : for (int j = 0; j < last_index; j++) {
			#pragma HLS PIPELINE
			//if(A_fifo.full())
			//	fifo_full_0++;
			 //if(A_fifo.empty())
				//fifo_empty_0++;

			//if(col_indices_fifo.full())
			//	fifo_full_1++;
			//if(col_indices_fifo.empty())
				//fifo_empty_1++;

			A_fifo <<  values[j];

			col_indices_fifo << columnIndex[j];

			//std::cout << "A_fifo " << values[j] << std::endl;
			//std::cout << "col index fea " << columnIndex[j] << std::endl;

		    //std::cout << "col index fea " << columnIndex[j] << std::endl;

			//std::cout << "A_fifo " << values[j] << std::endl;
			//std::cout << "col index fea " << columnIndex[j] << std::endl;


		}
	}
	else
	{
			int c=0;
			LOOP_J_SPMM2 : for (int j = 0; j < last_index; j++) {
			#pragma HLS PIPELINE
				//if(A_fifo.full())
				//	fifo_full_0++;
				 //if(A_fifo.empty())
					//fifo_empty_0++;

				//if(col_indices_fifo.full())
				//	fifo_full_1++;
				//if(col_indices_fifo.empty())
					//fifo_empty_1++;

				A_fifo <<  values[j];
				col_indices_fifo << c;

				//std::cout << "A_fifo " << values[j] << std::endl;
				//std::cout << "col index fea " << c << std::endl;

				if (c == (ccount-1)) //column count
					c=0;
				else
					c++;


			}

	}
}


void check_fifo_0(int a_values, hls::stream<ITYPE> &A_fifo, hls::stream<ITYPE> &A_fifo_out)
{
	ITYPE data_buffer;
	int data_count=0;
	bool loop_done = 0;
	bool data_in_buffer = 0; //data exits in data_buffer
	//bool fifo_active = 1;
	while((data_count < a_values) || data_in_buffer == 0)
	{
		#pragma HLS PIPELINE
			fifo_cycle_0++;
  	       	if (data_in_buffer == 0) //data_buffer empty
			{
				if(A_fifo.read_nb(data_buffer) == 1)
				{
					fifo_read_0++;
					data_count++;
					if(A_fifo_out.write_nb(data_buffer) == 0)
					{
						//fifo_write_0++;
						fifo_full_0++;
						data_in_buffer = 1; //fifo full and data stored in data_in_buffer
					}
					else
					{
						fifo_write_0++;
					}
				}
				else
				{
					//fifo_empty_0++;
					//fifo_read_0++;
				}
			}
			else //data_buffer not empty
			{
				if (A_fifo_out.write_nb(data_buffer) == 1)
				{
					fifo_write_0++;
					if(A_fifo.read_nb(data_buffer) == 0)
					{
						//fifo_empty_0++;
						data_in_buffer = 0; //data_buffer empty
					}
					else
					{
						fifo_read_0++;
						data_count++;
					}

				}
				else
				{
					//fifo_write_0++;
					fifo_full_0++;
				}
			}
	}


}

/*
void check_fifo_0(hls::stream<bool> &exit_loop, hls::stream<ITYPE> &A_fifo, hls::stream<ITYPE> &A_fifo_out)
{
	ITYPE data_buffer;
	//int data_count=0;
	bool loop_done = 0;
	bool data_in_buffer = 0; //data exits in data_buffer
	//bool fifo_active = 1;
	while(loop_done == 0 || data_in_buffer == 1)
	{
		#pragma HLS PIPELINE
			fifo_cycle_0++;
  	       	if (data_in_buffer == 0) //data_buffer empty
			{
				if(A_fifo.read_nb(data_buffer) == 1)
				{
					fifo_read_0++;
					if(A_fifo_out.write_nb(data_buffer) == 0)
					{
						//fifo_write_0++;
						fifo_full_0++;
						data_in_buffer = 1; //fifo full and data stored in data_in_buffer
					}
					else
					{
						//data_count++;
						fifo_write_0++;
					}
				}
				else
				{
					//fifo_empty_0++;
					//fifo_read_0++;
				}
			}
			else //data_buffer not empty
			{
				if (A_fifo_out.write_nb(data_buffer) == 1)
				{
					fifo_write_0++;
					if(A_fifo.read_nb(data_buffer) == 0)
					{
						//fifo_empty_0++;
						data_in_buffer = 0; //data_buffer empty
					}
					else
					{
						fifo_read_0++;
					}
					//data_count++;
				}
				else
				{
					//fifo_write_0++;
					fifo_full_0++;
				}
			}
  	    bool exit_data;
		if(exit_loop.read_nb(exit_data) == 1)
				loop_done == 1;

	}


}
*/
void check_fifo_2(int N, hls::stream<ITYPE> &C_fifo, hls::stream<ITYPE> &C_fifo_out)
{
	ITYPE data_buffer;
	int data_count=0;
	bool data_in_buffer= 0; //data exits in data_buffer


	while(data_count < N)
	{
		#pragma HLS PIPELINE
		//LOOP_CHECK_2 : for (int j = 0; j < B_WIDTH_BLOCK; j++)
		//{
		   //if (j < B_WIDTH_INT)
		   //{
			fifo_cycle_2++;
  	       		if (data_in_buffer == 0) //data_buffer empty
			{
				if(C_fifo.read_nb(data_buffer) == 1)
				{

					fifo_read_2++;
					if(C_fifo_out.write_nb(data_buffer) == 0)
					{
						//fifo_write_2++;
						fifo_full_2++;
						data_in_buffer = 1; //fifo full and data stored in data_in_buffer
					}
					else
					{
						data_count++;
						fifo_write_2++;
					}
				}
				else
				{
					fifo_empty_2++;
					//fifo_read_2++;
				}

			}
			else //data_buffer not empty
			{
				if (C_fifo_out.write_nb(data_buffer) == 1)
				{
					fifo_write_2++;
					if(C_fifo.read_nb(data_buffer) == 0)
					{
						fifo_empty_2++;
						data_in_buffer = 0; //data_buffer empty
					}
					else
					{
						fifo_read_2++;
					}
					data_count++;

				}
				else
				{
					//fifo_write_2++;
					fifo_full_2++;
				}
			}
		   //} // j < B_WIDTH_INT
		//} //LOOP_CHECK_2
	} //while

}



void check_fifo_1(int N, int B_index, int B_index_loop, int tail, hls::stream<ITYPE> &C_fifo, hls::stream<ITYPE> &C_fifo_out)
{
	ITYPE data_buffer;
	int data_count=0;
	bool data_in_buffer= 0; //data exits in data_buffer

       /*int B_WIDTH_INT;

       if (B_index < (B_index_loop-1))
 		B_WIDTH_INT = B_WIDTH_BLOCK;
       else
		B_WIDTH_INT = tail;*/

	//while(data_count < N*B_WIDTH_INT)
	while(data_count < N)
	{
		#pragma HLS PIPELINE
		//LOOP_CHECK_2 : for (int j = 0; j < B_WIDTH_BLOCK; j++)
		//{
		   //if (j < B_WIDTH_INT)
		   //{
			fifo_cycle_1++;
  	       		if (data_in_buffer == 0) //data_buffer empty
			{
				if(C_fifo.read_nb(data_buffer) == 1)
				{

					fifo_read_1++;
					if(C_fifo_out.write_nb(data_buffer) == 0)
					{
						//fifo_write_1++;
						fifo_full_1++;
						data_in_buffer = 1; //fifo full and data stored in data_in_buffer
					}
					else
					{
						data_count++;
						fifo_write_1++;
					}
				}
				else
				{
					//fifo_empty_1++;
					//fifo_read_1++;
				}

			}
			else //data_buffer not empty
			{
				if (C_fifo_out.write_nb(data_buffer) == 1)
				{
					fifo_write_1++;
					if(C_fifo.read_nb(data_buffer) == 0)
					{
						//fifo_empty_1++;
						data_in_buffer = 0; //data_buffer empty
					}
					else
					{
						fifo_read_1++;
					}
					data_count++;

				}
				else
				{
					//fifo_write_1++;
					fifo_full_1++;
				}
			}
		   //} // j < B_WIDTH_INT
		//} //LOOP_CHECK_2
	} //while

}




//void reada1(hls::stream<bool> &exit_loop,int first_row, int row_count, hls::stream<FTYPE> &A_fifo_fea,hls::stream<int> &col_indices_fifo_fea, hls::stream<int> &rnnz_fifo_fea,
//int B_index_loop, int tail,int *rowPtr_fea,int *columnIndex_fea,FTYPE *values_fea)
void reada1(bool gemm_mode,int M, int first_row, int row_count, hls::stream<FTYPE> &A_fifo_fea,hls::stream<int> &col_indices_fifo_fea, hls::stream<int> rnnz_fifo_fea[SPMM_BLOCK],
int B_index_loop, int tail,int *rowPtr_fea,int *columnIndex_fea,FTYPE *values_fea)
{
      //hls::stream<DTYPE>       A_accel;
      //#pragma HLS STREAM variable=A_accel depth=A_WIDTH_FIFO dim=1

        //feature sparse matrix




	int last_index_fea;

	//int last_index_adj;

	//last_index_adj=rowPtr_adj[N_adj];
	if (gemm_mode==0)
	{
		last_index_fea=rowPtr_fea[first_row+row_count]-rowPtr_fea[first_row];
		//std::cout << "Thread fea is processing non-zeros " << last_index_fea << "from address " << values_fea << std::endl;
		columnIndex_fea += rowPtr_fea[first_row];
		values_fea += rowPtr_fea[first_row];
        //std::cout << "rowPtr_fea[first_row] " << rowPtr_fea[first_row] << std::endl;
	    rowPtr_fea += first_row;
	}
	else
	{
		last_index_fea=row_count*M;
		values_fea+=first_row*M;
	}




    //std::cout << "last index fea " << last_index_fea << std::endl;

    //std::cout << "first_row " << first_row << "row_count " << row_count << std::endl;





        ////std::cout << "last index adj " << last_index_adj << std::endl;

	//last_index=N*M;

    	//for (int B_index = 0; B_index < B_index_loop; B_index++)
	//{

        //feature sparse matrix

	readptr_fea(gemm_mode,row_count,M,rowPtr_fea,rnnz_fifo_fea);
	readval_fea(gemm_mode,M,last_index_fea,A_fifo_fea,col_indices_fifo_fea,values_fea,columnIndex_fea);
	//readval_fea(last_index_fea,exit_loop,A_fifo_fea,col_indices_fifo_fea,values_fea,columnIndex_fea);


     //adjacency sparse matrix

        ////////std::cout << "last index adj " << last_index_adj << ////std::endl;

	//readptr_adj(N_adj,rowPtr_adj,rnnz_fifo_adj);
	//readval_adj(last_index_adj,A_fifo_adj,col_indices_fifo_adj,values_adj,columnIndex_adj);

    	//}
}

void reada2(int first_row, int row_count, int B_index_loop, int tail,hls::stream<ATYPE> &A_fifo_adj,hls::stream<int> &col_indices_fifo_adj, hls::stream<int> rnnz_fifo_adj[SPMM_BLOCK],
int *rowPtr_adj,int *columnIndex_adj,ATYPE *values_adj)
{
      //hls::stream<DTYPE>       A_accel;
      //#pragma HLS STREAM variable=A_accel depth=A_WIDTH_FIFO dim=1

        //feature sparse matrix




	//int last_index_fea;
	int last_index_adj;

	last_index_adj=rowPtr_adj[first_row+row_count]-rowPtr_adj[first_row];
	//std::cout << "Thread adj is processing non-zeros " << last_index_adj << "from address " << values_adj << std::endl;
	columnIndex_adj += rowPtr_adj[first_row];
	values_adj += rowPtr_adj[first_row];
	rowPtr_adj += first_row;
	//columnIndex_adj += first_row;
	//values_adj += first_row;


	//last_index_fea=rowPtr_fea[N_fea];

	//std::cout << "columnIndex_adj " << columnIndex_adj[0] << " first_row " << first_row << std::endl;

	//last_index=N*M;

    	//for (int B_index = 0; B_index < B_index_loop; B_index++)
	//{

        //feature sparse matrix

	//readptr_fea(N_fea,rowPtr_fea,rnnz_fifo_fea);
	//readval_fea(last_index_fea,A_fifo_fea,col_indices_fifo_fea,values_fea,columnIndex_fea);


     //adjacency sparse matrix

        ////////std::cout << "last index adj " << last_index_adj << ////std::endl;

	readptr_adj(row_count,rowPtr_adj,rnnz_fifo_adj);
	readval_adj(last_index_adj,A_fifo_adj,col_indices_fifo_adj,values_adj,columnIndex_adj);

    	//}
}


void dsp_kernel_wrapper_adj_4(int block_size,int M[SPMM_BLOCK],hls::stream<ATYPE> &A_fifo,hls::stream<int> &col_indices_fifo,ITYPE b_block1[B_HEIGHT/4][B_WIDTH_BLOCK],
		ITYPE b_block2[B_HEIGHT/4][B_WIDTH_BLOCK],ITYPE b_block3[B_HEIGHT/4][B_WIDTH_BLOCK],ITYPE b_block4[B_HEIGHT/4][B_WIDTH_BLOCK],
		ap_int<8> zero_point_lhs,ap_int<8> zero_point_rhs,ITYPE acc2[B_WIDTH_BLOCK][SPMM_BLOCK])
{

#if defined FLOAT || defined HALF


FTYPE acc_part[FADD_LATENCY_ADJ][B_WIDTH_BLOCK][SPMM_BLOCK];
#pragma HLS ARRAY_PARTITION variable=acc_part complete

FTYPE acc_float[B_WIDTH_BLOCK];
    #pragma HLS ARRAY_PARTITION variable=acc_float complete



for (int j = 0; j < B_WIDTH_BLOCK; j++) {

	#pragma HLS UNROLL

		acc_float[j] = 0;
}





		RESET_ACC_LOOP_SPMM: for (int j = 0; j < B_WIDTH_BLOCK; j++) {
		#pragma HLS UNROLL
			for (int l = 0; l < FADD_LATENCY_ADJ; l++) {
			#pragma HLS UNROLL
				for (int z = 0; z < SPMM_BLOCK; z++){
					#pragma HLS UNROLL
						acc_part[l][j][z] = 0;
				}
		}
 	}

		  int BM = M[SPMM_BLOCK-1];

		  int M_aux[SPMM_BLOCK+1]; //store the different number of nonzeros intervals
		  M_aux[0] = 0;
		  for (int j = 1; j < SPMM_BLOCK+1; j++)
		  {
			 #pragma HLS UNROLL
			 M_aux[j] = M[j-1];
		  }
		  //print

		DSP_LOOP_SPMM: for(int k = 0; k < BM; k+=FADD_LATENCY_ADJ) {
		#pragma HLS PIPELINE II=FADD_LATENCY_ADJ rewind

		DSP_LOOP_SPMM2: for(int i = 0; i < FADD_LATENCY_ADJ; i++) {

			DTYPE v;
			int ci;
			if ((k+i) < BM) //avoid trying to read empty FIFO that only contains M elements
			{
				v = A_fifo.read();
				ci = col_indices_fifo.read();
			}
		        else
			{
				v=0;
				ci=0;
			}


			////////std::cout << " compute1 read col fifo" << ////std::endl;



			dsp_kernel_float_adj_4(block_size,v,b_block1,b_block2,b_block3,b_block4,ci,zero_point_lhs,zero_point_rhs,acc_float);


		        for (int j = 0; j < B_WIDTH_BLOCK; j++) {
			  	//#pragma HLS UNROLL
				//#pragma HLS dependence variable=acc_val inter false
		        	for (int z = 0; z < SPMM_BLOCK; z++)
		        	{
		        	 		#pragma HLS UNROLL
		        	  		if ((k+i)>=M_aux[z]&&(k+i)<M_aux[z+1])
		        				        			acc_part[i][j][z] += acc_float[j];
		        	}
				#ifdef simulation
				if (acc_part[i][j] > max_adj)
							max_adj = acc_part[i][j];
						if (acc_part[i][j] < min_adj)
							min_adj = acc_part[i][j];
				#endif
			}

			      } //i loop

	} // k loop

for (int j = 0; j < B_WIDTH_BLOCK; j++) {
        #pragma HLS UNROLL
		for (int l = 1; l < FADD_LATENCY_ADJ; l++) {
		    for (int z = 0; z < SPMM_BLOCK; z++)
	        {
    		     acc_part[0][j][z] += acc_part[l][j][z];
	        }
		}
	}

   	for (int j = 0; j < B_WIDTH_BLOCK; j++) {
            #pragma HLS UNROLL
   	        for (int z = 0; z < SPMM_BLOCK; z++)
   			{
			  #pragma HLS UNROLL
   			  FTYPE acc_part_float = acc_part[0][j][z];
   			  acc2[j][z] = acc_part_float;
   			}
	}



#endif

	#ifdef EIGHTBIT

			ITYPE acc[B_WIDTH_BLOCK];
	#pragma HLS ARRAY_PARTITION variable=acc complete

	//for (int j = 0; j < B_WIDTH_BLOCK; j++) {

	//	#pragma HLS UNROLL

	//		acc2[j] = 0;
	//}


    int BM = M[SPMM_BLOCK-1];

	int M_aux[SPMM_BLOCK+1];
	M_aux[0] = 0;
	for (int j = 1; j < SPMM_BLOCK+1; j++)
	{
			#pragma HLS UNROLL
			M_aux[j] = M[j-1];
	}

	 DSP_LOOP_SPMM: for (int i = 0; i < BM; i+=1) {
		 	 #pragma HLS PIPELINE
        	 //#pragma HLS UNROLL factor=PARALLEL_ROW
				DTYPE v = A_fifo.read();

			int ci = col_indices_fifo.read();

			dsp_kernel_int_adj_4(block_size,v,b_block1,b_block2,
					b_block3,b_block4,
					ci,zero_point_lhs,zero_point_rhs,acc);



			for (int j = 0; j < B_WIDTH_BLOCK; j++) {

				#pragma HLS UNROLL
				for (int z = 0; z < SPMM_BLOCK; z++)
				{
						#pragma HLS UNROLL
						if (i>=M_aux[z]&&i<M_aux[z+1])
								acc2[j][z] += acc[j];
				}//z loop
						//////std::cout << " compute2 acc with j " << j << "acc2[j] is " << acc2[j] << ////std::endl;
			}//j loop

				//////////std::cout << " compute1 acc with j " << j << "acc2[j] is " << acc2[j] << ////std::endl;


	     	} //i loop





	#endif

}

void dsp_kernel_wrapper_adj_2(int block_size,int M[SPMM_BLOCK],hls::stream<ATYPE> &A_fifo,hls::stream<int> &col_indices_fifo,ITYPE b_block1[B_HEIGHT/4][B_WIDTH_BLOCK],
		ITYPE b_block2[B_HEIGHT/4][B_WIDTH_BLOCK],
		//ITYPE b_block3[B_HEIGHT/4][B_WIDTH_BLOCK],ITYPE b_block4[B_HEIGHT/4][B_WIDTH_BLOCK],
		ap_int<8> zero_point_lhs,ap_int<8> zero_point_rhs,ITYPE acc2[B_WIDTH_BLOCK][SPMM_BLOCK])
{



		#if defined FLOAT || defined HALF


	FTYPE acc_part[FADD_LATENCY_ADJ][B_WIDTH_BLOCK][SPMM_BLOCK];
	#pragma HLS ARRAY_PARTITION variable=acc_part complete

	FTYPE acc_float[B_WIDTH_BLOCK];
        #pragma HLS ARRAY_PARTITION variable=acc_float complete



	for (int j = 0; j < B_WIDTH_BLOCK; j++) {

		#pragma HLS UNROLL

			acc_float[j] = 0;
	}





 		RESET_ACC_LOOP_SPMM: for (int j = 0; j < B_WIDTH_BLOCK; j++) {
			#pragma HLS UNROLL
				for (int l = 0; l < FADD_LATENCY_ADJ; l++) {
				#pragma HLS UNROLL
					for (int z = 0; z < SPMM_BLOCK; z++){
								    acc_part[l][j][z] = 0;
				}
			}
	 	}

        int BM = M[SPMM_BLOCK-1];

 		 int M_aux[SPMM_BLOCK+1]; //store the different number of nonzeros intervals
 		 M_aux[0] = 0;
 	     for (int j = 1; j < SPMM_BLOCK+1; j++)
 		 {
 			#pragma HLS UNROLL
 			M_aux[j] = M[j-1];
 		 }
         //print


   		DSP_LOOP_SPMM: for(int k = 0; k < BM; k+=FADD_LATENCY_ADJ) {
			#pragma HLS PIPELINE II=FADD_LATENCY_ADJ rewind

			DSP_LOOP_SPMM2: for(int i = 0; i < FADD_LATENCY_ADJ; i++) {

				DTYPE v;
				int ci;
				if ((k+i) < BM) //avoid trying to read empty FIFO that only contains M elements
				{
					v = A_fifo.read();
					ci = col_indices_fifo.read();
				}
			        else
				{
					v=0;
					ci=0;
				}


				////////std::cout << " compute1 read col fifo" << ////std::endl;



				dsp_kernel_float_adj_2(block_size,v,b_block1,b_block2,ci,zero_point_lhs,zero_point_rhs,acc_float);


			        for (int j = 0; j < B_WIDTH_BLOCK; j++) {
			          	for (int z = 0; z < SPMM_BLOCK; z++)
			          	{
			    		    #pragma HLS UNROLL
			    			if ((k+i)>=M_aux[z]&&(k+i)<M_aux[z+1])
			    			  			acc_part[i][j][z] += acc_float[j];
			    			}//#pragma HLS UNROLL
					//#pragma HLS dependence variable=acc_val inter false
					#ifdef simulation
					if (acc_part[i][j] > max_adj)
								max_adj = acc_part[i][j];
							if (acc_part[i][j] < min_adj)
								min_adj = acc_part[i][j];
					#endif
				}

    			      } //i loop

		} // k loop

	for (int j = 0; j < B_WIDTH_BLOCK; j++) {
            #pragma HLS UNROLL
			for (int l = 1; l < FADD_LATENCY_ADJ; l++) {
			    #pragma HLS unroll
				  for (int z = 0; z < SPMM_BLOCK; z++)
				  {
					acc_part[0][j][z] += acc_part[l][j][z];
				  }

			}
		}

       	for (int j = 0; j < B_WIDTH_BLOCK; j++) {
                #pragma HLS UNROLL
		        for (int z = 0; z < SPMM_BLOCK; z++)
		        {
				   FTYPE acc_part_float = acc_part[0][j][z];
				   acc2[j][z] = acc_part_float;
		        }
		}



	#endif

		#ifdef EIGHTBIT

				ITYPE acc[B_WIDTH_BLOCK];
		#pragma HLS ARRAY_PARTITION variable=acc complete

		//for (int j = 0; j < B_WIDTH_BLOCK; j++) {

		//	#pragma HLS UNROLL

		//		acc2[j] = 0;
		//}


        int BM = M[SPMM_BLOCK-1];

		int M_aux[SPMM_BLOCK+1];
		M_aux[0] = 0;
		for (int j = 1; j < SPMM_BLOCK+1; j++)
		{
				#pragma HLS UNROLL
				M_aux[j] = M[j-1];
		}

		 DSP_LOOP_SPMM: for (int i = 0; i < BM; i+=1) {
 		 	 #pragma HLS PIPELINE
	        	 //#pragma HLS UNROLL factor=PARALLEL_ROW
    				DTYPE v = A_fifo.read();

				int ci = col_indices_fifo.read();

				dsp_kernel_int_adj_2(block_size,v,b_block1,b_block2,
						//b_block3,b_block4,
						ci,zero_point_lhs,zero_point_rhs,acc);



				for (int j = 0; j < B_WIDTH_BLOCK; j++) {

					#pragma HLS UNROLL
					for (int z = 0; z < SPMM_BLOCK; z++)
					{
							#pragma HLS UNROLL
							if (i>=M_aux[z]&&i<M_aux[z+1])
									acc2[j][z] += acc[j];
					}//z loop
							//////std::cout << " compute2 acc with j " << j << "acc2[j] is " << acc2[j] << ////std::endl;
				}//j loop

					//////////std::cout << " compute1 acc with j " << j << "acc2[j] is " << acc2[j] << ////std::endl;


		     	} //i loop





		#endif

}

void dsp_kernel_wrapper_adj_1(int block_size,int M[SPMM_BLOCK],hls::stream<ATYPE> &A_fifo,hls::stream<int> &col_indices_fifo,ITYPE b_block1[B_HEIGHT][B_WIDTH_BLOCK],
		//ITYPE b_block2[B_HEIGHT/4][B_WIDTH_BLOCK],
		//ITYPE b_block3[B_HEIGHT/4][B_WIDTH_BLOCK],ITYPE b_block4[B_HEIGHT/4][B_WIDTH_BLOCK],
		ap_int<8> zero_point_lhs,ap_int<8> zero_point_rhs,ITYPE acc2[B_WIDTH_BLOCK][SPMM_BLOCK])
{



		#if defined FLOAT || defined HALF


	FTYPE acc_part[FADD_LATENCY_ADJ][B_WIDTH_BLOCK][SPMM_BLOCK];
	#pragma HLS ARRAY_PARTITION variable=acc_part complete dim=0 //partition all dimensions

	FTYPE acc_float[B_WIDTH_BLOCK];
        #pragma HLS ARRAY_PARTITION variable=acc_float complete



	for (int j = 0; j < B_WIDTH_BLOCK; j++) {

		#pragma HLS UNROLL

			acc_float[j] = 0;
	}





 		RESET_ACC_LOOP_SPMM: for (int j = 0; j < B_WIDTH_BLOCK; j++) {
			#pragma HLS UNROLL
				for (int l = 0; l < FADD_LATENCY_ADJ; l++) {
				#pragma HLS UNROLL
					for (int z = 0; z < SPMM_BLOCK; z++){
				    acc_part[l][j][z] = 0;
				}
			}
	 	}

        int BM = M[SPMM_BLOCK-1];

		 int M_aux[SPMM_BLOCK+1]; //store the different number of nonzeros intervals
		 M_aux[0] = 0;
	     for (int j = 1; j < SPMM_BLOCK+1; j++)
		 {
			#pragma HLS UNROLL
			M_aux[j] = M[j-1];
		 }
        //print

   		DSP_LOOP_SPMM: for(int k = 0; k < BM; k+=FADD_LATENCY_ADJ) {
			#pragma HLS PIPELINE II=FADD_LATENCY_ADJ rewind

			DSP_LOOP_SPMM2: for(int i = 0; i < FADD_LATENCY_ADJ; i++) {

				DTYPE v;
				int ci;
				if ((k+i) < BM) //avoid trying to read empty FIFO that only contains M elements
				{
					v = A_fifo.read();
					ci = col_indices_fifo.read();
				}
			        else
				{
					v=0;
					ci=0;
				}


				////////std::cout << " compute1 read col fifo" << ////std::endl;


				dsp_kernel_float_adj_1(v,b_block1,ci,zero_point_lhs,zero_point_rhs,acc_float);

			        for (int j = 0; j < B_WIDTH_BLOCK; j++) {
				  	//#pragma HLS UNROLL
					//#pragma HLS dependence variable=acc_val inter false
			        	for (int z = 0; z < SPMM_BLOCK; z++)
			            {
			        		#pragma HLS UNROLL
			        		if ((k+i)>=M_aux[z]&&(k+i)<M_aux[z+1])
			        			acc_part[i][j][z] += acc_float[j];
			            }
					#ifdef simulation
					if (acc_part[i][j] > max_adj)
								max_adj = acc_part[i][j];
							if (acc_part[i][j] < min_adj)
								min_adj = acc_part[i][j];
					#endif
				}

    			      } //i loop

		} // k loop

	ACC_PART1 : for (int j = 0; j < B_WIDTH_BLOCK; j++) {
            #pragma HLS UNROLL
		ACC_PART2 : for (int z = 0; z < SPMM_BLOCK; z++)
            {
				#pragma HLS UNROLL
			    ACC_PART3 : for (int l = 1; l < FADD_LATENCY_ADJ; l++) {
					#pragma HLS PIPELINE=1
	    		     acc_part[0][j][z] += acc_part[l][j][z];
		        }
			}
		}

	FLOAT_PART1 : for (int j = 0; j < B_WIDTH_BLOCK; j++) {
                #pragma HLS UNROLL
		FLOAT_PART2 : for (int z = 0; z < SPMM_BLOCK; z++)
		        {
					#pragma HLS UNROLL
				   FTYPE acc_part_float = acc_part[0][j][z];
				   acc2[j][z] = acc_part_float;
		        }
		}



	#endif

		#ifdef EIGHTBIT

       		ITYPE acc[B_WIDTH_BLOCK];
		#pragma HLS ARRAY_PARTITION variable=acc complete

		//for (int j = 0; j < B_WIDTH_BLOCK; j++) {

		//	#pragma HLS UNROLL

		//		acc2[j] = 0;
		//}


        int BM = M[SPMM_BLOCK-1];

		int M_aux[SPMM_BLOCK+1];
		M_aux[0] = 0;
		for (int j = 1; j < SPMM_BLOCK+1; j++)
		{
				#pragma HLS UNROLL
				M_aux[j] = M[j-1];
		}

		 DSP_LOOP_SPMM: for (int i = 0; i < BM; i+=1) {
 		 	 #pragma HLS PIPELINE
	        	 //#pragma HLS UNROLL factor=PARALLEL_ROW
    				DTYPE v = A_fifo.read();

				int ci = col_indices_fifo.read();

				dsp_kernel_int_adj_1(block_size,v,b_block1,//b_block2,
						//b_block3,b_block4,
						ci,zero_point_lhs,zero_point_rhs,acc);



				for (int j = 0; j < B_WIDTH_BLOCK; j++) {

					#pragma HLS UNROLL
					for (int z = 0; z < SPMM_BLOCK; z++)
					{
							#pragma HLS UNROLL
							if (i>=M_aux[z]&&i<M_aux[z+1])
									acc2[j][z] += acc[j];
					}//z loop
							//////std::cout << " compute2 acc with j " << j << "acc2[j] is " << acc2[j] << ////std::endl;
				}//j loop

					//////////std::cout << " compute1 acc with j " << j << "acc2[j] is " << acc2[j] << ////std::endl;


		     	} //i loop



		#endif

}


void dsp_kernel_wrapper_fea(bool gemm_mode,int M[SPMM_BLOCK],hls::stream<FTYPE> &A_fifo,hls::stream<int> &col_indices_fifo,BTYPE b_block[B_HEIGHT/4][B_WIDTH_BLOCK],ap_int<8> zero_point_lhs,ap_int<8> zero_point_rhs,ITYPE acc2[B_WIDTH_BLOCK][SPMM_BLOCK])
{



#if defined FLOAT || defined HALF

	//#pragma HLS INLINE

		ITYPE acc_part[FADD_LATENCY_FEA][B_WIDTH_BLOCK][SPMM_BLOCK];
		//#pragma HLS ARRAY_PARTITION variable=acc_part complete dim=0
        #pragma HLS ARRAY_PARTITION variable=acc_part complete dim=0 //partition all dimensions

		ITYPE acc_float[B_WIDTH_BLOCK];
	    #pragma HLS ARRAY_PARTITION variable=acc_float complete



		for (int j = 0; j < B_WIDTH_BLOCK; j++) {

			#pragma HLS UNROLL

				acc_float[j] = 0;
		}



	 		RESET_ACC_LOOP_SPMM: for (int j = 0; j < B_WIDTH_BLOCK; j++) {
				#pragma HLS UNROLL
					 for (int l = 0; l < FADD_LATENCY_FEA; l++) {
					 #pragma HLS UNROLL
			     		for (int z = 0; z < SPMM_BLOCK; z++){
							#pragma HLS UNROLL
			     			acc_part[l][j][z] = 0;
				}
			  }
		 	}

	         int BM = M[SPMM_BLOCK-1];

			 int M_aux[SPMM_BLOCK+1]; //store the different number of nonzeros intervals
			 M_aux[0] = 0;
		     for (int j = 1; j < SPMM_BLOCK+1; j++)
			 {
				#pragma HLS UNROLL
				M_aux[j] = M[j-1];
			 }
	         //print

	   		DSP_LOOP_SPMM: for(int k = 0; k < BM; k+=FADD_LATENCY_FEA) {
				//#pragma HLS PIPELINE II=FADD_LATENCY rewind
	   			#pragma HLS PIPELINE II=FADD_LATENCY_FEA

				DSP_LOOP_SPMM2: for(int i = 0; i < FADD_LATENCY_FEA; i++) {
					DTYPE v;
					int ci;
					if ((k+i) < BM) //avoid trying to read empty FIFO that only contains BM elements
					{
						v = A_fifo.read();

						//printf("A fifo read %f \n", v);
						//if (gemm_mode==0)
							ci = col_indices_fifo.read();
						//else
						//	ci = k+i;
					}
				        else
					{
						v=0;
						ci=0;
					}

					dsp_kernel_float_fea(v,b_block,ci,zero_point_lhs,zero_point_rhs,acc_float);

					SPMM_BLOCK_LOOP1 :for (int j = 0; j < B_WIDTH_BLOCK; j++) {
					  	#pragma HLS UNROLL
						//#pragma HLS dependence variable=acc_val inter false
				        	SPMM_BLOCK_LOOP2 : for (int z = 0; z < SPMM_BLOCK; z++)
				        	{
							#pragma HLS PIPELINE II=1
				        	if ((k+i)>=M_aux[z]&&(k+i)<M_aux[z+1])
				        			acc_part[i][j][z] += acc_float[j];
				        	}//z loop
					} //j loop


        	    } //i loop

			} // k loop

			ACC_PART1 : for (int j = 0; j < B_WIDTH_BLOCK; j++) {
	            #pragma HLS UNROLL
				ACC_PART2: for (int z = 0; z < SPMM_BLOCK; z++)
			    {
					#pragma HLS UNROLL
				    ACC_PART3 : for (int l = 1; l < FADD_LATENCY_FEA; l++)
				    {
					    #pragma HLS PIPELINE II=1
		    		     acc_part[0][j][z] += acc_part[l][j][z];
		            }
				}
			}


			ACC_PART_FLOAT1 :for (int j = 0; j < B_WIDTH_BLOCK; j++) {
	            #pragma HLS UNROLL
				ACC_PART_FLOAT2 : for (int z = 0; z < SPMM_BLOCK; z++)
			    {
				#pragma HLS UNROLL
				FTYPE acc_part_float = acc_part[0][j][z];
		    		acc2[j][z] = acc_part_float;
			    }

			}




		#endif

#ifdef EIGHTBIT

       	ITYPE acc[B_WIDTH_BLOCK];
		#pragma HLS ARRAY_PARTITION variable=acc complete

		//LOOP_ACC2_IN: for (int j = 0; j < B_WIDTH_BLOCK; j++) {
        //    #pragma HLS UNROLL
		//	for (int i = 0; i < SPMM_BLOCK; i++) {
		//		acc2[j][i] = 0;
		//	}
		//}

         //int BM = M[0]+M[1]+M[2]+M[3];
         int BM = M[SPMM_BLOCK-1];

		 int M_aux[SPMM_BLOCK+1]; //store the different number of nonzeros intervals
		 M_aux[0] = 0;
	     for (int j = 1; j < SPMM_BLOCK+1; j++)
		 {
			#pragma HLS UNROLL
			M_aux[j] = M[j-1];
		 }
         //printf("BM is %d\n",BM);

		 DSP_LOOP_SPMM: for (int i = 0; i < BM; i+=1) {
 		 	 #pragma HLS PIPELINE
	        	 //#pragma HLS UNROLL factor=PARALLEL_ROW

			    //std::cout << "A fifo/col indices " << std::endl;

    			FTYPE v = A_fifo.read();

    			int ci;
    			//if (gemm_mode==0)
    				ci = col_indices_fifo.read();
    			//else
    			//	ci = i;



			    //std::cout << "Done " << std::endl;

				dsp_kernel_int_fea(v,b_block,ci,zero_point_lhs,zero_point_rhs,acc);




				for (int j = 0; j < B_WIDTH_BLOCK; j++) {

					#pragma HLS UNROLL
					for (int z = 0; z < SPMM_BLOCK; z++)
					{
						#pragma HLS UNROLL
						if (i>=M_aux[z]&&i<M_aux[z+1])
							acc2[j][z] += acc[j];
					//else if (i>=M[0]&&i<M[1])
					//	acc2[j][1] += acc[j];
					//else if (i>=M[1]&&i<M[2])
					//	acc2[j][2] += acc[j];
					//else
					//	acc2[j][3] += acc[j];
					//////std::cout << " compute2 acc with j " << j << "acc2[j] is " << acc2[j] << ////std::endl;
        			}//z loop
				 }//j loop

		     	} //i loop




		#endif

}


void scale(ap_int<32> *quantized_multiplier, ap_int<32> *shift, ap_int<32> *bias, ap_int<8> zero_point_dst, ap_int<8> clamp_max,ap_int<8> clamp_min,int N, int M, int P, hls::stream<ITYPE> C_fifo[C_WIDTH_BLOCK],int B_index, int B_index_loop,int tail,hls::stream<ITYPE> write_fifo[C_WIDTH_BLOCK])
{


			int B_WIDTH_INT;
			if (B_index < (B_index_loop-1))
				B_WIDTH_INT = B_WIDTH_BLOCK;
			else
				B_WIDTH_INT = tail;



			#if defined FLOAT || defined HALF
			        ////////////std::cout << "Float scale " << ////std::endl;
				LOOP_CH1f:    for (int i = 0; i < N; i++) {
				 LOOP_CW1f: for (int j = 0; j < B_WIDTH_BLOCK; j++) {
					#pragma HLS PIPELINE II=1
					if (j<B_WIDTH_INT)
					{
						#ifdef ENABLE_SCALING
							int bias_int = bias[i];
							FTYPE bias_val_float=*(FTYPE*)&(bias_int);
							DTYPE C_fifo_int = C_fifo[j].read();
					        	FTYPE C_fifo_float=*(FTYPE*)&C_fifo_int;
							FTYPE zero_point_dst_float=(FTYPE)zero_point_dst; //simply cast to float
							FTYPE clamp_min_float=(FTYPE)clamp_min; //simply cast to float
							FTYPE clamp_max_float=(FTYPE)clamp_max; //simply cast to float
 							//////////std::cout << "C fifo float " << C_fifo_float << "bias_val_float " << bias_val_float << "zero_point_dst_float " << zero_point_dst_float << ////std::endl;
							//////////std::cout << "clamp max float " << clamp_max_float << "clamp min float " << clamp_min_float << ////std::endl;
							FTYPE C_temp_float = C_fifo_float + bias_val_float + zero_point_dst_float;
                         				if (C_temp_float < clamp_min_float) C_temp_float = clamp_min_float;
							if (C_temp_float > clamp_max_float) C_temp_float = clamp_max_float;
							////////////std::cout << "c temp float 2 " << C_temp_float << ////std::endl;
							DTYPE C_out = *(int*)&C_temp_float;
							//DTYPE C_out = (int)C_temp_float;
							write_fifo[j] << C_out;
						#else
							DTYPE C_fifo_int = C_fifo[j].read();
							////////std::cout << "C fifo float" << *(FTYPE*)&C_fifo_int << ////std::endl;
					        	write_fifo[j] << C_fifo_int;
						#endif

					}

				 }
			     }
			#endif


			#if defined EIGHTBIT
			    LOOP_CH1:    for (int i = 0; i < N; i+=4) {
				//#pragma HLS UNROLL factor=BLOCK/32
				//#pragma HLS UNROLL factor=2
				ap_int<32> bias_val[4];
				ap_int<32> shift_val[4];
				ap_int<32> mult_val[4];
				bias_val[0] =  bias[i];
				bias_val[1] =  bias[i+1];
				bias_val[2] =  bias[i+2];
				bias_val[3] =  bias[i+3];
				shift_val[0] = shift[i];
				shift_val[1] = shift[i+1];
				shift_val[2] = shift[i+2];
				shift_val[3] = shift[i+3];
				mult_val[0] = quantized_multiplier[i];
				mult_val[1] = quantized_multiplier[i+1];
				mult_val[2] = quantized_multiplier[i+2];
				mult_val[3] = quantized_multiplier[i+3];
				LOOP_CW1: for (int j = 0; j < B_WIDTH_BLOCK; j++) {
					#pragma HLS PIPELINE II=4
					//#pragma HLS UNROLL factor=2
					DTYPE C_out;
					LOOP_CH3:    for (int z = 0; z < 4; z++) {
					 #pragma HLS loop_tripcount min=1 max=1 avg=1
 						if (j<B_WIDTH_INT)
						{
							#ifdef ENABLE_SCALING
							ap_int<64> C_temp1;
							C_temp1 =  C_fifo[j].read() + bias_val[z];
							ap_int<32> total_shift1 = 31 - shift_val[z];
 		             				ap_int<64> round1 = (ap_int<64>)1 << (total_shift1 - 1);
							C_temp1 = C_temp1*mult_val[z] + round1;
							C_temp1 = (C_temp1 >> total_shift1) + zero_point_dst;
							ap_int<8> C_temp5 = C_temp1;
		                         		if (C_temp1 < clamp_min) C_temp5 = clamp_min;
							if (C_temp1 > clamp_max) C_temp5 = clamp_max;
							C_out = ((C_out >> 8) | ((int)C_temp5 << 24));

							if (z==3)
							{
								write_fifo[j].write(C_out);

							}
							#else
								C_out =  C_fifo[j].read();
								write_fifo[j].write(C_out);
							#endif

						}
					}

				    }
			}
		#endif
}


void compute2_4(bool relu,int block_size,ap_int<8> zero_point_lhs,  ap_int<8> zero_point_rhs, int first_row, int row_count,hls::stream<ATYPE> &A_fifo, hls::stream<int> &col_indices_fifo, hls::stream<int> rnnz_fifo[SPMM_BLOCK], ITYPE B_accel1[B_HEIGHT/2][B_WIDTH_BLOCK],ITYPE B_accel2[B_HEIGHT/2][B_WIDTH_BLOCK],
ITYPE B_accel3[B_HEIGHT/4][B_WIDTH_BLOCK],ITYPE B_accel4[B_HEIGHT/4][B_WIDTH_BLOCK],
hls::stream<ITYPE> C_fifo[B_WIDTH_BLOCK][SPMM_BLOCK],int B_index, int B_index_loop, int tail)
{


	DTYPE A_accel[A_WIDTH];
	        //#pragma HLS array_partition variable=A_accel cyclic factor=


		ITYPE acc[B_WIDTH_BLOCK];
		#pragma HLS ARRAY_PARTITION variable=acc complete


		ITYPE acc2[B_WIDTH_BLOCK][SPMM_BLOCK];
		#pragma HLS ARRAY_PARTITION variable=acc2 complete dim=0


		//hls::stream<int>             col_indices_fifo;
		//#pragma HLS STREAM variable=col_indices_fifo depth=1024 dim=1

		//int col_indices[A_WIDTH];


	      int B_WIDTH_INT;
	        ITYPE C_fifo_val;

	      if (B_index < (B_index_loop-1))
			B_WIDTH_INT = B_WIDTH_BLOCK;
	      else
			B_WIDTH_INT = tail;



	        //#pragma HLS DATAFLOW


		for (int A_index = 0; A_index < row_count; A_index+=SPMM_BLOCK) {


			//computing

			LOOP_ACC21: for (int j = 0; j < B_WIDTH_BLOCK; j++) {
				#pragma HLS UNROLL
				LOOP_ACC22 : for (int i = 0; i < SPMM_BLOCK; i++) {
					#pragma HLS UNROLL
					acc2[j][i] = 0;
				}
			}





			int rnnz[SPMM_BLOCK];
			int crows = 0;
			LOOP_RNNZ :for (int i = 0; i < SPMM_BLOCK; i++) {
	            #pragma HLS UNROLL
				rnnz[i] = rnnz_fifo[i].read();
				if ((A_index+i)<row_count)
				    crows++;

			}
			//printf("crows %d\n",crows);

	         //std::cout << "The rnnz value is " << rnnz << std::endl;

			dsp_kernel_wrapper_adj_4(block_size,rnnz,A_fifo,col_indices_fifo,B_accel1,
					B_accel2,
					B_accel3,B_accel4,
					zero_point_lhs,zero_point_rhs,acc2);


			LOOP_C_BUF1: for (int j = 0; j < B_WIDTH_BLOCK; j++) {
	                #pragma HLS UNROLL
					#if (USE_TAIL == 1)
					if (j < B_WIDTH_INT)
					#endif
					{
						#ifdef simulation
						if (acc2[j] < acc2_adj_min)
							acc2_adj_min = acc2[j];
						else if (acc2[j] > acc2_adj_max)
							acc2_adj_max = acc2[j];
						#endif
						LOOP_C_BUF2 : for (int i = 0; i < SPMM_BLOCK; i++) {
							#pragma HLS UNROLL
							if (i<crows)
							{
								#if (USE_SBLOCKS == 1)
									C_fifo[j][i].write(acc2[j][i]);
								#endif
								#if (USE_SBLOCKS == 0)
									 if (acc2[j][i] > 0 || relu == 0)
										C_fifo_val = acc2[j][i];
									else
										C_fifo_val = 0.0;
									C_fifo[j][0].write(C_fifo_val);
								#endif
						         }  
						}
						////std::cout << "C_fifo " << acc2[j] << std::endl;

						//C_fifo[A_index][j]=acc2[j];
					}
			}


	          } // A_index loop


}



void compute2_2(int block_size,ap_int<8> zero_point_lhs,  ap_int<8> zero_point_rhs, int first_row, int row_count,hls::stream<ATYPE> &A_fifo, hls::stream<int> &col_indices_fifo, hls::stream<int> rnnz_fifo[SPMM_BLOCK], ITYPE B_accel1[B_HEIGHT/2][B_WIDTH_BLOCK],ITYPE B_accel2[B_HEIGHT/2][B_WIDTH_BLOCK],
		//ITYPE B_accel3[B_HEIGHT/4][B_WIDTH_BLOCK],ITYPE B_accel4[B_HEIGHT/4][B_WIDTH_BLOCK],
hls::stream<ITYPE> C_fifo[B_WIDTH_BLOCK][SPMM_BLOCK],int B_index, int B_index_loop, int tail)
{


	DTYPE A_accel[A_WIDTH];
	        //#pragma HLS array_partition variable=A_accel cyclic factor=


		ITYPE acc[B_WIDTH_BLOCK];
		#pragma HLS ARRAY_PARTITION variable=acc complete


		ITYPE acc2[B_WIDTH_BLOCK][SPMM_BLOCK];
		#pragma HLS ARRAY_PARTITION variable=acc2 complete dim=0


		//hls::stream<int>             col_indices_fifo;
		//#pragma HLS STREAM variable=col_indices_fifo depth=1024 dim=1

		//int col_indices[A_WIDTH];


	      int B_WIDTH_INT;

	      if (B_index < (B_index_loop-1))
			B_WIDTH_INT = B_WIDTH_BLOCK;
	      else
			B_WIDTH_INT = tail;



	        //#pragma HLS DATAFLOW


		for (int A_index = 0; A_index < row_count; A_index+=SPMM_BLOCK) {


			//computing

			LOOP_ACC21: for (int j = 0; j < B_WIDTH_BLOCK; j++) {
				#pragma HLS UNROLL
				LOOP_ACC22 : for (int i = 0; i < SPMM_BLOCK; i++) {
					#pragma HLS UNROLL
					acc2[j][i] = 0;
				}
			}





			int rnnz[SPMM_BLOCK];
			int crows = 0;
			LOOP_RNNZ :for (int i = 0; i < SPMM_BLOCK; i++) {
	            #pragma HLS UNROLL
				rnnz[i] = rnnz_fifo[i].read();
				if ((A_index+i)<row_count)
				    crows++;

			}
			//printf("crows %d\n",crows);

	         //std::cout << "The rnnz value is " << rnnz << std::endl;

			dsp_kernel_wrapper_adj_2(block_size,rnnz,A_fifo,col_indices_fifo,B_accel1,
					B_accel2,
					//B_accel3,B_accel4,
					zero_point_lhs,zero_point_rhs,acc2);


			LOOP_C_BUF1: for (int j = 0; j < B_WIDTH_BLOCK; j++) {
	                #pragma HLS UNROLL
					#if (USE_TAIL == 1)
					if (j < B_WIDTH_INT)
					#endif
					{
						#ifdef simulation
						if (acc2[j] < acc2_adj_min)
							acc2_adj_min = acc2[j];
						else if (acc2[j] > acc2_adj_max)
							acc2_adj_max = acc2[j];
						#endif
						LOOP_C_BUF2 : for (int i = 0; i < SPMM_BLOCK; i++) {
							#pragma HLS UNROLL
							if (i<crows)
							#if (USE_SBLOCKS == 1)
								C_fifo[j][i].write(acc2[j][i]);
							#endif
							#if (USE_SBLOCKS == 0)
								C_fifo[j][0].write(acc2[j][i]);
							#endif
						}
						////std::cout << "C_fifo " << acc2[j] << std::endl;

						//C_fifo[A_index][j]=acc2[j];
					}
			}


	          } // A_index loop


}

void compute2_1(bool relu,int block_size,ap_int<8> zero_point_lhs,  ap_int<8> zero_point_rhs, int first_row, int row_count,hls::stream<ATYPE> &A_fifo, hls::stream<int> &col_indices_fifo, hls::stream<int> rnnz_fifo[SPMM_BLOCK], ITYPE B_accel1[B_HEIGHT/2][B_WIDTH_BLOCK],
		//ITYPE B_accel2[B_HEIGHT/2][B_WIDTH_BLOCK],
		//ITYPE B_accel3[B_HEIGHT/4][B_WIDTH_BLOCK],ITYPE B_accel4[B_HEIGHT/4][B_WIDTH_BLOCK],
hls::stream<ITYPE> C_fifo[B_WIDTH_BLOCK][SPMM_BLOCK],int B_index, int B_index_loop, int tail)
{


       //#pragma HLS allocation function instances=dsp_kernel limit=1



        //hls::stream<DTYPE>       A_accel;
        //#pragma HLS STREAM variable=A_accel depth=A_WIDTH_FIFO dim=1

	DTYPE A_accel[A_WIDTH];
        //#pragma HLS array_partition variable=A_accel cyclic factor=


	ITYPE acc[B_WIDTH_BLOCK];
	#pragma HLS ARRAY_PARTITION variable=acc complete


	ITYPE acc2[B_WIDTH_BLOCK][SPMM_BLOCK];
	#pragma HLS ARRAY_PARTITION variable=acc2 complete dim=0


	//hls::stream<int>             col_indices_fifo;
	//#pragma HLS STREAM variable=col_indices_fifo depth=1024 dim=1

	//int col_indices[A_WIDTH];


      int B_WIDTH_INT;
      ITYPE C_fifo_val;

      if (B_index < (B_index_loop-1))
		B_WIDTH_INT = B_WIDTH_BLOCK;
      else
		B_WIDTH_INT = tail;



        //#pragma HLS DATAFLOW


	for (int A_index = 0; A_index < row_count; A_index+=SPMM_BLOCK) {


		//computing

		LOOP_ACC21: for (int j = 0; j < B_WIDTH_BLOCK; j++) {
			#pragma HLS UNROLL
			LOOP_ACC22 : for (int i = 0; i < SPMM_BLOCK; i++) {
				#pragma HLS UNROLL
				acc2[j][i] = 0;
			}
		}





		int rnnz[SPMM_BLOCK];
		int crows = 0;
		LOOP_RNNZ :for (int i = 0; i < SPMM_BLOCK; i++) {
            #pragma HLS UNROLL
			rnnz[i] = rnnz_fifo[i].read();
			if ((A_index+i)<row_count)
			    crows++;

		}
		//printf("crows %d\n",crows);

         //std::cout << "The rnnz value is " << rnnz << std::endl;

		dsp_kernel_wrapper_adj_1(block_size,rnnz,A_fifo,col_indices_fifo,B_accel1,
				//B_accel2,
				//B_accel3,B_accel4,
				zero_point_lhs,zero_point_rhs,acc2);


		LOOP_C_BUF1: for (int j = 0; j < B_WIDTH_BLOCK; j++) {
                #pragma HLS UNROLL
				#if (USE_TAIL == 1)
				if (j < B_WIDTH_INT)
				#endif
				{
					#ifdef simulation
					if (acc2[j] < acc2_adj_min)
						acc2_adj_min = acc2[j];
					else if (acc2[j] > acc2_adj_max)
						acc2_adj_max = acc2[j];
					#endif


						LOOP_C_BUF2 : for (int i = 0; i < SPMM_BLOCK; i++) {
							#pragma HLS UNROLL
							if (i<crows)
							{
								#if (USE_SBLOCKS == 1)
									C_fifo[j][i].write(acc2[j][i]);
								#endif
								#if (USE_SBLOCKS == 0)
								 if (acc2[j][i] > 0 || relu == 0)
									C_fifo_val = acc2[j][i];
								 else
									C_fifo_val = 0.0;
								 C_fifo[j][0].write(C_fifo_val);
								#endif
							}
					}
					////std::cout << "C_fifo " << acc2[j] << std::endl;

					//C_fifo[A_index][j]=acc2[j];
				}
		}


          } // A_index loop

}

void compute1_1(bool gemm_mode,ap_int<8> zero_point_lhs,  ap_int<8> zero_point_rhs, int first_row, int row_count,hls::stream<FTYPE> &A_fifo, hls::stream<int> &col_indices_fifo, hls::stream<int> rnnz_fifo[SPMM_BLOCK],BTYPE B_accel[B_HEIGHT][B_WIDTH_BLOCK],ITYPE C_buf1[B_HEIGHT][B_WIDTH_BLOCK],
		//ITYPE C_buf2[B_HEIGHT/4][B_WIDTH_BLOCK],
		//ITYPE C_buf3[B_HEIGHT/4][B_WIDTH_BLOCK],ITYPE C_buf4[B_HEIGHT/4][B_WIDTH_BLOCK],
		int B_index,int B_index_loop, int tail)
{


       //#pragma HLS allocation function instances=dsp_kernel limit=1


        //hls::stream<DTYPE>       A_accel;
        //#pragma HLS STREAM variable=A_accel depth=A_WIDTH_FIFO dim=1

	//DTYPE A_accel[A_WIDTH];
        //#pragma HLS array_partition variable=A_accel cyclic factor=


	ITYPE acc[B_WIDTH_BLOCK];
	#pragma HLS ARRAY_PARTITION variable=acc complete

  	//BTYPE B_accel[B_HEIGHT][B_WIDTH_BLOCK];
      	//#pragma HLS array_partition variable=B_accel block factor= BLOCK/2 dim=2


	ITYPE acc2[B_WIDTH_BLOCK][SPMM_BLOCK];
	#pragma HLS ARRAY_PARTITION variable=acc2 complete dim=0 //all dimensions are partitioned


	//hls::stream<int>             col_indices_fifo;
	//#pragma HLS STREAM variable=col_indices_fifo depth=1024 dim=1

	//int col_indices[A_WIDTH];


      int B_WIDTH_INT;

      //for (int B_index = 0; B_index < B_index_loop; B_index++) {

      if (B_index < (B_index_loop-1))
      		B_WIDTH_INT = B_WIDTH_BLOCK;
      else
      		B_WIDTH_INT = tail;



        //#pragma HLS DATAFLOW


        //std::cout << " row_count " << row_count << std::endl;

	for (int A_index = 0; A_index < row_count; A_index+=SPMM_BLOCK) {


		//computing

		LOOP_ACC21 :for (int j = 0; j < C_WIDTH_BLOCK; j++) {
            #pragma HLS UNROLL
			LOOP_ACC22 : for (int i = 0; i < SPMM_BLOCK; i++) {
               #pragma HLS UNROLL
				acc2[j][i] = 0;
			}
		}

		int rnnz[SPMM_BLOCK];
		LOOP_RNNZ :for (int i = 0; i < SPMM_BLOCK; i++) {
            #pragma HLS UNROLL
			//if ((A_index+i)<row_count)
				rnnz[i] = rnnz_fifo[i].read();
			//else
				//rnnz[i] = 0;
		}

  		//std::cout << " rnnz is " << rnnz << std::endl;

		dsp_kernel_wrapper_fea(gemm_mode,rnnz,A_fifo,col_indices_fifo,B_accel,zero_point_lhs,zero_point_rhs,acc2);

		LOOP_C_BUF1 : for (int j = 0; j < C_WIDTH_BLOCK; j++) {
			//#pragma HLS loop_tripcount min=16 max=16 avg=16
	           	#pragma HLS UNROLL
			#if (USE_TAIL == 1)
			if (j < B_WIDTH_INT)
			#endif
			{
				//C_fifo[j].write(acc2[j]);
				#ifdef simulation
				if (acc2[j] < acc2_fea_min)
					acc2_fea_min = acc2[j];
				else if (acc2[j] > acc2_fea_max)
					acc2_fea_max = acc2[j];
				#endif
				LOOP_C_BUF2 : for (int i = 0; i < SPMM_BLOCK; i++) {
	                //critical #pragma HLS UNROLL
					#pragma HLS UNROLL
					C_buf1[A_index+i][j]=acc2[j][i];
				}
				//C_buf2[A_index][j]=acc2[j];
				//C_buf3[A_index][j]=acc2[j];
				//C_buf4[A_index][j]=acc2[j];

			}
		   }




          } // A_index loop
	//}
}




void compute1_2(bool gemm_mode,ap_int<8> zero_point_lhs,  ap_int<8> zero_point_rhs, int first_row, int row_count,hls::stream<FTYPE> &A_fifo, hls::stream<int> &col_indices_fifo, hls::stream<int> rnnz_fifo[SPMM_BLOCK],BTYPE B_accel[B_HEIGHT][B_WIDTH_BLOCK],ITYPE C_buf1[B_HEIGHT/4][B_WIDTH_BLOCK],ITYPE C_buf2[B_HEIGHT/4][B_WIDTH_BLOCK],
		//ITYPE C_buf3[B_HEIGHT/4][B_WIDTH_BLOCK],ITYPE C_buf4[B_HEIGHT/4][B_WIDTH_BLOCK],
		int B_index,int B_index_loop, int tail)
{

    //#pragma HLS allocation function instances=dsp_kernel limit=1


       //hls::stream<DTYPE>       A_accel;
       //#pragma HLS STREAM variable=A_accel depth=A_WIDTH_FIFO dim=1

	//DTYPE A_accel[A_WIDTH];
       //#pragma HLS array_partition variable=A_accel cyclic factor=


	ITYPE acc[B_WIDTH_BLOCK];
	#pragma HLS ARRAY_PARTITION variable=acc complete

 	//BTYPE B_accel[B_HEIGHT][B_WIDTH_BLOCK];
     	//#pragma HLS array_partition variable=B_accel block factor= BLOCK/2 dim=2


	ITYPE acc2[B_WIDTH_BLOCK][SPMM_BLOCK];
	#pragma HLS ARRAY_PARTITION variable=acc2 complete dim=0 //all dimensions are partitioned


	//hls::stream<int>             col_indices_fifo;
	//#pragma HLS STREAM variable=col_indices_fifo depth=1024 dim=1

	//int col_indices[A_WIDTH];


     int B_WIDTH_INT;

     //for (int B_index = 0; B_index < B_index_loop; B_index++) {

     if (B_index < (B_index_loop-1))
     		B_WIDTH_INT = B_WIDTH_BLOCK;
     else
     		B_WIDTH_INT = tail;



       //#pragma HLS DATAFLOW


       //std::cout << " row_count " << row_count << std::endl;

	for (int A_index = 0; A_index < row_count; A_index+=SPMM_BLOCK) {


		//computing

		LOOP_ACC21 :for (int j = 0; j < C_WIDTH_BLOCK; j++) {
           #pragma HLS UNROLL
			LOOP_ACC22 : for (int i = 0; i < SPMM_BLOCK; i++) {
              #pragma HLS UNROLL
				acc2[j][i] = 0;
			}
		}

		int rnnz[SPMM_BLOCK];
		LOOP_RNNZ :for (int i = 0; i < SPMM_BLOCK; i++) {
           #pragma HLS UNROLL
			//if ((A_index+i)<row_count)
				rnnz[i] = rnnz_fifo[i].read();
			//else
				//rnnz[i] = 0;
		}

 		//std::cout << " rnnz is " << rnnz << std::endl;

		dsp_kernel_wrapper_fea(gemm_mode,rnnz,A_fifo,col_indices_fifo,B_accel,zero_point_lhs,zero_point_rhs,acc2);

		LOOP_C_BUF1 : for (int j = 0; j < C_WIDTH_BLOCK; j++) {
			//#pragma HLS loop_tripcount min=16 max=16 avg=16
	           	#pragma HLS UNROLL
			#if (USE_TAIL == 1)
			if (j < B_WIDTH_INT)
			#endif
			{
				//C_fifo[j].write(acc2[j]);
				#ifdef simulation
				if (acc2[j] < acc2_fea_min)
					acc2_fea_min = acc2[j];
				else if (acc2[j] > acc2_fea_max)
					acc2_fea_max = acc2[j];
				#endif
				LOOP_C_BUF2 : for (int i = 0; i < SPMM_BLOCK; i++) {
	                #pragma HLS UNROLL
					C_buf1[A_index+i][j]=acc2[j][i];
					C_buf2[A_index+i][j]=acc2[j][i];
				}
				//C_buf3[A_index][j]=acc2[j];
				//C_buf4[A_index][j]=acc2[j];

			}
		   }




         } // A_index loop
	//}
}



void compute1_4(bool gemm_mode,ap_int<8> zero_point_lhs,  ap_int<8> zero_point_rhs, int first_row, int row_count,hls::stream<FTYPE> &A_fifo, hls::stream<int> &col_indices_fifo, hls::stream<int> rnnz_fifo[SPMM_BLOCK],BTYPE B_accel[B_HEIGHT][B_WIDTH_BLOCK],ITYPE C_buf1[B_HEIGHT/4][B_WIDTH_BLOCK],ITYPE C_buf2[B_HEIGHT/4][B_WIDTH_BLOCK],ITYPE C_buf3[B_HEIGHT/4][B_WIDTH_BLOCK],ITYPE C_buf4[B_HEIGHT/4][B_WIDTH_BLOCK],int B_index,int B_index_loop, int tail)
{

    //#pragma HLS allocation function instances=dsp_kernel limit=1


       //hls::stream<DTYPE>       A_accel;
       //#pragma HLS STREAM variable=A_accel depth=A_WIDTH_FIFO dim=1

	//DTYPE A_accel[A_WIDTH];
       //#pragma HLS array_partition variable=A_accel cyclic factor=


	ITYPE acc[B_WIDTH_BLOCK];
	#pragma HLS ARRAY_PARTITION variable=acc complete

 	//BTYPE B_accel[B_HEIGHT][B_WIDTH_BLOCK];
     	//#pragma HLS array_partition variable=B_accel block factor= BLOCK/2 dim=2


	ITYPE acc2[B_WIDTH_BLOCK][SPMM_BLOCK];
	#pragma HLS ARRAY_PARTITION variable=acc2 complete dim=0 //all dimensions are partitioned


	//hls::stream<int>             col_indices_fifo;
	//#pragma HLS STREAM variable=col_indices_fifo depth=1024 dim=1

	//int col_indices[A_WIDTH];


     int B_WIDTH_INT;

     //for (int B_index = 0; B_index < B_index_loop; B_index++) {

     if (B_index < (B_index_loop-1))
     		B_WIDTH_INT = B_WIDTH_BLOCK;
     else
     		B_WIDTH_INT = tail;



       //#pragma HLS DATAFLOW


       //std::cout << " row_count " << row_count << std::endl;

	for (int A_index = 0; A_index < row_count; A_index+=SPMM_BLOCK) {


		//computing

		LOOP_ACC21 :for (int j = 0; j < C_WIDTH_BLOCK; j++) {
           #pragma HLS UNROLL
			LOOP_ACC22 : for (int i = 0; i < SPMM_BLOCK; i++) {
              #pragma HLS UNROLL
				acc2[j][i] = 0;
			}
		}

		int rnnz[SPMM_BLOCK];
		LOOP_RNNZ :for (int i = 0; i < SPMM_BLOCK; i++) {
           #pragma HLS UNROLL
			//if ((A_index+i)<row_count)
				rnnz[i] = rnnz_fifo[i].read();
			//else
				//rnnz[i] = 0;
		}

 		//std::cout << " rnnz is " << rnnz << std::endl;

		dsp_kernel_wrapper_fea(gemm_mode,rnnz,A_fifo,col_indices_fifo,B_accel,zero_point_lhs,zero_point_rhs,acc2);

		LOOP_C_BUF1 : for (int j = 0; j < C_WIDTH_BLOCK; j++) {
			//#pragma HLS loop_tripcount min=16 max=16 avg=16
	           	#pragma HLS UNROLL
			#if (USE_TAIL == 1)
			if (j < B_WIDTH_INT)
			#endif
			{
				//C_fifo[j].write(acc2[j]);
				#ifdef simulation
				if (acc2[j] < acc2_fea_min)
					acc2_fea_min = acc2[j];
				else if (acc2[j] > acc2_fea_max)
					acc2_fea_max = acc2[j];
				#endif
				LOOP_C_BUF2 : for (int i = 0; i < SPMM_BLOCK; i++) {
	                //critical #pragma HLS UNROLL
					C_buf1[A_index+i][j]=acc2[j][i];
					C_buf2[A_index+i][j]=acc2[j][i];
					C_buf3[A_index+i][j]=acc2[j][i];
					C_buf4[A_index+i][j]=acc2[j][i];
				}


			}
		   }




         } // A_index loop


}


void loop_fea(bool gemm_mode,int *rowPtr_fea1,int *rowPtr_fea2,int *rowPtr_fea3,int *rowPtr_fea4,
	int *columnIndex_fea1,int *columnIndex_fea2,int *columnIndex_fea3,int *columnIndex_fea4,
	FTYPE *values_fea1,FTYPE *values_fea2,FTYPE *values_fea3,FTYPE *values_fea4,
	BTYPE* B,
	int N_fea, int M_fea,ap_int<8> zero_point_lhs,  ap_int<8> zero_point_rhs,
	hls::stream_of_blocks<buf> &C_buffer11,hls::stream_of_blocks<buf> &C_buffer12,
	hls::stream_of_blocks<buf> &C_buffer13,hls::stream_of_blocks<buf> &C_buffer14,
	hls::stream_of_blocks<buf> &C_buffer21,hls::stream_of_blocks<buf> &C_buffer22,
	hls::stream_of_blocks<buf> &C_buffer23,hls::stream_of_blocks<buf> &C_buffer24,
	hls::stream_of_blocks<buf> &C_buffer31,hls::stream_of_blocks<buf> &C_buffer32,
	hls::stream_of_blocks<buf> &C_buffer33,hls::stream_of_blocks<buf> &C_buffer34,
	hls::stream_of_blocks<buf> &C_buffer41,hls::stream_of_blocks<buf> &C_buffer42,
	hls::stream_of_blocks<buf> &C_buffer43,hls::stream_of_blocks<buf> &C_buffer44,
	int B_index_loop, int tail)
{


     BTYPE B_accel1[B_HEIGHT][B_WIDTH_BLOCK];
     #pragma HLS array_partition variable=B_accel1 block factor= BLOCK/2 dim=2
     BTYPE B_accel2[B_HEIGHT][B_WIDTH_BLOCK];
     #pragma HLS array_partition variable=B_accel2 block factor= BLOCK/2 dim=2
     BTYPE B_accel3[B_HEIGHT][B_WIDTH_BLOCK];
     #pragma HLS array_partition variable=B_accel3 block factor= BLOCK/2 dim=2
     BTYPE B_accel4[B_HEIGHT][B_WIDTH_BLOCK];
     #pragma HLS array_partition variable=B_accel4 block factor= BLOCK/2 dim=2

	 hls::stream<int> rnnz_fifo_fea1[SPMM_BLOCK];
	 #pragma HLS STREAM variable=rnnz_fifo_fea1 depth=FIFO_DEPTH dim=1
	 hls::stream<int> rnnz_fifo_fea2[SPMM_BLOCK];
	 #pragma HLS STREAM variable=rnnz_fifo_fea2 depth=FIFO_DEPTH dim=1
	 hls::stream<int> rnnz_fifo_fea3[SPMM_BLOCK];
	 #pragma HLS STREAM variable=rnnz_fifo_fea3 depth=FIFO_DEPTH dim=1
	 hls::stream<int> rnnz_fifo_fea4[SPMM_BLOCK];
	 #pragma HLS STREAM variable=rnnz_fifo_fea4 depth=FIFO_DEPTH dim=1


	 hls::stream<FTYPE> A_fifo_fea1;
	 #pragma HLS STREAM variable=A_fifo_fea1 depth=FIFO_DEPTH dim=1
	 hls::stream<FTYPE> A_fifo_fea2;
	 #pragma HLS STREAM variable=A_fifo_fea2 depth=FIFO_DEPTH dim=1
	 hls::stream<FTYPE> A_fifo_fea3;
	 #pragma HLS STREAM variable=A_fifo_fea3 depth=FIFO_DEPTH dim=1
	 hls::stream<FTYPE> A_fifo_fea4;
	 #pragma HLS STREAM variable=A_fifo_fea4 depth=FIFO_DEPTH dim=1

	 hls::stream<FTYPE> A_fifo_fea1_out;
	 #pragma HLS STREAM variable=A_fifo_fea1 depth=FIFO_DEPTH dim=1

	 hls::stream<bool> exit_loop;
	 #pragma HLS STREAM variable=exit_loop depth=FIFO_DEPTH dim=1

	 hls::stream<int>  col_indices_fifo_fea1;
	 #pragma HLS STREAM variable=col_indices_fifo_fea1 depth=FIFO_DEPTH dim=1
	 hls::stream<int>  col_indices_fifo_fea2;
	 #pragma HLS STREAM variable=col_indices_fifo_fea2 depth=FIFO_DEPTH dim=1
	 hls::stream<int>  col_indices_fifo_fea3;
	 #pragma HLS STREAM variable=col_indices_fifo_fea3 depth=FIFO_DEPTH dim=1
	 hls::stream<int>  col_indices_fifo_fea4;
	 #pragma HLS STREAM variable=col_indices_fifo_fea4 depth=FIFO_DEPTH dim=1

	 int B_WIDTH_INT;


	LOOP_FEA : for (int B_index = 0; B_index < B_index_loop; B_index++) {

    	#pragma HLS DATAFLOW

	      if (B_index < (B_index_loop-1))
			B_WIDTH_INT = B_WIDTH_BLOCK;
	      else
			B_WIDTH_INT = tail;


	    //if (mode == 0) //GEMM
		//	a_values = N*M;
		//else //SPMM
		//	a_values = nnz_fea;



		/*these are the weights*/

		#if FEA_THREADS == 1

	  	/*these are the weights*/

	  	    hls::write_lock<buf> C_fea11(C_buffer11);
	  	    //hls::write_lock<buf> C_fea12(C_buffer12);
	  	    //hls::write_lock<buf> C_fea13(C_buffer13);
	  	    //hls::write_lock<buf> C_fea14(C_buffer14);
	  	    //hls::write_lock<buf> C_fea21(C_buffer21);
	  	    //hls::write_lock<buf> C_fea22(C_buffer22);
	  	    //hls::write_lock<buf> C_fea23(C_buffer23);
	  	    //hls::write_lock<buf> C_fea24(C_buffer24);
	  	    //hls::write_lock<buf> C_fea31(C_buffer31);
	  	    //hls::write_lock<buf> C_fea32(C_buffer32);
	  	    //hls::write_lock<buf> C_fea33(C_buffer33);
	  	    //hls::write_lock<buf> C_fea34(C_buffer34);
	  	    //hls::write_lock<buf> C_fea41(C_buffer41);
	  	    //hls::write_lock<buf> C_fea42(C_buffer42);
	  	    //hls::write_lock<buf> C_fea43(C_buffer43);
	  	    //hls::write_lock<buf> C_fea44(C_buffer44);


	  	    //std::cout << "Loop FEA " << std::endl;

	  		for (int j = 0; j < B_WIDTH_INT; j++) {
	  			        LOOP_BLOCKB : for (int i = 0; i < M_fea; i++) {
	  						    //#pragma HLS loop_tripcount min=84 max=84 avg=84
	  							#pragma HLS PIPELINE
	  							//#pragma HLS loop_tripcount min=16 max=16 avg=16
	  			        		BTYPE B_accel_temp = B[i+j*M_fea+B_index*B_WIDTH_BLOCK*M_fea];
	  			        		B_accel1[i][j] = B_accel_temp;
	  			        		//B_accel2[i][j] = B_accel_temp;
	  			        		//B_accel3[i][j] = B_accel_temp;
	  			        		//B_accel4[i][j] = B_accel_temp;

	  							////std::cout << " " << i << " " << j << " " << B_accel[i][j]  << std::endl;
	  						}
	  		}



	  		  // read sparse matrices

	  	          ////std::cout << "reada " << //std::endl;


	                int first_row1,first_row2,first_row3,first_row4;
	                int row_count1,row_count2,row_count3,row_count4;

	                int N_fea_block = N_fea;
	  			  int N_fea_rest = 0;
	  		      row_count1 = N_fea_block;
	  		      //row_count2 = N_fea_block;
	  		      //row_count3 = N_fea_block;
	  		      //row_count2 = N_fea_block+N_fea_rest;
	  		      first_row1 = 0;
	  		      //first_row2 = N_fea_block;
	  		      //first_row3 = 2*N_fea_block;
	  		      //first_row4 = 3*N_fea_block;

	  			    //std::cout << "READA1 " << std::endl;

	  	          //reada1(exit_loop,first_row1,row_count1,A_fifo_fea1,col_indices_fifo_fea1,rnnz_fifo_fea1,B_index_loop,tail,
	  		      //  rowPtr_fea1,columnIndex_fea1,values_fea1);
	  	          reada1(gemm_mode,M_fea,first_row1,row_count1,A_fifo_fea1,col_indices_fifo_fea1,rnnz_fifo_fea1,B_index_loop,tail,
	  		        rowPtr_fea1,columnIndex_fea1,values_fea1);
	  	          //reada1(first_row2,row_count2,A_fifo_fea2,col_indices_fifo_fea2,rnnz_fifo_fea2,B_index_loop,tail,
	  	            //rowPtr_fea2,columnIndex_fea2,values_fea2);
	  	          //reada1(first_row3,row_count3,A_fifo_fea3,col_indices_fifo_fea3,rnnz_fifo_fea3,B_index_loop,tail,
	  	          //  rowPtr_fea3,columnIndex_fea3,values_fea3);
	  	          //reada1(first_row4,row_count4,A_fifo_fea4,col_indices_fifo_fea4,rnnz_fifo_fea4,B_index_loop,tail,
	  	          //  rowPtr_fea4,columnIndex_fea4,values_fea4);


	  		      //check_fifo_0(105165, A_fifo_fea1, A_fifo_fea1_out);

	  	          // inputs A_fifo_fea, col_indices_fifo_fea, rnnz_fifo_fea   and B_accel

	  		  //outputs C_buffer

	  	          //compute1 FEA * W = C

	  	          ////std::cout << "compute1" << //std::endl;


	  	  	    //std::cout << "COMPUTE1 " << std::endl;

	  		  compute1_1(gemm_mode,zero_point_lhs,  zero_point_rhs, first_row1,row_count1,A_fifo_fea1, col_indices_fifo_fea1, rnnz_fifo_fea1,B_accel1,C_fea11,
	  				  //C_fea12,
	  				  //C_fea13, C_fea14,
	  				  B_index, B_index_loop, tail);







		#endif

#if FEA_THREADS == 2



	   hls::write_lock<buf> C_fea11(C_buffer11);
		#if ADJ_THREADS == 2
		    hls::write_lock<buf> C_fea12(C_buffer12);
		    hls::write_lock<buf> C_fea22(C_buffer22);
		#endif
		    //hls::write_lock<buf> C_fea13(C_buffer13);
		    //hls::write_lock<buf> C_fea14(C_buffer14);
		    hls::write_lock<buf> C_fea21(C_buffer21);

		    //hls::write_lock<buf> C_fea23(C_buffer23);
		    //hls::write_lock<buf> C_fea24(C_buffer24);
		    //hls::write_lock<buf> C_fea31(C_buffer31);
		    //hls::write_lock<buf> C_fea32(C_buffer32);
		    //hls::write_lock<buf> C_fea33(C_buffer33);
		    //hls::write_lock<buf> C_fea34(C_buffer34);
		    //hls::write_lock<buf> C_fea41(C_buffer41);
		    //hls::write_lock<buf> C_fea42(C_buffer42);
		    //hls::write_lock<buf> C_fea43(C_buffer43);
		    //hls::write_lock<buf> C_fea44(C_buffer44);


		    //std::cout << "Loop FEA " << std::endl;

			for (int j = 0; j < B_WIDTH_INT; j++) {
				        LOOP_BLOCKB : for (int i = 0; i < M_fea; i++) {
							    //#pragma HLS loop_tripcount min=84 max=84 avg=84
								#pragma HLS PIPELINE
								//#pragma HLS loop_tripcount min=16 max=16 avg=16
				        		BTYPE B_accel_temp = B[i+j*M_fea+B_index*B_WIDTH_BLOCK*M_fea];
				        		B_accel1[i][j] = B_accel_temp;
				        		B_accel2[i][j] = B_accel_temp;
				        		//B_accel3[i][j] = B_accel_temp;
				        		//B_accel4[i][j] = B_accel_temp;

								////std::cout << " " << i << " " << j << " " << B_accel[i][j]  << std::endl;
							}
			}

			  int first_row1,first_row2,first_row3,first_row4;
			  int row_count1,row_count2,row_count3,row_count4;

      	  	  int N_fea_block = N_fea/2;
			  int N_fea_rest = N_fea%2;
		      row_count1 = N_fea_block;
		      row_count2 = N_fea_block+N_fea_rest;
		      first_row1 = 0;
		      first_row2 = N_fea_block;

              std::cout << "Thread fea 1" << std::endl;
	          reada1(gemm_mode,M_fea,first_row1,row_count1,A_fifo_fea1,col_indices_fifo_fea1,rnnz_fifo_fea1,B_index_loop,tail,
		        rowPtr_fea1,columnIndex_fea1,values_fea1);
	          std::cout << "Thread fea 2" << std::endl;
	          reada1(gemm_mode,M_fea,first_row2,row_count2,A_fifo_fea2,col_indices_fifo_fea2,rnnz_fifo_fea2,B_index_loop,tail,
	            rowPtr_fea2,columnIndex_fea2,values_fea2);

	#if ADJ_THREADS == 2
		  compute1_2(gemm_mode,zero_point_lhs,  zero_point_rhs, first_row1,row_count1,A_fifo_fea1, col_indices_fifo_fea1, rnnz_fifo_fea1,B_accel1,C_fea11, C_fea12,
				  //C_fea13, C_fea14,
				  B_index, B_index_loop, tail);
		  compute1_2(gemm_mode,zero_point_lhs,  zero_point_rhs, first_row2,row_count2,A_fifo_fea2, col_indices_fifo_fea2, rnnz_fifo_fea2,B_accel2,C_fea21, C_fea22,
				  //C_fea23, C_fea24,
				  B_index, B_index_loop, tail);
	#endif

	#if ADJ_THREADS == 1

		  compute1_1(gemm_mode,zero_point_lhs,  zero_point_rhs, first_row1,row_count1,A_fifo_fea1, col_indices_fifo_fea1, rnnz_fifo_fea1,B_accel1,C_fea11,
	  				  //C_fea12,
	  				  //C_fea13, C_fea14,
	  				  B_index, B_index_loop, tail);

		  compute1_1(gemm_mode,zero_point_lhs,  zero_point_rhs, first_row2,row_count2,A_fifo_fea2, col_indices_fifo_fea2, rnnz_fifo_fea2,B_accel2,C_fea21,
	  				  //C_fea12,
	  				  //C_fea13, C_fea14,
	  				  B_index, B_index_loop, tail);
	#endif


	#endif


       #if FEA_THREADS == 4

		#if ADJ_THREADS == 4
	    hls::write_lock<buf> C_fea11(C_buffer11);
	    hls::write_lock<buf> C_fea12(C_buffer12);
	    hls::write_lock<buf> C_fea13(C_buffer13);
	    hls::write_lock<buf> C_fea14(C_buffer14);
	    hls::write_lock<buf> C_fea21(C_buffer21);
	    hls::write_lock<buf> C_fea22(C_buffer22);
	    hls::write_lock<buf> C_fea23(C_buffer23);
	    hls::write_lock<buf> C_fea24(C_buffer24);
	    hls::write_lock<buf> C_fea31(C_buffer31);
	    hls::write_lock<buf> C_fea32(C_buffer32);
	    hls::write_lock<buf> C_fea33(C_buffer33);
	    hls::write_lock<buf> C_fea34(C_buffer34);
	    hls::write_lock<buf> C_fea41(C_buffer41);
	    hls::write_lock<buf> C_fea42(C_buffer42);
	    hls::write_lock<buf> C_fea43(C_buffer43);
	    hls::write_lock<buf> C_fea44(C_buffer44);
		#endif

		#if ADJ_THREADS == 2

	    	hls::write_lock<buf> C_fea11(C_buffer11);
	 	    hls::write_lock<buf> C_fea12(C_buffer12);
	 	    //hls::write_lock<buf> C_fea13(C_buffer13);
	 	    //hls::write_lock<buf> C_fea14(C_buffer14);
	 	    hls::write_lock<buf> C_fea21(C_buffer21);
	 	    hls::write_lock<buf> C_fea22(C_buffer22);
	 	    //hls::write_lock<buf> C_fea23(C_buffer23);
	 	    //hls::write_lock<buf> C_fea24(C_buffer24);
	 	    hls::write_lock<buf> C_fea31(C_buffer31);
	 	    hls::write_lock<buf> C_fea32(C_buffer32);
	 	    //hls::write_lock<buf> C_fea33(C_buffer33);
	 	    //hls::write_lock<buf> C_fea34(C_buffer34);
	 	    hls::write_lock<buf> C_fea41(C_buffer41);
	 	    hls::write_lock<buf> C_fea42(C_buffer42);
	 	    //hls::write_lock<buf> C_fea43(C_buffer43);
	 	    //hls::write_lock<buf> C_fea44(C_buffer44);

		#endif

	    //std::cout << "Loop FEA " << std::endl;

		for (int j = 0; j < B_WIDTH_INT; j++) {
			        LOOP_BLOCKB : for (int i = 0; i < M_fea; i++) {
						    //#pragma HLS loop_tripcount min=84 max=84 avg=84
							#pragma HLS PIPELINE
							//#pragma HLS loop_tripcount min=16 max=16 avg=16
			        		BTYPE B_accel_temp = B[i+j*M_fea+B_index*B_WIDTH_BLOCK*M_fea];
			        		B_accel1[i][j] = B_accel_temp;
			        		B_accel2[i][j] = B_accel_temp;
			        		B_accel3[i][j] = B_accel_temp;
			        		B_accel4[i][j] = B_accel_temp;

							////std::cout << " " << i << " " << j << " " << B_accel[i][j]  << std::endl;
						}
		}



		  // read sparse matrices

	          ////std::cout << "reada " << //std::endl;


              int first_row1,first_row2,first_row3,first_row4;
              int row_count1,row_count2,row_count3,row_count4;


              int N_fea_block = N_fea/4;
			  int N_fea_rest = N_fea%4;
		      row_count1 = N_fea_block;
		      row_count2 = N_fea_block;
		      row_count3 = N_fea_block;
		      row_count4 = N_fea_block+N_fea_rest;
		      first_row1 = 0;
		      first_row2 = N_fea_block;
		      first_row3 = 2*N_fea_block;
		      first_row4 = 3*N_fea_block;


		      //std::cout << "thread1" << std::endl;

	          reada1(gemm_mode,M_fea,first_row1,row_count1,A_fifo_fea1,col_indices_fifo_fea1,rnnz_fifo_fea1,B_index_loop,tail,
		        rowPtr_fea1,columnIndex_fea1,values_fea1);
	          //std::cout << "thread2" << std::endl;
	          reada1(gemm_mode,M_fea,first_row2,row_count2,A_fifo_fea2,col_indices_fifo_fea2,rnnz_fifo_fea2,B_index_loop,tail,
	            rowPtr_fea2,columnIndex_fea2,values_fea2);
	          //std::cout << "thread3" << std::endl;
	          reada1(gemm_mode,M_fea,first_row3,row_count3,A_fifo_fea3,col_indices_fifo_fea3,rnnz_fifo_fea3,B_index_loop,tail,
	            rowPtr_fea3,columnIndex_fea3,values_fea3);
	          //std::cout << "thread4" << std::endl;
	          reada1(gemm_mode,M_fea,first_row4,row_count4,A_fifo_fea4,col_indices_fifo_fea4,rnnz_fifo_fea4,B_index_loop,tail,
	            rowPtr_fea4,columnIndex_fea4,values_fea4);


		  //check_fifo_0(a_values, A_fifo, A_fifo_out);

	          // inputs A_fifo_fea, col_indices_fifo_fea, rnnz_fifo_fea   and B_accel

		  //outputs C_buffer

	          //compute1 FEA * W = C

	          ////std::cout << "compute1" << //std::endl;


	  	    //std::cout << "COMPUTE1 " << std::endl;

			#if ADJ_THREADS == 4

	          compute1_4(gemm_mode,zero_point_lhs,  zero_point_rhs, first_row1,row_count1,A_fifo_fea1, col_indices_fifo_fea1, rnnz_fifo_fea1,B_accel1,C_fea11, C_fea12, C_fea13, C_fea14,B_index, B_index_loop, tail);
	          compute1_4(gemm_mode,zero_point_lhs,  zero_point_rhs, first_row2,row_count2,A_fifo_fea2, col_indices_fifo_fea2, rnnz_fifo_fea2,B_accel2,C_fea21, C_fea22, C_fea23, C_fea24,B_index, B_index_loop, tail);
	          compute1_4(gemm_mode,zero_point_lhs,  zero_point_rhs, first_row3,row_count3,A_fifo_fea3, col_indices_fifo_fea3, rnnz_fifo_fea3,B_accel3,C_fea31, C_fea32, C_fea33, C_fea34,B_index, B_index_loop, tail);
	          compute1_4(gemm_mode,zero_point_lhs,  zero_point_rhs, first_row4,row_count4,A_fifo_fea4, col_indices_fifo_fea4, rnnz_fifo_fea4,B_accel4,C_fea41, C_fea42, C_fea43, C_fea44,B_index, B_index_loop, tail);

	        #endif

			#if ADJ_THREADS == 2

			  compute1_2(gemm_mode,zero_point_lhs,  zero_point_rhs, first_row1,row_count1,A_fifo_fea1, col_indices_fifo_fea1, rnnz_fifo_fea1,B_accel1,C_fea11, C_fea12, //C_fea13, C_fea14,
					  B_index, B_index_loop, tail);
			  compute1_2(gemm_mode,zero_point_lhs,  zero_point_rhs, first_row2,row_count2,A_fifo_fea2, col_indices_fifo_fea2, rnnz_fifo_fea2,B_accel2,C_fea21, C_fea22, //C_fea23, C_fea24,
					  B_index, B_index_loop, tail);
			  compute1_2(gemm_mode,zero_point_lhs,  zero_point_rhs, first_row3,row_count3,A_fifo_fea3, col_indices_fifo_fea3, rnnz_fifo_fea3,B_accel3,C_fea31, C_fea32, //C_fea33, C_fea34,
					  B_index, B_index_loop, tail);
			  compute1_2(gemm_mode,zero_point_lhs,  zero_point_rhs, first_row4,row_count4,A_fifo_fea4, col_indices_fifo_fea4, rnnz_fifo_fea4,B_accel4,C_fea41, C_fea42, //C_fea43, C_fea44,
					  B_index, B_index_loop, tail);

			#endif
#endif


		   }


}


void loop_adj(bool relu,int *rowPtr_adj1,int *rowPtr_adj2,int *rowPtr_adj3,int *rowPtr_adj4,
	int *columnIndex_adj1,int *columnIndex_adj2,int *columnIndex_adj3,int *columnIndex_adj4,
	FTYPE *values_adj1,FTYPE *values_adj2,FTYPE *values_adj3,FTYPE *values_adj4,
	int N_adj, int M_adj,int P_w, ap_int<8> zero_point_lhs,  ap_int<8> zero_point_rhs,
	hls::stream_of_blocks<buf> &C_buffer11,hls::stream_of_blocks<buf> &C_buffer12,
	hls::stream_of_blocks<buf> &C_buffer13,hls::stream_of_blocks<buf> &C_buffer14,
	hls::stream_of_blocks<buf> &C_buffer21,hls::stream_of_blocks<buf> &C_buffer22,
	hls::stream_of_blocks<buf> &C_buffer23,hls::stream_of_blocks<buf> &C_buffer24,
	hls::stream_of_blocks<buf> &C_buffer31,hls::stream_of_blocks<buf> &C_buffer32,
	hls::stream_of_blocks<buf> &C_buffer33,hls::stream_of_blocks<buf> &C_buffer34,
	hls::stream_of_blocks<buf> &C_buffer41,hls::stream_of_blocks<buf> &C_buffer42,
	hls::stream_of_blocks<buf> &C_buffer43,hls::stream_of_blocks<buf> &C_buffer44,
	int B_index_loop, int tail,DTYPE* D1,DTYPE* D2,DTYPE* D3,DTYPE* D4)
{

       hls::stream<int> rnnz_fifo_adj1[SPMM_BLOCK];
       #pragma HLS STREAM variable=rnnz_fifo_adj1 depth=FIFO_DEPTH dim=1
       hls::stream<int> rnnz_fifo_adj2[SPMM_BLOCK];
       #pragma HLS STREAM variable=rnnz_fifo_adj2 depth=FIFO_DEPTH dim=1
       hls::stream<int> rnnz_fifo_adj3[SPMM_BLOCK];
       #pragma HLS STREAM variable=rnnz_fifo_adj3 depth=FIFO_DEPTH dim=1
       hls::stream<int> rnnz_fifo_adj4[SPMM_BLOCK];
       #pragma HLS STREAM variable=rnnz_fifo_adj4 depth=FIFO_DEPTH dim=1


       hls::stream<ATYPE> A_fifo_adj1;
       #pragma HLS STREAM variable=A_fifo_adj1 depth=FIFO_DEPTH dim=1
       hls::stream<ATYPE> A_fifo_adj2;
       #pragma HLS STREAM variable=A_fifo_adj2 depth=FIFO_DEPTH dim=1
       hls::stream<ATYPE> A_fifo_adj3;
       #pragma HLS STREAM variable=A_fifo_adj3 depth=FIFO_DEPTH dim=1
       hls::stream<ATYPE> A_fifo_adj4;
       #pragma HLS STREAM variable=A_fifo_adj4 depth=FIFO_DEPTH dim=1


       hls::stream<int>  col_indices_fifo_adj1;
       #pragma HLS STREAM variable=col_indices_fifo_adj1 depth=FIFO_DEPTH dim=1
       hls::stream<int>  col_indices_fifo_adj2;
       #pragma HLS STREAM variable=col_indices_fifo_adj2 depth=FIFO_DEPTH dim=1
       hls::stream<int>  col_indices_fifo_adj3;
       #pragma HLS STREAM variable=col_indices_fifo_adj3 depth=FIFO_DEPTH dim=1
       hls::stream<int>  col_indices_fifo_adj4;
       #pragma HLS STREAM variable=col_indices_fifo_adj4 depth=FIFO_DEPTH dim=1



       hls::stream<ITYPE>       D_fifo1[C_WIDTH_BLOCK][SPMM_BLOCK];
       #pragma HLS STREAM variable=D_fifo1 depth=FIFO_DEPTH dim=1
       hls::stream<ITYPE>       D_fifo2[C_WIDTH_BLOCK][SPMM_BLOCK];
       #pragma HLS STREAM variable=D_fifo2 depth=FIFO_DEPTH dim=1
       hls::stream<ITYPE>       D_fifo3[C_WIDTH_BLOCK][SPMM_BLOCK];
       #pragma HLS STREAM variable=D_fifo3 depth=FIFO_DEPTH dim=1
       hls::stream<ITYPE>       D_fifo4[C_WIDTH_BLOCK][SPMM_BLOCK];
       #pragma HLS STREAM variable=D_fifo4 depth=FIFO_DEPTH dim=1

       hls::stream<ITYPE>       write_fifo1[C_WIDTH_BLOCK];
       #pragma HLS STREAM variable=write_fifo1 depth=FIFO_DEPTH dim=1
       hls::stream<ITYPE>       write_fifo2[C_WIDTH_BLOCK];
       #pragma HLS STREAM variable=write_fifo2 depth=FIFO_DEPTH dim=1
       hls::stream<ITYPE>       write_fifo3[C_WIDTH_BLOCK];
       #pragma HLS STREAM variable=write_fifo3 depth=FIFO_DEPTH dim=1
       hls::stream<ITYPE>       write_fifo4[C_WIDTH_BLOCK];
       #pragma HLS STREAM variable=write_fifo4 depth=FIFO_DEPTH dim=1




   	   //int B_WIDTH_INT;


	   LOOP_ADJ : for (int B_index = 0; B_index < B_index_loop; B_index++) {

	#pragma HLS DATAFLOW

	    //if (B_index < (B_index_loop-1))
			//B_WIDTH_INT = B_WIDTH_BLOCK;
	      //else
			//B_WIDTH_INT = tail;

	#if ADJ_THREADS == 1

		   hls::read_lock<buf> C_adj11(C_buffer11);
		 		    //hls::read_lock<buf> C_adj12(C_buffer12);
		 		    //hls::read_lock<buf> C_adj13(C_buffer13);
		 		    //hls::read_lock<buf> C_adj14(C_buffer14);
			#if FEA_THREADS == 2
		    	hls::read_lock<buf> C_adj21(C_buffer21);
			#endif
		 		    //hls::read_lock<buf> C_adj22(C_buffer22);
		 		    /*hls::read_lock<buf> C_adj23(C_buffer23);
		 		    hls::read_lock<buf> C_adj24(C_buffer24);
		 		    hls::read_lock<buf> C_adj31(C_buffer31);
		 		    hls::read_lock<buf> C_adj32(C_buffer32);
		 		    hls::read_lock<buf> C_adj33(C_buffer33);
		 		    hls::read_lock<buf> C_adj34(C_buffer34);
		 		    hls::read_lock<buf> C_adj41(C_buffer41);
		 		    hls::read_lock<buf> C_adj42(C_buffer42);
		 		    hls::read_lock<buf> C_adj43(C_buffer43);
		 		    hls::read_lock<buf> C_adj44(C_buffer44);*/

		 	    int first_row1;//,first_row2;//,first_row3,first_row4;
		 	    int row_count1;//,row_count2;//,row_count3,row_count4;

		         int N_adj_block = N_adj/ADJ_THREADS;
		         int N_adj_block_compute = N_adj/FEA_THREADS; // in compute2 each block only contains  N_adj/FEA_THREADS elements
		 	    //int N_adj_rest = N_adj%2;
		 		row_count1 = N_adj_block;
		 		//row_count2 = N_adj_block;
		 		//row_count3 = N_adj_block;
		 		//row_count4 = N_adj_block+N_adj_rest;
		 		first_row1 = 0;
		 		//first_row2 = N_adj_block;
		 		//first_row3 = 2*N_adj_block;
		 		//first_row4 = 3*N_adj_block;


		 	    //std::cout << "READA2 " << std::endl;

		 	    reada2(first_row1,row_count1,B_index_loop,tail,A_fifo_adj1,col_indices_fifo_adj1,rnnz_fifo_adj1,rowPtr_adj1,columnIndex_adj1,values_adj1);
		        	//reada2(first_row2,row_count2,B_index_loop,tail,A_fifo_adj2,col_indices_fifo_adj2,rnnz_fifo_adj2,rowPtr_adj2,columnIndex_adj2,values_adj2);
		         //reada2(first_row3,row_count3,B_index_loop,tail,A_fifo_adj3,col_indices_fifo_adj3,rnnz_fifo_adj3,rowPtr_adj3,columnIndex_adj3,values_adj3);
		         //reada2(first_row4,row_count4,B_index_loop,tail,A_fifo_adj4,col_indices_fifo_adj4,rnnz_fifo_adj4,rowPtr_adj4,columnIndex_adj4,values_adj4);

		 	    //std::cout << "COMPUTE2 " << std::endl;
				#if FEA_THREADS == 1
		 	    	compute2_1(relu,N_adj_block,zero_point_lhs,  zero_point_rhs, first_row1,row_count1,A_fifo_adj1, col_indices_fifo_adj1, rnnz_fifo_adj1,C_adj11,
		 	    		//C_adj21,
		 	    		//C_adj31,C_adj41,
		 	    		D_fifo1, B_index, B_index_loop, tail);
				#endif
				#if FEA_THREADS == 2
		 	    	compute2_2(N_adj_block_compute,zero_point_lhs,  zero_point_rhs, first_row1,row_count1,A_fifo_adj1, col_indices_fifo_adj1, rnnz_fifo_adj1,C_adj11,C_adj21,
			   	    		//C_adj31,C_adj41,
			   	    		D_fifo1, B_index, B_index_loop, tail);

				#endif
		         //compute2(N_adj_block,zero_point_lhs,  zero_point_rhs, first_row2,row_count2,A_fifo_adj2, col_indices_fifo_adj2, rnnz_fifo_adj2,C_adj12,C_adj22,
		         		//C_adj32,C_adj42,
		         	//	D_fifo2, B_index, B_index_loop, tail);
		         //compute2(N_adj_block,zero_point_lhs,  zero_point_rhs, first_row3,row_count3,A_fifo_adj3, col_indices_fifo_adj3, rnnz_fifo_adj3,C_adj13,C_adj23,C_adj33,C_adj43,D_fifo3, B_index, B_index_loop, tail);
		         //compute2(N_adj_block,zero_point_lhs,  zero_point_rhs, first_row4,row_count4,A_fifo_adj4, col_indices_fifo_adj4, rnnz_fifo_adj4,C_adj14,C_adj24,C_adj34,C_adj44,D_fifo4, B_index, B_index_loop, tail);

		 	          //compute2(zero_point_lhs,  zero_point_rhs, N_adj,M_fea,A_fifo_adj, col_indices_fifo_adj, rnnz_fifo_adj,B,D_fifo, B_index_loop, tail);

		 	          //////std::cout << "scale" << //std::endl;

		 		  //scale(quantized_multiplier, shift, bias, zero_point_dst, clamp_max,clamp_min,N_adj, M_adj, P_w, D_fifo, B_index, B_index_loop, tail,write_fifo);

		 	          //check_fifo_2(N/4, write_fifo_0, write_fifo_out_0);

		 		 // write write _fifo into D

		 	          ////std::cout << "write matrix size " << N_adj << "," << P_w << //std::endl;

		 	    //std::cout << "WRITEC " << std::endl;

		 	          writec(relu,first_row1,row_count1,P_w, D_fifo1, D1,B_index,B_index_loop, tail);

	#endif

#if ADJ_THREADS == 2

	    hls::read_lock<buf> C_adj11(C_buffer11);
	    hls::read_lock<buf> C_adj12(C_buffer12);
	   	hls::read_lock<buf> C_adj21(C_buffer21);
	   	hls::read_lock<buf> C_adj22(C_buffer22);

		#if FEA_THREADS == 4
	    	hls::read_lock<buf> C_adj31(C_buffer31);
		    hls::read_lock<buf> C_adj32(C_buffer32);
		    hls::read_lock<buf> C_adj41(C_buffer41);
		    hls::read_lock<buf> C_adj42(C_buffer42);
		#endif


	   	int first_row1,first_row2;//,first_row3,first_row4;
	   	int row_count1,row_count2;//,row_count3,row_count4;

	    int N_adj_block = N_adj/ADJ_THREADS;
	   	int N_adj_rest = N_adj%ADJ_THREADS;
	    int N_adj_block_compute = N_adj/FEA_THREADS; // in compute2 each block only contains  N_adj/FEA_THREADS elements
	   	row_count1 = N_adj_block;
	   	row_count2 = N_adj_block+N_adj_rest;;
	   	first_row1 = 0;
	   	first_row2 = N_adj_block;


        std::cout << "Thread adj 1" << std::endl;
	   	reada2(first_row1,row_count1,B_index_loop,tail,A_fifo_adj1,col_indices_fifo_adj1,rnnz_fifo_adj1,rowPtr_adj1,columnIndex_adj1,values_adj1);
        std::cout << "Thread adj 2" << std::endl;
	   	reada2(first_row2,row_count2,B_index_loop,tail,A_fifo_adj2,col_indices_fifo_adj2,rnnz_fifo_adj2,rowPtr_adj2,columnIndex_adj2,values_adj2);

		#if FEA_THREADS == 2

	   	compute2_2(N_adj_block_compute,zero_point_lhs,  zero_point_rhs, first_row1,row_count1,A_fifo_adj1, col_indices_fifo_adj1, rnnz_fifo_adj1,C_adj11,C_adj21,
	   	    		//C_adj31,C_adj41,
	   	    		D_fifo1, B_index, B_index_loop, tail);

	    compute2_2(N_adj_block_compute,zero_point_lhs,  zero_point_rhs, first_row2,row_count2,A_fifo_adj2, col_indices_fifo_adj2, rnnz_fifo_adj2,C_adj12,C_adj22,
	           		//C_adj32,C_adj42,
	           		D_fifo2, B_index, B_index_loop, tail);

		#endif

		#if FEA_THREADS == 4


	   	compute2_4(relu,N_adj_block_compute,zero_point_lhs,  zero_point_rhs, first_row1,row_count1,A_fifo_adj1, col_indices_fifo_adj1, rnnz_fifo_adj1,C_adj11,C_adj21,
	   	    		C_adj31,C_adj41,
	   	    		D_fifo1, B_index, B_index_loop, tail);

	    compute2_4(relu,N_adj_block_compute,zero_point_lhs,  zero_point_rhs, first_row2,row_count2,A_fifo_adj2, col_indices_fifo_adj2, rnnz_fifo_adj2,C_adj12,C_adj22,
	           		C_adj32,C_adj42,
	           		D_fifo2, B_index, B_index_loop, tail);

		#endif

	   	 writec(relu,first_row1,row_count1,P_w, D_fifo1, D1,B_index,B_index_loop, tail);
	   	 writec(relu,first_row2,row_count2,P_w, D_fifo2, D2,B_index,B_index_loop, tail);

		#endif

	#if ADJ_THREADS == 4

		    hls::read_lock<buf> C_adj11(C_buffer11);
		    hls::read_lock<buf> C_adj12(C_buffer12);
		    hls::read_lock<buf> C_adj13(C_buffer13);
		    hls::read_lock<buf> C_adj14(C_buffer14);
		    hls::read_lock<buf> C_adj21(C_buffer21);
		    hls::read_lock<buf> C_adj22(C_buffer22);
		    hls::read_lock<buf> C_adj23(C_buffer23);
		    hls::read_lock<buf> C_adj24(C_buffer24);
		    hls::read_lock<buf> C_adj31(C_buffer31);
		    hls::read_lock<buf> C_adj32(C_buffer32);
		    hls::read_lock<buf> C_adj33(C_buffer33);
		    hls::read_lock<buf> C_adj34(C_buffer34);
		    hls::read_lock<buf> C_adj41(C_buffer41);
		    hls::read_lock<buf> C_adj42(C_buffer42);
		    hls::read_lock<buf> C_adj43(C_buffer43);
		    hls::read_lock<buf> C_adj44(C_buffer44);

	    int first_row1,first_row2,first_row3,first_row4;
	    int row_count1,row_count2,row_count3,row_count4;



        int N_adj_block = N_adj/4;
	    int N_adj_rest = N_adj%4;
		row_count1 = N_adj_block;
		row_count2 = N_adj_block;
		row_count3 = N_adj_block;
		row_count4 = N_adj_block+N_adj_rest;
		first_row1 = 0;
		first_row2 = N_adj_block;
		first_row3 = 2*N_adj_block;
		first_row4 = 3*N_adj_block;


	    //std::cout << "READA2 " << std::endl;



	    reada2(first_row1,row_count1,B_index_loop,tail,A_fifo_adj1,col_indices_fifo_adj1,rnnz_fifo_adj1,rowPtr_adj1,columnIndex_adj1,values_adj1);
       	reada2(first_row2,row_count2,B_index_loop,tail,A_fifo_adj2,col_indices_fifo_adj2,rnnz_fifo_adj2,rowPtr_adj2,columnIndex_adj2,values_adj2);
        reada2(first_row3,row_count3,B_index_loop,tail,A_fifo_adj3,col_indices_fifo_adj3,rnnz_fifo_adj3,rowPtr_adj3,columnIndex_adj3,values_adj3);
        reada2(first_row4,row_count4,B_index_loop,tail,A_fifo_adj4,col_indices_fifo_adj4,rnnz_fifo_adj4,rowPtr_adj4,columnIndex_adj4,values_adj4);

	    //std::cout << "COMPUTE2 " << std::endl;

	    compute2_4(relu,N_adj_block,zero_point_lhs,  zero_point_rhs, first_row1,row_count1,A_fifo_adj1, col_indices_fifo_adj1, rnnz_fifo_adj1,C_adj11,C_adj21,C_adj31,C_adj41,D_fifo1, B_index, B_index_loop, tail);
        compute2_4(relu,N_adj_block,zero_point_lhs,  zero_point_rhs, first_row2,row_count2,A_fifo_adj2, col_indices_fifo_adj2, rnnz_fifo_adj2,C_adj12,C_adj22,C_adj32,C_adj42,D_fifo2, B_index, B_index_loop, tail);
        compute2_4(relu,N_adj_block,zero_point_lhs,  zero_point_rhs, first_row3,row_count3,A_fifo_adj3, col_indices_fifo_adj3, rnnz_fifo_adj3,C_adj13,C_adj23,C_adj33,C_adj43,D_fifo3, B_index, B_index_loop, tail);
        compute2_4(relu,N_adj_block,zero_point_lhs,  zero_point_rhs, first_row4,row_count4,A_fifo_adj4, col_indices_fifo_adj4, rnnz_fifo_adj4,C_adj14,C_adj24,C_adj34,C_adj44,D_fifo4, B_index, B_index_loop, tail);

	    writec(relu,first_row1,row_count1,P_w, D_fifo1, D1,B_index,B_index_loop, tail);
	    writec(relu,first_row2,row_count2,P_w, D_fifo2, D2,B_index,B_index_loop, tail);
	    writec(relu,first_row3,row_count3,P_w, D_fifo3, D3,B_index,B_index_loop, tail);
	    writec(relu,first_row4,row_count4,P_w, D_fifo4, D4,B_index,B_index_loop, tail);



		#endif



	     }


}

void mmult_wrapper(bool gemm_mode,bool relu,ap_int<32> *quantized_multiplier, ap_int<32> *shift, ap_int<32> *bias,
	ap_int<32> bias_count, ap_int<8> zero_point_lhs,  ap_int<8> zero_point_rhs,
	ap_int<8> zero_point_dst, ap_int<8> clamp_max,ap_int<8> clamp_min,int N_adj, int M_adj, int M_fea,
	int P_w, BTYPE* B,
	 DTYPE* D1, DTYPE* D2, DTYPE* D3,DTYPE* D4,
	 int array_c_adjust, int B_index_loop, int tail,
	 int *rowPtr_fea1,int *rowPtr_fea2,int *rowPtr_fea3,int *rowPtr_fea4,
	 int *columnIndex_fea1, int *columnIndex_fea2, int *columnIndex_fea3, int *columnIndex_fea4,
	 FTYPE *values_fea1,FTYPE *values_fea2,FTYPE *values_fea3,FTYPE *values_fea4,
	 int *rowPtr_adj1,int *rowPtr_adj2,int *rowPtr_adj3,int *rowPtr_adj4,
	 int *columnIndex_adj1,int *columnIndex_adj2,int *columnIndex_adj3,int *columnIndex_adj4,
	 ATYPE *values_adj1,ATYPE *values_adj2,ATYPE *values_adj3,ATYPE *values_adj4)
{






      //ITYPE  C_buffer[B_HEIGHT][C_WIDTH_BLOCK];
      //#pragma HLS stream type=pipo variable=C_buffer depth=2

      hls::stream_of_blocks<buf> C_buffer11;
	  #pragma HLS array_partition variable=C_buffer11 block factor= BLOCK/2 dim=2
      #pragma HLS array_partition variable=C_buffer11 cyclic factor= SBLOCK dim=1

      hls::stream_of_blocks<buf> C_buffer12;
	  #pragma HLS array_partition variable=C_buffer12 block factor= BLOCK/2 dim=2
      #pragma HLS array_partition variable=C_buffer12 cyclic factor= SBLOCK dim=1

      hls::stream_of_blocks<buf> C_buffer13;
      #pragma HLS array_partition variable=C_buffer13 block factor= BLOCK/2 dim=2
      #pragma HLS array_partition variable=C_buffer13 cyclic factor= SBLOCK dim=1

      hls::stream_of_blocks<buf> C_buffer14;
      #pragma HLS array_partition variable=C_buffer14 block factor= BLOCK/2 dim=2
      #pragma HLS array_partition variable=C_buffer14 cyclic factor= SBLOCK dim=1

      hls::stream_of_blocks<buf> C_buffer21;
	  #pragma HLS array_partition variable=C_buffer21 block factor= BLOCK/2 dim=2
      #pragma HLS array_partition variable=C_buffer21 cyclic factor= SBLOCK dim=1

      hls::stream_of_blocks<buf> C_buffer22;
      #pragma HLS array_partition variable=C_buffer22 block factor= BLOCK/2 dim=2
      #pragma HLS array_partition variable=C_buffer22 cyclic factor= SBLOCK dim=1

      hls::stream_of_blocks<buf> C_buffer23;
      #pragma HLS array_partition variable=C_buffer23 block factor= BLOCK/2 dim=2
      #pragma HLS array_partition variable=C_buffer23 cyclic factor= SBLOCK dim=1

      hls::stream_of_blocks<buf> C_buffer24;
      #pragma HLS array_partition variable=C_buffer24 block factor= BLOCK/2 dim=2
      #pragma HLS array_partition variable=C_buffer24 cyclic factor= SBLOCK dim=1

      hls::stream_of_blocks<buf> C_buffer31;
      #pragma HLS array_partition variable=C_buffer31 block factor= BLOCK/2 dim=2
      #pragma HLS array_partition variable=C_buffer31 cyclic factor= SBLOCK dim=1

      hls::stream_of_blocks<buf> C_buffer32;
      #pragma HLS array_partition variable=C_buffer32 block factor= BLOCK/2 dim=2
      #pragma HLS array_partition variable=C_buffer32 cyclic factor= SBLOCK dim=1

      hls::stream_of_blocks<buf> C_buffer33;
      #pragma HLS array_partition variable=C_buffer33 block factor= BLOCK/2 dim=2
      #pragma HLS array_partition variable=C_buffer33 cyclic factor= SBLOCK dim=1

      hls::stream_of_blocks<buf> C_buffer34;
      #pragma HLS array_partition variable=C_buffer34 block factor= BLOCK/2 dim=2
      #pragma HLS array_partition variable=C_buffer34 cyclic factor= SBLOCK dim=1

      hls::stream_of_blocks<buf> C_buffer41;
      #pragma HLS array_partition variable=C_buffer41 block factor= BLOCK/2 dim=2
      #pragma HLS array_partition variable=C_buffer41 cyclic factor= SBLOCK dim=1

      hls::stream_of_blocks<buf> C_buffer42;
      #pragma HLS array_partition variable=C_buffer42 block factor= BLOCK/2 dim=2
      #pragma HLS array_partition variable=C_buffer42 cyclic factor= SBLOCK dim=1

      hls::stream_of_blocks<buf> C_buffer43;
      #pragma HLS array_partition variable=C_buffer43 block factor= BLOCK/2 dim=2
      #pragma HLS array_partition variable=C_buffer43 cyclic factor= SBLOCK dim=1

      hls::stream_of_blocks<buf> C_buffer44;
      #pragma HLS array_partition variable=C_buffer44 block factor= BLOCK/2 dim=2
      #pragma HLS array_partition variable=C_buffer44 cyclic factor= SBLOCK dim=1


      //hls::stream<ITYPE> D_fifo_0;
      //#pragma HLS STREAM variable=D_fifo_0 depth=128 dim=1

      //hls::stream<ITYPE> write_fifo_0;
      //#pragma HLS STREAM variable=write_fifo_0 depth=128 dim=1

      //hls::stream<ITYPE> write_fifo_out_0;
      //#pragma HLS STREAM variable=write_fifo_out_0 depth=8 dim=1



      int B_WIDTH_INT,a_values;

      #pragma HLS DATAFLOW

      loop_fea(gemm_mode,rowPtr_fea1,rowPtr_fea2,rowPtr_fea3,rowPtr_fea4,
      columnIndex_fea1,columnIndex_fea2,columnIndex_fea3,columnIndex_fea4,
      values_fea1,values_fea2,values_fea3,values_fea4,
      B,M_adj, M_fea,zero_point_lhs, zero_point_rhs,
      C_buffer11,C_buffer12,C_buffer13,C_buffer14,
      C_buffer21,C_buffer22,C_buffer23,C_buffer24,
      C_buffer31,C_buffer32,C_buffer33,C_buffer34,
      C_buffer41,C_buffer42,C_buffer43,C_buffer44,
      B_index_loop,tail);

	  loop_adj(relu,rowPtr_adj1,rowPtr_adj2,rowPtr_adj3,rowPtr_adj4,
	  columnIndex_adj1,columnIndex_adj2,columnIndex_adj3,columnIndex_adj4,
	  values_adj1,values_adj2,values_adj3,values_adj4,
	  N_adj, M_adj,P_w,zero_point_lhs,zero_point_rhs,
      C_buffer11,C_buffer12,C_buffer13,C_buffer14,
      C_buffer21,C_buffer22,C_buffer23,C_buffer24,
      C_buffer31,C_buffer32,C_buffer33,C_buffer34,
      C_buffer41,C_buffer42,C_buffer43,C_buffer44,
	  B_index_loop,tail,D1,D2,D3,D4);


}


typedef unsigned long u32;

/*
 The amount of data saved in the FPGA is B_HEIGHT*B_WIDTH_BLOCK+A_WIDTH+B_WIDTH_BLOCK which should be less than FPGA BRAM size
*/


void mmult_top(bool gemm_mode,bool relu, ap_int<32> *quantized_multiplier, ap_int<32> *shift, ap_int<32> *bias,
ap_int<32> bias_count,ap_int<64> *profiling, ap_int<8> zero_point_lhs,  ap_int<8> zero_point_rhs,
ap_int<8> zero_point_dst,
ap_int<8> clamp_max,ap_int<8> clamp_min,int N_adj, int M_adj, int M_fea, int P_w,
BTYPE* B,
DTYPE* D1, DTYPE* D2, DTYPE* D3,DTYPE* D4,
int array_c_adjust,
int *rowPtr_fea1,int *rowPtr_fea2,int *rowPtr_fea3,int *rowPtr_fea4,
int *columnIndex_fea1, int *columnIndex_fea2, int *columnIndex_fea3, int *columnIndex_fea4,
FTYPE *values_fea1,FTYPE *values_fea2,FTYPE *values_fea3,FTYPE *values_fea4,
int *rowPtr_adj1,int *rowPtr_adj2,int *rowPtr_adj3,int *rowPtr_adj4,
int *columnIndex_adj1,int *columnIndex_adj2,int *columnIndex_adj3,int *columnIndex_adj4,
ATYPE *values_adj1,ATYPE *values_adj2,ATYPE *values_adj3,ATYPE *values_adj4)
{

     #pragma HLS INTERFACE s_axilite port = return bundle = control
	 #pragma HLS INTERFACE s_axilite port = bias_count bundle = control
     #pragma HLS INTERFACE s_axilite port = zero_point_lhs bundle = control
     #pragma HLS INTERFACE s_axilite port = zero_point_rhs bundle = control
     #pragma HLS INTERFACE s_axilite port = zero_point_dst bundle = control
     #pragma HLS INTERFACE s_axilite port = clamp_max bundle = control
     #pragma HLS INTERFACE s_axilite port = clamp_min bundle = control
	 #pragma HLS INTERFACE s_axilite port = N_adj bundle = control
     #pragma HLS INTERFACE s_axilite port = M_adj bundle = control
     #pragma HLS INTERFACE s_axilite port = M_fea bundle = control
     #pragma HLS INTERFACE s_axilite port = P_w bundle = control
	 #pragma HLS INTERFACE s_axilite port = array_c_adjust bundle = control
     #pragma HLS INTERFACE s_axilite port = gemm_mode bundle = control
     #pragma HLS INTERFACE s_axilite port = relu bundle = control

     #pragma HLS INTERFACE m_axi port = profiling depth=16 offset=slave bundle = profiling
     #pragma HLS INTERFACE m_axi port=rowPtr_fea1 depth=4096 offset=slave bundle = rowPtr_fea1
	 #pragma HLS INTERFACE m_axi port=rowPtr_fea2 depth=4096 offset=slave bundle = rowPtr_fea2
     #pragma HLS INTERFACE m_axi port=rowPtr_fea3 depth=4096 offset=slave bundle = rowPtr_fea3
     #pragma HLS INTERFACE m_axi port=rowPtr_fea4 depth=4096 offset=slave bundle = rowPtr_fea4
	 #pragma HLS INTERFACE m_axi port=columnIndex_fea1 depth=128000 offset=slave bundle = columnIndex_fea1
     #pragma HLS INTERFACE m_axi port=columnIndex_fea2 depth=128000 offset=slave bundle = columnIndex_fea2
     #pragma HLS INTERFACE m_axi port=columnIndex_fea3 depth=128000 offset=slave bundle = columnIndex_fea3
     #pragma HLS INTERFACE m_axi port=columnIndex_fea4 depth=128000 offset=slave bundle = columnIndex_fea4
	 #pragma HLS INTERFACE m_axi port=values_fea1 depth=128000 offset=slave bundle = values_fea1
     #pragma HLS INTERFACE m_axi port=values_fea2 depth=128000 offset=slave bundle = values_fea2
     #pragma HLS INTERFACE m_axi port=values_fea3 depth=128000 offset=slave bundle = values_fea3
     #pragma HLS INTERFACE m_axi port=values_fea4 depth=128000 offset=slave bundle = values_fea4
	 #pragma HLS INTERFACE m_axi port=rowPtr_adj1 depth=4096 offset=slave bundle = rowPtr_adj1
     #pragma HLS INTERFACE m_axi port=rowPtr_adj2 depth=4096 offset=slave bundle = rowPtr_adj2
     #pragma HLS INTERFACE m_axi port=rowPtr_adj3 depth=4096 offset=slave bundle = rowPtr_adj3
     #pragma HLS INTERFACE m_axi port=rowPtr_adj4 depth=4096 offset=slave bundle = rowPtr_adj4
     #pragma HLS INTERFACE m_axi port=columnIndex_adj1 depth=128000 offset=slave bundle = columnIndex_adj1
     #pragma HLS INTERFACE m_axi port=columnIndex_adj2 depth=128000 offset=slave bundle = columnIndex_adj2
     #pragma HLS INTERFACE m_axi port=columnIndex_adj3 depth=128000 offset=slave bundle = columnIndex_adj3
     #pragma HLS INTERFACE m_axi port=columnIndex_adj4 depth=128000 offset=slave bundle = columnIndex_adj4
	 #pragma HLS INTERFACE m_axi port=values_adj1 depth=128000 offset=slave bundle = values_adj1
	 #pragma HLS INTERFACE m_axi port=values_adj2 depth=128000 offset=slave bundle = values_adj2
     #pragma HLS INTERFACE m_axi port=values_adj3 depth=128000 offset=slave bundle = values_adj3
     #pragma HLS INTERFACE m_axi port=values_adj4 depth=128000 offset=slave bundle = values_adj4
	 #pragma HLS INTERFACE m_axi port=B depth=128000 offset=slave bundle=B
     //#pragma HLS INTERFACE m_axi port=D1 depth=128000 offset=slave latency=0 num_write_outstanding=2048 bundle=D1
	 //#pragma HLS INTERFACE m_axi port=D2 depth=128000 offset=slave latency=0 num_write_outstanding=2048 bundle=D2
     //#pragma HLS INTERFACE m_axi port=D3 depth=128000 offset=slave latency=0 num_write_outstanding=2048 bundle=D3
     //#pragma HLS INTERFACE m_axi port=D4 depth=128000 offset=slave latency=0 num_write_outstanding=2048 bundle=D4
     #pragma HLS INTERFACE m_axi port=D1 depth=128000 offset=slave bundle=D1
	 #pragma HLS INTERFACE m_axi port=D2 depth=128000 offset=slave bundle=D2
     #pragma HLS INTERFACE m_axi port=D3 depth=128000 offset=slave bundle=D3
     #pragma HLS INTERFACE m_axi port=D4 depth=128000 offset=slave bundle=D4
	 #pragma HLS INTERFACE m_axi port=quantized_multiplier depth=1024 offset=slave bundle=quantized_multiplier
     #pragma HLS INTERFACE m_axi port=shift offset=slave depth=1024 bundle=shift
     #pragma HLS INTERFACE m_axi port=bias offset=slave depth=1024 bundle=bias
     #pragma HLS INTERFACE s_axilite port=columnIndex_fea1 bundle = control
	 #pragma HLS INTERFACE s_axilite port=columnIndex_fea2 bundle = control
     #pragma HLS INTERFACE s_axilite port=columnIndex_fea3 bundle = control
     #pragma HLS INTERFACE s_axilite port=columnIndex_fea4 bundle = control
     #pragma HLS INTERFACE s_axilite port=rowPtr_fea1 bundle = control
     #pragma HLS INTERFACE s_axilite port=rowPtr_fea2 bundle = control
     #pragma HLS INTERFACE s_axilite port=rowPtr_fea3 bundle = control
     #pragma HLS INTERFACE s_axilite port=rowPtr_fea4 bundle = control
     #pragma HLS INTERFACE s_axilite port=columnIndex_adj1 bundle = control
     #pragma HLS INTERFACE s_axilite port=columnIndex_adj2 bundle = control
     #pragma HLS INTERFACE s_axilite port=columnIndex_adj3 bundle = control
     #pragma HLS INTERFACE s_axilite port=columnIndex_adj4 bundle = control
     #pragma HLS INTERFACE s_axilite port=rowPtr_adj1 bundle = control
     #pragma HLS INTERFACE s_axilite port=rowPtr_adj2 bundle = control
     #pragma HLS INTERFACE s_axilite port=rowPtr_adj3 bundle = control
     #pragma HLS INTERFACE s_axilite port=rowPtr_adj4 bundle = control
     #pragma HLS INTERFACE s_axilite port=values_adj1  bundle = control
	 #pragma HLS INTERFACE s_axilite port=values_adj2  bundle = control
     #pragma HLS INTERFACE s_axilite port=values_adj3  bundle = control
     #pragma HLS INTERFACE s_axilite port=values_adj4  bundle = control
	 #pragma HLS INTERFACE s_axilite port=values_fea1  bundle = control
     #pragma HLS INTERFACE s_axilite port=values_fea2  bundle = control
     #pragma HLS INTERFACE s_axilite port=values_fea3  bundle = control
     #pragma HLS INTERFACE s_axilite port=values_fea4  bundle = control
     #pragma HLS INTERFACE s_axilite port=B  bundle = control
     #pragma HLS INTERFACE s_axilite port=D1  bundle = control
	 #pragma HLS INTERFACE s_axilite port=D2  bundle = control
     #pragma HLS INTERFACE s_axilite port=D3  bundle = control
     #pragma HLS INTERFACE s_axilite port=D4  bundle = control
     #pragma HLS INTERFACE s_axilite port=profiling  bundle = control
     #pragma HLS INTERFACE s_axilite port=quantized_multiplier  bundle = control
     #pragma HLS INTERFACE s_axilite port=shift  bundle = control
     #pragma HLS INTERFACE s_axilite port=bias  bundle = control

	 //c_fifo_stream_t       C_fifo[B_WIDTH_BLOCK];
	 //#pragma HLS STREAM variable=C_fifo depth=1024 dim=1


	 ap_int<32> bias_data[1024];
	 ap_int<32> shift_data[1024];
	 ap_int<32> quantized_multiplier_data[1024];


	 //load bias
         //preloading bias and param data seems to be a good idea but in practice performance is the same and we save preloading overhead
         //param data is loaded in demand in this case
         //preloading is important for certain matrix configurations with small A and large B so I am going to leave it
         if (bias_count > 0)
         {

	 	for(int bias_index=0;bias_index<bias_count;bias_index++)
	 	{
			#pragma HLS PIPELINE
			 bias_data[bias_index]=bias[bias_index];
			 shift_data[bias_index]=shift[bias_index];
			 quantized_multiplier_data[bias_index]=quantized_multiplier[bias_index];
	 	}


	 }
     else
	 {

	 fifo_empty_0 = 0;
 	 fifo_empty_1 = 0;
 	 fifo_empty_2 = 0;
	 fifo_full_0 = 0;
	 fifo_full_1 = 0;
	 fifo_full_2 = 0;
	 fifo_read_0 = 0;
	 fifo_read_1 = 0;
	 fifo_read_2 = 0;
	 fifo_write_0 = 0;
	 fifo_write_1 = 0;
	 fifo_write_2 = 0;
	 fifo_cycle_0 = 0;
	 fifo_cycle_1 = 0;
	 fifo_cycle_2 = 0;


	 ap_int<32> tail = P_w%B_WIDTH_BLOCK;
     ap_int<32> B_index_loop = P_w/B_WIDTH_BLOCK + 1;

	 if (tail == 0)
	 {
	 	B_index_loop = P_w/B_WIDTH_BLOCK;
		tail = B_WIDTH_BLOCK;
	 }
	 //else
	 //{
	  /*simulation run short, remove in normal synthesis*/
	 // B_index_loop = 2;
	 // tail = 0;
	 //}

         ////////std::cout << " B_index_loop is "<< B_index_loop<< " tail is "<< tail << ////std::endl;

         //for (int B_index = 0; B_index < B_index_loop; B_index++) {

	  int N_adj_block = N_adj/ADJ_THREADS;
	  D2+=N_adj_block*P_w;
	  D3+=2*N_adj_block*P_w;
	  D4+=3*N_adj_block*P_w;


	  mmult_wrapper(gemm_mode,relu,quantized_multiplier_data, shift_data, bias_data, bias_count, zero_point_lhs, zero_point_rhs, zero_point_dst, clamp_max,clamp_min,N_adj, M_adj, M_fea, P_w,
	  B,D1, D2, D3,D4,
      array_c_adjust, B_index_loop, tail,
	  rowPtr_fea1,rowPtr_fea2,rowPtr_fea3,rowPtr_fea4,
	  columnIndex_fea1, columnIndex_fea2, columnIndex_fea3,columnIndex_fea4,
	  values_fea1,values_fea2,values_fea3,values_fea4,
	  rowPtr_adj1,rowPtr_adj2,rowPtr_adj3,rowPtr_adj4,
	  columnIndex_adj1,columnIndex_adj2,columnIndex_adj3,columnIndex_adj4,
	  values_adj1,values_adj2,values_adj3,values_adj4);



        //}

	profiling[0] = fifo_full_0;
	profiling[1] = fifo_full_1;
	profiling[2] = fifo_full_2;
	profiling[3] = fifo_empty_0;
	profiling[4] = fifo_empty_1;
	profiling[5] = fifo_empty_2;
	profiling[6] = fifo_read_0;
	profiling[7] = fifo_read_1;
	profiling[8] = fifo_read_2;
	profiling[9] = fifo_write_0;
	profiling[10] = fifo_write_1;
	profiling[11] = fifo_write_2;
	profiling[12] = fifo_cycle_0;
	profiling[13] = fifo_cycle_1;
	profiling[14] = fifo_cycle_2;



     }
}

void kernelmult1(
bool gemm_mode,
ap_int<32> *quantized_multiplier,
ap_int<32> *shift,
ap_int<32> *bias,
ap_int<32> bias_count,
ap_int<64> *profiling,
ap_int<8> zero_point_lhs,
ap_int<8> zero_point_rhs,
ap_int<8> zero_point_dst,
ap_int<8> clamp_max,
ap_int<8> clamp_min,
BTYPE *array_b,
DTYPE *array_d1,
DTYPE *array_d2,
DTYPE *array_d3,
DTYPE *array_d4,
FTYPE *values_fea1,
FTYPE *values_fea2,
FTYPE *values_fea3,
FTYPE *values_fea4,
int *colIndices_fea1,
int *colIndices_fea2,
int *colIndices_fea3,
int *colIndices_fea4,
int *rowPtr_fea1,
int *rowPtr_fea2,
int *rowPtr_fea3,
int *rowPtr_fea4,
ATYPE *values_adj1,
ATYPE *values_adj2,
ATYPE *values_adj3,
ATYPE *values_adj4,
int *colIndices_adj1,
int *colIndices_adj2,
int *colIndices_adj3,
int *colIndices_adj4,
int *rowPtr_adj1,
int *rowPtr_adj2,
int *rowPtr_adj3,
int *rowPtr_adj4,
int N_adj,
int M_adj,
int M_fea,
int P_w
)
{

    int array_c_adjust=N_adj;

    //int N_adj_block = N_adj/ADJ_THREADS;
    //array_d2+=N_adj_block*P_w;
    //array_d3+=2*N_adj_block*P_w;
    //array_d4+=3*N_adj_block*P_w;

    //#pragma SDS resource(1)
    //#pragma SDS async(1)

    printf(" sw starting ");

    bool relu = 0;
    mmult_top(gemm_mode,relu,quantized_multiplier,shift,bias,bias_count,profiling,zero_point_lhs,zero_point_rhs, zero_point_dst,clamp_max,clamp_min,
    N_adj, M_adj, M_fea, P_w,
    array_b,
    array_d1,array_d2,array_d3,array_d4,
    array_c_adjust,
    rowPtr_fea1,rowPtr_fea2,rowPtr_fea3,rowPtr_fea4,
    colIndices_fea1,colIndices_fea2,colIndices_fea3,colIndices_fea4,
    values_fea1,values_fea2,values_fea3,values_fea4,
    rowPtr_adj1,rowPtr_adj2,rowPtr_adj3,rowPtr_adj4,
    colIndices_adj1,colIndices_adj2,colIndices_adj3,colIndices_adj4,
    values_adj1,values_adj2,values_adj3,values_adj4);



    //#pragma SDS wait(1)

    ////////////std::cout << " sw done " << //////std::endl;
}
