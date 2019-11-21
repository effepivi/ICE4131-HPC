/**
********************************************************************************
*
*   @file       CudaImage.cxx
*
*   @brief      Class to handle a greyscale image using Nvidia CUDA to speedup computations on GPU.
*
*   @version    1.0
*
*   @todo       Write here anything you did not implement.
*
*   @date       21/11/2019
*
*   @author     Franck Vidal
*   @author     YOUR NAME
*
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <cmath> // Header file for abs and log
#include <limits>
#include <iostream>
#include <sstream>

#include <cuda.h> // Main cuda header for low-level driver API
#include <cuda_runtime_api.h> // Main cuda header for high-level runtime  programming in C
#include <cuda_runtime.h> // Main cuda header for high-level programming in C++
//#include <cutil_math.h>

#include "CudaImage.h"


int CudaImage::m_device_count = 0;


__global__ void  logFilterKernel(float* apOutputData, float* apInputData)
{

}


//---------------------
CudaImage::CudaImage():
//---------------------
        Image()
//---------------------
{
    getNumberOfDevices();
}


//-----------------------------------------
CudaImage::CudaImage(const Image& anImage):
//-----------------------------------------
        Image(anImage)
//-----------------------------------------
{
    getNumberOfDevices();
}


//-----------------------------------------
CudaImage::CudaImage(const float* apData,
                     unsigned int aWidth,
                     unsigned int aHeight):
//-----------------------------------------
        Image(apData, aWidth, aHeight)
//-----------------------------------------
{
    getNumberOfDevices();
}


//-------------------------------------------
CudaImage::CudaImage(unsigned int aWidth,
                     unsigned int aHeight,
                     float aDefaultValue):
//-------------------------------------------
        Image(aWidth, aHeight, aDefaultValue)
//-------------------------------------------
{
    getNumberOfDevices();
}


//-------------------------------------------
void CudaImage::savePGM(const char* aFileName)
//-------------------------------------------
{
    loadDevice2Host();
    Image::savePGM(aFileName);
}


//---------------------------------------------
void CudaImage::saveASCII(const char* aFileName)
//---------------------------------------------
{
    loadDevice2Host();
    Image::saveASCII(aFileName);
}


//------------------------------------
CudaImage CudaImage::operator!() const
//------------------------------------
{
    // Create an image of the right size
    CudaImage temp(getWidth(), getHeight(), 0.0);

    //float min_value(getMinValue());
    //float max_value(getMaxValue());
    //float range(max_value - min_value);

    // There is no device
    if (!m_device_count)
    {
        // Use serial code
        Image::operator!();
    }

    loadHost2Device();

/*
    // Process every pixel of the sub-image
    for (unsigned int i = pixel_start_id; i <= pixel_end_id; ++i)
    {
        // Take care to preserve the dynamic of the image
        temp[i] = min_value + range * (1.0 - (m_p_image[i] - min_value) / range);
    }
*/

    return temp;
}


//----------------------------------------------------------
CudaImage CudaImage::shiftScaleFilter(float aShiftValue,
                                    float aScaleValue) const
//----------------------------------------------------------
{
    // Create an image of the right size
    CudaImage temp(getWidth(), getHeight(), 0.0);
/*
    // Get the work load
    unsigned int pixel_start_id = 0;
    unsigned int pixel_end_id = 0;
    workload(m_width * m_height, pixel_start_id, pixel_end_id);

    // Process every pixel of the sub-image
    for (unsigned int i = pixel_start_id; i <= pixel_end_id; ++i)
    {
        // Apply the shilft/scale filter
        temp[i] = (m_p_image[i] + aShiftValue) * aScaleValue;
    }

    // Get the process' rank
    int rank;
    Cuda_Comm_rank(Cuda_COMM_WORLD, &rank);

    // Master gather results from all the processes
    if (rank == ROOT)
    {
        int world_size;
        Cuda_Comm_size(Cuda_COMM_WORLD, &world_size);

        for (int i = 1; i < world_size; ++i)
        {
            int pixel_start_id;
            int pixel_end_id;

            Cuda_Status status;

            checkCudaError(Cuda_Recv(&pixel_start_id, 1, Cuda_INT, i, 0, Cuda_COMM_WORLD, &status));
            checkCudaError(Cuda_Recv(&pixel_end_id, 1, Cuda_INT, i, 1, Cuda_COMM_WORLD, &status));
            checkCudaError(Cuda_Recv(&temp[pixel_start_id], pixel_end_id - pixel_start_id + 1, Cuda_FLOAT, i, 2, Cuda_COMM_WORLD, &status));
        }
    }
    // Other processes send the data to the master
    else
    {

        checkCudaError(Cuda_Send(&pixel_start_id, 1, Cuda_INT, ROOT, 0, Cuda_COMM_WORLD));
        checkCudaError(Cuda_Send(&pixel_end_id,   1, Cuda_INT, ROOT, 1, Cuda_COMM_WORLD));
        checkCudaError(Cuda_Send(&temp[pixel_start_id], pixel_end_id - pixel_start_id + 1, Cuda_FLOAT, ROOT, 2, Cuda_COMM_WORLD));
    }
*/
    return temp;
}


