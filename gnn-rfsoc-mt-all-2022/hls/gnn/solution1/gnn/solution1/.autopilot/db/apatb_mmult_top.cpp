#include <systemc>
#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <stdint.h>
#include "SysCFileHandler.h"
#include "ap_int.h"
#include "ap_fixed.h"
#include <complex>
#include <stdbool.h>
#include "autopilot_cbe.h"
#include "hls_stream.h"
#include "hls_half.h"
#include "hls_signal_handler.h"

using namespace std;
using namespace sc_core;
using namespace sc_dt;

// wrapc file define:
#define AUTOTB_TVIN_gemm_mode "../tv/cdatafile/c.mmult_top.autotvin_gemm_mode.dat"
#define AUTOTB_TVOUT_gemm_mode "../tv/cdatafile/c.mmult_top.autotvout_gemm_mode.dat"
#define AUTOTB_TVIN_quantized_multiplier_offset "../tv/cdatafile/c.mmult_top.autotvin_quantized_multiplier_offset.dat"
#define AUTOTB_TVOUT_quantized_multiplier_offset "../tv/cdatafile/c.mmult_top.autotvout_quantized_multiplier_offset.dat"
#define AUTOTB_TVIN_shift_offset "../tv/cdatafile/c.mmult_top.autotvin_shift_offset.dat"
#define AUTOTB_TVOUT_shift_offset "../tv/cdatafile/c.mmult_top.autotvout_shift_offset.dat"
#define AUTOTB_TVIN_bias_offset "../tv/cdatafile/c.mmult_top.autotvin_bias_offset.dat"
#define AUTOTB_TVOUT_bias_offset "../tv/cdatafile/c.mmult_top.autotvout_bias_offset.dat"
#define AUTOTB_TVIN_bias_count "../tv/cdatafile/c.mmult_top.autotvin_bias_count.dat"
#define AUTOTB_TVOUT_bias_count "../tv/cdatafile/c.mmult_top.autotvout_bias_count.dat"
#define AUTOTB_TVIN_profiling_offset "../tv/cdatafile/c.mmult_top.autotvin_profiling_offset.dat"
#define AUTOTB_TVOUT_profiling_offset "../tv/cdatafile/c.mmult_top.autotvout_profiling_offset.dat"
#define AUTOTB_TVIN_zero_point_lhs "../tv/cdatafile/c.mmult_top.autotvin_zero_point_lhs.dat"
#define AUTOTB_TVOUT_zero_point_lhs "../tv/cdatafile/c.mmult_top.autotvout_zero_point_lhs.dat"
#define AUTOTB_TVIN_zero_point_rhs "../tv/cdatafile/c.mmult_top.autotvin_zero_point_rhs.dat"
#define AUTOTB_TVOUT_zero_point_rhs "../tv/cdatafile/c.mmult_top.autotvout_zero_point_rhs.dat"
#define AUTOTB_TVIN_zero_point_dst "../tv/cdatafile/c.mmult_top.autotvin_zero_point_dst.dat"
#define AUTOTB_TVOUT_zero_point_dst "../tv/cdatafile/c.mmult_top.autotvout_zero_point_dst.dat"
#define AUTOTB_TVIN_clamp_max "../tv/cdatafile/c.mmult_top.autotvin_clamp_max.dat"
#define AUTOTB_TVOUT_clamp_max "../tv/cdatafile/c.mmult_top.autotvout_clamp_max.dat"
#define AUTOTB_TVIN_clamp_min "../tv/cdatafile/c.mmult_top.autotvin_clamp_min.dat"
#define AUTOTB_TVOUT_clamp_min "../tv/cdatafile/c.mmult_top.autotvout_clamp_min.dat"
#define AUTOTB_TVIN_N_adj "../tv/cdatafile/c.mmult_top.autotvin_N_adj.dat"
#define AUTOTB_TVOUT_N_adj "../tv/cdatafile/c.mmult_top.autotvout_N_adj.dat"
#define AUTOTB_TVIN_M_adj "../tv/cdatafile/c.mmult_top.autotvin_M_adj.dat"
#define AUTOTB_TVOUT_M_adj "../tv/cdatafile/c.mmult_top.autotvout_M_adj.dat"
#define AUTOTB_TVIN_M_fea "../tv/cdatafile/c.mmult_top.autotvin_M_fea.dat"
#define AUTOTB_TVOUT_M_fea "../tv/cdatafile/c.mmult_top.autotvout_M_fea.dat"
#define AUTOTB_TVIN_P_w "../tv/cdatafile/c.mmult_top.autotvin_P_w.dat"
#define AUTOTB_TVOUT_P_w "../tv/cdatafile/c.mmult_top.autotvout_P_w.dat"
#define AUTOTB_TVIN_B_offset "../tv/cdatafile/c.mmult_top.autotvin_B_offset.dat"
#define AUTOTB_TVOUT_B_offset "../tv/cdatafile/c.mmult_top.autotvout_B_offset.dat"
#define AUTOTB_TVIN_D1_offset "../tv/cdatafile/c.mmult_top.autotvin_D1_offset.dat"
#define AUTOTB_TVOUT_D1_offset "../tv/cdatafile/c.mmult_top.autotvout_D1_offset.dat"
#define AUTOTB_TVIN_D2_offset "../tv/cdatafile/c.mmult_top.autotvin_D2_offset.dat"
#define AUTOTB_TVOUT_D2_offset "../tv/cdatafile/c.mmult_top.autotvout_D2_offset.dat"
#define AUTOTB_TVIN_D3_offset "../tv/cdatafile/c.mmult_top.autotvin_D3_offset.dat"
#define AUTOTB_TVOUT_D3_offset "../tv/cdatafile/c.mmult_top.autotvout_D3_offset.dat"
#define AUTOTB_TVIN_D4_offset "../tv/cdatafile/c.mmult_top.autotvin_D4_offset.dat"
#define AUTOTB_TVOUT_D4_offset "../tv/cdatafile/c.mmult_top.autotvout_D4_offset.dat"
#define AUTOTB_TVIN_array_c_adjust "../tv/cdatafile/c.mmult_top.autotvin_array_c_adjust.dat"
#define AUTOTB_TVOUT_array_c_adjust "../tv/cdatafile/c.mmult_top.autotvout_array_c_adjust.dat"
#define AUTOTB_TVIN_rowPtr_fea1_offset "../tv/cdatafile/c.mmult_top.autotvin_rowPtr_fea1_offset.dat"
#define AUTOTB_TVOUT_rowPtr_fea1_offset "../tv/cdatafile/c.mmult_top.autotvout_rowPtr_fea1_offset.dat"
#define AUTOTB_TVIN_rowPtr_fea2_offset "../tv/cdatafile/c.mmult_top.autotvin_rowPtr_fea2_offset.dat"
#define AUTOTB_TVOUT_rowPtr_fea2_offset "../tv/cdatafile/c.mmult_top.autotvout_rowPtr_fea2_offset.dat"
#define AUTOTB_TVIN_rowPtr_fea3_offset "../tv/cdatafile/c.mmult_top.autotvin_rowPtr_fea3_offset.dat"
#define AUTOTB_TVOUT_rowPtr_fea3_offset "../tv/cdatafile/c.mmult_top.autotvout_rowPtr_fea3_offset.dat"
#define AUTOTB_TVIN_rowPtr_fea4_offset "../tv/cdatafile/c.mmult_top.autotvin_rowPtr_fea4_offset.dat"
#define AUTOTB_TVOUT_rowPtr_fea4_offset "../tv/cdatafile/c.mmult_top.autotvout_rowPtr_fea4_offset.dat"
#define AUTOTB_TVIN_columnIndex_fea1_offset "../tv/cdatafile/c.mmult_top.autotvin_columnIndex_fea1_offset.dat"
#define AUTOTB_TVOUT_columnIndex_fea1_offset "../tv/cdatafile/c.mmult_top.autotvout_columnIndex_fea1_offset.dat"
#define AUTOTB_TVIN_columnIndex_fea2_offset "../tv/cdatafile/c.mmult_top.autotvin_columnIndex_fea2_offset.dat"
#define AUTOTB_TVOUT_columnIndex_fea2_offset "../tv/cdatafile/c.mmult_top.autotvout_columnIndex_fea2_offset.dat"
#define AUTOTB_TVIN_columnIndex_fea3_offset "../tv/cdatafile/c.mmult_top.autotvin_columnIndex_fea3_offset.dat"
#define AUTOTB_TVOUT_columnIndex_fea3_offset "../tv/cdatafile/c.mmult_top.autotvout_columnIndex_fea3_offset.dat"
#define AUTOTB_TVIN_columnIndex_fea4_offset "../tv/cdatafile/c.mmult_top.autotvin_columnIndex_fea4_offset.dat"
#define AUTOTB_TVOUT_columnIndex_fea4_offset "../tv/cdatafile/c.mmult_top.autotvout_columnIndex_fea4_offset.dat"
#define AUTOTB_TVIN_values_fea1_offset "../tv/cdatafile/c.mmult_top.autotvin_values_fea1_offset.dat"
#define AUTOTB_TVOUT_values_fea1_offset "../tv/cdatafile/c.mmult_top.autotvout_values_fea1_offset.dat"
#define AUTOTB_TVIN_values_fea2_offset "../tv/cdatafile/c.mmult_top.autotvin_values_fea2_offset.dat"
#define AUTOTB_TVOUT_values_fea2_offset "../tv/cdatafile/c.mmult_top.autotvout_values_fea2_offset.dat"
#define AUTOTB_TVIN_values_fea3_offset "../tv/cdatafile/c.mmult_top.autotvin_values_fea3_offset.dat"
#define AUTOTB_TVOUT_values_fea3_offset "../tv/cdatafile/c.mmult_top.autotvout_values_fea3_offset.dat"
#define AUTOTB_TVIN_values_fea4_offset "../tv/cdatafile/c.mmult_top.autotvin_values_fea4_offset.dat"
#define AUTOTB_TVOUT_values_fea4_offset "../tv/cdatafile/c.mmult_top.autotvout_values_fea4_offset.dat"
#define AUTOTB_TVIN_rowPtr_adj1_offset "../tv/cdatafile/c.mmult_top.autotvin_rowPtr_adj1_offset.dat"
#define AUTOTB_TVOUT_rowPtr_adj1_offset "../tv/cdatafile/c.mmult_top.autotvout_rowPtr_adj1_offset.dat"
#define AUTOTB_TVIN_rowPtr_adj2_offset "../tv/cdatafile/c.mmult_top.autotvin_rowPtr_adj2_offset.dat"
#define AUTOTB_TVOUT_rowPtr_adj2_offset "../tv/cdatafile/c.mmult_top.autotvout_rowPtr_adj2_offset.dat"
#define AUTOTB_TVIN_rowPtr_adj3_offset "../tv/cdatafile/c.mmult_top.autotvin_rowPtr_adj3_offset.dat"
#define AUTOTB_TVOUT_rowPtr_adj3_offset "../tv/cdatafile/c.mmult_top.autotvout_rowPtr_adj3_offset.dat"
#define AUTOTB_TVIN_rowPtr_adj4_offset "../tv/cdatafile/c.mmult_top.autotvin_rowPtr_adj4_offset.dat"
#define AUTOTB_TVOUT_rowPtr_adj4_offset "../tv/cdatafile/c.mmult_top.autotvout_rowPtr_adj4_offset.dat"
#define AUTOTB_TVIN_columnIndex_adj1_offset "../tv/cdatafile/c.mmult_top.autotvin_columnIndex_adj1_offset.dat"
#define AUTOTB_TVOUT_columnIndex_adj1_offset "../tv/cdatafile/c.mmult_top.autotvout_columnIndex_adj1_offset.dat"
#define AUTOTB_TVIN_columnIndex_adj2_offset "../tv/cdatafile/c.mmult_top.autotvin_columnIndex_adj2_offset.dat"
#define AUTOTB_TVOUT_columnIndex_adj2_offset "../tv/cdatafile/c.mmult_top.autotvout_columnIndex_adj2_offset.dat"
#define AUTOTB_TVIN_columnIndex_adj3_offset "../tv/cdatafile/c.mmult_top.autotvin_columnIndex_adj3_offset.dat"
#define AUTOTB_TVOUT_columnIndex_adj3_offset "../tv/cdatafile/c.mmult_top.autotvout_columnIndex_adj3_offset.dat"
#define AUTOTB_TVIN_columnIndex_adj4_offset "../tv/cdatafile/c.mmult_top.autotvin_columnIndex_adj4_offset.dat"
#define AUTOTB_TVOUT_columnIndex_adj4_offset "../tv/cdatafile/c.mmult_top.autotvout_columnIndex_adj4_offset.dat"
#define AUTOTB_TVIN_values_adj1_offset "../tv/cdatafile/c.mmult_top.autotvin_values_adj1_offset.dat"
#define AUTOTB_TVOUT_values_adj1_offset "../tv/cdatafile/c.mmult_top.autotvout_values_adj1_offset.dat"
#define AUTOTB_TVIN_values_adj2_offset "../tv/cdatafile/c.mmult_top.autotvin_values_adj2_offset.dat"
#define AUTOTB_TVOUT_values_adj2_offset "../tv/cdatafile/c.mmult_top.autotvout_values_adj2_offset.dat"
#define AUTOTB_TVIN_values_adj3_offset "../tv/cdatafile/c.mmult_top.autotvin_values_adj3_offset.dat"
#define AUTOTB_TVOUT_values_adj3_offset "../tv/cdatafile/c.mmult_top.autotvout_values_adj3_offset.dat"
#define AUTOTB_TVIN_values_adj4_offset "../tv/cdatafile/c.mmult_top.autotvin_values_adj4_offset.dat"
#define AUTOTB_TVOUT_values_adj4_offset "../tv/cdatafile/c.mmult_top.autotvout_values_adj4_offset.dat"
#define AUTOTB_TVIN_B "../tv/cdatafile/c.mmult_top.autotvin_B.dat"
#define AUTOTB_TVOUT_B "../tv/cdatafile/c.mmult_top.autotvout_B.dat"
#define AUTOTB_TVIN_D1 "../tv/cdatafile/c.mmult_top.autotvin_D1.dat"
#define AUTOTB_TVOUT_D1 "../tv/cdatafile/c.mmult_top.autotvout_D1.dat"
#define AUTOTB_TVIN_D2 "../tv/cdatafile/c.mmult_top.autotvin_D2.dat"
#define AUTOTB_TVOUT_D2 "../tv/cdatafile/c.mmult_top.autotvout_D2.dat"
#define AUTOTB_TVIN_D3 "../tv/cdatafile/c.mmult_top.autotvin_D3.dat"
#define AUTOTB_TVOUT_D3 "../tv/cdatafile/c.mmult_top.autotvout_D3.dat"
#define AUTOTB_TVIN_D4 "../tv/cdatafile/c.mmult_top.autotvin_D4.dat"
#define AUTOTB_TVOUT_D4 "../tv/cdatafile/c.mmult_top.autotvout_D4.dat"
#define AUTOTB_TVIN_bias "../tv/cdatafile/c.mmult_top.autotvin_bias.dat"
#define AUTOTB_TVOUT_bias "../tv/cdatafile/c.mmult_top.autotvout_bias.dat"
#define AUTOTB_TVIN_columnIndex_adj1 "../tv/cdatafile/c.mmult_top.autotvin_columnIndex_adj1.dat"
#define AUTOTB_TVOUT_columnIndex_adj1 "../tv/cdatafile/c.mmult_top.autotvout_columnIndex_adj1.dat"
#define AUTOTB_TVIN_columnIndex_adj2 "../tv/cdatafile/c.mmult_top.autotvin_columnIndex_adj2.dat"
#define AUTOTB_TVOUT_columnIndex_adj2 "../tv/cdatafile/c.mmult_top.autotvout_columnIndex_adj2.dat"
#define AUTOTB_TVIN_columnIndex_adj3 "../tv/cdatafile/c.mmult_top.autotvin_columnIndex_adj3.dat"
#define AUTOTB_TVOUT_columnIndex_adj3 "../tv/cdatafile/c.mmult_top.autotvout_columnIndex_adj3.dat"
#define AUTOTB_TVIN_columnIndex_adj4 "../tv/cdatafile/c.mmult_top.autotvin_columnIndex_adj4.dat"
#define AUTOTB_TVOUT_columnIndex_adj4 "../tv/cdatafile/c.mmult_top.autotvout_columnIndex_adj4.dat"
#define AUTOTB_TVIN_columnIndex_fea1 "../tv/cdatafile/c.mmult_top.autotvin_columnIndex_fea1.dat"
#define AUTOTB_TVOUT_columnIndex_fea1 "../tv/cdatafile/c.mmult_top.autotvout_columnIndex_fea1.dat"
#define AUTOTB_TVIN_columnIndex_fea2 "../tv/cdatafile/c.mmult_top.autotvin_columnIndex_fea2.dat"
#define AUTOTB_TVOUT_columnIndex_fea2 "../tv/cdatafile/c.mmult_top.autotvout_columnIndex_fea2.dat"
#define AUTOTB_TVIN_columnIndex_fea3 "../tv/cdatafile/c.mmult_top.autotvin_columnIndex_fea3.dat"
#define AUTOTB_TVOUT_columnIndex_fea3 "../tv/cdatafile/c.mmult_top.autotvout_columnIndex_fea3.dat"
#define AUTOTB_TVIN_columnIndex_fea4 "../tv/cdatafile/c.mmult_top.autotvin_columnIndex_fea4.dat"
#define AUTOTB_TVOUT_columnIndex_fea4 "../tv/cdatafile/c.mmult_top.autotvout_columnIndex_fea4.dat"
#define AUTOTB_TVIN_profiling "../tv/cdatafile/c.mmult_top.autotvin_profiling.dat"
#define AUTOTB_TVOUT_profiling "../tv/cdatafile/c.mmult_top.autotvout_profiling.dat"
#define AUTOTB_TVIN_quantized_multiplier "../tv/cdatafile/c.mmult_top.autotvin_quantized_multiplier.dat"
#define AUTOTB_TVOUT_quantized_multiplier "../tv/cdatafile/c.mmult_top.autotvout_quantized_multiplier.dat"
#define AUTOTB_TVIN_rowPtr_adj1 "../tv/cdatafile/c.mmult_top.autotvin_rowPtr_adj1.dat"
#define AUTOTB_TVOUT_rowPtr_adj1 "../tv/cdatafile/c.mmult_top.autotvout_rowPtr_adj1.dat"
#define AUTOTB_TVIN_rowPtr_adj2 "../tv/cdatafile/c.mmult_top.autotvin_rowPtr_adj2.dat"
#define AUTOTB_TVOUT_rowPtr_adj2 "../tv/cdatafile/c.mmult_top.autotvout_rowPtr_adj2.dat"
#define AUTOTB_TVIN_rowPtr_adj3 "../tv/cdatafile/c.mmult_top.autotvin_rowPtr_adj3.dat"
#define AUTOTB_TVOUT_rowPtr_adj3 "../tv/cdatafile/c.mmult_top.autotvout_rowPtr_adj3.dat"
#define AUTOTB_TVIN_rowPtr_adj4 "../tv/cdatafile/c.mmult_top.autotvin_rowPtr_adj4.dat"
#define AUTOTB_TVOUT_rowPtr_adj4 "../tv/cdatafile/c.mmult_top.autotvout_rowPtr_adj4.dat"
#define AUTOTB_TVIN_rowPtr_fea1 "../tv/cdatafile/c.mmult_top.autotvin_rowPtr_fea1.dat"
#define AUTOTB_TVOUT_rowPtr_fea1 "../tv/cdatafile/c.mmult_top.autotvout_rowPtr_fea1.dat"
#define AUTOTB_TVIN_rowPtr_fea2 "../tv/cdatafile/c.mmult_top.autotvin_rowPtr_fea2.dat"
#define AUTOTB_TVOUT_rowPtr_fea2 "../tv/cdatafile/c.mmult_top.autotvout_rowPtr_fea2.dat"
#define AUTOTB_TVIN_rowPtr_fea3 "../tv/cdatafile/c.mmult_top.autotvin_rowPtr_fea3.dat"
#define AUTOTB_TVOUT_rowPtr_fea3 "../tv/cdatafile/c.mmult_top.autotvout_rowPtr_fea3.dat"
#define AUTOTB_TVIN_rowPtr_fea4 "../tv/cdatafile/c.mmult_top.autotvin_rowPtr_fea4.dat"
#define AUTOTB_TVOUT_rowPtr_fea4 "../tv/cdatafile/c.mmult_top.autotvout_rowPtr_fea4.dat"
#define AUTOTB_TVIN_shift "../tv/cdatafile/c.mmult_top.autotvin_shift.dat"
#define AUTOTB_TVOUT_shift "../tv/cdatafile/c.mmult_top.autotvout_shift.dat"
#define AUTOTB_TVIN_values_adj1 "../tv/cdatafile/c.mmult_top.autotvin_values_adj1.dat"
#define AUTOTB_TVOUT_values_adj1 "../tv/cdatafile/c.mmult_top.autotvout_values_adj1.dat"
#define AUTOTB_TVIN_values_adj2 "../tv/cdatafile/c.mmult_top.autotvin_values_adj2.dat"
#define AUTOTB_TVOUT_values_adj2 "../tv/cdatafile/c.mmult_top.autotvout_values_adj2.dat"
#define AUTOTB_TVIN_values_adj3 "../tv/cdatafile/c.mmult_top.autotvin_values_adj3.dat"
#define AUTOTB_TVOUT_values_adj3 "../tv/cdatafile/c.mmult_top.autotvout_values_adj3.dat"
#define AUTOTB_TVIN_values_adj4 "../tv/cdatafile/c.mmult_top.autotvin_values_adj4.dat"
#define AUTOTB_TVOUT_values_adj4 "../tv/cdatafile/c.mmult_top.autotvout_values_adj4.dat"
#define AUTOTB_TVIN_values_fea1 "../tv/cdatafile/c.mmult_top.autotvin_values_fea1.dat"
#define AUTOTB_TVOUT_values_fea1 "../tv/cdatafile/c.mmult_top.autotvout_values_fea1.dat"
#define AUTOTB_TVIN_values_fea2 "../tv/cdatafile/c.mmult_top.autotvin_values_fea2.dat"
#define AUTOTB_TVOUT_values_fea2 "../tv/cdatafile/c.mmult_top.autotvout_values_fea2.dat"
#define AUTOTB_TVIN_values_fea3 "../tv/cdatafile/c.mmult_top.autotvin_values_fea3.dat"
#define AUTOTB_TVOUT_values_fea3 "../tv/cdatafile/c.mmult_top.autotvout_values_fea3.dat"
#define AUTOTB_TVIN_values_fea4 "../tv/cdatafile/c.mmult_top.autotvin_values_fea4.dat"
#define AUTOTB_TVOUT_values_fea4 "../tv/cdatafile/c.mmult_top.autotvout_values_fea4.dat"

