#include <iostream>
#include <algorithm>

#include <cuda_runtime_api.h> // Main cuda header for high-level runtime
                              //   programming in C


//	Function declarations
__global__ void Kernel1(float* apOutputData,
                        float* apInputData0,
                        float* apInputData1);

void initializeArray(float* apArray);
void cleanup();
void checkCudaError(const char* aFileName,
                    const char* aFunctionName,
                    unsigned int aLineNumber);


// Constant variables
const unsigned int WIDTH    = 4096;

//	Global variables
float* g_p_host_float_set0   = NULL;
float* g_p_host_float_set1   = NULL;
float* g_p_host_float_set2   = NULL;
float* g_p_device_float_set0 = NULL;
float* g_p_device_float_set1 = NULL;
float* g_p_device_float_set2 = NULL;


//------------------------------------------------
__global__ void Kernel1(float* apOutputData,
                        float* apInputData0,
                        float* apInputData1)
//------------------------------------------------
{
	// Get element index
	int i = blockDim.x * blockIdx.x + threadIdx.x;

    // i is a valid index
    if (i < WIDTH)
    	// Element-wise sum
    	apOutputData[i] = apInputData0[i] + apInputData1[i];
}

int main()
{
	// Register an exit callback to make sure that the memory is released in cas
    // the program stops due to an error:
	atexit(cleanup);

	// Make sure there is at least one CUDA device
	int device_count;
	cudaGetDeviceCount(&device_count);
	checkCudaError(__FILE__, __FUNCTION__, __LINE__);

	// There is no device
	if (!device_count)
	{
        std::cerr << "There is no CUDA device on this system. The program will terminate." << std::endl;
		return 1;
	}

    // Compute the size of the array in number of bytes
	unsigned int array_size = WIDTH * sizeof(float);

	// Allocate the memory on host
    g_p_host_float_set0 = new float[WIDTH];
    g_p_host_float_set1 = new float[WIDTH];
    g_p_host_float_set2 = new float[WIDTH];

	// Allocate the memory on device
	cudaMalloc((void**) &g_p_device_float_set0, array_size);
	cudaMalloc((void**) &g_p_device_float_set1, array_size);
	cudaMalloc((void**) &g_p_device_float_set2, array_size);
	checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Initialize random number generator
	srand(time(NULL));

	// Initialize host memory using random numbers
	initializeArray(g_p_host_float_set0);
	initializeArray(g_p_host_float_set1);

    // Copy host memory to device memory
	cudaMemcpy(g_p_device_float_set0, g_p_host_float_set0, array_size, cudaMemcpyHostToDevice);
	cudaMemcpy(g_p_device_float_set1, g_p_host_float_set1, array_size, cudaMemcpyHostToDevice);
	checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Configure the kernel
    int   DimBlock = 256; // 256 threads per block
    int   DimGrid  =  WIDTH / DimBlock;

    // Make sure there are enough blocks
    if (!(WIDTH % DimBlock)) ++DimGrid;

    // Run the kernel
	Kernel1<<< DimGrid, DimBlock >>>(g_p_device_float_set2,
		g_p_device_float_set0, g_p_device_float_set1);
	cudaThreadSynchronize();
	checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Retrieve the result
    cudaMemcpy(g_p_host_float_set2, g_p_device_float_set2, array_size,
        cudaMemcpyDeviceToHost);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Display the results
    for (unsigned int i = 0; i < WIDTH; ++i)
    {
        printf("%i:\t%f + %f = %f\n",
            i,
            g_p_host_float_set0[i],
            g_p_host_float_set1[i],
            g_p_host_float_set2[i]);

    }

    return 0;
}


//--------------------------------------------
void checkCudaError(const char* aFileName,
                    const char* aFunctionName,
                    unsigned int aLineNumber)
//--------------------------------------------
{
	// Get the latest CUDA error.
	cudaError_t error_code = cudaGetLastError();

	// A CUDA error has occured.
	if (error_code != cudaSuccess)
	{
		// Display an error message in the console.
		fprintf(stderr, "CUDA error: %s\n\tin file: %s\n\tin function%s\n\tat line: %i.",
			cudaGetErrorString(error_code),
			aFileName, aFunctionName, aLineNumber);

		// Quit the program.
		exit(EXIT_FAILURE);
	}
}


//------------
void cleanup()
//------------
{
	// Release memory
	printf( "Release memory if needed.\n");

	// Free host memory
    if (g_p_host_float_set0)
	{
		delete [] g_p_host_float_set0;
		g_p_host_float_set0 = NULL;
	}
	if (g_p_host_float_set1)
	{
		delete [] g_p_host_float_set1;
		g_p_host_float_set1 = NULL;
	}
	if (g_p_host_float_set2)
	{
		delete [] g_p_host_float_set2;
		g_p_host_float_set2 = NULL;
	}

    // Free device memory
    if (g_p_device_float_set0)
    {
        cudaFree(g_p_device_float_set0);
        g_p_device_float_set0 = NULL;
    }
    if (g_p_device_float_set1)
    {
        cudaFree(g_p_device_float_set1);
        g_p_device_float_set1 = NULL;
    }
    if (g_p_device_float_set2)
    {
        cudaFree(g_p_device_float_set2);
        g_p_device_float_set2 = NULL;
    }
}


//----------------------------------
void initializeArray(float* apArray)
//----------------------------------
{
	for (unsigned int i = 0; i < WIDTH; ++i)
	{
		apArray[i] = floor(255.0f * (float)rand() / (float)RAND_MAX);
	}
}
