/**
********************************************************************************
*
*   @file       Image.cpp
*
*   @brief      Class to handle a greyscale image.
*
*   @version    1.0
*
*   @todo       Write here anything you did not implement.
*
*   @date       11/11/2016
*
*   @author     Franck Vidal
*
*
********************************************************************************
*/


//******************************************************************************
//  Define
//******************************************************************************
#define LINE_SIZE 2048


//******************************************************************************
//  Include
//******************************************************************************
#include <sstream> // Header file for stringstream
#include <fstream> // Header file for filestream
#include <algorithm> // Header file for min/max/fill
#include <numeric> // Header file for accumulate
#include <cmath> // Header file for abs and pow
#include <vector>

#include "Image.h"


//-----------------
Image::Image():
//-----------------
        m_width(0),
        m_height(0)
//-----------------
{}


//----------------------------------
Image::Image(const Image& anImage):
//----------------------------------
        m_width(anImage.m_width),
        m_height(anImage.m_height),
        m_p_image(anImage.m_p_image)
//----------------------------------
{
    // Out of memory
    if (m_width * m_height && m_p_image.empty())
    {
        throw ("Out of memory");
    }
}


//-----------------------------------------------------
Image::Image(const float* apData,
             unsigned int aWidth,
             unsigned int aHeight):
//-----------------------------------------------------
        m_width(aWidth),
        m_height(aHeight),
        m_p_image(std::vector<float>(aWidth * aHeight))
//-----------------------------------------------------
{
    // Out of memory
    if (m_width * m_height && m_p_image.empty())
    {
        throw ("Out of memory");
    }

    // Copy the data
    std::copy(apData, apData + m_width * m_height, m_p_image.begin());
}


//----------------------------------------------
Image::Image(unsigned int aWidth,
             unsigned int aHeight,
             float aDefaultValue):
//----------------------------------------------
        m_width(aWidth),
        m_height(aHeight),
        m_p_image(std::vector<float>(aWidth * aHeight, aDefaultValue))
//----------------------------------------------
{
    // Out of memory
    if (m_width * m_height && m_p_image.empty())
    {
        throw ("Out of memory");
    }
}


//-------------
Image::~Image()
//-------------
{
    // Release memory
    destroy();
}


//-------------------
void Image::destroy()
//-------------------
{
    // Memory has been dynamically allocated
    m_p_image.clear();

    // There is no pixel in the image
    m_width  = 0;
    m_height = 0;
}


//--------------------------------------
float& Image::operator[](unsigned int i)
//--------------------------------------
{
    // The pixel index is not valid
    if (i >= m_width * m_height)
    {
        throw "Invalid pixel coordinate";
    }

    return (m_p_image[i]);
}


//--------------------------------------------------
const float& Image::operator[](unsigned int i) const
//--------------------------------------------------
{
    // The pixel index is not valid
    if (i >= m_width * m_height)
    {
        throw "Invalid pixel coordinate";
    }

    return (m_p_image[i]);
}


//------------------------------------------------------
float& Image::operator()(unsigned int i, unsigned int j)
//------------------------------------------------------
{
    // The pixel index is not valid
    if (i >= m_width || j >= m_height)
    {
        throw "Invalid pixel coordinate";
    }

    // Change the pixel value
    return m_p_image[j * m_width + i];
}


//------------------------------------------------------------------
const float& Image::operator()(unsigned int i, unsigned int j) const
//------------------------------------------------------------------
{
    // The pixel index is not valid
    if (i >= m_width || j >= m_height)
    {
        throw "Invalid pixel coordinate";
    }

    // Change the pixel value
    return m_p_image[j * m_width + i];
}


//----------------------------------------------------------------
void Image::setPixel(unsigned int i, unsigned int j, float aValue)
//----------------------------------------------------------------
{
    // The pixel index is not valid
    if (i >= m_width || j >= m_height)
    {
        throw "Invalid pixel coordinate";
    }

    // Change the pixel value
    m_p_image[j * m_width + i] = aValue;
}


//---------------------------------------------------------
float Image::getPixel(unsigned int i, unsigned int j) const
//---------------------------------------------------------
{
    // The pixel index is valid
    if (i >= m_width || j >= m_height)
    {
        throw "Invalid pixel coordinate";
    }

    // Return the pixel value
    return (m_p_image[j * m_width + i]);
}


