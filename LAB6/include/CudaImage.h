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
    /// Copy constructor.
    /**
    * @param anImage: the image to copy
    */
    //------------------------------------------------------------------------
    CudaImage(const CudaImage& anImage);


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


    //--------------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------------
    ~CudaImage();



    float getElasedTime() const;

    //------------------------------------------------------------------------
    /// Assignment operator (also called copy operator).
    /**
    * @param anImage: the image to copy
    * @return the updated version of the current image
    */
    //------------------------------------------------------------------------
    CudaImage& operator=(const CudaImage& anImage);


    //------------------------------------------------------------------------
    /// Release the memory.
    //------------------------------------------------------------------------
    virtual void destroy();


    //------------------------------------------------------------------------
    /// Release the memory on the device.
    //------------------------------------------------------------------------
	void releaseDeviceMemory();


    //------------------------------------------------------------------------
    /// Load an image from a PGM file
    /**
    * @param aFileName: the name of the file to load
    */
    //------------------------------------------------------------------------
    virtual void loadPGM(const char* aFileName);


    //------------------------------------------------------------------------
    /// Load an image from a PGM file
    /**
    * @param aFileName: the name of the file to load
    */
    //------------------------------------------------------------------------
    virtual void loadPGM(const std::string& aFileName);


    //------------------------------------------------------------------------
    /// Save the image in a PGM file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    virtual void savePGM(const char* aFileName);


    //------------------------------------------------------------------------
    /// Save the image in a PGM file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    virtual void savePGM(const std::string& aFileName);


    //------------------------------------------------------------------------
    /// Load an image from an ASCII file
    /**
    * @param aFileName: the name of the file to load
    */
    //------------------------------------------------------------------------
    virtual void loadASCII(const char* aFileName);


    //------------------------------------------------------------------------
    /// Load an image from an ASCII file
    /**
    * @param aFileName: the name of the file to load
    */
    //------------------------------------------------------------------------
    virtual void loadASCII(const std::string& aFileName);


    //------------------------------------------------------------------------
    /// Save the image in an ASCII file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    virtual void saveASCII(const char* aFileName);


    //------------------------------------------------------------------------
    /// Save the image in an ASCII file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    virtual void saveASCII(const std::string& aFileName);


    //------------------------------------------------------------------------
    /// Negation operator. Compute the negative of the current image.
    /**
    * @return the negative image
    */
    //------------------------------------------------------------------------
    CudaImage operator!();


    //------------------------------------------------------------------------
    /// Add aShiftValue to every pixel, then multiply every pixel
    /// by aScaleValue
    /**
    * @param aShiftValue: the shift parameter of the filter
    * @param aScaleValue: the scale parameter of the filter
    * @return the new image
    */
    //------------------------------------------------------------------------
    CudaImage shiftScaleFilter(float aShiftValue, float aScaleValue);


    //------------------------------------------------------------------------
    /// Normalise the image between 0 and 1
    /**
    * @return the new image
    */
    //------------------------------------------------------------------------
    CudaImage getNormalised();


    //------------------------------------------------------------------------
    /// Normalize the image between 0 and 1
    /**
    * @return the new image
    */
    //------------------------------------------------------------------------
    CudaImage getNormalized();


    //------------------------------------------------------------------------
    /// Apply a log filter on the image
    /**
    * @return the new image
    */
    //------------------------------------------------------------------------
    CudaImage logFilter();


    //------------------------------------------------------------------------
    /// Flip the image horizonatally
    /**
    * @return the new image
    */
    //------------------------------------------------------------------------
    CudaImage flipHorizontally();


    //------------------------------------------------------------------------
    /// Flip the image vertically
    /**
    * @return the new image
    */
    //------------------------------------------------------------------------
    CudaImage flipVertically();


    //------------------------------------------------------------------------
    /// Accessor on the number of devices
    /**
    * @return the number of devices
    */
    //------------------------------------------------------------------------
    static int getNumberOfDevices();


    void loadDevice2Host();

//******************************************************************************
protected:
    static void checkCudaError(const char* aFileName,
            const char* aFunctionName,
            unsigned int aLineNumber);

    void loadHost2Device();
    void loadDevice2Device(const float* apImageOnDevice);

    // Make sure there is at least one CUDA device
    static int m_device_count;

    float* m_p_device_memory;

    float m_computing_time;
    float m_host_to_device_transfer;
    float m_device_to_host_transfer;
    float m_device_to_device_transfer;
};


#endif
