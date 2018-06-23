#include <stdio.h>
#include <omp.h>
#include <windows.h>
#define NSTEPS 15

int main(){
	int i, j, k, nthreads;
	int a[NSTEPS][NSTEPS];
    int b[NSTEPS][NSTEPS];
    int c[NSTEPS][NSTEPS];
	for(i = 0; i < NSTEPS; i++){
		for(j = 0; j < NSTEPS; j++){
			a[i][j] = 1;
			b[i][j] = 1;
		}
	}
	//计时用，精确到ms 
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;  
	LARGE_INTEGER nEndTime;  
	QueryPerformanceFrequency(&nFreq);  
	QueryPerformanceCounter(&nBeginTime);
	
	omp_set_num_threads(NSTEPS*NSTEPS);
	
	#pragma omp parallel shared(a, b, c)
	{
		#pragma omp for private(k) schedule(runtime)
		for(i = 0; i < NSTEPS*NSTEPS; i++){
			c[i/NSTEPS][i%NSTEPS] = 0;
			for(k = 0; k < NSTEPS; k++){
				c[i/NSTEPS][i%NSTEPS] += a[i/NSTEPS][k] * b[k][i%NSTEPS];
			}
		}
	}
	QueryPerformanceCounter(&nEndTime);
	for(i = 0; i < NSTEPS; i++){
		for(j = 0; j < NSTEPS; j++){
			printf("%d ", c[i][j]);
		}
		printf("\n");
	}
	printf("time = %lfms\n", (double)(nEndTime.QuadPart-nBeginTime.QuadPart)/(double)nFreq.QuadPart*1000);
} 
