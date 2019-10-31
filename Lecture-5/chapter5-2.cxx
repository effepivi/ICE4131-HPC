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
#pragma omp parallel
	Hello();
  
  	return 0;
}