//-------------------------------------------
Image& Image::operator=(const Image& anImage)
//-------------------------------------------
{
    // The images different
    if (this != &anImage)
    {
        // Release memory
        destroy();

        // Copy the image properites
        m_width   = anImage.m_width;
        m_height  = anImage.m_height;
        m_p_image = anImage.m_p_image;

        // Out of memory
        if (m_width * m_height && m_p_image.empty())
        {
            throw ("Out of memory");
        }
    }

    // Return the instance
    return (*this);
}


//----------------------------------
unsigned int Image::getWidth() const
//----------------------------------
{
    return (m_width);
}


//-----------------------------------
unsigned int Image::getHeight() const
//-----------------------------------
{
    return (m_height);
}


//------------------------------
float Image::getMinValue() const
//------------------------------
{
    // The image is empty
    if (m_p_image.empty())
    {
        throw "Empty image";
    }

    return (*std::min_element(&m_p_image[0], &m_p_image[m_width * m_height]));
}


//------------------------------
float Image::getMaxValue() const
//------------------------------
{
    // The image is empty
    if (m_p_image.empty())
    {
        throw "Empty image";
    }

    return (*std::max_element(&m_p_image[0], &m_p_image[m_width * m_height]));
}


//-------------------------
float Image::getSum() const
//-------------------------
{
    return (std::accumulate(m_p_image.begin(), m_p_image.end(), 0.0f));
}


//-----------------------------
float Image::getAverage() const
//-----------------------------
{
    return (getSum() / (m_width * m_height));
}


//--------------------------
float Image::getMean() const
//--------------------------
{
    return (getAverage());
}


//------------------------------
float Image::getVariance() const
//------------------------------
{
    float mean = getAverage();

    float sum = 0.0;
    for (std::vector<float>::const_iterator ite = m_p_image.begin();
            ite != m_p_image.end();
            ++ite)
    {
        sum += (*ite - mean) * (*ite - mean);
    }

    return (sum / (m_width * m_height));
}


//----------------------------
float Image::getStdDev() const
//----------------------------
{
    return (std::sqrt(getVariance()));
}


