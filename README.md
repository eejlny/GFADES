GFADES sources and data

Steps to run full implementation

#copy the contents of this repository under a workspace directory

#setup path to tools, for example
module load vitis/2022.1

#go to hls solution directory
cd ..../workspace/gnn-rfsoc-mt-all-2022/hls/gnn/solution1

#HLS syn and IP export
vitis_hls -f script.tcl 

#go to Vivado script directory
cd ..../workspace

#Modify this line as needed in project_1.tcl to set a new project name/directory
#set _xil_proj_name_ "4t4t8c"

#RTL implementation and bitstream generation
vivado -mode batch -source project_1.tcl  

#after completion all results are available under the new project name directory
