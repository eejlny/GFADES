/*===============================================================================
* This file is part of the gFADES GNN accelerator
* has been written at Linkoping University
* for the WASP project
* Author : Jose Nunez-Yanez
*
===============================================================================
*/

#ifndef KERNELMATRIXMULT_H_
#define KERNELMATRIXMULT_H_

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
);


int kernelmult1_sw(
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
DTYPE *array_d,
FTYPE *values_fea,
int *colIndices_fea,
int *rowPtr_fea,
int nnz_fea,
ATYPE *values_adj,
int *colIndices_adj,
int *rowPtr_adj,
int nnz_adj,
int N_adj,
int M_adj,
int M_fea,
int P_w
);

#endif 
