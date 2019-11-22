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


//--------------------------------------------------------
__global__ void  negativeFilterKernel(float* apOutputData,
                                      float* apInputData,
                                      float aMinValue,
                                      float aDynamicRange,
                                      int anImageSize)
//--------------------------------------------------------
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;

    if (i < anImageSize)
        apOutputData[i] = aMinValue + aDynamicRange * (1.0 - (apInputData[i] - aMinValue) / aDynamicRange);
}


//----------------------------------------------------------
__global__ void  shiftScaleFilterKernel(float* apOutputData,
                                        float* apInputData,
                                        float aShiftValue,
                                        float aScaleValue,
                                        int anImageSize)
//----------------------------------------------------------
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;

    if (i < anImageSize)
        apOutputData[i] = (apInputData[i] + aShiftValue) * aScaleValue;
}



//---------------------------------------------------
__global__ void  logFilterKernel(float* apOutputData,
                                 float* apInputData,
                                 int anImageSize)
//---------------------------------------------------
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;

    if (i < anImageSize)
        apOutputData[i] = log(apInputData[i]);
}


//----------------------------------------------------------
__global__ void  flipHorizontallyKernel(float* apOutputData,
                                        float* apInputData,
                                        int anImageWidth,
                                        int anImageHeight)
//----------------------------------------------------------
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;

    if (i < anImageWidth * anImageHeight)
    {
        // Retrieve the 2D index (x, y) from i

        // % is the "modulo operator", the remainder of i / m_width
        int x = i % anImageWidth;

        // where "/" is an integer division
        int y = i / anImageWidth;

        // Get the pixel
        apOutputData[i] = apInputData[y * anImageWidth + anImageWidth - x - 1];
    }
}


//--------------------------------------------------------
__global__ void  flipVerticallyKernel(float* apOutputData,
                                  float* apInputData,
                                  int anImageWidth,
                                  int anImageHeight)
//--------------------------------------------------------
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;

    if (i < anImageWidth * anImageHeight)
    {
        // Retrieve the 2D index (x, y) from i

        // % is the "modulo operator", the remainder of i / m_width
        int x = i % anImageWidth;

        // where "/" is an integer division
        int y = i / anImageWidth;

        // Get the pixel
        apOutputData[i] = apInputData[(anImageHeight - y - 1) * anImageWidth + x];
    }
}


//--------------------------
CudaImage::CudaImage():
//--------------------------
        Image(),
        m_p_device_memory(0),
        m_computing_time(0),
        m_host_to_device_transfer(0),
        m_device_to_host_transfer(0),
        m_device_to_device_transfer(0)
//--------------------------
{
    getNumberOfDevices();
}


//-----------------------------------------
CudaImage::CudaImage(const Image& anImage):
//-----------------------------------------
        Image(anImage),
        m_p_device_memory(0),
        m_computing_time(0),
        m_host_to_device_transfer(0),
        m_device_to_host_transfer(0),
        m_device_to_device_transfer(0)
//-----------------------------------------
{
    getNumberOfDevices();
    loadHost2Device();
}


//---------------------------------------------
CudaImage::CudaImage(const CudaImage& anImage):
//---------------------------------------------
        Image(anImage),
        m_p_device_memory(0),
        m_computing_time(anImage.m_computing_time),
        m_host_to_device_transfer(anImage.m_host_to_device_transfer),
        m_device_to_host_transfer(anImage.m_device_to_host_transfer),
        m_device_to_device_transfer(anImage.m_device_to_device_transfer)
//---------------------------------------------
{
    getNumberOfDevices();
    loadDevice2Device(anImage.m_p_device_memory);
}


//-----------------------------------------
CudaImage::CudaImage(const float* apData,
                     unsigned int aWidth,
                     unsigned int aHeight):
//-----------------------------------------
        Image(apData, aWidth, aHeight),
        m_p_device_memory(0),
        m_computing_time(0),
        m_host_to_device_transfer(0),
        m_device_to_host_transfer(0),
        m_device_to_device_transfer(0)
