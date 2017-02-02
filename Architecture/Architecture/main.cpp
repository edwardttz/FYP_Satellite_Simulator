//libraries
#include <iostream>
#include <cmath>
#include <string>
#include <thread>
#include <mutex>

//headers
#include <time.h>

using namespace std;

mutex groundTruthMutex;
mutex noiseModelsMutex;
mutex transportProtocolMutex;

void foo()
{
	printf("ground\n");
}

void bar()
{
	printf("noise\n");
}

void tango()
{
	printf("tpt\n");
}

int main(void)
{
	thread groundTruth(foo);
	thread noiseModels(bar);
	thread transportProtocol(tango);

	clock_t tStart = clock();

	printf("Time taken = %.10fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

	system("PAUSE");
}

