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

	int N=2000;
	int hist_size=256;

	int *global_image=NULL; 
    	int *local_image=malloc(((N*N)/size)*sizeof(int));
	int *local_histogram=calloc(hist_size,sizeof(int));
	int *histogram=calloc(hist_size,sizeof(int));
	
    	if (rank == 0)
	{
		global_image=malloc(N*N*sizeof(int));

		for(i=0; i<(N*N);  i=i++)
			 global_image[i]=rand() % hist_size;
 	}

	MPI_Barrier(MPI_COMM_WORLD);

    	MPI_Scatter(global_image, (N*N)/size, MPI_INT, local_image, (N*N)/size, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);

	for(i=0; i<(N*N/size); i=i++)
	{

		local_histogram[local_image[i]]++;

	}

    	MPI_Reduce(local_histogram, histogram, hist_size, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);

/*	if (rank == 0)
        	for(i=0; i<hist_size; i++)
			printf("%d. %d\n",i+1,histogram[i]);
*/	

        free(global_image);
	free(local_image);
	free(local_histogram);
	free(histogram);

	double end=get_wall_time();

	printf("\nElapsed time: %f\n",(end-start));

	MPI_Finalize();
	return 0;
}