#define INTER_TCL "../tv/cdatafile/ref.tcl"

// tvout file define:
#define AUTOTB_TVOUT_PC_gemm_mode "../tv/rtldatafile/rtl.mmult_top.autotvout_gemm_mode.dat"
#define AUTOTB_TVOUT_PC_quantized_multiplier_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_quantized_multiplier_offset.dat"
#define AUTOTB_TVOUT_PC_shift_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_shift_offset.dat"
#define AUTOTB_TVOUT_PC_bias_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_bias_offset.dat"
#define AUTOTB_TVOUT_PC_bias_count "../tv/rtldatafile/rtl.mmult_top.autotvout_bias_count.dat"
#define AUTOTB_TVOUT_PC_profiling_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_profiling_offset.dat"
#define AUTOTB_TVOUT_PC_zero_point_lhs "../tv/rtldatafile/rtl.mmult_top.autotvout_zero_point_lhs.dat"
#define AUTOTB_TVOUT_PC_zero_point_rhs "../tv/rtldatafile/rtl.mmult_top.autotvout_zero_point_rhs.dat"
#define AUTOTB_TVOUT_PC_zero_point_dst "../tv/rtldatafile/rtl.mmult_top.autotvout_zero_point_dst.dat"
#define AUTOTB_TVOUT_PC_clamp_max "../tv/rtldatafile/rtl.mmult_top.autotvout_clamp_max.dat"
#define AUTOTB_TVOUT_PC_clamp_min "../tv/rtldatafile/rtl.mmult_top.autotvout_clamp_min.dat"
#define AUTOTB_TVOUT_PC_N_adj "../tv/rtldatafile/rtl.mmult_top.autotvout_N_adj.dat"
#define AUTOTB_TVOUT_PC_M_adj "../tv/rtldatafile/rtl.mmult_top.autotvout_M_adj.dat"
#define AUTOTB_TVOUT_PC_M_fea "../tv/rtldatafile/rtl.mmult_top.autotvout_M_fea.dat"
#define AUTOTB_TVOUT_PC_P_w "../tv/rtldatafile/rtl.mmult_top.autotvout_P_w.dat"
#define AUTOTB_TVOUT_PC_B_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_B_offset.dat"
#define AUTOTB_TVOUT_PC_D1_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_D1_offset.dat"
#define AUTOTB_TVOUT_PC_D2_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_D2_offset.dat"
#define AUTOTB_TVOUT_PC_D3_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_D3_offset.dat"
#define AUTOTB_TVOUT_PC_D4_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_D4_offset.dat"
#define AUTOTB_TVOUT_PC_array_c_adjust "../tv/rtldatafile/rtl.mmult_top.autotvout_array_c_adjust.dat"
#define AUTOTB_TVOUT_PC_rowPtr_fea1_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_rowPtr_fea1_offset.dat"
#define AUTOTB_TVOUT_PC_rowPtr_fea2_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_rowPtr_fea2_offset.dat"
#define AUTOTB_TVOUT_PC_rowPtr_fea3_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_rowPtr_fea3_offset.dat"
#define AUTOTB_TVOUT_PC_rowPtr_fea4_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_rowPtr_fea4_offset.dat"
#define AUTOTB_TVOUT_PC_columnIndex_fea1_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_columnIndex_fea1_offset.dat"
#define AUTOTB_TVOUT_PC_columnIndex_fea2_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_columnIndex_fea2_offset.dat"
#define AUTOTB_TVOUT_PC_columnIndex_fea3_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_columnIndex_fea3_offset.dat"
#define AUTOTB_TVOUT_PC_columnIndex_fea4_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_columnIndex_fea4_offset.dat"
#define AUTOTB_TVOUT_PC_values_fea1_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_values_fea1_offset.dat"
#define AUTOTB_TVOUT_PC_values_fea2_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_values_fea2_offset.dat"
#define AUTOTB_TVOUT_PC_values_fea3_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_values_fea3_offset.dat"
#define AUTOTB_TVOUT_PC_values_fea4_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_values_fea4_offset.dat"
#define AUTOTB_TVOUT_PC_rowPtr_adj1_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_rowPtr_adj1_offset.dat"
#define AUTOTB_TVOUT_PC_rowPtr_adj2_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_rowPtr_adj2_offset.dat"
#define AUTOTB_TVOUT_PC_rowPtr_adj3_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_rowPtr_adj3_offset.dat"
#define AUTOTB_TVOUT_PC_rowPtr_adj4_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_rowPtr_adj4_offset.dat"
#define AUTOTB_TVOUT_PC_columnIndex_adj1_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_columnIndex_adj1_offset.dat"
#define AUTOTB_TVOUT_PC_columnIndex_adj2_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_columnIndex_adj2_offset.dat"
#define AUTOTB_TVOUT_PC_columnIndex_adj3_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_columnIndex_adj3_offset.dat"
#define AUTOTB_TVOUT_PC_columnIndex_adj4_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_columnIndex_adj4_offset.dat"
#define AUTOTB_TVOUT_PC_values_adj1_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_values_adj1_offset.dat"
#define AUTOTB_TVOUT_PC_values_adj2_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_values_adj2_offset.dat"
#define AUTOTB_TVOUT_PC_values_adj3_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_values_adj3_offset.dat"
#define AUTOTB_TVOUT_PC_values_adj4_offset "../tv/rtldatafile/rtl.mmult_top.autotvout_values_adj4_offset.dat"
#define AUTOTB_TVOUT_PC_B "../tv/rtldatafile/rtl.mmult_top.autotvout_B.dat"
#define AUTOTB_TVOUT_PC_D1 "../tv/rtldatafile/rtl.mmult_top.autotvout_D1.dat"
#define AUTOTB_TVOUT_PC_D2 "../tv/rtldatafile/rtl.mmult_top.autotvout_D2.dat"
#define AUTOTB_TVOUT_PC_D3 "../tv/rtldatafile/rtl.mmult_top.autotvout_D3.dat"
#define AUTOTB_TVOUT_PC_D4 "../tv/rtldatafile/rtl.mmult_top.autotvout_D4.dat"
#define AUTOTB_TVOUT_PC_bias "../tv/rtldatafile/rtl.mmult_top.autotvout_bias.dat"
#define AUTOTB_TVOUT_PC_columnIndex_adj1 "../tv/rtldatafile/rtl.mmult_top.autotvout_columnIndex_adj1.dat"
#define AUTOTB_TVOUT_PC_columnIndex_adj2 "../tv/rtldatafile/rtl.mmult_top.autotvout_columnIndex_adj2.dat"
#define AUTOTB_TVOUT_PC_columnIndex_adj3 "../tv/rtldatafile/rtl.mmult_top.autotvout_columnIndex_adj3.dat"
#define AUTOTB_TVOUT_PC_columnIndex_adj4 "../tv/rtldatafile/rtl.mmult_top.autotvout_columnIndex_adj4.dat"
#define AUTOTB_TVOUT_PC_columnIndex_fea1 "../tv/rtldatafile/rtl.mmult_top.autotvout_columnIndex_fea1.dat"
#define AUTOTB_TVOUT_PC_columnIndex_fea2 "../tv/rtldatafile/rtl.mmult_top.autotvout_columnIndex_fea2.dat"
#define AUTOTB_TVOUT_PC_columnIndex_fea3 "../tv/rtldatafile/rtl.mmult_top.autotvout_columnIndex_fea3.dat"
#define AUTOTB_TVOUT_PC_columnIndex_fea4 "../tv/rtldatafile/rtl.mmult_top.autotvout_columnIndex_fea4.dat"
#define AUTOTB_TVOUT_PC_profiling "../tv/rtldatafile/rtl.mmult_top.autotvout_profiling.dat"
#define AUTOTB_TVOUT_PC_quantized_multiplier "../tv/rtldatafile/rtl.mmult_top.autotvout_quantized_multiplier.dat"
#define AUTOTB_TVOUT_PC_rowPtr_adj1 "../tv/rtldatafile/rtl.mmult_top.autotvout_rowPtr_adj1.dat"
#define AUTOTB_TVOUT_PC_rowPtr_adj2 "../tv/rtldatafile/rtl.mmult_top.autotvout_rowPtr_adj2.dat"
#define AUTOTB_TVOUT_PC_rowPtr_adj3 "../tv/rtldatafile/rtl.mmult_top.autotvout_rowPtr_adj3.dat"
#define AUTOTB_TVOUT_PC_rowPtr_adj4 "../tv/rtldatafile/rtl.mmult_top.autotvout_rowPtr_adj4.dat"
#define AUTOTB_TVOUT_PC_rowPtr_fea1 "../tv/rtldatafile/rtl.mmult_top.autotvout_rowPtr_fea1.dat"
#define AUTOTB_TVOUT_PC_rowPtr_fea2 "../tv/rtldatafile/rtl.mmult_top.autotvout_rowPtr_fea2.dat"
#define AUTOTB_TVOUT_PC_rowPtr_fea3 "../tv/rtldatafile/rtl.mmult_top.autotvout_rowPtr_fea3.dat"
#define AUTOTB_TVOUT_PC_rowPtr_fea4 "../tv/rtldatafile/rtl.mmult_top.autotvout_rowPtr_fea4.dat"
#define AUTOTB_TVOUT_PC_shift "../tv/rtldatafile/rtl.mmult_top.autotvout_shift.dat"
#define AUTOTB_TVOUT_PC_values_adj1 "../tv/rtldatafile/rtl.mmult_top.autotvout_values_adj1.dat"
#define AUTOTB_TVOUT_PC_values_adj2 "../tv/rtldatafile/rtl.mmult_top.autotvout_values_adj2.dat"
#define AUTOTB_TVOUT_PC_values_adj3 "../tv/rtldatafile/rtl.mmult_top.autotvout_values_adj3.dat"
#define AUTOTB_TVOUT_PC_values_adj4 "../tv/rtldatafile/rtl.mmult_top.autotvout_values_adj4.dat"
#define AUTOTB_TVOUT_PC_values_fea1 "../tv/rtldatafile/rtl.mmult_top.autotvout_values_fea1.dat"
#define AUTOTB_TVOUT_PC_values_fea2 "../tv/rtldatafile/rtl.mmult_top.autotvout_values_fea2.dat"
#define AUTOTB_TVOUT_PC_values_fea3 "../tv/rtldatafile/rtl.mmult_top.autotvout_values_fea3.dat"
#define AUTOTB_TVOUT_PC_values_fea4 "../tv/rtldatafile/rtl.mmult_top.autotvout_values_fea4.dat"


