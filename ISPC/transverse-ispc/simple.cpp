#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define N 15
// Include the header file that the ispc compiler generates
#include "simple_ispc.h"
using namespace ispc;

int main() {
	int vin[N*N];
	int vout[N*N];
	for (int i = 0; i < N*N; ++i)
		vin[i] = i;
	//计时用，精确到ms 
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBeginTime);
    // Initialize input buffer
    // Call simple() function from simple.ispc file
    simple(vin, vout, N);

	QueryPerformanceCounter(&nEndTime);

    // Print results
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf("%d ", vout[i*N + j]);
		}
		printf("\n");
	}
	printf("time = %lfms\n", (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart * 1000);
	system("pause"); 
    return 0;
}
