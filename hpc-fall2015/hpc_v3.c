#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <upc.h>

#define N 2000
#define hist_size 256

shared int rank,size;
//shared int histogram[hist_size];
//shared int local_histogram[hist_size];

shared [hist_size] int* shared histogram;
shared [hist_size] int* shared local_histogram;
shared [] int *local_image;
shared [N*N] int *global_image;

double get_wall_time()
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return (double)time.tv_sec + (double)time.tv_usec/1000000;
}

int main(int argc, char *argv[])
{

	int i, j;

	if(MYTHREAD==0){

		MPI_Init(&argc, &argv);
		MPI_Comm_size(MPI_COMM_WORLD, &size);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	}

	upc_barrier;

	double start=get_wall_time();
	
	histogram=upc_all_alloc(1, hist_size*sizeof(int));
	local_histogram=upc_all_alloc(1, hist_size*sizeof(int));
	local_image=upc_all_alloc(1, (N*N)/size*sizeof(int));

	if(rank==0)
	{
		global_image=upc_all_alloc(1,N*N*sizeof(int));

		for(i=MYTHREAD; i<(N*N); i=i+THREADS)
			global_image[i]=rand() % hist_size;


		upc_barrier;

	}	

	if(MYTHREAD==0){
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Scatter(global_image, (N*N)/size, MPI_INT, local_image, (N*N)/size, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
	}

	upc_barrier;

	for(i=MYTHREAD; i<N*N/size; i=i+THREADS)
		local_histogram[local_image[i]]++;	

	upc_barrier;

	if(MYTHREAD==0){
		MPI_Reduce(local_histogram, histogram, hist_size, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

		MPI_Barrier(MPI_COMM_WORLD);
	}

	upc_barrier;

/*	if(rank==0 && MYTHREAD==0)
		for(i=0;i<hist_size;i++)
			printf("%d. %d\n",i+1,histogram[i]);
*/

	upc_free(global_image);
	upc_free(local_image);
	upc_free(histogram);
	upc_free(local_histogram);

	double end=get_wall_time();

	printf("Elapsed time: %f\n",(end-start));

	if(MYTHREAD==0)
		MPI_Finalize();
	return 0;
}