static const bool little_endian()
{
  int a = 1;
  return *(char*)&a == 1;
}

inline static void rev_endian(char* p, size_t nbytes)
{
  std::reverse(p, p+nbytes);
}

template<size_t bit_width>
struct transaction {
  typedef uint64_t depth_t;
  static const size_t wbytes = (bit_width+7)>>3;
  static const size_t dbytes = sizeof(depth_t);
  const depth_t depth;
  const size_t vbytes;
  const size_t tbytes;
  char * const p;
  typedef char (*p_dat)[wbytes];
  p_dat vp;

  transaction(depth_t depth)
    : depth(depth), vbytes(wbytes*depth), tbytes(dbytes+vbytes),
      p(new char[tbytes]) {
    *(depth_t*)p = depth;
    rev_endian(p, dbytes);
    vp = (p_dat) (p+dbytes);
  }

  void reorder() {
    rev_endian(p, dbytes);
    p_dat vp = (p_dat) (p+dbytes);
    for (depth_t i = 0; i < depth; ++i) {
      rev_endian(vp[i], wbytes);
    }
  }

  template<size_t psize>
  void import(char* param, depth_t num, int64_t offset) {
    param -= offset*psize;
    for (depth_t i = 0; i < num; ++i) {
      memcpy(vp[i], param, wbytes);
      param += psize;
      if (little_endian()) {
        rev_endian(vp[i], wbytes);
      }
    }
    vp += num;
  }

  template<size_t psize>
  void send(char* param, depth_t num) {
    for (depth_t i = 0; i < num; ++i) {
      memcpy(param, vp[i], wbytes);
      param += psize;
    }
    vp += num;
  }

  template<size_t psize>
  void send(char* param, depth_t num, int64_t skip) {
    for (depth_t i = 0; i < num; ++i) {
      memcpy(param, vp[skip+i], wbytes);
      param += psize;
    }
  }

  ~transaction() { if (p) { delete[] p; } }
};


inline static const std::string begin_str(int num)
{
  return std::string("[[transaction]]           ")
         .append(std::to_string(num))
         .append("\n");
}

inline static const std::string end_str()
{
  return std::string("[[/transaction]]\n");
}

const std::string formatData(unsigned char *pos, size_t wbits)
{
  bool LE = little_endian();
  size_t wbytes = (wbits+7)>>3;
  size_t i = LE ? wbytes-1 : 0;
  auto next = [&] () {
    auto c = pos[i];
    LE ? --i : ++i;
    return c;
  };
  std::ostringstream ss;
  ss << "0x";
  if (int t = (wbits & 0x7)) {
    if (t <= 4) {
      unsigned char mask = (1<<t)-1;
      ss << std::hex << std::setfill('0') << std::setw(1)
         << (int) (next() & mask);
      wbytes -= 1;
    }
  }
  for (size_t i = 0; i < wbytes; ++i) {
    ss << std::hex << std::setfill('0') << std::setw(2) << (int)next();
  }
  ss.put('\n');
  return ss.str();
}

static bool RTLOutputCheckAndReplacement(std::string &data)
{
  bool changed = false;
  for (size_t i = 2; i < data.size(); ++i) {
    if (data[i] == 'X' || data[i] == 'x') {
      data[i] = '0';
      changed = true;
    }
  }
  return changed;
}

struct SimException : public std::exception {
  const char *msg;
  const size_t line;
  SimException(const char *msg, const size_t line)
    : msg(msg), line(line)
  {
  }
};

template<size_t bit_width>
class PostCheck
{
  static const char *bad;
  static const char *err;
  std::fstream stream;
  std::string s;

  void send(char *p, ap_uint<bit_width> &data, size_t l, size_t rest)
  {
    if (rest == 0) {
      if (!little_endian()) {
        const size_t wbytes = (bit_width+7)>>3;
        rev_endian(p-wbytes, wbytes);
      }
    } else if (rest < 8) {
      *p = data.range(l+rest-1, l).to_uint();
      send(p+1, data, l+rest, 0);
    } else {
      *p = data.range(l+8-1, l).to_uint();
      send(p+1, data, l+8, rest-8);
    }
  }

  void readline()
  {
    std::getline(stream, s);
    if (stream.eof()) {
      throw SimException(bad, __LINE__);
    }
  }

public:
  char *param;
  size_t psize;
  size_t depth;

  PostCheck(const char *file)
  {
    stream.open(file);
    if (stream.fail()) {
      throw SimException(err, __LINE__);
    } else {
      readline();
      if (s != "[[[runtime]]]") {
        throw SimException(bad, __LINE__);
      }
    }
  }

  ~PostCheck() noexcept(false)
  {
    stream.close();
  }

  void run(size_t AESL_transaction_pc, size_t skip)
  {
    if (stream.peek() == '[') {
      readline();
    }

    for (size_t i = 0; i < skip; ++i) {
      readline();
    }

    bool foundX = false;
    for (size_t i = 0; i < depth; ++i) {
      readline();
      foundX |= RTLOutputCheckAndReplacement(s);
      ap_uint<bit_width> data(s.c_str(), 16);
      send(param+i*psize, data, 0, bit_width);
    }
    if (foundX) {
      std::cerr << "WARNING: [SIM 212-201] RTL produces unknown value "
                << "'x' or 'X' on some port, possible cause: "
                << "There are uninitialized variables in the design.\n";
    }

    if (stream.peek() == '[') {
      readline();
    }
  }
};

