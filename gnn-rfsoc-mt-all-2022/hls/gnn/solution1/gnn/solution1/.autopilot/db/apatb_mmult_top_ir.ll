; ModuleID = '/home/josnu02/projects/workspace/gnn-rfsoc-mt-all-2022/hls/gnn/solution1/gnn/solution1/.autopilot/db/a.g.ld.5.gdce.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-i64:64-i128:128-i256:256-i512:512-i1024:1024-i2048:2048-i4096:4096-n8:16:32:64-S128-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024"
target triple = "fpga64-xilinx-none"

%"struct.ap_int<32>" = type { %"struct.ap_int_base<32, true>" }
%"struct.ap_int_base<32, true>" = type { %"struct.ssdm_int<32, true>" }
%"struct.ssdm_int<32, true>" = type { i32 }
%"struct.ap_int<64>" = type { %"struct.ap_int_base<64, true>" }
%"struct.ap_int_base<64, true>" = type { %"struct.ssdm_int<64, true>" }
%"struct.ssdm_int<64, true>" = type { i64 }
%"struct.ap_int<8>" = type { %"struct.ap_int_base<8, true>" }
%"struct.ap_int_base<8, true>" = type { %"class.std::ios_base::Init" }
%"class.std::ios_base::Init" = type { i8 }

