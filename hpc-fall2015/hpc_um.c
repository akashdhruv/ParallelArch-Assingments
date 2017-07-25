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

	int N=10000;
	int hist_size=256;

	//int **global_image=allocate_2d_matrix(N,N); // Allocating global_image as 2D Array
	int *global_image=NULL; // Allocating global_image to use it as 1D Array
    	int *local_image=malloc(((N*N)/size)*sizeof(int));
	int *local_histogram=calloc(hist_size,sizeof(int));
	int *histogram=calloc(hist_size,sizeof(int));
	int counter=0;

    	if (rank == 0)
	{

		// Using 1D Array
		global_image=malloc(N*N*sizeof(int));
		for(i=0; i<(N*N);  i++)
			 global_image[i]=rand() % hist_size;


		// Using 2D Array
		/*for(i=0; i<N; i++)
			for(j=0; j<N; j++)
				global_image[i][j]=rand() % hist_size;*/
 	}

    	MPI_Scatter(global_image, (N*N)/size, MPI_INT, local_image, (N*N)/size, MPI_INT, 0, MPI_COMM_WORLD); // Using 1D Array
	//MPI_Scatter(*global_image, (N*N)/size, MPI_INT, local_image, (N*N)/size, MPI_INT, 0, MPI_COMM_WORLD); // Using 2D Array

	MPI_Barrier(MPI_COMM_WORLD);

	for(i=0; i<(N*N)/size; i++)
	{

		local_histogram[local_image[i]]++;

	}

    	MPI_Reduce(local_histogram, histogram, hist_size, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);

	/*if (rank == 0)
	{
		printf("\nImage Histogram\n");
        	for(i=0; i<hist_size; i++)
        	{
			printf("\n%d",histogram[i]);

        	}

    	}*/

        free(global_image);
	free(local_image);
	free(local_histogram);
	free(histogram);

	double end=get_wall_time();

	printf("\nElapsed time: %f\n",(end-start));

	MPI_Finalize();
	return 0;
}