template<size_t bit_width>
const char* PostCheck<bit_width>::bad = "Bad TV file";

template<size_t bit_width>
const char* PostCheck<bit_width>::err = "Error on TV file";
      
class INTER_TCL_FILE {
  public:
INTER_TCL_FILE(const char* name) {
  mName = name; 
  gemm_mode_depth = 0;
  quantized_multiplier_offset_depth = 0;
  shift_offset_depth = 0;
  bias_offset_depth = 0;
  bias_count_depth = 0;
  profiling_offset_depth = 0;
  zero_point_lhs_depth = 0;
  zero_point_rhs_depth = 0;
  zero_point_dst_depth = 0;
  clamp_max_depth = 0;
  clamp_min_depth = 0;
  N_adj_depth = 0;
  M_adj_depth = 0;
  M_fea_depth = 0;
  P_w_depth = 0;
  B_offset_depth = 0;
  D1_offset_depth = 0;
  D2_offset_depth = 0;
  D3_offset_depth = 0;
  D4_offset_depth = 0;
  array_c_adjust_depth = 0;
  rowPtr_fea1_offset_depth = 0;
  rowPtr_fea2_offset_depth = 0;
  rowPtr_fea3_offset_depth = 0;
  rowPtr_fea4_offset_depth = 0;
  columnIndex_fea1_offset_depth = 0;
  columnIndex_fea2_offset_depth = 0;
  columnIndex_fea3_offset_depth = 0;
  columnIndex_fea4_offset_depth = 0;
  values_fea1_offset_depth = 0;
  values_fea2_offset_depth = 0;
  values_fea3_offset_depth = 0;
  values_fea4_offset_depth = 0;
  rowPtr_adj1_offset_depth = 0;
  rowPtr_adj2_offset_depth = 0;
  rowPtr_adj3_offset_depth = 0;
  rowPtr_adj4_offset_depth = 0;
  columnIndex_adj1_offset_depth = 0;
  columnIndex_adj2_offset_depth = 0;
  columnIndex_adj3_offset_depth = 0;
  columnIndex_adj4_offset_depth = 0;
  values_adj1_offset_depth = 0;
  values_adj2_offset_depth = 0;
  values_adj3_offset_depth = 0;
  values_adj4_offset_depth = 0;
  B_depth = 0;
  D1_depth = 0;
  D2_depth = 0;
  D3_depth = 0;
  D4_depth = 0;
  bias_depth = 0;
  columnIndex_adj1_depth = 0;
  columnIndex_adj2_depth = 0;
  columnIndex_adj3_depth = 0;
  columnIndex_adj4_depth = 0;
  columnIndex_fea1_depth = 0;
  columnIndex_fea2_depth = 0;
  columnIndex_fea3_depth = 0;
  columnIndex_fea4_depth = 0;
  profiling_depth = 0;
  quantized_multiplier_depth = 0;
  rowPtr_adj1_depth = 0;
  rowPtr_adj2_depth = 0;
  rowPtr_adj3_depth = 0;
  rowPtr_adj4_depth = 0;
  rowPtr_fea1_depth = 0;
  rowPtr_fea2_depth = 0;
  rowPtr_fea3_depth = 0;
  rowPtr_fea4_depth = 0;
  shift_depth = 0;
  values_adj1_depth = 0;
  values_adj2_depth = 0;
  values_adj3_depth = 0;
  values_adj4_depth = 0;
  values_fea1_depth = 0;
  values_fea2_depth = 0;
  values_fea3_depth = 0;
  values_fea4_depth = 0;
  trans_num =0;
}
~INTER_TCL_FILE() {
  mFile.open(mName);
  if (!mFile.good()) {
    cout << "Failed to open file ref.tcl" << endl;
    exit (1); 
  }
  string total_list = get_depth_list();
  mFile << "set depth_list {\n";
  mFile << total_list;
  mFile << "}\n";
  mFile << "set trans_num "<<trans_num<<endl;
  mFile.close();
}
string get_depth_list () {
  stringstream total_list;
  total_list << "{gemm_mode " << gemm_mode_depth << "}\n";
  total_list << "{quantized_multiplier_offset " << quantized_multiplier_offset_depth << "}\n";
  total_list << "{shift_offset " << shift_offset_depth << "}\n";
  total_list << "{bias_offset " << bias_offset_depth << "}\n";
  total_list << "{bias_count " << bias_count_depth << "}\n";
  total_list << "{profiling_offset " << profiling_offset_depth << "}\n";
  total_list << "{zero_point_lhs " << zero_point_lhs_depth << "}\n";
  total_list << "{zero_point_rhs " << zero_point_rhs_depth << "}\n";
  total_list << "{zero_point_dst " << zero_point_dst_depth << "}\n";
  total_list << "{clamp_max " << clamp_max_depth << "}\n";
  total_list << "{clamp_min " << clamp_min_depth << "}\n";
  total_list << "{N_adj " << N_adj_depth << "}\n";
  total_list << "{M_adj " << M_adj_depth << "}\n";
  total_list << "{M_fea " << M_fea_depth << "}\n";
  total_list << "{P_w " << P_w_depth << "}\n";
  total_list << "{B_offset " << B_offset_depth << "}\n";
  total_list << "{D1_offset " << D1_offset_depth << "}\n";
  total_list << "{D2_offset " << D2_offset_depth << "}\n";
  total_list << "{D3_offset " << D3_offset_depth << "}\n";
  total_list << "{D4_offset " << D4_offset_depth << "}\n";
  total_list << "{array_c_adjust " << array_c_adjust_depth << "}\n";
  total_list << "{rowPtr_fea1_offset " << rowPtr_fea1_offset_depth << "}\n";
  total_list << "{rowPtr_fea2_offset " << rowPtr_fea2_offset_depth << "}\n";
  total_list << "{rowPtr_fea3_offset " << rowPtr_fea3_offset_depth << "}\n";
  total_list << "{rowPtr_fea4_offset " << rowPtr_fea4_offset_depth << "}\n";
  total_list << "{columnIndex_fea1_offset " << columnIndex_fea1_offset_depth << "}\n";
  total_list << "{columnIndex_fea2_offset " << columnIndex_fea2_offset_depth << "}\n";
  total_list << "{columnIndex_fea3_offset " << columnIndex_fea3_offset_depth << "}\n";
  total_list << "{columnIndex_fea4_offset " << columnIndex_fea4_offset_depth << "}\n";
  total_list << "{values_fea1_offset " << values_fea1_offset_depth << "}\n";
  total_list << "{values_fea2_offset " << values_fea2_offset_depth << "}\n";
  total_list << "{values_fea3_offset " << values_fea3_offset_depth << "}\n";
  total_list << "{values_fea4_offset " << values_fea4_offset_depth << "}\n";
  total_list << "{rowPtr_adj1_offset " << rowPtr_adj1_offset_depth << "}\n";
  total_list << "{rowPtr_adj2_offset " << rowPtr_adj2_offset_depth << "}\n";
  total_list << "{rowPtr_adj3_offset " << rowPtr_adj3_offset_depth << "}\n";
  total_list << "{rowPtr_adj4_offset " << rowPtr_adj4_offset_depth << "}\n";
  total_list << "{columnIndex_adj1_offset " << columnIndex_adj1_offset_depth << "}\n";
  total_list << "{columnIndex_adj2_offset " << columnIndex_adj2_offset_depth << "}\n";
  total_list << "{columnIndex_adj3_offset " << columnIndex_adj3_offset_depth << "}\n";
  total_list << "{columnIndex_adj4_offset " << columnIndex_adj4_offset_depth << "}\n";
  total_list << "{values_adj1_offset " << values_adj1_offset_depth << "}\n";
  total_list << "{values_adj2_offset " << values_adj2_offset_depth << "}\n";
  total_list << "{values_adj3_offset " << values_adj3_offset_depth << "}\n";
  total_list << "{values_adj4_offset " << values_adj4_offset_depth << "}\n";
  total_list << "{B " << B_depth << "}\n";
  total_list << "{D1 " << D1_depth << "}\n";
  total_list << "{D2 " << D2_depth << "}\n";
  total_list << "{D3 " << D3_depth << "}\n";
  total_list << "{D4 " << D4_depth << "}\n";
  total_list << "{bias " << bias_depth << "}\n";
  total_list << "{columnIndex_adj1 " << columnIndex_adj1_depth << "}\n";
  total_list << "{columnIndex_adj2 " << columnIndex_adj2_depth << "}\n";
  total_list << "{columnIndex_adj3 " << columnIndex_adj3_depth << "}\n";
  total_list << "{columnIndex_adj4 " << columnIndex_adj4_depth << "}\n";
  total_list << "{columnIndex_fea1 " << columnIndex_fea1_depth << "}\n";
  total_list << "{columnIndex_fea2 " << columnIndex_fea2_depth << "}\n";
  total_list << "{columnIndex_fea3 " << columnIndex_fea3_depth << "}\n";
  total_list << "{columnIndex_fea4 " << columnIndex_fea4_depth << "}\n";
  total_list << "{profiling " << profiling_depth << "}\n";
  total_list << "{quantized_multiplier " << quantized_multiplier_depth << "}\n";
  total_list << "{rowPtr_adj1 " << rowPtr_adj1_depth << "}\n";
  total_list << "{rowPtr_adj2 " << rowPtr_adj2_depth << "}\n";
  total_list << "{rowPtr_adj3 " << rowPtr_adj3_depth << "}\n";
  total_list << "{rowPtr_adj4 " << rowPtr_adj4_depth << "}\n";
  total_list << "{rowPtr_fea1 " << rowPtr_fea1_depth << "}\n";
  total_list << "{rowPtr_fea2 " << rowPtr_fea2_depth << "}\n";
  total_list << "{rowPtr_fea3 " << rowPtr_fea3_depth << "}\n";
  total_list << "{rowPtr_fea4 " << rowPtr_fea4_depth << "}\n";
  total_list << "{shift " << shift_depth << "}\n";
  total_list << "{values_adj1 " << values_adj1_depth << "}\n";
  total_list << "{values_adj2 " << values_adj2_depth << "}\n";
  total_list << "{values_adj3 " << values_adj3_depth << "}\n";
  total_list << "{values_adj4 " << values_adj4_depth << "}\n";
  total_list << "{values_fea1 " << values_fea1_depth << "}\n";
  total_list << "{values_fea2 " << values_fea2_depth << "}\n";
  total_list << "{values_fea3 " << values_fea3_depth << "}\n";
  total_list << "{values_fea4 " << values_fea4_depth << "}\n";
  return total_list.str();
}
void set_num (int num , int* class_num) {
  (*class_num) = (*class_num) > num ? (*class_num) : num;
}
void set_string(std::string list, std::string* class_list) {
  (*class_list) = list;
}
  public:
    int gemm_mode_depth;
    int quantized_multiplier_offset_depth;
    int shift_offset_depth;
    int bias_offset_depth;
    int bias_count_depth;
    int profiling_offset_depth;
    int zero_point_lhs_depth;
    int zero_point_rhs_depth;
    int zero_point_dst_depth;
    int clamp_max_depth;
    int clamp_min_depth;
    int N_adj_depth;
    int M_adj_depth;
    int M_fea_depth;
    int P_w_depth;
    int B_offset_depth;
    int D1_offset_depth;
    int D2_offset_depth;
    int D3_offset_depth;
    int D4_offset_depth;
    int array_c_adjust_depth;
    int rowPtr_fea1_offset_depth;
    int rowPtr_fea2_offset_depth;
    int rowPtr_fea3_offset_depth;
    int rowPtr_fea4_offset_depth;
    int columnIndex_fea1_offset_depth;
    int columnIndex_fea2_offset_depth;
    int columnIndex_fea3_offset_depth;
    int columnIndex_fea4_offset_depth;
    int values_fea1_offset_depth;
    int values_fea2_offset_depth;
    int values_fea3_offset_depth;
    int values_fea4_offset_depth;
    int rowPtr_adj1_offset_depth;
    int rowPtr_adj2_offset_depth;
    int rowPtr_adj3_offset_depth;
    int rowPtr_adj4_offset_depth;
    int columnIndex_adj1_offset_depth;
    int columnIndex_adj2_offset_depth;
    int columnIndex_adj3_offset_depth;
    int columnIndex_adj4_offset_depth;
    int values_adj1_offset_depth;
    int values_adj2_offset_depth;
    int values_adj3_offset_depth;
    int values_adj4_offset_depth;
    int B_depth;
    int D1_depth;
    int D2_depth;
    int D3_depth;
    int D4_depth;
    int bias_depth;
    int columnIndex_adj1_depth;
    int columnIndex_adj2_depth;
    int columnIndex_adj3_depth;
    int columnIndex_adj4_depth;
    int columnIndex_fea1_depth;
    int columnIndex_fea2_depth;
    int columnIndex_fea3_depth;
    int columnIndex_fea4_depth;
    int profiling_depth;
    int quantized_multiplier_depth;
    int rowPtr_adj1_depth;
    int rowPtr_adj2_depth;
    int rowPtr_adj3_depth;
    int rowPtr_adj4_depth;
    int rowPtr_fea1_depth;
    int rowPtr_fea2_depth;
    int rowPtr_fea3_depth;
    int rowPtr_fea4_depth;
    int shift_depth;
    int values_adj1_depth;
    int values_adj2_depth;
    int values_adj3_depth;
    int values_adj4_depth;
    int values_fea1_depth;
    int values_fea2_depth;
    int values_fea3_depth;
    int values_fea4_depth;
    int trans_num;
  private:
    ofstream mFile;
    const char* mName;
};