//----------------------------------------
void Image::loadPGM(const char* aFileName)
//----------------------------------------
{
    // Open the file
    std::ifstream input_file(aFileName, std::ifstream::binary);

    // The file does not exist
    if (!input_file.is_open())
    {
        // Build the error message
        std::stringstream error_message;
        error_message << "Cannot open the file \"" << aFileName << "\". It does not exist";

        // Throw an error
        throw (error_message.str());
    }
    // The file is open
    else
    {
        // Release the memory if necessary
        destroy();

        // Variable to store a line
        char p_line_data[LINE_SIZE];

        // Get the first line
        input_file.getline(p_line_data, LINE_SIZE);

        // Get the image type
        std::string image_type(p_line_data);

        // Valid ASCII format
        if (image_type == "P2")
        {
            // Variable to save the max value
            int max_value(-1);

            // There is data to read
            unsigned int pixel_count(0);
            while (input_file.good())
            {
                // Get the new line
                input_file.getline(p_line_data, LINE_SIZE);

                // It is not a comment
                if (p_line_data[0] != '#')
                {
                    // Store the line in a stream
                    std::stringstream stream_line;
                    stream_line << std::string(p_line_data);

                    // The memory is not allocated
                    if (m_p_image.empty() && !m_width && !m_height)
                    {
                        // Load the image size
                        stream_line >> m_width >> m_height;

                        // Alocate the memory
                        m_p_image = std::vector<float>(m_width * m_height);

                        // Out of memory
                        if (m_width * m_height && m_p_image.empty())
                        {
                            throw ("Out of memory");
                        }
                    }
                    // The max value is not set
                    else if (max_value < 0)
                    {
                        // Get the max value;
                        stream_line >> max_value;
                    }
                    // Read the pixel data
                    else
                    {
                        // Process all the pixels of the line
                        while (stream_line.good())
                        {
                            // Get the pixel value
                            int pixel_value(-1);
                            stream_line >> pixel_value;
                            // The pixel exists
                            if (pixel_count < m_width * m_height)
                            {
                                m_p_image[pixel_count++] = pixel_value;
                            }
                        }
                    }
                }
            }
        }
        // Valid binary format
        else if (image_type == "P5")
        {
            // Variable to save the max value
            int max_value(-1);

            // There is data to read
            unsigned int pixel_count(0);
            while (input_file.good() && !pixel_count)
            {
                // Process as an ASCII file
                if (!m_width || !m_height || max_value < 0)
                {
                    // Get the new line
                    input_file.getline(p_line_data, LINE_SIZE);

                    // It is not a comment
                    if (p_line_data[0] != '#')
                    {
                        // Store the line in a stream
                        std::stringstream stream_line;
                        stream_line << std::string(p_line_data);

                        // The memory is not allocated
                        if (m_p_image.empty() && !m_width && !m_height)
                        {
                            // Load the image size
                            stream_line >> m_width >> m_height;

                            // Alocate the memory
                            m_p_image = std::vector<float>(m_width * m_height);

                            // Out of memory
                            if (m_width * m_height && m_p_image.empty())
                            {
                                throw ("Out of memory");
                            }
                        }
                        // The max value is not set
                        else
                        {
                            // Get the max value;
                            stream_line >> max_value;
                        }
                    }
                }
                // Read the pixel data
                else
                {
                    unsigned char* p_temp(new unsigned char[m_width * m_height]);

                    // Out of memory
                    if (!p_temp)
                    {
                        throw ("Out of memory");
                    }

                    input_file.read(reinterpret_cast<char*>(p_temp), m_width * m_height);

                    for (unsigned int i(0); i < m_width * m_height; ++i)
                    {
                        m_p_image[i] = p_temp[i];
                        ++pixel_count;
                    }
                    delete [] p_temp;
                }
            }
        }
        // Invalid format
        else
        {
            // Build the error message
            std::stringstream error_message;
            error_message << "Invalid file (\"" << aFileName << "\")";

            // Throw an error
            throw (error_message.str());
        }
    }
}


//-----------------------------------------------
void Image::loadPGM(const std::string& aFileName)
//-----------------------------------------------
{
    loadPGM(aFileName.data());
}


//----------------------------------------
void Image::savePGM(const char* aFileName)
//----------------------------------------
{
    // Open the file
    std::ofstream output_file(aFileName);

    // The file does not exist
    if (!output_file.is_open())
    {
        // Build the error message
        std::stringstream error_message;
        error_message << "Cannot create the file \"" << aFileName << "\"";

        // Throw an error
        throw (error_message.str());
    }
    // The file is open
    else
    {
        // Set the image type
        output_file << "P2" << std::endl;

        // Print a comment
        output_file << "# ICP3038 -- Assignment 1 -- 2016/2017" << std::endl;

        // The image size
        output_file << m_width << " " << m_height << std::endl;

        // The get the max value
        //output_file << std::min(255, std::max(0, int(getMaxValue()))) << std::endl;
        output_file << std::max(255, int(getMaxValue())) << std::endl;

        // Process every line
        for (unsigned int j = 0; j < m_height; ++j)
        {
            // Process every column
            for (unsigned int i = 0; i < m_width; ++i)
            {
                // Process the pixel
                int pixel_value(m_p_image[j * m_width + i]);
                pixel_value = std::max(0, pixel_value);
                pixel_value = std::min(255, pixel_value);

                output_file << pixel_value;

                // It is not the last pixel of the line
                if (i < (m_width - 1))
                {
                    output_file << " ";
                }
            }

            // It is not the last line of the image
            if (j < (m_height - 1))
            {
                output_file << std::endl;
            }
        }
    }
}


//-----------------------------------------------
void Image::savePGM(const std::string& aFileName)
//-----------------------------------------------
{
    savePGM(aFileName.data());
}


