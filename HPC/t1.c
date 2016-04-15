#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>

double get_wall_time()
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return (double)time.tv_sec + (double)time.tv_usec/1000000;
}

int main(int argc, char *argv[])
{
	int size, rank;
	int i, j;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double start=get_wall_time();

	int N=1000;
	int hist_size=256;

	int global_image[N][N];
	int *local_histogram=calloc(hist_size,sizeof(int));
	int *histogram=calloc(hist_size,sizeof(int));

	for(i=rank; i<N; i=i+size)
		for(j=0; j<N; j++)
			global_image[i][j]=rand() % hist_size;

	MPI_Barrier(MPI_COMM_WORLD);

	for(i=rank; i<N; i=i+size)
		for(j=0; j<N; j++)
			local_histogram[global_image[i][j]]++;

    	MPI_Reduce(local_histogram, histogram, hist_size, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	/*if(rank==0)
		for(j=0; j<hist_size; j++)
			printf("%d. %d\n",j+1,histogram[j]);

	for(i=rank; i<N; i=i+size)
		for(j=0; j<N; j++)
			printf("%d\n",global_image[i][j]);

	MPI_Barrier(MPI_COMM_WORLD);*/

	free(local_histogram);
	free(histogram);

	double end=get_wall_time();

	printf("\nElapsed time: %f\n",(end-start));

	MPI_Finalize();
	return 0;
}

