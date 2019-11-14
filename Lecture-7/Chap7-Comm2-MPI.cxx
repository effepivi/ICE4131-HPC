#include <iostream>
#include <fstream>

#include <mpi.h>

using namespace std;

const int LEADER = 0;

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

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
        *p_line_start++ = 0.5;
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

    MPI_Finalize();

    return 0;
}
