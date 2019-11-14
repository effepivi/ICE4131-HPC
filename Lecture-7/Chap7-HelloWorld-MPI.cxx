#include <iostream>

#include <mpi.h>

using namespace std;

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    // Get the world size
    int world_size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the process' rank
    int process_rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    cout << "I am Process #" << 1 + process_rank << " of " << world_size << endl;
    cout << "I run on " << processor_name << endl << endl;

    MPI_Finalize();

    return 0;
}
