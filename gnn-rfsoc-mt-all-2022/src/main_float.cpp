
#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <memory>
#include <sys/time.h>
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end

#include <string>
#include <fstream>
#include <sstream> // std::stringstream

#include "ap_int.h"
#include "matrix_mult.h"
#include "kernelMatrixmult.h"


#define max_N_adj MAX_N
#define max_M_fea MAX_M
#define max_P_w MAX_P

#define use_gemm 1

float max_adj=0.0;
float min_adj=0.0;
float max_fea=0.0;
float min_fea=0.0;


//#define generate_data

// for citeseer dataset
//#define citeseer
#define molecule
//#define citeseer_mod
//#define cora
//#define test

#ifdef molecule
int N_adj = 2273;  // number of nodes
int M_fea = 7;  // number of input features
int P_w = 32;  // number of features in the hidden layer
int NNZ_adj = 5028;  // number of non-zero values of adjacency
int NNZ_fea = 2273;  // number of non-zero values of feature
static const std::string adj_name = "/home/josnu02/projects/gfades_test/GFADES/gnn-rfsoc-mt-all-2022/data/matrices/mol_adj.txt";
#if (use_gemm == 0)
	static const std::string fea_name = "/home/josnu02/projects/gfades_test/GFADES/gnn-rfsoc-mt-all-2022/data/matrices/mol_feat.txt";
#else
	static const std::string fea_name = "/home/josnu02/projects/gfades_test/GFADES/gnn-rfsoc-mt-all-2022/data/matrices/mol_feat_dense.txt";
#endif
static const std::string w_name = "/home/josnu02/projects/gfades_test/GFADES/gnn-rfsoc-mt-all-2022/data/matrices/mol_weights.txt";
#endif

#ifdef citeseer_mod
int N_adj = 3327;  // number of nodes
int M_fea = 3703;  // number of input features
int P_w = 16;  // number of features in the hidden layer
int NNZ_adj = 12431;  // number of non-zero values of adjacency
int NNZ_fea = 105165;  // number of non-zero values of feature
static const std::string adj_name = "../../../../../../../data/matrices/citeseer_adj.txt";
static const std::string fea_name = "../../../../../../../data/matrices/citeseer_feat.txt";
static const std::string w_name = "../../../../../../../data/matrices/citeseer_weights.txt";
#endif

#ifdef pubmed
int N_adj = 19717;  // number of nodes
int M_fea = 500;  // number of input features
int P_w = 18;  // number of features in the hidden layer
int NNZ_adj = 108365;  // number of non-zero values of adjacency
int NNZ_fea = 988031;  // number of non-zero values of feature
static const std::string adj_name = "C:/Users/josnu02/workspace/gnn-z2/data/matrices/cora_adj.txt";
static const std::string fea_name = "C:/Users/josnu02/workspace/gnn-z2/data/matrices/cora_feat.txt";
static const std::string w_name = "C:/Users/josnu02/workspace/gnn-z2/data/matrices/cora_weights.txt";
#endif

#ifdef cora
int N_adj = 2708;  // number of nodes
int M_fea = 1433;  // number of input features
int P_w = 64;  // number of features in the hidden layer
int NNZ_adj = 13264;  // number of non-zero values of adjacency
int NNZ_fea = 49216;  // number of non-zero values of feature
static const std::string adj_name = "C:/Users/josnu02/workspace/gnn-z2/data/matrices/cora_adj.txt";
static const std::string fea_name = "C:/Users/josnu02/workspace/gnn-z2/data/matrices/cora_feat.txt";
static const std::string w_name = "C:/Users/josnu02/workspace/gnn-z2/data/matrices/cora_weights.txt";
#endif

#ifdef citeseer
int N_adj = 3327;  // number of nodes
int M_fea = 3703;  // number of input features
int P_w = 21;  // number of features in the hidden layer
//int P_w = 4;  // number of features in the hidden layer
int NNZ_adj = 12431;  // number of non-zero values of adjacency
//int NNZ_adj = 10;  // number of non-zero values of adjacency
int NNZ_fea = 105165;  // number of non-zero values of feature
//int NNZ_fea = 10;  // number of non-zero values of feature
static const std::string adj_name = "C:/Users/josnu02/workspace/gnn-z2/data/matrices/citeseer_adj.txt";
static const std::string fea_name = "C:/Users/josnu02/workspace/gnn-z2/data/matrices/citeseer_feat.txt";
static const std::string w_name = "C:/Users/josnu02/workspace/gnn-z2/data/matrices/citeseer_weights.txt";
#endif

