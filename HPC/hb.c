#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>

#define N 16

double get_wall_time()
{
        struct timeval time;
        gettimeofday(&time, NULL);
        return (double)time.tv_sec + (double)time.tv_usec/1000000;
}

int main(int argc, char *argv[])
{
        int i, j;
	int rank,size;
	float temp_c[N];
	float temp_f[N];
	float local_tc[N/4];
	float local_tf[N/4];
	double start=get_wall_time();

        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(rank==0) {
		for(i=0; i<N; i=i++)
			temp_c[i]=i;

		for(i=0; i<N; i++)
			printf("%d. Temperature in C: %fC\n",i+1,temp_c[i]);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Scatter(&temp_c, N/size, MPI_FLOAT, &local_tc, N/size, MPI_FLOAT, 0, MPI_COMM_WORLD);

	for(i=0; i<N/size; i=i++)
		local_tf[i]=(9.0/5.0)*local_tc[i] + 32.0;

	for(i=0; i<N/size; i++)
		printf("%d. Temperature in C: %fC\n",i+1,local_tf[i]);

	MPI_Gather(&local_tf, N/size, MPI_FLOAT, &temp_f, N/size, MPI_FLOAT, 0, MPI_COMM_WORLD);

	if(rank==0)
		for(i=0; i<N; i++)
			printf("%d. Temperature in C: %fC\n",i+1,temp_f[i]);


	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Finalize();

	double end = get_wall_time();
	return 0;

}
