#include <iostream> // To use cout
#include <random>   // To use pseudo-random numbers
#include <vector>   // To use STL vectors
#include <chrono>   // To measure durations

using namespace std;


struct ThreadData
{
    pthread_t thread_id;
    unsigned int thread_int_id;
    unsigned int start_id;
    unsigned int end_id;
};


void* initialisation_callback(void* aThreadData);


int main(int argc, char** argv)
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << "\t" << "N  T [with N the number of elements in the arrays, and T the number of threads]" << endl;
        return EXIT_FAILURE;
    }

    int N = atoi(argv[1]);
    int T = atoi(argv[2]);

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_real_distribution<double> uniform_distribution;

    cout << "Run allocation" << endl;
    auto start = chrono::high_resolution_clock::now();
    vector<double> a(N);
    vector<double> b(N);
    vector<double> c(N);
    vector<double> d(N);
    auto end = chrono::high_resolution_clock::now();
    double time_taken = chrono::duration<double>(end - start).count();
    cout << "Allocation took " << time_taken << " seconds" << endl << endl;

    cout << "Run initialisation" << endl;

    // Workload allocation
    vector<ThreadData> p_thread_data(T);
    int last_element = -1;
    unsigned int cells_per_thread = N / T;
    unsigned int remainder = N % T;

    cout << "Default number of cells per thread: " << cells_per_thread << endl;
    for (int i = 0; i < T; ++i)
    {
        p_thread_data[i].thread_int_id = i;
        p_thread_data[i].start_id = ++last_element;
        p_thread_data[i].end_id = last_element + cells_per_thread - 1;

        if (remainder > 0)
        {
            p_thread_data[i].end_id++;
            --remainder;
        }
        last_element = p_thread_data[i].end_id;

        cout << "Thread[" << i << "] starts with " << p_thread_data[i].start_id << " and stop on " << p_thread_data[i].end_id<< endl;
    }

    // Create threads
    for (int i = 0; i < T; ++i)
    {
        pthread_create(&p_thread_data[i].thread_id,
            NULL,
            initialisation_callback,
            &p_thread_data[i]);
    }

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < T; i++)
    {
        a[i] = uniform_distribution(generator);
        b[i] = uniform_distribution(generator);
    }
    end = chrono::high_resolution_clock::now();
    time_taken = chrono::duration<double>(end - start).count();
    cout << "Initialisation took " << time_taken << " seconds" << endl << endl;

    cout << "Run sums" << endl;
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++)
    c[i] = a[i] + b[i];
    end = chrono::high_resolution_clock::now();
    time_taken = chrono::duration<double>(end - start).count();
    cout << "Sums took " << time_taken << " seconds" << endl << endl;

    cout << "Run products" << endl;
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++)
        d[i] = a[i] * b[i];
    end = chrono::high_resolution_clock::now();
    time_taken = chrono::duration<double>(end - start).count();
    cout << "Products took " << time_taken << " seconds" << endl << endl;

    // Print the result in the terminal
    for (int i = 0; i < N; i++)
    {
        cout << c[i] << " = " << a[i] << " + " << b[i] << endl;
        cout << d[i] << " = " << a[i] << " * " << b[i] << endl << endl;
    }

    return 0;
}


void* initialisation_callback(void* aThreadData)
{

    return 0;
}