#ifdef test
int N_adj = 4;  // number of nodes
int M_fea = 4;  // number of input features
int P_w = 2;  // number of features in the hidden layer
int NNZ_adj = 4;  // number of non-zero values of adjacency
int NNZ_fea = 2;  // number of non-zero values of feature
static const std::string adj_name = "C:/Users/josnu02/workspace/gnn-z2/data/matrices/test_adj.txt";
static const std::string fea_name = "C:/Users/josnu02/workspace/gnn-z2/data/matrices/test_feat.txt";
static const std::string w_name = "C:/Users/josnu02/workspace/gnn-z2/data/matrices/test_weights.txt";
#endif

double getTimestamp() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_usec + tv.tv_sec * 1e6;
}



static void init_weights(BTYPE *B, DTYPE *C_sw, DTYPE *C)
{
     for (int i = 0; i < M_fea; i++) {
          for (int j = 0; j < P_w; j++) {
		   float B_float = 0.4;
        	   B[i * P_w + j] =  (BTYPE)B_float;
		   //std::cout << "B value is "<< B[i * P_w + j] << std::endl;
          }
     }
     for (int i = 0; i < N_adj; i++) {
          for (int j = 0; j < P_w; j++) {
               C_sw[i * P_w + j] = 0;
               C[i * P_w + j] = 0;
          }
     }
}

static void load_weights(int N,int M,BTYPE *A,std::string file_name)
{

	// Create an input filestream
         std::ifstream myFile(file_name);

	// Make sure the file is open
    std::cout <<  "the file is " << file_name << std::endl;
	if(!myFile.is_open()) throw std::runtime_error("Could not open float file");

	// Helper vars
	std::string line;
	float val;
	int val_count=0;
	int val_zero=0;
	BTYPE array_val;

    for (int i = 0; i < N; i++) {
    	// Read data, line by line
    	std::getline(myFile, line);

	    // Create a stringstream of the current line
	    std::stringstream ss(line);


        for (int j = 0; j < M; j++) {

	        //fill one array val
        	array_val = 0;
        	// Extract each integer
        	ss >> val;

        	if (val==0)
        		val_zero++;

        	array_val = (BTYPE)val;

	        // If the next token is a comma, ignore it and move on
	        if(ss.peek() == ',') ss.ignore();

	        //A[i * N + j] = 16;
	        //std::cout << i <<" "<< j << " " << array_val << std::endl;
	        A[i + j*N] = array_val; //transpose so hardware can read column in contiguos address space
	        val_count++;
	  		if (array_val > 1)
	 	  	{
	  			std::cout << "array_val " << array_val << std::endl;
	 			exit(0);
	 	  	}
	        //std::cout << i <<" "<< j << " " << array_val << " " << A[i * M + j] << std::endl;

	    }
    }

    //if (M > 16)
	//exit(0);

    std::cout << "Total " << sizeof(BTYPE)*8  << " bit values in weight matrix " << val_count << std::endl;
    std::cout << "Total values set to zero in weight matrix " << val_zero << std::endl;
    

}



static void load_adj(int N,int M,ATYPE *A,std::string file_name)
{

	// Create an input filestream
         std::ifstream myFile(file_name);

	// Make sure the file is open
	if(!myFile.is_open()) throw std::runtime_error("Could not open float file");

	// Helper vars
	std::string line;
	float val;
	int val_count=0;
	int val_zero=0;
	ATYPE array_val;

    for (int i = 0; i < N; i++) {
    	// Read data, line by line
    	std::getline(myFile, line);

	    // Create a stringstream of the current line
	    std::stringstream ss(line);


        for (int j = 0; j < M; j++) {

	        //fill one array val
        	array_val = 0;
        	// Extract each integer
        	ss >> val;

        	if (val==0)
        		val_zero++;

        	array_val = (ATYPE)val;

	        // If the next token is a comma, ignore it and move on
	        if(ss.peek() == ',') ss.ignore();

	        //A[i * N + j] = 16;
	        //std::cout << i <<" "<< j << " " << array_val << std::endl;
	        A[i * M + j] = array_val;
	        val_count++;
	        //std::cout << i <<" "<< j << " " << array_val << " " << A[i * N + j] << std::endl;

	    }
    }

    //if (M > 16)
	//exit(0);

    std::cout << "Total " << sizeof(ATYPE)*8  << " bit values in adj matrix " << val_count << std::endl;
    std::cout << "Total values set to zero in adj matrix " << val_zero << std::endl;
    

}