//-----------------------------------------
{
    getNumberOfDevices();
    loadHost2Device();
}


//--------------------------------------------
CudaImage::CudaImage(unsigned int aWidth,
                     unsigned int aHeight,
                     float aDefaultValue):
//--------------------------------------------
        Image(aWidth, aHeight, aDefaultValue),
        m_p_device_memory(0),
        m_computing_time(0),
        m_host_to_device_transfer(0),
        m_device_to_host_transfer(0),
        m_device_to_device_transfer(0)
//--------------------------------------------
{
    getNumberOfDevices();
    loadHost2Device();
}


//---------------------
CudaImage::~CudaImage()
//---------------------
{
    // Release the memory
    destroy();
}


//------------------------------------
float CudaImage::getElasedTime() const
//------------------------------------
{
    return m_computing_time + m_host_to_device_transfer + m_device_to_host_transfer + m_device_to_device_transfer;
}



//-------------------------------------------------------
CudaImage& CudaImage::operator=(const CudaImage& anImage)
//-------------------------------------------------------
{
    if (this != &anImage)
    {
        Image::operator=(anImage);

        m_computing_time = anImage.m_computing_time;
        m_host_to_device_transfer = anImage.m_host_to_device_transfer;
        m_device_to_host_transfer = anImage.m_device_to_host_transfer;
        m_device_to_device_transfer = anImage.m_device_to_device_transfer;

        loadDevice2Device(anImage.m_p_device_memory);
    }

    return *this;
}


//-----------------------
void CudaImage::destroy()
//-----------------------
{
    Image::destroy();
    releaseDeviceMemory();
}


//-----------------------------------
void CudaImage::releaseDeviceMemory()
//-----------------------------------
{
    // Release memory on device
    if (m_p_device_memory)
    {
        cudaFree(m_p_device_memory);
        checkCudaError(__FILE__, __FUNCTION__, __LINE__);

        m_p_device_memory = 0;
    }
}

//--------------------------------------------
void CudaImage::loadPGM(const char* aFileName)
//--------------------------------------------
{
    Image::loadPGM(aFileName);
    loadHost2Device();
}


//---------------------------------------------------
void CudaImage::loadPGM(const std::string& aFileName)
//---------------------------------------------------
{
    CudaImage::loadPGM(aFileName.c_str());
}


//--------------------------------------------
void CudaImage::savePGM(const char* aFileName)
//--------------------------------------------
{
    loadDevice2Host();
    Image::savePGM(aFileName);
}


//---------------------------------------------------
void CudaImage::savePGM(const std::string& aFileName)
//---------------------------------------------------
{
    CudaImage::savePGM(aFileName.c_str());
}


//----------------------------------------------
void CudaImage::loadASCII(const char* aFileName)
//----------------------------------------------
{
    Image::loadASCII(aFileName);
    loadHost2Device();
}


//-----------------------------------------------------
void CudaImage::loadASCII(const std::string& aFileName)
//-----------------------------------------------------
{
    CudaImage::loadASCII(aFileName.c_str());
}


//---------------------------------------------
void CudaImage::saveASCII(const char* aFileName)
//---------------------------------------------
{
    loadDevice2Host();
    Image::saveASCII(aFileName);
}


//-----------------------------------------------------
void CudaImage::saveASCII(const std::string& aFileName)
//-----------------------------------------------------
{
    CudaImage::saveASCII(aFileName.c_str());
}