; Function Attrs: noinline
define void @apatb_mmult_top_ir(i1 zeroext %gemm_mode, %"struct.ap_int<32>"* noalias nocapture nonnull readonly %quantized_multiplier, %"struct.ap_int<32>"* noalias nocapture nonnull readonly %shift, %"struct.ap_int<32>"* noalias nocapture nonnull readonly %bias, %"struct.ap_int<32>"* nocapture readonly %bias_count, %"struct.ap_int<64>"* noalias nocapture nonnull %profiling, %"struct.ap_int<8>"* nocapture readonly %zero_point_lhs, %"struct.ap_int<8>"* nocapture readonly %zero_point_rhs, %"struct.ap_int<8>"* nocapture readonly %zero_point_dst, %"struct.ap_int<8>"* nocapture readonly %clamp_max, %"struct.ap_int<8>"* nocapture readonly %clamp_min, i32 %N_adj, i32 %M_adj, i32 %M_fea, i32 %P_w, half* noalias nocapture nonnull readonly %B, half* noalias nocapture nonnull %D1, half* noalias nocapture nonnull readonly %D2, half* noalias nocapture nonnull readonly %D3, half* noalias nocapture nonnull readonly %D4, i32 %array_c_adjust, i32* noalias nocapture nonnull readonly %rowPtr_fea1, i32* noalias nocapture nonnull readonly %rowPtr_fea2, i32* noalias nocapture nonnull readonly %rowPtr_fea3, i32* noalias nocapture nonnull readonly %rowPtr_fea4, i32* noalias nocapture nonnull readonly %columnIndex_fea1, i32* noalias nocapture nonnull readonly %columnIndex_fea2, i32* noalias nocapture nonnull readonly %columnIndex_fea3, i32* noalias nocapture nonnull readonly %columnIndex_fea4, half* noalias nocapture nonnull readonly %values_fea1, half* noalias nocapture nonnull readonly %values_fea2, half* noalias nocapture nonnull readonly %values_fea3, half* noalias nocapture nonnull readonly %values_fea4, i32* noalias nocapture nonnull readonly %rowPtr_adj1, i32* noalias nocapture nonnull readonly %rowPtr_adj2, i32* noalias nocapture nonnull readonly %rowPtr_adj3, i32* noalias nocapture nonnull readonly %rowPtr_adj4, i32* noalias nocapture nonnull readonly %columnIndex_adj1, i32* noalias nocapture nonnull readonly %columnIndex_adj2, i32* noalias nocapture nonnull readonly %columnIndex_adj3, i32* noalias nocapture nonnull readonly %columnIndex_adj4, half* noalias nocapture nonnull readonly %values_adj1, half* noalias nocapture nonnull readonly %values_adj2, half* noalias nocapture nonnull readonly %values_adj3, half* noalias nocapture nonnull readonly %values_adj4) local_unnamed_addr #0 {
entry:
  %malloccall = tail call i8* @malloc(i64 4096)
  %quantized_multiplier_copy = bitcast i8* %malloccall to [1024 x %"struct.ap_int<32>"]*
  %malloccall1 = tail call i8* @malloc(i64 4096)
  %shift_copy = bitcast i8* %malloccall1 to [1024 x %"struct.ap_int<32>"]*
  %malloccall2 = tail call i8* @malloc(i64 4096)
  %bias_copy = bitcast i8* %malloccall2 to [1024 x %"struct.ap_int<32>"]*
  %profiling_copy = alloca [16 x i64], align 512
  %malloccall3 = tail call i8* @malloc(i64 256000)
  %B_copy = bitcast i8* %malloccall3 to [128000 x half]*
  %malloccall4 = tail call i8* @malloc(i64 256000)
  %D1_copy = bitcast i8* %malloccall4 to [128000 x half]*
  %malloccall5 = tail call i8* @malloc(i64 256000)
  %D2_copy = bitcast i8* %malloccall5 to [128000 x half]*
  %malloccall6 = tail call i8* @malloc(i64 256000)
  %D3_copy = bitcast i8* %malloccall6 to [128000 x half]*
  %malloccall7 = tail call i8* @malloc(i64 256000)
  %D4_copy = bitcast i8* %malloccall7 to [128000 x half]*
  %malloccall8 = tail call i8* @malloc(i64 16384)
  %rowPtr_fea1_copy = bitcast i8* %malloccall8 to [4096 x i32]*
  %malloccall9 = tail call i8* @malloc(i64 16384)
  %rowPtr_fea2_copy = bitcast i8* %malloccall9 to [4096 x i32]*
  %malloccall10 = tail call i8* @malloc(i64 16384)
  %rowPtr_fea3_copy = bitcast i8* %malloccall10 to [4096 x i32]*
  %malloccall11 = tail call i8* @malloc(i64 16384)
  %rowPtr_fea4_copy = bitcast i8* %malloccall11 to [4096 x i32]*
  %malloccall12 = tail call i8* @malloc(i64 512000)
  %columnIndex_fea1_copy = bitcast i8* %malloccall12 to [128000 x i32]*
  %malloccall13 = tail call i8* @malloc(i64 512000)
  %columnIndex_fea2_copy = bitcast i8* %malloccall13 to [128000 x i32]*
  %malloccall14 = tail call i8* @malloc(i64 512000)
  %columnIndex_fea3_copy = bitcast i8* %malloccall14 to [128000 x i32]*
  %malloccall15 = tail call i8* @malloc(i64 512000)
  %columnIndex_fea4_copy = bitcast i8* %malloccall15 to [128000 x i32]*
  %malloccall16 = tail call i8* @malloc(i64 256000)
  %values_fea1_copy = bitcast i8* %malloccall16 to [128000 x half]*
  %malloccall17 = tail call i8* @malloc(i64 256000)
  %values_fea2_copy = bitcast i8* %malloccall17 to [128000 x half]*
  %malloccall18 = tail call i8* @malloc(i64 256000)
  %values_fea3_copy = bitcast i8* %malloccall18 to [128000 x half]*
  %malloccall19 = tail call i8* @malloc(i64 256000)
  %values_fea4_copy = bitcast i8* %malloccall19 to [128000 x half]*
  %malloccall20 = tail call i8* @malloc(i64 16384)
  %rowPtr_adj1_copy = bitcast i8* %malloccall20 to [4096 x i32]*
  %malloccall21 = tail call i8* @malloc(i64 16384)
  %rowPtr_adj2_copy = bitcast i8* %malloccall21 to [4096 x i32]*
  %malloccall22 = tail call i8* @malloc(i64 16384)
  %rowPtr_adj3_copy = bitcast i8* %malloccall22 to [4096 x i32]*
  %malloccall23 = tail call i8* @malloc(i64 16384)
  %rowPtr_adj4_copy = bitcast i8* %malloccall23 to [4096 x i32]*
  %malloccall24 = tail call i8* @malloc(i64 512000)
  %columnIndex_adj1_copy = bitcast i8* %malloccall24 to [128000 x i32]*
  %malloccall25 = tail call i8* @malloc(i64 512000)
  %columnIndex_adj2_copy = bitcast i8* %malloccall25 to [128000 x i32]*
  %malloccall26 = tail call i8* @malloc(i64 512000)
  %columnIndex_adj3_copy = bitcast i8* %malloccall26 to [128000 x i32]*
  %malloccall27 = tail call i8* @malloc(i64 512000)
  %columnIndex_adj4_copy = bitcast i8* %malloccall27 to [128000 x i32]*
  %malloccall28 = tail call i8* @malloc(i64 256000)
  %values_adj1_copy = bitcast i8* %malloccall28 to [128000 x half]*
  %malloccall29 = tail call i8* @malloc(i64 256000)
  %values_adj2_copy = bitcast i8* %malloccall29 to [128000 x half]*
  %malloccall30 = tail call i8* @malloc(i64 256000)
  %values_adj3_copy = bitcast i8* %malloccall30 to [128000 x half]*
  %malloccall31 = tail call i8* @malloc(i64 256000)
  %values_adj4_copy = bitcast i8* %malloccall31 to [128000 x half]*
  %0 = bitcast %"struct.ap_int<32>"* %quantized_multiplier to [1024 x %"struct.ap_int<32>"]*
  %1 = bitcast %"struct.ap_int<32>"* %shift to [1024 x %"struct.ap_int<32>"]*
  %2 = bitcast %"struct.ap_int<32>"* %bias to [1024 x %"struct.ap_int<32>"]*
  %3 = bitcast %"struct.ap_int<64>"* %profiling to [16 x %"struct.ap_int<64>"]*
  %4 = bitcast half* %B to [128000 x half]*
  %5 = bitcast half* %D1 to [128000 x half]*
  %6 = bitcast half* %D2 to [128000 x half]*
  %7 = bitcast half* %D3 to [128000 x half]*
  %8 = bitcast half* %D4 to [128000 x half]*
  %9 = bitcast i32* %rowPtr_fea1 to [4096 x i32]*
  %10 = bitcast i32* %rowPtr_fea2 to [4096 x i32]*
  %11 = bitcast i32* %rowPtr_fea3 to [4096 x i32]*
  %12 = bitcast i32* %rowPtr_fea4 to [4096 x i32]*
  %13 = bitcast i32* %columnIndex_fea1 to [128000 x i32]*
  %14 = bitcast i32* %columnIndex_fea2 to [128000 x i32]*
  %15 = bitcast i32* %columnIndex_fea3 to [128000 x i32]*
  %16 = bitcast i32* %columnIndex_fea4 to [128000 x i32]*
  %17 = bitcast half* %values_fea1 to [128000 x half]*
  %18 = bitcast half* %values_fea2 to [128000 x half]*
  %19 = bitcast half* %values_fea3 to [128000 x half]*
  %20 = bitcast half* %values_fea4 to [128000 x half]*
  %21 = bitcast i32* %rowPtr_adj1 to [4096 x i32]*
  %22 = bitcast i32* %rowPtr_adj2 to [4096 x i32]*
  %23 = bitcast i32* %rowPtr_adj3 to [4096 x i32]*
  %24 = bitcast i32* %rowPtr_adj4 to [4096 x i32]*
  %25 = bitcast i32* %columnIndex_adj1 to [128000 x i32]*
  %26 = bitcast i32* %columnIndex_adj2 to [128000 x i32]*
  %27 = bitcast i32* %columnIndex_adj3 to [128000 x i32]*
  %28 = bitcast i32* %columnIndex_adj4 to [128000 x i32]*
  %29 = bitcast half* %values_adj1 to [128000 x half]*
  %30 = bitcast half* %values_adj2 to [128000 x half]*
  %31 = bitcast half* %values_adj3 to [128000 x half]*
  %32 = bitcast half* %values_adj4 to [128000 x half]*
  call fastcc void @copy_in([1024 x %"struct.ap_int<32>"]* nonnull %0, [1024 x %"struct.ap_int<32>"]* %quantized_multiplier_copy, [1024 x %"struct.ap_int<32>"]* nonnull %1, [1024 x %"struct.ap_int<32>"]* %shift_copy, [1024 x %"struct.ap_int<32>"]* nonnull %2, [1024 x %"struct.ap_int<32>"]* %bias_copy, [16 x %"struct.ap_int<64>"]* nonnull %3, [16 x i64]* nonnull align 512 %profiling_copy, [128000 x half]* nonnull %4, [128000 x half]* %B_copy, [128000 x half]* nonnull %5, [128000 x half]* %D1_copy, [128000 x half]* nonnull %6, [128000 x half]* %D2_copy, [128000 x half]* nonnull %7, [128000 x half]* %D3_copy, [128000 x half]* nonnull %8, [128000 x half]* %D4_copy, [4096 x i32]* nonnull %9, [4096 x i32]* %rowPtr_fea1_copy, [4096 x i32]* nonnull %10, [4096 x i32]* %rowPtr_fea2_copy, [4096 x i32]* nonnull %11, [4096 x i32]* %rowPtr_fea3_copy, [4096 x i32]* nonnull %12, [4096 x i32]* %rowPtr_fea4_copy, [128000 x i32]* nonnull %13, [128000 x i32]* %columnIndex_fea1_copy, [128000 x i32]* nonnull %14, [128000 x i32]* %columnIndex_fea2_copy, [128000 x i32]* nonnull %15, [128000 x i32]* %columnIndex_fea3_copy, [128000 x i32]* nonnull %16, [128000 x i32]* %columnIndex_fea4_copy, [128000 x half]* nonnull %17, [128000 x half]* %values_fea1_copy, [128000 x half]* nonnull %18, [128000 x half]* %values_fea2_copy, [128000 x half]* nonnull %19, [128000 x half]* %values_fea3_copy, [128000 x half]* nonnull %20, [128000 x half]* %values_fea4_copy, [4096 x i32]* nonnull %21, [4096 x i32]* %rowPtr_adj1_copy, [4096 x i32]* nonnull %22, [4096 x i32]* %rowPtr_adj2_copy, [4096 x i32]* nonnull %23, [4096 x i32]* %rowPtr_adj3_copy, [4096 x i32]* nonnull %24, [4096 x i32]* %rowPtr_adj4_copy, [128000 x i32]* nonnull %25, [128000 x i32]* %columnIndex_adj1_copy, [128000 x i32]* nonnull %26, [128000 x i32]* %columnIndex_adj2_copy, [128000 x i32]* nonnull %27, [128000 x i32]* %columnIndex_adj3_copy, [128000 x i32]* nonnull %28, [128000 x i32]* %columnIndex_adj4_copy, [128000 x half]* nonnull %29, [128000 x half]* %values_adj1_copy, [128000 x half]* nonnull %30, [128000 x half]* %values_adj2_copy, [128000 x half]* nonnull %31, [128000 x half]* %values_adj3_copy, [128000 x half]* nonnull %32, [128000 x half]* %values_adj4_copy)
  %33 = getelementptr inbounds [1024 x %"struct.ap_int<32>"], [1024 x %"struct.ap_int<32>"]* %quantized_multiplier_copy, i32 0, i32 0
  %34 = getelementptr inbounds [1024 x %"struct.ap_int<32>"], [1024 x %"struct.ap_int<32>"]* %shift_copy, i32 0, i32 0
  %35 = getelementptr inbounds [1024 x %"struct.ap_int<32>"], [1024 x %"struct.ap_int<32>"]* %bias_copy, i32 0, i32 0
  %36 = getelementptr [16 x i64], [16 x i64]* %profiling_copy, i32 0, i32 0
  %37 = getelementptr inbounds [128000 x half], [128000 x half]* %B_copy, i32 0, i32 0
  %38 = getelementptr inbounds [128000 x half], [128000 x half]* %D1_copy, i32 0, i32 0
  %39 = getelementptr inbounds [128000 x half], [128000 x half]* %D2_copy, i32 0, i32 0
  %40 = getelementptr inbounds [128000 x half], [128000 x half]* %D3_copy, i32 0, i32 0
  %41 = getelementptr inbounds [128000 x half], [128000 x half]* %D4_copy, i32 0, i32 0
  %42 = getelementptr inbounds [4096 x i32], [4096 x i32]* %rowPtr_fea1_copy, i32 0, i32 0
  %43 = getelementptr inbounds [4096 x i32], [4096 x i32]* %rowPtr_fea2_copy, i32 0, i32 0
  %44 = getelementptr inbounds [4096 x i32], [4096 x i32]* %rowPtr_fea3_copy, i32 0, i32 0
  %45 = getelementptr inbounds [4096 x i32], [4096 x i32]* %rowPtr_fea4_copy, i32 0, i32 0
  %46 = getelementptr inbounds [128000 x i32], [128000 x i32]* %columnIndex_fea1_copy, i32 0, i32 0
  %47 = getelementptr inbounds [128000 x i32], [128000 x i32]* %columnIndex_fea2_copy, i32 0, i32 0
  %48 = getelementptr inbounds [128000 x i32], [128000 x i32]* %columnIndex_fea3_copy, i32 0, i32 0
  %49 = getelementptr inbounds [128000 x i32], [128000 x i32]* %columnIndex_fea4_copy, i32 0, i32 0
  %50 = getelementptr inbounds [128000 x half], [128000 x half]* %values_fea1_copy, i32 0, i32 0
  %51 = getelementptr inbounds [128000 x half], [128000 x half]* %values_fea2_copy, i32 0, i32 0
  %52 = getelementptr inbounds [128000 x half], [128000 x half]* %values_fea3_copy, i32 0, i32 0
  %53 = getelementptr inbounds [128000 x half], [128000 x half]* %values_fea4_copy, i32 0, i32 0
  %54 = getelementptr inbounds [4096 x i32], [4096 x i32]* %rowPtr_adj1_copy, i32 0, i32 0
  %55 = getelementptr inbounds [4096 x i32], [4096 x i32]* %rowPtr_adj2_copy, i32 0, i32 0
  %56 = getelementptr inbounds [4096 x i32], [4096 x i32]* %rowPtr_adj3_copy, i32 0, i32 0
  %57 = getelementptr inbounds [4096 x i32], [4096 x i32]* %rowPtr_adj4_copy, i32 0, i32 0
  %58 = getelementptr inbounds [128000 x i32], [128000 x i32]* %columnIndex_adj1_copy, i32 0, i32 0
  %59 = getelementptr inbounds [128000 x i32], [128000 x i32]* %columnIndex_adj2_copy, i32 0, i32 0
  %60 = getelementptr inbounds [128000 x i32], [128000 x i32]* %columnIndex_adj3_copy, i32 0, i32 0
  %61 = getelementptr inbounds [128000 x i32], [128000 x i32]* %columnIndex_adj4_copy, i32 0, i32 0
  %62 = getelementptr inbounds [128000 x half], [128000 x half]* %values_adj1_copy, i32 0, i32 0
  %63 = getelementptr inbounds [128000 x half], [128000 x half]* %values_adj2_copy, i32 0, i32 0
  %64 = getelementptr inbounds [128000 x half], [128000 x half]* %values_adj3_copy, i32 0, i32 0
  %65 = getelementptr inbounds [128000 x half], [128000 x half]* %values_adj4_copy, i32 0, i32 0
  call void @apatb_mmult_top_hw(i1 %gemm_mode, %"struct.ap_int<32>"* %33, %"struct.ap_int<32>"* %34, %"struct.ap_int<32>"* %35, %"struct.ap_int<32>"* %bias_count, i64* %36, %"struct.ap_int<8>"* %zero_point_lhs, %"struct.ap_int<8>"* %zero_point_rhs, %"struct.ap_int<8>"* %zero_point_dst, %"struct.ap_int<8>"* %clamp_max, %"struct.ap_int<8>"* %clamp_min, i32 %N_adj, i32 %M_adj, i32 %M_fea, i32 %P_w, half* %37, half* %38, half* %39, half* %40, half* %41, i32 %array_c_adjust, i32* %42, i32* %43, i32* %44, i32* %45, i32* %46, i32* %47, i32* %48, i32* %49, half* %50, half* %51, half* %52, half* %53, i32* %54, i32* %55, i32* %56, i32* %57, i32* %58, i32* %59, i32* %60, i32* %61, half* %62, half* %63, half* %64, half* %65)
  call void @copy_back([1024 x %"struct.ap_int<32>"]* %0, [1024 x %"struct.ap_int<32>"]* %quantized_multiplier_copy, [1024 x %"struct.ap_int<32>"]* %1, [1024 x %"struct.ap_int<32>"]* %shift_copy, [1024 x %"struct.ap_int<32>"]* %2, [1024 x %"struct.ap_int<32>"]* %bias_copy, [16 x %"struct.ap_int<64>"]* %3, [16 x i64]* %profiling_copy, [128000 x half]* %4, [128000 x half]* %B_copy, [128000 x half]* %5, [128000 x half]* %D1_copy, [128000 x half]* %6, [128000 x half]* %D2_copy, [128000 x half]* %7, [128000 x half]* %D3_copy, [128000 x half]* %8, [128000 x half]* %D4_copy, [4096 x i32]* %9, [4096 x i32]* %rowPtr_fea1_copy, [4096 x i32]* %10, [4096 x i32]* %rowPtr_fea2_copy, [4096 x i32]* %11, [4096 x i32]* %rowPtr_fea3_copy, [4096 x i32]* %12, [4096 x i32]* %rowPtr_fea4_copy, [128000 x i32]* %13, [128000 x i32]* %columnIndex_fea1_copy, [128000 x i32]* %14, [128000 x i32]* %columnIndex_fea2_copy, [128000 x i32]* %15, [128000 x i32]* %columnIndex_fea3_copy, [128000 x i32]* %16, [128000 x i32]* %columnIndex_fea4_copy, [128000 x half]* %17, [128000 x half]* %values_fea1_copy, [128000 x half]* %18, [128000 x half]* %values_fea2_copy, [128000 x half]* %19, [128000 x half]* %values_fea3_copy, [128000 x half]* %20, [128000 x half]* %values_fea4_copy, [4096 x i32]* %21, [4096 x i32]* %rowPtr_adj1_copy, [4096 x i32]* %22, [4096 x i32]* %rowPtr_adj2_copy, [4096 x i32]* %23, [4096 x i32]* %rowPtr_adj3_copy, [4096 x i32]* %24, [4096 x i32]* %rowPtr_adj4_copy, [128000 x i32]* %25, [128000 x i32]* %columnIndex_adj1_copy, [128000 x i32]* %26, [128000 x i32]* %columnIndex_adj2_copy, [128000 x i32]* %27, [128000 x i32]* %columnIndex_adj3_copy, [128000 x i32]* %28, [128000 x i32]* %columnIndex_adj4_copy, [128000 x half]* %29, [128000 x half]* %values_adj1_copy, [128000 x half]* %30, [128000 x half]* %values_adj2_copy, [128000 x half]* %31, [128000 x half]* %values_adj3_copy, [128000 x half]* %32, [128000 x half]* %values_adj4_copy)
  tail call void @free(i8* %malloccall)
  tail call void @free(i8* %malloccall1)
  tail call void @free(i8* %malloccall2)
  tail call void @free(i8* %malloccall3)
  tail call void @free(i8* %malloccall4)
  tail call void @free(i8* %malloccall5)
  tail call void @free(i8* %malloccall6)
  tail call void @free(i8* %malloccall7)
  tail call void @free(i8* %malloccall8)
  tail call void @free(i8* %malloccall9)
  tail call void @free(i8* %malloccall10)
  tail call void @free(i8* %malloccall11)
  tail call void @free(i8* %malloccall12)
  tail call void @free(i8* %malloccall13)
  tail call void @free(i8* %malloccall14)
  tail call void @free(i8* %malloccall15)
  tail call void @free(i8* %malloccall16)
  tail call void @free(i8* %malloccall17)
  tail call void @free(i8* %malloccall18)
  tail call void @free(i8* %malloccall19)
  tail call void @free(i8* %malloccall20)
  tail call void @free(i8* %malloccall21)
  tail call void @free(i8* %malloccall22)
  tail call void @free(i8* %malloccall23)
  tail call void @free(i8* %malloccall24)
  tail call void @free(i8* %malloccall25)
  tail call void @free(i8* %malloccall26)
  tail call void @free(i8* %malloccall27)
  tail call void @free(i8* %malloccall28)
  tail call void @free(i8* %malloccall29)
  tail call void @free(i8* %malloccall30)
  tail call void @free(i8* %malloccall31)
  ret void
}