struct __cosim_s4__ { char data[4]; };
struct __cosim_s1__ { char data[1]; };
struct __cosim_s2__ { char data[2]; };
extern "C" void mmult_top_hw_stub_wrapper(char, volatile void *, volatile void *, volatile void *, __cosim_s4__*, volatile void *, __cosim_s1__*, __cosim_s1__*, __cosim_s1__*, __cosim_s1__*, __cosim_s1__*, int, int, int, int, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, int, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *, volatile void *);

extern "C" void apatb_mmult_top_hw(char __xlx_apatb_param_gemm_mode, volatile void * __xlx_apatb_param_quantized_multiplier, volatile void * __xlx_apatb_param_shift, volatile void * __xlx_apatb_param_bias, __cosim_s4__* __xlx_apatb_param_bias_count, volatile void * __xlx_apatb_param_profiling, __cosim_s1__* __xlx_apatb_param_zero_point_lhs, __cosim_s1__* __xlx_apatb_param_zero_point_rhs, __cosim_s1__* __xlx_apatb_param_zero_point_dst, __cosim_s1__* __xlx_apatb_param_clamp_max, __cosim_s1__* __xlx_apatb_param_clamp_min, int __xlx_apatb_param_N_adj, int __xlx_apatb_param_M_adj, int __xlx_apatb_param_M_fea, int __xlx_apatb_param_P_w, volatile void * __xlx_apatb_param_B, volatile void * __xlx_apatb_param_D1, volatile void * __xlx_apatb_param_D2, volatile void * __xlx_apatb_param_D3, volatile void * __xlx_apatb_param_D4, int __xlx_apatb_param_array_c_adjust, volatile void * __xlx_apatb_param_rowPtr_fea1, volatile void * __xlx_apatb_param_rowPtr_fea2, volatile void * __xlx_apatb_param_rowPtr_fea3, volatile void * __xlx_apatb_param_rowPtr_fea4, volatile void * __xlx_apatb_param_columnIndex_fea1, volatile void * __xlx_apatb_param_columnIndex_fea2, volatile void * __xlx_apatb_param_columnIndex_fea3, volatile void * __xlx_apatb_param_columnIndex_fea4, volatile void * __xlx_apatb_param_values_fea1, volatile void * __xlx_apatb_param_values_fea2, volatile void * __xlx_apatb_param_values_fea3, volatile void * __xlx_apatb_param_values_fea4, volatile void * __xlx_apatb_param_rowPtr_adj1, volatile void * __xlx_apatb_param_rowPtr_adj2, volatile void * __xlx_apatb_param_rowPtr_adj3, volatile void * __xlx_apatb_param_rowPtr_adj4, volatile void * __xlx_apatb_param_columnIndex_adj1, volatile void * __xlx_apatb_param_columnIndex_adj2, volatile void * __xlx_apatb_param_columnIndex_adj3, volatile void * __xlx_apatb_param_columnIndex_adj4, volatile void * __xlx_apatb_param_values_adj1, volatile void * __xlx_apatb_param_values_adj2, volatile void * __xlx_apatb_param_values_adj3, volatile void * __xlx_apatb_param_values_adj4) {
  refine_signal_handler();
  fstream wrapc_switch_file_token;
  wrapc_switch_file_token.open(".hls_cosim_wrapc_switch.log");
static AESL_FILE_HANDLER aesl_fh;
  int AESL_i;
  if (wrapc_switch_file_token.good())
  {

    CodeState = ENTER_WRAPC_PC;
    static unsigned AESL_transaction_pc = 0;
    string AESL_token;
    string AESL_num;
#ifdef USE_BINARY_TV_FILE
{
transaction<16> tr(128000);
aesl_fh.read(AUTOTB_TVOUT_PC_D1, tr.p, tr.tbytes);
if (little_endian()) { tr.reorder(); }
tr.send<2>((char*)__xlx_apatb_param_D1, 128000, 0);
}
#else
try {
static PostCheck<16> pc(AUTOTB_TVOUT_PC_D1);
pc.psize = 2;
pc.param = (char*)__xlx_apatb_param_D1;
pc.depth = 128000;
pc.run(AESL_transaction_pc, 0);
} catch (SimException &e) {
  std::cout << "at line " << e.line << " occurred exception, " << e.msg << "\n";
}
#endif
#ifdef USE_BINARY_TV_FILE
{
transaction<64> tr(16);
aesl_fh.read(AUTOTB_TVOUT_PC_profiling, tr.p, tr.tbytes);
if (little_endian()) { tr.reorder(); }
tr.send<8>((char*)__xlx_apatb_param_profiling, 16, 0);
}
#else
try {
static PostCheck<64> pc(AUTOTB_TVOUT_PC_profiling);
pc.psize = 8;
pc.param = (char*)__xlx_apatb_param_profiling;
pc.depth = 16;
pc.run(AESL_transaction_pc, 0);
} catch (SimException &e) {
  std::cout << "at line " << e.line << " occurred exception, " << e.msg << "\n";
}
#endif

    AESL_transaction_pc++;
    return ;
  }
static unsigned AESL_transaction;
static INTER_TCL_FILE tcl_file(INTER_TCL);
std::vector<char> __xlx_sprintf_buffer(1024);
CodeState = ENTER_WRAPC;
CodeState = DUMP_INPUTS;
unsigned __xlx_offset_byte_param_quantized_multiplier = 0;
unsigned __xlx_offset_byte_param_shift = 0;
unsigned __xlx_offset_byte_param_bias = 0;
unsigned __xlx_offset_byte_param_profiling = 0;
unsigned __xlx_offset_byte_param_B = 0;
unsigned __xlx_offset_byte_param_D1 = 0;
unsigned __xlx_offset_byte_param_D2 = 0;
unsigned __xlx_offset_byte_param_D3 = 0;
unsigned __xlx_offset_byte_param_D4 = 0;
unsigned __xlx_offset_byte_param_rowPtr_fea1 = 0;
unsigned __xlx_offset_byte_param_rowPtr_fea2 = 0;
unsigned __xlx_offset_byte_param_rowPtr_fea3 = 0;
unsigned __xlx_offset_byte_param_rowPtr_fea4 = 0;
unsigned __xlx_offset_byte_param_columnIndex_fea1 = 0;
unsigned __xlx_offset_byte_param_columnIndex_fea2 = 0;
unsigned __xlx_offset_byte_param_columnIndex_fea3 = 0;
unsigned __xlx_offset_byte_param_columnIndex_fea4 = 0;
unsigned __xlx_offset_byte_param_values_fea1 = 0;
unsigned __xlx_offset_byte_param_values_fea2 = 0;
unsigned __xlx_offset_byte_param_values_fea3 = 0;
unsigned __xlx_offset_byte_param_values_fea4 = 0;
unsigned __xlx_offset_byte_param_rowPtr_adj1 = 0;
unsigned __xlx_offset_byte_param_rowPtr_adj2 = 0;
unsigned __xlx_offset_byte_param_rowPtr_adj3 = 0;
unsigned __xlx_offset_byte_param_rowPtr_adj4 = 0;
unsigned __xlx_offset_byte_param_columnIndex_adj1 = 0;
unsigned __xlx_offset_byte_param_columnIndex_adj2 = 0;
unsigned __xlx_offset_byte_param_columnIndex_adj3 = 0;
unsigned __xlx_offset_byte_param_columnIndex_adj4 = 0;
unsigned __xlx_offset_byte_param_values_adj1 = 0;
unsigned __xlx_offset_byte_param_values_adj2 = 0;
unsigned __xlx_offset_byte_param_values_adj3 = 0;
unsigned __xlx_offset_byte_param_values_adj4 = 0;
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_B, 'b');
transaction<16> tr(128000);
__xlx_offset_byte_param_B = 0*2;
if (__xlx_apatb_param_B) {
  tr.import<2>((char*)__xlx_apatb_param_B, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_B, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.B_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_B);
{
aesl_fh.write(AUTOTB_TVIN_B, begin_str(AESL_transaction));
__xlx_offset_byte_param_B = 0*2;
if (__xlx_apatb_param_B) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_B + i * 2;
std::string s = formatData(pos, 16);
aesl_fh.write(AUTOTB_TVIN_B, s);
}
}
tcl_file.set_num(128000, &tcl_file.B_depth);
aesl_fh.write(AUTOTB_TVIN_B, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_D1, 'b');
transaction<16> tr(128000);
__xlx_offset_byte_param_D1 = 0*2;
if (__xlx_apatb_param_D1) {
  tr.import<2>((char*)__xlx_apatb_param_D1, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_D1, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.D1_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_D1);
{
aesl_fh.write(AUTOTB_TVIN_D1, begin_str(AESL_transaction));
__xlx_offset_byte_param_D1 = 0*2;
if (__xlx_apatb_param_D1) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_D1 + i * 2;
std::string s = formatData(pos, 16);
aesl_fh.write(AUTOTB_TVIN_D1, s);
}
}
tcl_file.set_num(128000, &tcl_file.D1_depth);
aesl_fh.write(AUTOTB_TVIN_D1, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_D2, 'b');
transaction<16> tr(128000);
__xlx_offset_byte_param_D2 = 0*2;
if (__xlx_apatb_param_D2) {
  tr.import<2>((char*)__xlx_apatb_param_D2, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_D2, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.D2_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_D2);
{
aesl_fh.write(AUTOTB_TVIN_D2, begin_str(AESL_transaction));
__xlx_offset_byte_param_D2 = 0*2;
if (__xlx_apatb_param_D2) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_D2 + i * 2;
std::string s = formatData(pos, 16);
aesl_fh.write(AUTOTB_TVIN_D2, s);
}
}
tcl_file.set_num(128000, &tcl_file.D2_depth);
aesl_fh.write(AUTOTB_TVIN_D2, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_D3, 'b');
transaction<16> tr(128000);
__xlx_offset_byte_param_D3 = 0*2;
if (__xlx_apatb_param_D3) {
  tr.import<2>((char*)__xlx_apatb_param_D3, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_D3, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.D3_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_D3);
{
aesl_fh.write(AUTOTB_TVIN_D3, begin_str(AESL_transaction));
__xlx_offset_byte_param_D3 = 0*2;
if (__xlx_apatb_param_D3) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_D3 + i * 2;
std::string s = formatData(pos, 16);
aesl_fh.write(AUTOTB_TVIN_D3, s);
}
}
tcl_file.set_num(128000, &tcl_file.D3_depth);
aesl_fh.write(AUTOTB_TVIN_D3, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_D4, 'b');
transaction<16> tr(128000);
__xlx_offset_byte_param_D4 = 0*2;
if (__xlx_apatb_param_D4) {
  tr.import<2>((char*)__xlx_apatb_param_D4, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_D4, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.D4_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_D4);
{
aesl_fh.write(AUTOTB_TVIN_D4, begin_str(AESL_transaction));
__xlx_offset_byte_param_D4 = 0*2;
if (__xlx_apatb_param_D4) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_D4 + i * 2;
std::string s = formatData(pos, 16);
aesl_fh.write(AUTOTB_TVIN_D4, s);
}
}
tcl_file.set_num(128000, &tcl_file.D4_depth);
aesl_fh.write(AUTOTB_TVIN_D4, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_bias, 'b');
transaction<32> tr(1024);
__xlx_offset_byte_param_bias = 0*4;
if (__xlx_apatb_param_bias) {
  tr.import<4>((char*)__xlx_apatb_param_bias, 1024, 0);
}
aesl_fh.write(AUTOTB_TVIN_bias, tr.p, tr.tbytes);
tcl_file.set_num(1024, &tcl_file.bias_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_bias);
{
aesl_fh.write(AUTOTB_TVIN_bias, begin_str(AESL_transaction));
__xlx_offset_byte_param_bias = 0*4;
if (__xlx_apatb_param_bias) {
for (size_t i = 0; i < 1024; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_bias + i * 4;
std::string s = formatData(pos, 32);
aesl_fh.write(AUTOTB_TVIN_bias, s);
}
}
tcl_file.set_num(1024, &tcl_file.bias_depth);
aesl_fh.write(AUTOTB_TVIN_bias, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_columnIndex_adj1, 'b');
transaction<32> tr(128000);
__xlx_offset_byte_param_columnIndex_adj1 = 0*4;
if (__xlx_apatb_param_columnIndex_adj1) {
  tr.import<4>((char*)__xlx_apatb_param_columnIndex_adj1, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj1, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.columnIndex_adj1_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_columnIndex_adj1);
{
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj1, begin_str(AESL_transaction));
__xlx_offset_byte_param_columnIndex_adj1 = 0*4;
if (__xlx_apatb_param_columnIndex_adj1) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_columnIndex_adj1 + i * 4;
std::string s = formatData(pos, 32);
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj1, s);
}
}
tcl_file.set_num(128000, &tcl_file.columnIndex_adj1_depth);
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj1, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_columnIndex_adj2, 'b');
transaction<32> tr(128000);
__xlx_offset_byte_param_columnIndex_adj2 = 0*4;
if (__xlx_apatb_param_columnIndex_adj2) {
  tr.import<4>((char*)__xlx_apatb_param_columnIndex_adj2, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj2, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.columnIndex_adj2_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_columnIndex_adj2);
{
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj2, begin_str(AESL_transaction));
__xlx_offset_byte_param_columnIndex_adj2 = 0*4;
if (__xlx_apatb_param_columnIndex_adj2) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_columnIndex_adj2 + i * 4;
std::string s = formatData(pos, 32);
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj2, s);
}
}
tcl_file.set_num(128000, &tcl_file.columnIndex_adj2_depth);
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj2, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_columnIndex_adj3, 'b');
transaction<32> tr(128000);
__xlx_offset_byte_param_columnIndex_adj3 = 0*4;
if (__xlx_apatb_param_columnIndex_adj3) {
  tr.import<4>((char*)__xlx_apatb_param_columnIndex_adj3, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj3, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.columnIndex_adj3_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_columnIndex_adj3);
{
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj3, begin_str(AESL_transaction));
__xlx_offset_byte_param_columnIndex_adj3 = 0*4;
if (__xlx_apatb_param_columnIndex_adj3) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_columnIndex_adj3 + i * 4;
std::string s = formatData(pos, 32);
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj3, s);
}
}
tcl_file.set_num(128000, &tcl_file.columnIndex_adj3_depth);
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj3, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_columnIndex_adj4, 'b');
transaction<32> tr(128000);
__xlx_offset_byte_param_columnIndex_adj4 = 0*4;
if (__xlx_apatb_param_columnIndex_adj4) {
  tr.import<4>((char*)__xlx_apatb_param_columnIndex_adj4, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj4, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.columnIndex_adj4_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_columnIndex_adj4);
{
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj4, begin_str(AESL_transaction));
__xlx_offset_byte_param_columnIndex_adj4 = 0*4;
if (__xlx_apatb_param_columnIndex_adj4) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_columnIndex_adj4 + i * 4;
std::string s = formatData(pos, 32);
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj4, s);
}
}
tcl_file.set_num(128000, &tcl_file.columnIndex_adj4_depth);
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj4, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_columnIndex_fea1, 'b');
transaction<32> tr(128000);
__xlx_offset_byte_param_columnIndex_fea1 = 0*4;
if (__xlx_apatb_param_columnIndex_fea1) {
  tr.import<4>((char*)__xlx_apatb_param_columnIndex_fea1, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea1, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.columnIndex_fea1_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_columnIndex_fea1);
{
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea1, begin_str(AESL_transaction));
__xlx_offset_byte_param_columnIndex_fea1 = 0*4;
if (__xlx_apatb_param_columnIndex_fea1) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_columnIndex_fea1 + i * 4;
std::string s = formatData(pos, 32);
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea1, s);
}
}
tcl_file.set_num(128000, &tcl_file.columnIndex_fea1_depth);
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea1, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_columnIndex_fea2, 'b');
transaction<32> tr(128000);
__xlx_offset_byte_param_columnIndex_fea2 = 0*4;
if (__xlx_apatb_param_columnIndex_fea2) {
  tr.import<4>((char*)__xlx_apatb_param_columnIndex_fea2, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea2, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.columnIndex_fea2_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_columnIndex_fea2);
{
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea2, begin_str(AESL_transaction));
__xlx_offset_byte_param_columnIndex_fea2 = 0*4;
if (__xlx_apatb_param_columnIndex_fea2) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_columnIndex_fea2 + i * 4;
std::string s = formatData(pos, 32);
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea2, s);
}
}
tcl_file.set_num(128000, &tcl_file.columnIndex_fea2_depth);
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea2, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_columnIndex_fea3, 'b');
transaction<32> tr(128000);
__xlx_offset_byte_param_columnIndex_fea3 = 0*4;
if (__xlx_apatb_param_columnIndex_fea3) {
  tr.import<4>((char*)__xlx_apatb_param_columnIndex_fea3, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea3, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.columnIndex_fea3_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_columnIndex_fea3);
{
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea3, begin_str(AESL_transaction));
__xlx_offset_byte_param_columnIndex_fea3 = 0*4;
if (__xlx_apatb_param_columnIndex_fea3) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_columnIndex_fea3 + i * 4;
std::string s = formatData(pos, 32);
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea3, s);
}
}
tcl_file.set_num(128000, &tcl_file.columnIndex_fea3_depth);
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea3, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_columnIndex_fea4, 'b');
transaction<32> tr(128000);
__xlx_offset_byte_param_columnIndex_fea4 = 0*4;
if (__xlx_apatb_param_columnIndex_fea4) {
  tr.import<4>((char*)__xlx_apatb_param_columnIndex_fea4, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea4, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.columnIndex_fea4_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_columnIndex_fea4);
{
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea4, begin_str(AESL_transaction));
__xlx_offset_byte_param_columnIndex_fea4 = 0*4;
if (__xlx_apatb_param_columnIndex_fea4) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_columnIndex_fea4 + i * 4;
std::string s = formatData(pos, 32);
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea4, s);
}
}
tcl_file.set_num(128000, &tcl_file.columnIndex_fea4_depth);
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea4, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_profiling, 'b');
transaction<64> tr(16);
__xlx_offset_byte_param_profiling = 0*8;
if (__xlx_apatb_param_profiling) {
  tr.import<8>((char*)__xlx_apatb_param_profiling, 16, 0);
}
aesl_fh.write(AUTOTB_TVIN_profiling, tr.p, tr.tbytes);
tcl_file.set_num(16, &tcl_file.profiling_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_profiling);
{
aesl_fh.write(AUTOTB_TVIN_profiling, begin_str(AESL_transaction));
__xlx_offset_byte_param_profiling = 0*8;
if (__xlx_apatb_param_profiling) {
for (size_t i = 0; i < 16; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_profiling + i * 8;
std::string s = formatData(pos, 64);
aesl_fh.write(AUTOTB_TVIN_profiling, s);
}
}
tcl_file.set_num(16, &tcl_file.profiling_depth);
aesl_fh.write(AUTOTB_TVIN_profiling, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_quantized_multiplier, 'b');
transaction<32> tr(1024);
__xlx_offset_byte_param_quantized_multiplier = 0*4;
if (__xlx_apatb_param_quantized_multiplier) {
  tr.import<4>((char*)__xlx_apatb_param_quantized_multiplier, 1024, 0);
}
aesl_fh.write(AUTOTB_TVIN_quantized_multiplier, tr.p, tr.tbytes);
tcl_file.set_num(1024, &tcl_file.quantized_multiplier_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_quantized_multiplier);
{
aesl_fh.write(AUTOTB_TVIN_quantized_multiplier, begin_str(AESL_transaction));
__xlx_offset_byte_param_quantized_multiplier = 0*4;
if (__xlx_apatb_param_quantized_multiplier) {
for (size_t i = 0; i < 1024; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_quantized_multiplier + i * 4;
std::string s = formatData(pos, 32);
aesl_fh.write(AUTOTB_TVIN_quantized_multiplier, s);
}
}
tcl_file.set_num(1024, &tcl_file.quantized_multiplier_depth);
aesl_fh.write(AUTOTB_TVIN_quantized_multiplier, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_rowPtr_adj1, 'b');
transaction<32> tr(4096);
__xlx_offset_byte_param_rowPtr_adj1 = 0*4;
if (__xlx_apatb_param_rowPtr_adj1) {
  tr.import<4>((char*)__xlx_apatb_param_rowPtr_adj1, 4096, 0);
}
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj1, tr.p, tr.tbytes);
tcl_file.set_num(4096, &tcl_file.rowPtr_adj1_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_rowPtr_adj1);
{
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj1, begin_str(AESL_transaction));
__xlx_offset_byte_param_rowPtr_adj1 = 0*4;
if (__xlx_apatb_param_rowPtr_adj1) {
for (size_t i = 0; i < 4096; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_rowPtr_adj1 + i * 4;
std::string s = formatData(pos, 32);
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj1, s);
}
}
tcl_file.set_num(4096, &tcl_file.rowPtr_adj1_depth);
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj1, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_rowPtr_adj2, 'b');
transaction<32> tr(4096);
__xlx_offset_byte_param_rowPtr_adj2 = 0*4;
if (__xlx_apatb_param_rowPtr_adj2) {
  tr.import<4>((char*)__xlx_apatb_param_rowPtr_adj2, 4096, 0);
}
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj2, tr.p, tr.tbytes);
tcl_file.set_num(4096, &tcl_file.rowPtr_adj2_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_rowPtr_adj2);
{
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj2, begin_str(AESL_transaction));
__xlx_offset_byte_param_rowPtr_adj2 = 0*4;
if (__xlx_apatb_param_rowPtr_adj2) {
for (size_t i = 0; i < 4096; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_rowPtr_adj2 + i * 4;
std::string s = formatData(pos, 32);
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj2, s);
}
}
tcl_file.set_num(4096, &tcl_file.rowPtr_adj2_depth);
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj2, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_rowPtr_adj3, 'b');
transaction<32> tr(4096);
__xlx_offset_byte_param_rowPtr_adj3 = 0*4;
if (__xlx_apatb_param_rowPtr_adj3) {
  tr.import<4>((char*)__xlx_apatb_param_rowPtr_adj3, 4096, 0);
}
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj3, tr.p, tr.tbytes);
tcl_file.set_num(4096, &tcl_file.rowPtr_adj3_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_rowPtr_adj3);
{
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj3, begin_str(AESL_transaction));
__xlx_offset_byte_param_rowPtr_adj3 = 0*4;
if (__xlx_apatb_param_rowPtr_adj3) {
for (size_t i = 0; i < 4096; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_rowPtr_adj3 + i * 4;
std::string s = formatData(pos, 32);
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj3, s);
}
}
tcl_file.set_num(4096, &tcl_file.rowPtr_adj3_depth);
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj3, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_rowPtr_adj4, 'b');
transaction<32> tr(4096);
__xlx_offset_byte_param_rowPtr_adj4 = 0*4;
if (__xlx_apatb_param_rowPtr_adj4) {
  tr.import<4>((char*)__xlx_apatb_param_rowPtr_adj4, 4096, 0);
}
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj4, tr.p, tr.tbytes);
tcl_file.set_num(4096, &tcl_file.rowPtr_adj4_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_rowPtr_adj4);
{
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj4, begin_str(AESL_transaction));
__xlx_offset_byte_param_rowPtr_adj4 = 0*4;
if (__xlx_apatb_param_rowPtr_adj4) {
for (size_t i = 0; i < 4096; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_rowPtr_adj4 + i * 4;
std::string s = formatData(pos, 32);
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj4, s);
}
}
tcl_file.set_num(4096, &tcl_file.rowPtr_adj4_depth);
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj4, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_rowPtr_fea1, 'b');
transaction<32> tr(4096);
__xlx_offset_byte_param_rowPtr_fea1 = 0*4;
if (__xlx_apatb_param_rowPtr_fea1) {
  tr.import<4>((char*)__xlx_apatb_param_rowPtr_fea1, 4096, 0);
}
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea1, tr.p, tr.tbytes);
tcl_file.set_num(4096, &tcl_file.rowPtr_fea1_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_rowPtr_fea1);
{
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea1, begin_str(AESL_transaction));
__xlx_offset_byte_param_rowPtr_fea1 = 0*4;
if (__xlx_apatb_param_rowPtr_fea1) {
for (size_t i = 0; i < 4096; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_rowPtr_fea1 + i * 4;
std::string s = formatData(pos, 32);
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea1, s);
}
}
tcl_file.set_num(4096, &tcl_file.rowPtr_fea1_depth);
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea1, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_rowPtr_fea2, 'b');
transaction<32> tr(4096);
__xlx_offset_byte_param_rowPtr_fea2 = 0*4;
if (__xlx_apatb_param_rowPtr_fea2) {
  tr.import<4>((char*)__xlx_apatb_param_rowPtr_fea2, 4096, 0);
}
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea2, tr.p, tr.tbytes);
tcl_file.set_num(4096, &tcl_file.rowPtr_fea2_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_rowPtr_fea2);
{
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea2, begin_str(AESL_transaction));
__xlx_offset_byte_param_rowPtr_fea2 = 0*4;
if (__xlx_apatb_param_rowPtr_fea2) {
for (size_t i = 0; i < 4096; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_rowPtr_fea2 + i * 4;
std::string s = formatData(pos, 32);
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea2, s);
}
}
tcl_file.set_num(4096, &tcl_file.rowPtr_fea2_depth);
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea2, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_rowPtr_fea3, 'b');
transaction<32> tr(4096);
__xlx_offset_byte_param_rowPtr_fea3 = 0*4;
if (__xlx_apatb_param_rowPtr_fea3) {
  tr.import<4>((char*)__xlx_apatb_param_rowPtr_fea3, 4096, 0);
}
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea3, tr.p, tr.tbytes);
tcl_file.set_num(4096, &tcl_file.rowPtr_fea3_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_rowPtr_fea3);
{
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea3, begin_str(AESL_transaction));
__xlx_offset_byte_param_rowPtr_fea3 = 0*4;
if (__xlx_apatb_param_rowPtr_fea3) {
for (size_t i = 0; i < 4096; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_rowPtr_fea3 + i * 4;
std::string s = formatData(pos, 32);
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea3, s);
}
}
tcl_file.set_num(4096, &tcl_file.rowPtr_fea3_depth);
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea3, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_rowPtr_fea4, 'b');
transaction<32> tr(4096);
__xlx_offset_byte_param_rowPtr_fea4 = 0*4;
if (__xlx_apatb_param_rowPtr_fea4) {
  tr.import<4>((char*)__xlx_apatb_param_rowPtr_fea4, 4096, 0);
}
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea4, tr.p, tr.tbytes);
tcl_file.set_num(4096, &tcl_file.rowPtr_fea4_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_rowPtr_fea4);
{
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea4, begin_str(AESL_transaction));
__xlx_offset_byte_param_rowPtr_fea4 = 0*4;
if (__xlx_apatb_param_rowPtr_fea4) {
for (size_t i = 0; i < 4096; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_rowPtr_fea4 + i * 4;
std::string s = formatData(pos, 32);
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea4, s);
}
}
tcl_file.set_num(4096, &tcl_file.rowPtr_fea4_depth);
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea4, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_shift, 'b');
transaction<32> tr(1024);
__xlx_offset_byte_param_shift = 0*4;
if (__xlx_apatb_param_shift) {
  tr.import<4>((char*)__xlx_apatb_param_shift, 1024, 0);
}
aesl_fh.write(AUTOTB_TVIN_shift, tr.p, tr.tbytes);
tcl_file.set_num(1024, &tcl_file.shift_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_shift);
{
aesl_fh.write(AUTOTB_TVIN_shift, begin_str(AESL_transaction));
__xlx_offset_byte_param_shift = 0*4;
if (__xlx_apatb_param_shift) {
for (size_t i = 0; i < 1024; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_shift + i * 4;
std::string s = formatData(pos, 32);
aesl_fh.write(AUTOTB_TVIN_shift, s);
}
}
tcl_file.set_num(1024, &tcl_file.shift_depth);
aesl_fh.write(AUTOTB_TVIN_shift, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_values_adj1, 'b');
transaction<16> tr(128000);
__xlx_offset_byte_param_values_adj1 = 0*2;
if (__xlx_apatb_param_values_adj1) {
  tr.import<2>((char*)__xlx_apatb_param_values_adj1, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_values_adj1, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.values_adj1_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_values_adj1);
{
aesl_fh.write(AUTOTB_TVIN_values_adj1, begin_str(AESL_transaction));
__xlx_offset_byte_param_values_adj1 = 0*2;
if (__xlx_apatb_param_values_adj1) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_values_adj1 + i * 2;
std::string s = formatData(pos, 16);
aesl_fh.write(AUTOTB_TVIN_values_adj1, s);
}
}
tcl_file.set_num(128000, &tcl_file.values_adj1_depth);
aesl_fh.write(AUTOTB_TVIN_values_adj1, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_values_adj2, 'b');
transaction<16> tr(128000);
__xlx_offset_byte_param_values_adj2 = 0*2;
if (__xlx_apatb_param_values_adj2) {
  tr.import<2>((char*)__xlx_apatb_param_values_adj2, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_values_adj2, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.values_adj2_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_values_adj2);
{
aesl_fh.write(AUTOTB_TVIN_values_adj2, begin_str(AESL_transaction));
__xlx_offset_byte_param_values_adj2 = 0*2;
if (__xlx_apatb_param_values_adj2) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_values_adj2 + i * 2;
std::string s = formatData(pos, 16);
aesl_fh.write(AUTOTB_TVIN_values_adj2, s);
}
}
tcl_file.set_num(128000, &tcl_file.values_adj2_depth);
aesl_fh.write(AUTOTB_TVIN_values_adj2, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_values_adj3, 'b');
transaction<16> tr(128000);
__xlx_offset_byte_param_values_adj3 = 0*2;
if (__xlx_apatb_param_values_adj3) {
  tr.import<2>((char*)__xlx_apatb_param_values_adj3, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_values_adj3, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.values_adj3_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_values_adj3);
{
aesl_fh.write(AUTOTB_TVIN_values_adj3, begin_str(AESL_transaction));
__xlx_offset_byte_param_values_adj3 = 0*2;
if (__xlx_apatb_param_values_adj3) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_values_adj3 + i * 2;
std::string s = formatData(pos, 16);
aesl_fh.write(AUTOTB_TVIN_values_adj3, s);
}
}
tcl_file.set_num(128000, &tcl_file.values_adj3_depth);
aesl_fh.write(AUTOTB_TVIN_values_adj3, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_values_adj4, 'b');
transaction<16> tr(128000);
__xlx_offset_byte_param_values_adj4 = 0*2;
if (__xlx_apatb_param_values_adj4) {
  tr.import<2>((char*)__xlx_apatb_param_values_adj4, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_values_adj4, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.values_adj4_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_values_adj4);
{
aesl_fh.write(AUTOTB_TVIN_values_adj4, begin_str(AESL_transaction));
__xlx_offset_byte_param_values_adj4 = 0*2;
if (__xlx_apatb_param_values_adj4) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_values_adj4 + i * 2;
std::string s = formatData(pos, 16);
aesl_fh.write(AUTOTB_TVIN_values_adj4, s);
}
}
tcl_file.set_num(128000, &tcl_file.values_adj4_depth);
aesl_fh.write(AUTOTB_TVIN_values_adj4, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_values_fea1, 'b');
transaction<16> tr(128000);
__xlx_offset_byte_param_values_fea1 = 0*2;
if (__xlx_apatb_param_values_fea1) {
  tr.import<2>((char*)__xlx_apatb_param_values_fea1, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_values_fea1, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.values_fea1_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_values_fea1);
{
aesl_fh.write(AUTOTB_TVIN_values_fea1, begin_str(AESL_transaction));
__xlx_offset_byte_param_values_fea1 = 0*2;
if (__xlx_apatb_param_values_fea1) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_values_fea1 + i * 2;
std::string s = formatData(pos, 16);
aesl_fh.write(AUTOTB_TVIN_values_fea1, s);
}
}
tcl_file.set_num(128000, &tcl_file.values_fea1_depth);
aesl_fh.write(AUTOTB_TVIN_values_fea1, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_values_fea2, 'b');
transaction<16> tr(128000);
__xlx_offset_byte_param_values_fea2 = 0*2;
if (__xlx_apatb_param_values_fea2) {
  tr.import<2>((char*)__xlx_apatb_param_values_fea2, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_values_fea2, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.values_fea2_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_values_fea2);
{
aesl_fh.write(AUTOTB_TVIN_values_fea2, begin_str(AESL_transaction));
__xlx_offset_byte_param_values_fea2 = 0*2;
if (__xlx_apatb_param_values_fea2) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_values_fea2 + i * 2;
std::string s = formatData(pos, 16);
aesl_fh.write(AUTOTB_TVIN_values_fea2, s);
}
}
tcl_file.set_num(128000, &tcl_file.values_fea2_depth);
aesl_fh.write(AUTOTB_TVIN_values_fea2, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_values_fea3, 'b');
transaction<16> tr(128000);
__xlx_offset_byte_param_values_fea3 = 0*2;
if (__xlx_apatb_param_values_fea3) {
  tr.import<2>((char*)__xlx_apatb_param_values_fea3, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_values_fea3, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.values_fea3_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_values_fea3);
{
aesl_fh.write(AUTOTB_TVIN_values_fea3, begin_str(AESL_transaction));
__xlx_offset_byte_param_values_fea3 = 0*2;
if (__xlx_apatb_param_values_fea3) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_values_fea3 + i * 2;
std::string s = formatData(pos, 16);
aesl_fh.write(AUTOTB_TVIN_values_fea3, s);
}
}
tcl_file.set_num(128000, &tcl_file.values_fea3_depth);
aesl_fh.write(AUTOTB_TVIN_values_fea3, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVIN_values_fea4, 'b');
transaction<16> tr(128000);
__xlx_offset_byte_param_values_fea4 = 0*2;
if (__xlx_apatb_param_values_fea4) {
  tr.import<2>((char*)__xlx_apatb_param_values_fea4, 128000, 0);
}
aesl_fh.write(AUTOTB_TVIN_values_fea4, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.values_fea4_depth);
}
#else
aesl_fh.touch(AUTOTB_TVIN_values_fea4);
{
aesl_fh.write(AUTOTB_TVIN_values_fea4, begin_str(AESL_transaction));
__xlx_offset_byte_param_values_fea4 = 0*2;
if (__xlx_apatb_param_values_fea4) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_values_fea4 + i * 2;
std::string s = formatData(pos, 16);
aesl_fh.write(AUTOTB_TVIN_values_fea4, s);
}
}
tcl_file.set_num(128000, &tcl_file.values_fea4_depth);
aesl_fh.write(AUTOTB_TVIN_values_fea4, end_str());
}
#endif
// print gemm_mode Transactions
{
aesl_fh.write(AUTOTB_TVIN_gemm_mode, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_apatb_param_gemm_mode;
aesl_fh.write(AUTOTB_TVIN_gemm_mode, formatData(pos, 1));
}
  tcl_file.set_num(1, &tcl_file.gemm_mode_depth);
aesl_fh.write(AUTOTB_TVIN_gemm_mode, end_str());
}

