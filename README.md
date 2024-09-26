GFADES sources and data

Steps to run full implementation with a base design with one thread and 2 compute units done with Linux Ubuntu.

Clone the contents of this repository under a directory named workspace.

Setup path to the Vitis FPGA tools, for example:

**module load vitis/2022.1**

or

**source <path to tools>/Xilinx/Vitis/2022.1/settings64-Vitis.sh**

edit matrix.h located the gnn-rfsoc-mt-all-2022/src directory and verify the following lines so the hardware is generated
with 1 hardware thread and 2 compute units per thread. That is the base configuration and the highest performance configuration
that fits in the RFSOC 2x2 board consist of 4 hardware threads for FEA and ADJ and 8 compute units per thread. 

#define FEA_THREADS 1

#define ADJ_THREADS 1

#define B_WIDTH_BLOCK 2 

#define C_WIDTH_BLOCK 2

The compute unit always uses sblocks, sblocks groups multiple adj sparse rows into one for processing and it is useful with very sparse matrix to improve performance.
The USE_SBLOCKS controls if the write unit also uses sblocks and reads multiple FIFO channels comming from the compute unit 
or reads only one FIFO channel per core and this is generally better because it optimizes the loop that writes data to memory.

#define SPMM_BLOCK 1

#define USE_SBLOCKS 0

IF use_tail is set to 0 then the tail has to be zero so the division bewtween the number of weight columns (number of hidden units or neurons) and the number of compute units needs to be integer
so  P_w (width of weight matrix) has to be 16, 34, 64 etc but NOT 21. If weight columns is not multiple of 2 then use USE_TAIL 1.

#define USE_TAIL 0

recommended that USE_SBLOCKS is 0 and USE_TAIL 0 for performance

now from the download directory perform HLS simulation, HLS synthesis and IP export with this command:

Before launching this command update the data file paths (e.g:/home/josnu02/projects/gfades_test/GFADES/gnn-rfsoc-mt-all-2022/data/matrices/mol_adj.txt) in main_float.cpp according to your directories.

**vitis_hls -f ./gnn-rfsoc-mt-all-2022/hls/gnn/solution1/script.tcl**

Check script.tcl to make sure that the set_part command matches your device is correct or modify as needed.
The default part is xczu27dr-ffve1156-2-i that is the FPGA available in an RFSOC2x2 board.


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

Once HLS synthesis and IP export has completed launch implementation and bitstream generation with this command:

**vivado -mode batch -source project_1.tcl**

Optionally modify this line as needed in project_1.tcl to set a new project name/directory 

**set _xil_proj_name_ "1t1t2c"**

After completion all results are available under the new project name directory. 
Also, you can open and work with the projects using the GUI for Vitis HLS and VIVADO located at: 

VITIS HLS: ./gnn


VIVADO: ./<project_name/

The jupyter directory contains a test jupyter notebook that can be used to test the design in the PYNQ FPGA board
and measure performance.  The jupyter notebook sets all the control registers for the pynq buffers. In addition it sets

my_ip.register_map.relu=0  //set to 1 to perform relu in hardware directly

my_ip.register_map.gemm_mode=0 //set to 1 to compute with a dense feature matrix instead of a sparse feature matrix. 

In sparse mode a CSR matrix contains values, column_index and row_pointer for adjacency and feature matrices.

In the dense case the adjacency matrix is still sparse but the feature matrix is dense.
 
In this dense case the feature values buffer contains all the values including zeros. 
In this dense case the row_pointer and column_index buffers are not used since they are generated internally by the hardware
internally.

The dense case is useful with datasets with dense feature matrices or after the first layer where the amount of sparsity could be low or the overhead
of obtaining a csr matrix at run-time unfeasible. 

The Jupyter directory also contains a full GCN network for molecule graph classification that uses the accelerator during training. It requires torch_geometric==2.0.1 installed on the board.
The notebook currently offloads the forward passes to the FPGA for both layers and contains a custom backward path that uses tensors saved from the hardware. T
Future work invoves offloading the backward path calculations. The training loop should reach 0.76 accuracy around epoch 36.  

 

If you find this repository useful for your research, please, consider citing:

Jose Nunez-Yanez. 2023. Accelerating Graph Neural Networks in Pytorch with HLS and Deep Dataflows. In Applied Reconfigurable Computing. Architectures, Tools, and Applications: 19th International Symposium, ARC 2023, Cottbus, Germany, September 27–29, 2023, Proceedings. Springer-Verlag, Berlin, Heidelberg, 131–145. https://doi.org/10.1007/978-3-031-42921-7_9