declare noalias i8* @malloc(i64) local_unnamed_addr

; Function Attrs: argmemonly noinline norecurse
define internal fastcc void @copy_in([1024 x %"struct.ap_int<32>"]* noalias readonly, [1024 x %"struct.ap_int<32>"]* noalias, [1024 x %"struct.ap_int<32>"]* noalias readonly, [1024 x %"struct.ap_int<32>"]* noalias, [1024 x %"struct.ap_int<32>"]* noalias readonly, [1024 x %"struct.ap_int<32>"]* noalias, [16 x %"struct.ap_int<64>"]* noalias readonly, [16 x i64]* noalias align 512, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [4096 x i32]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [4096 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [4096 x i32]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [4096 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias) unnamed_addr #1 {
entry:
  call fastcc void @"onebyonecpy_hls.p0a1024struct.ap_int<32>"([1024 x %"struct.ap_int<32>"]* %1, [1024 x %"struct.ap_int<32>"]* %0)
  call fastcc void @"onebyonecpy_hls.p0a1024struct.ap_int<32>"([1024 x %"struct.ap_int<32>"]* %3, [1024 x %"struct.ap_int<32>"]* %2)
  call fastcc void @"onebyonecpy_hls.p0a1024struct.ap_int<32>"([1024 x %"struct.ap_int<32>"]* %5, [1024 x %"struct.ap_int<32>"]* %4)
  call fastcc void @"onebyonecpy_hls.p0a16struct.ap_int<64>"([16 x i64]* align 512 %7, [16 x %"struct.ap_int<64>"]* %6)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %9, [128000 x half]* %8)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %11, [128000 x half]* %10)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %13, [128000 x half]* %12)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %15, [128000 x half]* %14)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %17, [128000 x half]* %16)
  call fastcc void @onebyonecpy_hls.p0a4096i32([4096 x i32]* %19, [4096 x i32]* %18)
  call fastcc void @onebyonecpy_hls.p0a4096i32([4096 x i32]* %21, [4096 x i32]* %20)
  call fastcc void @onebyonecpy_hls.p0a4096i32([4096 x i32]* %23, [4096 x i32]* %22)
  call fastcc void @onebyonecpy_hls.p0a4096i32([4096 x i32]* %25, [4096 x i32]* %24)
  call fastcc void @onebyonecpy_hls.p0a128000i32([128000 x i32]* %27, [128000 x i32]* %26)
  call fastcc void @onebyonecpy_hls.p0a128000i32([128000 x i32]* %29, [128000 x i32]* %28)
  call fastcc void @onebyonecpy_hls.p0a128000i32([128000 x i32]* %31, [128000 x i32]* %30)
  call fastcc void @onebyonecpy_hls.p0a128000i32([128000 x i32]* %33, [128000 x i32]* %32)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %35, [128000 x half]* %34)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %37, [128000 x half]* %36)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %39, [128000 x half]* %38)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %41, [128000 x half]* %40)
  call fastcc void @onebyonecpy_hls.p0a4096i32([4096 x i32]* %43, [4096 x i32]* %42)
  call fastcc void @onebyonecpy_hls.p0a4096i32([4096 x i32]* %45, [4096 x i32]* %44)
  call fastcc void @onebyonecpy_hls.p0a4096i32([4096 x i32]* %47, [4096 x i32]* %46)
  call fastcc void @onebyonecpy_hls.p0a4096i32([4096 x i32]* %49, [4096 x i32]* %48)
  call fastcc void @onebyonecpy_hls.p0a128000i32([128000 x i32]* %51, [128000 x i32]* %50)
  call fastcc void @onebyonecpy_hls.p0a128000i32([128000 x i32]* %53, [128000 x i32]* %52)
  call fastcc void @onebyonecpy_hls.p0a128000i32([128000 x i32]* %55, [128000 x i32]* %54)
  call fastcc void @onebyonecpy_hls.p0a128000i32([128000 x i32]* %57, [128000 x i32]* %56)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %59, [128000 x half]* %58)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %61, [128000 x half]* %60)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %63, [128000 x half]* %62)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %65, [128000 x half]* %64)
  ret void
}