//----------------------------------
CudaImage CudaImage::logFilter() const
//----------------------------------
{
    // Create an image of the right size
    CudaImage temp(getWidth(), getHeight(), 0.0);

	// Allocate the memory on device
    float* p_device_input_image_data  = 0;
    float* p_device_output_image_data = 0;

    unsigned int array_size = sizeof(float) * m_width * m_height;

    cudaMalloc((void**) &p_device_input_image_data, array_size);
    cudaMalloc((void**) &p_device_output_image_data, array_size);
	checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Copy host memory to device memory
	cudaMemcpy(p_device_input_image_data, &m_p_image[0], array_size, cudaMemcpyHostToDevice);

    // Configure the kernel
	int   blockSize;
    int   minGridSize;

    // Get the work load
    cudaOccupancyMaxPotentialBlockSize(&minGridSize, &blockSize, logFilterKernel, 0, m_width * m_height);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);


    // Note that 256 x 16 = 4096, i.e. the number of elements in the arrays.

	// Run the kernel
	logFilterKernel<<< minGridSize, blockSize >>>(p_device_output_image_data,
		p_device_input_image_data);
	cudaDeviceSynchronize();
	checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Copy host memory to device memory
    cudaMemcpy(p_device_output_image_data, &(temp.m_p_image[0]), array_size, cudaMemcpyDeviceToHost);

/*
    // Get the work load
    unsigned int pixel_start_id = 0;
    unsigned int pixel_end_id = 0;
    workload(m_width * m_height, pixel_start_id, pixel_end_id);

    // Process every pixel of the sub-image
    for (unsigned int i = pixel_start_id; i <= pixel_end_id; ++i)
    {
        // Apply the log filter
        temp[i] = log(m_p_image[i]);
    }

    // Get the process' rank
    int rank;
    Cuda_Comm_rank(Cuda_COMM_WORLD, &rank);

    // Master gather results from all the processes
    if (rank == ROOT)
    {
        int world_size;
        Cuda_Comm_size(Cuda_COMM_WORLD, &world_size);

        for (int i = 1; i < world_size; ++i)
        {
            int pixel_start_id;
            int pixel_end_id;

            Cuda_Status status;

            checkCudaError(Cuda_Recv(&pixel_start_id, 1, Cuda_INT, i, 0, Cuda_COMM_WORLD, &status));
            checkCudaError(Cuda_Recv(&pixel_end_id, 1, Cuda_INT, i, 1, Cuda_COMM_WORLD, &status));
            checkCudaError(Cuda_Recv(&temp[pixel_start_id], pixel_end_id - pixel_start_id + 1, Cuda_FLOAT, i, 2, Cuda_COMM_WORLD, &status));
        }
    }
    // Other processes send the data to the master
    else
    {
        checkCudaError(Cuda_Send(&pixel_start_id, 1, Cuda_INT, ROOT, 0, Cuda_COMM_WORLD));
        checkCudaError(Cuda_Send(&pixel_end_id,   1, Cuda_INT, ROOT, 1, Cuda_COMM_WORLD));
        checkCudaError(Cuda_Send(&temp[pixel_start_id], pixel_end_id - pixel_start_id + 1, Cuda_FLOAT, ROOT, 2, Cuda_COMM_WORLD));
    }
*/
    return temp;
}


//-----------------------------------------
CudaImage CudaImage::flipHorizontally() const
//-----------------------------------------
{
    // Create an image of the right size
    CudaImage temp(getWidth(), getHeight(), 0.0);
/*
    // Get the work load
    unsigned int pixel_start_id = 0;
    unsigned int pixel_end_id = 0;
    workload(m_width * m_height, pixel_start_id, pixel_end_id);

    // Process every pixel of the sub-image
    for (unsigned int i = pixel_start_id; i <= pixel_end_id; ++i)
    {
        // Retrieve the 2D index (x, y) from i

        // % is the "modulo operator", the remainder of i / m_width
        int x = i % m_width;

        // where "/" is an integer division
        int y = i / m_width;

        // Get the pixel
        temp(x, y) = getPixel(m_width - x - 1, y);
    }

    // Get the process' rank
    int rank;
    Cuda_Comm_rank(Cuda_COMM_WORLD, &rank);

    // Master gather results from all the processes
    if (rank == ROOT)
    {
        int world_size;
        Cuda_Comm_size(Cuda_COMM_WORLD, &world_size);

        for (int i = 1; i < world_size; ++i)
        {
            int pixel_start_id;
            int pixel_end_id;

            Cuda_Status status;

            checkCudaError(Cuda_Recv(&pixel_start_id, 1, Cuda_INT, i, 0, Cuda_COMM_WORLD, &status));
            checkCudaError(Cuda_Recv(&pixel_end_id, 1, Cuda_INT, i, 1, Cuda_COMM_WORLD, &status));
            checkCudaError(Cuda_Recv(&temp[pixel_start_id], pixel_end_id - pixel_start_id + 1, Cuda_FLOAT, i, 2, Cuda_COMM_WORLD, &status));
        }
    }
    // Other processes send the data to the master
    else
    {
        checkCudaError(Cuda_Send(&pixel_start_id, 1, Cuda_INT, ROOT, 0, Cuda_COMM_WORLD));
        checkCudaError(Cuda_Send(&pixel_end_id,   1, Cuda_INT, ROOT, 1, Cuda_COMM_WORLD));
        checkCudaError(Cuda_Send(&temp[pixel_start_id], pixel_end_id - pixel_start_id + 1, Cuda_FLOAT, ROOT, 2, Cuda_COMM_WORLD));
    }
*/
    return temp;
}


