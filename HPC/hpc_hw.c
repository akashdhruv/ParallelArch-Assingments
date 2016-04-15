#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

double get_wall_time()
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return (double)time.tv_sec + (double)time.tv_usec/1000000;
}

int main(int argc, char *argv[])
{
        int rank, size, n, i;
	double PI25DT = 3.141592653589793238462643;
	double mypi, pi, h, sum, x;

	//clock_t start=clock();
	double st=get_wall_time();

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//MPI_Comm comm;

	int counter=1;
	n=100000000;

	while(counter==1)
	{
		//if(rank==0)
		//{
		//	printf("Enter the number of intervals: (0 quits)");
		//	scanf("%d",n);
		//}

		MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

		if(n==0)

		{
			break;
		}

		else

		{

			h=1.0/(double)n;
			sum=0.0;

			for (i = rank + 1; i <= n ; i+= size)
			{
				x = h * ((double) i - 0.5); sum += (4.0 / (1.0 + x*x));
			}

			mypi = h * sum;

			//MPI_Barrier(comm);

			MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

			if (rank == 0)
			{
				printf("pi is approximately %.16f, Error is %.16f\n",pi, fabs(pi-PI25DT));
			}


		}


	counter=counter+1;

	}

	//clock_t end=clock();
	double en=get_wall_time();

	//printf("Elapsed time: %f\n",(end-start)/(double)CLOCKS_PER_SEC);
	printf("Elapsed time: %f\n",(en-st));

        MPI_Finalize();
	return 0;
}