//------------------------------
CudaImage CudaImage::operator!()
//------------------------------
{
    // Create an image of the right size
    CudaImage temp(getWidth(), getHeight(), 0.0);

    // Configure the kernel
    unsigned int image_size = m_width * m_height;
    int   block_size;
    int   min_grid_size;

    cudaOccupancyMaxPotentialBlockSize(&min_grid_size, &block_size, negativeFilterKernel, 0, image_size);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    int dim_grid = image_size / block_size;
    if (image_size % block_size) ++dim_grid;

    // Create events
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Record start event
       cudaEventRecord(start, 0);

       // Run the kernel
    float min_value = getMinValue();
    float range = getMaxValue() - min_value;
    negativeFilterKernel<<< dim_grid, block_size >>>(temp.m_p_device_memory,
            m_p_device_memory, min_value, range, image_size);
    cudaDeviceSynchronize();
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);


    // Record stop event
       cudaEventRecord(stop, 0);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

       // Compute the time in ms
       float duration;
    cudaEventElapsedTime(&duration, start, stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Update the total time
    m_computing_time += duration;

    // Destroy the events
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    return temp;
}


//------------------------------------------------------
CudaImage CudaImage::shiftScaleFilter(float aShiftValue,
                                      float aScaleValue)
//------------------------------------------------------
{
    // Create an image of the right size
    CudaImage temp(getWidth(), getHeight(), 0.0);

    // Configure the kernel
    unsigned int image_size = m_width * m_height;
    int   block_size;
    int   min_grid_size;

    cudaOccupancyMaxPotentialBlockSize(&min_grid_size, &block_size, shiftScaleFilterKernel, 0, image_size);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    int dim_grid = image_size / block_size;
    if (image_size % block_size) ++dim_grid;

    // Create events
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Record start event
       cudaEventRecord(start, 0);

       // Run the kernel
    shiftScaleFilterKernel<<< dim_grid, block_size >>>(temp.m_p_device_memory,
            m_p_device_memory, aShiftValue, aScaleValue, image_size);
    cudaDeviceSynchronize();
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);


    // Record stop event
       cudaEventRecord(stop, 0);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

       // Compute the time in ms
       float duration;
    cudaEventElapsedTime(&duration, start, stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Update the total time
    m_computing_time += duration;

    // Destroy the events
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    return temp;
}


//----------------------------------
CudaImage CudaImage::getNormalised()
//----------------------------------
{
    return shiftScaleFilter(-getMinValue(), 1.0 / (getMaxValue() - getMinValue()));
}


//----------------------------------
CudaImage CudaImage::getNormalized()
//----------------------------------
{
    return (getNormalised());
}


//------------------------------
CudaImage CudaImage::logFilter()
//------------------------------
{
    // Create an image of the right size
    CudaImage temp(getWidth(), getHeight(), 0.0);

    // Configure the kernel
    unsigned int image_size = m_width * m_height;
    int   block_size;
    int   min_grid_size;

    cudaOccupancyMaxPotentialBlockSize(&min_grid_size, &block_size, logFilterKernel, 0, image_size);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    int dim_grid = image_size / block_size;
    if (image_size % block_size) ++dim_grid;

    // Create events
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Record start event
       cudaEventRecord(start, 0);

       // Run the kernel
    logFilterKernel<<< dim_grid, block_size >>>(temp.m_p_device_memory,
            m_p_device_memory, image_size);
    cudaDeviceSynchronize();
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);


    // Record stop event
       cudaEventRecord(stop, 0);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

       // Compute the time in ms
       float duration;
    cudaEventElapsedTime(&duration, start, stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Update the total time
    m_computing_time += duration;

    // Destroy the events
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    return temp;
}


//-------------------------------------
CudaImage CudaImage::flipHorizontally()
//-------------------------------------
{
    // Create an image of the right size
    CudaImage temp(getWidth(), getHeight(), 0.0);

    // Configure the kernel
    unsigned int image_size = m_width * m_height;
    int   block_size;
    int   min_grid_size;

    cudaOccupancyMaxPotentialBlockSize(&min_grid_size, &block_size, flipHorizontallyKernel, 0, image_size);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    int dim_grid = image_size / block_size;
    if (image_size % block_size) ++dim_grid;

    // Create events
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Record start event
       cudaEventRecord(start, 0);

       // Run the kernel
    flipHorizontallyKernel<<< dim_grid, block_size >>>(temp.m_p_device_memory,
            m_p_device_memory, m_width, m_height);
    cudaDeviceSynchronize();
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);


    // Record stop event
       cudaEventRecord(stop, 0);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

       // Compute the time in ms
       float duration;
    cudaEventElapsedTime(&duration, start, stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Update the total time
    m_computing_time += duration;

    // Destroy the events
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    return temp;
}