static void load_fea(int N,int M,FTYPE *A,std::string file_name)
{

	// Create an input filestream
         std::ifstream myFile(file_name);

	// Make sure the file is open
	if(!myFile.is_open()) throw std::runtime_error("Could not open float file");

	// Helper vars
	std::string line;
	float val;
	int val_count=0;
	int val_zero=0;
	FTYPE array_val;

	std::cout << "Reading dense FEA "<< std::endl;

    for (int i = 0; i < N; i++) {
    	// Read data, line by line
    	std::getline(myFile, line);

	    // Create a stringstream of the current line
	    std::stringstream ss(line);


        for (int j = 0; j < M; j++) {

	        //fill one array val
        	array_val = 0;
        	// Extract each integer
        	ss >> val;

        	if (val==0)
        		val_zero++;

        	array_val = (FTYPE)val;

	        // If the next token is a comma, ignore it and move on
	        if(ss.peek() == ',') ss.ignore();

	        //A[i * N + j] = 16;
	        //std::cout << i <<" "<< j << " " << array_val << std::endl;
	        A[i * M + j] = array_val;
	        val_count++;
            //std::cout << i <<" "<< j << " " << A[i * M + j] << std::endl;

	    }
    }

    //if (M > 16)
	//exit(0);

    std::cout << "Total " << sizeof(FTYPE)*8  << " bit values in fea matrix " << val_count << std::endl;
    std::cout << "Total values set to zero in fea matrix " << val_zero << std::endl;
    

}



void mmult_golden(int N,int M, int P, DTYPE *A,  DTYPE *B, DTYPE *C)
{
     for (int row = 0; row < N; row++) {
          for (int col = 0; col < P; col++) {
        	   DTYPE result = 0;
               for (int k = 0; k < M; k++) {
       			//for(int z = 0; z < DTYPE_LENGTH; z+=8) {
       				DTYPE A_temp1 = A[row*M+k];
       				//ap_int<8> A_val = A_temp1.range(z+7,z);
				DTYPE A_val = A_temp1;
      				DTYPE B_temp = B[k*P+col];
           			result+=A_val*B_temp;
       			//}
               }
               C[row*P+col] = result;
               //std::cout << row << " " << col << " result is " << result << std::endl;
          }
          //std::cout << row << " " << col << " result is " << result << std::endl;
     }
}

static int result_check(int N,int P, DTYPE *D, DTYPE *D_sw)
{
     //for (int i = 0; i < P_w; i++) {
         //for (int j = 0; j < N_adj; j++) {
     for (int i = 0; i < 1; i++) {
        for (int j = 0; j < P_w; j++) {
          //if (C_sw[i] != C[i]) {
          //     std::cout << "Mismatch: data index= " << i << " golden = " << C_sw[i]
          //               << ", kernel = " << C[i] << std::endl;
          //     return 1;
          //}
          //else
        	//  std::cout << "out :data index= " << i << " golden = " << C_sw[i] << std::endl;

	        std::cout << "out :data index= " << i << " " << j << " kernel = " << D[i*P_w+j] << std::endl;
         }
     }
     for (int i = (300); i < 301; i++) {
        for (int j = 0; j < P_w; j++) {
          //if (C_sw[i] != C[i]) {
          //     std::cout << "Mismatch: data index= " << i << " golden = " << C_sw[i]
          //               << ", kernel = " << C[i] << std::endl;
          //     return 1;
          //}
          //else
        	//  std::cout << "out :data index= " << i << " golden = " << C_sw[i] << std::endl;

	        std::cout << "out :data index= " << i << " " << j << " kernel = " << D[i*P_w+j] << std::endl;
         }
     }
     for (int i = (831); i < 832; i++) {
        for (int j = 0; j < P_w; j++) {
          //if (C_sw[i] != C[i]) {
          //     std::cout << "Mismatch: data index= " << i << " golden = " << C_sw[i]
          //               << ", kernel = " << C[i] << std::endl;
          //     return 1;
          //}
          //else
        	//  std::cout << "out :data index= " << i << " golden = " << C_sw[i] << std::endl;

	        std::cout << "out :data index= " << i << " " << j << " kernel = " << D[i*P_w+j] << std::endl;
         }
     }
     for (int i = (2000); i < 2001; i++) {
        for (int j = 0; j < P_w; j++) {
          //if (C_sw[i] != C[i]) {
          //     std::cout << "Mismatch: data index= " << i << " golden = " << C_sw[i]
          //               << ", kernel = " << C[i] << std::endl;
          //     return 1;
          //}
          //else
        	//  std::cout << "out :data index= " << i << " golden = " << C_sw[i] << std::endl;

	        std::cout << "out :data index= " << i << " " << j << " kernel = " << D[i*P_w+j] << std::endl;
         }
     }
     return 0;
}


void printVector(const vi& V, char* msg)
{

	std::cout << msg << "[ ";
	for_each(V.begin(), V.end(), [](int a) {
		std::cout << a << " ";
	});
	std::cout << "]" << std::endl;
}


