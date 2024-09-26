/*===============================================================================
* This file is part of the gFADES GNN accelerator
* has been written at Linkoping University
* for the WASP project
* Author : Jose Nunez-Yanez
*
===============================================================================
*/

#ifndef __MATRIX_MULT_H__
#define __MATRIX_MULT_H__

#include <vector>

//WARNING: this accerator uses a writec loop that ignores possible tails. This means
//that the width of the WEIGHT matrix has to be a multiple of the number of cores.
//For example 64 for P_w and then 16 cores so a total of 4 EXACT tiles are processed.
//use_sblocks is configurable to use SPMM_BLOCK or not. This optimizes the writec loop
//without SPMM_BLOCK so that the writing to memory is more efficient. This is important
//if the amount of work is so little that the bottleneck is the write loop

// 64 * 16(values in 32-bit word (ternary)) = 1024
//1536 and 16 in a word so 96 words of 32-bit
//1536 and 4 in a word so 384 words of 8-bit

//if you want to generate CSR then set this DTYPE_LENGTH to 8 since spmm uses packing to 8-bit for A matrix
// if you set it to 32 then A matrix will be formed with 32-bits words instead of 8-bit
//8-bit packs
// ./generate_csr_8.elf /mnt/weights_tern_090_block_32_1.csv 1 6 336  512
//32-bit packs
// ./generate_csr_32.elf /mnt/weights_tern_090_block_32_1.csv 1 6 84  512
//quad
//8-bit packs
// ./generate_csr_8.elf /mnt/weights_quad_block_095_32_1.csv 2 6 672  512
//32-bit packs
// ./generate_csr_32.elf /mnt/weights_quad_block_095_32_1.csv 2 6 168  512

//#define GENERATE_CSR 0
//#define DTYPE_LENGTH 32 //8//32 //8 //32//32 //8//32//8

//#define simulation

#define MAX_N    6144 //adj dimensions adj matrix NxN //20480//16384 //4096 //32768 //20480 //64
#define MAX_M    6144 //Number of features fea matrix NxM //20480//16384 //4096 //24576 //2048 //384 //1536 //384 //1536 // 384 //48 //768 //96 //384 //96 //384 //96 //384// 96
#define MAX_P    128 //Number of hideen units weight matrix MxP 2048 //512 //64//1//64//1

#define MAX_FIFO 8

//#define SN    64 //2048 //64
//#define SM    384 //1536 //384 //1536 //384 //1536 // 384 //48 //768 //96 //384 //96 //384 //96 //384// 96
//#define SP    512//64//1//64//1


//48 for WL 64
//96 for WL 32
// 384 for WL 8
// 768 for WL 4

#define A_HEIGHT   MAX_N
#define A_WIDTH    MAX_N

#define B_HEIGHT   MAX_N
#define B_WIDTH    MAX_M

#define C_HEIGHT   MAX_M
#define C_WIDTH    MAX_P


//#define TWOBIT_TERN
//#define TWOBIT_SIX
//#define TWOBIT_OPT
//#define TWOBIT_POS
//#define TWOBIT_NAIVE
//#define TWOBIT
//#define FOURBIT_NAIVE
//#define FOURBIT
//#define EIGHTBIT
//#define FOURBIT_XIL
//#define ONEBIT
#define HALF
//#define FLOAT
//#define TEST


