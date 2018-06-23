#include <stdio.h>
#include <omp.h>
#include <windows.h>
#define NSTEPS 1000

int main(){
	int i, nthreads = NSTEPS;
	double x, pi;
	double sum = 0.0;
	double step = 1.0/(double)NSTEPS;
	//计时用，精确到ms 
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;  
	LARGE_INTEGER nEndTime;  
	QueryPerformanceFrequency(&nFreq);  
	QueryPerformanceCounter(&nBeginTime);
	
	omp_set_num_threads(nthreads);  
	
	#pragma omp parallel
	{
		#pragma omp for private(x) reduction(+:sum) schedule(runtime)
		for(i = 0; i < NSTEPS; i++){
			x = (i+0.5) * step;
			sum += 4.0/(1.0 + x * x);
		}
		#pragma omp master
		{
			pi = step * sum;
		}
	}
	QueryPerformanceCounter(&nEndTime);
	printf("PI = %f\n", pi);
	printf("time = %lfms\n", (double)(nEndTime.QuadPart-nBeginTime.QuadPart)/(double)nFreq.QuadPart*1000);
} 
