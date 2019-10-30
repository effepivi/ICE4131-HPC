#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv)
{
    int N = 10;

    if (argc == 2)
    {
        N = atoi(argv[1]);
    }

    for (int i = 0; i < N; ++i)
    {
        cout << "Hello " << i + 1 << " of " << N << endl;
    }

    return 0;
}