; Function Attrs: argmemonly noinline norecurse
define internal fastcc void @"onebyonecpy_hls.p0a1024struct.ap_int<32>"([1024 x %"struct.ap_int<32>"]* noalias, [1024 x %"struct.ap_int<32>"]* noalias readonly) unnamed_addr #2 {
entry:
  %2 = icmp eq [1024 x %"struct.ap_int<32>"]* %0, null
  %3 = icmp eq [1024 x %"struct.ap_int<32>"]* %1, null
  %4 = or i1 %2, %3
  br i1 %4, label %ret, label %copy

copy:                                             ; preds = %entry
  br label %for.loop

for.loop:                                         ; preds = %for.loop, %copy
  %for.loop.idx7 = phi i64 [ 0, %copy ], [ %for.loop.idx.next, %for.loop ]
  %src.addr.0.0.05 = getelementptr [1024 x %"struct.ap_int<32>"], [1024 x %"struct.ap_int<32>"]* %1, i64 0, i64 %for.loop.idx7, i32 0, i32 0, i32 0
  %dst.addr.0.0.06 = getelementptr [1024 x %"struct.ap_int<32>"], [1024 x %"struct.ap_int<32>"]* %0, i64 0, i64 %for.loop.idx7, i32 0, i32 0, i32 0
  %5 = load i32, i32* %src.addr.0.0.05, align 4
  store i32 %5, i32* %dst.addr.0.0.06, align 4
  %for.loop.idx.next = add nuw nsw i64 %for.loop.idx7, 1
  %exitcond = icmp ne i64 %for.loop.idx.next, 1024
  br i1 %exitcond, label %for.loop, label %ret