//------------------------------------------
void Image::loadASCII(const char* aFileName)
//------------------------------------------
{
    // Release the memory
    destroy();

    // Open the file
    std::ifstream input_file (aFileName);

    // The file is not open
    if (!input_file.is_open())
    {
        std::string error_message("The file (");
        error_message += aFileName;
        error_message += ") does not exist";

        throw error_message;
    }

    // Load the data into a vector
    std::string line;
    int number_of_rows(0);
    int number_of_columns(0);

    // Read evely line
    while (std::getline(input_file, line))
    {
        number_of_columns = 0;
        float intensity;
        std::stringstream line_parser;
        line_parser << line;
        while (line_parser >> intensity)
        {
            m_p_image.push_back(intensity);
            ++number_of_columns;
        }
        ++number_of_rows;
    }

    // Wrong number of pixels
    if (number_of_rows * number_of_columns != m_p_image.size())
    {
        std::string error_message("The file (");
        error_message += aFileName;
        error_message += ") is invalid";

        throw error_message;
    }

    // Allocate memory for file content
    m_width = number_of_columns;
    m_height = number_of_rows;
}


//-------------------------------------------------
void Image::loadASCII(const std::string& aFileName)
//-------------------------------------------------
{
    loadASCII(aFileName.data());
}


//------------------------------------------
void Image::saveASCII(const char* aFileName)
//------------------------------------------
{
    // Open the file
    std::ofstream output_file (aFileName);

    // The file is not open
    if (!output_file.is_open())
    {
        std::string error_message("The file (");
        error_message += aFileName;
        error_message += ") cannot be created";

        throw error_message;
    }

    // Write content to file
    float* p_data(&m_p_image[0]);
    for (unsigned int j(0); j < m_height; ++j)
    {
        for (unsigned int i(0); i < m_width; ++i)
        {
            output_file << *p_data++;

            // This is not the last pixel of the line
            if (i < m_width - 1)
            {
                output_file << " ";
            }
        }

        // This is not the last line
        if (j < m_height - 1)
        {
            output_file << std::endl;
        }
    }
}


//-------------------------------------------------
void Image::saveASCII(const std::string& aFileName)
//-------------------------------------------------
{
    saveASCII(aFileName.data());
}


//------------------------------------------------
bool Image::operator==(const Image& anImage) const
//------------------------------------------------
{
    if (m_width != anImage.m_width)
    {
        return (false);
    }

    if (m_height != anImage.m_height)
    {
        return (false);
    }

    std::vector<float>::const_iterator ite1 = m_p_image.begin();
    std::vector<float>::const_iterator ite2 = anImage.m_p_image.begin();

    for (unsigned int i(0); i < m_width * m_height; ++i)
    {
        if (std::abs(*ite1++ - *ite2++) > 1.0e-6)
        {
            return (false);
        }
    }

    return (true);
}


//------------------------------------------------
bool Image::operator!=(const Image& anImage) const
//------------------------------------------------
{
    return (!(operator==(anImage)));
}


//----------------------------
Image Image::operator!() const
//----------------------------
{
    // Copy the instance into a temporary variable
    Image temp(*this);

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


//-----------------------------------------------------------------------
Image Image::shiftScaleFilter(float aShiftValue, float aScaleValue) const
//-----------------------------------------------------------------------
{
    // Create an image of the right size
    Image temp(getWidth(), getHeight());

    // Process every pixel of the image
    for (unsigned int i = 0; i < m_width * m_height; ++i)
    {
        // Apply the shilft/scale filter
        temp[i] = (m_p_image[i] + aShiftValue) * aScaleValue;
    }

    return temp;
}


//--------------------------------
Image Image::getNormalised() const
//--------------------------------
{
    return shiftScaleFilter(-getMinValue(), 1.0 / (getMaxValue() - getMinValue()));
}


//--------------------------------
Image Image::getNormalized() const
//--------------------------------
{
    return (getNormalised());
}


//----------------------------
Image Image::logFilter() const
//----------------------------
{
    // Create an image of the right size
    Image temp(getWidth(), getHeight());

    // Process every pixel of the image
    for (unsigned int i = 0; i < m_width * m_height; ++i)
    {
        // Apply the log filter
        temp[i] = log(m_p_image[i]);
    }

    return temp;
}


//-----------------------------------
Image Image::flipHorizontally() const
//-----------------------------------
{
    // Create an image of the right size
    Image temp(getWidth(), getHeight());

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


//---------------------------------
Image Image::flipVertically() const
//---------------------------------
{
    // Create an image of the right size
    Image temp(getWidth(), getHeight());

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
