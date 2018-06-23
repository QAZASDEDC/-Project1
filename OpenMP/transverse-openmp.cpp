#include <stdio.h>
#include <omp.h>
#include <windows.h>
#define NSTEPS 15

int main(){
	int i, j, nthreads;
	int a[NSTEPS][NSTEPS];
    int t_a[NSTEPS][NSTEPS];
	for(i = 0; i < NSTEPS; i++){
		for(j = 0; j < NSTEPS; j++){
			a[i][j] = i * NSTEPS + j;
		}
	}
	//计时用，精确到ms 
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;  
	LARGE_INTEGER nEndTime;  
	QueryPerformanceFrequency(&nFreq);  
	QueryPerformanceCounter(&nBeginTime);
	
	omp_set_num_threads(NSTEPS*NSTEPS);
	
	#pragma omp parallel shared(a, t_a)
	{
		#pragma omp for private(i) schedule(runtime)
		for(i = 0; i < NSTEPS*NSTEPS; i++){
			t_a[i/NSTEPS][i%NSTEPS] = a[i%NSTEPS][i/NSTEPS];
		}
	}
	QueryPerformanceCounter(&nEndTime);
	for(i = 0; i < NSTEPS; i++){
		for(j = 0; j < NSTEPS; j++){
			printf("%d ", t_a[i][j]);
		}
		printf("\n");
	}
	printf("time = %lfms\n", (double)(nEndTime.QuadPart-nBeginTime.QuadPart)/(double)nFreq.QuadPart*1000);
} 
