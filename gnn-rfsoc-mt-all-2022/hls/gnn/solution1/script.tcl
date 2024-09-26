############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
############################################################
open_project gnn -reset
set_top mmult_top
add_files /home/josnu02/projects/gfades_test/GFADES/gnn-rfsoc-mt-all-2022/src/kernelMatrixmult_all.cpp
add_files /home/josnu02/projects/gfades_test/GFADES/gnn-rfsoc-mt-all-2022/src/matrix_mult.h
add_files -tb /home/josnu02/projects/gfades_test/GFADES/gnn-rfsoc-mt-all-2022/src/main_float.cpp
add_files -tb /home/josnu02/projects/gfades_test/GFADES/gnn-rfsoc-mt-all-2022/src/kernelMatrixmult.h
open_solution "solution1" -flow_target vivado
set_part {xczu27dr-ffve1156-2-i}
create_clock -period 5 -name default
#source "./gnn/solution1/directives.tcl"
csynth_design
#cosim_design
export_design -format ip_catalog
csim_design