// print quantized_multiplier_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_quantized_multiplier_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_quantized_multiplier;
aesl_fh.write(AUTOTB_TVIN_quantized_multiplier_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.quantized_multiplier_offset_depth);
aesl_fh.write(AUTOTB_TVIN_quantized_multiplier_offset, end_str());
}

// print shift_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_shift_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_shift;
aesl_fh.write(AUTOTB_TVIN_shift_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.shift_offset_depth);
aesl_fh.write(AUTOTB_TVIN_shift_offset, end_str());
}

// print bias_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_bias_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_bias;
aesl_fh.write(AUTOTB_TVIN_bias_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.bias_offset_depth);
aesl_fh.write(AUTOTB_TVIN_bias_offset, end_str());
}

// print bias_count Transactions
{
aesl_fh.write(AUTOTB_TVIN_bias_count, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)__xlx_apatb_param_bias_count;
aesl_fh.write(AUTOTB_TVIN_bias_count, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.bias_count_depth);
aesl_fh.write(AUTOTB_TVIN_bias_count, end_str());
}

// print profiling_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_profiling_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_profiling;
aesl_fh.write(AUTOTB_TVIN_profiling_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.profiling_offset_depth);
aesl_fh.write(AUTOTB_TVIN_profiling_offset, end_str());
}

