/**
********************************************************************************
*
*   @file       PthreadImage.cxx
*
*   @brief      Class to handle a greyscale image using Pthread to speedup computations.
*
*   @version    1.0
*
*   @todo       Write here anything you did not implement.
*
*   @date       17/10/2019
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
#include <pthread.h> // Header file for Pthreads

#include "PthreadImage.h"


//--------------------------------------------------------
PthreadImage::PthreadImage(unsigned int aNumberOfThreads):
//--------------------------------------------------------
        Image(),
        m_thread_number(aNumberOfThreads)
//--------------------------------------------------------
{}


//--------------------------------------------------------
PthreadImage::PthreadImage(const Image& anImage,
                           unsigned int aNumberOfThreads):
//--------------------------------------------------------
		Image(anImage),
        m_thread_number(aNumberOfThreads)
//--------------------------------------------------------
{}


//------------------------------------------------------
PthreadImage::PthreadImage(const PthreadImage& anImage):
//------------------------------------------------------
		Image(anImage),
        m_thread_number(anImage.m_thread_number)
//------------------------------------------------------
{}


//-------------------------------------------------------
PthreadImage::PthreadImage(const float* apData,
                           unsigned int aWidth,
                           unsigned int aHeight,
                           unsigned int aNumberOfThreads):
//-------------------------------------------------------
        Image(apData, aWidth, aHeight),
        m_thread_number(aNumberOfThreads)
//-------------------------------------------------------
{}


    //--------------------------------------------------------
PthreadImage::PthreadImage(unsigned int aWidth,
                           unsigned int aHeight,
                           float aDefaultValue,
                           unsigned int aNumberOfThreads):
//--------------------------------------------------------
        Image(aWidth, aHeight, aDefaultValue),
        m_thread_number(aNumberOfThreads)
//--------------------------------------------------------
{}


//---------------------------
PthreadImage::~PthreadImage()
//---------------------------
{}


//------------------------------------------------------------------
void PthreadImage::setNumberOfThreads(unsigned int aNumberOfThreads)
//------------------------------------------------------------------
{
    m_thread_number = aNumberOfThreads;
}


//---------------------------------------------------
unsigned int PthreadImage::getNumberOfThreads() const
//---------------------------------------------------
{
    return m_thread_number;
}


//---------------------------------------------------------
PthreadImage& PthreadImage::operator=(const Image& anImage)
//---------------------------------------------------------
{
    Image::operator=(anImage);
    return *this;
}


//----------------------------------------------------------------
PthreadImage& PthreadImage::operator=(const PthreadImage& anImage)
//----------------------------------------------------------------
{
    Image::operator=(anImage);
    m_thread_number = anImage.m_thread_number;
    return *this;
}


//-------------------------------------------------------
bool PthreadImage::operator==(const Image& anImage) const
//-------------------------------------------------------
{
    //if (m_thread_number == 0 || m_thread_number == 1)
    {
        return Image::operator==(anImage);
    }
    /*else
    {
        if (m_width != anImage.getWidth())
        {
            return (false);
        }

        if (m_height != anImage.getHeight())
        {
            return (false);
        }

        // Add your code here




        return true;
    }*/
}


//------------------------------------------
PthreadImage PthreadImage::operator!() const
//------------------------------------------
{
    if (m_thread_number == 0 || m_thread_number == 1)
    {
        return PthreadImage(Image::operator!(), m_thread_number);
    }
    else
    {
        // Create an image of the right size
        PthreadImage temp(getWidth(), getHeight(), 0.0, m_thread_number);

        // Add your code here




        return temp;
    }
}


//------------------------------------------------------------------
PthreadImage PthreadImage::shiftScaleFilter(float aShiftValue,
                                            float aScaleValue) const
//------------------------------------------------------------------
{
    if (m_thread_number == 0 || m_thread_number == 1)
    {
        return PthreadImage(Image::shiftScaleFilter(aShiftValue, aScaleValue), m_thread_number);
    }
    else
    {
        // Create an image of the right size
        PthreadImage temp(getWidth(), getHeight(), 0.0, m_thread_number);

        // Add your code here




        return temp;
    }
}


//------------------------------------------
PthreadImage PthreadImage::logFilter() const
//------------------------------------------
{
    if (m_thread_number == 0 || m_thread_number == 1)
    {
        return PthreadImage(Image::logFilter(), m_thread_number);
    }
    else
    {
        // Create an image of the right size
        PthreadImage temp(getWidth(), getHeight(), 0.0, m_thread_number);

        // Add your code here




        return temp;
    }
}


//-------------------------------------------------
PthreadImage PthreadImage::flipHorizontally() const
//-------------------------------------------------
{
    if (m_thread_number == 0 || m_thread_number == 1)
    {
        return PthreadImage(Image::flipHorizontally(), m_thread_number);
    }
    else
    {
        // Create an image of the right size
        PthreadImage temp(getWidth(), getHeight(), 0.0, m_thread_number);

        // Add your code here




        return temp;
    }
}


//-----------------------------------------------
PthreadImage PthreadImage::flipVertically() const
//-----------------------------------------------
{
    if (m_thread_number == 0 || m_thread_number == 1)
    {
        return PthreadImage(Image::flipVertically(), m_thread_number);
    }
    else
    {
        // Create an image of the right size
        PthreadImage temp(getWidth(), getHeight(), 0.0, m_thread_number);

        // Add your code here




        return temp;
    }
}