void loadcsr_adj(
std::string file_name,
int   N,
int   M,
ATYPE *array_values,
int   *array_colIndices,
int   *array_rowPtr,
int   nnz_value)
{
	int i;
	// Helper vars
	std::string line;



	// Create an outuptu filestream
	//std::string file_name = "./weights_layer_" + std::to_string(layer_number) + ".csr";
	std::ifstream inFile(file_name);

	// Make sure the file is open
	if(!inFile.is_open()) 
		throw std::runtime_error("Could not open csr file");
	else
		std::cout << "reading " << file_name << " file" << std::endl;

    	// Read data, line by line
        // rowptr
    	std::getline(inFile, line);
	// Create a stringstream of the current line
	std::stringstream ss;

	ss << line;

	for (i = 0; i < N+1; i++) {
		int temp;
		ss >> temp; 
	        //std::cout << "row pointer " << array_rowPtr[i] << std::endl;

		//if (temp > nnz_value)
		//{
		//	std::cout << "Accumulated non-zeros " << array_rowPtr[i-1] << std::endl;
		//	array_rowPtr[i] = array_rowPtr[i-1];
		//}
		//else
		//{
			array_rowPtr[i] = temp;
		//}

        	// If the next token is a comma, ignore it and move on
	        if(ss.peek() == ',') ss.ignore();
	}

	

        // column_index
    	std::getline(inFile, line);
        //int *check_cols = malloc(100 * max_N_adj*sizeof(int));
 
	// Create a stringstream of the current line

	ss.str("");
        ss.clear();
	ss <<  line;

        //std::cout << "ss: " << ss.str() << std::endl;

        //std::cout << "nnz_value: " << nnz_value << std::endl;

	for (i = 0; i <  nnz_value; i++) {
		ss >> array_colIndices[i]; 
		//check_cols[i] = array_colIndices[i];
		//std::cout << "array colindx " << array_colIndices[i] << std::endl;

        	// If the next token is a comma, ignore it and move on
	        if(ss.peek() == ',') ss.ignore();
	}	

	//check if column is empty


    //for(i = 0; i< M;i++)
	//{
  	//	bool exists = std::find(std::begin(check_cols), std::end(check_cols), i) != std::end(check_cols);
	//	if (!exists)
	//	{
			//std::cout << "Attention: Column " << i << " is empty (loading it is not efficient) " << std::endl;
			//exit(0);
	//	}
	//}
				

        // values
    	std::getline(inFile, line);
	// Create a stringstream of the current line
	ss.str("");
        ss.clear();
	ss<<line;

	for (i = 0; i <  nnz_value; i++) {
		float float_val;
		ss >> float_val;
		array_values[i] = (ATYPE)float_val; 
		//std::cout << " float value " << float_val << " atype value " << array_values[i] << std::endl;
        	// If the next token is a comma, ignore it and move on
	        if(ss.peek() == ',') ss.ignore();
	}


	inFile.close();
	std::cout << "Number of non-zeros adj values in CSR file: " << nnz_value << std::endl;
	std::cout << "adj matrix size: " << M*N << std::endl;
	std::cout << "Total percentage of zero values in adj: " << (float)(M*N-nnz_value)/(float)(M*N) << std::endl;
	//std::cout << "Total percentage of zero values per row in adj: " << std::endl;
        //for(int i=0;i<N;i++)
	//	std::cout << 100*(1-(float)NNZR[i]/(float)M) << ",";

        //std::cout << "array values and col indices size each uses " << A.size()   << " integers" << std::endl;
        //std::cout << "row ptr uses " << IA.size()   << " integers" << std::endl;
	//exit(0);


}

