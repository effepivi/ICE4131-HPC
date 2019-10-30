/**
********************************************************************************
*
*   @file       OpenMPImage.cxx
*
*   @brief      Class to handle a greyscale image using OpenMP to speedup computations.
*
*   @version    1.0
*
*   @todo       Write here anything you did not implement.
*
*   @date       30/10/2019
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
#include <omp.h> // Header file for OpenMP

#include "OpenMPImage.h"


//--------------------------------------------------------
OpenMPImage::OpenMPImage(unsigned int aNumberOfThreads):
//--------------------------------------------------------
        Image(),
        m_thread_number(aNumberOfThreads)
//--------------------------------------------------------
{}


//--------------------------------------------------------
OpenMPImage::OpenMPImage(const Image& anImage,
                           unsigned int aNumberOfThreads):
//--------------------------------------------------------
		Image(anImage),
        m_thread_number(aNumberOfThreads)
//--------------------------------------------------------
{}


//------------------------------------------------------
OpenMPImage::OpenMPImage(const OpenMPImage& anImage):
//------------------------------------------------------
		Image(anImage),
        m_thread_number(anImage.m_thread_number)
//------------------------------------------------------
{}


//-------------------------------------------------------
OpenMPImage::OpenMPImage(const float* apData,
                           unsigned int aWidth,
                           unsigned int aHeight,
                           unsigned int aNumberOfThreads):
//-------------------------------------------------------
        Image(apData, aWidth, aHeight),
        m_thread_number(aNumberOfThreads)
//-------------------------------------------------------
{}


    //--------------------------------------------------------
OpenMPImage::OpenMPImage(unsigned int aWidth,
                           unsigned int aHeight,
                           float aDefaultValue,
                           unsigned int aNumberOfThreads):
//--------------------------------------------------------
        Image(aWidth, aHeight, aDefaultValue),
        m_thread_number(aNumberOfThreads)
//--------------------------------------------------------
{}


//---------------------------
OpenMPImage::~OpenMPImage()
//---------------------------
{}


//------------------------------------------------------------------
void OpenMPImage::setNumberOfThreads(unsigned int aNumberOfThreads)
//------------------------------------------------------------------
{
    m_thread_number = aNumberOfThreads;
}


//---------------------------------------------------
unsigned int OpenMPImage::getNumberOfThreads() const
//---------------------------------------------------
{
    return m_thread_number;
}


//---------------------------------------------------------
OpenMPImage& OpenMPImage::operator=(const Image& anImage)
//---------------------------------------------------------
{
    Image::operator=(anImage);
    return *this;
}


//----------------------------------------------------------------
OpenMPImage& OpenMPImage::operator=(const OpenMPImage& anImage)
//----------------------------------------------------------------
{
    Image::operator=(anImage);
    m_thread_number = anImage.m_thread_number;
    return *this;
}


//------------------------------------
float OpenMPImage::getMinValue() const
//------------------------------------
{
    float min_value = std::numeric_limits<float>::max();

#pragma omp parallel for reduction(min: min_value)
    for (unsigned int i = 0; i < m_width * m_height; ++i)
    {
        if (min_value > m_p_image[i]) min_value = m_p_image[i];
    }


    return (min_value);
}


//------------------------------------
float OpenMPImage::getMaxValue() const
//------------------------------------
{
    float max_value = std::numeric_limits<float>::min();

#pragma omp parallel for reduction(max: max_value)
    for (unsigned int i = 0; i < m_width * m_height; ++i)
    {
        if (max_value < m_p_image[i]) max_value = m_p_image[i];
    }


    return (max_value);
}


//-------------------------------
float OpenMPImage::getSum() const
//-------------------------------
{
    float sum = 0.0;

#pragma omp parallel for reduction(+: sum)
    for (unsigned int i = 0; i < m_width * m_height; ++i)
    {
        sum += m_p_image[i];
    }

    return (sum);
}


//------------------------------------
float OpenMPImage::getVariance() const
//------------------------------------
{
    float mean = getAverage();
    float sum = 0.0;

#pragma omp parallel for reduction(+: sum)
    for (unsigned int i = 0; i < m_width * m_height; ++i)
    {
        sum += (m_p_image[i] - mean) * (m_p_image[i] - mean);
    }

    return (sum / (m_width * m_height));
}


//------------------------------------------------------
bool OpenMPImage::operator==(const Image& anImage) const
//------------------------------------------------------
{
    if (m_width != anImage.getWidth())
    {
        return (false);
    }

    if (m_height != anImage.getHeight())
    {
        return (false);
    }

    for (unsigned int i = 0; i < m_width * m_height; ++i)
    {
        if (std::abs(m_p_image[i] - anImage[i]) > 1.0e-6)
        {
            return (false);
        }
    }

    return (true);
}


//------------------------------------------
OpenMPImage OpenMPImage::operator!() const
//------------------------------------------
{
    // Copy the instance into a temporary variable
	OpenMPImage temp(*this);

	float min_value(getMinValue());
	float max_value(getMaxValue());
	float range(max_value - min_value);

	// Process every pixel
	for (std::vector<float>::iterator ite = temp.m_p_image.begin();
		ite != temp.m_p_image.end();
		++ite)
	{
		// Take care to preserve the dynamic of the image
		*ite = min_value + range * (1.0 - (*ite - min_value) / range);
	}

	// Return the result
	return (temp);
}


//------------------------------------------------------------------
OpenMPImage OpenMPImage::shiftScaleFilter(float aShiftValue,
                                            float aScaleValue) const
//------------------------------------------------------------------
{
    // Create an image of the right size
    OpenMPImage temp(getWidth(), getHeight(), 0.0, m_thread_number);

    // Process every pixel of the image
	for (unsigned int i = 0; i < m_width * m_height; ++i)
	{
		// Apply the shilft/scale filter
		temp[i] = (m_p_image[i] + aShiftValue) * aScaleValue;
	}

	return temp;
}


//----------------------------------------
OpenMPImage OpenMPImage::logFilter() const
//----------------------------------------
{
	// Create an image of the right size
    OpenMPImage temp(getWidth(), getHeight(), 0.0, m_thread_number);

	// Process every pixel of the image
	for (unsigned int i = 0; i < m_width * m_height; ++i)
	{
		// Apply the log filter
		temp[i] = log(m_p_image[i]);
	}

	return temp;
}


//-----------------------------------------------
OpenMPImage OpenMPImage::flipHorizontally() const
//-----------------------------------------------
{
    // Create an image of the right size
	OpenMPImage temp(getWidth(), getHeight(), 0.0, m_thread_number);

	// Process every row of the image
	for (unsigned int j = 0; j < m_height; ++j)
	{
		// Process every column of the image
		for (unsigned int i = 0; i < m_width / 2; ++i)
		{
			temp(              i, j) = getPixel(m_width - i - 1, j);
			temp(m_width - i - 1, j) = getPixel(              i, j);
		}
	}

	return temp;
}


//---------------------------------------------
OpenMPImage OpenMPImage::flipVertically() const
//---------------------------------------------
{
	// Create an image of the right size
	OpenMPImage temp(getWidth(), getHeight(), 0.0, m_thread_number);

	// Process every row of the image
	for (unsigned int j = 0; j < m_height / 2; ++j)
	{
		// Process every column of the image
		for (unsigned int i = 0; i < m_width; ++i)
		{
			temp(i,                j) = getPixel(i, m_height - j - 1);
			temp(i, m_height - j - 1) = getPixel(i,                j);
		}
	}

	return temp;
}