ret:                                              ; preds = %for.loop, %entry
  ret void
}

; Function Attrs: argmemonly noinline norecurse
define internal fastcc void @"onebyonecpy_hls.p0a16struct.ap_int<64>"([16 x i64]* noalias align 512, [16 x %"struct.ap_int<64>"]* noalias readonly) unnamed_addr #2 {
entry:
  %2 = icmp eq [16 x i64]* %0, null
  %3 = icmp eq [16 x %"struct.ap_int<64>"]* %1, null
  %4 = or i1 %2, %3
  br i1 %4, label %ret, label %copy

copy:                                             ; preds = %entry
  br label %for.loop

for.loop:                                         ; preds = %for.loop, %copy
  %for.loop.idx7 = phi i64 [ 0, %copy ], [ %for.loop.idx.next, %for.loop ]
  %src.addr.0.0.05 = getelementptr [16 x %"struct.ap_int<64>"], [16 x %"struct.ap_int<64>"]* %1, i64 0, i64 %for.loop.idx7, i32 0, i32 0, i32 0
  %5 = getelementptr [16 x i64], [16 x i64]* %0, i64 0, i64 %for.loop.idx7
  %6 = load i64, i64* %src.addr.0.0.05, align 8
  store i64 %6, i64* %5, align 8
  %for.loop.idx.next = add nuw nsw i64 %for.loop.idx7, 1
  %exitcond = icmp ne i64 %for.loop.idx.next, 16
  br i1 %exitcond, label %for.loop, label %ret

ret:                                              ; preds = %for.loop, %entry
  ret void
}

; Function Attrs: argmemonly noinline norecurse
define internal fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* noalias, [128000 x half]* noalias readonly) unnamed_addr #2 {
entry:
  %2 = icmp eq [128000 x half]* %0, null
  %3 = icmp eq [128000 x half]* %1, null
  %4 = or i1 %2, %3
  br i1 %4, label %ret, label %copy

copy:                                             ; preds = %entry
  br label %for.loop

for.loop:                                         ; preds = %for.loop, %copy
  %for.loop.idx1 = phi i64 [ 0, %copy ], [ %for.loop.idx.next, %for.loop ]
  %dst.addr = getelementptr [128000 x half], [128000 x half]* %0, i64 0, i64 %for.loop.idx1
  %src.addr = getelementptr [128000 x half], [128000 x half]* %1, i64 0, i64 %for.loop.idx1
  %5 = load half, half* %src.addr, align 2
  store half %5, half* %dst.addr, align 2
  %for.loop.idx.next = add nuw nsw i64 %for.loop.idx1, 1
  %exitcond = icmp ne i64 %for.loop.idx.next, 128000
  br i1 %exitcond, label %for.loop, label %ret

ret:                                              ; preds = %for.loop, %entry
  ret void
}

; Function Attrs: argmemonly noinline norecurse
define internal fastcc void @onebyonecpy_hls.p0a4096i32([4096 x i32]* noalias, [4096 x i32]* noalias readonly) unnamed_addr #2 {
entry:
  %2 = icmp eq [4096 x i32]* %0, null
  %3 = icmp eq [4096 x i32]* %1, null
  %4 = or i1 %2, %3
  br i1 %4, label %ret, label %copy

copy:                                             ; preds = %entry
  br label %for.loop

for.loop:                                         ; preds = %for.loop, %copy
  %for.loop.idx1 = phi i64 [ 0, %copy ], [ %for.loop.idx.next, %for.loop ]
  %dst.addr = getelementptr [4096 x i32], [4096 x i32]* %0, i64 0, i64 %for.loop.idx1
  %src.addr = getelementptr [4096 x i32], [4096 x i32]* %1, i64 0, i64 %for.loop.idx1
  %5 = load i32, i32* %src.addr, align 4
  store i32 %5, i32* %dst.addr, align 4
  %for.loop.idx.next = add nuw nsw i64 %for.loop.idx1, 1
  %exitcond = icmp ne i64 %for.loop.idx.next, 4096
  br i1 %exitcond, label %for.loop, label %ret

