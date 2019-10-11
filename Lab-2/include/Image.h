#ifndef __Image_h
#define __Image_h


/**
********************************************************************************
*
*   @file       Image.h
*
*   @brief      Class to handle a greyscale image.
*
*   @version    1.0
*
*   @todo       Write here anything you did not implement.
*
*   @date       08/10/2019
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
#include <string>
#include <vector>


//==============================================================================
/**
*   @class  Image
*   @brief  Image is a class to manage a greyscale image.
*/
//==============================================================================
class Image
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    Image();


    //------------------------------------------------------------------------
    /// Copy constructor.
    /**
    * @param anImage: the image to copy
    */
    //------------------------------------------------------------------------
    Image(const Image& anImage);


    //------------------------------------------------------------------------
    /// Constructor from an array.
    /**
    * @param apData: the array to copy
    * @param aWidth: the width of the image
    * @param aHeight: the height of the image
    */
    //------------------------------------------------------------------------
    Image(const float* apData,
                 unsigned int aWidth,
                 unsigned int aHeight);


    //------------------------------------------------------------------------
    /// Constructor to build a black image.
    /**
    * @param aWidth: the width of the image
    * @param aHeight: the height of the image
    */
    //------------------------------------------------------------------------
    Image(unsigned int aWidth,
                 unsigned int aHeight,
                float aDefaultValue = 0.0);


    //------------------------------------------------------------------------
    /// Destructor.
    //------------------------------------------------------------------------
    ~Image();


    //------------------------------------------------------------------------
    /// Assignment operator (also called copy operator).
    /**
    * @param anImage: the image to copy
    * @return the updated version of the current image
    */
    //------------------------------------------------------------------------
    Image& operator=(const Image& anImage);


    //------------------------------------------------------------------------
    /// Release the memory.
    //------------------------------------------------------------------------
    void destroy();


	float& operator[](unsigned int i);
	const float& operator[](unsigned int i) const;


	float& operator()(unsigned int i, unsigned int j);
	const float& operator()(unsigned int i, unsigned int j) const;

    //------------------------------------------------------------------------
    /// Set a pixel.
    /**
    * @param i: the position of the pixel along the horizontal axis
    * @param j: the position of the pixel along the vertical axis
    * @param aValue: the new pixel value
    */
    //------------------------------------------------------------------------
    void setPixel(unsigned int i, unsigned int j, float aValue);


    //------------------------------------------------------------------------
    /// Accessor on a pixel value.
    /**
    * @param i: the position of the pixel along the horizontal axis
    * @param j: the position of the pixel along the vertical axis
    * @return the pixel value
    */
    //------------------------------------------------------------------------
    float getPixel(unsigned int i, unsigned int j) const;


    //------------------------------------------------------------------------
    /// Number of pixels along the horizontal axis
    /**
    * @return the width
    */
    //------------------------------------------------------------------------
    unsigned int getWidth() const;


    //------------------------------------------------------------------------
    /// Number of pixels along the vertical axis
    /**
    * @return the height
    */
    //------------------------------------------------------------------------
    unsigned int getHeight() const;


    //------------------------------------------------------------------------
    /// Compute the minimum pixel value in the image
    /**
    * @return the minimum pixel
    */
    //------------------------------------------------------------------------
    float getMinValue() const;


    //------------------------------------------------------------------------
    /// Compute the maximum pixel value in the image
    /**
    * @return the maximum pixel
    */
    //------------------------------------------------------------------------
    float getMaxValue() const;


    //------------------------------------------------------------------------
    /// Compute the sum of all the pixel values of the image
    /**
    * @return the sum of all the pixel values of the image
    */
    //------------------------------------------------------------------------
    float getSum() const;


    //------------------------------------------------------------------------
    /// Compute the average value of all the pixels of the image
    /**
    * @return the average value of all the pixels of the image
    */
    //------------------------------------------------------------------------
    float getAverage() const;


    //------------------------------------------------------------------------
    /// Compute the average value of all the pixels of the image
    /**
    * @return the average value of all the pixels of the image
    */
    //------------------------------------------------------------------------
    float getMean() const;


    //------------------------------------------------------------------------
    /// Compute the variance of the pixel values of the image
    /**
    * @return the variance of the pixel values of the image
    */
    //------------------------------------------------------------------------
    float getVariance() const;


    //------------------------------------------------------------------------
    /// Compute the standard deviation of the pixel values of the image
    /**
    * @return the standard deviation of the pixel values of the image
    */
    //------------------------------------------------------------------------
    float getStdDev() const;


    //------------------------------------------------------------------------
    /// Load an image from a PGM file
    /**
    * @param aFileName: the name of the file to load
    */
    //------------------------------------------------------------------------
    void loadPGM(const char* aFileName);


    //------------------------------------------------------------------------
    /// Load an image from a PGM file
    /**
    * @param aFileName: the name of the file to load
    */
    //------------------------------------------------------------------------
    void loadPGM(const std::string& aFileName);


    //------------------------------------------------------------------------
    /// Save the image in a PGM file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void savePGM(const char* aFileName);


    //------------------------------------------------------------------------
    /// Save the image in a PGM file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void savePGM(const std::string& aFileName);


    //------------------------------------------------------------------------
    /// Load an image from an ASCII file
    /**
    * @param aFileName: the name of the file to load
    */
    //------------------------------------------------------------------------
    void loadASCII(const char* aFileName);


    //------------------------------------------------------------------------
    /// Load an image from an ASCII file
    /**
    * @param aFileName: the name of the file to load
    */
    //------------------------------------------------------------------------
    void loadASCII(const std::string& aFileName);


    //------------------------------------------------------------------------
    /// Save the image in an ASCII file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void saveASCII(const char* aFileName);


    //------------------------------------------------------------------------
    /// Save the image in an ASCII file
    /**
    * @param aFileName: the name of the file to write
    */
    //------------------------------------------------------------------------
    void saveASCII(const std::string& aFileName);


    //------------------------------------------------------------------------
    /// Operator Equal to
    /**
    * @param anImage: the image to compare with
    * @return true if the images are similar,
    *         false if they are different
    */
    //------------------------------------------------------------------------
    bool operator==(const Image& anImage) const;


    //------------------------------------------------------------------------
    /// Operator Not equal to
    /**
    * @param anImage: the image to compare with
    * @return true if the images are different,
    *         false if they are similar
    */
    //------------------------------------------------------------------------
	bool operator!=(const Image& anImage) const;


	//------------------------------------------------------------------------
	/// Negation operator. Compute the negative of the current image.
	/**
	* @return the negative image
	*/
	//------------------------------------------------------------------------
	Image operator!() const;


	//------------------------------------------------------------------------
	/// Add aShiftValue to every pixel, then multiply every pixel
	/// by aScaleValue
	/**
	* @param aShiftValue: the shift parameter of the filter
	* @param aScaleValue: the scale parameter of the filter
	*/
	//------------------------------------------------------------------------
	Image shiftScaleFilter(float aShiftValue, float aScaleValue) const;


	//------------------------------------------------------------------------
	/// Normalise the image between 0 and 1
	//------------------------------------------------------------------------
	Image getNormalised() const;


	//------------------------------------------------------------------------
	/// Normalize the image between 0 and 1
	//------------------------------------------------------------------------
	Image getNormalized() const;


	Image logFilter() const;


	Image flipHorizontally() const;


	Image flipVertically() const;


//******************************************************************************
private:
    /// Number of pixel along the horizontal axis
    unsigned int m_width;


    /// Number of pixel along the vertical axis
    unsigned int m_height;


    /// The pixel data
    std::vector<float> m_p_image;
};

#endif