void loadcsr_fea(
std::string file_name,
int   N,
int   M,
FTYPE *array_values,
int   *array_colIndices,
int   *array_rowPtr,
int   nnz_value)
{
	int i;
	// Helper vars
	std::string line;



	// Create an outuptu filestream
	//std::string file_name = "./weights_layer_" + std::to_string(layer_number) + ".csr";
	std::ifstream inFile(file_name);

	// Make sure the file is open
	if(!inFile.is_open()) 
		throw std::runtime_error("Could not open csr file");
	else
		std::cout << "reading " << file_name << " file" << std::endl;

    	// Read data, line by line
        // rowptr
    	std::getline(inFile, line);
	// Create a stringstream of the current line
	std::stringstream ss;

	ss << line;

	for (i = 0; i < N+1; i++) {
		int temp;
		ss >> temp; 
	        //std::cout << "row pointer " << array_rowPtr[i] << std::endl;

		//if (temp > nnz_value)
		//{
		//	std::cout << "Accumulated non-zeros " << array_rowPtr[i-1] << std::endl;
		//	array_rowPtr[i] = array_rowPtr[i-1];
		//}
		//else
		//{
			array_rowPtr[i] = temp;
		//}

        	// If the next token is a comma, ignore it and move on
	        if(ss.peek() == ',') ss.ignore();
	}

	

        // column_index
    	std::getline(inFile, line);
        //int check_cols[100 * max_N_adj];
 
	// Create a stringstream of the current line

	ss.str("");
        ss.clear();
	ss <<  line;

        //std::cout << "ss: " << ss.str() << std::endl;

        //std::cout << "nnz_value: " << nnz_value << std::endl;

	for (i = 0; i <  nnz_value; i++) {
		ss >> array_colIndices[i]; 
		//check_cols[i] = array_colIndices[i];
		//std::cout << "array colindx " << array_colIndices[i] << std::endl;

        	// If the next token is a comma, ignore it and move on
	        if(ss.peek() == ',') ss.ignore();
	}	

	//check if column is empty


    //    for(i = 0; i< M;i++)
	//{
  	//	bool exists = std::find(std::begin(check_cols), std::end(check_cols), i) != std::end(check_cols);
	//	if (!exists)
	//	{
			//std::cout << "Attention: Column " << i << " is empty (loading it is not efficient) " << std::endl;
			//exit(0);
	//	}
	//}
				

        // values
    	std::getline(inFile, line);
	// Create a stringstream of the current line
	ss.str("");
        ss.clear();
	ss<<line;

	for (i = 0; i <  nnz_value; i++) {
		float float_val;
		ss >> float_val;
		array_values[i] = (FTYPE)float_val; 
		//std::cout << " float value " << float_val << " atype value " << array_values[i] << std::endl;
        	// If the next token is a comma, ignore it and move on
	        if(ss.peek() == ',') ss.ignore();
	}


	inFile.close();
	std::cout << "Number of non-zeros adj values in CSR file: " << nnz_value << std::endl;
	std::cout << "adj matrix size: " << M*N << std::endl;
	std::cout << "Total percentage of zero values in adj: " << (float)(M*N-nnz_value)/(float)(M*N) << std::endl;
	//std::cout << "Total percentage of zero values per row in adj: " << std::endl;
        //for(int i=0;i<N;i++)
	//	std::cout << 100*(1-(float)NNZR[i]/(float)M) << ",";

        //std::cout << "array values and col indices size each uses " << A.size()   << " integers" << std::endl;
        //std::cout << "row ptr uses " << IA.size()   << " integers" << std::endl;
	//exit(0);


}



// Generate the three vectors A, IA, JA

void arraytocsr_fea(
FTYPE *V,
int N,
int M,
FTYPE *array_values,
int   *array_colIndices,
int   *array_rowPtr,
int   *nnz_value)
{
	int i, j;
	vi IA = { 0 }; // IA matrix has N+1 rows
	vi JA;
	int NNZ = 0;
	int NNZR[2500] = {0}; //number of non-zeros per row


	// Create an outuptu filestream
	//std::string file_name = "./weights_layer_" + std::to_string(layer_number) + ".csr";
	//std::ofstream outFile(file_name);

	// Make sure the file is open
	//if(!outFile.is_open()) 
	//	throw std::runtime_error("Could not open csr file");
	//else
	//	std::cout << "writting " << file_name << " file" << std::endl;

        std::cout << "arraytocsr size: " << N << " "<< M  << std::endl;


	for (i = 0; i < N; i++) {
		for (j = 0; j < M; j++) {
			//std::cout << " input " << i <<" "<< j << " " << V[i*M+j] << std::endl;
			if (V[i*M+j] != 0) {
				//printf("Non-zero is %f\n", V[i*M+j]);
				//std::cout << "Non zero adj is " << V[i*M+j] << std::endl;
				//A.push_back(V[i*M+j]);
				array_values[i*M+j] = V[i*M+j];
				JA.push_back(j);
				NNZ++;
				// Count Number of Non Zero
				// Elements in row i
				NNZR[i]++; 
			}
		}
		IA.push_back(NNZ);
	}


	//outFile << N << " " << M << " " << NNZ << std::endl;
        *nnz_value = NNZ;

	for(int i=0;i<JA.size();i++)
	{
		//outFile << JA[i] << " " << A[i] << std::endl;
		//array_values[i] = A[i];
		array_colIndices[i] = JA[i];
		//std::cout << "array values " << array_values[i] << " " << "array colindices " << array_colIndices[i] << std::endl;
	}

	for(int i=0;i<IA.size();i++)
	{
		//outFile << IA[i] << std::endl;
		array_rowPtr[i] =  IA[i];
		//std::cout << "row pointer " << IA[i] << std::endl;
	}

	//outFile.close();
	std::cout << "Number of non-zero fea values in CSR file: " << NNZ << std::endl;
	std::cout << "Total Number of fea values in CSR file: " << N*M << std::endl;
	std::cout << "Total percentage of zero values: " << (float)(N*M-NNZ)/(float)(N*M) << std::endl;
	//std::cout << "Total percentage of zero values per row: " << std::endl;
        //for(int i=0;i<N;i++)
	//	std::cout << 100*(1-(float)NNZR[i]/(float)M) << ",";

        std::cout << "array values and col indices size each uses " << JA.size()   << " integers" << std::endl;
        std::cout << "row ptr uses " << IA.size()   << " integers" << std::endl;


}