ret:                                              ; preds = %for.loop, %entry
  ret void
}

; Function Attrs: argmemonly noinline norecurse
define internal fastcc void @onebyonecpy_hls.p0a128000i32([128000 x i32]* noalias, [128000 x i32]* noalias readonly) unnamed_addr #2 {
entry:
  %2 = icmp eq [128000 x i32]* %0, null
  %3 = icmp eq [128000 x i32]* %1, null
  %4 = or i1 %2, %3
  br i1 %4, label %ret, label %copy

copy:                                             ; preds = %entry
  br label %for.loop

for.loop:                                         ; preds = %for.loop, %copy
  %for.loop.idx1 = phi i64 [ 0, %copy ], [ %for.loop.idx.next, %for.loop ]
  %dst.addr = getelementptr [128000 x i32], [128000 x i32]* %0, i64 0, i64 %for.loop.idx1
  %src.addr = getelementptr [128000 x i32], [128000 x i32]* %1, i64 0, i64 %for.loop.idx1
  %5 = load i32, i32* %src.addr, align 4
  store i32 %5, i32* %dst.addr, align 4
  %for.loop.idx.next = add nuw nsw i64 %for.loop.idx1, 1
  %exitcond = icmp ne i64 %for.loop.idx.next, 128000
  br i1 %exitcond, label %for.loop, label %ret

ret:                                              ; preds = %for.loop, %entry
  ret void
}

; Function Attrs: argmemonly noinline norecurse
define internal fastcc void @copy_out([1024 x %"struct.ap_int<32>"]* noalias, [1024 x %"struct.ap_int<32>"]* noalias readonly, [1024 x %"struct.ap_int<32>"]* noalias, [1024 x %"struct.ap_int<32>"]* noalias readonly, [1024 x %"struct.ap_int<32>"]* noalias, [1024 x %"struct.ap_int<32>"]* noalias readonly, [16 x %"struct.ap_int<64>"]* noalias, [16 x i64]* noalias readonly align 512, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly) unnamed_addr #3 {
entry:
  call fastcc void @"onebyonecpy_hls.p0a1024struct.ap_int<32>"([1024 x %"struct.ap_int<32>"]* %0, [1024 x %"struct.ap_int<32>"]* %1)
  call fastcc void @"onebyonecpy_hls.p0a1024struct.ap_int<32>"([1024 x %"struct.ap_int<32>"]* %2, [1024 x %"struct.ap_int<32>"]* %3)
  call fastcc void @"onebyonecpy_hls.p0a1024struct.ap_int<32>"([1024 x %"struct.ap_int<32>"]* %4, [1024 x %"struct.ap_int<32>"]* %5)
  call fastcc void @"onebyonecpy_hls.p0a16struct.ap_int<64>.173"([16 x %"struct.ap_int<64>"]* %6, [16 x i64]* align 512 %7)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %8, [128000 x half]* %9)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %10, [128000 x half]* %11)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %12, [128000 x half]* %13)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %14, [128000 x half]* %15)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %16, [128000 x half]* %17)
  call fastcc void @onebyonecpy_hls.p0a4096i32([4096 x i32]* %18, [4096 x i32]* %19)
  call fastcc void @onebyonecpy_hls.p0a4096i32([4096 x i32]* %20, [4096 x i32]* %21)
  call fastcc void @onebyonecpy_hls.p0a4096i32([4096 x i32]* %22, [4096 x i32]* %23)
  call fastcc void @onebyonecpy_hls.p0a4096i32([4096 x i32]* %24, [4096 x i32]* %25)
  call fastcc void @onebyonecpy_hls.p0a128000i32([128000 x i32]* %26, [128000 x i32]* %27)
  call fastcc void @onebyonecpy_hls.p0a128000i32([128000 x i32]* %28, [128000 x i32]* %29)
  call fastcc void @onebyonecpy_hls.p0a128000i32([128000 x i32]* %30, [128000 x i32]* %31)
  call fastcc void @onebyonecpy_hls.p0a128000i32([128000 x i32]* %32, [128000 x i32]* %33)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %34, [128000 x half]* %35)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %36, [128000 x half]* %37)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %38, [128000 x half]* %39)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %40, [128000 x half]* %41)
  call fastcc void @onebyonecpy_hls.p0a4096i32([4096 x i32]* %42, [4096 x i32]* %43)
  call fastcc void @onebyonecpy_hls.p0a4096i32([4096 x i32]* %44, [4096 x i32]* %45)
  call fastcc void @onebyonecpy_hls.p0a4096i32([4096 x i32]* %46, [4096 x i32]* %47)
  call fastcc void @onebyonecpy_hls.p0a4096i32([4096 x i32]* %48, [4096 x i32]* %49)
  call fastcc void @onebyonecpy_hls.p0a128000i32([128000 x i32]* %50, [128000 x i32]* %51)
  call fastcc void @onebyonecpy_hls.p0a128000i32([128000 x i32]* %52, [128000 x i32]* %53)
  call fastcc void @onebyonecpy_hls.p0a128000i32([128000 x i32]* %54, [128000 x i32]* %55)
  call fastcc void @onebyonecpy_hls.p0a128000i32([128000 x i32]* %56, [128000 x i32]* %57)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %58, [128000 x half]* %59)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %60, [128000 x half]* %61)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %62, [128000 x half]* %63)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %64, [128000 x half]* %65)
  ret void
}

declare void @free(i8*) local_unnamed_addr

; Function Attrs: argmemonly noinline norecurse
define internal fastcc void @"onebyonecpy_hls.p0a16struct.ap_int<64>.173"([16 x %"struct.ap_int<64>"]* noalias, [16 x i64]* noalias readonly align 512) unnamed_addr #2 {
entry:
  %2 = icmp eq [16 x %"struct.ap_int<64>"]* %0, null
  %3 = icmp eq [16 x i64]* %1, null
  %4 = or i1 %2, %3
  br i1 %4, label %ret, label %copy

copy:                                             ; preds = %entry
  br label %for.loop

for.loop:                                         ; preds = %for.loop, %copy
  %for.loop.idx7 = phi i64 [ 0, %copy ], [ %for.loop.idx.next, %for.loop ]
  %5 = getelementptr [16 x i64], [16 x i64]* %1, i64 0, i64 %for.loop.idx7
  %dst.addr.0.0.06 = getelementptr [16 x %"struct.ap_int<64>"], [16 x %"struct.ap_int<64>"]* %0, i64 0, i64 %for.loop.idx7, i32 0, i32 0, i32 0
  %6 = load i64, i64* %5, align 8
  store i64 %6, i64* %dst.addr.0.0.06, align 8
  %for.loop.idx.next = add nuw nsw i64 %for.loop.idx7, 1
  %exitcond = icmp ne i64 %for.loop.idx.next, 16
  br i1 %exitcond, label %for.loop, label %ret

