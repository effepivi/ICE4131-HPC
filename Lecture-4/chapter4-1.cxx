#include <iostream> // To use cout
#include <random>   // To use pseudo-random numbers
#include <vector>    // To use STL vectors


using namespace std;


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << "\t" << "N   [with N the number of elements in the arrays]" << endl;
        return EXIT_FAILURE;
    }

    int N = atoi(argv[1]);

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_real_distribution<double> uniform_distribution;

    cout << "Run allocation" << endl;
    vector<double> a(N);
    vector<double> b(N);
    vector<double> c(N);
    vector<double> d(N);

    cout << "Run initialisation" << endl;
    for (int i = 0; i < N; i++)
    {
        a[i] = uniform_distribution(generator);
        b[i] = uniform_distribution(generator);
    }

    cout << "Run sums" << endl;
    for (int i = 0; i < N; i++)
        c[i] = a[i] + b[i];

    cout << "Run products" << endl;
    for (int i = 0; i < N; i++)
        d[i] = a[i] * b[i];

    return 0;
}