void arraytocsr_adj(
ATYPE *V,
int N,
int M,
ATYPE *array_values,
int   *array_colIndices,
int   *array_rowPtr,
int   *nnz_value)
{
	int i, j;
	vi IA = { 0 }; // IA matrix has N+1 rows
	vi JA;
	int NNZ = 0;
	int NNZR[2500] = {0}; //number of non-zeros per row


	// Create an outuptu filestream
	//std::string file_name = "./weights_layer_" + std::to_string(layer_number) + ".csr";
	//std::ofstream outFile(file_name);

	// Make sure the file is open
	//if(!outFile.is_open()) 
	//	throw std::runtime_error("Could not open csr file");
	//else
	//	std::cout << "writting " << file_name << " file" << std::endl;

        std::cout << "arraytocsr size: " << N << " "<< M  << std::endl;


	for (i = 0; i < N; i++) {
		for (j = 0; j < M; j++) {
			//std::cout << " input " << i <<" "<< j << " " << V[i*M+j] << std::endl;
			if (V[i*M+j] != 0) {
				//printf("Non-zero is %f\n", V[i*M+j]);
				//std::cout << "Non zero adj is " << V[i*M+j] << std::endl;
				//A.push_back(V[i*M+j]);
				array_values[i*M+j] = V[i*M+j];
				JA.push_back(j);
				NNZ++;
				// Count Number of Non Zero
				// Elements in row i
				NNZR[i]++; 
			}
		}
		IA.push_back(NNZ);
	}


	//outFile << N << " " << M << " " << NNZ << std::endl;
        *nnz_value = NNZ;

	for(int i=0;i<JA.size();i++)
	{
		//outFile << JA[i] << " " << A[i] << std::endl;
		//array_values[i] = A[i];
		array_colIndices[i] = JA[i];
		//std::cout << "array values " << array_values[i] << " " << "array colindices " << array_colIndices[i] << std::endl;
	}

	for(int i=0;i<IA.size();i++)
	{
		//outFile << IA[i] << std::endl;
		array_rowPtr[i] =  IA[i];
		//std::cout << "row pointer " << IA[i] << std::endl;
	}

	//outFile.close();
	std::cout << "Number of non-zero adj values in CSR file: " << NNZ << std::endl;
	std::cout << "Total Number of adj values in CSR file: " << N*M << std::endl;
	std::cout << "Total percentage of zero values: " << (float)(N*M-NNZ)/(float)(N*M) << std::endl;
	//std::cout << "Total percentage of zero values per row: " << std::endl;
        //for(int i=0;i<N;i++)
	//	std::cout << 100*(1-(float)NNZR[i]/(float)M) << ",";

        std::cout << "array values and col indices size each uses " << JA.size()   << " integers" << std::endl;
        std::cout << "row ptr uses " << IA.size()   << " integers" << std::endl;


}

