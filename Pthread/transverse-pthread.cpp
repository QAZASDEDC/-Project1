#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <windows.h>
#define N 15 

int A[N][N];
int T_A[N][N];

struct v
{
	int i;
	int j;
};

void* runner(void* param);

int main(){
	int i, j;
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			A[i][j] = i * N + j;
		}
	}
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;  
	LARGE_INTEGER nEndTime;  
	QueryPerformanceFrequency(&nFreq);  
	QueryPerformanceCounter(&nBeginTime);
	
	pthread_t tid[N*N];
	pthread_attr_t attr;

	pthread_attr_init(&attr);

	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			struct v *data = (struct v *)malloc(sizeof(struct v));
			data->i = i;
			data->j = j;
			T_A[i][j] = 0;
			pthread_create(&tid[i*N + j], &attr, runner, (void*)data);
		}
	}
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			pthread_join(tid[i*N + j], NULL);
		}
	}
	QueryPerformanceCounter(&nEndTime);
	printf("以下为矩阵转置结果：\n");
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			printf("%d ", T_A[i][j]);
		}
		printf("\n");
	}
	printf("time = %lfms\n", (double)(nEndTime.QuadPart-nBeginTime.QuadPart)/(double)nFreq.QuadPart*1000);
	return 0;
}

void *runner(void *param){
	int k;
	struct v *p = (struct v*)param;
	T_A[p->i][p->j] += A[p->j][p->i];
	pthread_exit(0);
}
