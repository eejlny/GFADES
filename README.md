GFADES sources and data

Steps to run full implementation

#setup path to tools
module load vitis/2022.1

#go to hls solution directory
cd ./projects/workspace/gnn-rfsoc-mt-all-2022/hls/gnn/solution1

#HLS syn and IP export
vitis_hls -f script.tcl 

#go to Vivado script directory
cd /home/josnu02/projects/workspace

#RTL implementation and bitstream generation
vivado -mode batch -source project_1.tcl  

