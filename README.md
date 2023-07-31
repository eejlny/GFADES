GFADES sources and data

Steps to run full implementation with a base design with one thread and 2 compute units done with Linux Ubuntu.

Clone the contents of this repository under a directory named workspace.

Setup path to the Vitis FPGA tools, for example:

**module load vitis/2022.1**

or

**source <path to tools>/Xilinx/Vitis/2022.1/settings64-Vitis.sh**

edit matrix.h located the gnn-rfsoc-mt-all-2022/src directory and verify the following lines so the hardware is generated
with 1 hardware thread and 2 compute units per thread

#define FEA_THREADS 1
#define ADJ_THREADS 1

#define B_WIDTH_BLOCK 2 
#define C_WIDTH_BLOCK 2

now go to hls solution directory

**cd ..../workspace/gnn-rfsoc-mt-all-2022/hls/gnn/solution1**

Check script.tcl to make sure that the set_part command matches your device is correct or modify as needed.
The default part is xczu27dr-ffve1156-2-i that is the FPGA available in an RFSOC2x2 board.
Perform HLS simulation, HLS synthesis and IP export with this command:

**vitis_hls -f script.tcl**

HLS simulation should display results like this:

out :data index= 0 0 kernel = 0.00196838

out :data index= 0 1 kernel = 0.477783

out :data index= 0 2 kernel = 0.59668

out :data index= 0 3 kernel = 0.0311279

out :data index= 0 4 kernel = 0.0022583

out :data index= 0 5 kernel = 0.564453

out :data index= 0 6 kernel = 0.53125

out :data index= 0 7 kernel = 0.0110474

out :data index= 0 8 kernel = 0.114624

out :data index= 0 9 kernel = 0.71582

Then wait for HLS synthesis and IP export to complete.
once this is done go to Vivado script directory

**cd ..../workspace**

Modify this line as needed in project_1.tcl to set a new project name/directory 

**set _xil_proj_name_ "1t1t2c"**

RTL implementation and bitstream generation

**vivado -mode batch -source project_1.tcl**  

after completion all results are available under the new project name directory. 
Also, you can open and work with the projects using the GUI for Vitis HLS and VIVADO located at: 

VITIS HLS: ..../workspace/gnn-rfsoc-mt-all-2022/hls/gnn

VIVADO: ..../workspace/<project_name/