#ifdef EIGHTBIT

	////typedef ap_fixed<16, 2> ATYPE;
	//typedef ap_int<8> ATYPE;
	////typedef ap_fixed<16, 2> BTYPE;
	//typedef ap_int<8> BTYPE;
	//typedef ap_int<8> DTYPE;
	////typedef ap_fixed<16, 2> DTYPE;
	////typedef ap_fixed<16, 2> FTYPE;
	//typedef ap_int<16> ITYPE;
	////typedef ap_fixed<16, 2> ITYPE;


	//typedef ap_ufixed<8, 0> ATYPE;
	//typedef ap_ufixed<8, 0> BTYPE;
	//typedef ap_ufixed<8, 0> DTYPE;
	//typedef ap_ufixed<8, 0> FTYPE;
	//typedef ap_ufixed<16, 8> ITYPE;


	typedef ap_fixed<16, 2> ATYPE;
	typedef ap_fixed<16, 2> BTYPE;
	typedef ap_fixed<16, 2> DTYPE;
	typedef ap_fixed<16, 2> FTYPE;
	typedef ap_fixed<16, 2> ITYPE;

	//typedef ap_fixed<8, 2> ATYPE;
	//typedef ap_fixed<8, 2> BTYPE;
	//typedef ap_fixed<8, 2> DTYPE;
	//typedef ap_fixed<8, 2> FTYPE;
	//typedef ap_fixed<8, 2> ITYPE;

    #define FTYPE_LATENCY_ADJ 1
    #define FTYPE_LATENCY_FEA 1

	//typedef ap_fixed<32, 2> ATYPE;
	//typedef ap_fixed<32, 2> BTYPE;
	//typedef ap_fixed<32, 2> DTYPE;
	//typedef ap_fixed<32, 2> FTYPE;
	//typedef ap_fixed<32, 2> ITYPE;


#endif

#ifdef HALF

	typedef half ATYPE;
	typedef half BTYPE;
	typedef half DTYPE;
	typedef half FTYPE;
	//typedef hls::vector<half, 4> PFTYPE;
	typedef half ITYPE;
	#define FTYPE_LATENCY_ADJ 4
	#define FTYPE_LATENCY_FEA 4
#endif

#ifdef FLOAT

	typedef float ATYPE;
	typedef float BTYPE;
	typedef float DTYPE;
	typedef float FTYPE;
	typedef float ITYPE;
	//#define FTYPE_LATENCY 4 //100 MHZ
	#define FTYPE_LATENCY_ADJ 6
	#define FTYPE_LATENCY_FEA 6 //200 MHZ
#endif

//#ifdef FLOAT
//	typedef ap_uint<DTYPE_LENGTH> DTYPE;
//	typedef ap_int<32> DTYPE_OUT;
//	typedef ap_int<8> ATYPE;
//  	#define FTYPE_LATENCY 6   //optimal latency is 6 for 200 MHz with 3 II increases but we reduce pressure on logic
//#endif

//#ifdef HALF
//	typedef half FTYPE;
//        #define FTYPE_LATENCY 4
//	typedef float FTYPE;
//#endif

#define FEA_THREADS 1
#define ADJ_THREADS 1

#define SPMM_BLOCK 1
#define USE_SBLOCKS 0

/*the compute unit always uses sblocks, the USE_SBLOCKS controls if the write unit also uses sblocks and reads multiple FIFO channels
 * or reads only one FIFO channel per core and this is generally better because it optimizes the loop
 * that writes data to memory.
 */


#define USE_TAIL 0 //parameter discontinued. it should always be zero so the block size is always processed but only tail remainer is written to memory.
#define USE_RELU 1

/*IF use_tail is set to 0 then the tail has to be zero so the division bewtween the number of weight columns (number of hidden units or neurons) and the number of compute units needs to be integer
* so  P_w has to be 16, 34, 64 etc but NOT 21. If weight columns is not multiple of 2 then use USE_TAIL 1*/

/*recommended that USE_SBLOCKS is 0 and USE_TAIL 0*/

/* spmm block controls how many rows of the sparse matrix are processed in a single for loop. In principle only one
 * row is processed and then a matrix mult output is written into the C buffer memory. If only a few elements in the row
 * are nonzero then the overhead is significant since the loop needs to start again for the next row. The loop
 * achieves II 1 but if the number of elements of the row is small the flushing the pipeline and restarting the row
 * is an overhead. By grouping several rows in a single loop it is possible to alleviate this problem and have more nonzeros to process
 */

