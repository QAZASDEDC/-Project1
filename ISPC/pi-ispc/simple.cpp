#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define N 1000
// Include the header file that the ispc compiler generates
#include "simple_ispc.h"
using namespace ispc;

int main() {
	//计时用，精确到ms 
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBeginTime);
    // Initialize input buffer
    // Call simple() function from simple.ispc file
    double pi = simple(N);

	QueryPerformanceCounter(&nEndTime);

    // Print results
    printf("pi = %f\n", pi);
	printf("time = %lfms\n", (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart * 1000);
	system("pause"); 
    return 0;
}