//---------------------------------------
CudaImage CudaImage::flipVertically() const
//---------------------------------------
{
    // Create an image of the right size
    CudaImage temp(getWidth(), getHeight(), 0.0);
/*
    // Get the work load
    unsigned int pixel_start_id = 0;
    unsigned int pixel_end_id = 0;
    workload(m_width * m_height, pixel_start_id, pixel_end_id);

    // Process every pixel of the sub-image
    for (unsigned int i = pixel_start_id; i <= pixel_end_id; ++i)
    {
        // Retrieve the 2D index (x, y) from i

        // % is the "modulo operator", the remainder of i / m_width
        int x = i % m_width;

        // where "/" is an integer division
        int y = i / m_width;

        // Get the pixel
        temp(x, y) = getPixel(x, m_height - y - 1);
    }

    // Get the process' rank
    int rank;
    Cuda_Comm_rank(Cuda_COMM_WORLD, &rank);

    // Master gather results from all the processes
    if (rank == ROOT)
    {
        int world_size;
        Cuda_Comm_size(Cuda_COMM_WORLD, &world_size);

        for (int i = 1; i < world_size; ++i)
        {
            int pixel_start_id;
            int pixel_end_id;

            Cuda_Status status;

            checkCudaError(Cuda_Recv(&pixel_start_id, 1, Cuda_INT, i, 0, Cuda_COMM_WORLD, &status));
            checkCudaError(Cuda_Recv(&pixel_end_id, 1, Cuda_INT, i, 1, Cuda_COMM_WORLD, &status));
            checkCudaError(Cuda_Recv(&temp[pixel_start_id], pixel_end_id - pixel_start_id + 1, Cuda_FLOAT, i, 2, Cuda_COMM_WORLD, &status));
        }
    }
    // Other processes send the data to the master
    else
    {
        checkCudaError(Cuda_Send(&pixel_start_id, 1, Cuda_INT, ROOT, 0, Cuda_COMM_WORLD));
        checkCudaError(Cuda_Send(&pixel_end_id,   1, Cuda_INT, ROOT, 1, Cuda_COMM_WORLD));
        checkCudaError(Cuda_Send(&temp[pixel_start_id], pixel_end_id - pixel_start_id + 1, Cuda_FLOAT, ROOT, 2, Cuda_COMM_WORLD));
    }
*/
    return temp;
}

/*
//-----------------------------------------------------
void CudaImage::workload(unsigned int aNumberOfElements,
                        unsigned int& aStartID,
                        unsigned int& aEndID) const
//-----------------------------------------------------
{
    int world_size;
    int rank;

    Cuda_Comm_size(Cuda_COMM_WORLD, &world_size);
    Cuda_Comm_rank(Cuda_COMM_WORLD, &rank);

    int last_element = -1;
    unsigned int element_per_task = aNumberOfElements / world_size;
    unsigned int remainder = aNumberOfElements % world_size;

    for (int i = 0; i < world_size; ++i)
    {
        aStartID = ++last_element;
        aEndID = last_element + element_per_task - 1;

        if (remainder > 0)
        {
            aEndID++;
            --remainder;
        }
        last_element = aEndID;

        // Exit the method
        if (rank == i) return;
    }
}
*/

//---------------------------------
int CudaImage::getNumberOfDevices()
//---------------------------------
{
    cudaGetDeviceCount(&m_device_count);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    std::cerr << "Number of CUDA devices:\t" << m_device_count << std::endl;

    return m_device_count;
}


//-------------------------------------------------------
void CudaImage::checkCudaError(const char* aFileName,
                               const char* aFunctionName,
                               unsigned int aLineNumber)
//-------------------------------------------------------
{
	// Get the latest CUDA error.
	cudaError_t error_code = cudaGetLastError();

	// A CUDA error has occured.
	if (error_code != cudaSuccess)
	{
        std::stringstream error_message;
        error_message << "CUDA error: " << cudaGetErrorString(error_code) << std::endl;

        error_message << "\tin file: " << aFileName << std::endl;
        error_message << "\tin function: " << aFunctionName << std::endl;
        error_message << "\tat line: " << aLineNumber << std::endl;

		throw error_message;
	}
}


//-------------------------------------
void CudaImage::loadHost2Device() const
//-------------------------------------
{}


//-------------------------------------
void CudaImage::loadDevice2Host() const
//-------------------------------------
{}