// print zero_point_lhs Transactions
{
aesl_fh.write(AUTOTB_TVIN_zero_point_lhs, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)__xlx_apatb_param_zero_point_lhs;
aesl_fh.write(AUTOTB_TVIN_zero_point_lhs, formatData(pos, 8));
}
  tcl_file.set_num(1, &tcl_file.zero_point_lhs_depth);
aesl_fh.write(AUTOTB_TVIN_zero_point_lhs, end_str());
}

// print zero_point_rhs Transactions
{
aesl_fh.write(AUTOTB_TVIN_zero_point_rhs, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)__xlx_apatb_param_zero_point_rhs;
aesl_fh.write(AUTOTB_TVIN_zero_point_rhs, formatData(pos, 8));
}
  tcl_file.set_num(1, &tcl_file.zero_point_rhs_depth);
aesl_fh.write(AUTOTB_TVIN_zero_point_rhs, end_str());
}

// print zero_point_dst Transactions
{
aesl_fh.write(AUTOTB_TVIN_zero_point_dst, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)__xlx_apatb_param_zero_point_dst;
aesl_fh.write(AUTOTB_TVIN_zero_point_dst, formatData(pos, 8));
}
  tcl_file.set_num(1, &tcl_file.zero_point_dst_depth);
aesl_fh.write(AUTOTB_TVIN_zero_point_dst, end_str());
}

// print clamp_max Transactions
{
aesl_fh.write(AUTOTB_TVIN_clamp_max, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)__xlx_apatb_param_clamp_max;
aesl_fh.write(AUTOTB_TVIN_clamp_max, formatData(pos, 8));
}
  tcl_file.set_num(1, &tcl_file.clamp_max_depth);
