#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <upc.h>

#define N 4*THREADS

shared int rank, size;
shared float temp_c[N];
shared float temp_f[N];
shared float local_tc[N/4];
shared float local_tf[N/4];

double get_wall_time()
{
        struct timeval time;
        gettimeofday(&time, NULL);
        return (double)time.tv_sec + (double)time.tv_usec/1000000;
}

int main(int argc, char *argv[])
{
        int i, j;

	double start=get_wall_time();

	if(MYTHREAD==0) {
        	MPI_Init(&argc, &argv);
        	MPI_Comm_size(MPI_COMM_WORLD, &size);
        	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	}


	upc_barrier;

	if(rank==0) {
		for(i=MYTHREAD; i<N; i=i+THREADS)
			temp_c[i]=i;

		upc_barrier;

		if(MYTHREAD==0){
			for(i=0; i<N; i++)
				printf("%d. Temperature in C: %fC\n",i+1,temp_c[i]);
			//printf("N=%d\n and %f %f %f %f %f",N,temp_c[N], temp_c[N+1], temp_c[64], temp_c[65], temp_c[256]);
		}
	}

	upc_barrier;

	if(MYTHREAD==0){
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Scatter(&temp_c, N/size, MPI_FLOAT, &local_tc, N/size, MPI_FLOAT, 0, MPI_COMM_WORLD);
	}

	upc_barrier;

	if(rank==1)
	for(i=MYTHREAD; i<N/size; i=i+THREADS)
		local_tf[i]=(9.0/5.0)*local_tc[i] + 32.0;

	upc_barrier;

	if(rank==1 && MYTHREAD==0){
		for(i=0; i<N/size; i++)
			printf("%d. Temperature in C: %fC\n",i+1,local_tc[i]);
	}

	upc_barrier;

	if(MYTHREAD==0){
		MPI_Gather(&temp_f, N/size, MPI_FLOAT, &local_tf, N/size, MPI_FLOAT, 0, MPI_COMM_WORLD);
	}

	upc_barrier;

	if(rank==0){
		if(MYTHREAD==0)
			for(i=0; i<N; i++)
				printf("%d. Temperature in C: %fC\n",i+1,temp_f[i]);
	}


	if(MYTHREAD==0)
		MPI_Barrier(MPI_COMM_WORLD);

	upc_barrier;

	if(MYTHREAD==0) {
		MPI_Finalize();
	}

	double end = get_wall_time();
	return 0;

}
