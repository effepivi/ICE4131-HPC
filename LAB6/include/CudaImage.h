#ifndef __CudaImage_h
#define __CudaImage_h


/**
********************************************************************************
*
*   @file       CudaImage.h
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
#include "Image.h"


//==============================================================================
/**
*   @class  CudaImage
*   @brief  CudaImage is a class to manage a greyscale image using Nvidia CUDA
*           to speedup computations on GPU
*/
//==============================================================================
class CudaImage: public Image
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    CudaImage();


    //------------------------------------------------------------------------
    /// Copy constructor.
    /**
    * @param anImage: the image to copy
    */
    //------------------------------------------------------------------------
    CudaImage(const Image& anImage);


    //------------------------------------------------------------------------
    /// Constructor from an array.
    /**
    * @param apData: the array to copy
    * @param aWidth: the width of the image
    * @param aHeight: the height of the image
    */
    //------------------------------------------------------------------------
    CudaImage(const float* apData,
                 unsigned int aWidth,
                 unsigned int aHeight);


    //------------------------------------------------------------------------
    /// Constructor to build a black image.
    /**
    * @param aWidth: the width of the image
    * @param aHeight: the height of the image
    */
    //------------------------------------------------------------------------
    CudaImage(unsigned int aWidth,
                 unsigned int aHeight,
                float aDefaultValue = 0.0);


    //------------------------------------------------------------------------
    /// Save the image in a PGM file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void savePGM(const char* aFileName);


    //------------------------------------------------------------------------
    /// Save the image in an ASCII file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void saveASCII(const char* aFileName);


    //------------------------------------------------------------------------
    /// Negation operator. Compute the negative of the current image.
    /**
    * @return the negative image
    */
    //------------------------------------------------------------------------
    CudaImage operator!() const;


    //------------------------------------------------------------------------
    /// Add aShiftValue to every pixel, then multiply every pixel
    /// by aScaleValue
    /**
    * @param aShiftValue: the shift parameter of the filter
    * @param aScaleValue: the scale parameter of the filter
    * @return the new image
    */
    //------------------------------------------------------------------------
    CudaImage shiftScaleFilter(float aShiftValue, float aScaleValue) const;


    //------------------------------------------------------------------------
    /// Apply a log filter on the image
    /**
    * @return the new image
    */
    //------------------------------------------------------------------------
    CudaImage logFilter() const;


    //------------------------------------------------------------------------
    /// Flip the image horizonatally
    /**
    * @return the new image
    */
    //------------------------------------------------------------------------
    CudaImage flipHorizontally() const;


    //------------------------------------------------------------------------
    /// Flip the image vertically
    /**
    * @return the new image
    */
    //------------------------------------------------------------------------
    CudaImage flipVertically() const;


    //------------------------------------------------------------------------
    /// Accessor on the number of devices
    /**
    * @return the number of devices
    */
    //------------------------------------------------------------------------
    static int getNumberOfDevices();


//******************************************************************************
protected:
    static void checkCudaError(const char* aFileName,
            const char* aFunctionName,
            unsigned int aLineNumber);

    void loadHost2Device() const;
    void loadDevice2Host() const;

    // Make sure there is at least one CUDA device
    static int m_device_count;
};


#endif
