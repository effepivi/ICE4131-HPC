#ifndef __MPIImage_h
#define __MPIImage_h


/**
********************************************************************************
*
*   @file       MPIImage.h
*
*   @brief      Class to handle a greyscale image using MPI to speedup computations.
*
*   @version    1.0
*
*   @todo       Write here anything you did not implement.
*
*   @date       31/10/2019
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
*   @class  MPIImage
*   @brief  MPIImage is a class to manage a greyscale image using MPI
*           to speedup computations
*/
//==============================================================================
class MPIImage: public Image
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    MPIImage();


    //------------------------------------------------------------------------
    /// Copy constructor.
    /**
    * @param anImage: the image to copy
    */
    //------------------------------------------------------------------------
    MPIImage(const Image& anImage);


    //------------------------------------------------------------------------
    /// Constructor from an array.
    /**
    * @param apData: the array to copy
    * @param aWidth: the width of the image
    * @param aHeight: the height of the image
    */
    //------------------------------------------------------------------------
    MPIImage(const float* apData,
                 unsigned int aWidth,
                 unsigned int aHeight);


    //------------------------------------------------------------------------
    /// Constructor to build a black image.
    /**
    * @param aWidth: the width of the image
    * @param aHeight: the height of the image
    */
    //------------------------------------------------------------------------
    MPIImage(unsigned int aWidth,
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
    MPIImage operator!() const;


    //------------------------------------------------------------------------
    /// Add aShiftValue to every pixel, then multiply every pixel
    /// by aScaleValue
    /**
    * @param aShiftValue: the shift parameter of the filter
    * @param aScaleValue: the scale parameter of the filter
    * @return the new image
    */
    //------------------------------------------------------------------------
    MPIImage shiftScaleFilter(float aShiftValue, float aScaleValue) const;


    //------------------------------------------------------------------------
    /// Apply a log filter on the image
    /**
    * @return the new image
    */
    //------------------------------------------------------------------------
    MPIImage logFilter() const;


    //------------------------------------------------------------------------
    /// Flip the image horizonatally
    /**
    * @return the new image
    */
    //------------------------------------------------------------------------
    MPIImage flipHorizontally() const;


    //------------------------------------------------------------------------
    /// Flip the image vertically
    /**
    * @return the new image
    */
    //------------------------------------------------------------------------
    MPIImage flipVertically() const;


    static const int ROOT = 0;


//******************************************************************************
protected:
    void workload(unsigned int aNumberOfElements,
            unsigned int& aStartID,
            unsigned int& aEndID) const;

    void checkMPIError(int errorCode) const;
};


#endif
