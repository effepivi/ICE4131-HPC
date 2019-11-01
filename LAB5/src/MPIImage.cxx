/**
********************************************************************************
*
*   @file       MPIImage.cxx
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
#include <cmath> // Header file for abs and log
#include <limits>
#include <mpi.h> // Header file for MPI

#include "MPIImage.h"


//-------------------
MPIImage::MPIImage():
//-------------------
        Image()
//-------------------
{}


//---------------------------------------
MPIImage::MPIImage(const Image& anImage):
//---------------------------------------
        Image(anImage)
//---------------------------------------
{}


//---------------------------------------
MPIImage::MPIImage(const float* apData,
                   unsigned int aWidth,
                   unsigned int aHeight):
//---------------------------------------
        Image(apData, aWidth, aHeight)
//---------------------------------------
{}


//-------------------------------------------
MPIImage::MPIImage(unsigned int aWidth,
                   unsigned int aHeight,
                   float aDefaultValue):
//-------------------------------------------
        Image(aWidth, aHeight, aDefaultValue)
//-------------------------------------------
{}


//-------------------------------------------
void MPIImage::savePGM(const char* aFileName)
//-------------------------------------------
{
    // Get the process' rank
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Only the master is allowed to save
    if (rank == ROOT)
    {
        Image::savePGM(aFileName);
    }
}


//---------------------------------------------
void MPIImage::saveASCII(const char* aFileName)
//---------------------------------------------
{
    // Get the process' rank
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Only the master is allowed to save
    if (rank == ROOT)
    {
        Image::saveASCII(aFileName);
    }
}


//----------------------------------
MPIImage MPIImage::operator!() const
//----------------------------------
{
    // Create an image of the right size
    MPIImage temp(getWidth(), getHeight(), 0.0);

    float min_value(getMinValue());
    float max_value(getMaxValue());
    float range(max_value - min_value);

    // Get the work load
    unsigned int pixel_start_id = 0;
    unsigned int pixel_end_id = 0;
    workload(m_width * m_height, pixel_start_id, pixel_end_id);

    // Process every pixel of the sub-image
    for (unsigned int i = pixel_start_id; i <= pixel_end_id; ++i)
    {
        // Take care to preserve the dynamic of the image
        temp[i] = min_value + range * (1.0 - (m_p_image[i] - min_value) / range);
    }

    // Get the process' rank
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Master gather results from all the processes
    if (rank == ROOT)
    {
        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        for (int i = 1; i < world_size; ++i)
        {
            int pixel_start_id;
            int pixel_end_id;

            MPI_Status status;

            checkMPIError(MPI_Recv(&pixel_start_id, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status));
            checkMPIError(MPI_Recv(&pixel_end_id, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status));
            checkMPIError(MPI_Recv(&temp[pixel_start_id], pixel_end_id - pixel_start_id + 1, MPI_FLOAT, i, 2, MPI_COMM_WORLD, &status));
        }
    }
    // Other processes send the data to the master
    else
    {
        checkMPIError(MPI_Send(&pixel_start_id, 1, MPI_INT, ROOT, 0, MPI_COMM_WORLD));
        checkMPIError(MPI_Send(&pixel_end_id,   1, MPI_INT, ROOT, 1, MPI_COMM_WORLD));
        checkMPIError(MPI_Send(&temp[pixel_start_id], pixel_end_id - pixel_start_id + 1, MPI_FLOAT, ROOT, 2, MPI_COMM_WORLD));
    }

    return temp;
}


//----------------------------------------------------------
MPIImage MPIImage::shiftScaleFilter(float aShiftValue,
                                    float aScaleValue) const
//----------------------------------------------------------
{
    // Create an image of the right size
    MPIImage temp(getWidth(), getHeight(), 0.0);

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
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Master gather results from all the processes
    if (rank == ROOT)
    {
        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        for (int i = 1; i < world_size; ++i)
        {
            int pixel_start_id;
            int pixel_end_id;

            MPI_Status status;

            checkMPIError(MPI_Recv(&pixel_start_id, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status));
            checkMPIError(MPI_Recv(&pixel_end_id, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status));
            checkMPIError(MPI_Recv(&temp[pixel_start_id], pixel_end_id - pixel_start_id + 1, MPI_FLOAT, i, 2, MPI_COMM_WORLD, &status));
        }
    }
    // Other processes send the data to the master
    else
    {

        checkMPIError(MPI_Send(&pixel_start_id, 1, MPI_INT, ROOT, 0, MPI_COMM_WORLD));
        checkMPIError(MPI_Send(&pixel_end_id,   1, MPI_INT, ROOT, 1, MPI_COMM_WORLD));
        checkMPIError(MPI_Send(&temp[pixel_start_id], pixel_end_id - pixel_start_id + 1, MPI_FLOAT, ROOT, 2, MPI_COMM_WORLD));
    }

    return temp;
}


//----------------------------------
MPIImage MPIImage::logFilter() const
//----------------------------------
{
    // Create an image of the right size
    MPIImage temp(getWidth(), getHeight(), 0.0);

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
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Master gather results from all the processes
    if (rank == ROOT)
    {
        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        for (int i = 1; i < world_size; ++i)
        {
            int pixel_start_id;
            int pixel_end_id;

            MPI_Status status;

            checkMPIError(MPI_Recv(&pixel_start_id, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status));
            checkMPIError(MPI_Recv(&pixel_end_id, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status));
            checkMPIError(MPI_Recv(&temp[pixel_start_id], pixel_end_id - pixel_start_id + 1, MPI_FLOAT, i, 2, MPI_COMM_WORLD, &status));
        }
    }
    // Other processes send the data to the master
    else
    {
        checkMPIError(MPI_Send(&pixel_start_id, 1, MPI_INT, ROOT, 0, MPI_COMM_WORLD));
        checkMPIError(MPI_Send(&pixel_end_id,   1, MPI_INT, ROOT, 1, MPI_COMM_WORLD));
        checkMPIError(MPI_Send(&temp[pixel_start_id], pixel_end_id - pixel_start_id + 1, MPI_FLOAT, ROOT, 2, MPI_COMM_WORLD));
    }

    return temp;
}


//-----------------------------------------
MPIImage MPIImage::flipHorizontally() const
//-----------------------------------------
{
    // Create an image of the right size
    MPIImage temp(getWidth(), getHeight(), 0.0);

    // Get the work load
    unsigned int row_start_id = 0;
    unsigned int row_end_id = 0;
    workload(m_height, row_start_id, row_end_id);

    // Process every row of the sub-image
    for (unsigned int j = row_start_id; j <= row_end_id; ++j)
    {
        // Process every column of the image
        for (unsigned int i = 0; i < m_width / 2; ++i)
        {
            temp(              i, j) = getPixel(m_width - i - 1, j);
            temp(m_width - i - 1, j) = getPixel(              i, j);
        }
    }

    // Get the process' rank
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Master gather results from all the processes
    if (rank == ROOT)
    {
        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        for (int i = 1; i < world_size; ++i)
        {
            int pixel_start_id;
            int pixel_end_id;

            MPI_Status status;

            checkMPIError(MPI_Recv(&pixel_start_id, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status));
            checkMPIError(MPI_Recv(&pixel_end_id, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status));
            checkMPIError(MPI_Recv(&temp[pixel_start_id], pixel_end_id - pixel_start_id + getWidth(), MPI_FLOAT, i, 2, MPI_COMM_WORLD, &status));
        }
    }
    // Other processes send the data to the master
    else
    {
        int pixel_start_id = row_start_id * getWidth();
        int pixel_end_id   = row_end_id   * getWidth();

        checkMPIError(MPI_Send(&pixel_start_id, 1, MPI_INT, ROOT, 0, MPI_COMM_WORLD));
        checkMPIError(MPI_Send(&pixel_end_id,   1, MPI_INT, ROOT, 1, MPI_COMM_WORLD));
        checkMPIError(MPI_Send(&temp[pixel_start_id], pixel_end_id - pixel_start_id + getWidth(), MPI_FLOAT, ROOT, 2, MPI_COMM_WORLD));
    }

    return temp;
}


//---------------------------------------
MPIImage MPIImage::flipVertically() const
//---------------------------------------
{
    // Create an image of the right size
    MPIImage temp(getWidth(), getHeight(), 0.0);

    // Get the work load
    unsigned int row_start_id = 0;
    unsigned int row_end_id = 0;
    workload(m_height / 2, row_start_id, row_end_id);

    // Process every row of the sub-image
    for (unsigned int j = row_start_id; j <= row_end_id; ++j)
    {
        // Process every column of the image
        for (unsigned int i = 0; i < m_width; ++i)
        {
            temp(i,                j) = getPixel(i, m_height - j - 1);
            temp(i, m_height - j - 1) = getPixel(i,                j);
        }
    }

    // Get the process' rank
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Master gather results from all the processes
    if (rank == ROOT)
    {
        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        for (int i = 1; i < world_size; ++i)
        {
            int pixel_start_id;
            int pixel_end_id;

            MPI_Status status;

            checkMPIError(MPI_Recv(&pixel_start_id, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status));
            checkMPIError(MPI_Recv(&pixel_end_id, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status));
            checkMPIError(MPI_Recv(&temp[pixel_start_id], pixel_end_id - pixel_start_id + getWidth(), MPI_FLOAT, i, 2, MPI_COMM_WORLD, &status));

            checkMPIError(MPI_Recv(&pixel_start_id, 1, MPI_INT, i, 3, MPI_COMM_WORLD, &status));
            checkMPIError(MPI_Recv(&pixel_end_id, 1, MPI_INT, i, 4, MPI_COMM_WORLD, &status));
            checkMPIError(MPI_Recv(&temp[pixel_start_id], pixel_end_id - pixel_start_id + getWidth(), MPI_FLOAT, i, 5, MPI_COMM_WORLD, &status));
        }

        // Don't forget the middle row
	if (m_height % 2)
        {
            // Process every column of the image
            for (unsigned int i = 0; i < m_width; ++i)
            {
                temp(i, m_height / 2) = getPixel(i, m_height / 2);
            }
        }
    }
    // Other processes send the data to the master
    else
    {
        int pixel_start_id = row_start_id * getWidth();
        int pixel_end_id   = row_end_id   * getWidth();

        checkMPIError(MPI_Send(&pixel_start_id, 1, MPI_INT, ROOT, 0, MPI_COMM_WORLD));
        checkMPIError(MPI_Send(&pixel_end_id,   1, MPI_INT, ROOT, 1, MPI_COMM_WORLD));
        checkMPIError(MPI_Send(&temp[pixel_start_id], pixel_end_id - pixel_start_id + getWidth(), MPI_FLOAT, ROOT, 2, MPI_COMM_WORLD));

        pixel_start_id = (m_height - row_end_id   - 1) * getWidth();
        pixel_end_id   = (m_height - row_start_id - 1) * getWidth();

        checkMPIError(MPI_Send(&pixel_start_id, 1, MPI_INT, ROOT, 3, MPI_COMM_WORLD));
        checkMPIError(MPI_Send(&pixel_end_id,   1, MPI_INT, ROOT, 4, MPI_COMM_WORLD));
        checkMPIError(MPI_Send(&temp[pixel_start_id], pixel_end_id - pixel_start_id + getWidth(), MPI_FLOAT, ROOT, 5, MPI_COMM_WORLD));
    }

    return temp;
}


//-----------------------------------------------------
void MPIImage::workload(unsigned int aNumberOfElements,
                        unsigned int& aStartID,
                        unsigned int& aEndID) const
//-----------------------------------------------------
{
    int world_size;
    int rank;

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

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


//-----------------------------------------------
void MPIImage::checkMPIError(int errorCode) const
//-----------------------------------------------
{
    if (errorCode == MPI_ERR_COMM)
    {
        throw "Invalid communicator. A common error is to use a null communicator in a call (not even allowed in MPI_Comm_rank).";
    }
    else if (errorCode == MPI_ERR_TYPE)
    {
        throw "Invalid datatype argument. Additionally, this error can occur if an uncommitted MPI_Datatype (see MPI_Type_commit) is used in a communication call.";
    }
    else if (errorCode == MPI_ERR_COUNT)
    {
        throw "Invalid count argument. Count arguments must be non-negative; a count of zero is often valid.";
    }
    else if (errorCode == MPI_ERR_TAG)
    {
        throw "Invalid tag argument. Tags must be non-negative; tags in a receive (MPI_Recv, MPI_Irecv, MPI_Sendrecv, etc.) may also be MPI_ANY_TAG. The largest tag value is available through the the attribute MPI_TAG_UB.";
    }
    else if (errorCode == MPI_ERR_RANK)
    {
        throw "Invalid source or destination rank. Ranks must be between zero and the size of the communicator minus one; ranks in a receive (MPI_Recv, MPI_Irecv, MPI_Sendrecv, etc.) may also be MPI_ANY_SOURCE. ";
    }
}