aesl_fh.write(AUTOTB_TVIN_clamp_max, end_str());
}

// print clamp_min Transactions
{
aesl_fh.write(AUTOTB_TVIN_clamp_min, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)__xlx_apatb_param_clamp_min;
aesl_fh.write(AUTOTB_TVIN_clamp_min, formatData(pos, 8));
}
  tcl_file.set_num(1, &tcl_file.clamp_min_depth);
aesl_fh.write(AUTOTB_TVIN_clamp_min, end_str());
}

// print N_adj Transactions
{
aesl_fh.write(AUTOTB_TVIN_N_adj, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_apatb_param_N_adj;
aesl_fh.write(AUTOTB_TVIN_N_adj, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.N_adj_depth);
aesl_fh.write(AUTOTB_TVIN_N_adj, end_str());
}

// print M_adj Transactions
{
aesl_fh.write(AUTOTB_TVIN_M_adj, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_apatb_param_M_adj;
aesl_fh.write(AUTOTB_TVIN_M_adj, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.M_adj_depth);
aesl_fh.write(AUTOTB_TVIN_M_adj, end_str());
}

// print M_fea Transactions
{
aesl_fh.write(AUTOTB_TVIN_M_fea, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_apatb_param_M_fea;
aesl_fh.write(AUTOTB_TVIN_M_fea, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.M_fea_depth);
aesl_fh.write(AUTOTB_TVIN_M_fea, end_str());
}

// print P_w Transactions
{
aesl_fh.write(AUTOTB_TVIN_P_w, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_apatb_param_P_w;
aesl_fh.write(AUTOTB_TVIN_P_w, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.P_w_depth);
aesl_fh.write(AUTOTB_TVIN_P_w, end_str());
}

// print B_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_B_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_B;
aesl_fh.write(AUTOTB_TVIN_B_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.B_offset_depth);
aesl_fh.write(AUTOTB_TVIN_B_offset, end_str());
}

// print D1_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_D1_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_D1;
aesl_fh.write(AUTOTB_TVIN_D1_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.D1_offset_depth);
aesl_fh.write(AUTOTB_TVIN_D1_offset, end_str());
}

// print D2_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_D2_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_D2;
aesl_fh.write(AUTOTB_TVIN_D2_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.D2_offset_depth);
aesl_fh.write(AUTOTB_TVIN_D2_offset, end_str());
}

// print D3_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_D3_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_D3;
aesl_fh.write(AUTOTB_TVIN_D3_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.D3_offset_depth);
aesl_fh.write(AUTOTB_TVIN_D3_offset, end_str());
}

// print D4_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_D4_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_D4;
aesl_fh.write(AUTOTB_TVIN_D4_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.D4_offset_depth);
aesl_fh.write(AUTOTB_TVIN_D4_offset, end_str());
}

// print array_c_adjust Transactions
{
aesl_fh.write(AUTOTB_TVIN_array_c_adjust, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_apatb_param_array_c_adjust;
aesl_fh.write(AUTOTB_TVIN_array_c_adjust, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.array_c_adjust_depth);
aesl_fh.write(AUTOTB_TVIN_array_c_adjust, end_str());
}

// print rowPtr_fea1_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea1_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_rowPtr_fea1;
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea1_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.rowPtr_fea1_offset_depth);
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea1_offset, end_str());
}

// print rowPtr_fea2_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea2_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_rowPtr_fea2;
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea2_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.rowPtr_fea2_offset_depth);
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea2_offset, end_str());
}

// print rowPtr_fea3_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea3_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_rowPtr_fea3;
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea3_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.rowPtr_fea3_offset_depth);
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea3_offset, end_str());
}

// print rowPtr_fea4_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea4_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_rowPtr_fea4;
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea4_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.rowPtr_fea4_offset_depth);
aesl_fh.write(AUTOTB_TVIN_rowPtr_fea4_offset, end_str());
}

// print columnIndex_fea1_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea1_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_columnIndex_fea1;
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea1_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.columnIndex_fea1_offset_depth);
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea1_offset, end_str());
}

// print columnIndex_fea2_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea2_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_columnIndex_fea2;
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea2_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.columnIndex_fea2_offset_depth);
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea2_offset, end_str());
}

// print columnIndex_fea3_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea3_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_columnIndex_fea3;
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea3_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.columnIndex_fea3_offset_depth);
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea3_offset, end_str());
}

// print columnIndex_fea4_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea4_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_columnIndex_fea4;
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea4_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.columnIndex_fea4_offset_depth);
aesl_fh.write(AUTOTB_TVIN_columnIndex_fea4_offset, end_str());
}

// print values_fea1_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_values_fea1_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_values_fea1;
aesl_fh.write(AUTOTB_TVIN_values_fea1_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.values_fea1_offset_depth);
aesl_fh.write(AUTOTB_TVIN_values_fea1_offset, end_str());
}

// print values_fea2_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_values_fea2_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_values_fea2;
aesl_fh.write(AUTOTB_TVIN_values_fea2_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.values_fea2_offset_depth);
aesl_fh.write(AUTOTB_TVIN_values_fea2_offset, end_str());
}

// print values_fea3_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_values_fea3_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_values_fea3;
aesl_fh.write(AUTOTB_TVIN_values_fea3_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.values_fea3_offset_depth);
aesl_fh.write(AUTOTB_TVIN_values_fea3_offset, end_str());
}

// print values_fea4_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_values_fea4_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_values_fea4;
aesl_fh.write(AUTOTB_TVIN_values_fea4_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.values_fea4_offset_depth);
aesl_fh.write(AUTOTB_TVIN_values_fea4_offset, end_str());
}

// print rowPtr_adj1_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj1_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_rowPtr_adj1;
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj1_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.rowPtr_adj1_offset_depth);
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj1_offset, end_str());
}

// print rowPtr_adj2_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj2_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_rowPtr_adj2;
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj2_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.rowPtr_adj2_offset_depth);
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj2_offset, end_str());
}

// print rowPtr_adj3_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj3_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_rowPtr_adj3;
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj3_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.rowPtr_adj3_offset_depth);
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj3_offset, end_str());
}

// print rowPtr_adj4_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj4_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_rowPtr_adj4;
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj4_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.rowPtr_adj4_offset_depth);
aesl_fh.write(AUTOTB_TVIN_rowPtr_adj4_offset, end_str());
}

// print columnIndex_adj1_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj1_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_columnIndex_adj1;
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj1_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.columnIndex_adj1_offset_depth);
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj1_offset, end_str());
}

// print columnIndex_adj2_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj2_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_columnIndex_adj2;
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj2_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.columnIndex_adj2_offset_depth);
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj2_offset, end_str());
}

// print columnIndex_adj3_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj3_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_columnIndex_adj3;
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj3_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.columnIndex_adj3_offset_depth);
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj3_offset, end_str());
}

// print columnIndex_adj4_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj4_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_columnIndex_adj4;
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj4_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.columnIndex_adj4_offset_depth);
aesl_fh.write(AUTOTB_TVIN_columnIndex_adj4_offset, end_str());
}

// print values_adj1_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_values_adj1_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_values_adj1;
aesl_fh.write(AUTOTB_TVIN_values_adj1_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.values_adj1_offset_depth);
aesl_fh.write(AUTOTB_TVIN_values_adj1_offset, end_str());
}

// print values_adj2_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_values_adj2_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_values_adj2;
aesl_fh.write(AUTOTB_TVIN_values_adj2_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.values_adj2_offset_depth);
aesl_fh.write(AUTOTB_TVIN_values_adj2_offset, end_str());
}

// print values_adj3_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_values_adj3_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_values_adj3;
aesl_fh.write(AUTOTB_TVIN_values_adj3_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.values_adj3_offset_depth);
aesl_fh.write(AUTOTB_TVIN_values_adj3_offset, end_str());
}

// print values_adj4_offset Transactions
{
aesl_fh.write(AUTOTB_TVIN_values_adj4_offset, begin_str(AESL_transaction));
{
auto *pos = (unsigned char*)&__xlx_offset_byte_param_values_adj4;
aesl_fh.write(AUTOTB_TVIN_values_adj4_offset, formatData(pos, 32));
}
  tcl_file.set_num(1, &tcl_file.values_adj4_offset_depth);
aesl_fh.write(AUTOTB_TVIN_values_adj4_offset, end_str());
}

CodeState = CALL_C_DUT;
mmult_top_hw_stub_wrapper(__xlx_apatb_param_gemm_mode, __xlx_apatb_param_quantized_multiplier, __xlx_apatb_param_shift, __xlx_apatb_param_bias, __xlx_apatb_param_bias_count, __xlx_apatb_param_profiling, __xlx_apatb_param_zero_point_lhs, __xlx_apatb_param_zero_point_rhs, __xlx_apatb_param_zero_point_dst, __xlx_apatb_param_clamp_max, __xlx_apatb_param_clamp_min, __xlx_apatb_param_N_adj, __xlx_apatb_param_M_adj, __xlx_apatb_param_M_fea, __xlx_apatb_param_P_w, __xlx_apatb_param_B, __xlx_apatb_param_D1, __xlx_apatb_param_D2, __xlx_apatb_param_D3, __xlx_apatb_param_D4, __xlx_apatb_param_array_c_adjust, __xlx_apatb_param_rowPtr_fea1, __xlx_apatb_param_rowPtr_fea2, __xlx_apatb_param_rowPtr_fea3, __xlx_apatb_param_rowPtr_fea4, __xlx_apatb_param_columnIndex_fea1, __xlx_apatb_param_columnIndex_fea2, __xlx_apatb_param_columnIndex_fea3, __xlx_apatb_param_columnIndex_fea4, __xlx_apatb_param_values_fea1, __xlx_apatb_param_values_fea2, __xlx_apatb_param_values_fea3, __xlx_apatb_param_values_fea4, __xlx_apatb_param_rowPtr_adj1, __xlx_apatb_param_rowPtr_adj2, __xlx_apatb_param_rowPtr_adj3, __xlx_apatb_param_rowPtr_adj4, __xlx_apatb_param_columnIndex_adj1, __xlx_apatb_param_columnIndex_adj2, __xlx_apatb_param_columnIndex_adj3, __xlx_apatb_param_columnIndex_adj4, __xlx_apatb_param_values_adj1, __xlx_apatb_param_values_adj2, __xlx_apatb_param_values_adj3, __xlx_apatb_param_values_adj4);
CodeState = DUMP_OUTPUTS;
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVOUT_D1, 'b');
transaction<16> tr(128000);
__xlx_offset_byte_param_D1 = 0*2;
if (__xlx_apatb_param_D1) {
  tr.import<2>((char*)__xlx_apatb_param_D1, 128000, 0);
}
aesl_fh.write(AUTOTB_TVOUT_D1, tr.p, tr.tbytes);
tcl_file.set_num(128000, &tcl_file.D1_depth);
}
#else
aesl_fh.touch(AUTOTB_TVOUT_D1);
{
aesl_fh.write(AUTOTB_TVOUT_D1, begin_str(AESL_transaction));
__xlx_offset_byte_param_D1 = 0*2;
if (__xlx_apatb_param_D1) {
for (size_t i = 0; i < 128000; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_D1 + i * 2;
std::string s = formatData(pos, 16);
aesl_fh.write(AUTOTB_TVOUT_D1, s);
}
}
tcl_file.set_num(128000, &tcl_file.D1_depth);
aesl_fh.write(AUTOTB_TVOUT_D1, end_str());
}
#endif
#ifdef USE_BINARY_TV_FILE
{
aesl_fh.touch(AUTOTB_TVOUT_profiling, 'b');
transaction<64> tr(16);
__xlx_offset_byte_param_profiling = 0*8;
if (__xlx_apatb_param_profiling) {
  tr.import<8>((char*)__xlx_apatb_param_profiling, 16, 0);
}
aesl_fh.write(AUTOTB_TVOUT_profiling, tr.p, tr.tbytes);
tcl_file.set_num(16, &tcl_file.profiling_depth);
}
#else
aesl_fh.touch(AUTOTB_TVOUT_profiling);
{
aesl_fh.write(AUTOTB_TVOUT_profiling, begin_str(AESL_transaction));
__xlx_offset_byte_param_profiling = 0*8;
if (__xlx_apatb_param_profiling) {
for (size_t i = 0; i < 16; ++i) {
unsigned char *pos = (unsigned char*)__xlx_apatb_param_profiling + i * 8;
std::string s = formatData(pos, 64);
aesl_fh.write(AUTOTB_TVOUT_profiling, s);
}
}
tcl_file.set_num(16, &tcl_file.profiling_depth);
aesl_fh.write(AUTOTB_TVOUT_profiling, end_str());
}
#endif
CodeState = DELETE_CHAR_BUFFERS;
AESL_transaction++;
tcl_file.set_num(AESL_transaction , &tcl_file.trans_num);
}
