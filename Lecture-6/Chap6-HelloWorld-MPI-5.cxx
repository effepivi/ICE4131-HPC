#include <iostream>
#include <cstdlib>

#include <mpi.h>

using namespace std;

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    /*int N = 10;

    if (argc == 2)
    {
        N = atoi(argv[1]);
    }*/

    int N = 0;
    int i = 0;
    //for (int i = 0; i < N; ++i)
    {
        cout << "Hello " << i + 1 << " of " << N << endl;
    }

    MPI_Finalize();

    return 0;
}