ret:                                              ; preds = %for.loop, %entry
  ret void
}

declare void @apatb_mmult_top_hw(i1, %"struct.ap_int<32>"*, %"struct.ap_int<32>"*, %"struct.ap_int<32>"*, %"struct.ap_int<32>"*, i64*, %"struct.ap_int<8>"*, %"struct.ap_int<8>"*, %"struct.ap_int<8>"*, %"struct.ap_int<8>"*, %"struct.ap_int<8>"*, i32, i32, i32, i32, half*, half*, half*, half*, half*, i32, i32*, i32*, i32*, i32*, i32*, i32*, i32*, i32*, half*, half*, half*, half*, i32*, i32*, i32*, i32*, i32*, i32*, i32*, i32*, half*, half*, half*, half*)

; Function Attrs: argmemonly noinline norecurse
define internal fastcc void @copy_back([1024 x %"struct.ap_int<32>"]* noalias, [1024 x %"struct.ap_int<32>"]* noalias readonly, [1024 x %"struct.ap_int<32>"]* noalias, [1024 x %"struct.ap_int<32>"]* noalias readonly, [1024 x %"struct.ap_int<32>"]* noalias, [1024 x %"struct.ap_int<32>"]* noalias readonly, [16 x %"struct.ap_int<64>"]* noalias, [16 x i64]* noalias readonly align 512, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [4096 x i32]* noalias, [4096 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x i32]* noalias, [128000 x i32]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly, [128000 x half]* noalias, [128000 x half]* noalias readonly) unnamed_addr #3 {
entry:
  call fastcc void @"onebyonecpy_hls.p0a16struct.ap_int<64>.173"([16 x %"struct.ap_int<64>"]* %6, [16 x i64]* align 512 %7)
  call fastcc void @onebyonecpy_hls.p0a128000f16([128000 x half]* %10, [128000 x half]* %11)
  ret void
}

