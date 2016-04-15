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

int **allocate_2d_matrix(int m, int n)
{
	int *linear,**mat;
	int i;

	linear=malloc(sizeof(int)*m*n);
	mat=malloc(sizeof(int *)*m );

	for(i=0; i<m; i++)
	{
		mat[i]=&linear[i*n];
	}

	return mat;

}

int main(int argc, char *argv[])
{
	int size, rank;
	int i, j;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double start=get_wall_time();

	int N=5;
	int hist_size=25;

	int **global_image=allocate_2d_matrix(N,N);
	int *local_histogram=calloc(hist_size,sizeof(int));
	int *histogram=calloc(hist_size,sizeof(int));

	int counter=0;

	for(i=rank; i<N; i=i+size)
		for(j=0; j<N; j++)
			global_image[i][j]=i*N+j;

	MPI_Barrier(MPI_COMM_WORLD);

	for(i=rank; i<N; i=i+size)
		for(j=0; j<N; j++)
			local_histogram[global_image[i][j]]++;

	MPI_Barrier(MPI_COMM_WORLD);

    	MPI_Reduce(local_histogram, histogram, hist_size, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);

	if(rank==0)
		for(i=0;i<hist_size;i++)
			printf("%d. %d\n",i+1,histogram[i]);

        free(global_image);
	free(local_histogram);
	free(histogram);

	double end=get_wall_time();

	//printf("\nElapsed time: %f\n",(end-start));

	MPI_Finalize();
	return 0;
}

