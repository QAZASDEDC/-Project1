#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <windows.h>
#define N 1000

double pi = 0.0;
double step = 1.0/(double)N;
double sum[N];

void* runner(void* param);

int main(){
	int i;
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;  
	LARGE_INTEGER nEndTime;  
	QueryPerformanceFrequency(&nFreq);  
	QueryPerformanceCounter(&nBeginTime);
	
	pthread_t tid[N];
	pthread_attr_t attr;

	pthread_attr_init(&attr);

	for(i = 0; i < N; i++){
		int *data = (int*)malloc(sizeof(int));
		*data = i;
		pthread_create(&tid[i], &attr, runner, (void*)data);
	}
	for(i = 0; i < N; i++){
		pthread_join(tid[i], NULL);
		pi += sum[i] * step;
	}
	QueryPerformanceCounter(&nEndTime);
	printf("PI = %f\n", pi);
	printf("time = %lfms\n", (double)(nEndTime.QuadPart-nBeginTime.QuadPart)/(double)nFreq.QuadPart*1000);
	return 0;
}

void *runner(void *param){
	int k;
	int* p = (int*)param;
	double x = (*p + 0.5) * step;
	sum[*p] = 4.0/(1.0 + x * x);
	pthread_exit(0);
}