define void @mmult_top_hw_stub_wrapper(i1, %"struct.ap_int<32>"*, %"struct.ap_int<32>"*, %"struct.ap_int<32>"*, %"struct.ap_int<32>"*, i64*, %"struct.ap_int<8>"*, %"struct.ap_int<8>"*, %"struct.ap_int<8>"*, %"struct.ap_int<8>"*, %"struct.ap_int<8>"*, i32, i32, i32, i32, half*, half*, half*, half*, half*, i32, i32*, i32*, i32*, i32*, i32*, i32*, i32*, i32*, half*, half*, half*, half*, i32*, i32*, i32*, i32*, i32*, i32*, i32*, i32*, half*, half*, half*, half*) #4 {
entry:
  %45 = alloca [16 x %"struct.ap_int<64>"]
  %46 = bitcast %"struct.ap_int<32>"* %1 to [1024 x %"struct.ap_int<32>"]*
  %47 = bitcast %"struct.ap_int<32>"* %2 to [1024 x %"struct.ap_int<32>"]*
  %48 = bitcast %"struct.ap_int<32>"* %3 to [1024 x %"struct.ap_int<32>"]*
  %49 = bitcast i64* %5 to [16 x i64]*
  %50 = bitcast half* %15 to [128000 x half]*
  %51 = bitcast half* %16 to [128000 x half]*
  %52 = bitcast half* %17 to [128000 x half]*
  %53 = bitcast half* %18 to [128000 x half]*
  %54 = bitcast half* %19 to [128000 x half]*
  %55 = bitcast i32* %21 to [4096 x i32]*
  %56 = bitcast i32* %22 to [4096 x i32]*
  %57 = bitcast i32* %23 to [4096 x i32]*
  %58 = bitcast i32* %24 to [4096 x i32]*
  %59 = bitcast i32* %25 to [128000 x i32]*
  %60 = bitcast i32* %26 to [128000 x i32]*
  %61 = bitcast i32* %27 to [128000 x i32]*
  %62 = bitcast i32* %28 to [128000 x i32]*
  %63 = bitcast half* %29 to [128000 x half]*
  %64 = bitcast half* %30 to [128000 x half]*
  %65 = bitcast half* %31 to [128000 x half]*
  %66 = bitcast half* %32 to [128000 x half]*
  %67 = bitcast i32* %33 to [4096 x i32]*
  %68 = bitcast i32* %34 to [4096 x i32]*
  %69 = bitcast i32* %35 to [4096 x i32]*
  %70 = bitcast i32* %36 to [4096 x i32]*
  %71 = bitcast i32* %37 to [128000 x i32]*
  %72 = bitcast i32* %38 to [128000 x i32]*
  %73 = bitcast i32* %39 to [128000 x i32]*
  %74 = bitcast i32* %40 to [128000 x i32]*
  %75 = bitcast half* %41 to [128000 x half]*
  %76 = bitcast half* %42 to [128000 x half]*
  %77 = bitcast half* %43 to [128000 x half]*
  %78 = bitcast half* %44 to [128000 x half]*
  call void @copy_out([1024 x %"struct.ap_int<32>"]* null, [1024 x %"struct.ap_int<32>"]* %46, [1024 x %"struct.ap_int<32>"]* null, [1024 x %"struct.ap_int<32>"]* %47, [1024 x %"struct.ap_int<32>"]* null, [1024 x %"struct.ap_int<32>"]* %48, [16 x %"struct.ap_int<64>"]* %45, [16 x i64]* %49, [128000 x half]* null, [128000 x half]* %50, [128000 x half]* null, [128000 x half]* %51, [128000 x half]* null, [128000 x half]* %52, [128000 x half]* null, [128000 x half]* %53, [128000 x half]* null, [128000 x half]* %54, [4096 x i32]* null, [4096 x i32]* %55, [4096 x i32]* null, [4096 x i32]* %56, [4096 x i32]* null, [4096 x i32]* %57, [4096 x i32]* null, [4096 x i32]* %58, [128000 x i32]* null, [128000 x i32]* %59, [128000 x i32]* null, [128000 x i32]* %60, [128000 x i32]* null, [128000 x i32]* %61, [128000 x i32]* null, [128000 x i32]* %62, [128000 x half]* null, [128000 x half]* %63, [128000 x half]* null, [128000 x half]* %64, [128000 x half]* null, [128000 x half]* %65, [128000 x half]* null, [128000 x half]* %66, [4096 x i32]* null, [4096 x i32]* %67, [4096 x i32]* null, [4096 x i32]* %68, [4096 x i32]* null, [4096 x i32]* %69, [4096 x i32]* null, [4096 x i32]* %70, [128000 x i32]* null, [128000 x i32]* %71, [128000 x i32]* null, [128000 x i32]* %72, [128000 x i32]* null, [128000 x i32]* %73, [128000 x i32]* null, [128000 x i32]* %74, [128000 x half]* null, [128000 x half]* %75, [128000 x half]* null, [128000 x half]* %76, [128000 x half]* null, [128000 x half]* %77, [128000 x half]* null, [128000 x half]* %78)
  %79 = bitcast [1024 x %"struct.ap_int<32>"]* %46 to %"struct.ap_int<32>"*
  %80 = bitcast [1024 x %"struct.ap_int<32>"]* %47 to %"struct.ap_int<32>"*
  %81 = bitcast [1024 x %"struct.ap_int<32>"]* %48 to %"struct.ap_int<32>"*
  %82 = bitcast [16 x %"struct.ap_int<64>"]* %45 to %"struct.ap_int<64>"*
  %83 = bitcast [128000 x half]* %50 to half*
  %84 = bitcast [128000 x half]* %51 to half*
  %85 = bitcast [128000 x half]* %52 to half*
  %86 = bitcast [128000 x half]* %53 to half*
  %87 = bitcast [128000 x half]* %54 to half*
  %88 = bitcast [4096 x i32]* %55 to i32*
  %89 = bitcast [4096 x i32]* %56 to i32*
  %90 = bitcast [4096 x i32]* %57 to i32*
  %91 = bitcast [4096 x i32]* %58 to i32*
  %92 = bitcast [128000 x i32]* %59 to i32*
  %93 = bitcast [128000 x i32]* %60 to i32*
  %94 = bitcast [128000 x i32]* %61 to i32*
  %95 = bitcast [128000 x i32]* %62 to i32*
  %96 = bitcast [128000 x half]* %63 to half*
  %97 = bitcast [128000 x half]* %64 to half*
  %98 = bitcast [128000 x half]* %65 to half*
  %99 = bitcast [128000 x half]* %66 to half*
  %100 = bitcast [4096 x i32]* %67 to i32*
  %101 = bitcast [4096 x i32]* %68 to i32*
  %102 = bitcast [4096 x i32]* %69 to i32*
  %103 = bitcast [4096 x i32]* %70 to i32*
  %104 = bitcast [128000 x i32]* %71 to i32*
  %105 = bitcast [128000 x i32]* %72 to i32*
  %106 = bitcast [128000 x i32]* %73 to i32*
  %107 = bitcast [128000 x i32]* %74 to i32*
  %108 = bitcast [128000 x half]* %75 to half*
  %109 = bitcast [128000 x half]* %76 to half*
  %110 = bitcast [128000 x half]* %77 to half*
  %111 = bitcast [128000 x half]* %78 to half*
  call void @mmult_top_hw_stub(i1 %0, %"struct.ap_int<32>"* %79, %"struct.ap_int<32>"* %80, %"struct.ap_int<32>"* %81, %"struct.ap_int<32>"* %4, %"struct.ap_int<64>"* %82, %"struct.ap_int<8>"* %6, %"struct.ap_int<8>"* %7, %"struct.ap_int<8>"* %8, %"struct.ap_int<8>"* %9, %"struct.ap_int<8>"* %10, i32 %11, i32 %12, i32 %13, i32 %14, half* %83, half* %84, half* %85, half* %86, half* %87, i32 %20, i32* %88, i32* %89, i32* %90, i32* %91, i32* %92, i32* %93, i32* %94, i32* %95, half* %96, half* %97, half* %98, half* %99, i32* %100, i32* %101, i32* %102, i32* %103, i32* %104, i32* %105, i32* %106, i32* %107, half* %108, half* %109, half* %110, half* %111)
  call void @copy_in([1024 x %"struct.ap_int<32>"]* null, [1024 x %"struct.ap_int<32>"]* %46, [1024 x %"struct.ap_int<32>"]* null, [1024 x %"struct.ap_int<32>"]* %47, [1024 x %"struct.ap_int<32>"]* null, [1024 x %"struct.ap_int<32>"]* %48, [16 x %"struct.ap_int<64>"]* %45, [16 x i64]* %49, [128000 x half]* null, [128000 x half]* %50, [128000 x half]* null, [128000 x half]* %51, [128000 x half]* null, [128000 x half]* %52, [128000 x half]* null, [128000 x half]* %53, [128000 x half]* null, [128000 x half]* %54, [4096 x i32]* null, [4096 x i32]* %55, [4096 x i32]* null, [4096 x i32]* %56, [4096 x i32]* null, [4096 x i32]* %57, [4096 x i32]* null, [4096 x i32]* %58, [128000 x i32]* null, [128000 x i32]* %59, [128000 x i32]* null, [128000 x i32]* %60, [128000 x i32]* null, [128000 x i32]* %61, [128000 x i32]* null, [128000 x i32]* %62, [128000 x half]* null, [128000 x half]* %63, [128000 x half]* null, [128000 x half]* %64, [128000 x half]* null, [128000 x half]* %65, [128000 x half]* null, [128000 x half]* %66, [4096 x i32]* null, [4096 x i32]* %67, [4096 x i32]* null, [4096 x i32]* %68, [4096 x i32]* null, [4096 x i32]* %69, [4096 x i32]* null, [4096 x i32]* %70, [128000 x i32]* null, [128000 x i32]* %71, [128000 x i32]* null, [128000 x i32]* %72, [128000 x i32]* null, [128000 x i32]* %73, [128000 x i32]* null, [128000 x i32]* %74, [128000 x half]* null, [128000 x half]* %75, [128000 x half]* null, [128000 x half]* %76, [128000 x half]* null, [128000 x half]* %77, [128000 x half]* null, [128000 x half]* %78)
  ret void
}

declare void @mmult_top_hw_stub(i1, %"struct.ap_int<32>"*, %"struct.ap_int<32>"*, %"struct.ap_int<32>"*, %"struct.ap_int<32>"*, %"struct.ap_int<64>"*, %"struct.ap_int<8>"*, %"struct.ap_int<8>"*, %"struct.ap_int<8>"*, %"struct.ap_int<8>"*, %"struct.ap_int<8>"*, i32, i32, i32, i32, half*, half*, half*, half*, half*, i32, i32*, i32*, i32*, i32*, i32*, i32*, i32*, i32*, half*, half*, half*, half*, i32*, i32*, i32*, i32*, i32*, i32*, i32*, i32*, half*, half*, half*, half*)

attributes #0 = { noinline "fpga.wrapper.func"="wrapper" }
attributes #1 = { argmemonly noinline norecurse "fpga.wrapper.func"="copyin" }
attributes #2 = { argmemonly noinline norecurse "fpga.wrapper.func"="onebyonecpy_hls" }
attributes #3 = { argmemonly noinline norecurse "fpga.wrapper.func"="copyout" }
attributes #4 = { "fpga.wrapper.func"="stub" }

!llvm.dbg.cu = !{}
!llvm.ident = !{!0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0}
!llvm.module.flags = !{!1, !2, !3}
!blackbox_cfg = !{!4}

!0 = !{!"clang version 7.0.0 "}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 1, !"wchar_size", i32 4}
!4 = !{}
