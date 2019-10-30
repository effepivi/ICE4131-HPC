#include <iostream>
#include <cstdlib>

#include <mpi.h>

using namespace std;

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int N = 0;
    int rank = 0;
    
    MPI_Comm_size(MPI_COMM_WORLD, &N);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    cout << "Hello " << rank + 1 << " of " << N << endl;

    MPI_Finalize();

    return 0;
}