int gnn_test(bool gemm_mode,ap_int<32> *quantized_multiplier,ap_int<32> *shift,ap_int<32> *bias,ap_int<32> bias_count,ap_int<64> *profiling,ap_int<8> zero_point_lhs,
ap_int<8> zero_point_rhs,ap_int<8> zero_point_dst,ap_int<8> clamp_max,ap_int<8> clamp_min,BTYPE *w_m,DTYPE *D_sw,DTYPE *D1,DTYPE *D2,DTYPE *D3,DTYPE *D4,FTYPE *values_fea,
int *colIndices_fea,int *rowPtr_fea,int nnz_fea,ATYPE *values_adj,int *colIndices_adj,int *rowPtr_adj,int nnz_adj,ATYPE *adj_m,  FTYPE *fea_m, 
int N_adj,int M_adj,int M_fea,int P_w,std::string adj_file,std::string fea_file,std::string w_file)
{
     //std::cout << "Testing " << std::endl;

     
     for (int i = 0; i < 1; i++) 
     {

        std::cout << "Loading sparse arrays" << std::endl;

	#ifdef generate_data

    	load_adj(N_adj,N_adj,adj_m,adj_file);

    	load_fea(N_adj,M_fea,fea_m,fea_file);    	

        std::cout << std::endl << std::endl << "Generating CSR for adj matrix" << std::endl;
        
	arraytocsr_adj(adj_m,N_adj,N_adj,values_adj,colIndices_adj,rowPtr_adj,&nnz_adj);

        std::cout << "done for adj matrix" << std::endl << std::endl << std::endl;
  
        std::cout << "Generating CSR for feature matrix" << std::endl;

	arraytocsr_fea(fea_m,N_adj,M_fea,values_fea,colIndices_fea,rowPtr_fea,&nnz_fea);
        
        std::cout << "done for feature matrix" << std::endl << std::endl << std::endl;

        load_weights(M_fea,P_w,w_m,w_file);

        //init_weights(w_m, D_sw, D);

	#else
        if (gemm_mode == 0)
        	loadcsr_fea(fea_name,N_adj,M_fea,values_fea,colIndices_fea,rowPtr_fea,NNZ_fea);
        else
        	load_fea(N_adj,M_fea,values_fea,fea_name);
        loadcsr_adj(adj_name,N_adj,N_adj,values_adj,colIndices_adj,rowPtr_adj,NNZ_adj);
        load_weights(M_fea,P_w,w_m,w_name);

	#endif

	double start_time, end_time, execution_time;

      	//======================ONLY CPU ==========================================


        std::cout << "Running GNN accelerator" << std::endl;
        start_time = getTimestamp();
    

   	kernelmult1(gemm_mode,quantized_multiplier,shift,bias,bias_count,profiling,zero_point_lhs,zero_point_rhs,zero_point_dst,clamp_max,clamp_min,
	w_m,D1,D2,D3,D4,values_fea,values_fea,values_fea,values_fea,
	colIndices_fea,colIndices_fea,colIndices_fea,colIndices_fea,
	rowPtr_fea,rowPtr_fea,rowPtr_fea,rowPtr_fea,
	values_adj,values_adj,values_adj,values_adj,
	colIndices_adj,colIndices_adj,colIndices_adj,colIndices_adj,
	rowPtr_adj,rowPtr_adj,rowPtr_adj,rowPtr_adj,
	N_adj,N_adj,M_fea,P_w);
   

        end_time = getTimestamp();

  	execution_time = (end_time - start_time) / (1000);       	

	std::cout << "CPU " << " Total execution time = " << execution_time << " msec" << std::endl;

	//if (N_adj > 102400)
	//{

    //   		 std::cout << "Running hw GNN accelerator" << std::endl;

	
   	//	kernelmult1(quantized_multiplier,shift,bias,bias_count,profiling,zero_point_lhs,zero_point_rhs,zero_point_dst,clamp_max,clamp_min,
	//	w_m,D,values_fea,colIndices_fea,rowPtr_fea,values_adj,colIndices_adj,rowPtr_adj,N_adj,N_adj,M_fea,P_w);
    //   	         execution_time = (end_time - start_time) / (1000);
    //   		 std::cout << "FPGA " << " Total execution time = " << execution_time << " msec" << std::endl;
	//}

    	//std::cout << "Golden" << std::endl;
        	  //std::cout << "A0  is" << A[0] << std::endl;

       	//start_time = getTimestamp();

     
      	//mmult_golden(A, B, C_sw);

	//end_time = getTimestamp();

     


         if (result_check(N_adj,P_w,D1, D_sw))
               return 1;
     }

     return 0;
}

/**
 * Design principles to achieve performance
 *
 * 1. sds_alloc to guarantee physically contiguous buffer allocation
 *    that enables the most efficient DMA configuration (axidma_simple)
 */