#define A_HEIGHT_BLOCK  1// 4096 //(512/4)
#define B_WIDTH_BLOCK 2 //the width of compute1 BLOCK BUFFER A*B = C 16 //32 //64 //64 //128 // 64 //64 //64 //8//8// //16//32//1//32//1//32//1// 1//32//(128/4)
#define C_WIDTH_BLOCK 2 //the width of compute2 BLOCK BUFFER C*D = F
#define B_BLOCK_PARALLEL 1
//#define PES_ADJ 2 // Number of PEs for ADJ processing
//#define PES_FEA 2 // Number iof PEs for FEA processing


#define ENABLE_GEMM
#define ENABLE_SPMM
//#define ENABLE_SCALING
//#define ENABLE_TRANSPOSE


//typedef ap_int<32> c_fifo_t;
//typedef hls::stream<c_fifo_t> c_fifo_stream_t;

//how many rows of B are computed in parallel in multiplication loop
//for example a couple of B rows are multiplied for A 1 row in each loop iteration
//it basically reduces how the loop iterations by 2 if it is 2.
/*
 A_HEIGHT_BLOCK  is for software part data partitioning due to the limitation in 
 the Xilinx kernel sds_alloc so A_HEIGHT_BLOCK should be A_HEIGHT divided by 
 the number of considered blocks
*/
#define C_HEIGHT_BLOCK  A_HEIGHT_BLOCK 


//typedef unsigned long u32;

typedef std::vector<int> vi;



//int mmult_accel(ap_uint<2> ternary, ap_int<8> zero_point_lhs,  ap_int<8> zero_point_rhs, int M, DTYPE A[A_HEIGHT_BLOCK][A_WIDTH], DTYPE B[B_HEIGHT][B_WIDTH_BLOCK], DTYPE_OUT C[C_HEIGHT_BLOCK][B_WIDTH_BLOCK]);
//#pragma SDS data access_pattern(A:SEQUENTIAL)


//#pragma SDS data zero_copy(B[0:B_HEIGHT*B_WIDTH], C[0:A_HEIGHT*B_WIDTH])
//#pragma SDS data copy(A[0:A_HEIGHT*A_WIDTH])
//#pragma SDS data access_pattern(A:RANDOM)
//#pragma SDS data zero_copy(A[0:A_HEIGHT*A_WIDTH],B[0:B_HEIGHT*B_WIDTH], C[0:A_HEIGHT*B_WIDTH])
//#pragma SDS data sys_port(A:AFI,B:AFI,C:AFI)
//#pragma SDS data access_pattern(A:ACP;B:ACP;C:ACP)
//#pragma SDS data zero_copy(A[0:line_count*A_WIDTH],B[0:B_HEIGHT*B_WIDTH], C[0:A_HEIGHT*B_WIDTH])
//int mmult_top(ap_uint<2> ternary,ap_int<32> *quantized_multiplier, ap_int<32> *shift, ap_int<32> *bias, ap_int<32> bias_count,ap_int<8> zero_point_lhs,  ap_int<8> zero_point_rhs, ap_int<8> zero_point_dst,ap_int<8> clamp_max,ap_int<8> clamp_min,int N, int M, int P, DTYPE* A, DTYPE* B, DTYPE* C);
//int mmult_top2(int A[A_WIDTH*A_HEIGHT], int B[B_HEIGHT*B_WIDTH], int C[C_HEIGHT*C_WIDTH],int line_count);
//int mmult_top3(int A[A_WIDTH*A_HEIGHT], int B[B_HEIGHT*B_WIDTH], int C[C_HEIGHT*C_WIDTH],int line_count);
//int mmult_top4(int A[A_WIDTH*A_HEIGHT], int B[B_HEIGHT*B_WIDTH], int C[C_HEIGHT*C_WIDTH],int line_count);

#endif //__MATRIX_MULT_H__



