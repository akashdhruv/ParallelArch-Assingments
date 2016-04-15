#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <upc.h>

#define N 1000
#define hist_size 256

shared int rank,size;
shared [hist_size] int* shared histogram;
shared int* shared local_histogram;

double get_wall_time()
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return (double)time.tv_sec + (double)time.tv_usec/1000000;
}

int main(int argc, char *argv[])
{

	int i, j;
	int local_image[N][N];

	if(MYTHREAD==0){

		MPI_Init(&argc, &argv);
		MPI_Comm_size(MPI_COMM_WORLD, &size);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	}

	upc_barrier;

	double start=get_wall_time();

	histogram=upc_all_alloc(hist_size, hist_size*sizeof(int));
	local_histogram=upc_all_alloc(1,hist_size*sizeof(int));

	for(i=rank; i<N; i=i+size)
		for(j=MYTHREAD; j<N; j=j+THREADS)
			local_image[i][j]=rand() % hist_size;

	upc_barrier;
	

	for(i=rank; i<N; i=i+size)
		for(j=MYTHREAD; j<N; j=j+THREADS)
			local_histogram[local_image[i][j]]++;

	upc_barrier;

	if(MYTHREAD==0){
		MPI_Reduce(local_histogram, histogram, hist_size, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

		//MPI_Barrier(MPI_COMM_WORLD);
	}
	upc_barrier;

	/*if(rank==0 && MYTHREAD==0)
		for(j=0; j<hist_size; j++)
			printf("%d. %d\n",j+1,histogram[j]);*/
  
	upc_free(histogram);
	upc_free(local_histogram);

	double end=get_wall_time();

	printf("\nElapsed time: %f\n",(end-start));

	if(MYTHREAD==0)
		MPI_Finalize();
	return 0;
}

