#include <iostream>
#include <omp.h>


using namespace std;


void Hello()
{
	cout << "Hello, I am a thread " << endl;
}

int main()
{
#pragma omp parallel
	Hello();
  
  	return 0;
}