int main(int argc, char* argv[]){
	 int test_passed = 0;
     ATYPE *adj_m;
     FTYPE *fea_m; 
     BTYPE *w_m;

     ap_int<8> zero_point_lhs,zero_point_rhs,zero_point_dst,clamp_max,clamp_min;

     DTYPE *D_sw, *D;//, *D1,*D2,*D3,*D4;
     FTYPE *values_fea;
     int *colIndices_fea,*rowPtr_fea;
     ATYPE *values_adj;
     int *colIndices_adj,*rowPtr_adj;
     ap_int<32> *quantized_multiplier,*shift,*bias;
     ap_int<64> *profiling;
     int bias_count,nnz_fea,nnz_adj;

     #ifdef input_arguments

     if(argc<6)
     {
      	std::cout << "Error not enough arguments " << std::endl;
     	exit(1);
     }


     //std::ifstream adj_file(argv[1]);
     //std::ifstream fea_file(argv[2]);

     std::string adj_file(argv[1]);
     std::string fea_file(argv[2]);

     //SN,SM,SP
     N_adj = atoi(argv[3]);
     M_fea = atoi(argv[4]); 
     P_w = atoi(argv[5]);
     #else

     std::string adj_file(adj_name);
     std::string fea_file(fea_name);
     std::string w_file(w_name);

     #endif

     std::cout << "Matrix dimensions N_adj/M_adj " << N_adj << " M_fea " << M_fea << " P_w " << P_w << std::endl;

     D = (DTYPE *)malloc(N_adj*P_w *sizeof(DTYPE));
     //D1 = D;
     //D2 = D+N_adj/4;
     //D3 = (DTYPE *)malloc(N_adj*P_w *sizeof(DTYPE));
     //D4 = (DTYPE *)malloc(N_adj*P_w *sizeof(DTYPE));

     quantized_multiplier = (ap_int<32> *)malloc(max_N_adj*sizeof(ap_int<32>));
     shift = (ap_int<32> *)malloc(max_N_adj*sizeof(ap_int<32>));
     bias = (ap_int<32> *)malloc(max_N_adj*sizeof(ap_int<32>));
     profiling = (ap_int<64> *)malloc(max_N_adj*sizeof(ap_int<32>));

     values_fea = (FTYPE *)(malloc(max_M_fea * max_N_adj * sizeof(FTYPE)));
     //posix_memalign((void**)&values_fea, 4096, max_M_fea * max_N_adj * sizeof(FTYPE));
     colIndices_fea  = (int *)(malloc(max_M_fea * max_N_adj  * sizeof(int)));
     rowPtr_fea  = (int *)malloc(max_N_adj * sizeof(int));

     values_adj  = (ATYPE *)(malloc(max_N_adj * max_N_adj  * sizeof(ATYPE)));
     colIndices_adj  = (int *)(malloc(max_N_adj * max_N_adj  * sizeof(int)));
     rowPtr_adj  = (int *)malloc(max_N_adj * sizeof(int));
     
     w_m = (BTYPE *)malloc(max_M_fea * max_P_w * sizeof(BTYPE));
     D_sw = (DTYPE *)malloc(max_N_adj * max_P_w * sizeof(DTYPE));


     adj_m = (ATYPE *)malloc(max_N_adj * max_N_adj * sizeof(ATYPE));
     fea_m = (FTYPE *)malloc(max_N_adj * max_M_fea * sizeof(FTYPE));
     
     if (!values_adj || !colIndices_adj || !rowPtr_adj) {
          if (values_adj) free(values_adj);
          if (colIndices_adj) free(colIndices_adj);
          if (rowPtr_adj) free(rowPtr_adj);

	  std::cout << "Error allocating sparse adj memory " << std::endl;
          return 1;
     }

     if (!values_fea || !colIndices_fea || !rowPtr_fea) {
          if (values_fea) free(values_fea);
          if (colIndices_fea) free(colIndices_fea);
          if (rowPtr_fea) free(rowPtr_fea);

	  std::cout << "Error allocating sparse fea memory " << std::endl;
          return 1;
     }


     if (!adj_m || !fea_m || !D || !D_sw) {
          if (adj_m) free(adj_m);
          if (fea_m) free(fea_m);
          if (D) free(D);
          if (D_sw) free(D_sw);
	  std::cout << "Error allocating dense memory " << std::endl;
          return 1;
     }

     bias_count = 0;

     bool gemm_mode = use_gemm;

     test_passed = gnn_test(gemm_mode,quantized_multiplier,shift,bias,bias_count,profiling,zero_point_lhs,zero_point_rhs,zero_point_dst,clamp_max,
                  clamp_min,w_m,D_sw,D,D,D,D,values_fea,colIndices_fea,rowPtr_fea,nnz_fea,values_adj,colIndices_adj,rowPtr_adj,nnz_adj,adj_m, fea_m,
		   N_adj,N_adj,M_fea,P_w,adj_file,fea_file,w_file);
     
     std::cout << "TEST " << (test_passed ? "FAILED" : "PASSED") << std::endl;

     std::cout << "MAX ADJ " << max_adj << std::endl;
     std::cout << "MIN ADJ " << min_adj << std::endl;
     std::cout << "MAX FEA " << max_fea << std::endl;
     std::cout << "MIN FEA " << min_fea << std::endl;


     free(adj_m);
     free(fea_m);
     free(D);
     //free(D2);
     //free(D3);
     //free(D4);
     free(D_sw);
     
     return (test_passed ? -1 : 0);
}

