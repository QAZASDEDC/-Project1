// multiple-mpi.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "mpi.h"  
#include <stdio.h>
#include <cmath>
#include <Windows.h>

int main(int argc, char* argv[])
{
	int i;
	int npes, dims[2], periods[2];
	int myrank, my2drank, mycoords[2];
	int uprank, downrank, leftrank, rightrank, coords[2];
	int shiftsource, shiftdest, shiftsource2, shiftdest2;
	MPI_Status status;
	MPI_Comm comm_2d;
	int aij[2], bij[2], cij;
	int temp;
	MPI_Request reqs[4];

	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBeginTime);

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	
	

	dims[0] = dims[1] = sqrt(npes);

	periods[0] = periods[1] = 1;

	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &comm_2d);

	MPI_Comm_rank(comm_2d, &my2drank);
	MPI_Cart_coords(comm_2d, my2drank, 2, mycoords);

	aij[0] = aij[1] = 1;
	bij[0] = bij[1] = 1;
	cij = 0;

	MPI_Cart_shift(comm_2d, 1, -1, &rightrank, &leftrank);
	MPI_Cart_shift(comm_2d, 0, -1, &downrank, &uprank);
	
	/*MPI_Cart_shift(comm_2d, 0, -mycoords[0], &shiftsource, &shiftdest);
	MPI_Sendrecv_replace(&aij[0], 1, MPI_INT, shiftdest, 1, shiftsource, 1, comm_2d, &status);
	
	MPI_Cart_shift(comm_2d, 1, -mycoords[1], &shiftsource, &shiftdest);
	MPI_Sendrecv_replace(&bij[0], 1, MPI_INT, shiftdest, 1, shiftsource, 1, comm_2d, &status);*/
	
	MPI_Cart_shift(comm_2d, 1, -mycoords[0], &shiftsource, &shiftdest);
	MPI_Cart_shift(comm_2d, 0, -mycoords[1], &shiftsource2, &shiftdest2);

	MPI_Isend(&aij[1], 1, MPI_INT, shiftdest, 1, comm_2d, &reqs[0]);
	MPI_Isend(&bij[1], 1, MPI_INT, shiftdest2, 1, comm_2d, &reqs[1]);
	MPI_Irecv(&aij[0], 1, MPI_INT, shiftsource, 1, comm_2d, &reqs[2]);
	MPI_Irecv(&bij[0], 1, MPI_INT, shiftsource2, 1, comm_2d, &reqs[3]);

	for (int j = 0; j < 4; j++) {
		MPI_Wait(&reqs[j], &status);
	}

	for (i = 0; i < dims[0]; i++) {
		cij += aij[i % 2] * bij[i % 2];
		//if(myrank == 1) fprintf(stderr, "%d %d %d\n", myrank, aij, bij);
		MPI_Isend(&aij[i % 2], 1, MPI_INT, leftrank, 1, comm_2d, &reqs[0]);
		MPI_Isend(&bij[i % 2], 1, MPI_INT, uprank, 1, comm_2d, &reqs[1]);
		MPI_Irecv(&aij[(i + 1) % 2], 1, MPI_INT, rightrank, 1, comm_2d, &reqs[2]);
		MPI_Irecv(&bij[(i + 1) % 2], 1, MPI_INT, downrank, 1, comm_2d, &reqs[3]);
		//MPI_Sendrecv_replace(&aij, 1, MPI_INT, leftrank, 1, rightrank, 1, comm_2d, &status);
		//MPI_Sendrecv_replace(&bij, 1, MPI_INT, uprank, 1, downrank, 1, comm_2d, &status);
		
		for (int j = 0; j < 4; j++) {
			MPI_Wait(&reqs[j], &status);
		}
	}

	/*MPI_Cart_shift(comm_2d, 0, +mycoords[0], &shiftsource, &shiftdest);
	MPI_Sendrecv_replace(&aij[i%2], 1, MPI_INT, shiftdest, 1, shiftsource, 1, comm_2d, &status);

	MPI_Cart_shift(comm_2d, 1, +mycoords[1], &shiftsource, &shiftdest);
	MPI_Sendrecv_replace(&bij[i%2], 1, MPI_INT, shiftdest, 1, shiftsource, 1, comm_2d, &status);*/
	
	
	if (my2drank == 0 && my2drank + 1<npes) {
		QueryPerformanceCounter(&nEndTime);
		printf("time = %lfms\n", (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart * 1000);
		fprintf(stderr, "c(%d,%d):%d\n", my2drank/dims[0], my2drank % dims[0], cij);
		MPI_Send(&temp, 1, MPI_INT, my2drank + 1, 3, MPI_COMM_WORLD);
	}
	else {
		MPI_Recv(&temp, 1, MPI_INT, my2drank - 1, 3, MPI_COMM_WORLD, &status);
		fprintf(stderr, "c(%d,%d):%d\n", my2drank/dims[0], my2drank % dims[0], cij);
		if (my2drank + 1 < npes) {
			MPI_Send(&temp, 1, MPI_INT, my2drank + 1, 3, MPI_COMM_WORLD);
		}
	}
	MPI_Comm_free(&comm_2d);
	MPI_Finalize();

	//QueryPerformanceCounter(&nEndTime);
	//printf("time = %lfms\n", (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart * 1000);
	return 0;
}