//-----------------------------------
CudaImage CudaImage::flipVertically()
//-----------------------------------
{
    // Create an image of the right size
    CudaImage temp(getWidth(), getHeight(), 0.0);

    // Configure the kernel
    unsigned int image_size = m_width * m_height;
    int   block_size;
    int   min_grid_size;

    cudaOccupancyMaxPotentialBlockSize(&min_grid_size, &block_size, flipVerticallyKernel, 0, image_size);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    int dim_grid = image_size / block_size;
    if (image_size % block_size) ++dim_grid;

    // Create events
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Record start event
       cudaEventRecord(start, 0);

       // Run the kernel
    flipVerticallyKernel<<< dim_grid, block_size >>>(temp.m_p_device_memory,
            m_p_device_memory, m_width, m_height);
    cudaDeviceSynchronize();
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);


    // Record stop event
       cudaEventRecord(stop, 0);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

       // Compute the time in ms
       float duration;
    cudaEventElapsedTime(&duration, start, stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Update the total time
    m_computing_time += duration;

    // Destroy the events
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    return temp;
}


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

    throw error_message.str();
    }
}


//-------------------------------
void CudaImage::loadHost2Device()
//-------------------------------
{
    // Release the device memory if needed
    releaseDeviceMemory();

    // Allocate the memory on device
    unsigned int image_size = m_width * m_height;
    unsigned int array_size = sizeof(float) * image_size;

    cudaMalloc((void**) &m_p_device_memory, array_size);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Create events
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Record start event
       cudaEventRecord(start, 0);

       // Copy host memory to device memory
    cudaMemcpy(m_p_device_memory, &m_p_image[0], array_size, cudaMemcpyHostToDevice);

    // Record stop event
       cudaEventRecord(stop, 0);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

       // Compute the time in ms
       float duration;
    cudaEventElapsedTime(&duration, start, stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Update the total time
    m_host_to_device_transfer += duration;

    // Destroy the events
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);
}


//-------------------------------
void CudaImage::loadDevice2Host()
//-------------------------------
{
    unsigned int image_size = m_width * m_height;
    unsigned int array_size = sizeof(float) * image_size;

    // Create events
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Record start event
       cudaEventRecord(start, 0);

       // Copy device memory to host memory
    cudaMemcpy(&m_p_image[0], m_p_device_memory, array_size, cudaMemcpyDeviceToHost);

    // Record stop event
       cudaEventRecord(stop, 0);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

       // Compute the time in ms
       float duration;
    cudaEventElapsedTime(&duration, start, stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Update the total time
    m_device_to_host_transfer += duration;

    // Destroy the events
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);
}


//-------------------------------------------------------------
void CudaImage::loadDevice2Device(const float* apImageOnDevice)
//-------------------------------------------------------------
{
    // Release the device memory if needed
    releaseDeviceMemory();

    // Allocate the memory on device
    unsigned int image_size = m_width * m_height;
    unsigned int array_size = sizeof(float) * image_size;

    cudaMalloc((void**) &m_p_device_memory, array_size);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Create events
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Record start event
       cudaEventRecord(start, 0);

       // Copy device memory to device memory
    cudaMemcpy(m_p_device_memory, apImageOnDevice, array_size, cudaMemcpyDeviceToDevice);

    // Record stop event
       cudaEventRecord(stop, 0);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

       // Compute the time in ms
       float duration;
    cudaEventElapsedTime(&duration, start, stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);

    // Update the total time
    m_device_to_device_transfer += duration;

    // Destroy the events
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    checkCudaError(__FILE__, __FUNCTION__, __LINE__);
}
