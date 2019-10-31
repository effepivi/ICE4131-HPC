#include <iostream>
#include <omp.h>


using namespace std;


void Hello()
{
  	int rank = omp_get_thread_num();
  	int thread_count = omp_get_num_threads();
  
	cout << "Hello, I am thread " << rank << " of " << thread_count << endl;
}

int main()
{
  	cout << "Number of threads?" << endl;
  	int nb_threads;
  	cin >> nb_threads;
  
#pragma omp parallel num_threads(nb_threads)
	Hello();
  
  	return 0;
}
