#include <iostream>
#include <fstream>

#include <mpi.h>

using namespace std;

const int LEADER = 0;


void checkMPIError(int errorCode);


int main(int argc, char** argv)
{
    // Initialsation
    MPI_Init(&argc, &argv);

    try
    {
        // Get the world size
        int world_size = 0;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        // Get the process' rank
        int process_rank = 0;
        MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

        // Create an image
        int image_width  = 512;
        int image_height = world_size;
        float* p_image = new float [image_width * image_height];

        // Get the start of the line corresponding to the process rank
        float* p_line_start = p_image + image_width * process_rank;

        // Set the pixel values
        for (int i = 0; i < image_width; ++i)
        {
            *p_line_start++ = process_rank;
        }

        // Leader gather results from all the processes
        int flag = 0; // Could be anything
        if (process_rank == LEADER)
        {
            // i = 1 as the LEADER already got its own data
            for (int i = 1; i < world_size; ++i)
            {
                MPI_Status status;
                float* temp = p_image + image_width * i;
                int error_code = MPI_Recv(temp, image_width, MPI_FLOAT, i, flag, MPI_COMM_WORLD, &status);

                checkMPIError(error_code);
            }
        }
        // Other processes send the data to the leader
        else
        {
            float* temp = p_image + image_width * process_rank;
            int error_code = MPI_Send(temp, image_width, MPI_FLOAT, LEADER, flag, MPI_COMM_WORLD);

            checkMPIError(error_code);
        }

        // Only the leader print the image
        if (process_rank == LEADER)
        {
            for (int j = 0; j < image_height; ++j)
            {
                for (int i = 0; i < image_width; ++i)
                {
                    cout << p_image[j * image_width + i] << " ";
                }
                cout << endl;
            }
        }

    }
    catch (const char* error_message)
    {
        cerr << "Exception: " << error_message << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception" << endl;
    }

    // Finalisation
    MPI_Finalize();

    return 0;
}


//-------------------------------
void checkMPIError(int errorCode)
//-------------------------------
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
